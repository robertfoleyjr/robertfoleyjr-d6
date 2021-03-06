<?php
// $Id: archive_audio.inc,v 1.2 2009/07/27 04:00:16 winston Exp $

/**
 * @file
 *   This include processes archive.org audio files for use with Embedded Media Field.
 */

/**
 *  This is the main URL for your provider.
 */
define('EMAUDIO_ARCHIVE_MAIN_URL', 'http://www.archive.org/');

/**
 *  This is the URL to the API of your provider, if this exists.
 */
define('EMAUDIO_ARCHIVE_API_URL', 'http://www.google.com/search?q=archive.org+api');

/**
 *  This defines the version of the content data array that we serialize
 *  in emaudio_archive_data(). If we change the expected keys of that array,
 *  we must increment this value, which will allow older content to be updated
 *  to the new version automatically.
 */
define('EMAUDIO_ARCHIVE_DATA_VERSION', 1);

/**
 * hook emaudio_PROVIDER_info
 * This returns information relevant to a specific 3rd party video provider.
 *
 * @return
 *   A keyed array of strings requested by various admin and other forms.
 *    'provider' => The machine name of the provider. This must be the same as
 *      the base name of this filename, before the .inc extension.
 *    'name' => The translated name of the provider.
 *    'url' => The url to the main page for the provider.
 *    'settings_description' => A description of the provider that will be
 *      posted in the admin settings form.
 *    'supported_features' => An array of rows describing the state of certain
 *      supported features by the provider. These will be rendered in a table,
 *      with the columns being 'Feature', 'Supported', 'Notes'. In general,
 *      the 'Feature' column will give the name of the feature, 'Supported'
 *      will be Yes or No, and 'Notes' will give an optional description or
 *      caveats to the feature.
 */
function emaudio_archive_audio_info() {
  $features = array(
    array(t('Thumbnails'), t('No'), ''),
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS attachment'), t('No'), ''),
  );
  return array(
    'provider' => 'archive_audio',
    'name' => t('Archive'),
    'url' => 'EMAUDIO_ARCHIVE_MAIN_URL',
    'settings_description' => t('These settings specifically affect audio played from !archive. You can also read more about its !api.', array('!archive' => l(t('Archive.com'), EMAUDIO_ARCHIVE_MAIN_URL), '!api' => l(t("developer's API"), EMAUDIO_ARCHIVE_API_URL))),
    'supported_features' => $features,
  );
}

/**
 *  hook emaudio_PROVIDER_settings
 *  This should return a subform to be added to the emaudio_settings() admin
 *  settings page.
 *
 *  Note that a form field set will already be provided at $form['archive'],
 *  so if you want specific provider settings within that field set, you should
 *  add the elements to that form array element.
 */
function emaudio_archive_audio_settings() {
  /* No Settings for this provider */
}

/**
 *  hook emaudio_PROVIDER_extract
 *
 *  This is called to extract the audio code from a pasted URL or embed code.
 *
 *  We'll be passed a URL or the embed code from a audio when an editor pastes
 *  that in the field's textfield. We'll need to either pass back an array of
 *  regex expressions to match, or do the matching ourselves and return the
 *  resulting audio code.
 *
 *  @param $parse
 *    An optional string with the pasted URL or embed code.
 *  @return
 *    Either an array of regex expressions to be tested, or a string with the
 *    audio code to be used. If the hook tests the code itself, it should
 *    return either the string of the audio code (if matched), or an empty
 *    array. Otherwise, the calling function will handle testing the embed code
 *    against each regex string in the returned array.
 */
