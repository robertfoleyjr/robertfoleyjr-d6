<?php

/**
 * Menu callback; Displays the administration settings for Disqus.
 */
function disqus_admin_settings() {
  $form = array();
  $form['disqus_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Shortname'),
    '#description' => t('The domain that you registered Disqus with. If you registered http://example.disqus.com, you would enter "example" here.'),
    '#default_value' => variable_get('disqus_domain', ''),
  );
  // Visibility settings.
  $form['visibility'] = array(
    '#type' => 'fieldset',
    '#title' => t('Visibility'),
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
  );
  $form['visibility']['disqus_nodetypes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node Types'),
    '#description' => t('Apply comments to only the following node types.'),
    '#default_value' => variable_get('disqus_nodetypes', array()),
    '#options' => node_get_types('names'),
  );
  $form['visibility']['disqus_location'] = array(
    '#type' => 'select',
    '#title' => t('Location'),
    '#description' => t('Display the Disqus comments in the given location. When "Block" is selected, the comments will appear in the <a href="@disquscomments">Disqus Comments block</a>. The "Injected Variable" option will provide a variable for precise placement of the comments.', array('@disquscomments' => url('admin/build/block'))),
    '#default_value' => variable_get('disqus_location', 'content_area'),
    '#options' => array(
      'content_area' => t('Content Area'),
      'block' => t('Block'),
      'variable' => t('Injected Variable'),
    ),
  );
  $form['visibility']['disqus_weight'] = array(
    '#type' => 'select',
    '#title' => t('Weight'),
    '#description' => t('When the comments are displayed in the content area, you can change the position at which they will be shown.'),
    '#default_value' => variable_get('disqus_weight', 50),
    '#options' => drupal_map_assoc(array(-100, -75, -50, -25, 0, 25, 50, 75, 100)),
  );
  // Behavior settings.
  $form['behavior'] = array(
    '#type' => 'fieldset',
    '#title' => t('Behavior'),
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
  );
  $form['behavior']['disqus_userapikey'] = array(
    '#type' => 'textfield',
    '#title' => t('User API Key'),
    '#description' => t('The API key of the administrator account on Disqus. You can get yours <a href="@key">here</a>.', array('@key' => 'http://disqus.com/api/get_my_key/')),
    '#default_value' => variable_get('disqus_userapikey', ''),
  );
  $form['behavior']['disqus_localization'] = array(
    '#type' => 'checkbox',
    '#title' => t('Localization support'),
    '#description' => t("When enabled, overrides the language set by Disqus with the language provided by the site."),
    '#default_value' => variable_get('disqus_localization', FALSE),
  );
  $form['behavior']['disqus_inherit_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Inherit User Credentials'),
    '#description' => t("When enabled and a user is logged in, the Disqus 'Post as Guest' login form will be pre-filled with the user's name and email address."),
    '#default_value' => variable_get('disqus_inherit_login', TRUE),
  );
  $form['behavior']['disqus_developer'] = array(
    '#type' => 'checkbox',
    '#title' => t('Testing'),
    '#description' => t('When enabled, uses the <a href="http://disqus.com/help/#faq-14">disqus_developer</a> flag to tell Disqus that you are in a testing environment. Threads will not display on the public community page with this set.'),
    '#default_value' => variable_get('disqus_developer', FALSE),
  );
  // Advanced settings.
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
    '#description' => t('Use these settings to configure the more advanced uses of Disqus. You can find more information about these in the <a href="@applications">Applications</a> section of Disqus. To enable some of these features, you will require a <a href="@addons">Disqus Add-on Package</a>.', array(
      '@applications' => 'http://disqus.com/api/applications/',
      '@addons' => 'http://disqus.com/addons/',
    )),
  );
  $form['advanced']['disqus_publickey'] = array(
    '#type' => 'textfield',
    '#title' => t('Public Key'),
    '#default_value' => variable_get('disqus_publickey', ''),
    '#size' => 85,
  );
  $form['advanced']['disqus_secretkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret Key'),
    '#default_value' => variable_get('disqus_secretkey', ''),
    '#size' => 85,
  );
  $form['advanced']['disqus_sso'] = array(
    '#type' => 'checkbox',
    '#title' => t('Single Sign-On'),
    '#description' => t('Provide <a href="@sso">Single Sign-On</a> access to your site.', array(
      '@sso' => 'http://disqus.com/api/sso/',
    )),
    '#default_value' => variable_get('disqus_sso', FALSE),
  );
  
  /*
  $oauth_expires = variable_get('disqus_oauth_expires_on', '');
  if (empty($oauth_expires)) {
    $form['advanced']['authenticate'] = array(
      '#type' => 'submit',
      '#prefix' => '<strong>API Authentication: </strong>',
      '#value' => t('Authenticate this website'),
      '#submit' => array('_disqus_oauth_authenticate'),
    );
  }
  else {
    $form['advanced']['authenticate'] = array(
      '#type' => 'submit',
      '#prefix' => '<strong>API Authentication: </strong>',
      '#value' => t('De-Authenticate this website'),
      '#submit' => array('_disqus_oauth_deauthenticate'),
    );
  }
  */
  
  return system_settings_form($form);
}

