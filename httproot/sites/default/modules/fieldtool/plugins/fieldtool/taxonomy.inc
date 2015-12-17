<?php
/**
 * @file
 * Field Tool - Taxonomy plugin
 * Taxonomy field handler for Field Tool module.
 *
 * Copyright 2009 Matthew Davidson.
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

/**
 * Implementation of [module]_[plugin]_fieldtool_fields().
 */
function fieldtool_taxonomy_fieldtool_fields() {
  if (!module_exists('taxonomy')) {
    return array();
  }
  
  $node_fields = array();

  $node_fields['taxonomy']['label'] = t('Taxonomy');
  $node_fields['taxonomy']['stored'] = TRUE;
  $node_fields['taxonomy']['getter callback'] = 'fieldtool_field_taxonomy_get';
  $node_fields['taxonomy']['setter callback'] = 'fieldtool_field_taxonomy_set';
  $node_fields['taxonomy']['export callback'] = 'fieldtool_field_taxonomy_export';
  $node_fields['taxonomy']['import callback'] = 'fieldtool_field_taxonomy_import';
  $node_fields['taxonomy']['fields'] = array();

  $node_types = array();

  foreach (taxonomy_get_vocabularies() as $vid => $vocabulary) {
    $node_fields['taxonomy-'. $vid]['label'] = $vocabulary->name . ' (taxonomy-'. $vid .')';
    $node_fields['taxonomy-'. $vid]['stored'] = TRUE;
    $node_fields['taxonomy-'. $vid]['virtual'] = TRUE;
    $node_fields['taxonomy-'. $vid]['taxonomy vid'] = $vid;
    $node_fields['taxonomy-'. $vid]['weight'] = $vocabulary->weight;
    $node_fields['taxonomy-'. $vid]['getter callback'] = 'fieldtool_field_taxonomy_get';
    $node_fields['taxonomy-'. $vid]['setter callback'] = 'fieldtool_field_taxonomy_set';
    $node_fields['taxonomy-'. $vid]['export callback'] = 'fieldtool_field_taxonomy_export';
    $node_fields['taxonomy-'. $vid]['import callback'] = 'fieldtool_field_taxonomy_import';
    $node_fields['taxonomy']['fields'][] = 'taxonomy-'. $vid;
    
    foreach ($vocabulary->nodes as $node_type) {
      $node_fields['taxonomy']['node types'][$node_type] = $node_type;
      $node_fields['taxonomy-'. $vid]['node types'][] = $node_type;
    }
  }
  
  $node_fields['taxonomy']['node types'] = array_keys($node_fields['taxonomy']['node types']);

  return array('node' => $node_fields);
}

/**
 * Getter callback.
 *
 * @return
 *   An array like array($tid => $tid).
 */
function fieldtool_field_taxonomy_get($node, $field_name = 'taxonomy', $field_info) {
  if (!is_array($node->taxonomy)) {
    return array();
  }
  
  $values = array();
  
  foreach (array_keys($node->taxonomy) as $tid) {
    if ($field_info['taxonomy vid']) { // If this is a vocabulary field
      $term = taxonomy_get_term($tid);
      if ($term->vid == $field_info['taxonomy vid']) {
        $values[$tid] = $tid;
      }
    }
    else {
      $values[$tid] = $tid;
    }
  }
  return $values;
}

/**
 * Setter callback.
 *
 * @param $value
 *   If you want to delete a value the array key must be the tid, and the 
 *   value evaluate to FALSE in boolean context. Otherwise we just ignore the
 *   key and use the value as the tid. To delete all values, pass FALSE rather
 *   than an array.
 */

function fieldtool_field_taxonomy_set(&$node, $field_name = 'taxonomy', $value, $field_info) {
  if ($value === FALSE) { // We want to delete all terms
    if ($field_info['taxonomy vid']) {
      foreach ($node->taxonomy as $tid => $term) {
        if ($term->vid == $field_info['taxonomy vid']) {
          unset($node->taxonomy[$tid]);
        }
      }
    }
    else {
      $node->taxonomy = array();
    }
    return;
  }
  
  foreach ($value as $key => $tid) {
    if ($field_info['taxonomy vid']) { 
      // If this is a vocabulary field we do have to do some validation.
      $term = $tid ? taxonomy_get_term($tid) : taxonomy_get_term($key); // In case we're deleting a term
      if ($term->vid != $field_info['taxonomy vid']) {
        continue;
      }
    }
    
    if (!$tid && array_key_exists($key, $node->taxonomy)) {
      unset($node->taxonomy[$key]);
    }
    elseif (is_numeric($tid)) {
      $node->taxonomy[$tid] = taxonomy_get_term($tid);
    }
  }
}

/**
 * Taxonomy value itemization callback.
 *
 * Can accept practically anything, as this is used for imported values.
 *
 * @param $type
 *  Optionally specify content type to ensure returned terms are in relevant vocabularies.
 *
 * @return
 *   A term id.
 */
