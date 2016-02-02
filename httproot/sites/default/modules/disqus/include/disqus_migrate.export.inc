<?php

/**
 * Menu callback for the export settings
 */
function disqus_migrate_admin_export_settings() {
  $form = array();
  
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Export Settings'),
    '#description' => t('Select the following options to effect what comments are exported.'),
  );
  $status_options = array(
    '--' => t('No Change'),
    0 => t('Disabled'),
    1 => t('Read Only'),
  );
  $form['settings']['disqus_migrate_export_status_alter'] = array(
    '#type' => 'select',
    '#default_value' => variable_get('disqus_migrate_export_status_alter', '--'),
    '#title' => t('Node comment status change'),
    '#description' => t('After comments for a node have been exported, the commenting status can be altered. Useful for disabling Drupal commenting once comments have been exported.'),
    '#options' => $status_options,
  );
  $api_limit_options = array(
    20 => 20,
    100 => 100,
    250 => 250,
    500 => 500,
  );
  $form['settings']['disqus_migrate_export_api_limit'] = array(
    '#type' => 'select',
    '#options' => $api_limit_options,
    '#default_value' => variable_get('disqus_migrate_export_api_limit', 100),
    '#title' => t('Batch limit for API export'),
    '#description' => t('How many comments should be exported at a time? This does not apply to the XML export. The default value is usually the best bet for performance.'),
  );

  return system_settings_form($form);
}

/**
 * Menu callback for the export actions
 */
function disqus_migrate_admin_export() {
  $form = array();

  $form['export'] = array(
    '#type' => 'fieldset',
    '#title' => t('Export Now'),
  );
  $pending_comments = db_result(db_query("SELECT COUNT(*) FROM {comments} c WHERE c.cid NOT IN (SELECT dm.cid FROM {disqus_migrate} dm)"));
  if ($pending_comments === FALSE) {
    $pending_comments = 0;
  }
  $form['export']['pending'] = array(
    '#type' => 'markup',
    '#prefix' => '<p>',
    '#value' => t('There are @pending comments awaiting export.', array('@pending' => $pending_comments)),
    '#suffix' => '</p>',
  );
  $form['export']['export_api'] = array(
    '#type' => 'submit',
    '#value' => t('Export awaiting comments using API'),
    '#validate' => array('_disqus_migrate_export_api_validate'),
    '#submit' => array('_disqus_migrate_export_api'),
  );
  $form['export']['export_xml'] = array(
    '#type' => 'submit',
    '#value' => t('Export all comments to XML file'),
    '#submit' => array('_disqus_migrate_export_wxr'),
  ); 
  
  return $form;
}

/**
 * When exporting via the API, make sure the user filled out the necessary info
 */
function _disqus_migrate_export_api_validate($form, &$form_state) {
  // Check for existance of Disqus API
  if (!file_exists('sites/all/libraries/disqusapi/disqusapi.php')) {
    form_set_error('', t('Download the <a href="@disqusapiurl">PHP API implementation here</a> and upload the disqusapi folder to your sites/all/libraries folder. Also ensure that you have authenticated with Disqus.', array('@disqusapiurl' => url('https://github.com/disqus/disqus-php', array('absolute' => TRUE)))));
  }
  
  // Make sure secret key is filled out
  $api_secret = variable_get('disqus_secretkey','');
  if (empty($api_secret)) {
    form_set_error('', t('Your secret key must be entered before using this export functionality. Please get your application API keys from Disqus and enter them in the "Advanced" fieldset on the settings page.'));
  }
}

/**
 * Calls function to gather comments to export, then attempts to create the comments
 * in Disqus using the API.
 */
