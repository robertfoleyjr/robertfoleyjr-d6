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
function fieldtool_cck_number_fieldtool_cck_field_types() {
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

  $types['number_integer'] = array(
    'cck extraction callback' => $extraction_callback,
    'extraction callback' => $extraction_callback,
    'getter callback' => 'fieldtool_node_cck_field_get',
    'first value getter callback' => 'fieldtool_node_cck_field_first_get',
    'cck itemization callback' => $itemization_callback,
    'itemization callback' => $itemization_callback,
    'setter callback' => 'fieldtool_node_cck_field_set',
    'single value setter callback' => 'fieldtool_node_cck_field_single_set',
    'non-array value' => TRUE,
    'transformations slot property callback' => 'fieldtool_cck_number_transformations_slot_property',
    'cck subfields' => array('value'),
  );
  $types['number_decimal'] = array(
    'cck extraction callback' => $extraction_callback,
    'extraction callback' => $extraction_callback,
    'getter callback' => 'fieldtool_node_cck_field_get',
    'first value getter callback' => 'fieldtool_node_cck_field_first_get',
    'cck itemization callback' => $itemization_callback,
    'itemization callback' => $itemization_callback,
    'setter callback' => 'fieldtool_node_cck_field_set',
    'single value setter callback' => 'fieldtool_node_cck_field_single_set',
    'non-array value' => TRUE,
    'transformations slot property callback' => 'fieldtool_cck_number_transformations_slot_property',
    'cck subfields' => array('value'),
  );
  $types['number_float'] = array(
    'cck extraction callback' => $extraction_callback,
    'extraction callback' => $extraction_callback,
    'getter callback' => 'fieldtool_node_cck_field_get',
    'first value getter callback' => 'fieldtool_node_cck_field_first_get',
    'cck itemization callback' => $itemization_callback,
    'itemization callback' => $itemization_callback,
    'setter callback' => 'fieldtool_node_cck_field_set',
    'single value setter callback' => 'fieldtool_node_cck_field_single_set',
    'non-array value' => TRUE,
    'transformations slot property callback' => 'fieldtool_cck_number_transformations_slot_property',
    'cck subfields' => array('value'),
  );
  return array('cck field type' => $types);
}

/**
 * Transformations slot property callback.
 */
function fieldtool_cck_number_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'expectedType':
      if ($field_info['name'] == 'number_integer') {
        return 'php:type:integer';
      }
      return 'php:number';
  }
}
