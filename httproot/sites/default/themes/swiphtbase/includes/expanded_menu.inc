<?php

/**
 * Expanded menu functions display a unordered list that renders all child menu items.
 */

/**
 * Returns menus using either the menu_access module or the default menu module.
 */
  
function themename_get_menu($menu_name='primary-links', $expanded=FALSE, $forced=FALSE, $depth = 0) {
  
  $menu_output = "";
  $menu_output = themename_menu_tree($menu_name, $expanded, $forced, $depth);
  
    return $menu_output;
}

/**
 * Render a menu tree based on the current path.
 *
 * The tree is expanded based on the current path and dynamic paths are also
 * changed according to the defined to_arg functions (for example the 'My account'
 * link is changed from user/% to a link with the current user's uid).
 *
 * @param $menu_name
 *   The name of the menu.
 * @return
 *   The rendered HTML of that menu on the current page.
 */
function themename_menu_tree($menu_name = 'navigation', $isexpanded = FALSE, $forced = FALSE, $depth = 0) {
  static $menu_output = array();

  if (!isset($menu_output[$menu_name])) {
    if ($forced == TRUE) {
      $tree = themename_menu_tree_forced_data($menu_name, $depth);
    }
    else {
      $tree = themename_menu_tree_page_data($menu_name, $isexpanded, $depth);
    }
    $menu_output[$menu_name] = themename_menu_tree_output($tree, $depth);
  }
  return $menu_output[$menu_name];
}



/**
 * Get the data structure representing a named menu tree.
 *
 * The tree order is maintained by storing each parent in an individual
 * field, see http://drupal.org/node/141866 for more.
 *
 * @param $menu_name
 *   The named menu links to return
 * @return
 *   An array of menu links, in the order they should be rendered. The array
 *   is a list of associative arrays -- these have two keys, link and below.
 *   link is a menu item, ready for theming as a link. Below represents the
 *   submenu below the link if there is one, and it is a subtree that has the
 *   same structure described for the top-level array.
 */
function themename_menu_tree_forced_data($menu_name = 'primary-links', $depth) {
  static $tree = array();
  
  // Generate a cache ID (cid) specific for the expanded menu.
  // Normally a menu item is cached by page this is a extended menu that displays
  // the same on all pages reguardless of location
    $cid = 'links:'. $menu_name .'-expanded';
    
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
        // Build and run the query, and build the tree.
        // Check whether a menu link exists that corresponds to the current path.
          
          $parents[] = '0';
          $parents_result = db_query("SELECT mlid FROM {menu_links} WHERE menu_name = '%s' AND plid = 0 AND hidden <> 1", $menu_name);
          while ($row = db_fetch_array($parents_result)) {
            $parents[] = $row['mlid'];
          }
        // Stage One: Get top Link elements
          
          $children = themename_menu_get_child_menu_item_by_menu_name($menu_name,  $depth);
        
          $args = $parents = array_unique(array_values($parents));
          
          $args = array_unique(array_merge($args, $children));
          
          //$args = array_diff(array_merge($args, $children), array_intersect($args, $children));
          
          $placeholders = implode(', ', array_fill(0, count($args), '%d'));
        
        //menu_access_menu_get_child_menu_item_by_menu_name($menu_name);
        
        // Check whether a menu link exists that corresponds to the current path.
              /*
              $children = menu_access_menu_get_child_menu_item_by_menu_name($menu_name);
              
              //$children = db_fetch_array(db_query("SELECT mlid FROM {menu_links} WHERE plid = %d", $mlid));
              if (!empty($children)) {
                $childplaceholders = implode(', ', array_fill(0, count($children), '%d'));
              } else {
                $childplaceholders = '';
              }
              
              
              do {
                $result = db_query("SELECT mlid FROM {menu_links} WHERE menu_name = '%s' AND has_children = 1 AND plid IN (". $childplaceholders .") AND mlid NOT IN (". $childplaceholders .")", array_merge(array($menu_name), $children));
                $num_rows = FALSE;
                while ($item = db_fetch_array($result)) {
                  $args[] = $item['mlid'];
                  $num_rows = TRUE;
                }
                $placeholders = implode(', ', array_fill(0, count($args), '%d'));
              } while ($num_rows);
              array_unshift($args, $mlid);
            */
        
            /*
              do {
                $result = db_query("SELECT mlid FROM {menu_links} WHERE menu_name = '".$menu_name."' AND has_children = 1 AND plid IN (". $placeholders .") AND mlid NOT IN (". $placeholders .")", $args);
                $num_rows = FALSE;
                while ($item = db_fetch_array($result)) {
                  $args[] = $item['mlid'];
                  $num_rows = TRUE;
                }
                $placeholders = implode(', ', array_fill(0, count($args), '%d'));
              } while ($num_rows);
              array_unshift($args, $mlid);
            */
        
        $data['tree'] = menu_tree_data(db_query("
          SELECT m.load_functions, m.to_arg_functions, m.access_callback, m.access_arguments, m.page_callback, m.page_arguments, m.title, m.title_callback, m.title_arguments, m.type, m.description, ml.*
          FROM {menu_links} ml LEFT JOIN {menu_router} m ON m.path = ml.router_path
          WHERE ml.menu_name = '". $menu_name ."' AND ml.plid IN (". $placeholders .")
          ORDER BY p1 ASC, p2 ASC, p3 ASC, p4 ASC, p5 ASC, p6 ASC, p7 ASC, p8 ASC, p9 ASC", $args), $parents);
        
        $data['node_links'] = array();
        menu_tree_collect_node_links($data['tree'], $data['node_links']);
        
        // Cache the data, if it is not already in the cache.
        $tree_cid = _menu_tree_cid($menu_name, $data);
        if (!cache_get($tree_cid, 'cache_menu')) {
          cache_set($tree_cid, $data, 'cache_menu');
        }
        // Cache the cid of the (shared) data using the page-specific cid.
        cache_set($cid, $tree_cid, 'cache_menu');
        
      }
      
      // Check access for the current user to each item in the tree.
      menu_tree_check_access($data['tree'], $data['node_links']);
      $tree[$cid] = $data['tree'];
    }
    return $tree[$cid];

}




