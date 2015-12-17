<?php
/**
 * @file
 * Field Tool - Generic callbacks
 *
 * Generic field handler callbacks for Field Tool.
 *
 * Copyright 2009 Matthew Davidson (http://drupal.org/user/52996).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/**
 * Generic fallback getter callback. Fields with no explicitly declared
 * getter callback and no child fields are assumed to want this as their
 * getter callback.
 */
function fieldtool_field_generic_get($entity, $field_name, $field_info, $deltas = array()) {
  $path = _fieldtool_delta_replace($field_info['field path'], $deltas);
  
  // Turn path into full entity path
  if (is_array($entity)) {
    $path = _fieldtool_object_path_to_array($path);
    $path = '$entity'. $path;
  }
  else {
    $path = '$entity->'. $path;
  }
  
  $pos = strpos($path, '[all]');
  if ($pos !== FALSE ) {
    $paths = array();
    $list = substr($path, 0, $pos);
    eval($list .' = is_array('. $list .') ? '. $list .' : array();');
    $keys = eval('return array_keys('. $list .');');
    foreach ($keys as $key) {
      $paths[] = _fieldtool_field_generic_single_value_get($entity, str_replace('[all]', "[$key]", $path));
    }
    if (count($paths) > 1) {
      return $paths;
    }
    else {
      return $paths[0];
    }
  }
  else {
    return _fieldtool_field_generic_single_value_get($entity, $path);
  }
}

/**
 * Helper function for fieldtool_field_generic_get().
 */
function _fieldtool_field_generic_single_value_get($entity, $path) {
  return eval('return isset('. $path .') ? '. $path .' : NULL;');
}

/**
 * Generic fallback setter callback. Fields with no explicitly declared
 * setter callback and no child fields are assumed to want this as their
 * setter callback.
 */
function fieldtool_field_generic_set(&$entity, $field_name, $value, $field_info, $deltas = array()) {
  $path = _fieldtool_delta_replace($field_info['field path'], $deltas);

  // Turn path into full entity path
  if (is_array($entity)) {
    $path = _fieldtool_object_path_to_array($path);
    $path = '$entity'. $path;
  }
  else {
    $path = '$entity->'. $path;
  }

  if (!is_array($value)) {
    $value = array($value);
  }
  
  $pos = strpos($path, '[all]');
  if ($pos !== FALSE ) {
    $paths = array();
    $list = substr($path, 0, $pos);
    eval($list .' = is_array('. $list .') ? '. $list .' : array();');
    $keys = eval('return array_keys('. $list .');');
    foreach ($keys as $key) {
      _fieldtool_field_generic_single_value_set($entity, str_replace('[all]', "[$key]", $path), $value[$key]);
      unset($value[$key]);
    }
    // if there are any new values left over, add them.
    if (count($value)) { 
      foreach (array_keys($value) as $key) {
        _fieldtool_field_generic_single_value_set($entity, str_replace('[all]', "[$key]", $path), $value[$key]);
        unset($value[$key]);
      }
    }
  }
  else { // Assuming that you won't have passed an array here...
    _fieldtool_field_generic_single_value_set($entity, $path, $value[0]);
  }
}

/**
 * Helper function for fieldtool_field_generic_set().
 */
function _fieldtool_field_generic_single_value_set($entity, $path, $value = NULL) {
  eval($path .' = $value;');
}

/**
 * Generic fallback import callback. Fields with no explicitly declared
 * import callback and no child fields are assumed to want this as their
 * import callback.
 */
