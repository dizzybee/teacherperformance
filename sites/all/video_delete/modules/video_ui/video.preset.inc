<?php
/**
 * @file
 * Administrative interface for maintaining video presets.
 */

define('VIDEO_PRESET_MAX_LENGTH', 64);

/**
 * All the other settings that you need to declare in your preset.
 *
 * The preset object is passed to this form, so you can use that data to
 * populate the default values for your form elements by using
 * $preset['settings']['param2'].
 *
 * @param $preset
 *   The preset object.
 * @return
 *   The additional form settings that you would like to add to your preset.
 */
function video_preset_default_form($form, &$form_state, $preset) {
  $factory = new TranscoderAbstractionAbstractFactory();
  $transcoder = $factory->getProduct();
  $codecs = $transcoder->getCodecs();
  $formats = $transcoder->getAvailableFormats('muxing');
  $form = array();
  // video settings
  $form['settings']['video'] = array(
    '#type' => 'fieldset',
    '#title' => t('Video settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );
  $form['settings']['video']['video_extension'] = array(
    '#type' => 'select',
    '#title' => t('Video output extension'),
    '#description' => t('Extension of the output video.'),
    '#options' => $formats,
    '#default_value' => (!empty($preset['settings']['video_extension'])) ? $preset['settings']['video_extension'] : 'flv'
  );
  $form['settings']['video']['video_codec'] = array(
    '#type' => 'select',
    '#title' => t('Video codec'),
    '#description' => t('The video codec used in the video file can affect the ability to play the video on certain devices. The default codec used is H.264.'),
    '#options' => $codecs['encode']['video'],
    '#default_value' => (!empty($preset['settings']['video_codec'])) ? $preset['settings']['video_codec'] : 'h264'
  );
  $form['settings']['video']['video_preset'] = array(
    '#type' => 'select',
    '#title' => t('Video preset'),
    '#description' => t('A preset file contains a sequence of option=value pairs, one for each line, specifying a sequence of options which would be awkward to specify on the command line. Lines starting with the hash (\'#\') character are ignored and are used to provide comments. Check the \`ffpresets\' directory in the FFmpeg source tree for examples. See the !doc', array('!doc' => l(t('FFmpeg documentation'), 'http://ffmpeg.org/ffmpeg.html#TOC13'))),
    '#options' => array(
      'none' => 'None',
      'libx264-baseline' => 'libx264-baseline',
      'libx264-default' => 'libx264-default',
      'libx264-fast' => 'libx264-fast',
      'libx264-faster' => 'libx264-faster',
      'libx264-hq' => 'libx264-hq',
      'libx264-ipod320' => 'libx264-ipod320',
      'libx264-ipod640' => 'libx264-ipod640',
      'libx264-main' => 'libx264-main',
      'libx264-max' => 'libx264-max',
      'libx264-medium' => 'libx264-medium',
      'libx264-normal' => 'libx264-normal',
      'libx264-slow' => 'libx264-slow',
      'libx264-slower' => 'libx264-slower',
      'libx264-superfast' => 'libx264-superfast',
      'libx264-ultrafast' => 'libx264-ultrafast',
      'libx264-veryfast' => 'libx264-veryfast',
      'libvpx-1080p' => 'libvpx-1080p',
      'libvpx-1080p50_60' => 'libvpx-1080p50_60',
      'libvpx-360p' => 'libvpx-360p',
      'libvpx-720p' => 'libvpx-720p',
      'libvpx-720p50_60' => 'libvpx-720p50_60',
      'libx264-lossless_fast' => 'libx264-lossless_fast',
      'libx264-lossless_max' => 'libx264-lossless_max',
      'libx264-lossless_medium' => 'libx264-lossless_medium',
      'libx264-lossless_slow' => 'libx264-lossless_slow',
      'libx264-lossless_slower' => 'libx264-lossless_slower',
      'libx264-lossless_ultrafast' => 'libx264-lossless_ultrafast'
    ),
    '#default_value' => (!empty($preset['settings']['video_preset'])) ? $preset['settings']['video_preset'] : 2
  );
  $form['settings']['video']['video_quality'] = array(
    '#type' => 'select',
    '#title' => t('Video quality'),
    '#description' => t('A target video quality. Affects bitrate and file size.'),
    '#options' => array(
      'none' => 'None',
      1 => '1 - Poor quality (smaller file)',
      2 => '2',
      3 => '3 (default)',
      4 => '4',
      5 => '5 - High quality (larger file)'
    ),
    '#default_value' => (!empty($preset['settings']['video_quality'])) ? $preset['settings']['video_quality'] : 2
  );
  $form['settings']['video']['video_speed'] = array(
    '#type' => 'select',
    '#title' => t('Video speed'),
    '#description' => t('Speed of encoding. Affects compression.'),
    '#options' => array(
      'none' => 'None',
      1 => '1 - Slow (better compression)',
      2 => '2',
      3 => '3 (default)',
      4 => '4',
      5 => '5 - Fast (worse compression)'
    ),
    '#default_value' => (!empty($preset['settings']['video_speed'])) ? $preset['settings']['video_speed'] : 2
  );
  $form['settings']['video']['wxh'] = array(
    '#type' => 'select',
    '#title' => t('Dimensions'),
    '#description' => t('Select the desired widthxheight of the video player. You can add your own dimensions from !settings.', array('!settings' => l(t('Video metadata settings'), 'admin/config/media/video/metadata'))),
    '#default_value' => !empty($preset['settings']['wxh']) ? $preset['settings']['wxh'] : '640x480',
    '#options' => video_explode("\n", variable_get("video_metadata_dimensions", video_default_dimensions()))
  );
  $form['settings']['video']['video_aspectmode'] = array(
    '#type' => 'select',
    '#title' => t('Aspect mode'),
    '#description' => t('What to do when aspect ratio of input file does not match the target width/height aspect ratio.'),
    '#options' => array(
      'preserve' => 'Preserve aspect ratio (default)',
      'crop' => 'Crop to fit output aspect ratio',
      'pad' => 'Pad (letterbox) to fit output aspect ratio',
      'stretch' => 'Stretch (distort) to output aspect ratio'
    ),
    '#default_value' => (!empty($preset['settings']['video_aspectmode'])) ? $preset['settings']['video_aspectmode'] : 'preserve'
  );
  $form['settings']['video']['video_upscale'] = array(
    '#type' => 'checkbox',
    '#title' => t('Upscale'),
    '#description' => t('If the input file is smaller than the target output, should the file be upscaled to the target size?'),
    '#default_value' => !empty($preset['settings']['video_upscale']) ? $preset['settings']['video_upscale'] : ''
  );
  // audio settings
  $form['settings']['audio'] = array(
    '#type' => 'fieldset',
    '#title' => t('Audio settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['settings']['audio']['audio_codec'] = array(
    '#type' => 'select',
    '#title' => t('Audio codec'),
    '#description' => t('The audio codec to be used.'),
    '#options' => $codecs['encode']['audio'],
    '#default_value' => (!empty($preset['settings']['audio_codec'])) ? $preset['settings']['audio_codec'] : 'h264'
  );
  $form['settings']['audio']['audio_quality'] = array(
    '#type' => 'select',
    '#title' => t('Audio quality'),
    '#description' => t('A target audio quality. Affects bitrate and file size.'),
    '#options' => array(
      'none' => 'None',
      1 => '1 - Poor quality (smaller file)',
      2 => '2',
      3 => '3 (default)',
      4 => '4',
      5 => '5 - High quality (larger file)'
    ),
    '#default_value' => (!empty($preset['settings']['audio_quality'])) ? $preset['settings']['audio_quality'] : 2
  );
  // advanced video settings
  $form['settings']['adv_video'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced video settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['settings']['adv_video']['deinterlace'] = array(
    '#type' => 'select',
    '#title' => t('Deinterlace'),
    '#description' => t('Note that detect mode will auto-detect and deinterlace interlaced content.'),
    '#options' => array(
      'detect' => 'Detect (default)',
      'on' => 'On (reduces quality of non-interlaced content)',
      'off' => 'Off'
    ),
    '#default_value' => (!empty($preset['settings']['deinterlace'])) ? $preset['settings']['deinterlace'] : 'detect'
  );
  $form['settings']['adv_video']['max_frame_rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum frame rate'),
    '#description' => t('A maximum frame rate cap (in frames per second).'),
    '#default_value' => !empty($preset['settings']['max_frame_rate']) ? $preset['settings']['max_frame_rate'] : ''
  );
  $form['settings']['adv_video']['frame_rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Frame rate'),
    '#description' => t('Force a specific output frame rate (in frames per second). For best quality, do not use this setting.'),
    '#default_value' => !empty($preset['settings']['frame_rate']) ? $preset['settings']['frame_rate'] : ''
  );
  $form['settings']['adv_video']['keyframe_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Key frame interval'),
    '#description' => t('By default, a keyframe will be created at most every 250 frames. Specifying a different keyframe interval will allow you to create more or fewer keyframes in your video. A greater number of keyframes will increase the size of your output file, but will allow for more precise scrubbing in most players. Keyframe interval should be specified as a positive integer. For example, a value of 100 will create a keyframe every 100 frames.'),
    '#default_value' => !empty($preset['settings']['keyframe_interval']) ? $preset['settings']['keyframe_interval'] : ''
  );
  $form['settings']['adv_video']['video_bitrate'] = array(
    '#type' => 'textfield',
    '#title' => t('Video bitrate'),
    '#description' => t('A target bitrate in kbps. Not necessary if you select a Video Quality setting, unless you want to target a specific bitrate.'),
    '#default_value' => !empty($preset['settings']['video_bitrate']) ? $preset['settings']['video_bitrate'] : '',
  );
  $form['settings']['adv_video']['bitrate_cap'] = array(
    '#type' => 'textfield',
    '#title' => t('Bitrate cap'),
    '#description' => t('A bitrate cap in kbps, used for streaming servers.'),
    '#default_value' => !empty($preset['settings']['bitrate_cap']) ? $preset['settings']['bitrate_cap'] : ''
  );
  $form['settings']['adv_video']['buffer_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Buffer size'),
    '#description' => t('The buffer size for the bitrate cap in kbps.'),
    '#default_value' => !empty($preset['settings']['buffer_size']) ? $preset['settings']['buffer_size'] : ''
  );
  $form['settings']['adv_video']['one_pass'] = array(
    '#type' => 'checkbox',
    '#title' => t('One pass'),
    '#description' => t('Force one-pass encoding when targeting a specific video_bitrate. TRUE or FALSE.'),
    '#default_value' => !empty($preset['settings']['one_pass']) ? $preset['settings']['one_pass'] : ''
  );
  $form['settings']['adv_video']['skip_video'] = array(
    '#type' => 'checkbox',
    '#title' => t('Skip video'),
    '#description' => t('The video track will be omitted from the output. You can still specify a video format, however, no video track will be present in the resulting file.'),
    '#default_value' => !empty($preset['settings']['skip_video']) ? $preset['settings']['skip_video'] : ''
  );
  // advanced audio settings
  $form['settings']['adv_audio'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced audio settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['settings']['adv_audio']['audio_bitrate'] = array(
    '#type' => 'textfield',
    '#title' => t('Audio bitrate'),
    '#description' => t('The overall audio bitrate specified as kilobits per second (kbps, e.g. 96 or 160). This value can\'t exceed 160 kbps per channel. 96-160 is usually a good range for stereo output.'),
    '#default_value' => !empty($preset['settings']['audio_bitrate']) ? $preset['settings']['audio_bitrate'] : ''
  );
  $form['settings']['adv_audio']['audio_channels'] = array(
    '#type' => 'select',
    '#title' => t('Audio channels'),
    '#description' => t('By default we will choose the lesser of the number of audio channels in the input file or 2 (stereo).'),
    '#options' => array(
      1 => '1 - Mono',
      2 => '2 - Stereo (default)'
    ),
    '#default_value' => (!empty($preset['settings']['audio_channels'])) ? $preset['settings']['audio_channels'] : 2
  );
  $form['settings']['adv_audio']['audio_sample_rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Audio sample rate'),
    '#description' => t('The sample rate of the audio in hertz. Manually setting this may cause problems, depending on the selected bitrate and number of channels.'),
    '#default_value' => !empty($preset['settings']['audio_sample_rate']) ? $preset['settings']['audio_sample_rate'] : ''
  );
  $form['settings']['adv_audio']['skip_audio'] = array(
    '#type' => 'checkbox',
    '#title' => t('Skip audio'),
    '#description' => t('The audio track will be omitted from the output. You must specify a video format and no audio track will be present in the resulting file.'),
    '#default_value' => !empty($preset['settings']['skip_audio']) ? $preset['settings']['skip_audio'] : ''
  );
  // Watermark
//  $form['settings']['warermark'] = array(
//    '#type' => 'fieldset',
//    '#title' => t('Video watermark'),
//    '#collapsible' => TRUE,
//    '#collapsed' => ($preset['settings']['video_enable_watermark'] == TRUE) ? FALSE : TRUE,
//  );
//  $form['settings']['warermark']['video_enable_watermark'] = array(
//    '#type' => 'checkbox',
//    '#title' => t('<b>Enable watermark video</b>'),
//    '#default_value' => !empty($preset['settings']['video_enable_watermark']) ? $preset['settings']['video_enable_watermark'] : FALSE,
//    '#description' => t('In visible digital watermarking, the information is
//        visible in the video. Typically, the information is text or a
//        logo, which identifies the owner of the media. When a television broadcaster adds its logo to the
//        corner of transmitted video, this also is a visible watermark.')
//  );
//  $form['settings']['warermark']['video_watermark_fid'] = array(
//    '#type' => 'managed_file',
//    '#title' => t('Upload watermark image'),
//    '#description' => t('Watermark image should be a PNG or JPG image.'),
//    '#default_value' => !empty($preset['settings']['video_watermark_fid']) ? $preset['settings']['video_watermark_fid'] : 'http://heidisoft.com/watermark_drupal_40x40.png',
//    '#upload_location' => 'public://videos/warermark',
//  );
//  $form['settings']['warermark']['video_warermarkx'] = array(
//    '#type' => 'textfield',
//    '#title' => t('X-axis'),
//    '#description' => t('Where to place the watermark, on the x axis..'),
//    '#default_value' => !empty($preset['settings']['video_warermarkx']) ? $preset['settings']['video_warermarkx'] : 5,
//  );
//  $form['settings']['warermark']['video_warermarky'] = array(
//    '#type' => 'textfield',
//    '#title' => t('Y-axis'),
//    '#description' => t('Where to place the watermark, on the y axis.'),
//    '#default_value' => !empty($preset['settings']['video_warermarky']) ? $preset['settings']['video_warermarky'] : 5,
//  );
//  $form['settings']['warermark']['video_warermark_position'] = array(
//    '#title' => t('Position'),
//    '#type' => 'select',
//    '#options' => array(
//      'top-left' => 'top-left',
//      'top-middle' => 'top-middle',
//      'top-right' => 'top-right',
//      'center-left' => 'center-left',
//      'center-middle' => 'center-middle',
//      'center-right' => 'center-right',
//      'bottom-left' => 'bottom-left',
//      'bottom-middle' => 'bottom-middle',
//      'bottom-right' => 'bottom-right'
//    ),
//    '#default_value' => !empty($preset['settings']['video_warermark_position']) ? $preset['settings']['video_warermark_position'] : 'bottom-right',
//    '#weight' => 20,
//  );
  // video optimizations
  $form['settings']['vid_optimization'] = array(
    '#type' => 'fieldset',
    '#title' => t('Video optimization'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['settings']['vid_optimization']['autolevels'] = array(
    '#type' => 'checkbox',
    '#title' => t('Autolevels'),
    '#description' => t('Automatic brightness / contrast correction.'),
    '#default_value' => !empty($preset['settings']['autolevels']) ? $preset['settings']['autolevels'] : ''
  );
  $form['settings']['vid_optimization']['deblock'] = array(
    '#type' => 'checkbox',
    '#title' => t('Deblock'),
    '#description' => t('Apply deblocking filter. Useful for highly compressed or blocky input videos.'),
    '#default_value' => !empty($preset['settings']['deblock']) ? $preset['settings']['deblock'] : ''
  );
  $form['settings']['vid_optimization']['denoise'] = array(
    '#type' => 'select',
    '#title' => t('Denoise'),
    '#description' => t('Apply denoise filter. Generally results in slightly better compression and slightly slower encoding. Beware of any value higher than "Weak" (unless you\'re encoding animation).'),
    '#options' => array(
      0 => 'None',
      'weak' => 'Weak - usually OK for general use',
      'medium' => 'Medium',
      'strong' => 'Strong - beware',
      'strongest' => 'Strongest - beware, except for Anime'
    ),
    '#default_value' => (!empty($preset['settings']['denoise'])) ? $preset['settings']['denoise'] : 2
  );
  // Create clip
  $form['settings']['create_clip'] = array(
    '#type' => 'fieldset',
    '#title' => t('Create clip'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['settings']['create_clip']['clip_start'] = array(
    '#type' => 'textfield',
    '#title' => t('Start clip'),
    '#description' => t('The starting point of a subclip (in timecode or number of seconds).'),
    '#default_value' => !empty($preset['settings']['clip_start']) ? $preset['settings']['clip_start'] : ''
  );
  $form['settings']['create_clip']['clip_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Clip length'),
    '#description' => t('The length of the subclip (in timecode or number of seconds).'),
    '#default_value' => !empty($preset['settings']['clip_length']) ? $preset['settings']['clip_length'] : ''
  );
  return $form;
}

/**
 * Menu callback: video Module administration.
 */
function video_presets_overview() {
  $presets = video_preset_get_presets();

  if (count($presets) > 0) {
    $header = array(t('Preset Name'), t('Description'), array('data' => t('Operations'), 'colspan' => '3'));
    $rows = array();
    foreach ($presets as $pid => $preset) {
      $row = array();
      $row[] = check_plain($preset['name']);
      $row[] = array('data' => filter_xss_admin($preset['description']), 'class' => 'description');
      $row[] = array('data' => l(t('edit'), 'admin/config/media/video/presets/preset/' . $preset['name']));

      if (!isset($preset['default'])) {
        $row[] = array('data' => l(t('delete'), 'admin/config/media/video/presets/preset/' . $preset['name'] . '/delete'));
      }

      $row[] = array('data' => l(t('export'), 'admin/config/media/video/presets/preset/' . $preset['name'] . '/export'));
      $rows[] = $row;
    }
    return theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {
    return t('No presets were found. Please use the !create_link link to create a new video preset, or upload an existing Feature to your modules directory.', array('!create_link' => l(t('Create Video Preset'), 'admin/config/media/video/presets/add')));
  }
}

/**
 * Preset form
 */
function video_preset_form($form, &$form_state, $preset = FALSE) {
  $form = array();
  // basic preset details
  $form['preset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Preset information'),
    '#weight' => -10,
  );
  if (isset($preset['pid'])) {
    $form['preset']['pid'] = array(
      '#type' => 'value',
      '#value' => $preset['pid'],
    );
  }
  elseif (video_is_default_preset($preset['name'])) {
    $form['preset']['default'] = array(
      '#type' => 'value',
      '#value' => TRUE,
    );
  }
  $form['preset']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Preset name'),
    '#maxlength' => VIDEO_PRESET_MAX_LENGTH,
    '#description' => t('Spaces are NOT allowed. The only permitted punctuation is period, hyphen, apostrophe, and underscore.'),
    '#required' => TRUE,
    '#weight' => -10,
    '#element_validate' => array('_video_preset_name_validate'),
    '#default_value' => !empty($preset['name']) ? $preset['name'] : ''
  );

  $form['preset']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('Add a brief description to your preset name.'),
    '#weight' => -9,
    '#default_value' => !empty($preset['description']) ? $preset['description'] : ''
  );

  // Preset settings
  $form += video_preset_default_form($form, $form_state, $preset);

  // Buttons
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save preset'),
    '#weight' => 30,
  );

  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#weight' => 40,
    '#submit' => array('video_preset_delete_submit'),
  );

  // Add the form submit handler.
  $form['#submit'][] = 'video_preset_submit';
  $form['#validate'][] = 'video_preset_validate';
  return $form;
}