function emaudio_archive_audio_extract($parse = '') {
  // Here we assume that a URL will be passed in the form of
  // http://www.archive.org/details/text-audio-title
  // or embed code in the form of 
  //<embed ... "playlist":[{"url":"http://www.archive.org/download/text-audio-title/...

  // We'll simply return an array of regular expressions for Embedded Media
  // Field to handle for us.
  return array(
    // In this expression, we're looking first for text matching the expression
    // between the @ signs. The 'i' at the end means we don't care about the
    // case. Thus, if someone enters http://www.Archive.com, it will still
    // match. We escape periods as \., as otherwise they match any character.
    // The text in parentheses () will be returned as the provider audio code,
    // if there's a match for the entire expression. In this particular case,
    // ([^?]+) means to match one more more characters (+) that are not a
    // question mark ([^\?]), which would denote a query in the URL.
    '@archive\.org\/details\/([^\"\&]+)@i',

    // Now we test for embedded audio code, which is similar in this case to
    // the above expression, except that we can safely assume we won't have a
    // query in the URL, and that the URL will be surrounded by quotation marks,
    // and have /download/ rather than /details/ in the URL. Note that regular
    // expressions will be tested for matches in the order provided, so you
    // may need to move this value above the other in some cases. Obviously,
    // in the case of this archive provider, you could easily improve the
    // regular expression to match against either a URL or embed code with
    // one expression, such as '@archive\.com/[watch|embed]/([^"\?]+)@i'.
    // However, many other providers have more complex requirements, so
    // we split them up for this demonstration.
    '@archive\.org/download/([^/]+)=@i',
  );
}

/**
 *  hook emaudio_PROVIDER_data
 *
 *  Provides an array to be serialised and made available with $item elsewhere.
 *
 *  This data can be used to store any extraneous information available
 *  specifically to the archive provider.
 */
function emaudio_archive_audio_data($field, $item, $error_field = '') {

  // Initialize the data array.
  $data = array();

  // Create some version control. Thus if we make changes to the data array
  // down the road, we can respect older content. If allowed by Embedded Media
  // Field, any older content will automatically update this array as needed.
  // In any case, you should account for the version if you increment it.
  $data['emaudio_archive_version'] = EMAUDIO_ARCHIVE_DATA_VERSION;

  // Construct a base data url
  $data_url = "http://www.archive.org/download/". $item['value'];

  // In this directory should be two XML files, one for the list of files, one for metadata.
  $xml_files_url = $data_url .'/'. $item['value'] .'_files.xml';
  $xml_meta_url = $data_url .'/'. $item['value'] .'_meta.xml';

  // Retreive the XML files.
  $xml_files = archive_audio_request_xml('archive', $xml_files_url, array(), TRUE, TRUE, $item['value'] .'_files');
  $xml_meta = archive_audio_request_xml('archive', $xml_meta_url, array(), TRUE, TRUE, $item['value'] .'_meta');
  if ($xml_meta['stat'] == 'error' || empty($xml_meta)) {
    drupal_set_message('Additional information about the item at archive.org could not be retrieved.  The audio can still be displayed.');
  }
  else {
    $data['metadata'] = $xml_meta['METADATA'];
  }
  if ($xml_files['stat'] == 'error' || empty($xml_files)) {
    form_set_error($error_field, 'The list of files for the item at archive.org could not be retrieved.  The audio can not be displayed.');
    return $data;
  }
  
  // Retrieve .m3u file (playlist) that archive.org is using for this item
  // This should represent the best "web playable" mp3 versions of each
  $result = drupal_http_request($data_url .'/'. $item['value'] .'_vbr.m3u');
  if (!empty($result->error)) {
    form_set_error($error_field, 'The playlist for the item at archive.org could not be retrieved.  The audio can not be displayed.');
    return $data;
  }
  $playlist = $result->data;
  // We'll store it in the data element in case it is useful to the themer
  $data['playlist'] = explode("\n", trim($playlist));

  // Get all file information just in case it is useful to the site designer/themer
  $xml_files = $xml_files['FILES'];
  foreach ($xml_files as $file_name => $file_data) {
    $data['files'][$file_name] = $file_data;
  }

  return $data;
}

/**
 * hook emfield_PROVIDER_rss
 */
function emaudio_archive_audio_rss($item, $teaser = NULL) {
  // Get size and mime type of first playlist file
  $url = $item['data']['playlist'][0];
  $response = emfield_request_header('archive_audio', $url, $cached = FALSE);
  if ($response->code == 200) {
    $data['size'] = $response->headers['Content-Length'];
    $data['mime'] = $response->headers['Content-Type'];
  }

  if ($data['size']) {
    $file = array();
    $file['filepath'] = $url;
    $file['filesize'] = $data['size'];
    $file['filemime'] = $data['mime'];
  }
  return $file;
}