function fieldtool_field_generic_import(&$entity, $field_name, $value, $field_info, $deltas = array()) {
  $itemization_callback = fieldtool_get_function($field_info, 'itemization callback');
  
  $messages = array(
    'error' => array(),
    'warning' => array(),
    'status' => array(),
  );
  
  // Replace line breaks in single-line fields
  if ($field_info['single line']) {
    $value = str_replace("\n", ' ', $value);
  }

  if (!$itemization_callback) {
    $value = $value;
  }
  else {
    // This is a bit convoluted for backward-compatibility.
    // In future we may require itemization callbacks return value + messages.
    if (is_array($value)) {
      foreach ($value as &$v) {
        $v = $itemization_callback($field_info, $v);
        if (is_array($v) && array_key_exists('fieldtool_itemized_value', $v)) {
          $messages = is_array($v['messages']) ? fieldtool_message_merge($messages, $v['messages']) : $messages;
          $v = $v['fieldtool_itemized_value'];
        }
      }
    }
    else {
      $value = $itemization_callback($field_info, $value);
      if (is_array($value) && array_key_exists('fieldtool_itemized_value', $value)) {
        $messages = is_array($value['messages']) ? fieldtool_message_merge($messages, $value['messages']) : $messages;
        $value = $value['fieldtool_itemized_value'];
      }
    }
  }
  
  fieldtool_set($field_info, $entity, $value, $deltas);
  
  return $messages;
}

/**
 * Generic fallback export callback. Fields with no explicitly declared
 * export callback and no child fields are assumed to want this as their
 * export callback.
 */
function fieldtool_field_generic_export($entity, $field_name, $field_info, $deltas = array()) {
  $values = fieldtool_get($field_info, $entity, $deltas);
  $extraction_callback = fieldtool_get_function($field_info, 'extraction callback');

  if (!$extraction_callback) {
    return $values;
  }
  else {
    if (is_array($values)) {
      foreach ($values as &$value) {
        $value = $extraction_callback($field_info, $value);
      }
      return $values;
    }
    else {
      return $extraction_callback($field_info, $values);
    }
  }
}

/**
 * Generic fallback extraction callback. Fields with no explicitly declared
 * extraction callback and no child fields are assumed to want this as their
 * extraction callback.
 */
function fieldtool_field_generic_value_extract($field_info, $item) {
  return $item;
}

/**
 * Generic fallback itemization callback. Fields with no explicitly declared
 * itemization callback and no child fields are assumed to want this as their
 * itemization callback.
 * 
 * We assume that the setter callback wants a scalar value and try to
 * convert objects and arrays in the most sensible way possible. Relying on
 * this is often a BAD IDEA. If you have a last-descendant or "leaf" field
 * that requires a non-scalar value for the setter callback, or can reasonably 
 * expect non-scalar import data, please write your own itemization callback.
 */
function fieldtool_field_generic_value_itemize($field_info, $value) {
  if (is_object($value)) {
    $value = (array) $value;
  }
  
  if (is_array($value)) {
    if (_fieldtool_is_keyed_array($value)) {
      $temp = array();
      // This will not work satisfactorily with multidimensional arrays.
      // Did I mention this is risky?
      foreach ($value as $key => $val) {
        $temp[] = $key .': '. $val;
      }
      $value = $temp;
    }
    
    $value = implode(', ', $value);
  }
  
  // Handle CCK allowed values
  if (array_key_exists('cck field type', $field_info)) {
    // Get 'root' CCK field name
    $tmp = explode('-', $field_info['name']);
    $cck_name = $tmp[0];
    $cck_field = content_fields($cck_name);
    $allowed_values = content_allowed_values($cck_field);
    if (count($allowed_values)) {
      if (!array_key_exists($value, $allowed_values)) {  // If $value isn't one of the allowed value keys.
        foreach ($allowed_values as $key => $allowed_value) {
          if ($value == $allowed_value) {
            $value = $key;
            break;
          }
        }
      }
    }
  }
  
  return $value;
}

/**
 * Helper function for generic callbacks.
 */
