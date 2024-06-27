# ESP32 - Observer
_by [#thespielplatz](https://t.me/thespielplatz)_

[![MIT License Badge](docs/img/license-badge.svg)](LICENSE)

This project is a boilerplate project for sending json data via post request to a specific url with the help of the wifimanager project. In this case I used it to monitor the battery data of the [battery shield](https://github.com/thespielplatz/esp-battery-shield).

The project sends the data per default only every 5min. If you want to change the intervall e.g. to 30 second
```cpp
const int postRequestTimeInSeconds = 30;
```

You can remove the battery shield implementation completely. See [Adapt the code](#adapt-the-code)

## Configure the post address
- Connect to the stationary wifi (Starting with `ESP32_`)
- Wait until the config page opens
- Configure Wifi
  - Configure Wifi and Post Path
- Configure Post Path
  - Select `Info` in main menu
  - Scroll down
  - Select `/param`
  - Set the post url
  - Restart the device

## Adapt the code

If you want to send different data, remove the sample part implementation of `IMPLEMENTATION OF BATTERYMETER`

## Dev Notes

- [Wifi Manager](https://github.com/tzapu/WiFiManager)
- [Read and Write config file](https://github.com/bblanchon/ArduinoJson/blob/7.x/examples/JsonConfigFile/JsonConfigFile.ino)

## Tip Me

If you like this project, give it a star! If you love it, fork it and take it out for dinner. 🌟🍽️ And hey, why not [send some tip love?](https://thespielplatz.com/tip-jar)
