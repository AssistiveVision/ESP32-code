# ESP32-code
Code that runs on the esp32.
The esp32 has a server running on it that provides 2 endpoints, one is /jpg and the other is /highres-jpg.
The /jpg endpoint returns a vga picture with a resolution of 640 x 480 that is used for tasks that need images quicker.
The /highres-jpg endpoint returns a sxga picture with a resolution of 1280 x 1024 that is used for tasks that need higher resolution images to work properly.

camera_pins.h: defines the camera pins depending on the model and of the dev-board, in our case we have an AI thinker board
esp32-cam.ino: has the server code
home_wifi_multi.h: has the SSID and password of the wifi network that the esp32 will try to connect to when it boots up
src/OV2640.h: has declarations for functions that are used to interface with the OV2640 camera, the corresponding cpp file has the definition


