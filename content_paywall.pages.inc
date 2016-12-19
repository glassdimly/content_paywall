<?php

function content_paywall_admin_settings_form() {
  $form['content_paywall_is_debug'] = array(
    '#title' => t('Debug Mode?'),
    '#type' => 'checkbox',
    '#description' => t('Check this box turn on debug mode, adding subscriber categories to body for debugging.'),
    '#default_value' => variable_get('content_paywall_is_debug', 0)
  );

  $form['content_paywall_role'] = array(
    '#title' => t('Premium Role'),
    '#type' => 'select',
    '#required' => TRUE,
    '#description' => t('The role that can view premium and all metered content'),
    '#default_value' => variable_get('content_paywall_role', ''),
    '#options' => user_roles(),
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

    'content_paywall_metered_access_reset' => array(
      '#title' => t('Metered content access timeperiod'),
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
      '#default_value' => variable_get('content_paywall_hover_bother_frequency', '0')
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
  return system_settings_form($form);
}

//Function definition

function timeAgo($time_ago)
{
  $time_ago = date(DATE_RSS, $time_ago);
  $time_ago = strtotime($time_ago);
  $cur_time   = time();
  $time_elapsed   = $cur_time - $time_ago;
  $seconds    = $time_elapsed ;
  $minutes    = round($time_elapsed / 60 );
  $hours      = round($time_elapsed / 3600);
  $days       = round($time_elapsed / 86400 );
  $weeks      = round($time_elapsed / 604800);
  $months     = round($time_elapsed / 2600640 );
  $years      = round($time_elapsed / 31207680 );
  // Seconds
  if($seconds <= 60){
    return "just now";
  }
  //Minutes
  else if($minutes <=60){
    if($minutes==1){
      return "one minute ago";
    }
    else{
      return "$minutes minutes ago";
    }
  }
  //Hours
  else if($hours <=24){
    if($hours==1){
      return "an hour ago";
    }else{
      return "$hours hrs ago";
    }
  }
  //Days
  else if($days <= 7){
    if($days==1){
      return "yesterday";
    }else{
      return "$days days ago";
    }
  }
  //Weeks
  else if($weeks <= 4.3){
    if($weeks==1){
      return "a week ago";
    }else{
      return "$weeks weeks ago";
    }
  }
  //Months
  else if($months <=12){
    if($months==1){
      return "a month ago";
    }else{
      return "$months months ago";
    }
  }
  //Years
  else{
    if($years==1){
      return "one year ago";
    }else{
      return "$years years ago";
    }
  }
  }