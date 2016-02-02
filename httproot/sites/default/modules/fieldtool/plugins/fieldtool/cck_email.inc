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
function fieldtool_cck_email_fieldtool_cck_field_types() {
  $types = array();

  $types['email'] = array(
    'cck extraction callback' => 'fieldtool_cck_email_value_extract',
    'extraction callback' => 'fieldtool_cck_email_value_extract',
    'getter callback' => 'fieldtool_node_cck_field_get',
    'first value getter callback' => 'fieldtool_node_cck_field_first_get',
    'cck itemization callback' => 'fieldtool_cck_email_value_itemize',
    'itemization callback' => 'fieldtool_cck_email_value_itemize',
    'setter callback' => 'fieldtool_node_cck_field_set',
    'single value setter callback' => 'fieldtool_node_cck_field_single_set',
    'non-array value' => TRUE,
    'multiple' => TRUE,
    'transformations slot property callback' => 'fieldtool_cck_email_transformations_slot_property',
    'cck subfields' => array('email'),
  );
  return array('cck field type' => $types);
}

/**
 * CCK value extraction callback.
 */
function fieldtool_cck_email_value_extract($field_info, $item) {
  return $item['email'];
}

/**
 * CCK value itemization callback.
 */
function fieldtool_cck_email_value_itemize($field_info, $value) {
  if (is_array($value)) {
    return $value;
  }
  elseif (is_string($value)) {
    return array('email' => $value);
  }
}

/**
 * Transformations slot property callback.
 */
function fieldtool_cck_email_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'expectedType':
      return 'php:type:string';
  }
}
