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

$presets['text_positioning'] = array (
  'presetname' => 'text_positioning',
  '#weight' => 6.0,
  'actions' => array (
    array (
      'weight' => '0',
      'module' => 'imagecache_textactions',
      'action' => 'textactions_text2canvas',
      'data' => array (
        'size' => '12',
        'alpha' => '100',
        'xpos' => 'right',
        'ypos' => 'middle-20',
        'RGB' => array (
          'HEX' => 'eb3333',
        ),
        'fontfile' => 'liberation-fonts-1.04/LiberationSans-Regular.ttf',
        'text' => 'right,middle-20',
        'angle' => 0,
      ),
    ),
    array (
      'weight' => '0',
      'module' => 'imagecache_textactions',
      'action' => 'textactions_text2canvas',
      'data' => array (
        'size' => '12',
        'alpha' => '100',
        'xpos' => '5',
        'ypos' => '5',
        'RGB' => array (
          'HEX' => 'eb3333',
        ),
        'fontfile' => 'liberation-fonts-1.04/LiberationSans-Regular.ttf',
        'text' => '5,5',
        'angle' => 0,
      ),
    ),
    array (
      'weight' => '0',
      'module' => 'imagecache_textactions',
      'action' => 'textactions_text2canvas',
      'data' => array (
        'size' => '12',
        'alpha' => '100',
        'xpos' => 'center',
        'ypos' => 'bottom+10',
        'RGB' => array (
          'HEX' => 'eb3333',
        ),
        'fontfile' => 'liberation-fonts-1.04/LiberationSans-Regular.ttf',
        'text' => 'center, bottom+10',
        'angle' => 0,
      ),
    ),
    array (
      'weight' => '0',
      'module' => 'imagecache_textactions',
      'action' => 'textactions_text2canvas',
      'data' => array (
        'size' => '12',
        'alpha' => '100',
        'xpos' => '30%',
        'ypos' => 'top',
        'RGB' => array (
          'HEX' => 'eb3333',
        ),
        'fontfile' => 'liberation-fonts-1.04/LiberationSans-Regular.ttf',
        'text' => 'top, 30%',
        'angle' => 0,
      ),
    ),
    array (
      'weight' => '0',
      'module' => 'imagecache_textactions',
      'action' => 'textactions_text2canvas',
      'data' => array (
        'size' => '12',
        'alpha' => '100',
        'xpos' => 'left',
        'ypos' => 'bottom',
        'RGB' => array (
          'HEX' => 'eb3333',
        ),
        'fontfile' => 'liberation-fonts-1.04/LiberationSans-Regular.ttf',
        'text' => 'left, bottom, angled',
        'angle' => 10,
      ),
    ),
    array (
      'weight' => '0',
      'module' => 'imagecache_textactions',
      'action' => 'textactions_text2canvas',
      'data' => array (
        'size' => '12',
        'alpha' => '100',
        'xpos' => 'left+20',
        'ypos' => 'bottom+20',
        'RGB' => array (
          'HEX' => 'eb3333',
        ),
        'fontfile' => 'liberation-fonts-1.04/LiberationSans-Regular.ttf',
        'text' => 'left+20, bottom+20',
        'angle' => 0,
      ),
    ),
  
  ),
);