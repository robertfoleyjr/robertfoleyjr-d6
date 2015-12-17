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

$presets['textactions_positioning'] = array (
  'presetname' => 'textactions_positioning',
  '#weight' => 5.4,

  'actions' => array (
    array (
      'weight' => '-1',
      'module' => 'imagecache_canvasactions',
      'action' => 'canvasactions_file2canvas',
      'data' =>  array (
        'xpos' => 'right',
        'ypos' => 'top',
        'alpha' => '100',
        'path' => "$filepath/grid-240x160.png",
      ),
    ),

    array (
      'weight' => '0',
      'module' => 'imagecache_textrender',
      'action' => 'textrender_rendertext',
      'data' => array (
        'textstyle' => array (
          'style' => 'font-size:10px; fill:#FFFFFF; text-align:center;',
          'fontfile' => drupal_get_path('module', 'imageapi_text') . '/fonts/liberation-fonts-1.04/LiberationSans-Bold.ttf',
        ),
        'text' => "Default Centered",
        'text' => "Default\nCentered",
        'evaluate_text' => 0,
      ),
    ),
    array (
      'weight' => '0',
      'module' => 'imagecache_textrender',
      'action' => 'textrender_rendertext',
      'data' => array (
        'textstyle' => array (
          'style' => 'font-size:8px; fill:#333366; top:10px; left:10px;',
          'fontfile' => drupal_get_path('module', 'imageapi_text') . '/fonts/liberation-fonts-1.04/LiberationSans-Regular.ttf',
        ),
        'text' => "top:10px;\nleft:10px;",
        'evaluate_text' => 0,
      ),
    ),
    array (
      'weight' => '0',
      'module' => 'imagecache_textrender',
      'action' => 'textrender_rendertext',
      'data' => array (
        'textstyle' => array (
          'style' => 'font-size:8px; fill:#333366; bottom:0px;',
          'fontfile' => drupal_get_path('module', 'imageapi_text') . '/fonts/liberation-fonts-1.04/LiberationSans-Regular.ttf',
        ),
        'text' => 'bottom:0px;',
        'evaluate_text' => 0,
      ),
    ),
    array (
      'weight' => '0',
      'module' => 'imagecache_textrender',
      'action' => 'textrender_rendertext',
      'data' => array (
        'textstyle' => array (
          'style' => 'font-size:8px; fill:#333366; top:-1; right:-2',
          'fontfile' => drupal_get_path('module', 'imageapi_text') . '/fonts/liberation-fonts-1.04/LiberationSans-Regular.ttf',
        ),
        'text' => "top:-1;\nright:-2px",
        'evaluate_text' => 0,
      ),
    ),
    array (
      'weight' => '0',
      'module' => 'imagecache_textrender',
      'action' => 'textrender_rendertext',
      'data' => array (
        'textstyle' => array (
          'style' => 'font-size:8px; fill:#333366; left:0px; top:90; stroke-width:6px; stroke:#ffffff;',
          'fontfile' => drupal_get_path('module', 'imageapi_text') . '/fonts/liberation-fonts-1.04/LiberationSans-Regular.ttf',
        ),
        'text' => "left:0; top:50;\nstroke-width:6px; stroke:#ffffff;",
        'evaluate_text' => 0,
      ),
    ),
    array (
      'weight' => '0',
      'module' => 'imagecache_textrender',
      'action' => 'textrender_rendertext',
      'data' => array (
        'textstyle' => array (
          'style' => 'font-size:8px; fill:#333366; right:0px; transform:rotate(90);',
          'fontfile' => drupal_get_path('module', 'imageapi_text') . '/fonts/liberation-fonts-1.04/LiberationSans-Regular.ttf',
        ),
        'text' => "right:0px;\n transform:rotate(90)",
        'evaluate_text' => 0,
      ),
    ),
    array (
      'weight' => '0',
      'module' => 'imagecache_textrender',
      'action' => 'textrender_rendertext',
      'data' => array (
        'textstyle' => array (
          'style' => 'font-size:8px; fill:#111111; bottom:-6px;left:-6px; transform:rotate(-45); stroke-width:2px; stroke:#ffffff;',
          'fontfile' => drupal_get_path('module', 'imageapi_text') . '/fonts/liberation-fonts-1.04/LiberationSans-Regular.ttf',
        ),
        'text' => "bottom:-6px;left:-6px;\ntransform:rotate(-45)",
        'evaluate_text' => 0,
      ),
    ),
  ),
);

$presets['textactions_positioning_percent'] = array (
  'presetname' => 'textactions_positioning_percent',
  '#weight' => 5.5,

  'actions' => array (

    array (
      'weight' => '-1',
      'module' => 'imagecache_canvasactions',
      'action' => 'canvasactions_file2canvas',
      'data' =>  array (
        'xpos' => 'right',
        'ypos' => 'top',
        'alpha' => '100',
        'path' => "$filepath/grid-240x160.png",
      ),
    ),

    array (
      'weight' => '0',
      'module' => 'imagecache_textrender',
      'action' => 'textrender_rendertext',
      'data' => array (
        'textstyle' => array (
          'style' => 'font-size:8px; fill:#333366; left:0px; top:25%;',
          'fontfile' => drupal_get_path('module', 'imageapi_text') . '/fonts/liberation-fonts-1.04/LiberationSans-Bold.ttf',
        ),
        'text' => "left:0; top:25%",
        'evaluate_text' => 0,
      ),
    ),

    array (
      'weight' => '0',
      'module' => 'imagecache_textrender',
      'action' => 'textrender_rendertext',
      'data' => array (
        'textstyle' => array (
          'style' => 'font-size:8px; fill:#333366; bottom:25%; right:25%',
          'fontfile' => drupal_get_path('module', 'imageapi_text') . '/fonts/liberation-fonts-1.04/LiberationSans-Bold.ttf',
        ),
        'text' => "bottom:25%;\n right:25%",
        'evaluate_text' => 0,
      ),
    ),
    array (
      'weight' => '0',
      'module' => 'imagecache_textrender',
      'action' => 'textrender_rendertext',
      'data' => array (
        'textstyle' => array (
          'style' => 'font-size:8px; fill:#333366; left:0%; top:60%',
          'fontfile' => drupal_get_path('module', 'imageapi_text') . '/fonts/liberation-fonts-1.04/LiberationSans-Bold.ttf',
        ),
        'text' => "left:0%;\ntop:60%",
        'evaluate_text' => 0,
      ),
    ),

  ),

);