<?php

/**
 * Administration UI for ding zero-hit form
 *
 * @param array $form_state
 * @return array form
 */
function ding_zerohit_form_settings_form($form_state) {
  // Load saved settings
  $values = variable_get('ding_zerohit_form', array());

  // Build the form
  $form = array();
  
  $form['ding_zerohit_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Zero-hit form'),
    '#required' => TRUE,
    '#description' => t(''),
    '#tree' => TRUE,
  );

  //Get available webforms - see also code in {MODULEFOLDER}/webform/includes/webform.admin.inc
  if (1 == 1) { //IF --webform is installed--
    $webform_types = webform_variable_get('webform_node_types');
    $webform_nodes = array();
    if ($webform_types) {
      $placeholders = implode(', ', array_fill(0, count($webform_types), "'%s'"));
      $result = db_query(db_rewrite_sql("SELECT n.nid,n.title FROM {node} n INNER JOIN {node_revisions} r ON n.vid = r.vid WHERE n.type IN ($placeholders)", 'n', 'nid', $webform_types), $webform_types);
      while ($webform_node = db_fetch_object($result)) {
        $webform_nodes[$webform_node->nid] = $webform_node->title;
      }
    }
  }
  
  $form['ding_zerohit_form']['webform_id'] = array(
    '#type' => 'select',
    '#title' => t('Webform ID'),
    '#options' => $webform_nodes,
    '#default_value' => isset($values['webform_id']) ? $values['webform_id'] : NULL, //Get standard zero-hit form from a variable. TODO: create this variable during module install
    '#description' => t('Webform to use as zero-hit form'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Submit handler for the administration forms. It preformces basic clean up
 * and hands the form to systemt settings submit.
 *
 * @param array $form
 * @param array $form_state
 */
function ting_proxy_settings_form_submit($form, &$form_state) {
  // Remove add hostname button
  unset($form_state['values']['ting_proxy']['hostnames']['add_hostname']);

  // Remove elements that have been marked as deleted
  foreach ($form_state['values']['ting_proxy']['hostnames'] as $key => $value) {
    if ($value['deleted']) {
      unset($form_state['values']['ting_proxy']['hostnames'][$key]);
    }
  }

  // Return control to system settings form submit.
  system_settings_form_submit($form, $form_state);
}

/**
 * Handels addition of extra ting_proxy_hostname elements via ahah to the
 * adminitration UI.
 *
 * @param array $form
 * @param array $form_state
 */
function ting_proxy_add_hostname_ahah($form, &$form_state) {

  $form_state = array('submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];

  // Drupal is not aware of this new elements existence and will not
  // process it. We retreive the cached form, add the element, and resave.
  $form = form_get_cache($form_build_id, $form_state);

  // Add one to the current highest delta value to get the next one.
  $delta = max(element_children($form['ting_proxy']['hostnames'])) + 1;

  // Added field and update form cache
  $form['ting_proxy']['hostnames'][$delta] = array(
    '#type' => 'ting_proxy_hostname',
    '#default_value' => '',
  );
  form_set_cache($form_build_id, $form, $form_state);

  // Reset form vars
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
  );

  // Rebuild the form.
  $form = form_builder($_POST['form_id'], $form, $form_state);

  // Render and return the new field.
  drupal_json(array(
    'data' => drupal_render($form['ting_proxy']['hostnames'][$delta]),
    'status' => TRUE));
  exit();
}

/**
 * Addes custom ccs and js script to the administration form. It's done this way
 * as it's add to the forms validation as well.
 *
 * @param array $form
 * @param array $form_state
 * @return array $form
 */
function ting_proxy_settings_form_after_build($form, &$form_state) {
  // Add form styling
  drupal_add_css(drupal_get_path('module', 'ting_proxy') .'/css/ting_proxy.css', 'module', 'all', FALSE);

  // Add js
  drupal_add_js(drupal_get_path('module', 'ting_proxy'). '/js/ting_proxy.admin.js');

  return $form;
}