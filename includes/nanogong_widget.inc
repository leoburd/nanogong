<?php
// $Id$

/**
 * Implementation of hook_widget_info().
 */
function nanogong_widget_info() {
  return array(
    'nanogong_listenUp' => array(
      'label' => t('Nanogong audio recorder'),
      'field types' => array('nanogong'),
    ),
  );
}


/**
 * Implementation of hook_widget().
 */
function nanogong_widget(&$form, &$form_state, $field, $items, $delta = 0) {
  $element = array(
    '#type' => $field['widget']['type'],
    '#default_value' => isset($items[$delta]) ? $items[$delta] : '',
  );
  return $element;
}


/**
 * Implementation of FAPI hook_elements().
 *
 * Any FAPI callbacks needed for individual widgets can be declared here,
 * and the element will be passed to those callbacks for processing.
 *
 * Drupal will automatically theme the element using a theme with
 * the same name as the hook_elements key.
 *
 */
function nanogong_elements() {
  return array(
    'nanogong_listenUp' => array(
      '#input' => TRUE,
      '#process' => array('nanogong_listenUp_process'),
      ),
    );
}


/**
 * Process an individual element.
 *
 * Build the form element. When creating a form using FAPI #process,
 * note that $element['#value'] is already set.
 *
 * The $fields array is in $form['#field_info'][$element['#field_name']].
 */
function nanogong_listenUp_process($element, $edit, $form_state, $form) {
  $field = $form['#field_info'][$element['#field_name']];
  $field_key = $element['#columns'][0];

  $element['applet'] = array(
    '#required' => $element['#required'],
    '#value' => nanogong_recorder_applet_markup($element,$field['widget'])
  );

  // this would be the hidden field the applet would insert the newly 
  // uploaded file fid to.
  $element[$field_key] = array(
    '#type' => 'hidden',
    '#title' => t($field['widget']['label']),
    '#required' => $element['#required'],
    '#default_value' => isset($element['#value'][$field_key]) ? $element['#value'][$field_key] : "",
  );

  return $element;
}


