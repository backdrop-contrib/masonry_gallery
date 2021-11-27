<?php
/**
 * @file
 * Contains the Masonry Gallery style plugin.
 */
class views_plugin_style_masonry_gallery extends views_plugin_style {

  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['gap']     = array('default' => 10);
    $options['columns'] = array(
      'default' => array(
        'auto'        => TRUE,
        'desktop'     => 5,
        'laptop'      => 4,
        'tablet'      => 3,
        'smartphone'  => 2,
        'phone'       => 1,
      ),
    );
    $options['caption'] = array(
      'default' => array(
        'source'       => '',
        'display'      => 'static',
        'height'       => 32,
        'no_wrap'      => TRUE,
        'no_padding_h' => FALSE,
      ),
    );
    $options['advanced'] = array(
      'default' => array(
        'threshold'    => 32,
      ),
    );

    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {

    $form['gap'] = array(
      '#title' => t('Gap'),
      '#description' => t('Amount of space between items.'),
      '#type' => 'number',
      '#min' => 0,
      '#max' => 99,
      '#field_suffix' => t('pix'),
      '#default_value' => $this->options['gap'],
    );

    $form['columns'] = array(
      '#type' => 'fieldset',
      '#title' => t('Columns settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['columns']['auto'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto columns'),
      '#description' => t("Automatically calculate number of columns for each screen size based on provided Desktop columns."),
      '#default_value' => $this->options['columns']['auto'],
    );

    $form['columns']['desktop'] = array(
      '#title' => t('Desktop') . ' ',
      '#type' => 'number',
      '#min' => 1,
      '#max' => 99,
      '#size' => 2,
      '#description' => t("Number of columns for large screens (above 1200px or 75em)."),
      '#default_value' => $this->options['columns']['desktop'],
    );

    $form['columns']['laptop'] = array(
      '#title' => t('Laptop') . ' ',
      '#type' => 'number',
      '#min' => 1,
      '#max' => 99,
      '#size' => 2,
      '#description' => t("Number of columns for medium sized screens (above 992px or 62em)."),
      '#default_value' => $this->options['columns']['laptop'],
      '#states' => array(
        'invisible' => array(
          ':input[name="style_options[columns][auto]"]' => array('checked' => TRUE),
        ),
      ),
    );

    $form['columns']['tablet'] = array(
      '#title' => t('Tablet') . ' ',
      '#type' => 'number',
      '#min' => 1,
      '#max' => 99,
      '#size' => 2,
      '#description' => t("Number of columns for tablets and smartphones (above 768px or 48em)."),
      '#default_value' => $this->options['columns']['tablet'],
      '#states' => array(
        'invisible' => array(
          ':input[name="style_options[columns][auto]"]' => array('checked' => TRUE),
        ),
      ),
    );

    $form['columns']['smartphone'] = array(
      '#title' => t('Smartphone') . ' ',
      '#type' => 'number',
      '#min' => 1,
      '#max' => 99,
      '#size' => 2,
      '#description' => t("Number of columns for smartphones (above 576px or 36em)."),
      '#default_value' => $this->options['columns']['smartphone'],
      '#states' => array(
        'invisible' => array(
          ':input[name="style_options[columns][auto]"]' => array('checked' => TRUE),
        ),
      ),
    );

    $form['columns']['phone'] = array(
      '#title' => t('Phone'),
      '#type' => 'number',
      '#min' => 1,
      '#max' => 99,
      '#size' => 2,
      '#description' => t("Number of columns for smallest screens."),
      '#default_value' => $this->options['columns']['phone'],
      '#states' => array(
        'invisible' => array(
          ':input[name="style_options[columns][auto]"]' => array('checked' => TRUE),
        ),
      ),
    );

    // Caption options fieldset.
    $form['caption'] = array(
      '#type' => 'fieldset',
      '#title' => t('Caption settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    // Set options list for caption selector.
    $caption_options = array(
      'alt'        => t('Image alt tag'),
      'title'      => t('Image title tag'),
    );
    // Detect if text or title field is present, and add it to the options list.
    $text_field = $this->get_first_non_excluded_text_field()['handler'];
    if (!empty($text_field)) {
      $caption_options['text'] = $text_field->ui_name();
    }
    // Create caption selector form.
    $form['caption']['source'] = array(
      '#type' => 'select',
      '#title' => t('Text source'),
      '#options' => $caption_options,
      '#empty_option' => t('None'),
      '#default_value' => $this->options['caption']['source'],
      '#description' => t('A text or title field can be added to the view, to be used as caption.'),
    );

    $caption_display = array(
      'static' => t('Always visible, below the image'),
      'hover'  => t('Hidden initially, shown on mouse hover'),
    );

    $form['caption']['display'] = array(
      '#type' => 'select',
      '#title' => t('Caption display mode'),
      '#default_value' => $this->options['caption']['display'],
      '#options' => $caption_display,
      '#states' => array(
        'disabled' => array(
          ':input[name="style_options[caption][source]"]' => array('value' => ''),
        ),
      ),
    );

    $form['caption']['height'] = array(
      '#type' => 'number',
      '#title' => t('Caption height'),
      '#min' => 1,
      '#max' => 999,
      '#step' => 1,
      '#size' => 3,
      '#field_suffix' => t('px'),
      '#default_value' => $this->options['caption']['height'],
      '#states' => array(
        'disabled' => array(
          ':input[name="style_options[caption][source]"]' => array('value' => ''),
        ),
      ),
    );

    $form['caption']['no_wrap'] = array(
      '#type' => 'checkbox',
      '#title' => t('No text wrap'),
      '#default_value' => $this->options['caption']['no_wrap'],
      '#description' => t('Truncate caption text when it does not fit column width. Prevents text wrapping.'),
      '#states' => array(
        'disabled' => array(
          ':input[name="style_options[caption][source]"]' => array('value' => ''),
        ),
      ),
    );

    $form['caption']['no_padding_h'] = array(
      '#type' => 'checkbox',
      '#title' => t('No horisontal padding'),
      '#default_value' => $this->options['caption']['no_padding_h'],
      '#description' => t('Horizontally align the caption with the gallery image. This overrides the padding setting in the <a href="@link" target="_blank">configuration page</a> (currently set to <em>@global_padding_setting em</em>).', array('@link' => url('admin/config/media/masonry_gallery'), '@global_padding_setting' => config_get('masonry_gallery.settings', 'caption_padding'))),
      '#states' => array(
        'disabled' => array(
          ':input[name="style_options[caption][source]"]' => array('value' => ''),
        ),
      ),
    );

    // Advanced options fieldset.
    $form['advanced'] = array(
      '#type' => 'fieldset',
      '#title' => t('Advanced settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['advanced']['threshold'] = array(
      '#title' => t('Sorting threshold'),
      '#description' => t('Set balance between left to right ordering of items and height sorting. Zero value sets strict sorting of items by their height. Higher values help to preserve left to right ordering but may result in more jaggy layout at the bottom.'),
      '#type' => 'number',
      '#min' => 0,
      '#max' => 99,
      '#step' => 1,
      '#size' => 2,
      '#field_suffix' => t('pix'),
      '#default_value' => $this->options['advanced']['threshold'],
    );
  }

  /**
   * Returns first non-excluded field by type.
   * Returned value is an array containing handler and field's name. Works only
   * for views_handler_field_field types that have field_info and for title fields.
   */
  protected function get_first_non_excluded_field(string $field_type) {
    $fields = $this->view->display_handler->get_handlers('field');

    foreach ($fields as $field_name => $field) {
      // Ignore excluded fields.
      if (!empty($field->options['exclude']))
        continue;

      if (isset($field->field_info) || $field_type == 'title') {
        $type = FALSE;
        if (!empty($field->field_info['type'])) {
          $type = $field->field_info['type'];
        }
        // If it is of required type, return field handler and field name.
        // Title has no field_info, so add a second condition.
        if ($field_type == $type || ($field_type == 'title' && $field_name == 'title')) {
          return array('handler' => $field, 'field_name' => $field_name);
        }
      }

    }
    return NULL;
  }

  /*
   * Returns first non excluded image field.
   */
  function get_first_non_excluded_image_field() {
    return $this->get_first_non_excluded_field('image');
  }

  /*
   * Returns first non excluded text field.
   */
  function get_first_non_excluded_text_field() {
    // Set possible text field types.
    $text_types = $this->get_text_field_types();
    // Search for each possible type of text field.
    foreach ($text_types as $text_type) {
      $field_data = $this->get_first_non_excluded_field($text_type);
      // Return the fist text field found.
      if (!empty($field_data)) {
        return $field_data;
      }
    }
    return NULL;
  }

  /**
   * Returns array of text field types.
   */
  function get_text_field_types() {
    return array('title', 'text', 'text_long', 'text_with_summary');
  }

  /**
   * Validates the view configuration.
   * Fails if there is a non-image or non-text field, or there are more
   * than one image and text fields that are not excluded from display.
   */
  function validate() {
    $errors = parent::validate();

    // Do not validate when view is being created as there is no image field
    // by default, and validation will fail.
    if (!$this->view->editing) {
      return $errors;
    }

    // Get a list of fields that have been added to the display.
    $fields = $this->display->handler->get_handlers('field');

    $image_field_count = 0;
    $text_field_count  = 0;

    foreach ($fields as $field_name => $field) {
      // Ignore fields excluded from display.
      if (!empty($field->options['exclude'])) {
        continue;
      }

      // Check field type.
      if (isset($field->field_info['type'])) {
        $field_type = $field->field_info['type'];
      }
      elseif ($field->field == 'title') {
        $field_type = 'title';
      }
      $is_image = $field_type == 'image';
      $is_text = in_array($field_type, $this->get_text_field_types());
      $is_title = $field_name == 'title';

      // If invalid field type found then validation failed.
      if (!$is_image && !$is_text && !$is_title) {
        $errors[] = t('Invalid field types found. This format requires one image field, and may optionally have one text or title field.');
        return $errors;
      }

      // Count valid fields.
      if ($is_image) {
        $image_field_count ++;
      }
      if ($is_text || $is_title) {
        $text_field_count++;
      }

      // Check if there is no more than one of each valid field types.
      if ($image_field_count > 1) {
        $errors[] = t('There is more than one image field. This format can display only one image field.');
        break;
      }
      if ($text_field_count > 1) {
        $errors[] = t('There is more than one text field. This format can display only one text or title field.');
        break;
      }
    }

    // Check if there is at least one image field.
    if ($image_field_count < 1) {
      $errors[] = t('There are no image fields. This format requires at least one image field.');
    }

    return $errors;
  }
}
