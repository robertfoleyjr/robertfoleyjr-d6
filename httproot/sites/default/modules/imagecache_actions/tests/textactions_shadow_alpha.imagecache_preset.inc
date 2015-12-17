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

$presets['textactions_shadow_alpha'] = array (
  'presetname' => 'textactions_shadow_alpha',
  '#weight' => 5.2,
  'actions' => array (
    0 => array (
      'module' => 'imagecache_textrender',
      'action' => 'textrender_rendertext',
      'data' => array (
        'textstyle' => array (
          'style' => '
font-size:40px;
fill:#333333;
text-shadow: #EEEEEE 3px 3px 3px;
text-shadow-opacity: .5;
',
          'fontfile' => drupal_get_path('module', 'imageapi_text') . '/fonts/Rufscript010.ttf',
        ),
        'text' => "Alpha\nShadow",
      ),
    ),
  ),
);