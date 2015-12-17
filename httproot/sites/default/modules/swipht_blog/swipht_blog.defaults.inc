<?php

/**
 * Helper to implementation of hook_content_default_fields().
 */
function _swipht_blog_content_default_fields() {
  $fields = array();

  // Exported field: field_blog_author_user
  $fields[] = array(
    'field_name' => 'field_blog_author_user',
    'type_name' => 'blog',
    'display_settings' => array(
      'weight' => '-3',
      'parent' => '',
      'label' => array(
        'format' => 'inline',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'userreference',
    'required' => '0',
    'multiple' => '0',
    'module' => 'userreference',
    'active' => '1',
    'referenceable_roles' => array(
      '2' => 0,
      '3' => 0,
    ),
    'referenceable_status' => '1',
    'advanced_view' => '',
    'advanced_view_args' => '',
    'widget' => array(
      'autocomplete_match' => 'contains',
      'size' => '60',
      'reverse_link' => 0,
      'default_value' => NULL,
      'default_value_php' => 'global $user;

return array(0 => array(\'uid\' => $user->uid));',
      'label' => 'Author',
      'weight' => '-3',
      'description' => 'Please select the user account to define as the author of this post.',
      'type' => 'userreference_autocomplete',
      'module' => 'userreference',
    ),
  );

  // Exported field: field_blog_postdate
  $fields[] = array(
    'field_name' => 'field_blog_postdate',
    'type_name' => 'blog',
    'display_settings' => array(
      'weight' => '-4',
      'parent' => '',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'short',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'long',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'long',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'long',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'datetime',
    'required' => '1',
    'multiple' => '0',
    'module' => 'date',
    'active' => '1',
    'granularity' => array(
      'year' => 'year',
      'month' => 'month',
      'day' => 'day',
      'hour' => 'hour',
      'minute' => 'minute',
    ),
    'timezone_db' => 'UTC',
    'tz_handling' => 'site',
    'todate' => '',
    'repeat' => 0,
    'repeat_collapsed' => '',
    'default_format' => 'long',
    'widget' => array(
      'default_value' => 'now',
      'default_value_code' => '',
      'default_value2' => 'blank',
      'default_value_code2' => '',
      'input_format' => 'm/d/Y - g:ia',
      'input_format_custom' => '',
      'increment' => '1',
      'text_parts' => array(),
      'year_range' => '-3:+3',
      'label_position' => 'above',
      'label' => 'Post Date',
      'weight' => '-4',
      'description' => 'Select the date and time the post was created on.',
      'type' => 'date_popup',
      'module' => 'date',
    ),
  );

  // Exported field: field_blog_tags
  $fields[] = array(
    'field_name' => 'field_blog_tags',
    'type_name' => 'blog',
    'display_settings' => array(
      'weight' => '-1',
      'parent' => '',
      'label' => array(
        'format' => 'inline',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'link',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'link',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'link',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'content_taxonomy',
    'required' => '0',
    'multiple' => '1',
    'module' => 'content_taxonomy',
    'active' => '1',
    'save_term_node' => 1,
    'vid' => '1',
    'parent' => '0',
    'parent_php_code' => '',
    'depth' => '',
    'widget' => array(
      'new_terms' => 'insert',
      'extra_parent' => '0',
      'maxlength' => '255',
      'active_tags' => NULL,
      'default_value' => NULL,
      'default_value_php' => NULL,
      'label' => 'Tags',
      'weight' => '-1',
      'description' => 'Select one or more terms to classify your content. Enter multiple terms separated by a "," comma.',
      'type' => 'content_taxonomy_autocomplete',
      'module' => 'content_taxonomy_autocomplete',
    ),
  );

  // Translatables
  array(
    t('Author'),
    t('Post Date'),
    t('Tags'),
  );

  return $fields;
}
