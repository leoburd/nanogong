<?php
// $Id$


/**
* Declare information about a field type.
*
* @return
*   An array keyed by field type name. Each element of the array is an associative
*   array with these keys and values:
*   - "label": The human-readable label for the field type.
*/
function nanogong_field_info() {

  return array(
    'nanogong' => array('label' => 'File'),
  );
}

/**
 * Implementation of hook_field_settings().
 */
function nanogong_field_settings($op, $field) {
  switch ($op) {
        case 'database columns':
                // value is audio file fid
                $columns['value'] = array(
                         'type' => 'int',
                         'length' => 12,
                         'sortable' => FALSE,
                         'not null' => FALSE);
                return $columns;
  }
}

/**
 * Implementation of hook_field().
 *
 */
function nanogong_field($op, &$node, $field, &$items, $teaser, $page) {

  switch ($op) {
        case 'presave':
                foreach($items as $key=>$value){
                        if(!empty($value['value'])){
                                $file = nanogong_get_file_object($value['value']);

                                // since the applet, and the file receive scripts saved the
                                // new recording for an anonymous user, we need to
                                // update the real uploading user here.         
                                if(file_set_status($file,1)){
                                        global $user;
                                        $file->uid = $user->uid;
                                        //update user columb in file table
                                        drupal_write_record('files', &$file, array('fid'));
                                }
                        }
                }
                break;
        case 'load':
                        // load file data to field 
                        $field_name = $field['field_name'];
                        foreach($items as $key=>$value){
                                $n[$field_name][$key]['value'] = $value['value'];
                                $n[$field_name][$key]['file'] = nanogong_get_file_object($value['value']);
                        }
                        return $n;

                break;
  }
}

/**
 * Implementation of hook_content_is_empty().
 *
 *
 * This function tells the content module whether or not to consider
 * the $item to be empty. This is used by the content module
 * to remove empty, non-required values before saving them.
 */
function nanogong_content_is_empty($item, $field) {
  if (empty($item['value'])) {
    return TRUE;
  }
  return FALSE;
}


