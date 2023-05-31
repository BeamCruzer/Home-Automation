# Home Automation
My home automation project is based on IoT which can convert any simple home to a smart home and can enable remote applications.

## Functionality of the project
* The project board can be connected to WiFi and can switch ON/OFF different devices like light, fan, charger etc. (3 devices) through internet.
* Real time feedback can be seen through android application and web dashboard. The devices can also be switched from there.
* You can switch the appliances from anywhere in the world through mobile/laptop as the project board is connected to internet.
* In case the project board is not connected to internet, you can control the devices with the help of an IR remote (like TV remote) as it has an IR sensor which requires direct line of sight with the remote to operate (like operating a TV).
* The project board also consist of a motion sensor which can be used to switch ON room light (or any desired appliances) in case motion is detected. And the light will also get turned OFF automatically with a desired duration when there is no motion in the room.
* Timing automations can be created to save electricity. For example, turn ON the charger for exactly 1 hour 30 minutes and turn it back OFF after that. This can also prevent mobile phones/laptops from overcharging.
* Emergency notifications can be sent to mobile when motion is detected in the room. This can prevent theft as the user will get notified as soon as someone enters the room.

## Specifications of the project
* The heart of the project is ESP8266 NODEMCU microcontroller which enables the board with WiFi connectivity.
* The board sends data to server as a client and the user’s input from mobile/laptop is also transferred to the server and data is interpreted there. After that, data is updated in the board as per the coding.
* The switching is done by relays which can switch AC loads (upto 10Amps).
