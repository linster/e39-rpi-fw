The RVC GPIO reader needs to respond to the GPIO pin,
turn on the screen, switch the video source to GPIO.


We're not going to do a fancy observer pattern,
we'll just let the PI do the do.

https://raspberrypi.github.io/pico-sdk-doxygen/group__hardware__gpio.html#ga0b10ec79e10d7df958373b178debcd2e
https://raspberrypi.github.io/pico-sdk-doxygen/group__hardware__gpio.html#ga6165f07f4b619dd08ea6dc97d069e78a


We'll make this just emit the LED for now.


When RVC on: Switch input to RVC. Send IBUS message to turn
on screen.