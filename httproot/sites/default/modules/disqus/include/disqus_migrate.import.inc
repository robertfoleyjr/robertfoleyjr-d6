<?php

/**
 * Menu callback for import settings
 */
function disqus_migrate_admin_import_settings() {
  $form = array();
  
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Import Settings'),
  );
  $form['settings']['disqus_migrate_import_stop_on_errors'] = array(
    '#type' => 'checkbox',
    '#title' => t('Stop on errors'),
    '#description' => t('Leave checked to stop importing when encountering an error. Uncheck to skip over comments that produce errors. Either way, errors will be logged, but once a comment is skipped over, it becomes difficult to re-import it.'),
    '#default_value' => variable_get('disqus_migrate_import_stop_on_errors', 1),
  );

  $form['sync'] = array(
    '#type' => 'fieldset',
    '#title' => t('Automatic Import Settings'),
  );
  $form['sync']['disqus_migrate_import_sync_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => variable_get('disqus_migrate_import_sync_enabled', 0),
    '#description' => t('Enable automatic comment imports on cron runs. New comments since the last import will be imported.'),
  );
  $form['sync']['disqus_migrate_import_sync_interval'] = array(
    '#type' => 'select',
    '#title' => t('Sync Interval'),
    '#default_value' => variable_get('disqus_migrate_import_sync_interval', 86400),
    '#description' => t('How often do you want to import new comments? Requires a properly configured cron.'),
    '#options' => array(
      0 => t('every cron run'),
      600 => t('every 10 minutes'),
      3600 => t('every hour'),
      43200 => t('every 12 hours'),
      86400 => t('once a day'),
    ),
  );
  
  $form['#validate'][] = 'disqus_migrate_admin_import_settings_validate';

  return system_settings_form($form);
}

/**
 * Validate callback for saving Disqus import settings
 */
function disqus_migrate_admin_import_settings_validate($form, &$form_state) {
  
  if ($form_state['values']['disqus_migrate_import_sync_enabled'] == 1) {
    // Check to make sure the Disqus API exists
    if (!_disqus_migrate_api_exists()) {
      form_set_error('sync][disqus_migrate_import_sync_enabled', t('To enable automatic importing, you need to first upload the Disqus API to your libraries folder.'));
    }
  }
  
}

/**
 * Menu callback for importing actions
 */
function disqus_migrate_admin_import() {
  $form = array();
  
  $form['import'] = array(
    '#type' => 'fieldset',
    '#title' => t('Import Now'),
  );
  $form['import']['since'] = array(
    '#type' => 'textfield',
    '#title' => t('Since this date/time'),
    '#size' => 15,
    '#description' => t('Import comments created after this date. PHP !strtotime format. Leave blank to import from the start.', array('!strtotime' => l('strtotime()', 'http://php.net/manual/en/function.strtotime.php'))),
  );
  $imports_exist = db_result(db_query("SELECT COUNT(*) FROM {disqus_migrate}"));
  $form['import']['since_last'] = array(
    '#type' => 'checkbox',
    '#title' => t("Use last import/export time as 'since' date/time"),
    '#disabled' => ($imports_exist == 0) ? TRUE : FALSE,
    '#description' => t('If you have imported previously, or used the export API, use this option to continue where left off. <b>If unchecked, this will likely bring in duplicate comments</b>, unless you have yet to import any.'),
    '#default_value' => ($imports_exist == 0) ? 0 : 1,
  );
  $form['import']['disqus_import_now'] = array(
    '#type' => 'submit',
    '#value' => t('Import Comments Now'),
    '#submit' => array('_disqus_migrate_import_now'),
    '#validate' => array('_disqus_migrate_import_now_validate'),
  );
  
  return $form;
}

/**
 * Validate handler to ensure user entered proper strtotime syntax
 */
function _disqus_migrate_import_now_validate($form, &$form_state) {
  if (!empty($form_state['values']['since']) && strtotime($form_state['values']['since']) === FALSE) {
    form_set_error('sync][since', t('Invalid strtotime() syntax.'));
  }
  
  $secret_key = variable_get('disqus_secretkey','');
  if (empty($secret_key)) {
    form_set_error('', t('Your secret key must be entered before using the import functionality. Please get your secret API key from your Disqus application and enter it in the settings.'));
  }
  
  if (!_disqus_migrate_api_exists()) {
    form_set_error('', t('You need to upload the Disqus API to your libraries folder.'));
  }
}