function _fieldtool_delta_replace($path, $deltas) {
  foreach ($deltas as $delta) {
    $pos = strpos($path, '[%]');
    if ($pos === FALSE) {
      // Assume we're trying to get an item on a field with a path that
      // doesn't explicitly declare the tail '[%]'
      $path .= "[$delta]";
    }
    else {
      $path = substr_replace($path, (int) $delta, $pos + 1, 1);
    }
  }
  
  // Assume that if there's at least one placeholder remaining,
  // we want all values.
  $pos = strrpos($path, '[%]');
  if ($pos !== FALSE) {
    $path = substr_replace($path, 'all', $pos + 1, 1);
  }
  
  // Assume for any leftover placeholders we wanted the first item
  $path = str_replace('[%]', '[0]', $path);
  
  return $path;
}

/**
 * Helper function for generic callbacks.
 *
 * Good gracious, this is kludgy.
 */
function _fieldtool_object_path_to_array($path) {
  $temp = explode('->', $path);
  $temp = explode('[', $temp[0]);
  
  return preg_replace('/^'. $temp[0] .'/', "['". $temp[0] ."']", $path);
}

/**
 * TODO: Recursive getter/setter/import/export/itemize/extract callbacks
 *
 * These are fallbacks for fields _with_ child fields, but no defined callback.
 */


/**
 * Assign generic callback to a field if a callback isn't defined.
 */
function _fieldtool_find_callbacks(&$field_info) {
  $callbacks = array(
    'getter' => 'fieldtool_field_generic_get',
    'setter' => 'fieldtool_field_generic_set',
    'import' => 'fieldtool_field_generic_import',
    'export' => 'fieldtool_field_generic_export',
    'extraction' => 'fieldtool_field_generic_value_extract',
    'itemization' => 'fieldtool_field_generic_value_itemize',
  );
  
  foreach ($callbacks as $type => $function) {
    $field_info[$type .' callback'] = $field_info[$type .' callback'] ? $field_info[$type .' callback'] : $callbacks[$type];
  }
    
  return $field_info;
}


/**
 * Work out path back to root field for generic getter/setter callbacks.
 *
 * @param $entity_type
 *   The thing that you want to retrieve field information about, e.g. 'node'
 *   for node types.
 * @param $field_info
 *   One of the field information arrays returned by fieldtool_fields().
 *
 * @return
 *   An string representing the path to the (sub)field from the root field,
 *   in the usual PHP variable notation, so you getter/setter callbacks can
 *   access the value via eval('return $object->'.$path).
 */
function fieldtool_path($entity_type, $field_info) {
  $path = array();
  while($parent = fieldtool_get_parent($entity_type, $field_info)) {
    if (!$field_info['virtual']){
      // Strip any helpful '-%-' delta placeholders in names
      $parent['name'] = str_replace('-%-', '-', $parent['name']);
      $field_info['name'] = str_replace('-%-', '-', $field_info['name']);
      $type = is_array($parent['fields']) ? 'array' : 'object';
      $bit = array(
        'type' => $type,
        'name' => preg_replace('/^'. $parent['name'] .'-/', '', $field_info['name']),
        'multiple' => $field_info['multiple'],
      );
      array_unshift($path, $bit);
    }
    
    $field_info = $parent;
  }

  // Root field. Last parent found that did not have a parent.
  if (!$field_info['virtual']) {
      $bit = array(
        'type' => 'root',
        'name' => $field_info['name'],
        'multiple' => $field_info['multiple'],
      );
      array_unshift($path, $bit);
  }
  
  $string = '';
  foreach ($path as $i => $bit) {
    if (($bit['type'] == 'array') && ($i > 0)) {
      $string .= "['". $bit['name'] ."']". _fieldtool_multiple_placeholder($bit['multiple']);
    }
    elseif (($bit['type'] == 'object') && ($i > 0)) {
      $string .= '->'. $bit['name']. _fieldtool_multiple_placeholder($bit['multiple']);
    }
    else {
      $string .= $bit['name']. _fieldtool_multiple_placeholder($bit['multiple']);
    }
  }
  
  return $string;
}

/**
 * Helper function for fieldtool_path().
 */
function _fieldtool_multiple_placeholder($multiple = FALSE) {
  if ($multiple) {
    return '[%]';
  }
  return '';
}


