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

  $iecss = '<link type="text/css" rel="stylesheet" media="all" href="'. base_path() . path_to_theme() .'/fix-ie.css" />';
  if ($language->direction == LANGUAGE_RTL) {
    $iecss .= '<style type="text/css" media="all">@import "'. base_path() . path_to_theme() .'/fix-ie-rtl.css";</style>';
  }

  return $iecss;
}



/**
 *
 * !!!!!!!! START Navigation Side-Menu Functions !!!!!!!!
 *
 **/



/**
 *
 *  Custom theme functions
 *  LEFT MENU FUNCTIONS
 *
 */


/**
 * Accepts the nid and returns the associated menu item
 */

function phptemplate_menu_item_load($nid, $menu_name = "") {

  $result = array();
  
  if (strlen($menu_name) > 0) {
    $menu = db_fetch_array(db_query("SELECT ml.* FROM {menu_links} AS ml, {node} AS n WHERE ml.link_path = concat('node/', n.nid) AND ml.menu_name = '%s' AND n.nid = %d", $menu_name, $nid));
  } else {
    $menu = db_fetch_array(db_query("SELECT ml.* FROM {menu_links} AS ml, {node} AS n WHERE ml.link_path = concat('node/', n.nid) AND n.nid = %d", $nid));
  }
  if (!empty($menu)) {
    $result = $menu;
  }

  return $result;
}



function _phptemplate_get_parentmlid($mlid, $menu_name = "") {
  $parent = 0;
  $targetmlid = 0;
  
  if (strlen($menu_name) > 0) {
    $menu = db_fetch_array(db_query("SELECT m.* FROM {menu_links} AS m WHERE mlid = %d AND menu_name = '%s'", $mlid, $menu_name));
  } else {
    $menu = db_fetch_array(db_query("SELECT m.* FROM {menu_links} AS m WHERE mlid = %d", $mlid));
  }
  
  if (!empty($menu)) {
    if ($menu['p1'] != 0) {
      $parent = $menu['p1'];
    } else if ($menu['plid'] == 0 && $menu['depth'] == 1) {
      $parent = $mlid;
    } else {
      $parent = _phptemplate_get_parentmlid($menu['mlid'], $menu_name);
    }
  }

  return $parent;
}


/**
 * Node menu item location resolution
 */

function phptemplate_get_parentmlid($nid, $menu_name = "") {

  $parent = 0;
  $mlid = 0;

  $menu = phptemplate_menu_item_load($nid, $menu_name);

  if (!empty($menu)) {
    $mlid = $menu['mlid'];
    $parent = _phptemplate_get_parentmlid($mlid, $menu_name);
  }

  return $parent;
}





/**
 * Get menu item children
 *
 * @param $mlid
 *   The menu item id
 * @return
 *   An array of menu item ids for all children menu items.
 */
function phptemplate_menu_get_child_by_menu_item($mlid, $menu_name = "") {
  $children = array();
  
  if (strlen($menu_name) > 0) {
    $childresult = db_query("SELECT mlid FROM {menu_links} WHERE plid = %d AND menu_name = '%s' AND hidden <> 1", $mlid, $menu_name);  
  } else {
    $childresult = db_query("SELECT mlid FROM {menu_links} WHERE plid = %d AND hidden <> 1", $mlid);  
  }
  while ($child_row = db_fetch_array($childresult)) {
    $children[] = $child_row['mlid'];
    
    $childarr = phptemplate_menu_get_child_by_menu_item($child_row, $menu_name);
    if (!empty($childarr)) {
      $children = array_merge($childarr, $children);
    }
  }
  
  return $children;
}

function phptemplate_remove_array_value($needle, $haystack) {
  
  $resultarray = array();
  $targetkey = (isset($needle)) ? array_search($needle, $haystack) : FALSE;
  
  if ($targetkey != FALSE) {
    
      foreach ($haystack as $itemkey => $itemvalue) {
        if ($itemkey != $targetkey) {
          $resultarray[] = $itemvalue;
        }
      }
    
  }
  else {
    $resultarray = $haystack;
  }
  
  return $resultarray;
}

/**
 * Get the data structure representing a menu tree, based on the mlid passed.
 *
 * The tree order is maintained by storing each parent in an individual
 * field, see http://drupal.org/node/141866 for more.
 *
 * @param $mlid
 *   The menu item id
 * @return
 *   An array of menu links, in the order they should be rendered. The array
 *   is a list of associative arrays -- these have two keys, link and below.
 *   link is a menu item, ready for theming as a link. Below represents the
 *   submenu below the link if there is one, and it is a subtree that has the
 *   same structure described for the top-level array.
 */
