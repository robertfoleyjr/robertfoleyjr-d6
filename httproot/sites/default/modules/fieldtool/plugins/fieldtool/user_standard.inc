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
function fieldtool_user_standard_fieldtool_fields() {
  $user_fields = array();

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

  $user_fields['uid']['label'] = t('User id');
  $user_fields['uid']['stored'] = TRUE;
  $user_fields['uid']['fieldtool_user_standard_verbatim'] = TRUE;
  $user_fields['uid']['fieldtool_user_standard_transformations_callback'] = TRUE;
  $user_fields['uid']['weight'] = -8;

  $user_fields['name']['label'] = t('User name');
  $user_fields['name']['stored'] = TRUE;
  $user_fields['name']['fieldtool_user_standard_verbatim'] = TRUE;
  $user_fields['name']['fieldtool_user_standard_transformations_callback'] = TRUE;
  $user_fields['name']['weight'] = -7;

  $user_fields['mail']['label'] = t('Email address');
  $user_fields['mail']['stored'] = TRUE;
  $user_fields['mail']['fieldtool_user_standard_verbatim'] = TRUE;
  $user_fields['mail']['fieldtool_user_standard_transformations_callback'] = TRUE;
  $user_fields['mail']['weight'] = -5;

  $user_fields['pass']['label'] = t('Password (plaintext)');
  $user_fields['pass']['stored'] = FALSE;
  $user_fields['pass']['setter callback'] = 'fieldtool_user_pass_set';
  $user_fields['pass']['fieldtool_user_standard_transformations_callback'] = TRUE;
  $user_fields['pass']['weight'] = -4;

  $user_fields['pass_md5']['label'] = t('Password (MD5 hash)');
  $user_fields['pass_md5']['stored'] = TRUE;
  $user_fields['pass_md5']['getter callback'] = 'fieldtool_user_pass_md5_get';
  $user_fields['pass_md5']['fieldtool_user_standard_transformations_callback'] = TRUE;
  $user_fields['pass_md5']['weight'] = -4;

  $user_fields['roles']['label'] = t('Roles');
  $user_fields['roles']['stored'] = TRUE;
  $user_fields['roles']['getter callback'] = $getter_callback;
  $user_fields['roles']['setter callback'] = 'fieldtool_user_roles_set';
  $user_fields['roles']['fieldtool_user_standard_transformations_callback'] = TRUE;
  $user_fields['roles']['weight'] = -5;

  $user_fields['created']['label'] = t('Creation date');
  $user_fields['created']['stored'] = TRUE;
  $user_fields['created']['fieldtool_user_standard_date_as_timestamp'] = TRUE;
  $user_fields['created']['fieldtool_user_standard_transformations_callback'] = TRUE;
  $user_fields['created']['weight'] = 5;

  $user_fields['access']['label'] = t('Last access');
  $user_fields['access']['stored'] = TRUE;
  $user_fields['access']['fieldtool_user_standard_date_as_timestamp'] = TRUE;
  $user_fields['access']['fieldtool_user_standard_transformations_callback'] = TRUE;
  $user_fields['access']['weight'] = 5;

  $user_fields['login']['label'] = t('Last login');
  $user_fields['login']['stored'] = TRUE;
  $user_fields['login']['fieldtool_user_standard_date_as_timestamp'] = TRUE;
  $user_fields['login']['fieldtool_user_standard_transformations_callback'] = TRUE;
  $user_fields['login']['weight'] = 5;

  $user_fields['status']['label'] = t('Active');
  $user_fields['status']['stored'] = TRUE;
  $user_fields['status']['getter callback'] = 'fieldtool_user_int_to_boolean';
  $user_fields['status']['setter callback'] = 'fieldtool_user_boolean_to_int';
  $user_fields['status']['fieldtool_user_standard_transformations_callback'] = TRUE;
  $user_fields['status']['weight'] = 6;

  foreach ($user_fields as $field_name => &$field_info) {
    if (!empty($field_info['fieldtool_user_standard_verbatim'])) {
      unset($field_info['fieldtool_user_standard_verbatim']);
      $field_info['getter callback'] = $getter_callback;
      $field_info['setter callback'] = $setter_callback;
    }

    if (empty($field_info['stored'])) {
      unset($field_info['getter callback']);
    }
    if (!empty($field_info['stored'])) {
      // All non-generated standard fields are exportable as is.
      if (!isset($field_info['export callback'])) {
        $field_info['export callback'] = $getter_callback;
      }
      if (!isset($field_info['import callback'])) {
        $field_info['import callback'] = $setter_callback;
      }
    }

    if (!empty($field_info['fieldtool_user_standard_transformations_callback'])) {
      $field_info['transformations slot property callback'] =
        'fieldtool_user_' . $field_name . '_transformations_slot_property';
      unset($field_info['fieldtool_user_standard_transformations_callback']);
    }
  }

  return array('user' => $user_fields);
}

