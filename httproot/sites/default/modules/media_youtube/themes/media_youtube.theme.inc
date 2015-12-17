<?php
// $Id: media_youtube.theme.inc,v 1.1.2.1 2010/04/22 18:33:52 aaron Exp $

/**
 * @file
 * Theme functions for Media: YouTube.
 */

function theme_media_youtube_video_unavailable($message) {
  return "<div class='media-youtube-status-unavailable'>$message</div>";
}
