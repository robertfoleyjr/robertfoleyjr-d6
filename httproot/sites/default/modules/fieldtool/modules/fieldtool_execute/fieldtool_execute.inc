<?php
/**
 * @file
 * Field Tool Execute
 *
 * Include file for fieldtool_execute.module.
 * The sole purpose of this file is to work around the bugs described at:
 * https://drupal.org/node/260934 and http://drupal.org/node/846890
 *
 * Copyright 2010 Matthew Davidson (http://drupal.org/user/52996).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

function fieldtool_execute_execute($form_id, &$form_state) {
  $args = func_get_args();

  // Make sure $form_state is passed around by reference.
  $args[1] = &$form_state;

  $form = call_user_func_array('drupal_retrieve_form', $args);
  $form['#post'] = $form_state['values'];
  drupal_prepare_form($form_id, $form, $form_state);
  fieldtool_execute_process_form($form_id, $form, $form_state);
}

function fieldtool_execute_process_form($form_id, &$form, &$form_state) {
  $form_state['values'] = array();

  $form = fieldtool_execute_form_builder($form_id, $form, $form_state);
  // Only process the form if it is programmed or the form_id coming
  // from the POST data is set and matches the current form_id.
  if ((!empty($form['#programmed'])) || (!empty($form['#post']) && (isset($form['#post']['form_id']) && ($form['#post']['form_id'] == $form_id)))) {
    fieldtool_execute_validate_form($form_id, $form, $form_state);

    // form_clean_id() maintains a cache of element IDs it has seen,
    // so it can prevent duplicates. We want to be sure we reset that
    // cache when a form is processed, so scenerios that result in
    // the form being built behind the scenes and again for the
    // browser don't increment all the element IDs needlessly.
    form_clean_id(NULL, TRUE);

    if ((!empty($form_state['submitted'])) && !form_get_errors() && empty($form_state['rebuild'])) {
      $form_state['redirect'] = NULL;
      form_execute_handlers('submit', $form, $form_state);

      // We'll clear out the cached copies of the form and its stored data
      // here, as we've finished with them. The in-memory copies are still
      // here, though.
      if (variable_get('cache', CACHE_DISABLED) == CACHE_DISABLED && !empty($form_state['values']['form_build_id'])) {
        cache_clear_all('form_'. $form_state['values']['form_build_id'], 'cache_form');
        cache_clear_all('storage_'. $form_state['values']['form_build_id'], 'cache_form');
      }

      // If batches were set in the submit handlers, we process them now,
      // possibly ending execution. We make sure we do not react to the batch
      // that is already being processed (if a batch operation performs a
      // drupal_execute).
      if ($batch =& batch_get() && !isset($batch['current_set'])) {
        // The batch uses its own copies of $form and $form_state for
        // late execution of submit handers and post-batch redirection.
        $batch['form'] = $form;
        $batch['form_state'] = $form_state;
        $batch['progressive'] = !$form['#programmed'];
        batch_process();
        // Execution continues only for programmatic forms.
        // For 'regular' forms, we get redirected to the batch processing
        // page. Form redirection will be handled in _batch_finished(),
        // after the batch is processed.
      }

      // If no submit handlers have populated the $form_state['storage']
      // bundle, and the $form_state['rebuild'] flag has not been set,
      // we're finished and should redirect to a new destination page
      // if one has been set (and a fresh, unpopulated copy of the form
      // if one hasn't). If the form was called by drupal_execute(),
      // however, we'll skip this and let the calling function examine
      // the resulting $form_state bundle itself.
      if (!$form['#programmed'] && empty($form_state['rebuild']) && empty($form_state['storage'])) {
        drupal_redirect_form($form, $form_state['redirect']);
      }
    }
  }
}


function fieldtool_execute_validate_form($form_id, $form, &$form_state) {
  // If the session token was set by drupal_prepare_form(), ensure that it
  // matches the current user's session.
  if (isset($form['#token'])) {
    if (!drupal_valid_token($form_state['values']['form_token'], $form['#token'])) {
      // Setting this error will cause the form to fail validation.
      form_set_error('form_token', t('Validation error, please try again. If this error persists, please contact the site administrator.'));
    }
  }

  _form_validate($form, $form_state, $form_id);
  $validated_forms[$form_id] = TRUE;
}


function fieldtool_execute_form_builder($form_id, $form, &$form_state) {
  static $complete_form, $cache;

  // Initialize as unprocessed.
  $form['#processed'] = FALSE;

  // Use element defaults.
  if ((!empty($form['#type'])) && ($info = _element_info($form['#type']))) {
    // Overlay $info onto $form, retaining preexisting keys in $form.
    $form += $info;
  }

  if (isset($form['#type']) && $form['#type'] == 'form') {
    $cache = NULL;
    $complete_form = $form;
    if (!empty($form['#programmed'])) {
      $form_state['submitted'] = TRUE;
    }
  }

  if (isset($form['#input']) && $form['#input']) {
    _form_builder_handle_input_element($form_id, $form, $form_state, $complete_form);
  }
  $form['#defaults_loaded'] = TRUE;

  // We start off assuming all form elements are in the correct order.
  $form['#sorted'] = TRUE;

  // Recurse through all child elements.
  $count = 0;
  foreach (element_children($form) as $key) {
    // Highly inelegant hack to solve http://drupal.org/node/846890
    // If submitting a node form, don't touch submit buttons other than the save button.
    if (array_key_exists('nid', $form_state['values']) && ($form_state['values']['op'] == 'Save') && ($form[$key]['#type'] == 'submit') && ($form[$key]['#value'] != 'Save')) {
      continue;
    }

    $form_state['storage'] = $key;
    $form[$key]['#post'] = $form['#post'];
    $form[$key]['#programmed'] = $form['#programmed'];
    // Don't squash an existing tree value.
    if (!isset($form[$key]['#tree'])) {
      $form[$key]['#tree'] = $form['#tree'];
    }

    // Deny access to child elements if parent is denied.
    if (isset($form['#access']) && !$form['#access']) {
      $form[$key]['#access'] = FALSE;
    }

    // Don't squash existing parents value.
    if (!isset($form[$key]['#parents'])) {
      // Check to see if a tree of child elements is present. If so,
      // continue down the tree if required.
      $form[$key]['#parents'] = $form[$key]['#tree'] && $form['#tree'] ? array_merge($form['#parents'], array($key)) : array($key);
      $array_parents = isset($form['#array_parents']) ? $form['#array_parents'] : array();
      $array_parents[] = $key;
      $form[$key]['#array_parents'] = $array_parents;
    }

    // Assign a decimal placeholder weight to preserve original array order.
    if (!isset($form[$key]['#weight'])) {
      $form[$key]['#weight'] = $count/1000;
    }
    else {
      // If one of the child elements has a weight then we will need to sort
      // later.
      unset($form['#sorted']);
    }
    $form[$key] = fieldtool_execute_form_builder($form_id, $form[$key], $form_state);
    $count++;
  }

  // The #after_build flag allows any piece of a form to be altered
  // after normal input parsing has been completed.
  if (isset($form['#after_build']) && !isset($form['#after_build_done'])) {
    foreach ($form['#after_build'] as $function) {
      $form = $function($form, $form_state);
      $form['#after_build_done'] = TRUE;
    }
  }

  // Now that we've processed everything, we can go back to handle the funky
  // Internet Explorer button-click scenario.
  _form_builder_ie_cleanup($form, $form_state);

  // We shoud keep the buttons array until the IE clean up function
  // has recognized the submit button so the form has been marked
  // as submitted. If we already know which button was submitted,
  // we don't need the array.
  if (!empty($form_state['submitted'])) {
    unset($form_state['buttons']);
  }

  // If some callback set #cache, we need to flip a static flag so later it
  // can be found.
  if (!empty($form['#cache'])) {
    $cache = $form['#cache'];
  }
  // We are on the top form, we can copy back #cache if it's set.
  if (isset($form['#type']) && $form['#type'] == 'form' && isset($cache)) {
    $form['#cache'] = TRUE;
  }
  return $form;
}

