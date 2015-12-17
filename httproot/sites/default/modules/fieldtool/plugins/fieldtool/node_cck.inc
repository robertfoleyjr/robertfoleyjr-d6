<?php
/**
 * @file
 * Field Tool -
 * An API for programmatically accessing fields of objects in a generic way.
 *
 * Copyright 2009 by Jakob Petsovits ("jpetso", http://drupal.org/user/56020)
 */

/**
 * Implementation of [module]_[plugin]_fieldtool_fields().
 */
function fieldtool_node_cck_fieldtool_fields() {
  if (!module_exists('content')) {
    return array();
  }
  $node_types = node_get_types('types');
  $node_fields = array();
  $supported_field_types = fieldtool_node_cck_supported_field_types();

  foreach (content_types() as $type_name => $type_info) {
    foreach ($type_info['fields'] as $field_name => $field) {
      $node_fields[$field_name] = array(
        // The actual UI string is only stored in the instance, as part of
        // the widget. We can't rely on the widget label to be consistent
        // across all instances, but it's a pretty good guess and correct
        // most of the time.
        'label' => t('@label (@field-name)', array(
          '@label' => $field['widget']['label'],
          '@field-name' => $field_name,
        )),
        'stored' => TRUE,
        'cck field type' => $field['type'],
        // We have changed the meaning of 'multiple'. http://drupal.org/node/644444#comment-2596870
        //'multiple' => $field['multiple'],
      );
      if (isset($supported_field_types[$field['type']])) {
        $node_fields[$field_name] += $supported_field_types[$field['type']];
        
        // Field Hierarchies
        if (count($node_fields[$field_name]['cck subfields'])) {
          _fieldtool_node_cck_create_subfields($node_fields, $field_name, $field);
        }
      }
      $node_fields[$field_name]['node types'][] = $type_name;
    }
  }

  return array('node' => $node_fields);
}

/**
 * Retrieve properties of CCK field types, calling the
 * [module]_[plugin]_fieldtool_node_cck_field_types() hook.
 *
 * (See this module's plugins/fieldtool_node_cck/ directory for some examples.)
 */
function fieldtool_node_cck_supported_field_types() {
  static $field_types;
  if (is_array($field_types)) {
    return $field_types;
  }
  
  _fieldtool_ctools_include_plugins('plugins');
  $plugins = ctools_get_plugins('fieldtool', 'cck_field_types');
  $field_types = array();

  foreach ($plugins as $plugin) {
    foreach ($plugin['cck field type'] as $field_type => &$field_type_info) {
      $field_type_info['module'] = $plugin['module'];
      $field_type_info['file'] = $plugin['file'];
      $field_type_info['path'] = $plugin['path'];
      $field_type_info['name'] = $field_type;
      // Assume multiple unless otherwise specified.
      $field_type_info['multiple'] = isset($plugin['multiple']) ? $plugin['multiple'] : TRUE;

      foreach (module_invoke_all('fieldtool_node_callback_names') as $callback_name) {
        if (isset($field_type_info[$callback_name]) && is_string($field_type_info[$callback_name])) {
          $field_type_info[$callback_name] = array(
            'function' => $field_type_info[$callback_name],
            'file' => $field_type_info['file'],
            'path' => $field_type_info['path'],
          );
        }
      }

      $field_types[$field_type] = $field_type_info;
    }
  }
  return $field_types;
}

/**
 * Common getter function for CCK fields, utilizing the CCK extraction callback
 * to create an array of values containing the results of that callback for
 * each field item.
 */
function fieldtool_node_cck_field_get($node, $field_name, $field_info) {
  if (!isset($node->$field_name)) {
    return array();
  }
  $values = array();
  $function = fieldtool_get_function($field_info, 'cck extraction callback');

  if (empty($function)) {
    throw new Exception('CCK extraction function could not be found.');
  }
  foreach ($node->$field_name as $delta => $item) {
    $values[$delta] = $function(content_fields($field_info['cck field type']), $item);
  }
  return $values;
}

/**
 * Extract the first item from a CCK field, or NULL if the number of items
 * is zero for the given field and node.
 */
function fieldtool_node_cck_field_first_get($node, $field_name, $field_info) {
  if (!isset($node->$field_name)) {
    return NULL;
  }
  $values = array();
  $function = fieldtool_get_function($field_info, 'cck extraction callback');

  if (empty($function)) {
    throw new Exception('CCK extraction function could not be found.');
  }
  foreach ($node->$field_name as $delta => $item) {
    return $function(content_fields($field_info['cck field type']), $item);
  }
  return NULL;
}

