<?php
// $Id: feedapi_tagger.inc,v 1.1 2009/09/22 22:54:48 alexb Exp $
/**
 * @file:
 * Internal functions for feed item tagging.
 */

/**
 * Main entry point.
 *
 * @param $feed
 *   FeedAPI - style feed object.
 */
function _feedapi_tagger_tag($feed) {

  if (isset($feed->items) && is_array($feed->items)) {
    if ($feed->nid) {
      $feed_node = node_load($feed->nid);
      $settings = feedapi_get_settings($feed_node->type, $feed_node->vid);

      foreach ($feed->items as $i => $item) {
        $feed->items[$i] = _feedapi_tagger_tag_item($item, $settings);
      }
    }
  }
}

/**
 * Tags a single item.
 */
function _feedapi_tagger_tag_item($item, $settings) {
  $vid = $settings['parsers']['feedapi_tagger']['vocabulary'];
  if (empty($vid)) {
    drupal_set_message(t('No vocabulary defined for your FeedAPI tagger, check your feed settings on content type and on node.'), 'error');
    return;
  }
  $tags = _feedapi_tagger_tag_text($item->title, $vid);
  $tags += _feedapi_tagger_tag_text($item->description, $vid);
  if (!is_array($item->options->auto_tags)) {
    $item->options->auto_tags = array();
  }
  if (!is_array($item->options->auto_tags_tids)) {
    $item->options->auto_tags_tids = array();
  }
  $item->options->auto_tags = array_merge($item->options->auto_tags, $tags);
  $item->options->auto_tags_tids = array_merge($item->options->auto_tags_tids, array_keys($tags));
  return $item;
}

/**
 * Finds tags in text.
 */
function _feedapi_tagger_tag_text($text, $vid) {
  $tags = array();
  $text = strip_tags($text);
  $words = _feedapi_tagger_split($text); 

  for ($pos = 0; $pos < count($words); $pos++) {
    $term_names = $term_tids = $terms = array();
    $word = $words[$pos];

    // 1) get all terms that start with this word.
    $result = db_query('SELECT tid, name FROM {term_data} WHERE name RLIKE "^%s[[:>:]]"', $word); // If this query gets too slow, we need to build a word index.
    while ($term = db_fetch_object($result)) {
      $term_tids[$term->name] = $term->tid;
      $term_names[$term->name] = $term->name;
    }
    // Skip to immediate loop if no results.
    if (!count($term_tids)) {
      continue;
    }

    // 2) Order result by length desc.
    $sorted = FALSE;
    $sorted_terms = array_values($term_names);
    $terms = array();
    while (!$sorted) {
      $sorted = TRUE;
      for ($i = 0; $i < count($sorted_terms) - 1; $i++) {
        if (strlen($sorted_terms[$i]) < strlen($sorted_terms[$i+1])) {
          $tmp = $sorted_terms[$i+1];
          $sorted_terms[$i+1] = $sorted_terms[$i];
          $sorted_terms[$i] = $tmp;
          $sorted = FALSE;
        }
      }
    }
    foreach ($sorted_terms as $term) {
      $terms[$term_tids[$term]] = $term_names[$term];
    }

    // 4) Determine which tag fits by looking at next word in text.
    foreach ($terms as $tid => $term) {
      $term_words = _feedapi_tagger_split($term);
      // Iterate through tag words backwards, skip to the start of the outmost loop
      // if we find a mismatch.
      for ($i = count($term_words) - 1; $i >= 0; $i--) {
        if ($term_words[$i] != $words[$pos+$i]) {
          continue 2; // Skips to start of foreach ($terms as $term) {
        }
      }
      // We have a full match, add to result array, shift $pos pointer to the next word 
      // after this term and break out of the immediate loop.
      $tags[$tid] = $term;
      $pos = $pos + count($term_words);
      break; // Skips to start of while (!$sorted) {
    }
  }

  return $tags;
}

/**
 * Split text into words.
 */
function _feedapi_tagger_split($text) {
  if (module_exists('search')) {
    $result = search_index_split(search_simplify($text));
  } 
  else {
    $result = preg_split("/[\s,.:\-\(\)\[\]{}*\/]+/", $text);
  }
  // search_index_split can return NULL
  if ($result) {
    return array_filter($result);
  }
}