/**
 * Validation for the preset form.
 */
function video_preset_validate($form, &$form_state) {
}

/**
 * Flatten the settings array.
 */
function video_preset_array_flatten($array, $return=array()) {
  foreach ($array as $key => $value) {
    if (is_array($value)) {
      $return = video_preset_array_flatten($value, $return);
    }
    else {
      $return[$key] = $value;
    }
  }
  return $return;
}

/**
 * Submit handler for the preset form.
 */
function video_preset_submit($form, &$form_state) {
  $preset = array();
  if (isset($form_state['values']['pid'])) {
    $preset['pid'] = $form_state['values']['pid'];
  }
  // save watermark image if exists
  // Load the file via file.fid.
  if (!empty($form_state['values']['video_watermark_fid'])) {
    $file = file_load($form_state['values']['video_watermark_fid']);
    // Change status to permanent.
    $file->status = FILE_STATUS_PERMANENT;
    // Save.
    file_save($file);
  }
  $preset['name'] = $form_state['values']['name'];
  $preset['description'] = $form_state['values']['description'];
  // unset unwanted values saved to database
  unset($form_state['values']['pid'], $form_state['values']['name'], $form_state['values']['description'], $form_state['values']['submit'], $form_state['values']['delete'], $form_state['values']['form_build_id'], $form_state['values']['form_token'], $form_state['values']['form_id'], $form_state['values']['op']);
  $preset['settings'] = video_preset_array_flatten($form_state['values']);

  // Save this preset.
  video_preset_save($preset);
  drupal_set_message(t('Preset %preset successfully saved.', array('%preset' => $preset['name'])));
  $form_state['redirect'] = 'admin/config/media/video/presets';
}

