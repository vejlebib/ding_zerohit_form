<?php
/**
 * @file
 * Adminstrative settings.
 *
 * Allows setting which webform should be used as the zerohit form.
 */

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
    '#tree' => TRUE,
  );

  //Get available webforms - see also code in {MODULEFOLDER}/webform/includes/webform.admin.inc
  if (1 == 1) { //IF --webform is installed--
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
  }

  $form['ding_zerohit_form']['webform_id'] = array(
    '#type' => 'select',
    '#title' => t('Webform ID'),
    '#options' => $webform_nodes,
    '#default_value' => isset($values['webform_id']) ? $values['webform_id'] : NULL,
    '#description' => t('Webform to use as zero-hit form'),
    '#required' => TRUE,
  );

  $form['ding_zerohit_form']['edit_info'] = array(
    '#title' => t('Editing webforms'),
    '#value' => '<p>' . t('Any webform configured can be used as the zero-hit form. Sample webforms have been provided with this module. The zero-hit forms and other webforms can be configured at <a href="@content-webforms">admin/content/webform</a>', array('@content-webforms' => url('admin/content/webform', array('absolute' => TRUE)))) . '</p>',
  );

  return system_settings_form($form);
}
