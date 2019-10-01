<?php

/**
 * @file
 * Views filter handler base.
 */

// @codingStandardsIgnoreStart
// This Whole file is ignored due to classes and methods are not camelcase and it
// being called all over the place. TODO bring up to coding standards
class islandora_solr_views_select_handler_filter extends islandora_solr_views_handler_filter {

  /**
   * Provide a simple textfield for equality.
   */
  function value_form(&$form, &$form_state) {
    $which = 'all';
    $solr_options = islandora_solr_views_get_field_values($this->field);
    $options = array_combine($solr_options, $solr_options);
    $form['value'] = array(
      '#type' => 'select',
      '#title' => check_plain($this->definition['title']),
      '#options' => $options,
      '#default_value' => $this->value,
    );
    if (!empty($form['operator'])) {
      $source = ($form['operator']['#type'] == 'radios') ? 'radio:options[operator]' : 'edit-options-operator';
    }
    if (!empty($form_state['exposed'])) {
      $identifier = $this->options['expose']['identifier'];

      if (empty($this->options['expose']['use_operator']) || empty($this->options['expose']['operator_id'])) {
        // exposed and locked.
        $which = in_array($this->operator, $this->operator_values(1)) ? 'value' : 'none';
      }
      else {
        $source = 'edit-' . drupal_html_id($this->options['expose']['operator_id']);
      }
    }
    if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier])) {
      $form_state['input'][$identifier] = $this->value;
    }

    if (empty($form_state['input'][$identifier])) {
      $form_state['input'][$identifier] = 'All';
    }

    if ($which == 'all') {
      $form['value'] += array(
        '#dependency' => array($source => $this->operator_values(1)),
      );
    }
  }

}
// @codingStandardsIgnoreEnd
