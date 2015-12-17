<?php
// $Id: template.php,v 1.16.2.1 2009/02/25 11:47:37 goba Exp $

/**
 * Sets the body-tag class attribute.
 *
 * Adds 'sidebar-left', 'sidebar-right' or 'sidebars' classes as needed.
 */
function phptemplate_body_class($left, $right) {
  if ($left != '' && $right != '') {
    $class = 'sidebars';
  }
  else {
    if ($left != '') {
      $class = 'sidebar-left';
    }
    if ($right != '') {
      $class = 'sidebar-right';
    }
  }

  if (isset($class)) {
    print ' class="'. $class .'"';
  }
}

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function phptemplate_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    return '<div class="breadcrumb">'. implode(' &raquo; ', $breadcrumb) .'</div>';
  }
}

/**
 * Allow themable wrapping of all comments.
 */
function phptemplate_comment_wrapper($content, $node) {
  if (!$content || $node->type == 'forum') {
    return '<div id="comments">'. $content .'</div>';
  }
  else {
    return '<div id="comments"><h2 class="comments">'. t('Comments') .'</h2>'. $content .'</div>';
  }
}


/**
 * Override or insert PHPTemplate variables into the node processor.
 */
function phptemplate_preprocess_node(&$vars) {
  
  /**
   * Register node templates if they don't load
 */
  /*
  $node_templates = array();
  $node_types = node_get_types('types');
  foreach($node_types as $type) {
    $template_filename = 'node-'.$type->type;
    $node_templates[] = $template_filename;
  }
  $vars['template_files'] = array_merge((array) $node_templates, $vars['template_files']);
*/
  
  
    
    /** Page specific functions based on the path **/

  /* 'portfolio/clients/%' */

  if(drupal_is_front_page()) {
      drupal_add_css(drupal_get_path('theme','carbon').'/css/slideshow.css', 'theme');
      drupal_add_js(drupal_get_path('theme','carbon').'/js/slideshow.js', 'theme');
  }


}



/**
 * Override or insert PHPTemplate variables into the templates.
 */
function phptemplate_preprocess_page(&$vars) {
  $vars['tabs2'] = menu_secondary_local_tasks();

  // Hook into color.module
  if (module_exists('color')) {
    _color_page_alter($vars);
  }
  
  // adding page alias function to correlate path alias to page templates
  if (module_exists('path')) {
    $alias = drupal_get_path_alias(str_replace('/edit','',$_GET['q']));
    if ($alias != $_GET['q']) {
      $suggestions = array();
      $template_filename = 'page';
      foreach (explode('/', $alias) as $path_part) {
        $template_filename = $template_filename . '-' . $path_part;
        $suggestions[] = $template_filename;
      }
      $vars['template_files'] = array_merge((array) $suggestions, $vars['template_files']);
    }
  }

  // Add content type specific page template suggestion
  if  ($node = menu_get_object()) {        
    $vars['template_files'][] = 'page' . '-' . str_replace('_', '-', $node->type);    
  }
  
  /**
   * This set of expressions provides a template alias for the login page
 */  
  /*
   
    global $user;
      if (arg(0) == 'user'){
        if ($user->uid == 0) {
          $vars['template_file'] = 'page-login';
        }
        elseif (arg(1) == 'login') {
          $vars['template_file'] = 'page-login';
        }
      }
  
  */


  /** register specific paths **/
  /** not this expects the $alias be set **/
  if (stristr($alias, 'portfolio/client/') !== FALSE) {
      $vars['template_files'][] = 'page-portfolio-client-index';
  }
  if (stristr($alias, 'portfolio/employer/') !== FALSE) {
      $vars['template_files'][] = 'page-portfolio-employer-index';
  }
  if (stristr($alias, 'portfolio/industry/') !== FALSE) {
      $vars['template_files'][] = 'page-portfolio-industry-index';
  }
  if (stristr($alias, 'portfolio/medium/') !== FALSE) {
      $vars['template_files'][] = 'page-portfolio-medium-index';
  }
  if (stristr($alias, 'portfolio/skill/') !== FALSE) {
      $vars['template_files'][] = 'page-portfolio-skill-index';
  }
  if (stristr($alias, 'portfolio/service/') !== FALSE) {
      $vars['template_files'][] = 'page-portfolio-service-index';
  }
  if (stristr($alias, 'portfolio/technology/') !== FALSE) {
      $vars['template_files'][] = 'page-portfolio-technology-index';
  }

}

/**
 * Returns the rendered local tasks. The default implementation renders
 * them as tabs. Overridden to split the secondary tasks.
 *
 * @ingroup themeable
 */
function phptemplate_menu_local_tasks() {
  return menu_primary_local_tasks();
}

function phptemplate_comment_submitted($comment) {
  return t('!datetime — !username',
    array(
      '!username' => theme('username', $comment),
      '!datetime' => format_date($comment->timestamp)
    ));
}

function phptemplate_node_submitted($node) {
  return t('!datetime — !username',
    array(
      '!username' => theme('username', $node),
      '!datetime' => format_date($node->created),
    ));
}

/**
 * Generates IE CSS links for LTR and RTL languages.
 */
function phptemplate_get_ie_styles() {
  global $language;

  $iecss = '<link type="text/css" rel="stylesheet" media="all" href="'. base_path() . path_to_theme() .'/css/fix-ie.css" />';
  if ($language->direction == LANGUAGE_RTL) {
    $iecss .= '<style type="text/css" media="all">@import "'. base_path() . path_to_theme() .'/css/fix-ie-rtl.css";</style>';
  }

  return $iecss;
}






/**
 * Utility Functions
 * Set of utility functions used for all requests
 */

/* returns the root URL of the system */
function phptemplate_get_root_url() {
  $root = 'http://';
  if ($_SERVER['SERVER_PORT'] != 80) {
    $root = 'https://';
  }
  
  $site_root = rtrim($_SERVER['SERVER_NAME'], '/');
  $path = $root.$site_root;
    preg_match('(^/([^/]+))', $_SERVER['REQUEST_URI'], $uri_matches);
    $doc_root = rtrim($_SERVER['DOCUMENT_ROOT'], '/');
    if(isset($uri_matches[1])){
        $sub_site_path = $doc_root.'/sites/'.$site_root.'.'.str_replace('/', '', $uri_matches[1]);
        if(is_dir($sub_site_path)) {
            $path = $root.$site_root.'/'.str_replace('/', '', $uri_matches[1]);
        }
    }
  
  return $path;

}



set_error_handler('takpar_error_handler');

function takpar_error_handler($errno, $message, $filename, $line, $context) {
    $aBypass = array(
        'Attempt to modify property of non-object',
    );
    foreach ($aBypass as $sTerm) {
        if (false !== strpos($message, $sTerm)) {
            return;
        }
    }
    return drupal_error_handler($errno, $message, $filename, $line, $context);
}