<?php
/**
 * @file
 * Field Tool - Location plugin
 *
 * Location module field handler for Field Tool module.
 * Tested with Location 6.x-3.1-rc1
 * Supports: location_node, location_fax, location_phone
 *
 * Copyright 2009 Matthew Davidson.
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
function fieldtool_location_fieldtool_fields() {
  if (!module_exists('location')) {
    return array();
  }

  $node_types = array();

  foreach (node_get_types('types') as $type_name => $type_info) {
    $settings = variable_get('location_settings_node_'. $type_name, array());
    if (count($settings) && $settings['multiple']['max']) { // If the maximum number of locations for this type is greater than zero
      $node_types[] = $type_name;
    }
  }
  
  $node_fields = array();
  
  // Low-level setting of all locations
  $node_fields['locations']['label'] = t('Locations');
  $node_fields['locations']['node types'] = $node_types;
  $node_fields['locations']['multiple'] = TRUE;


  $location_fields = array(
    'name',
    'street',
    'additional',
    'city',
    'province',
    'postal_code',
    'country',
    'latitude',
    'longitude',
    'source',
    'is_primary',
    'province_name',
    'country_name',
    'fax',
    'phone',
  );

  foreach ($location_fields as $field) {
    $node_fields['locations-'. $field]['label'] = t('Location '. ucwords(str_replace('_', ' ', $field)));
    $node_fields['locations-'. $field]['node types'] = $node_types;
    $node_fields['locations-'. $field]['single line'] = TRUE;
    $node_fields['locations']['fields'][] = 'locations-'. $field;
  }

  return array('node' => $node_fields);
}