/**
 * Button sumit function: handle the 'Delete' button on the preset form.
 */
function video_preset_delete_submit($form, &$form_state) {
  $preset['name'] = $form_state['values']['name'];
  $form_state['redirect'] = 'admin/config/media/video/presets/preset/' . $preset['name'] . '/delete';
}

/**
 * Menu callback; delete a single preset.
 */
function video_preset_delete_confirm($form, &$form_state, $preset) {
  $form['name'] = array('#type' => 'value', '#value' => $preset['name']);
  return confirm_form($form, t('Are you sure you want to delete the preset %name?', array('%name' => $preset['name'])), 'admin/config/media/video/presets', '<p>' . t('This action cannot be undone.') . '</p>', t('Delete'));
}

/**
 * Execute preset deletion
 */
function video_preset_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    video_preset_delete($form_state['values']['name']);
  }

  $form_state['redirect'] = 'admin/config/media/video/presets';
}

/**
 * Validation for the preset form.
 */
function _video_preset_name_validate($element, &$form_state) {
  $error = video_validate_preset_name($form_state['values']['name']);
  if ($error && !isset($form_state['values']['pid']) && !isset($form_state['values']['default'])) {
    form_set_error('name', $error);
  }
}

/**
 * Import's a preset.
 */
function video_preset_import_form($form, &$form_state) {
  $form['preset'] = array(
    '#title' => t('Preset code'),
    '#type' => 'textarea',
    '#rows' => 10,
    '#description' => t('Copy the export text and paste it into this text field to import a new preset.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import preset'),
    '#weight' => 30,
  );

  $form['#submit'][] = 'video_preset_import_submit';
  $form['#validate'][] = 'video_preset_import_validate';

  return $form;
}