function _disqus_migrate_export_api($form, &$form_state) {

  // Get API secret
  $api_secret = variable_get('disqus_secretkey','');
  
  // Load API
  include_once('sites/all/libraries/disqusapi/disqusapi.php');
  
  // Initialize the API
  $dapi = new DisqusAPI($api_secret);
  
  // What should we do to a local comment after processing
  $status_change = variable_get('disqus_migrate_export_status_alter', '--');
  
  // Gather thread data
  $thread_data = _disqus_migrate_export_gather();
  
  $comments_exported = 0;
  foreach ($thread_data as $nid => $thread) {

    // Check to see if this thread exists already in Disqus
    $thread_id = db_result(db_query("SELECT dtid FROM {disqus_migrate} WHERE nid=%d", $nid));
    if ($thread_id === FALSE) {
  
      $parameters = array(
        'forum' => variable_get('disqus_domain',''),
        'thread:ident' => 'node/' . $nid,
        'thread:link' => url('node/' . $nid, array('absolute' => TRUE, 'alias' => TRUE)),
        'api_key' => $api_key,
        'forum' => variable_get('disqus_domain',''),
      );

      try {
        $response = $dapi->threads->details($parameters);
        
        $thread_id = $response->id;
      } catch (Exception $e) {
        // Thread doesn't exist, don't do anything
      }
    }
    
    // No thread exists for this node, so we attempt to create one and obtain it's ID
    if ($thread_id === FALSE) {
      
      $parameters = array(
        'title' => $thread['title'],
        'url' => $thread['link'],
        'date' => $thread['post_date_gmt_unix'],
        'identifier' => $thread['identifier'],
        'api_key' => $api_key,
        'api_secret' => $api_secret,
        'forum' => variable_get('disqus_domain',''),
      );

      try {
        $response = $dapi->threads->create($parameters);
        
        $thread_id = $response->id;
      } catch (Exception $e) {
        // Log whatever error was returned
        drupal_set_message(t('Error when attempting to create Disqus thread for node id @nid (export has stopped): @error', array('@nid' => $nid, '@error' => $e)), 'error');
        break;
      }

    }
    // If we are able to get the Disqus thread ID, continue with exporting the comment
    else {
      
      foreach ($thread['comments'] as $cid => $comment) {
        
        // Get the Disqus parent ID
        $parent_did = db_result(db_query("SELECT did FROM {disqus_migrate} WHERE cid=%d", $comment['parent']));
        $parent_did = ($parent_did === FALSE) ? null : $parent_did;

        $parameters = array(
          'message' => substr($comment['content'], 0, 5000), // this seems to be the threshold fo how long a request can be
          'parent' => $parent_did,
          'thread' => $thread_id,
          'author_email' => $comment['author_email'],
          'author_name' => $comment['author'],
          'author_url' => $comment['author_url'],
          'state' => 'approved',
          'date' => $comment['date_gmt_unix'],
          'ip_address' => $comment['author_IP'],
          'api_secret' => $api_secret,
        );
        
        try {
          $response = $dapi->posts->create($parameters);
          
          // Comment successfully exported... let's make a record of it
          $new_record = new stdClass();
          $new_record->did = $response->id;
          $new_record->dtid = $thread_id;
          $new_record->nid = $nid;
          $new_record->cid = $cid;
          
          drupal_write_record('disqus_migrate', $new_record);
          $comments_exported++;
        } catch (Exception $e) {
          // Log whatever error was returned
          drupal_set_message(t('Error when attempting to create Disqus post for comment id @cid (export has stopped): @error', array('@cid' => $cid, '@error' => $e)), 'error');
          break 2;
        }

      } // foreach comments
      
      // Should change the comment status for the node?
      if ($status_change != '--') {
        $update = db_query("UPDATE {node} SET comment = %d WHERE nid = %d", $status_change, $nid);
      }
      
    }
    
  } //foreach threads (nodes)
  
  drupal_set_message(t("@exported comments have been exported from Drupal into Disqus.", array('@exported' => $comments_exported)));

}


/**
 * Calls function to gather comments to export, then builds the appropriate XML file
 * and presents it to the user for download.
 */