/**
 * Get the data structure representing a named menu tree, based on the current page.
 *
 * The tree order is maintained by storing each parent in an individual
 * field, see http://drupal.org/node/141866 for more.
 *
 * @param $menu_name
 *   The named menu links to return
 * @return
 *   An array of menu links, in the order they should be rendered. The array
 *   is a list of associative arrays -- these have two keys, link and below.
 *   link is a menu item, ready for theming as a link. Below represents the
 *   submenu below the link if there is one, and it is a subtree that has the
 *   same structure described for the top-level array.
 */
function themename_menu_tree_page_data($menu_name = 'navigation', $isexpanded = FALSE, $depth = 0) {
  static $tree = array();

  // Load the menu item corresponding to the current page.
  if ($item = menu_get_item()) {
    // Generate a cache ID (cid) specific for this page.
    $cid = 'links:'. $menu_name .':page-cid:'. $item['href'] .':'. (int)$item['access'];

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
        // Build and run the query, and build the tree.
        if ($item['access']) {
          // Check whether a menu link exists that corresponds to the current path.
          $args = array($menu_name, $item['href']);
          $placeholders = "'%s'";
          if (drupal_is_front_page()) {
            $args[] = '<front>';
            $placeholders .= ", '%s'";
          }
          $parents = db_fetch_array(db_query("SELECT p1, p2, p3, p4, p5, p6, p7, p8 FROM {menu_links} WHERE menu_name = '%s' AND link_path IN (". $placeholders .")", $args));

          if (empty($parents)) {
            // If no link exists, we may be on a local task that's not in the links.
            // TODO: Handle the case like a local task on a specific node in the menu.
            $parents = db_fetch_array(db_query("SELECT p1, p2, p3, p4, p5, p6, p7, p8 FROM {menu_links} WHERE menu_name = '%s' AND link_path = '%s'", $menu_name, $item['tab_root']));
          }
          // We always want all the top-level links with plid == 0.
          $parents[] = '0';

          // Use array_values() so that the indices are numeric for array_merge().
          $args = $parents = array_unique(array_values($parents));
          $placeholders = implode(', ', array_fill(0, count($args), '%d'));
          
          // override validation if true override and display the entire menu branch.
          if ($isexpanded) {
              
              do {
                if (strlen($depth) > 0) {
                  $result = db_query("SELECT mlid, depth FROM {menu_links} WHERE menu_name = '%s' AND has_children = 1 AND plid IN (". $placeholders .') AND mlid NOT IN ('. $placeholders .') AND depth < %d', array_merge(array($menu_name), $args, $args, $depth));
                } else {
                  $result = db_query("SELECT mlid, depth FROM {menu_links} WHERE menu_name = '%s' AND has_children = 1 AND plid IN (". $placeholders .') AND mlid NOT IN ('. $placeholders .')', array_merge(array($menu_name), $args, $args));
                }
                
                $num_rows = FALSE;
                while ($item = db_fetch_array($result)) {
                  $args[] = $item['mlid'];
                  $num_rows = TRUE;
                }
                $placeholders = implode(', ', array_fill(0, count($args), '%d'));
              } while ($num_rows);
              array_unshift($args, $menu_name);
          }
          else {
          
            $expanded = variable_get('menu_expanded', array());
            // Check whether the current menu has any links set to be expanded.
            if (in_array($menu_name, $expanded)) {
              // Collect all the links set to be expanded, and then add all of
              // their children to the list as well.
              do {
                $result = db_query("SELECT mlid FROM {menu_links} WHERE menu_name = '%s' AND expanded = 1 AND has_children = 1 AND plid IN (". $placeholders .') AND mlid NOT IN ('. $placeholders .')', array_merge(array($menu_name), $args, $args));
                $num_rows = FALSE;
                while ($item = db_fetch_array($result)) {
                  $args[] = $item['mlid'];
                  $num_rows = TRUE;
                }
                $placeholders = implode(', ', array_fill(0, count($args), '%d'));
              } while ($num_rows);
            }
            array_unshift($args, $menu_name);
            
          
          }
        }
        else {
          // Show only the top-level menu items when access is denied.
          $args = array($menu_name, '0');
          $placeholders = '%d';
          $parents = array();
        }
        // Select the links from the table, and recursively build the tree. We
        // LEFT JOIN since there is no match in {menu_router} for an external
        // link.
        $data['tree'] = menu_tree_data(db_query("
          SELECT m.load_functions, m.to_arg_functions, m.access_callback, m.access_arguments, m.page_callback, m.page_arguments, m.title, m.title_callback, m.title_arguments, m.type, m.description, ml.*
          FROM {menu_links} ml LEFT JOIN {menu_router} m ON m.path = ml.router_path
          WHERE ml.menu_name = '%s' AND ml.plid IN (". $placeholders .")
          ORDER BY p1 ASC, p2 ASC, p3 ASC, p4 ASC, p5 ASC, p6 ASC, p7 ASC, p8 ASC, p9 ASC", $args), $parents);
        $data['node_links'] = array();
        menu_tree_collect_node_links($data['tree'], $data['node_links']);
        // Cache the data, if it is not already in the cache.
        $tree_cid = _menu_tree_cid($menu_name, $data);
        if (!cache_get($tree_cid, 'cache_menu')) {
          cache_set($tree_cid, $data, 'cache_menu');
        }
        // Cache the cid of the (shared) data using the page-specific cid.
        cache_set($cid, $tree_cid, 'cache_menu');
      }
      
      // Check access for the current user to each item in the tree.
      if ($data['tree'] != NULL) {
        menu_tree_check_access($data['tree'], $data['node_links']);
      }
      $tree[$cid] = $data['tree'];
    }
    return $tree[$cid];
  }

  return array();
}