/**
 * Validation for the preset import form.
 */
function video_preset_import_validate($form, &$form_state) {
  $preset = '';

  // Get the preset that they declared in the text field.
  ob_start();
  eval($form_state['values']['preset']);
  ob_end_clean();

  if (is_array($preset)) {
    $name = isset($preset['name']) ? $preset['name'] : '';
    if ($error = video_validate_preset_name($name)) {
      form_set_error('name', $error);
    }
  }
  else {
    form_set_error('name', 'Invalid preset import.');
  }

  $form_state['preset'] = &$preset;
}

/**
 * Submit handler for the preset import form.
 */
function video_preset_import_submit($form, &$form_state) {
  $preset = $form_state['preset'];
  video_preset_save($preset);
  $form_state['redirect'] = 'admin/config/media/video/presets/preset/' . $preset['name'];
}

/**
 * The preset export form.
 */
function video_preset_export_form($form, &$form_state, $preset) {
  // Unset the unnecessary elements.
  unset($preset['pid']);
  unset($preset['default']);

  // Get the code string representation.
  $code = var_export($preset, TRUE);

  // Make sure to format the arrays like drupal.
  $code = str_replace("=> \n  ", '=> ', $code);

  // Add the preset variable.
  $code = '$preset = ' . $code . ';';

  $lines = substr_count($code, "\n");
  $form['export'] = array(
    '#title' => t('Export preset'),
    '#type' => 'textarea',
    '#value' => $code,
    '#rows' => $lines,
    '#description' => t('Copy the export text and paste it into another preset using the import function.'),
  );

  // Return the form.
  return $form;
}

