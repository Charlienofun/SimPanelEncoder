// Charlie Burtoff 11/15/20
// 

#include <Joystick.h>
#include <CD74HC4067.h>

// Vars
const int numberOfDirectlyConnectedButtons = 3;
const int buttonMap[numberOfDirectlyConnectedButtons] = {2,3,4}; // These are the pins on the Pro Mini (not the multiplexer)
const int pinToButtonMap = buttonMap[0]; // Constant that maps the phyical pin to the joystick button.
int lastButtonState[numberOfDirectlyConnectedButtons] = {0,0,0}; // Last state of the buttons attached to the device directly

               // s0 s1 s2 s3
CD74HC4067 my_mux(5, 6, 7, 8);  // create a new CD74HC4067 object with its four control pins
int lastButtonStateOfMultiplexer[16]; // Last state of the buttons attached to the device via the multiplexer=
const int g_common_pin = 10; // select a pin to share with the 16 channels of the CD74HC4067

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  19, 0,                   // Button Count, Hat Switch Count
  false, false, false,     // X and Y, but no Z Axis
  false, false, false,     // No Rx, Ry, or Rz
  false, false,            // No rudder or throttle
  false, false, false);    // No accelerator, brake, or steering

void setup() {

  // Set the directly attached pins to be inputs with pullup enabled. 
  for (int index = 0; index < (numberOfDirectlyConnectedButtons - 1); index ++) {
    pinMode(buttonMap[index], INPUT_PULLUP);
  }
  
  // Set the common input pin attached via the multiplexer to have pullup enabled. 
  pinMode(g_common_pin, INPUT_PULLUP); // set the initial mode of the common pin. (This can be changed in loop() for for each channel)

  // init the arrays for the button states
  for (int index = 0; index < 15; index ++) {
    // Set each of the 16 button states to 0
    lastButtonStateOfMultiplexer[index] = 0;
  }

  // Initialize Joystick Library
  Joystick.begin();

  // Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
 
}

void loop() {
  // Read pin values
  for (int index = 0; index < (numberOfDirectlyConnectedButtons); index++)
  {
    int currentButtonState = !digitalRead(index + pinToButtonMap);
    if (currentButtonState != lastButtonState[index])
    {
      Joystick.setButton(index + 16, currentButtonState); // 16 because the first 16 are from the multiplexer
      lastButtonState[index] = currentButtonState;
    }
  }

      // Read the multiplxer and send the presses
      for (int i = 0; i < 16; i++) {
        my_mux.channel(i);
        //  Serial.println(i);
        //  if (!digitalRead(g_common_pin)) {
        //     Serial.println("its Happening!!");
        //  }
        //  delay(400);
        
        int currentButtonState = !digitalRead(g_common_pin);
        if (currentButtonState != lastButtonStateOfMultiplexer[i]) {
          Joystick.setButton(i, currentButtonState);
          //Serial.println("Hello Computer");
          //Serial.print(i);
          //Serial.print(currentButtonState);
          lastButtonStateOfMultiplexer[i] = currentButtonState;
        }
    }
}

