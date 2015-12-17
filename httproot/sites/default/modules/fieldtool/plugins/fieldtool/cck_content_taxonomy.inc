<?php
/**
 * @file
 * Field Tool -
 * An API for programmatically accessing fields of objects in a generic way.
 *
 * Copyright 2009 by Jakob Petsovits ("jpetso", http://drupal.org/user/56020)
 */

/**
 * Implementation of [module]_[plugin]_fieldtool_cck_field_types().
 */
function fieldtool_cck_content_taxonomy_fieldtool_cck_field_types() {
  $extraction_callback = array(
    'function' => 'fieldtool_cck_verbatim_value_extract',
    'file' => 'fieldtool.module',
    'path' => drupal_get_path('module', 'fieldtool'),
  );
  $itemization_callback = array(
    'function' => 'fieldtool_cck_verbatim_value_itemize',
    'file' => 'fieldtool.module',
    'path' => drupal_get_path('module', 'fieldtool'),
  );
  $types = array();

  $types['taxonomy'] = array(
    'cck extraction callback' => $extraction_callback,
    'cck itemization callback' => $itemization_callback,
    'non-array value' => TRUE,
    'transformations slot property callback' => 'fieldtool_cck_content_taxonomy_transformations_slot_property',
  );
  return array('cck field type' => $types);
}

/**
 * Transformations slot property callback.
 */
function fieldtool_cck_content_taxonomy_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'expectedType':
      return 'php:type:integer';
  }
}