/**
 * Get's the player settings for the given player.
 */
function video_player_settings_form($preset) {
  $settings_form = $preset['player'] . '_player_settings_form';
  if (function_exists($settings_form)) {
    return $settings_form($preset);
  }
  return array();
}

/**
 * Title callback for a preset.
 */
function video_preset_page_title($preset) {
  return $preset['name'];
}

/**
 * Saves a new preset.
 */
function video_preset_save($preset) {
  // If they provide the pid, then this needs to be an update.
  $pid = (isset($preset['pid']) && $preset['pid']) ? array('pid') : array();

  // Save or update a preset.
  drupal_write_record('video_preset', $preset, $pid);
}

/**
 * Returns the default presets.
 */
function video_preset_get_default_presets() {
  $default_presets = module_invoke_all('video_default_presets');
  drupal_alter('video_default_presets', $default_presets);
  return $default_presets;
}

/**
 * Gets a list of all presets.
 */
function video_preset_get_presets() {
  $presets = array();
  $normal_presets = array();

  // Get all the presets from the database.
  $result = db_select('video_preset', 'p')
      ->fields('p')
      ->orderBy('p.name', 'ASC')
      ->execute();

  // Iterate through all the presets and structure them in an array.
  foreach ($result as $preset) {
    $preset = (array) $preset;
    $preset['settings'] = $preset['settings'] ? unserialize($preset['settings']) : array();
    $presets[$preset['pid']] = $preset;
    $normal_presets[$preset['name']] = $preset['pid'];
  }

  // Now allow other modules to add their default presets.
  foreach (video_preset_get_default_presets() as $preset) {
    // adding default TRUE
    $preset['default'] = TRUE;
    if (!empty($preset['name']) && !isset($normal_presets[$preset['name']])) {
      $presets[$preset['name']] = $preset;
    }
  }

  return $presets;
}

