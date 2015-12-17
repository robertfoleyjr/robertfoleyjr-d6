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
function fieldtool_cck_nodereference_fieldtool_cck_field_types() {
  $types = array();

  $types['nodereference'] = array(
    'extraction callback' => 'fieldtool_cck_nodereference_value_extract',
    'getter callback' => 'fieldtool_node_cck_field_get',
    'first value getter callback' => 'fieldtool_node_cck_field_first_get',
    'itemization callback' => 'fieldtool_cck_nodereference_value_itemize',
    'setter callback' => 'fieldtool_node_cck_field_set',
    'single value setter callback' => 'fieldtool_node_cck_field_single_set',
    'non-array value' => TRUE,
    'transformations slot property callback' => 'fieldtool_cck_nodereference_transformations_slot_property',
    'cck subfields' => array('nid'),
  );
  $types['nodereference-nid'] = array(
    'extraction callback' => 'fieldtool_cck_nodereference_nid_extract',
    'itemization callback' => 'fieldtool_cck_nodereference_nid_itemize',
    'label' => 'Node ID',
  );
  
  return array('cck field type' => $types);
}

/**
 * CCK value extraction callback.
 */
function fieldtool_cck_nodereference_value_extract($field_info, $item) {
  return $item['nid'];
}

/**
 * CCK value itemization callback.
 */
function fieldtool_cck_nodereference_value_itemize($field_info, $value) {
  return array('nid' => $value);
}

/**
 * Transformations slot property callback.
 */
function fieldtool_cck_nodereference_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'expectedType':
      return 'php:type:integer';
  }
}

/**
 * CCK value extraction callback.
 */
function fieldtool_cck_nodereference_nid_extract($field_info, $item) {
  if ($item) { // Don't 'theme' an nid of zero
    $node = node_load($item);
    return $node->title .' [nid:'. $item .']';
  }
}


/**
 * CCK value itemization callback.
 */
function fieldtool_cck_nodereference_nid_itemize($field_info, $value) {
  if (is_numeric($value)) {
    $node = node_load($value);
    if ($node->nid) {
      return($node->nid);
    }
    else { // Hey, it might be a numeric title.
      $node = node_load(array('title' => $value));
      if ($node->nid) {
        return($node->nid);
      }
    }
  }
  elseif (is_string($value)) {
    $bits = explode('[nid:', trim($value));
    if ((count($bits) == 2) && preg_match('/]$/', $bits[1])) {
      $nid = preg_replace('/]$/', '', $bits[1]);
      if (is_numeric($nid) && node_load($nid)) {
        return $nid;
      }
    }
    elseif ($node = node_load(array('title' => $value))) {
      if ($node->nid) {
        return($node->nid);
      }
    }
    else {
      $messages = array(
        'error' => array(
          $value => $field_info['label'] .': Could not find a node title corresponding to "'. $value .'".'
        ),
        'warning' => array(),
        'status' => array(),
      );
      return array(
        'fieldtool_itemized_value' => NULL,
        'messages' => $messages,
      );
    }
  }
  elseif (is_object($value) && $value->nid) {
    return ($value->nid);
  }
  elseif (is_array($value) && $value['nid']) {
    return ($value['nid']);
  }
}