function fieldtool_taxonomy_value_itemize($field_info, $value, $type = NULL) {
  $messages = array(
    'error' => array(),
    'warning' => array(),
    'status' => array(),
  );

  // Catch term as exported string, $tid (eg. from fieldtool_field_taxonomy_get()) or as term object/array
  if (is_string($value) && preg_match('/]$/', $value)) {
    $temp = explode('[tid:', $value);
    if ($temp[1]) {
      $temp = explode(' ', $temp[1]);
      if (is_numeric($temp[0])) {
        $tid = $temp[0];
      }
    }
  }
  elseif (is_array($value) && $value['tid'] && taxonomy_get_term($value['tid'])) {
    $tid = $value['tid'];
  }
  elseif (is_object($value && $value->tid && taxonomy_get_term($value->tid))) {
    $tid = $value->tid;
  }
  elseif (is_numeric($value) && taxonomy_get_term($value)) {
    $tid = $value;
  }

  // filter by vocabularies for this content type
  if ($type) {
    $term = taxonomy_get_term($tid);
    if (!in_array($term->vid, array_keys(taxonomy_get_vocabularies($type)))) {
      unset($tid);
    }
  }
  
  //TODO: Maybe doing this sloppy search should be optionally triggered by an argument.
  if (!$tid) {
    $tid = fieldtool_taxonomy_term_best_match($value, $node->type);
  }
  
  if (!$tid) {
    $messages['error'][$value] = $field_info['label'] .': Could not find a taxonomy term ID corresponding to "'. $value .'".';
  }
  
  $output = array(
    'fieldtool_itemized_value' => $tid,
    'messages' => $messages,
  );

  return $output;
}

function fieldtool_field_taxonomy_export($node, $field_name, $field_info) {
  $terms = $node->taxonomy ? $node->taxonomy : array();
  if ($field_info['taxonomy vid']) { // If this is a vocabulary field
    foreach ($terms as $tid => $term) {
      if ($term->vid != $field_info['taxonomy vid']) {
        unset($terms[$tid]);
      }
    }
  }

  foreach ($terms as &$term) {
    $term = fieldtool_field_taxonomy_extract($field_info, $term);
  }

  return $terms;
}


function fieldtool_field_taxonomy_extract($field_info, $item) {
  return $item->name .' [tid:'. $item->tid .' vid:'. $item->vid .']';
}


/**
 * Work out whether the imported value is a single item or array of items
 */
function fieldtool_field_taxonomy_import(&$node, $field_name = 'taxonomy', $value, $field_info) {
  $messages = array(
    'error' => array(),
    'warning' => array(),
    'status' => array(),
  );

  if ($value === FALSE) { // We want to delete all terms
    fieldtool_field_taxonomy_set($node, $field_name, $value, $field_info);
    $messages['status'] = array(
      'All existing terms in '. $field_info['label'] .' have been deleted.',
    );
    return $messages;
  }
  
  /* TODO: Can't currently do the $value = array($tid => 0) trick from 
     fieldtool_field_taxonomy_set() here. Not clear that this is desirable,
     as importers won't necessarily know which terms are set without using 
     the getter callback anyway. */
  
  $terms = array();
  if (is_array($value) && !($value['tid'] || $value->tid)) {
    foreach ($value as $tid => $term) { // May or may not actually be a tid. We don't care.
      if ($itemized = fieldtool_taxonomy_value_itemize($field_info, $term, $node->type)) {
        if ($itemized['fieldtool_itemized_value']) {
          $terms[] = $itemized['fieldtool_itemized_value'];
        }
        $messages = fieldtool_message_merge($messages, $itemized['messages']);
      }
    }
  }
  else {
    $itemized = fieldtool_taxonomy_value_itemize($field_info, $value, $node->type);
    if ($itemized['fieldtool_itemized_value']) {
      $terms[] = $itemized['fieldtool_itemized_value'];
    }
    $messages = fieldtool_message_merge($messages, $itemized['messages']);
  }
  
  fieldtool_field_taxonomy_set($node, $field_name, $terms, $field_info);
  return $messages;
}

/**
 * Search for a term that corresponds to a string, assuming we're importing from non-Drupal source.
 *
 * @param $value
 *  A string which may match a taxonomy term name or synonym. 
 *  WARNING: If numeric, will NOT look for matching tid. Use taxonomy_get_term() to check that first if necessary. 
 *
 * @param $type
 *  Optionally specify content type to ensure returned terms are in relevant vocabularies.
 *
 * @return
 *  A term id.
 */
function fieldtool_taxonomy_term_best_match($value, $type = NULL) {
  $matches = taxonomy_get_term_by_name($value);

  if ($type) { // filter by vocabularies for this content type
    $vids = array_keys(taxonomy_get_vocabularies($type));
    foreach ($matches as $term) {
      if (in_array($term->vid, $vids)) {
        $terms[] = $term;
      }
    }
  }
  else {
    $terms = $matches;
  }
  
  if (count($terms)) { //TODO: What to do if there's more than one term?
    return $terms[0]->tid;
  }
  
  // Can't find matching term name, look for synonyms
  if ($term = taxonomy_get_synonym_root($value)) {
    if (count($vids) && in_array($term->vid, $vids)) {
        return $term->tid;
    }
    else {
        return $term->tid;
    }
  }
}




