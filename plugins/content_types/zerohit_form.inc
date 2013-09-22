<?php

if (module_exists('search')) {
  /**
   * Plugins are described by creating a $plugin array which will be used
   * by the system that includes this file.
   */
  $plugin = array(
    'title' => t('Ding zero-hit form'),
    'description' => t('Form showing in case of 0 search results.'),
    'single' => TRUE,
    'render callback' => 'ding_zerohit_form_content_type_render',
    'category' => t('Ting'),
    'render last' => TRUE,
    'defaults' => array(
      'type' => 'node',
      'webform_id' => 0,
    ),
  );
}

/**
 * Render the custom content type.
 */
function ding_zerohit_form_content_type_render($subtype, $conf, $panel_args, $context) {
  global $pager_page_array, $pager_total;
  $block = new stdClass();
  //dpm($conf);
  //dpm($panel_args);
  
  // Get webform
  if (isset($conf['webform_id']) && !empty($conf['webform_id'])) {
    $webform = webform_block_view($delta = 'client-block-' . $conf['webform_id']);
  }
  else {
    $webform['content'] = t('No zero-hit form to show - go <a href="@url">configure Ding Zero-hit form</a>.', array('@url' => url("admin/config/ting/zerohitform", array('absolute' => TRUE))));
  }
  
  if ($conf['type'] == 'ting_search') {
    if ($pager_total[0] == TRUE) {
      $results = drupal_static('ting_search_results');
      //dpm($results);
      $total = $results->numTotalObjects;
      $block->title = t('Zerohit (in Ting search) form text');
      $block->content = '<div class="ting-search-amount-block">ZERO-HIT (Ting search), total was ' . $total . '</div>';
      $block->content .= $webform['content'];
    }
  }
  else {
    $block->title = t('Zerohit (in Node search) form text');
    $block->content = '<div class="ting-search-amount-block">ZERO-HIT (nodes)</div>';
    $block->content .= $webform['content'];
  }
  
  return $block;
}

/**
 * Returns an edit form for custom type settings.
 */
function ding_zerohit_form_zerohit_form_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $types = array();
  foreach (search_get_info() as $module => $info) {
    $types[$module] = $info['title'];
  }

  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Search type'),
    '#options' => $types,
    '#default_value' => $conf['type'],
  );

  // Load saved settings
  $values = variable_get('ding_zerohit_form', array());

  $webform_types = webform_variable_get('webform_node_types');
  $webform_nodes = array();

  if ($webform_types) {
    $query = db_select('node', 'n');
    $query->join('node_revision', 'r', 'n.vid = r.vid');
    $query
      ->fields('n', array('nid', 'title'))
      ->condition('n.type', $webform_types, 'IN')
      ->addTag('node_access');
    $result = $query->execute();

    foreach ($result as $webform_node) {
      $webform_nodes[$webform_node->nid] = $webform_node->title;
    }
  }

  $form['webform_id'] = array(
    '#type' => 'select',
    '#title' => t('Webform ID'),
    '#options' => $webform_nodes,
    '#default_value' => isset($values['webform_id']) ? $values['webform_id'] : NULL,
    '#description' => t('Webform to use as zero-hit form'),
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Submit handler for search form.
 */
function ding_zerohit_form_zerohit_form_content_type_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}