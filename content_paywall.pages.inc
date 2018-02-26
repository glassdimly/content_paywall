<?php

function content_paywall_admin_settings_form() {
  $form['content_paywall_intro'] = array(
    '#type' => 'markup',
    '#markup' => '<strong>Note:</strong> permission to access premium content must be set on the <a href="/admin/people/permissions#module-content_paywall">drupal permissions page</a>',
  );
  
  $form['content_paywall_content_types'] = array(
    '#title' => t('Node Bundles to Which Fields Should Be Added'),
    '#type' => 'checkboxes',
    '#description' => t('Select the bundles that you want to restrict access on. This will add a new field, %field_name. This field can be deleted to remove restrictions or value options can be removed.', array('%field_name' => CONTENT_PAYWALL_FIELD)),
    '#options' => node_type_get_names()
  );

  $form['content_paywall_messages'] = array(
    '#title' => t('Content Paywall Message Settings'),
    '#type' => 'fieldset',
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    'content_paywall_metered_limit' => array(
      '#title' => t('Metered Limit'),
      '#type' => 'textfield',
      '#size' => 3,
      '#field_suffix' => t('pieces of content'),
      '#required' => TRUE,
      '#description' => t('The number of pieces of metered content which can be viewed before all metered content is paywalled.'),
      '#default_value' => variable_get('content_paywall_metered_limit', 3)
    ),

    'content_paywall_metered_timeout' => array(
      '#title' => t('"Metered Timed" Timeout'),
      '#type' => 'textfield',
      '#size' => 3,
      '#field_suffix' => t('days'),
      '#description' => t('The number of days after which content with `metered_timed` value becomes `premium`'),
      '#default_value' => variable_get('content_paywall_metered_timeout', '')
    ),

    'content_paywall_premium_timeout' => array(
      '#title' => t('"Premium" Timeout'),
      '#type' => 'textfield',
      '#size' => 3,
      '#field_suffix' => t('days'),
      '#description' => t('The number of days after which content with `premium` value becomes `free`'),
      '#default_value' => variable_get('content_paywall_premium_timeout', '')
     ),

    'content_paywall_metered_access_reset' => array(
      '#title' => t('Metered content access time period'),
      '#type' => 'textfield',
      '#size' => 3,
      '#field_suffix' => t('days'),
      '#description' => t('The number of days after which the metered content cookie resets itself and users can access another round of metered content.'),
      '#default_value' => variable_get('content_paywall_metered_access_reset', '60')
    ),

    'content_paywall_hover_scrollto' => array(
      '#title' => t('Restrict to one pop-up per user session?'),
      '#type' => 'checkbox',
      '#description' => t('Pop up the paywall message once per user session.'),
      '#default_value' => variable_get('content_paywall_hover_scrollto', '0')
    ),

    'content_paywall_hover_bother_frequency' => array(
      '#title' => t('Percentage of paywall message pop-up content'),
      '#type' => 'textfield',
      '#size' => 3,
      '#field_suffix' => t('%'),
      '#description' => t('Paywall messages can be implemented with scrollto. When a paywall message is scrolled-to, the paywall message pops up. This will be randomized based on the percentage of pieces of content you wish to feature a popup. This feature requires jQuery Waypoints, jQuery UI, and dialog modules. If this setting is left at "0" the javascript handling this functionality is not added to pages.'),
      '#default_value' => variable_get('content_paywall_hover_bother_frequency', '0'),
      '#access' => FALSE // @TODO Implement this functionality.
    ),

    'content_paywall_message_metered' => array(
      '#title' => t('Metered Message'),
      '#type' => 'text_format',
      '#description' => t('The message users encounter when they are paywalled on metered content.'),
      '#format' => variable_get('content_paywall_message_premium', array('format' => 'filtered_html'))['format'],
      '#default_value' => variable_get('content_paywall_message_metered', array('value' => 'You must subscribe to see this premium content.'))['value']
    ),

    'content_paywall_message_premium' => array(
      '#title' => t('Premium Message'),
      '#type' => 'text_format',
      '#required' => TRUE,
      '#description' => t('The message users encounter when they are paywalled on premium content.'),
      '#format' => variable_get('content_paywall_message_premium', array('format' => 'filtered_html'))['format'],
      '#default_value' => variable_get('content_paywall_message_premium', array('value' => 'You must subscribe to see this premium content.'))['value']
    ),
  );

  $form['content_paywall_truncation'] = array(
    '#title' => t('Content Truncation Settings'),
    '#type' => 'fieldset',
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    'content_paywall_truncation_paragraphs' => array(
      '#type' => 'textfield',
      '#size' => 3,
      '#field_suffix' => t('paragraphs'),
      '#title' => t('Allowed Paragraph Number'),
      '#description' => t('Number of paragraphs / block-level elements displayed on premium content before truncation for non-permissioned users.'),
      '#default_value' => variable_get('content_paywall_truncation_paragraphs', 3)
    ),

    'content_paywall_paragraph_min_chars' => array(
      '#title' => t('Minimum characters per paragraph'),
      '#type' => 'textfield',
      '#size' => 3,
      '#field_suffix' => t('characters'),
      '#description' => t('A paragraph fewer than this number of characters will not be considered a paragraph.'),
      '#default_value' => variable_get('content_paywall_paragraph_min_chars', '140')
    ),

    'content_paywall_content_selector' => array(
      '#title' => t('Selector for content to truncate'),
      '#type' => 'textfield',
      '#field_prefix' => '$("',
      '#field_suffix' => '")',
      '#description' => t('Add a selector. This selector should be without quotes. It will be used to select the content which will be truncated. Example: <em>.node-contents .node-field-body</em>. <a href="https://api.jquery.com/category/selectors/">Documentation on selectors.</a>'),
      '#default_value' => variable_get('content_paywall_content_selector', '.field-name-body')
    ),

    'content_paywall_content_deselectors' => array(
      '#title' => t('Selectors to exclude in paragraph count'),
      '#type' => 'textfield',
      '#field_prefix' => t('$(":not('),
      '#field_suffix' => t(')")'),
      '#description' => t('Add a selector. This selector should be without quotes. It will be inserted within ":not(my-selector)" when considering block-level elements that are paragraphs. Add multiple if desired, and separate by commas. Example: <em>.my-exclusion-1, .this-is-a-content-insert-not-a-paragraph</em>.'),
      '#default_value' => variable_get('content_paywall_content_deselectors', '.field-name-body')
    ),

    'content_paywall_block_tags' => array(
      '#title' => t('Tags to count as paragraphs'),
      '#type' => 'textfield',
      '#field_prefix' => t('$("'),
      '#field_suffix' => t('")'),
      '#description' => t('Add tags that will be counted as paragraphs for the purposes of truncation. Comma separated. Example: <em>p,div,table</em>.'),
      '#default_value' => variable_get('content_paywall_block_tags', 'p')
    )
  );

  $form['content_paywall_is_debug'] = array(
    '#title' => t('Debug Mode?'),
    '#type' => 'checkbox',
    '#description' => t('Check this box turn on debug mode, adding subscriber categories to body for debugging.'),
    '#default_value' => variable_get('content_paywall_is_debug', 0)
  );

  $form['#submit'][] = 'content_paywall_settings_submit';

  return system_settings_form($form);
}

