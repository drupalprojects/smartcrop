<?php
// $Id$

/**
 * @file
 * Imagecache actions.
 */

/**
 * Build the settings form.
 */
function smartcrop_scale_and_crop_form($data = array()) {
  module_load_include('inc', 'imagecache', 'imagecache_actions');
  return imagecache_resize_form($data);
}

/**
 * Theme a cropped image.
 */
function theme_smartcrop_scale_and_crop($element) {
  module_load_include('inc', 'imagecache', 'imagecache_actions');
  return theme_imagecache_resize($element);
}

/**
 * Crop the image.
 */
function smartcrop_scale_and_crop_image(&$image, $data) {
  if (imageapi_toolkit_invoke('smartcrop', $image, $data)) {
    return TRUE;
  }
}