/**
 * Get's the preset.
 */
function video_get_preset($preset_name) {
  // Get the preset from the database.
  $preset = db_select('video_preset', 'p')
      ->fields('p')
      ->condition('p.name', $preset_name)
      ->execute()
      ->fetchAssoc();
  if ($preset) {
    $preset['settings'] = $preset['settings'] ? unserialize($preset['settings']) : array();
  }
  else {
    // Get all of the default presets.
    $default_presets = video_preset_get_default_presets();

    // Check to see if this preset exists.
    if (isset($default_presets[$preset_name])) {
      // Make this our preset
      $preset = $default_presets[$preset_name];
    }
  }

  // Return the preset.
  return $preset;
}

/**
 * Deletes a preset
 */
function video_preset_delete($preset_name) {
  // Delete the preset.
  db_delete('video_preset')
      ->condition('name', $preset_name)
      ->execute();

  // Rebuild Theme Registry
  drupal_theme_rebuild();
}

/**
 * Checks to see if another preset is already taken.
 */
function video_preset_name_exists($preset_name) {

  // Get the default presets.
  $default_presets = video_preset_get_default_presets();

  // See if there is a default preset name.
  if ($default_presets && isset($default_presets[$preset_name])) {
    return TRUE;
  }
  else {
    return (bool) db_select('video_preset', 'p')
            ->fields('p')
            ->condition('p.name', $preset_name)
            ->range(0, 1)
            ->execute()
            ->fetchField();
  }
}

/**
 * Verify the syntax of the given prefix name.
 *
 * Borrowed from the user.module.   :)
 */
function video_validate_preset_name($name) {
  if (!$name) {
    return t('You must enter a preset.');
  }
  if (video_preset_name_exists($name)) {
    return t('The preset name %name is already taken.', array('%name' => $name));
  }
  if (strpos($name, ' ') !== FALSE) {
    return t('The preset name cannot contain a space.');
  }
  if (is_numeric($name[0])) {
    return t('The preset name cannot begin with a number.');
  }
  if (preg_match('/[^a-z0-9_]/i', $name)) {
    return t('The preset name contains an illegal character.');
  }
  if (drupal_strlen($name) > VIDEO_PRESET_MAX_LENGTH) {
    return t('The preset name %name is too long: it must be %max characters or less.', array('%name' => $name, '%max' => VIDEO_PRESET_MAX_LENGTH));
  }
}

/**
 * Check for default preset
 */
function video_is_default_preset($preset_name) {
  // Get the default presets.
  $default_presets = video_preset_get_default_presets();

  // See if there is a default preset name.
  return ($default_presets && isset($default_presets[$preset_name]));
}