/**
 * Returns a rendered menu tree.
 *
 * @param $tree
 *   A data structure representing the tree as returned from menu_tree_data.
 * @return
 *   The rendered HTML of that data structure.
 */
function themename_menu_tree_output($tree, $depth = 0) {
  $depth_active = FALSE;
  $depth_set = FALSE;
  $output = '';
  $items = array();

  // Check if depth set
  if ($depth > 0) {
    $depth_active = TRUE;
  }

  // Pull out just the menu items we are going to render so that we
  // get an accurate count for the first/last classes.
  foreach ($tree as $data) {
    if (!$data['link']['hidden']) {
      $items[] = $data;
    }
  }

  $num_items = count($items);
  foreach ($items as $i => $data) {
    $extra_class = NULL;
    if ($i == 0) {
      $extra_class = 'first';
    }
    if ($i == $num_items - 1) {
      $extra_class = 'last';
    }
    $link = theme('menu_item_link', $data['link']);
    ($depth_active && $data['depth'] < $depth) ? $depth_set = TRUE : $depth_set = FALSE;
    if ($data['below'] && $depth_set == TRUE) {
      $output .= theme('menu_item', $link, $data['link']['has_children'], themename_menu_tree_output($data['below'], $depth), $data['link']['in_active_trail'], $extra_class);
    }
    else {
      $output .= theme('menu_item', $link, $data['link']['has_children'], '', $data['link']['in_active_trail'], $extra_class);
    }
  }
  return $output ? theme('menu_tree', $output) : '';
}


/** Get menu children by mlid
 * @param $mlid
 *   The menu item id
 * @return
 *   An array of menu item ids for all children menu items for the given mlid.
 */
function themename_menu_get_child_by_menu_item($parent_mlid, $depth = 0) {
  $children = array();
  
  $childresult = db_query("SELECT mlid, depth FROM {menu_links} WHERE plid = %d AND hidden <> 1", $parent_mlid);
  while ($child_row = db_fetch_array($childresult)) {
    $children[] = $child_row['mlid'];
    
    if ($depth > 0) {
      if ($child_row['depth'] < $depth) {
        $childarr = themename_menu_get_child_by_menu_item($child_row['mlid'], $depth);
      } else {
        $childarr = array();
      }
    } else {
      $childarr = themename_menu_get_child_by_menu_item($child_row['mlid'], $depth);
    }
    if (!empty($childarr)) {
      $children = array_merge($childarr, $children);
    }
  }
  
  return $children;
}


/**
 * Get menu children
 *
 * @param $menu_name
 *   The menu item id
 * @return
 *   An array of menu item ids for all children menu items for the given menu_name.
 */
function themename_menu_get_child_menu_item_by_menu_name($menu_name, $depth = 0) {
  $children = array();
  
  $childresult = db_query("SELECT mlid, depth FROM {menu_links} WHERE menu_name = '%s' AND plid = 0 AND hidden <> 1", $menu_name);
  while ($child_row = db_fetch_array($childresult)) {
    $children[] = $child_row['mlid'];
    if ($depth > 0) {
      if ($child_row['depth'] < $depth) {
        $childarr = themename_menu_get_child_by_menu_item($child_row['mlid'], $depth);
      } else {
        $childarr = array();
      }
    } else {
      $childarr = themename_menu_get_child_by_menu_item($child_row['mlid'], $depth);  
    }
    
    if (!empty($childarr)) {
      $children = array_merge($childarr, $children);
    }
  }
  
  return $children;
}
