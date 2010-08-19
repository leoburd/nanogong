<?php

//$Id$

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
 * Implementation of FAPI hook_telements().
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



/*
 * listenUp recorder applet
 */
function nanogong_recorder_applet_markup($element,$widget){
	$field_id = $element['#id']."-value";
	dsm(print_r($element, true), '$element from recorder');
	dsm($field_id, 'field_id');
	$url = url('nanogong/file_upload',array('query'=>array('field_id'=>$field_id)));
	$width = isset($widget['player_width'])?$widget['recorder_width']:300;
	$height = isset($widget['player_height'])? $widget['recorder_height']:100;
	$params = $widget['recorder_params'];
	$params_str = nanogong_build_params($params);
	
	
	$sampleURL = "";
	if(!empty($element['#value']['value'])){
		$file = $element['#value']['file'];            
		$surl = file_create_url($file->filename);
		$sampleURL = "<param name='sampleURL' value='".$surl."'>";
	}
	$submitVoice = 'function submitVoice() {var applet = document.getElementById("nanogong");'. 
						'var ret = applet.sendGongRequest("PostToForm", "http://localhost/recorder/?q=nanogong/file_upload", "userfile", "", "temp");'. 
						'if (ret == null || ret == "") alert("Failed to submit the voice recording!");'. 
						'else alert("Voice recording has been submitted!"); } ';
	drupal_add_js($submitVoice, 'inline');	
	
	$output = 

			'<applet '.
					    ' id = "nanogong"'.
					    ' code="gong.NanoGong"'.
					    ' archive="'.base_path().drupal_get_path('module', 'nanogong').'/nanogongBase/nanogong.jar"'.
					    ' name=ListenUpRecorder"'.
					    ' width="180"'.
					    ' height="40">'.
					' </applet>'.
					'<form>'.  
						'<input type="button" value="Submit!" onclick="submitVoice()" />'.
					'</form>';
					
	
	return $output;
}



/*
 * listenUp recorder applet callback. This is what happens when 
 * 'send' is pressed in the applet
 */
function nanogong_file_receive(){

    header("Cache-control: private");
    header("Content-Type: text/plain");	
	
	

	$field_id = $_GET['field_id'];
	
	//TODO save duration in field
	$duration = strip_tags($_POST['duration']);
	$tmp_name = $_FILES['userfile']['tmp_name'];
	$upload_dir = file_directory_path();
	
	

	$c = (file_check_directory($upload_dir,0))? 1:0;
	print("file folder writeable? $c");
		
	// TODO: get file extension from real file type
	$new_name = time().'.wav';
	$new_path = file_create_filename($new_name, $upload_dir);
	//$new_path = $upload_dir.$new_name;
	
	watchdog('nanogong', 'new path: '.$new_path);

	if($_FILES['userfile']['error']>0){
		watchdog('nanogong',"ERROR - error code: ".$_FILES['userfile']['error']."\n");
		$fid = "NULL";
	}else{
			$_FILES['files'] = $_FILES['userfile'];
			
			$_FILES['files']['name'] = $new_name;
			$use = 'name';//session_id();
			
			watchdog('nanogong', '$user: '.$use);
			variable_set($use, $new_name);
			foreach($_FILES['files'] as $key=>$value){
				$_FILES['files'][$key] = array(0 => $value);
			}

			watchdog('nanogong', 'exporting files: '.print_r($_FILES, true));
			dsm($_FILES);

			 if(!$file = file_save_upload(0,array(),$upload_dir)){
				watchdog('nanogong',"ERROR - file_save_upload failed\n");
				$fid = "NULL";		
			} else{
				watchdog('nanogong', 'exporting file2: '.print_r($file, true));
				$fid = $file->fid;
				
				//convert uploaded wav files to mp3 - does'nt work yet
				/*
				$ffmpeg_format    = 'libmp3lame';
				$ffmpeg_extension = 'mp3';
				$ffmpeg_frequency = '44100';
				$ffmpeg_bitrate   = '64k';
				$ffmpeg_channels  = '2';
				$ffmpeg_mime      = 'audio/mpeg';
				$ffmpeg_path      = $field['widget']['ffmpeg_path'] ?
				$field['widget']['ffmpeg_path'] : '/usr/bin/ffmpeg';
						
				$ffmpeg_arguments = '-y -i ' . $file->filepath . ' ' .
				'-f ' . $ffmpeg_extension . ' ' .
				'-acodec ' . $ffmpeg_format . ' ' .
				'-ar ' . $ffmpeg_frequency . ' ' .
				'-ab ' . $ffmpeg_bitrate . ' ' .
				'-ac ' . $ffmpeg_channels . ' ' .
				$file->filepath.".mp3";
	
				var_export($ffmpeg_arguments);
				// Convert the file to one with the fresh file name.
				 $ffmpeg_results = shell_exec("ffmpeg" . " " . $ffmpeg_arguments);		
				
				*/		

				watchdog('nanogong', "SUCCESS - File have been uploaded,\n");
			}
	}
	watchdog('nanogong', 'works2');
	$js = "document.getElementById('$field_id').value='$fid';";	
	watchdog('nanogong', "CALLJS $js"); 
}

