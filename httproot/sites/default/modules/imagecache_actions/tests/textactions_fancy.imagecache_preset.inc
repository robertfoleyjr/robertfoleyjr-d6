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

$presets['textactions_fancy'] = array (
  'presetname' => 'textactions_fancy',
  '#weight' => 5.1,
  'actions' => array (
    0 => array (
      'weight' => '0',
      'module' => 'imagecache_textrender',
      'action' => 'textrender_rendertext',
      'data' => array (
        'textstyle' => array (
          'style' => '
font-size:32px;
fill:#000066;
stroke-width:2px;
stroke:#ffffff;
bottom:10px;
text-shadow: #669966 3px 4px 3px;
',
        'fontfile' => drupal_get_path('module', 'imageapi_text') . '/fonts/Chopin Script.ttf',
        ),
        'text' => 'Flash Strokes',
      ),
    ),
  ),
);