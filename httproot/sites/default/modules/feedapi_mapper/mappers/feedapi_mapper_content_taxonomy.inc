<?php
// $Id$
/**
 * On behalf implementation of feed element mapper for content_taxonomy.module.
 */

/**
 * Implementation of hook_feedapi_mapper().
 * Maps feed elements to content taxonomy values.
 * @param $op
 *   'list' or 'map'
 * @param $node
 *   Feed item node to map on.
 * @param $field_name
 *   Name of field to map to.
 * @param $feed_element
 *   Feed item element to map from. Parameter only present on $op = 'map'
 * @param @sub_field
 *   Subfield on field to map to. Parameter only present on $op = 'map'.
 *   This parameter will depend on if the hook implementation returns a subfield on
 *   $op = 'list'.
 *
 * Return key => name array of sub fields on 'list'.
 * If only one field and no subfields are available for this content type return TRUE.
 * If no fields available for this content type return FALSE.
 * Options are necessary because a field like
 * e. g. "taxonomy" might have more than one slot for information - e. g. vocabularies.
 *
 */
function content_taxonomy_feedapi_mapper($op, $node, $feed_element = array(), $field_name = '', $sub_field = '') {
  $field_def = content_fields($field_name, $node->type);
  if ($op == 'describe') {
    // High level description that shows up in the mapper help area
    return t('Maps a string or an array of strings to content taxonomy terms. Chose a vocabulary from sub options.');
  }
  else if ($op == 'list') {
    // Listing of sub-fields we would like to map to. This displays the mappable fields in the drop-down.
    $info = content_types($node->type);
    $fields = array();
    if (@count($info['fields'])) {
      foreach ($info['fields'] as $field_name => $field) {
        if (in_array($field['type'], array('content_taxonomy'))) {
          $fields[$field_name] = $sub_fields;
        }
      }
    }
    if (count($fields)) {
      return $fields;
    }
    return FALSE;
  }
  else if ($op == 'map') {
  // The actual mapping process
    if (is_string($feed_element)) {
      $feed_element = array($feed_element);
    }
    if (is_array($feed_element)) {
      // Vocabulary ID assigned to the field
      $vid = $field_def['vid'];

      // Start by saving term to the core taxonomy if we are supposed to do that
      if ($field_def['save_term_node']) {
        if (!isset($node->taxonomy) || !is_array($node->taxonomy)) {
          $node->taxonomy = array();
        }
        $node->taxonomy = array_merge($node->taxonomy, _content_taxonomy_mapper_create_terms($feed_element, $vid));
      }

      // Cycle through each term
      foreach($feed_element as $term_name) {
        $term = _content_taxonomy_get_term_by_name_and_vid($term_name, $vid);
        if (!isset($node->$field_name)) {
          $node->$field_name = array();
        }

        // if the field can have multiple values, append the new values
        if ($field_def['multiple']) {
          $field_arr = $node->$field_name;
          $field_arr[] = array('value' => $term->tid);
          $node->$field_name = $field_arr;
        }
        else {
          // single values overwrite any old values
          $node->$field_name = array(array('value' => $term->tid));
        }
      }
    }
    return $node;
  }
}

function _content_taxonomy_get_term_by_name_and_vid($term_name, $vid) {
  $sql = db_rewrite_sql("SELECT t.tid, t.* FROM {term_data} t WHERE LOWER(t.name) = LOWER('%s') and t.vid = %d", 't', 'tid');
  $db_result = db_query($sql, trim($term_name), $vid);
  $result = array();
  while ($term = db_fetch_object($db_result)) {
    $result[] = $term;
  }

  if (count($result) < 1) {
    return NULL;
  }

  return $result[0];
}

/**
 * Creates terms from keywords, borrowed from yahoo_terms module.
 * @todo apply http://drupal.org/node/354657#comment-1184004
 *
 * @param keywords
 *    Keywords to be added
 * @param vid
 *    Vocabulary ID associated with this taxonomy
 */
function _content_taxonomy_mapper_create_terms($keywords, $vid, $static = FALSE) {
  if ($vid == FALSE) {
    return FALSE;
  }
  $tids = array();
  if (is_array($keywords)) {
    foreach ($keywords as $term) {
      $curr_terms = taxonomy_get_term_by_name($term);
      if (count($curr_terms) != 0) {
        foreach ($curr_terms as $curr_term) {
          if ($curr_term->vid == $vid) {
            $tids[$curr_term->tid] = $curr_term;
          }
        }
        if (count($tids) == 0 and $static == FALSE) {
          $new_term['name'] = $term;
          $new_term['vid'] = $vid;
          taxonomy_save_term($new_term);
          $tids[$new_term['tid']] = taxonomy_get_term($new_term['tid']);
          unset($new_term);
        }
      }
      else {
        if ($static == FALSE) {
          $new_term['name'] = $term;
          $new_term['vid'] = $vid;
          taxonomy_save_term($new_term);
          $tids[$new_term['tid']] = taxonomy_get_term($new_term['tid']);
          unset($new_term);
        }
      }
    }
  }
  return $tids;
}

