<?php
// $Id$

/**
 * @file
 * Functions needed to execute image effects provided by the SmartCrop module.
 */

/**
 * Implements hook_image_effect_info().
 */
function smartcrop_image_effect_info() {
  $effects = array(
    'smartcrop_scale_and_crop' => array(
      'label' => t('Scale and Smart Crop'),
      'help' => t('Similar to "Scale And Crop", but preserves the portion of the image with the most entropy.'),
      'effect callback' => 'smartcrop_scale_effect',
      'form callback' => 'image_scale_form',
      'summary theme' => 'image_scale_summary',
    ),
    'smartcrop_crop' => array(
      'label' => t('Smart Crop'),
      'help' => t('Similar to "Crop", but preserves the portion of the image with the most entropy.'),
      'effect callback' => 'smartcrop_crop_effect',
      'form callback' => 'image_resize_form',
      'summary theme' => 'image_resize_summary',
    ),
  );
  return $effects;
}

