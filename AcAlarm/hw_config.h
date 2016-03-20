/*



   .---------------------------.          100
   |                           |          ___               __ /|
   |               pin_speaker o---------|___|-------------|  | |
   |                           |                        .--|__| |
   |                           |                        |      \|
   |                           |         1N4148         |
   |                           |                        |
   |                pin_input  o----o------|<-----------o
   |                           |    |                   |
   |                           |    | 10 cm antenna     |
   |                           |    '-------------      |
   |                           |                        |
   |                           |                        |
   |                       |   o------------------------o
   |                      ===  |                        |
   |   arduino            GND  |                        |
   |                           |                        |
   |                           |               1k       |
   |                           |      LED     ___       |
   |            pin_debug_led  o------>|-----|___|------o
   |                           |                        |
   |                           |                        |
   |                           |                        |
   |                           |       T                |
   |                           |      ---               |
   |          pin_debug_input  o------o o----------------
   |                           |
   |                           |
   |                           |
   '---------------------------'



 */

#include <Arduino.h>

// pins
const int pin_speaker = 9;
const int pin_input = A3;
const int pin_debug_input = A0;
const int pin_debug_led = A5;