/**
 * Implementation of hook emaudio_archive_audio_audio().
 *
 * This actually displays the full/normal-sized video we want, usually on the default page view.
 *
 * @param $embed
 *   The video code for the audio to embed.
 * @param $width
 *   The width to display the audio.
 * @param $height
 *   The height to display the audio.
 * @param $field
 *   The field info from the requesting node.
 * @param $item
 *   The actual content from the field.
 * @return
 *   The html of the embedded audio.
 */
function emaudio_archive_audio_audio($embed = NULL, $width = 0, $height = 0, $field = NULL, $item, $node, $autoplay) {
  $output = theme('emaudio_archive_audio_flash', $embed, $width, $height, $field, $item, $node, $autoplay);

  return $output;
}

/**
 * The embedded flash displaying the archive audio.
 */
function theme_emaudio_archive_audio_flash($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = '';
  if ($item) {
    $flowplayerplaylist = _archive_audio_flowplayer_playlist($item['data']['playlist']);
    $controlplaylist = (count($item['data']['playlist'])>1) ? "true" : "false";
    $clipautoplay = $autoplay ? 'true' : 'false';
    $output = <<<EOD
<embed type="application/x-shockwave-flash" width="$width" height="$height" allowfullscreen="true" 
  allowscriptaccess="always" src="http://www.archive.org/flow/flowplayer.commercial-3.0.5.swf" w3c="true" 
  flashvars='config={
    "key":"#\$b6eb72a0f2f1e29f3d4",
    "playlist":[
    {$flowplayerplaylist}
    ],
    "clip":{
        "autoPlay":{$clipautoplay}
      },
    "canvas":{
      "backgroundColor":"0x000000",
      "backgroundGradient":"none"
    },
    "plugins":{
      "audio":{
        "url":"http://www.archive.org/flow/flowplayer.audio-3.0.3-dev.swf"
      },
      "controls":{
        "playlist":{$controlplaylist},
        "fullscreen":false,
        "gloss":"high",
        "backgroundColor":"0x000000",
        "backgroundGradient":"medium",
        "sliderColor":"0x777777",
        "progressColor":"0x777777",
        "timeColor":"0xeeeeee",
        "durationColor":"0x01DAFF",
        "buttonColor":"0x333333",
        "buttonOverColor":"0x505050"
      }
    },
    "contextMenu":[
      {
        "Item {$item['value']} at archive.org":"function()"
      },
      "-",
      "Flowplayer 3.0.5"
    ]
  }'>
</embed>
EOD;
  }
  return $output;
}

/**
 * Construct flowplayer style playlist from one or more files
 */
function _archive_audio_flowplayer_playlist($playlist) {
  foreach ($playlist as $key => $url) {
    //First item doesn't play automatically
    if ($key==0) {
      $fpplaylist["$key"]="{\"url\":\"$url\",\"autoplay\":false}";
    }
    else {
      $fpplaylist["$key"]="{\"url\":\"$url\",\"autoplay\":true}";
    }
  }
  $output = implode(",\n", $fpplaylist);
  return $output;
}


/**
 * Implementation of hook_emfield_subtheme.
 */
function emaudio_archive_audio_emfield_subtheme() {
  return array(
    'emaudio_archive_audio_flash'  => array(
            'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'field' => NULL, 'data' => NULL, 'node' => NULL, 'autoplay' => NULL),
            'file' => 'providers/emaudio/archive_audio.inc',
            'path' => drupal_get_path('module', 'media_archive'),
        )
    );
}

