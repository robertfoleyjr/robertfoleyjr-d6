<?php

/**
 * Helper to implementation of hook_node_info().
 */
function _swipht_blog_node_info() {
  $items = array(
    'blog' => array(
      'name' => t('Blog entry'),
      'module' => 'features',
      'description' => t('A blogs provides commentary or news on a particular subject; may be used as a personal online diary of your daily life.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