function _disqus_migrate_export_wxr($form, &$form_state) {

  // Gather ALL of the comments
  $thread_data = _disqus_migrate_export_gather(TRUE);
  
  $status_change = variable_get('disqus_migrate_export_status_alter', '--');
  
  if (!empty($thread_data)) {
    // Pass the data to a function that generates the XML
    $output = '';
  
    // Print header
    $output .= '<?xml version="1.0" encoding="UTF-8"?>';
    $output .= '<rss version="2.0"';
    $output .= '  xmlns:content="http://purl.org/rss/1.0/modules/content/"';
    $output .= '  xmlns:dsq="http://www.disqus.com/"';
    $output .= '  xmlns:dc="http://purl.org/dc/elements/1.1/"';
    $output .= '  xmlns:wp="http://wordpress.org/export/1.0/"';
    $output .= '>';
    $output .= '  <channel>';
    
    foreach ($thread_data as $nid => $thread) {
      // Skip thread if there were no comments attached to it. This would only happen if
      // an export is created and there are no published comments on a node (also depends
      // on what user selects in checkbox)
      if (empty($thread['comments'])) {
        continue;
      }
    
      $output .= '<item>';
      $output .= '<title>' . _disqus_migrate_cleanse_xml($thread['title']) . '</title>';
      $output .= '<link>' . $thread['link'] . '</link>';
      $output .= '<content:encoded></content:encoded>';
      $output .= '<dsq:thread_identifier>' . $thread['identifier'] . '</dsq:thread_identifier>';
      $output .= '<wp:post_date_gmt>' . $thread['post_date_gmt'] . '</wp:post_date_gmt>';
      $output .= '<wp:comment_status>open</wp:comment_status>';
      
      foreach ($thread['comments'] as $comment) {
      
        $output .= '<wp:comment>';
        
        $output .= '<wp:comment_id>' . $comment['id'] . '</wp:comment_id>';
        $output .= '<wp:comment_author>' . $comment['author'] . '</wp:comment_author>';
        $output .= '<wp:comment_author_email>' . $comment['author_email'] . '</wp:comment_author_email>';
        $output .= '<wp:comment_author_url>' . $comment['author_url'] . '</wp:comment_author_url>';
        $output .= '<wp:comment_author_IP>' . $comment['author_IP'] . '</wp:comment_author_IP>';
        $output .= '<wp:comment_date_gmt>' . $comment['date_gmt'] . '</wp:comment_date_gmt>';
        $output .= '<wp:comment_content><![CDATA[' . $comment['content'] . ']]></wp:comment_content>';
        $output .= '<wp:comment_approved>' . $comment['approved'] . '</wp:comment_approved>';
        $output .= '<wp:comment_parent>' . $comment['parent'] . '</wp:comment_parent>';
        
        $output .= '</wp:comment>';
      }
      
      $output .= '</item>';
      
      // Should change the comment status for the node?
      if ($status_change != '--') {
        $update = db_query("UPDATE {node} SET comment = %d WHERE nid = %d", $status_change, $nid);
      }
    }
    
    // Footer
    $output .= '  </channel>';
    $output .= '</rss>';
    
    // Output the XML file
    header("Content-disposition: attachment; filename=drupalcomments.xml");
    header("Content-Type: text/xml; charset=utf-8");
    
    print $output;
    
    exit();
  } else {
    drupal_set_message(t('No comments to export.'), 'error');
  }

}

/**
 * Helper function to gather comments to export. The boolean parameter determines
 * whether or not to select all the comments or all comments since last export
 */
