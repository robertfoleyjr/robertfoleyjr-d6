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
function fieldtool_node_standard_fieldtool_fields() {
  $node_fields = array();

  $node_fields['nid']['label'] = t('Node id');
  $node_fields['nid']['stored'] = TRUE;
  $node_fields['nid']['fieldtool_node_standard_verbatim'] = TRUE;
  $node_fields['nid']['fieldtool_node_standard_transformations_callback'] = TRUE;
  $node_fields['nid']['weight'] = -8;

  $node_fields['uid']['label'] = t('Node author');
  $node_fields['uid']['stored'] = TRUE;
  $node_fields['uid']['getter callback'] = 'fieldtool_node_user_get';
  $node_fields['uid']['setter callback'] = 'fieldtool_node_user_set';
  $node_fields['uid']['fieldtool_node_standard_transformations_callback'] = TRUE;
  $node_fields['uid']['weight'] = -7;

  $node_fields['type']['label'] = t('Content type');
  $node_fields['type']['stored'] = TRUE;
  $node_fields['type']['fieldtool_node_standard_verbatim'] = TRUE;
  $node_fields['type']['fieldtool_node_standard_transformations_callback'] = TRUE;
  $node_fields['type']['weight'] = -10;

  $node_fields['title']['label'] = t('Title');
  $node_fields['title']['stored'] = TRUE;
  $node_fields['title']['fieldtool_node_standard_verbatim'] = TRUE;
  $node_fields['title']['fieldtool_node_standard_transformations_callback'] = TRUE;
  $node_fields['title']['weight'] = -5;

  $node_fields['teaser']['label'] = t('Teaser');
  $node_fields['teaser']['stored'] = TRUE;
  $node_fields['teaser']['fieldtool_node_standard_verbatim'] = TRUE;
  $node_fields['teaser']['fieldtool_node_standard_transformations_callback'] = TRUE;
  $node_fields['teaser']['weight'] = -4;

  $node_fields['body']['label'] = t('Body');
  $node_fields['body']['stored'] = TRUE;
  $node_fields['body']['fieldtool_node_standard_verbatim'] = TRUE;
  $node_fields['body']['fieldtool_node_standard_transformations_callback'] = TRUE;
  $node_fields['body']['weight'] = -3;

  $node_fields['created']['label'] = t('Creation date');
  $node_fields['created']['stored'] = TRUE;
  $node_fields['created']['fieldtool_node_standard_date_as_timestamp'] = TRUE;
  $node_fields['created']['fieldtool_node_standard_transformations_callback'] = TRUE;
  $node_fields['created']['weight'] = 5;

  $node_fields['changed']['label'] = t('Last modified');
  $node_fields['changed']['stored'] = TRUE;
  $node_fields['changed']['fieldtool_node_standard_date_as_timestamp'] = TRUE;
  $node_fields['changed']['fieldtool_node_standard_transformations_callback'] = TRUE;
  $node_fields['changed']['weight'] = 5;

  $node_fields['status']['label'] = t('Published');
  $node_fields['status']['stored'] = TRUE;
  $node_fields['status']['fieldtool_node_standard_boolean_as_int'] = TRUE;
  $node_fields['status']['fieldtool_node_standard_transformations_callback'] = TRUE;
  $node_fields['status']['weight'] = 6;

  $node_fields['promote']['label'] = t('Promoted to front page');
  $node_fields['promote']['stored'] = TRUE;
  $node_fields['promote']['fieldtool_node_standard_boolean_as_int'] = TRUE;
  $node_fields['promote']['fieldtool_node_standard_transformations_callback'] = TRUE;
  $node_fields['promote']['weight'] = 6;

  $node_fields['sticky']['label'] = t('Sticky at top of lists');
  $node_fields['sticky']['stored'] = TRUE;
  $node_fields['sticky']['fieldtool_node_standard_boolean_as_int'] = TRUE;
  $node_fields['sticky']['fieldtool_node_standard_transformations_callback'] = TRUE;
  $node_fields['sticky']['weight'] = 6;

  $node_fields['format']['label'] = t('Text format');
  $node_fields['format']['stored'] = TRUE;
  $node_fields['format']['weight'] = 8;

//   if (module_exists('locale')) {
//     $node_fields['language']['label'] = t('Language');
//     $node_fields['language']['stored'] = TRUE;
//   }
  if (module_exists('comment')) {
    $node_fields['comment']['label'] = t('Comment settings');
    $node_fields['comment']['stored'] = TRUE;
    $node_fields['comment']['weight'] = 10;
  }

  foreach (node_get_types('types') as $type_name => $type_info) {
    foreach ($node_fields as $field_name => &$field_info) {
      switch ($field_name) {
        case 'title':
          if ($type_info->has_title) {
            $field_info['node types'][] = $type_name;
          }
          break;

        case 'body':
          if ($type_info->has_body) {
            $field_info['node types'][] = $type_name;
          }
          break;

        default:
          $field_info['node types'][] = $type_name;
          break;
      }
    }
  }

  $getter_callback = array(
    'function' => 'fieldtool_field_verbatim_get',
    'file' => 'fieldtool.module',
    'path' => drupal_get_path('module', 'fieldtool'),
  );
  $setter_callback = array(
    'function' => 'fieldtool_field_verbatim_set',
    'file' => 'fieldtool.module',
    'path' => drupal_get_path('module', 'fieldtool'),
  );

  foreach ($node_fields as $field_name => &$field_info) {
    if (!empty($field_info['fieldtool_node_standard_verbatim'])) {
      unset($field_info['fieldtool_node_standard_verbatim']);
      $field_info['getter callback'] = $getter_callback;
      $field_info['setter callback'] = $setter_callback;
    }
    if (!empty($field_info['fieldtool_node_standard_boolean_as_int'])) {
      unset($field_info['fieldtool_node_standard_boolean_as_int']);
      $field_info['getter callback'] = 'fieldtool_node_int_to_boolean';
      $field_info['setter callback'] = 'fieldtool_node_boolean_to_int';
    }

    if (empty($field_info['stored'])) {
      unset($field_info['setter callback']);
    }
    else {
      // All non-generated standard fields are exportable as is.
      if (!isset($field_info['export callback'])) {
        $field_info['export callback'] = $getter_callback;
      }
      if (!isset($field_info['import callback'])) {
        $field_info['import callback'] = $setter_callback;
      }
    }

    if (!empty($field_info['fieldtool_node_standard_transformations_callback'])) {
      $field_info['transformations slot property callback'] =
        'fieldtool_node_' . $field_name . '_transformations_slot_property';
      unset($field_info['fieldtool_node_standard_transformations_callback']);
    }
  }

  return array('node' => $node_fields);
}