/**
 * Submit handler for performing an on-demand import
 */
function _disqus_migrate_import_now($form, &$form_state) {

  $since = $form_state['values']['since'];
  if (!empty($since)) {
    // Convert the user entered time to UTC UNIX timestamp
    $curr_tz = date_default_timezone_get();
    date_default_timezone_set("UTC");
    $since = strtotime($since);
    date_default_timezone_set($curr_tz);
  } else if ($form_state['values']['since_last'] == 1) {
    $since = _disqus_migrate_last_synced_time() + 1; // add one second so we don't reimport comments
  }

  _disqus_migrate_import($since);
}

/**
 * The main function that handles importing comments from Disqus into Drupal
 **/
function _disqus_migrate_import($since = 0) {

  // Load API
  _disqus_migrate_api_load();
  $api_secret = variable_get('disqus_secretkey','');
  $dapi = new DisqusAPI($api_secret);
  
  $error_stop = variable_get('disqus_migrate_import_stop_on_errors', 1);
  
  $total_comments_imported = 0;
  $total_comments_processed = 0;
  $continue_importing = TRUE;
  // Continue to import comments until there are none left to import
  while ($continue_importing == TRUE) {
  
    // Build our request to Disqus
    $parameters = array(
      'limit' => 100, // 100 is the max allowed by Disqus
      'order' => 'asc',
      'since' => $since,
      'related' => 'thread',
      'forum' => variable_get('disqus_domain',''),
    );
    
    try {
      $result = $dapi->posts->list($parameters);
    } catch (Exception $e) {
      // Log whatever error was returned
      $message = 'Received error while sending an API request to Disqus: @error';
      $vars = array('@error' => $e);
      drupal_set_message(t($message, $vars), 'error');
      watchdog('disqus_migrate', $message, $vars, WATCHDOG_ERROR);

      return;
    }
    
    // If we received less than 100 comments from Disqus, no need to continue importing
    $comments_retrieved = count($result);
    if ($comments_retrieved < 100) {
      $continue_importing = FALSE;
    }
    
    // Loop through each comment retrieved
    $comments_imported = 0;
    $comments_processed = 0;
    foreach ($result as $comment) {
      $comments_processed++;
          
      // Ensure this comment was not already imported
      $imported_check = db_query("SELECT cid FROM {disqus_migrate} WHERE did=%d", $comment->id);
      if (($existing_cid = db_result($imported_check)) !== FALSE) {
        // already imported
        $message = 'Attempting to import a comment (id #@cid) that has already been imported.';
        $vars = array('@cid' => $existing_cid);
        _disqus_migrate_report_message($message, $vars, 'error', WATCHDOG_ERROR);
        
        if ($error_stop) {
          break 2;
        } else {
          continue;
        }
      }
      
      // Try to find the node ID from what we have.
      // First check the thread identifier for the node path
      $found_nid = FALSE;
      if (($nid = _disqus_migrate_check_nid_exists($comment->thread->identifiers[0])) !== FALSE) {
        $found_nid = TRUE;
      }
      // Could not find the NID in a thread identifier, try to find it in the URL instead
      if (!$found_nid && !empty($comment->thread->link)) {
        // Parse path from URL
        $url = parse_url($comment->thread->link);
        $url = substr($url['path'], strlen(base_path()));
        $normal_path = drupal_get_normal_path($url);
        
        if (($nid = _disqus_migrate_check_nid_exists($normal_path)) !== FALSE) {
          $found_nid = TRUE;
        }
      }
      
      // Tell the user if we found a comment that doesn't exist as a node in on the site
      if (!$found_nid) {
        $message = 'Attempted to import a comment (Disqus comment id: @did) with an invalid node path.';
        $vars = array('@did' => $comment->id);
        _disqus_migrate_report_message($message, $vars, 'error', WATCHDOG_ERROR);
        
        if ($error_stop) {
          break 2;
        } else {
          continue;
        }
      }
      
      // If the comment has a parent, attempt to resolve it against a comment we've already imported
      if (!empty($comment->parent)) {
        $parent_check = db_query("SELECT cid FROM {disqus_migrate} WHERE did=%d", $comment->parent);
        $parent_cid = db_result($parent_check);
        
        if ($parent_cid === FALSE) {
          // This comment has a parent in Disqus, but that parent comment was not imported into Drupal
          // This shouldn't happen, as comments are imported in ascending order
          $message = 'Attempted to import a Disqus comment beginning with "@dctext" (ID: @dcid) that has a parent, but the parent has not been imported. This should not have happened!';
          $vars = array('@dctext' => substr($comment->raw_message, 0, 100) . '...', '@dcid' => $comment->id);
          _disqus_migrate_report_message($message, $vars, 'error', WATCHDOG_ERROR);

          if ($error_stop) {
            break 2;
          } else {
            continue;
          }
        }
      } else {
        $parent_cid = 0;
      }
      
      // The timestamp Disqus provides is in UTC but not in Unix format
      // We use strtotime to convert it, but by default that converts to whatever the servers
      // TZ is. So we set the default timezone to UTC and then set it back after converting.
      $curr_tz = date_default_timezone_get();
      date_default_timezone_set("UTC");
      $comment_timestamp = strtotime($comment->createdAt);
      date_default_timezone_set($curr_tz);
      
      // See if the user exists in Drupal, and if so, assign the proper UID to the imported comment
      $author_uid = db_result(db_query("SELECT uid FROM {users} WHERE mail='%s'", $comment->author->email));
      if ($author_uid === FALSE) {
        $author_uid = 0;
      }
      
      // Check author email. If it matches our anonymous email pattern, disregard it
      if (preg_match('/^anon[0-9]{5}\@anonymous\.com$/', $comment->author->email)) {
        $author_email = '';
      } else {
        $author_email = $comment->author->email;
      }
      
      // TODO - how to deal with status of comments?
      
      $newcomment = array(
        'pid' => $parent_cid,
        'nid' => $nid,
        'uid' => $author_uid,
        'comment' => $comment->message,
        'hostname' => $comment->ipAddress,
        'timestamp' => $comment_timestamp,
        'name' => $comment->author->name,
        'mail' => $author_email,
        'url' => $comment->author->url,
      );
      
      // Save the comment association into our DB for future reference (important for threading)
      if (($newcomment_cid = comment_save($newcomment)) !== FALSE) {
        $comment_assoc = array(
          'did' => $comment->id,
          'dtid' => $comment->thread->id,
          'nid' => $nid,
          'cid' => $newcomment_cid,
        );
        $comment_assoc = (object)$comment_assoc;
        drupal_write_record('disqus_migrate',$comment_assoc);
        
        // Update "since" variable to this comments timestamp + 1 second. If we need to do another round of imports,
        // the last imported comments date will be the starting point
        $since = $comment_timestamp + 1;
        
        $comments_imported++;
      } else {
        $message = "There was an issue creating a new Drupal comment from Disqus comment #@did. Processing aborted.";
        $vars = array('@did' => $comment->id);
        _disqus_migrate_report_message($message, $vars, 'error', WATCHDOG_ERROR);
        break 2;
      }
      
    }
    
    // Add the comments we imported & processed on this run to the total imported
    $total_comments_imported += $comments_imported;
    $total_comments_processed += $comments_processed;
    
    // Prevent an infinite loop from occuring. If we processed all the comments that we retrived and didn't import
    // any, break out of the loop!
    if ($comments_processed == $comments_retrieved && $comments_imported == 0) {
      $continue_importing = FALSE;
    }
    
  }
  
  $message = "Imported @imported comments (of @processed processed) from Disqus into Drupal.";
  $vars = array('@imported' => $total_comments_imported, '@processed' => $total_comments_processed);
  _disqus_migrate_report_message($message, $vars, 'status', WATCHDOG_NOTICE);
}

/**
 * Helper function to log a message to watchdog and drupal messages
 */
function _disqus_migrate_report_message($message, $vars, $drupal_status, $watchdog_status) {
  drupal_set_message(t($message, $vars), $drupal_status);
  watchdog('disqus_migrate', $message, $vars, $watchdog_status);
}

/**
 * Helper function to check if node exists from given path. If it does, return the NID...
 * if not, return FALSE
 */
function _disqus_migrate_check_nid_exists($node_path) {
  if (preg_match("/^node\/([0-9]+)$/", $node_path, $matches)) {
    $node_exists = db_result(db_query("SELECT nid FROM {node} WHERE nid=%d", $matches[1]));
    if ($node_exists !== FALSE) {
      return $matches[1];
    }
  }
  return FALSE;
}