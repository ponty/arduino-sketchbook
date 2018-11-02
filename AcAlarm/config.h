
// songs
const int octave = 0;
const char alarm_song_P[] PROGMEM =
"Smurfs:d=32,o=5,b=200:4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8f#,p,8a#,p,4g#,4p,g#,p,a#,p,b,p,c6,p,4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8b,p,8f,p,4f#";
const char start_song_P[] PROGMEM ="Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6";

// sleep time after alarming
const int WAIT_SEC = 20; //sec

const int PERIOD_MS = 20; // 20 ms -> 50 Hz

// set it for lower power consumption
#define USE_NARCOLEPTIC 1
