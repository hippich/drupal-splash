<?php

function splash_admin_when($form, &$form_state) {
  $splash_when = variable_get('splash_when', _splash_var_defaults('when'));
  
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
  );
  
  $form['general']['frequency'] = array(
    '#type' => 'select', 
    '#title' => t('Display with frequency'), 
    '#default_value' => $splash_when['frequency'],
    '#options' => array(
      ''				=> t('Never (off)'),
      'always'	=> t('Always'),
      'once'		=> t('Once'),
      'daily'		=> t('Daily'),
      'weekly'	=> t('Weekly'),
    ), 
    '#description' => t('How often should visitors see the splash page?'),
  );
  
  $form['general']['cookie'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force display on next visit'), 
    '#description' => t('Reset the name of the cookie to force the display of the splash on next visit.  This is useful to preview changes to the splash content in development.'),
    '#default_value' => 0,
  );
  
  /*
  if (module_exists('condition')) {
    $form = array_merge($form, module_invoke('condition', 'selection_form', $splash_when['conditions']));
  }
  */
  
  $form = system_settings_form($form);
  
  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to Defaults'),
  );
  
  unset($form['#submit']);
  
  return $form;
}

function splash_admin_when_submit($form, &$form_state) {
  $op = $form_state['values']['op'];
  if ($op == t('Reset to Defaults')) {
    variable_del('splash_when');
    
    drupal_set_message(t('The configuration options have been reset to their default values.'));
  }  
  else {
    $splash_when = variable_get('splash_when', array());
    $splash_when['frequency'] = $form_state['values']['frequency'];
    
    if ($form_state['values']['cookie']) {
      $splash_when['cookie'] = 'splash_cookie_' . time();
    }
    
    /*
    if (module_exists('condition')) {
      $splash_when['conditions'] = $form_state['values']['conditions'];
    }
    */
    variable_set('splash_when', $splash_when);
      
    drupal_set_message(t('The configuration options have been saved.'));
  }
  
  cache_clear_all();
  drupal_theme_rebuild();
}

function splash_admin_how($form, &$form_state) {
  $splash_how = variable_get('splash_how', _splash_var_defaults('how'));
  
  $options = array(
    'redirect' => t('Redirect'),
    'window' => t('Open in new window'),
  );
  if (module_exists('colorbox')) {
    $options['colorbox'] = t('Open in Colorbox');
  }
  else {
    drupal_set_message(t('This module now uses the <a href="http://drupal.org/project/colorbox">Colorbox</a> contributed module.  Please go to the <a href="http://drupal.org/project/colorbox">Colorbox project page</a> and download it if you wish to use Colorbox to display splash pages.'));
  }
  
  $form['splash_how'] = array(
    '#tree' => TRUE,
  );
  $form['splash_how']['mode'] = array(
    '#type' => 'radios',
    '#required' => TRUE,
    '#title' => t('Display mode'),
    '#options' => $options,
    '#default_value' => $splash_how['mode'],
    '#description' => t('Redirect to the splash page, open it in a new window or in a fancy !colorbox.', array('!colorbox' => l(t('Colorbox'), 'http://colorpowered.com/colorbox/'))),
  );
  $form['splash_how']['size'] = array(
    '#type' => 'textfield',
    '#title' => t('Window/Box size'),
    '#default_value' => $splash_how['size'],
    '#description' => t('Size (<code>WIDTHxHEIGHT</code>, e.g. 400x300) of the window or Colorbox.'),
  );
  
  return system_settings_form($form);
}

function splash_admin_how_validate($form, &$form_state) {
  
  if (!empty($form_state['values']['splash_how']['size']) && !preg_match('/^[1-9][0-9]*x[1-9][0-9]*$/i', $form_state['values']['splash_how']['size'])) {
    form_set_error('size', t('Invalid Window/Box size. Use <code>WIDTHxHEIGHT</code>, e.g. 400x300.'));
  } 
}

function splash_admin_what($form, &$form_state) {
  $splash_what = variable_get('splash_what', _splash_var_defaults('what'));
  
  $form['splash_what'] = array(
    '#tree' => TRUE,
  );
  
  $form['splash_what']['mode'] = array(
    '#type' => 'select',
    '#title' => t('Content mode'),
    '#options' => array(
      '' => t('Pick random path or URL from the list'),
      'sequence' => t('Pick next path or URL from the list'),
      'template' => t('Display entered text in the site template'),
      'fullscreen' => t('Display entered text/HTML full screen'),
    ),
    '#default_value' => $splash_what['mode'],
    '#description' => t('Determines how the content field below will be used.'),
  );
  
  $form['splash_what']['content'] = array(
    '#type' => 'text_format',
    '#title' => t('Content'),
    '#default_value' => isset($splash_what['content']['value']) ? $splash_what['content']['value'] : '',
    '#format' => isset($splash_what['content']['format']) ? $splash_what['content']['format'] : NULL,
    '#description' => t('Text to show or paths/URLs (one on each line) to use.  To open a non-Drupal path, use an absolute URL, i.e. http://example.com/splash.html'),
  );
  
  return system_settings_form($form);
}