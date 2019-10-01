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
   * {@inheritdoc}
   */
  function query() {
    if (!empty($this->value)) {
      $value = $this->value;
      if (is_array($value) && count($value) == 1) {
        $this->value = reset($value);
      }
      // TODO: Remove everything below here to the next TODO when
      // https://jira.duraspace.org/browse/ISLANDORA-2481 is resolved.
      $value = $this->value;

      module_load_include('inc', 'islandora_solr', 'includes/utilities');
      $value = islandora_solr_lesser_escape($value);

      $fieldname = $this->field;
      $exclude = isset($this->operator) && $this->operator === '!=';
      if (is_array($value)) {
        $values = array_filter($value);
        // Ensure that some values have been selected.
        if (!empty($values)) {
          $this->query->add_filter($fieldname, '(' . implode(' OR ', $values) . ')', $this->options['group'], $exclude);
        }
        return;
      }
      $this->query->add_filter($fieldname, $value, $this->options['group'], $exclude);
    }
    // TODO: Once above is removed, uncomment below.
    //parent::query();
  }

  /**
   * Provide a simple textfield for equality.
   */
  function value_form(&$form, &$form_state) {
    $which = 'all';
    $solr_options = islandora_solr_views_get_field_values($this->field);
    $options = array_combine($solr_options, $solr_options);
    $default = array();
    if (!empty($this->value)) {
      if (is_array($this->value)) {
        $default = array_keys(array_filter($this->value));
      }
    }

    $form['value'] = array(
      '#type' => "checkboxes",
      '#options' => $options,
      '#default_value' => $default,
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

    if ($which == 'all') {
      $form['value'] += array(
        '#dependency' => array($source => $this->operator_values(1)),
      );
    }
  }

  /**
   * {@inheritdoc}
   */
  public function admin_summary() {
    $op = check_plain((string) $this->operator);
    $value = is_array($this->value) ? implode(", ", array_keys($this->value)) : $this->value;
    return $op . ' ' . check_plain((string) $value);
  }

  /**
   * {@inheritdoc}
   */
  public function value_submit($form, &$form_state) {
    parent::value_submit($form, $form_state);
    if (isset($form_state['values']['options']['value']) && is_array($form_state['values']['options']['value'])) {
      $defaults = array_filter($form_state['values']['options']['value']);
      $form_state['values']['options']['value'] = $defaults;
    }
    if (isset($form_state['input']['options']['value']) && is_array($form_state['input']['options']['value'])) {
      $defaults = array_filter($form_state['input']['options']['value']);
      $form_state['input']['options']['value'] = $defaults;
    }
  }

}
// @codingStandardsIgnoreEnd
