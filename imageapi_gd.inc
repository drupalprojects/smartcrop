<?php
// $Id$

/**
 * @file
 * libgd implementation of smartcrop action.
 */

/**
 * Crop an image, removing the lowest entropy areas.
 *
 * @FIXME - doesn't handle alpha transparency correctly
 *
 * @param array $image_data
 * @param int $requested_x
 * @param int $requested_y
 * @return booleon TRUE if successful
 */
function imageapi_gd_image_smartcrop(&$image_data, $requested_x, $requested_y) {
  $ratio = max($requested_x / $image_data->info['width'], $requested_y / $image_data->info['height']);
  imageapi_gd_image_resize($image_data, $ratio * $image_data->info['width'], $ratio * $image_data->info['height']);
  $image = $image_data->resource;
  $dx = (imagesx($image) - min(imagesx($image), $requested_x));
  $dy = (imagesy($image) - min(imagesy($image), $requested_y));
  $left = $top = 0;
  $right = imagesx($image);
  $bottom = imagesy($image);
  while ($dx) {
    $slice = min($dx, 10);
    $left_slice = imagecreatetruecolor($slice, imagesy($image));
    imagecopy($left_slice, $image, 0, 0, $left, $top, $slice, imagesy($image));
    $right_slice = imagecreatetruecolor($slice, imagesy($image));
    imagecopy($right_slice, $image, 0, 0, $right - $slice, $top, $slice, imagesy($image));
    if (_smartcrop_entropy($left_slice) >= _smartcrop_entropy($right_slice)) {
      $right -= $slice;
    }
    else {
      $left += $slice;
    }
    $dx -= $slice;
    imagedestroy($left_slice);
    imagedestroy($right_slice);
  }

  while ($dy) {
    $slice = min($dy, 10);
    $top_slice = imagecreatetruecolor($requested_x, $slice);
    imagecopy($top_slice, $image, 0, 0, $left, $top, $requested_x, $slice);
    $bottom_slice = imagecreatetruecolor($requested_x, $slice);
    imagecopy($bottom_slice, $image, 0, 0, $left, $bottom - $slice, $requested_x, $slice);
    if (_smartcrop_entropy($top_slice) >= _smartcrop_entropy($bottom_slice)) {
      $bottom -= $slice;
    }
    else {
      $top += $slice;
    }
    $dy -= $slice;
    imagedestroy($top_slice);
    imagedestroy($bottom_slice);
  }

  $cropped_image = imagecreatetruecolor($requested_x, $requested_y);
  imagecopy($cropped_image, $image, 0, 0, $left, $top, $requested_x, $requested_y);
  imagedestroy($image_data->resource);
  $image_data->resource = $cropped_image;
  $image_data->info['width'] = $requested_x;
  $image_data->info['height'] = $requested_y;
  return TRUE;
}

/**
 * Compute the entropy of an image, defined as -sum(p.*log2(p)).
 * @param resource $img GD image resource.
 * @return float The entropy of the image.
 */
function _smartcrop_entropy($img) {
  $histogram = _smartcrop_histogram($img);
  $histogram_size = array_sum($histogram);
  $entropy = 0;
  foreach ($histogram as $p) {
    if ($p == 0) {
      continue;
    }
    $p = $p / $histogram_size;
    $entropy += $p * log($p, 2);
  }
  return $entropy * -1;
}

/**
 * Compute a histogram of an image.
 * @param resource $img GD image resource.
 * @return array histogram as an array.
 */
function _smartcrop_histogram($img) {
  $histogram = array_fill(0, 768, 0);
  for ($i = 0; $i < imagesx($img); $i++) {
    for ($j = 0; $j < imagesy($img); $j++) {
      $rgb = imagecolorat($img, $i, $j);
      $r = ($rgb >> 16) & 0xFF;
      $g = ($rgb >> 8) & 0xFF;
      $b = $rgb & 0xFF;
      $histogram[$r]++;
      $histogram[$g + 256]++;
      $histogram[$b + 512]++;
    }
  }
  return $histogram;
}