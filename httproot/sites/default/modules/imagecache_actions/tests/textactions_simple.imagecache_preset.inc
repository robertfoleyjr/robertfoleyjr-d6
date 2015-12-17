<?php
// $ID:  $
/**
 * @file
 *   Test imagecache preset.
 * 
 * Created on Dec 29, 2009
 *
 * @author 'dman' Dan Morrison http://coders.co.nz/
 */

$presets['textactions_simple'] = array (
  'presetname' => 'textactions_simple',
  '#weight' => 5.0,
  'actions' => array (
    0 => array (
      'weight' => '0',
      'module' => 'imagecache_textrender',
      'action' => 'textrender_rendertext',
      'data' => array (
        'textstyle' => array (
          'style' => '
font-size:24px;
fill:#333366;
top:10px;
',
        'fontfile' => drupal_get_path('module', 'imageapi_text') . '/fonts/liberation-fonts-1.04/LiberationSans-Regular.ttf',
        ),
        'text' => 'Hello World!',
        'evaluate_text' => 0,
      ),
    ),
  ),
);