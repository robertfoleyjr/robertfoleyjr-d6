<?php

function tweetmeme_admin_settings() {
  $form['tweetmeme_source'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter name'),
    '#description' => t("This is used with RT: 'RT @name'. If not set 'tweetmeme' will be used."),
    '#default_value' => variable_get('tweetmeme_source', ''),
  );
  $form['tweetmeme_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types'),
    '#description' => t('Which content types to apply the TweetMeme button to.'),
    '#options' => node_get_types('names'),
    '#default_value' => variable_get('tweetmeme_types', array()),
  );
  $form['tweetmeme_urlShortener'] = array(
    '#type' => 'select',
    '#title' => t('URL shortening service'),
    '#description' => t('Your URLs will be shortened using selected service.'),
    '#options' => array(
      '' => 'bit.ly',
      'awe.sm' => 'awe.sm',
      'cli.gs' => 'cli.gs',
      'digg.com' => 'digg.com',
      'is.gd' => 'is.gd',
      'TinyURL.com' => 'TinyURL.com',
      'tr.im' => 'tr.im',
      'su.pr' => 'su.pr',
      'ow.ly' => 'ow.ly',
      'lopurls.us' => 'lopurls.us',
      'twurl.nl' => 'twurl.nl'
    ),
    '#default_value' => variable_get('tweetmeme_urlShortener', '')
  );
  $form['tweetmeme_location'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Location'),
    '#description' => t('Where to show the TweetMeme button.'),
    '#options' => array(
      'content' => t('Full view'),
      'teasers' => t('Teasers'),
    ),
    '#default_value' => variable_get('tweetmeme_location', array()),
  );
  $form['tweetmeme_style'] = array(
    '#type' => 'select',
    '#title' => t('Style'),
    '#description' => t('The style of the button to use.'),
    '#options' => array(
      'normal' => t('Normal'),
      'compact' => t('Compact'),
    ),
    '#default_value' => variable_get('tweetmeme_style', 'normal'),
  );
  $form['tweetmeme_weight'] = array(
    '#type' => 'select',
    '#title' => t('Full view weight'),
    '#description' => t('The weight at which the TweetMeme widget will appear on full pages.'),
    '#options' => drupal_map_assoc(array(-100, -75, -50, -10, 0, 10, 50, 75, 100)),
    '#default_value' => variable_get('tweetmeme_weight', -10),
  );
  $form['tweetmeme_weight_teaser'] = array(
    '#type' => 'select',
    '#title' => t('Teaser weight'),
    '#description' => t('The weight at which the TweetMeme widget will appear in node teasers.'),
    '#options' => drupal_map_assoc(array(-100, -75, -50, -10, 0, 10, 50, 75, 100)),
    '#default_value' => variable_get('tweetmeme_weight_teaser', -10),
  );
  return system_settings_form($form);
}
