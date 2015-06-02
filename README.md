![smart light](assets/header.jpg "smart light")

# Smart Light v1.0.0

#### Smart Light ( Arduino Sketch, Fritzing Schematic &amp; Mobile App )

Build your own Smart Light with this Complete FOS Project.  The completed project will allow you to use a free mobile application to control how &amp; when you want your Smart Light to work.  We are also releasing our Smart Light Mobile App as part of this project.

The main goal of this project is to show how anyone can build their own Custom Product from Start to Finish.  You now have with Custom Software, and even control that Product with a powerful Mobile Application.

Parts List
---

#### You will need to purchase the following hardware:

* `ARD` [Arduino Pro Mini 328 - 5V/16 MHz](https://www.adafruit.com/products/2378?utm_medium=referral&utm_source=manifestinteractive)
* `PIR` [PIR (motion) sensor](https://www.adafruit.com/products/189?utm_medium=referral&utm_source=manifestinteractive)
* `NEO` [NeoPixel Ring - 12 x WS2812 5050 RGB LED with Integrated Drivers](https://www.adafruit.com/products/1643?utm_medium=referral&utm_source=manifestinteractive)
* `LUX` [Adafruit TSL2561 Digital Luminosity/Lux/Light Sensor Breakout](https://www.adafruit.com/products/439?utm_medium=referral&utm_source=manifestinteractive)
* `BLU` [Bluefruit LE - Bluetooth Low Energy (BLE 4.0) - nRF8001 Breakout - v1.0](https://www.adafruit.com/products/1697?utm_medium=referral&utm_source=manifestinteractive)
* `SDC` [Mini DC/DC Step-Down (Buck) Converter - 5V @ 1A output - TSR12450](https://www.adafruit.com/products/1065?utm_medium=referral&utm_source=manifestinteractive)

#### Optional Parts:

* [FTDI Serial TTL-232 USB Cable](https://www.adafruit.com/products/70?utm_medium=referral&utm_source=manifestinteractive) ( I use this to program the Arduino boards )
* [4 x AA Battery Holder with On/Off Switch](https://www.adafruit.com/products/830?utm_medium=referral&utm_source=manifestinteractive) ( needed if your enclosure does not have battery space )
* [Adafruit Perma-Proto Quarter-sized Breadboard PCB](https://www.adafruit.com/products/1608?utm_medium=referral&utm_source=manifestinteractive) ( might make wiring a little easier )
* [6-conductor 0.1" socket-socket cable - 6" long](https://www.adafruit.com/products/206?utm_medium=referral&utm_source=manifestinteractive) ( I cut these in half and use them to extend FTDI access to program Arduino boards )

#### Optional Enclosure:

I found it was actually cheaper to just get an existing Motion Light and replace it's components with the list above.  This is the enclosure I went with for this project:

* [Hallomall™ Clover Style Night Lights](http://www.amazon.com/gp/product/B00MLW99MK?utm_medium=referral&utm_source=manifestinteractive)

Wiring Instructions
---

| Part     | Pin    | > | Part | Pin  | Cable  |
|----------|--------|---|------|------|--------|
| __PIR__  | Red    | > | ARD  | VCC  | Red    |
| PIR      | Yellow | > | ARD  | 3    | Yellow |
| PIR      | Black  | > | ARD  | GND  | Black  |
| __NEO__  | PWR    | > | ARD  | VCC  | Red    |
| NEO      | IN     | > | ARD  | 6    | Blue   |
| NEO      | GND    | > | ARD  | GND  | Black  |
| __LUX__  | VIN    | > | ARD  | VCC  | Red    |
| LUX      | GND    | > | ARD  | GND  | Black  |
| LUX      | SDA    | > | ARD  | A4   | Yellow |
| LUX      | SCL    | > | ARD  | A5   | Blue   |
| __BLU__  | SCK    | > | ARD  | 13   | Blue   |
| BLU      | MISO   | > | ARD  | 12   | Yellow |
| BLU      | MOSI   | > | ARD  | 11   | Orange |
| BLU      | REQ    | > | ARD  | 10   | Grey   |
| BLU      | RDY    | > | ARD  | 2    | White  |
| BLU      | RST    | > | ARD  | 9    | Orange |
| BLU      | GND    | > | ARD  | GND  | Black  |
| BLU      | VIN    | > | ARD  | VCC  | Red    |

![wiring](fritzing/exports/smart_light.jpg "wiring")