/**
 * Common setter function for CCK fields, utilizing the CCK itemization
 * callback to create an array of items containing the results of that callback
 * for each value in the passed value array.
 */
function fieldtool_node_cck_field_set(&$node, $field_name, $values, $field_info) {
  $args = func_get_args();
  if (!is_array($values)) {
    if (!empty($field_info['non-array value'])) {
      // If the extracted value is not an array
      $values = array($values);
    }
    else {
      throw new Exception('Invalid value passed to the CCK setter function. ' . print_r($values, TRUE));
    }
  }
  $items = array();
  $function = fieldtool_get_function($field_info, 'cck itemization callback');

  if (empty($function)) {
    throw new Exception('CCK itemization function could not be found.');
  }
  foreach ($values as $value) {
    $items[] = $function(content_fields($field_info['cck field type']), $value);
  }
  $node->$field_name = $items;
}

/**
 * Set a single value as first item of the CCK field.
 */
function fieldtool_node_cck_field_single_set(&$node, $field_name, $value, $field_info) {
  fieldtool_node_cck_field_set($node, $field_name, array($value), $field_info);
}

/**
 * Common CCK value extraction callback that retrieves the value verbatim from
 * the 'value' key of a node's CCK field. CCK field type info hooks can use
 * this callback to avoid code duplication.
 */
function fieldtool_cck_verbatim_value_extract($field_info, $item) {
  return $item['value'];
}

/**
 * Common CCK value itemization callback that sets the value verbatim to the
 * 'value' key of a node's CCK field. CCK field type info hooks can use this
 * callback to avoid code duplication.
 */
function fieldtool_cck_verbatim_value_itemize($field_info, $value) {
  return array('value' => $value);
}


/**
 * Automagically create CCK subfields.
 *
 * @param $subfields
 *   We try to cope with just about any kind of array.
 *   eg. array('value', 'description'), or array('title', 'href', 'attributes' => array('class', 'id'))
 */
function _fieldtool_node_cck_create_subfields(&$node_fields, $field_name, $field) {
  $supported_field_types = fieldtool_node_cck_supported_field_types();

  foreach($node_fields[$field_name]['cck subfields'] as $subfield) {
    $node_fields[$field_name .'-'. $subfield]['cck field type'] = $node_fields[$field_name]['cck field type'] .'-'. $subfield;
    $node_fields[$field_name .'-'. $subfield]['stored'] = TRUE;
    $node_fields[$field_name .'-'. $subfield]['multiple'] = FALSE;

    $node_fields[$field_name .'-'. $subfield]['node types'][] = $field['type_name'];
    if (isset($supported_field_types[$node_fields[$field_name]['cck field type'] .'-'. $subfield])) {
      $node_fields[$field_name.'-'. $subfield] += $supported_field_types[$node_fields[$field_name]['cck field type'] .'-'. $subfield];
    }
    
    // Don't just keep appending if we've been here before
    // Got to be a better way of doing this.
    if (strlen($node_fields[$field_name .'-'. $subfield]['label']) > strlen($node_fields[$field_name]['label'])) {
      $chunk = substr($node_fields[$field_name .'-'. $subfield]['label'], 0, strlen($node_fields[$field_name]['label']));
      if ($chunk == $node_fields[$field_name]['label']) {
        $node_fields[$field_name .'-'. $subfield]['label'] = substr($node_fields[$field_name .'-'. $subfield]['label'], strlen($node_fields[$field_name]['label']), strlen($node_fields[$field_name .'-'. $subfield]['label']));
      }
    }
    
    $node_fields[$field_name.'-'. $subfield]['label'] = $node_fields[$field_name.'-'. $subfield]['label']
      ? implode(' ', array($node_fields[$field_name]['label'], $node_fields[$field_name.'-'. $subfield]['label']))
      : implode(' ', array($node_fields[$field_name]['label'], ucwords(str_replace('_', ' ', $subfield))));
    
    // I want _my_ default callbacks, dammit!
    _fieldtool_find_callbacks($node_fields[$field_name .'-'. $subfield]);
    
    $node_fields[$field_name]['fields'][] = $field_name.'-'. $subfield;
    
    // recurse
    if ($node_fields[$field_name .'-'. $subfield]['cck subfields']) {
      //dsm($field_name .'-'. $subfield);
      _fieldtool_node_cck_create_subfields(&$node_fields, $field_name .'-'. $subfield, $field);
    }
  }
}


