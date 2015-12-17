<?php 
/**
 * @file
 * Views style plugins to render nodes in the XML data format.
 *
 * @see views_plugin_style_xml.inc
 * @ingroup views_plugins
 */


/**
 * Implementation of hook_views_plugin().
 */
function swipht_blog_views_plugins() {
  $path = drupal_get_path('module', 'swipht_blog'); 
	return array(
	   'module' => 'swipht_blog',
      'style'   => array(                 // declare the views_xml_* style plugins
        'views_blog_xml_style' => array(
          'title'           => t('Blog XML data document'),
	        'path'            => $path,
          'help'            => t('Display a view as an XML document.'),
          'handler'         => 'views_plugin_style_blog_rss',
          'uses row plugin' => FALSE,
          'uses fields'     => TRUE,
          'uses options'    => TRUE,
	        'type'            => 'normal',
	        'help_topic'      => 'style-xml',
	        'even empty'      => TRUE
       ),
     ),
    );
}