/**
 * The 'status', 'promote' and 'sticky' fields store 0 or 1 even if those
 * values are supposed to mean TRUE or FALSE. In the name of type correctness,
 * we'll perform a conversion to and from boolean in the callbacks.
 */
function fieldtool_node_int_to_boolean($node, $field_name, $field_info) {
  return empty($node->$field_name) ? FALSE : TRUE;
}

/**
 * @see fieldtool_int_to_boolean()
 */
function fieldtool_node_boolean_to_int(&$node, $field_name, $value, $field_info) {
  $node->$field_name = empty($value) ? 0 : 1;
}


function fieldtool_node_user_get($node, $field_name, $field_info) {
  return user_load($node->uid);
}

function fieldtool_node_user_set(&$node, $field_name, $value, $field_info) {
  $node->uid = is_object($value) ? $value->uid : 0;
  $node->name = ($node->uid != 0) ? $value->name : variable_get('anonymous', t('Anonymous'));
}

function fieldtool_node_user_export($node, $field_name, $field_info) {
  return $node->uid;
}

function fieldtool_node_user_import(&$node, $field_name, $value, $field_info) {
  $account = user_load($value);
  fieldtool_node_user_set($node, $field_name, $account, $field_info);
}


//
// Transformations slot property callbacks.
//

function fieldtool_node_nid_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'required':
      return FALSE;

    case 'expectedType':
      return 'php:type:integer';
  }
}

function fieldtool_node_uid_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'required':
      return FALSE;

    case 'expectedType':
      return 'drupal:user:object';
  }
}

function fieldtool_node_type_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'expectedType':
      return 'drupal:node:type'; // is a 'php:type:string'
  }
}

function fieldtool_node_title_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'expectedType':
      return 'php:type:string';
  }
}

function fieldtool_node_body_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'required':
      return FALSE;

    case 'expectedType':
      return 'php:type:string';

    case 'dataWidgetTypeHints':
      return array('string:multiline' => TRUE);
  }
}

function fieldtool_node_created_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'required':
      return FALSE;

    case 'expectedType':
      return 'date:timestamp';
  }
}

function fieldtool_node_changed_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'required':
      return FALSE;

    case 'expectedType':
      return 'date:timestamp';
  }
}

function fieldtool_node_status_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'required':
      return FALSE;

    case 'expectedType':
      return 'php:type:boolean';
  }
}

function fieldtool_node_promote_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'required':
      return FALSE;

    case 'expectedType':
      return 'php:type:boolean';
  }
}

function fieldtool_node_sticky_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'required':
      return FALSE;

    case 'expectedType':
      return 'php:type:boolean';
  }
}
