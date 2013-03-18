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
    '#description' => t('admin settings for zero-hit form'),
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