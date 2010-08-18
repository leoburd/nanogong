<?php
// $Id$


/**
 * Implementation of hook_field_formatter_info().
 */
function nanogong_field_formatter_info() {
watchdog('nanogong', 'in nanogong_field_formatter_info');
  return array(
    'listenup_player' => array(
      'label' => t('ListenUp player'),
      'field types' => array('nanogong'),
      'multiple values' => CONTENT_HANDLE_CORE,
    ),
  );
}

/*
 * listenUp player formatter theme
 */
function theme_nanogong_formatter_listenup_player($element){
watchdog('nanogong', 'inside theme_nanogong_formatter_listenup_player');
   $field = content_fields($element['#field_name']);
   $markup = nanogong_player_applet_markup($element,$field['widget']);
   return $markup;
}

/*
 * player applet html markup
 */
function nanogong_player_applet_markup($element,$widget){
watchdog('nanogong', 'inside nanogong_player_applet_markup');
   //$v = $element['#item']['value'];
dsm(print_r($element, true), '$element');

   $file = $element['#item']['file'];
   $url = file_create_url($file->filename);
   
        //$width = isset($widget['player_width'])? $widget['player_width']:300;
        //$height = isset($widget['player_height'])? $widget['player_height']:100 ;
        //$params = $widget['player_params'];
        //$params_str = nanogong_build_params($params);

        $output =  "";

        $use = 'name';//session_id();
dsm($use, '$user2');
        $name = variable_get($use, $default = NULL);
        $directory = file_directory_path();
dsm(print_r($_FILES, true), '$_FILES');
dsm($name, '$name');
dsm($directory, '$directory');

        $value = $directory.'/'.$name;
dsm($value, 'where am i');

   if(!empty($value)){

                $output =       /*'<applet '.
                                            ' code="com.softsynth.javasonics.recplay.PlayerApplet"'.
                                            ' codebase="'.base_path().drupal_get_path('module','nanogong').'/codebase"'.
                                            ' archive="JavaSonicsListenUp.jar"'.
                                            ' name="ListenUpRecorder"'.
                                            ' width="'.$width.'"'.
                                            ' height="'.$height.'">'.
                                                ' <param name="sampleURL" value="'.$url.'">'.$params_str.
                                        ' </applet>'.*/

         '<applet '.
            ' id = "nanogong"'.
            ' code="gong.NanoGong"'.
            ' archive="'.base_path().drupal_get_path('module', 'nanogong').'/nanogongBase/nanogong.jar"'.
            ' name=ListenUpRecorder"'.
            ' width="180"'.
            ' height="40"'.
            ' SoundFileURL = "'.$value.'">'.
         ' </applet>';

         //      ' <param name="sampleURL" value="'.$url.'">'.$params_str.
   }
dsm($output, '$output');
   return $output;
}