function _disqus_migrate_export_gather($select_all = FALSE) {
  // Select all the nodes with comments
  $thread_data = array();

  // Make sure the dates are exported in GMT time as expected by Disqus
  date_default_timezone_set('GMT');

  // Get max amount of comments to gather
  $export_threshold = variable_get('disqus_migrate_export_api_limit', 100);
  $comments_gathered = 0;

  // Storage for looked up user emails
  $user_emails = array();
  
  // Gather distinct node IDs from comments
  if ($select_all) {
    $node_results = db_query("SELECT DISTINCT c.nid FROM {comments} c");
  }
  else {
    // Find all comments that haven't already been imported/exported via this module. Let's also limit the result to prevent
    // any gathering an unnecessary amount of comments.
    $node_results = db_query(
    "SELECT DISTINCT c.nid FROM {comments} c LEFT JOIN {disqus_migrate} dm ON c.cid = dm.cid WHERE dm.cid IS NULL LIMIT 0, %d",
    $export_threshold);
  }
  $max_comment_id = 0;
  while ($nid = db_result($node_results)) {
  
    // get some bits of info from the node. This is much more efficient than doing a node_load
    $node_data_query = db_query("SELECT title, created, status FROM {node} WHERE nid = %d", $nid);
    $node_data = db_fetch_object($node_data_query);
    
    // Load up the thread data array for this node
    $thread_data[$nid] = array(
      'title' => $node_data->title,
      'link' => url("node/" . $nid, array('absolute' => TRUE, 'alias' => TRUE)),
      'identifier' => 'node/' . $nid,
      'post_date_gmt' => date("Y-m-d H:i:s", $node_data->created),
      'post_date_gmt_unix' => $node_data->created,
    );

    // Find all comments attached to this node
    if ($select_all) {
      $comment_query = "SELECT * FROM {comments} c WHERE c.nid = %d ORDER BY c.cid ASC";
    }
    else {
      $comment_query = "SELECT c.* FROM {comments} c LEFT JOIN {disqus_migrate} dm ON c.cid = dm.cid WHERE dm.cid IS NULL AND c.nid = %d ORDER BY c.cid ASC";
    }
    $comments_results = db_query($comment_query, $nid);
    while ($comment = db_fetch_object($comments_results)) {

      // Set max comment ID
      $max_comment_id = ($comment->cid > $max_comment_id) ? $comment->cid : $max_comment_id;
    
      if ($comment->uid != 0) {
        // Logged in user left comment, obtain email from user account
        if (isset($user_emails[$comment->uid])) {
          $comment_mail = $user_emails[$comment->uid];
        } else {
          $user_mail = db_result(db_query("SELECT mail FROM {users} WHERE uid=%d", $comment->uid));
          $comment_mail = $user_mail;
          // Save into storage so we don't need another query
          $user_emails[$comment->uid] == $user_mail;
        }
      } elseif ($comment->mail) {
        // Anon commenter left email, use that
        $comment_mail = $comment->mail;
      } else {
        // Anon commenter did NOT leave email, but it's required by Disqus, so
        // generate a random one (to make each unique)
        $random = rand(1000,100000);
        $comment_mail = "anon" . $random . "@anonymous.com";
      }
      
      // Use name "Anonymous" for anonymous users
      if ($comment->name) {
        $comment_name = $comment->name;
      } else {
        $comment_name = "Anonymous";
      }
    
      // Load up the comment data for this comment
      $thread_data[$nid]['comments'][$comment->cid] = array(
        'id' => $comment->cid,
        'author' => $comment_name,
        'author_email' => $comment_mail,
        'author_url' => $comment->homepage,
        'author_IP' => $comment->hostname,
        'date_gmt' => date("Y-m-d H:i:s", $comment->timestamp),
        'date_gmt_unix' => $comment->timestamp,
        'content' => $comment->comment,
        'approved' => ($comment->status == 1) ? 0 : 1,
        'parent' => $comment->pid,
      );
      
      $comments_gathered++;
      // Check to see if we reached limit of comments we shoudl export
      if ($comments_gathered >= $export_threshold && !$select_all) {
        break 2;
      }
    }
  }
  
  return $thread_data;
}

/**
 * Helper function cleanse a string for placement in an XML file
 */
function _disqus_migrate_cleanse_xml($string) {
  $find = array(
    '>',
    '<',
    '&',
    '"',
    "'",
  );

  $replace = array(
    '&gt;',
    '&lt;',
    '&amp;',
    '&quot;',
    '&apos;',
  );

  return str_replace($find, $replace, $string);
}
