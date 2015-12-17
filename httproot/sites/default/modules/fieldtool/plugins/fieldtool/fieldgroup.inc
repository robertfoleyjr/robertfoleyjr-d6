<?php
/**
 * @file
 * Field Tool - Fieldgroup plugin
 * Fieldgroup module field handler for Field Tool module.
 *
 * Copyright 2010 Matthew Davidson.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/**
 * Implementation of [module]_[plugin]_fieldtool_fields().
 */
function fieldtool_fieldgroup_fieldtool_fields() {
  if (!module_exists('fieldgroup')) {
    return array();
  }

  $node_fields = array();
  
  foreach (fieldgroup_groups() as $type => $groups) {
    foreach ($groups as $group) {
      $group_field_name = 'fieldgroup_'. $type .'_'. $group['group_name'];
      if (isset($node_fields[$group_field_name])) {
        $node_fields[$group_field_name]['node types'][] = $type;
        continue;
      }
      $node_fields[$group_field_name] = array(
        'label' => $group['label'] .' ('. $group_field_name .')',
        'stored' => TRUE,
        'virtual' => TRUE,
        'cck field type' => 'fieldgroup',
        'name' => $group_field_name,
        'node types' => array($type),
        'weight' => $group['weight'],
        'fields' => array_keys($group['fields']),
      );
    }
  }

  return array('node' => $node_fields);
}
