<?php

/**
 * Implementation of hook_content_default_fields().
 */
function swipht_blog_content_default_fields() {
  module_load_include('inc', 'swipht_blog', 'swipht_blog.defaults');
  $args = func_get_args();
  return call_user_func_array('_swipht_blog_content_default_fields', $args);
}

/**
 * Implementation of hook_node_info().
 */
function swipht_blog_node_info() {
  module_load_include('inc', 'swipht_blog', 'swipht_blog.features.node');
  $args = func_get_args();
  return call_user_func_array('_swipht_blog_node_info', $args);
}

/**
 * Implementation of hook_views_default_views().
 */
function swipht_blog_views_default_views() {
  module_load_include('inc', 'swipht_blog', 'swipht_blog.features.views');
  $args = func_get_args();
  return call_user_func_array('_swipht_blog_views_default_views', $args);
}