/**
 * The 'status' field stores 0 or 1 even if those values are supposed to mean
 * TRUE or FALSE. In the name of type correctness, we'll perform a conversion
 * to and from boolean in the callbacks.
 */
function fieldtool_user_int_to_boolean($user, $field_name, $field_info) {
  return empty($user->$field_name) ? FALSE : TRUE;
}

/**
 * @see fieldtool_int_to_boolean()
 */
function fieldtool_user_boolean_to_int(&$user, $field_name, $value, $field_info) {
  $user->$field_name = empty($value) ? 0 : 1;
}


function fieldtool_user_pass_set(&$user, $field_name, $value, $field_info) {
  $user->pass = $value;
  $user->fieldtool_pass_plaintext = TRUE;
}

function fieldtool_user_pass_md5_get($user, $field_name, $field_info) {
  if (!empty($user->fieldtool_pass_plaintext)) {
    return md5($user->pass);
  }
  return $user->pass;
}

/**
 * Accepts any kind of iterator as $value, not just plain arrays.
 */
function fieldtool_user_roles_set(&$user, $field_name, $value, $field_info) {
  $user->roles = array();
  foreach ($value as $rid => $rolename) {
    $user->roles[$rid] = $rolename;
  }
}


//
// Transformations slot property callbacks.
//

function fieldtool_user_uid_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'description':
      return t('The integer that uniquely identifies the user in this Drupal installation.');

    case 'required':
      return FALSE;

    case 'expectedType':
      return 'php:type:integer';
  }
}

function fieldtool_user_name_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'description':
      return t('The unique user name.');

    case 'expectedType':
      return 'php:type:string';
  }
}

function fieldtool_user_mail_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'description':
      return t('User\'s email address.');

    case 'expectedType':
      return 'php:type:string';
  }
}

function fieldtool_user_pass_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'description':
      return t('The unencrypted password of the user. Once stored, this plaintext version of the password cannot be recovered anymore.');

    case 'expectedType':
      return 'php:type:string';
  }
}

function fieldtool_user_pass_md5_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'description':
      return t('An MD5 hash of the user\'s password. The original password cannot be recovered from this field.');

    case 'required':
      return FALSE;

    case 'expectedType':
      return 'php:type:string';
  }
}

function fieldtool_user_roles_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'description':
      return t('The list of roles that are assigned to the user. Each key is a role id (rid) and the corresponding value is the unique name of the role.');

    case 'required':
      return FALSE;

    case 'expectedType':
      return 'transformations:list<php:type:string>';
  }
}

function fieldtool_user_created_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'description':
      return t('Timestamp for when the user was created.');

    case 'required':
      return FALSE;

    case 'expectedType':
      return 'date:timestamp';
  }
}

function fieldtool_user_access_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'description':
      return t('Timestamp for the most recent time the user accessed the site.');

    case 'required':
      return FALSE;

    case 'expectedType':
      return 'date:timestamp';
  }
}

function fieldtool_user_login_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'description':
      return t('Timestamp for the user\'s last login.');

    case 'required':
      return FALSE;

    case 'expectedType':
      return 'date:timestamp';
  }
}

function fieldtool_user_status_transformations_slot_property($field_info, $property_key) {
  switch ($property_key) {
    case 'description':
      return t('Whether the user is active (TRUE) or blocked (FALSE).');

    case 'required':
      return FALSE;

    case 'expectedType':
      return 'php:type:boolean';
  }
}
