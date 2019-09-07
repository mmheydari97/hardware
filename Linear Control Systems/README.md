## About
This is my linear control systems course project.

This project is done using an Arduino Uno. we tried to derive the temperature of a box made 
of glass to the desired value. 

The microcontroller takes action when the temperature reported by an LM32 sensor is not within
a threshold of the desired value and turns the motor of a hairdryer on. When the temperature
is below desired, the heating coil will also be turned on so that the hairdryer blows hot.

A user can change the desired temperature by some push-buttons and both desired and the current
temperature is visible on a character LCD. 

For more stability, I used a digital filter too.

## Notes
* You cannot turn a hairdryer on and off with your microcontroller directly and you should 
use relays.
* As the motor of a hairdryer propagates some strong noise you should keep the LCD far from
the motor. However, Arduino Uno works fine due to having some initial denoising circuits.
* Do not forget to use debouncing circuits for push-buttons.
* If any pin on the LCD is not connected, it will be prone to noise.