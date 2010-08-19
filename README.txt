 Audio recording field supplies a cck field for recording audio, directly through the user's browser, while in the site.

 For the recording task, this module uses a java applet, called <a href="http://www.javasonics.com/">ListenUp by javasonic</a>. In order to use the module you need to download ListenUp from it's home site.
 In addition to the audio recorder, ListenUp provides an audio player applet, which is available as the fields cck formatter.

 INSTALLATION:
 --------------

 1. Download ListenUp applet from http://www.javasonics.com/downloads.

 2. Extract ListenUps archive to some folder on your server, you're welcome to try the included ListenUp demos, but for the module, you only need one folder :
    <extracted folder>/listenup/codebase. Copy this folder to the module's directory.

 3. Now you can add an audio recording field to your cck content types.
    Notice the ListenUp recorder settings box on the field settings page. The ListenUp applet is customable, and this is your way to customize it.
    A list of possible applet parameters can be found here:
    http://www.javasonics.com/listenup/docs/recorder_parameters.html,
    and here: http://www.javasonics.com/listenup/docs/player_parameters.html
    For example, in order to prolong the recording length, from the default 30 seconds,
    add this to the ListenUp recorder settings:
      maxRecordTime|50

 REMARKS:
 --------
 The freeware version of ListenUp only allows 60 seconds, and has a 3 seconds delay on start up.

 Enjoy, Alex and the linnovate team