function phptemplate_menu_tree_menu_item_data($mlid, $isexpanded = FALSE, $menu_name = "") {
  
  $tree = array();
  $args = array();
  
  $menulink = db_fetch_array(db_query("SELECT * FROM {menu_links} WHERE mlid = %d", $mlid));
  $menurouter = menu_get_item($menulink["link_path"]);
  $pageitem = menu_get_item();
  
  // Load the menu item corresponding to the current page.
  //if ($item = menu_get_item()) {
    // Generate a cache ID (cid) specific for this page.
    $cid = 'links:'. $mlid .':page-cid:'. $menurouter['href'] .':'. (int)$menurouter['access'];
    
    /*
    if (!isset($tree[$cid])) {
      // If the static variable doesn't have the data, check {cache_menu}.
      $cache = cache_get($cid, 'cache_menu');
      if ($cache && isset($cache->data)) {
        // If the cache entry exists, it will just be the cid for the actual data.
        // This avoids duplication of large amounts of data.
        $cache = cache_get($cache->data, 'cache_menu');
        if ($cache && isset($cache->data)) {
          $data = $cache->data;
        }
      }
      // If the tree data was not in the cache, $data will be NULL.
      if (!isset($data)) {
    */
      
      
      
        // Build and run the query, and build the tree.
        if ($menurouter['access']) {
        
            $parents = array();
            
          if ($isexpanded) {
          
            
          
            //$parents = db_fetch_array(db_query("SELECT p1, p2, p3, p4, p5, p6, p7, p8 FROM {menu_links} WHERE menu_name = '%s' AND p1 = %d AND hidden <> 1", $menulink['menu_name'], $mlid));
            
            $parents[] = $mlid;
            if (strlen($menu_name) > 0) {
              $parents = db_fetch_array(db_query("SELECT p1, p2, p3, p4, p5, p6, p7, p8 FROM {menu_links} WHERE menu_name = '%s' AND plid= %d AND hidden <> 1", $menu_name, $mlid));
            } else {
              $parents = db_fetch_array(db_query("SELECT p1, p2, p3, p4, p5, p6, p7, p8 FROM {menu_links} WHERE menu_name = '%s' AND plid= %d AND hidden <> 1", $menulink['menu_name'], $mlid));
            }
            if (!empty($parents) && is_array($parents)) {
              $parents = array_unique(array_values($parents));
              rsort($parents);
              
              // remove 0 to clear the root
              if (array_key_exists(0, $parents)) {
                array_pop($parents);
              }
              $args = $parents = array_unique(array_values($parents));
            }
            
            // Stage One: Get top Link elements
            $children = phptemplate_menu_get_child_by_menu_item($mlid, $menu_name);
            
            if (!empty($parents) && is_array($parents) && !empty($args) && is_array($args)) {
              $args = $parents = array_unique(array_values($parents));
            }
            
            if (!empty($children) && is_array($children) && !empty($args) && is_array($args)) {
              $args = array_unique(array_merge($args, $children));
            }
            
            if (!empty($args)) {
              $placeholders = implode(', ', array_fill(0, count($args), '%d'));
            }
            else {
              $placeholders = "";
            }
            
          }
          else {
            
            // Check whether a menu link exists that corresponds to the current path.
            if (!empty($pageitem)) {
              $args = array($pageitem['href']);
              $placeholders = "'%s'";
              
              $pageparents = db_fetch_array(db_query("SELECT mlid, p1, p2, p3, p4, p5, p6, p7, p8 FROM {menu_links} WHERE menu_name = '". $menulink['menu_name'] ."' AND plid <> ". $mlid ." AND link_path IN ( ". $placeholders ." )", $args));
              
              if ($pageparents == FALSE) {
                $pageparents = db_fetch_array(db_query("SELECT mlid, p1, p2, p3, p4, p5, p6, p7, p8 FROM {menu_links} WHERE menu_name = '". $menulink['menu_name'] ."' AND plid = ". $mlid  ." AND link_path IN ( ". $placeholders ." )", $args));
              }
            }
            
            $placeholders = "";
            
              //$parents[] = $mlid;
            $parents = db_fetch_array(db_query("SELECT p1, p2, p3, p4, p5, p6, p7, p8 FROM {menu_links} WHERE menu_name = '". $menulink['menu_name'] ."' AND plid = ". $mlid ." AND hidden <> 1"));
            
            if (!is_array($parents)) {
              $parents = array();
            }
            
            if (is_array($pageparents) && is_array($parents)) {
              $parents = array_merge($parents, $pageparents);
            }
            
            if (is_array($parents)) {
              
              $parents = array_unique(array_values($parents));
              rsort($parents);
              
              // remove 0 to clear the root
              $parents = phptemplate_remove_array_value(0, $parents);
              
              $args = $parents; //= array_unique(array_values($parents));
            }
            
            if (!empty($args)) {
              $placeholders = implode(', ', array_fill(0, count($args), '%d'));
            }
            else {
              $placeholders = "";
            }
            
              $expanded = variable_get('menu_expanded', array());
              // Check whether the current menu has any links set to be expanded.
              if (in_array($menulink['menu_name'], $expanded)) {
                // Collect all the links set to be expanded, and then add all of
                // their children to the list as well.
                do {
                  $result = db_query("SELECT mlid FROM {menu_links} WHERE menu_name = '". $menulink['menu_name'] ."' AND plid IN (". $placeholders .") AND mlid NOT IN (". $placeholders .")", array_merge($args, $args));
                  $num_rows = FALSE;
                  while ($item = db_fetch_array($result)) {
                    $args[] = $item['mlid'];
                    $num_rows = TRUE;
                  }
                  $placeholders = implode(', ', array_fill(0, count($args), '%d'));
                } while ($num_rows);
              }
              //array_unshift($args, $menulink['menu_name']);
            
          }
        }
        else {
          // Show only the top-level menu items when access is denied.
          $parents_result = db_query("SELECT p1, p2, p3, p4, p5, p6, p7, p8 FROM {menu_links} WHERE menu_name = '%s' AND plid = %d AND hidden <> 1", $menulink['menu_name'], $mlid);
            while ($row = db_fetch_array($parents_result)) {
              $parents[] = $row['mlid'];
            }
          $args = $parents;
          if (!empty($args)) {
              $placeholders = implode(', ', array_fill(0, count($args), '%d'));
            }
          else {
              $placeholders = "";
            }
          
        }
        
        if (!empty($args)) {
        
        $data['tree'] = menu_tree_data(db_query("
          SELECT m.load_functions, m.to_arg_functions, m.access_callback, m.access_arguments, m.page_callback, m.page_arguments, m.title, m.title_callback, m.title_arguments, m.type, m.description, ml.*
          FROM {menu_links} ml LEFT JOIN {menu_router} m ON m.path = ml.router_path
          WHERE ml.menu_name = '". $menulink['menu_name'] ."' AND ml.plid IN (". $placeholders .")
          ORDER BY p1 ASC, p2 ASC, p3 ASC, p4 ASC, p5 ASC, p6 ASC, p7 ASC, p8 ASC, p9 ASC", $args), $parents);
        
        }
        else {
          $data['tree'] = array();
        }
        
        $data['node_links'] = array();
        menu_tree_collect_node_links($data['tree'], $data['node_links']);
        /*
        // Cache the data, if it is not already in the cache.
        $tree_cid = _menu_tree_cid($mlid, $data);
        if (!cache_get($tree_cid, 'cache_menu')) {
          cache_set($tree_cid, $data, 'cache_menu');
        }
        // Cache the cid of the (shared) data using the page-specific cid.
        cache_set($cid, $tree_cid, 'cache_menu');
        
      }
      */
      // Check access for the current user to each item in the tree.
      menu_tree_check_access($data['tree'], $data['node_links']);
      $tree[$cid] = $data['tree'];
    //}
    return $tree[$cid];
  //}

  //return array();
}


/**
 * Render a menu tree based on a seed menu item id.
 *
 * The tree is expanded if the isexpanded option attribute is TRUE
 *
 * @param $mlid
 *   The menu id
 * @return
 *   The rendered HTML of the menu.
 */
function phptemplate_menu_tree_by_menu_item($mlid = NULL, $isexpanded = FALSE, $menu_name = "") {
  static $menu_output = array();
   
  if (!is_null($mlid)) {
    if (!isset($menu_output[$mlid])) {
      $tree = phptemplate_menu_tree_menu_item_data($mlid, $isexpanded, $menu_name);
      $menu_output[$mlid] = menu_tree_output($tree);
    }
  }
  else {
      $menu_output[$mlid] = "ERROR menu item ID was not passed!";
  }
  
  return $menu_output[$mlid];
  
}

/**
 *
 * !!!!!!!! END Navigation Side-Menu Functions !!!!!!!!
 *
 **/





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

/**
 * Including expanded menu functions
 */
include_once('includes/expanded_menu.inc');