/**
 * Custom version of emfield_request_xml to attempt to fix issues with XML return.
 * TODO: Remove this when emfield_request_xml is fixed
 * see issue - http://drupal.org/node/516202
 *
 * When an include file requires to read an xml to receive information, such as for thumbnails,
 * this script can be used to request the xml and return it as an array.
 *   @param $provider
 *     the string of the third party provider, such as 'youtube', 'flikr', or 'google'
 *   @param $url
 *     the url for the xml request
 *   @param $args
 *     an array of args to pass to the xml url
 *   @param $cached
 *     optional; if TRUE, the result of this xml request will be cached. good to play nice w/
 *     the third party folks so they don't stop providing service to your site...
 *   @param $return_errors
 *     optional; if TRUE and an error is encountered, a descriptive error array will be returned with elements for
 *     code, message and stat => 'error'
 *   @param $hash_extra
 *     optional; The key for caching is created from the arguments.  If your provider does not use arguments
 *     (or uses the same arguments for each media item, you must pass a unique string as $hash_extra.  Currently
 *     this is only used by bliptv and archive.org
 *   @param $serialized
 *     optional; Most uses of this function are expecting an XML file to be returned.  However some providers (Flickr)
 *     can instead return a serialized PHP array.  In this case set $serialized to TRUE.
 *  @param $json
 *    If TRUE, then the result will be a json encoded string.
 *   @return
 *     the xml results returned as an array
 */
function archive_audio_request_xml($provider, $url, $args = array(), $cached = TRUE, $return_errors = FALSE, $hash_extra = FALSE, $serialized = FALSE, $json = FALSE) {
  ksort($args);

  // Build an argument hash that we'll use for the cache id and api signing.
  $arghash = $provider .':';
  foreach ($args as $k => $v) {
    $arghash .= $k . $v;
  }

  // Build the url.
  foreach ( $args as $k => $v) {
    $encoded_params[] = urlencode($k) .'='. urlencode($v);
  }
  if (!empty($encoded_params)) {
    $url .= '?'. implode('&', $encoded_params);
  }

  // some providers, such as bliptv, actually change the url, and not just the queries.
  // we provide an extra section for a unique identifier in that case
  if (isset($hash_extra)) {
    $arghash .= ':'. $hash_extra;
  }

  // if it's a cachable request, try to load a cached value
  if ($cached && $cache = cache_get($arghash, 'cache')) {
    return $cache->data;
  }

  // connect and fetch a value
  $result = drupal_http_request($url);

  if (!empty($result->error)) {
    if ($return_errors) {
      return array(
        'stat' => 'error',
        'code' => $result->code,
        'message' => 'HTTP Error: '. $result->error,
      );
    }
    emfield_set_error(t("Could not connect to @provider, HTTP error @error", array('@error' => $result->code, '@provider' => $provider)));

    return array();
  }

  if ($json) {
    $response = (array)json_decode($result->data);
  }
  else if ($serialized) {
    // Flickr gives us a serialized php array. Make sure it unserializes.
    $response = unserialize($result->data);
    if (!$response) {
      if ($return_errors) {
        return array(
          'stat' => 'error',
          'code' => '-1',
          'message' => 'The response was corrupted, it could not be unserialized.',
        );
      }

      emfield_set_error(t("The response from @provider was corrupted and could not be unserialized.", array('@provider' => $provider)));

      return array();
    }
  }
  else {
    $parser = drupal_xml_parser_create($result->data);
    $vals = array();
    $index = array();
    xml_parse_into_struct($parser, $result->data, $vals, $index);
    xml_parser_free($parser);

    $response = array();
    $response['_emfield_arghash'] = $arghash;
    $level = array();
    
    foreach ($vals as $xml_elem) {
      if ($xml_elem['type'] == 'open') {
        if (array_key_exists('attributes', $xml_elem)) {
          list($level[$xml_elem['level']], $extra) = array_values($xml_elem['attributes']);
        }
        else {
          $level[$xml_elem['level']] = $xml_elem['tag'];
        }
      }
      if ($xml_elem['type'] == 'complete') {
        $start_level = 1;
        $php_stmt = '$response';
        while ($start_level < $xml_elem['level']) {
          $php_stmt .= '[$level['. $start_level .']]';
          $start_level++;
        }
        $php_stmt .= '[$xml_elem[\'tag\']] = $xml_elem[\'value\'];';
        eval($php_stmt);
      }
    }
  }

  if ($cached) {
    cache_set($arghash, $response, 'cache', time() + variable_get('emfield_cache_duration', 3600));
  }

  return $response;
}