/**
 * Additional submit handle for the settings form.
 */
function content_paywall_settings_submit($form, &$form_state) {
  if (!empty($form_state['values']['content_paywall_content_types'])) {
    $settings = array('allowed_values' => array(
      'free' => t('Free'),
      'premium' => t('Premium'),
      'metered_permanently' => t('Metered (permanently)'),
      'metered_timed' => t('Metered (becomes premium)'),
      'premium_timed' => t('Premium (becomes free)'),
    ));
    // Create field if it doesn't exist.
    if (!($field_info = field_info_field(CONTENT_PAYWALL_FIELD))) {
          field_create_field(array(
            'field_name' => CONTENT_PAYWALL_FIELD,
            'type' => 'list_text',
            'settings' => $settings,
          ));
    }
    foreach($form_state['values']['content_paywall_content_types'] as $bundle) {
      if (empty($bundle)) {
        continue;
      }
      if (field_info_instance('node', CONTENT_PAYWALL_FIELD, $bundle)) {
        drupal_set_message(t('Refused to create field %field on %bundle because it already exists. Delete this field first and try again.', array('%field' => CONTENT_PAYWALL_FIELD, '%bundle' => $bundle)), 'warning');
        continue;
      }
      // Create requested field instance.
      field_create_instance(array(
        'field_name' => CONTENT_PAYWALL_FIELD,
        'entity_type' => 'node',
        'bundle' => $bundle,
        'label' => t('Content Paywall Trunction'),
        'description' => t('Select from these options to determine the conditions under which this content should be truncated.'),
        'required' => TRUE,
        'default_value' => array(array('value' => 'free')),
        'settings' => $settings,
      ));
      drupal_set_message(t('Created field %field on %bundle.', array('%field' => CONTENT_PAYWALL_FIELD, '%bundle' => $bundle)), 'status');
    }
  }
}