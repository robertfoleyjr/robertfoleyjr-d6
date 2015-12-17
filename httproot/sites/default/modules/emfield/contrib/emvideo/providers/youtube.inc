<?php
// $Id: youtube.inc,v 1.1.2.50.2.3 2010/04/06 18:11:46 aaron Exp $

/**
 * @file
 * This provider file has been replaced by the Media: YouTube module.
 */

function emvideo_youtube_obsolete() {
  drupal_set_message(t('Please install the !media_youtube module for YouTube integration with Embedded Video Field and Embedded Audio Field.', array('!media_youtube' => l(t('Media: YouTube'), 'http://drupal.org/project/media_youtube'))), 'error');
}
