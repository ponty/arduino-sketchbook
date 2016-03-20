/*





 */



const float Vref = 1.1; // V
const float Vcc = 5; // V
const bool use_internal=false;

//const float R1 = 10000+47000;
//const float R2 = 4700;
const float R1 = 4700;
const float R2 = 1000;

// External Vref
// MCP1541
const bool use_external_ref=true;
const int pin_Vref_ext = A4;
const float Vref_ext = 4.096; // V


// pins
//Declare what pins are connected to the digits
const int pin_digit1 = 0; //Pin 12 on my 4 digit display
const int pin_digit2 = 3; //Pin 9 on my 4 digit display
const int pin_digit3 = 4; //Pin 8 on my 4 digit display
const int pin_digit4 = 8; //Pin 6 on my 4 digit display

//Declare what pins are connected to the segments
const int pin_segA = 1; //Pin 11 on my 4 digit display
const int pin_segB = 5; //Pin 7 on my 4 digit display
const int pin_segC = 6; //Pin 4 on my 4 digit display
const int pin_segD = 10; //Pin 2 on my 4 digit display
const int pin_segE = A2; //Pin 1 on my 4 digit display
const int pin_segF = 2; //Pin 10 on my 4 digit display
const int pin_segG = 7; //Pin 5 on my 4 digit display
const int pin_segDP = 9; //Pin 3 on my 4 digit display

const int numberOfDigits = 4; //Do you have a 2 or 4 digit display?

const int displayType = COMMON_CATHODE; //Your display is either COMMON_CATHODE or COMMON_ANODE

const int pin_input = A5;