/**
 * Menu callback; Automatically closes the window after the user logs in.
 */
function disqus_closewindow() {
  drupal_add_js('window.close();', 'inline');
  return t('Thank you for logging in. Please close this window, or <a href="@clickhere">click here</a> to continue.', array('@clickhere' => 'javascript:window.close();'));
}

/**
 * Initiate an OAuth authentication with Disqus to get an access token
 */
/*
function _disqus_oauth_authenticate($form, &$form_state) {
  $path = 'https://disqus.com/api/oauth/2.0/authorize/';
  $query = array(
    'client_id' => variable_get('disqus_publickey',''),
    'scope' => 'read,write,admin',
    'response_type' => 'code',
    'redirect_uri' => url('disqus/oauth_callback', array('absolute' => TRUE)),
  );
  
  // Bring the user to the OAuth2 authentication page. They will be redirected back to this website.
  drupal_goto($path, $query);
}
*/

/**
 * Clear out the access keys that have been saved
 */
/*
function _disqus_oauth_deauthenticate($form, &$form_state) {
  variable_del('disqus_oauth_access_token');
  variable_del('disqus_oauth_expires_on');
  variable_del('disqus_oauth_refresh_token');
}
*/


/**
 * This is the callback function that gets called when a user has authenticated with Disqus via OAuth 2
 *
 * If the user allowed the application access, we will be able to extract a token from the URL
 * and use it get obtain our access token for Disqus.
 */
/*
function _disqus_oauth_callback() {
  
  $code = $_GET['code'];
  if (!empty($code)) {
     
    // Build the request URI and query
    $parameters = array(
      'grant_type' => 'authorization_code',
      'client_id' => variable_get('disqus_publickey',''),
      'client_secret' => variable_get('disqus_secretkey',''),
      'redirect_uri' => url('disqus/oauth_callback', array('absolute' => TRUE)),
      'code' => $code,
    );
    
    _disqus_oath_request_access_token($parameters);
  }
  
  drupal_set_message(t("You have authenticated with Disqus (unless you chose not to authorize the application). You can manually drop the access keys, but they will auto-renew every 30 days."));
  
  drupal_goto('admin/settings/disqus');
}
*/

/**
 * Send a request to Disqus to obtain an access token. Takes in an array of parameters
 * to send along w/ the post request.
 */
/*
function _disqus_oath_request_access_token($parameters) {

  $data = http_build_query($parameters, '', '&');
  $url = 'https://disqus.com/api/oauth/2.0/access_token/';
  $headers = array('Content-Type' => 'application/x-www-form-urlencoded');
  
  // Send request and return response
  $response = drupal_http_request($url, $headers, 'POST', $data);
  $content = $response->data;
  $data = json_decode($content);
  
  // Error
  if ($response->code != 200) {
    $message = 'An error occured while attempting to obtain an access token: @error';
    $vars = array('@error' => $data->error_description);
    drupal_set_message(t($message,$vars), 'error');
    watchdog('disqus', $message, $vars, WATCHDOG_ERROR);
  }
  // Success
  else {
    // Save the access token and other values
    $expires_on = time() + $data->expires_in;
    
    variable_set('disqus_oauth_access_token', $data->access_token);
    variable_set('disqus_oauth_expires_on', $expires_on);
    variable_set('disqus_oauth_refresh_token', $data->refresh_token);
  }
}
*/