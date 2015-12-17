<?php
// $Id: feedapi_taxonomy_compare_admin.pages.inc,v 1.1.4.5 2009/06/26 04:27:10 indytechcook Exp $

/**
 * @file
 * Admin page callbacks for feedapi taxonomy compare module
 */

/**
 * Form builder.  Configure feedapi taxonomy compare
 *
 * @inggroup forms
 * @see system_settings_form()
 */
function feedapi_taxonomy_compare_admin_settings() {
  //get list of vocabularies
  foreach (taxonomy_get_vocabularies() as $voc) {
    $options[$voc->vid] = $voc->name;
  }

  $form['feedapi_taxonomy_compare_vocabs_compare'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Safe Vocabularies'),
    '#description' => t('Terms in this vocabulary will be added'),
    '#options' => $options,
    '#default_value' => variable_get('feedapi_taxonomy_compare_vocabs_compare', array('0')),
    '#multiple' => TRUE,
  );

  $form['feedapi_taxonomy_compare_syn'] = array(
    '#type' => 'checkbox',
    '#title' => t('Compare To Synonyms'),
    '#default_value' => variable_get('feedapi_taxonomy_compare_syn', 0),
    '#description' => t('Selecting this option will compare the incoming tags from the feed to the selected vocabulary.  If a match is found then the node is related to the existing term and unrelated to the tag'),
  );

  $form['feedapi_taxonomy_remove_found'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove Original Tag if found'),
    '#default_value' => variable_get('feedapi_taxonomy_remove_found', 0),
    '#description' => t('Select this option to remove the term from the tag vocabulary if it if found. <br /> If using FeedAPI Node Inherit, be sure to add the terms to the Safe List'),
  );

  return system_settings_form($form);
}