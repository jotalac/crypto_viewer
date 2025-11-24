# Crypto Price Viewer

A DIY cryptocurrency price dispaly.

## Description

This project is diy simple **cryptocurrency price viewer**. It fetches real-time data from a cryptocurrency API and displays it on a screen. The device is housed in a 3D-printed case with colored screen.

This is a standalone device that is not dependent on your computer.

## Features

*   **Price Tracking:** Fetches and displays up-to-date cryptocurrency prices.
*   **Display:** Shows data on 240x320px screen.
*   **Button Controls:** Navigate through different cryptocurrencies.
*   **Buzzer:** Provides audible alerts.
*   **3D-Printed Case:** A custom-designed case to house all the components.
*   **Configurable from phone:** Connect to AP and configure the device from your phone.
*   **Wi-Fi Connectivity:** Connects to your local network to access the internet.

## Hardware

This project uses the following hardware components:

*   **Microcontroller:** XIAO ESP32C6
*   **Display:** 2inch 240x320px TFT Display: 
*   **Buttons:** 2x - 12x12mm tactile switch (B3F-4055)
*   **Buzzer (optional):** buzzer without generator
*   **Antenna (optional, for better signal):** mini antenna with uFL connector
*   **Perfboard:** prefboard 30x70 mm
*   **Cables and wires**
*   **3D printed case:** 3d printed parts, in the model folder
*   **Screws:** 2x 2.5x20mm screw (for case assembly)

#### Hardware links
- Dispaly (~3.0$)
    - https://www.aliexpress.com/item/1005006589727895.html?spm=a2g0o.order_list.order_list_main.11.757d1802qw4zkG
- Microcontroller (~8.0$, get from your local supplier)
    - https://botland.cz/moduly-wifi-a-bt-esp32/24783-seeed-xiao-esp32-c6-wifi-bluetooth-seeedstudio-113991254-5904422385705.html
- Tactile switches (~0.5$):
    - https://makershop.ie/B3F4055?srsltid=AfmBOooik3h_e8dq_zdSf28YTv6m92bTonL3PdaJR46RWWBAH6ENf9dU
- Buzzer (~2.0$):
    - https://botland.store/buzzers-sound-generators/4187-buzzer-without-generator-23mm-1-40v-in-a-case-5904422366070.html
- Antenna (~2$)
    - https://botland.store/wifi-antennas/13810-mini-flexible-wifi-antenna-24ghz-with-ufl-connector-100mm-5904422321505.html
- Perfboard (~0.5$)
    - https://www.amazon.com/Fielect-Electronic-Protoboard-Prototyping-Printed/dp/B081Q4XH7Z/ref=sr_1_3?crid=2N4S8WU50C3RK&dib=eyJ2IjoiMSJ9.emV8xgSUvHOc0VUDB3B3YlEAvieZboYOSrhZQ5SG9SYaUSP3Gd2VSy02SVwHoqc1.eeK-BK6vzqfk05x0BmEKDLrOViYTPoEts0hirGKnXpA&dib_tag=se&keywords=perfboard+30x70&qid=1763998909&sprefix=perfboard+30x70%2Caps%2C245&sr=8-3
- 3D printed case
    - https://github.com/jotalac/crypto_viewer/tree/main/model

## Usage guide
### Setup
*   After first connection you will se this screen
*   Connect to that wifi from your phone, and this setup screen will popup
*   Select configue wifi
*   Select your wifi ssid and fill in the password
*   Below you can set up to 3 cryptocurrencies to be displayed on your display
*   Hit save

## Usage
*   Price refreshes once in 10 minutes for the currently displayed coin 
    *   you can chagne the interval in the code
    *   don't make the interval too short, because you might hit the rate limit of the api

*   **Left button press** - go to next coin
*   **Left button hold** - fetches new price, for the currently displayed coin
*   **Right button press** - reset the device
*   **Right button hold** - enter config mode



## Software

The firmware is written in C++ using the Arduino framework.

### Libraries

*   TFT_eSPI - by Bodmer
*   ArduinoHttpClient
*   ArduinoJson
*   WiFiManager
*   Preferences - by Volodymyr Shymanskyy

## Assembly Guide

### Step 1: Setup arduino ide
- add xiao esp32c6 support for the arduino ide
    - guide: https://wiki.seeedstudio.com/xiao_esp32c6_getting_started/
- downlaod listed libraries
- find this path on you computer: **Arduino/libraries/TFT_eSPI/User_Setup.h**
    - paste this to the user setup file:
    ```bash
    #define USER_SETUP_INFO "User_Setup"
    #define ST7789_2_DRIVER
    #define TFT_SDA_READ   
    #define TFT_RGB_ORDER TFT_RGB  // Colour order Red-Green-Blue

    #define TFT_WIDTH  240 
    #define TFT_HEIGHT 320

    #define TFT_MOSI 18  
    #define TFT_SCLK 19  
    #define TFT_CS   22  
    #define TFT_DC   21  
    #define TFT_RST  23  

    #define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
    #define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
    #define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
    #define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
    #define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
    #define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
    #define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts
    #define SMOOTH_FONT

    #define SPI_FREQUENCY  27000000
    #define SPI_READ_FREQUENCY  20000000
    #define SPI_TOUCH_FREQUENCY  2500000
    ``` 
### Step 2: Soldering components 
*   Solder controller on the prefboard (9 pins availible left, 8 pin availible on right)
*   Solder display
*   Solder buzzer
*   Put the antenna on the controller port
*   Don't solder the buttons just yet

*  *solder the components according to the wiring diagram and tables*

### Wiring

**Display**

| From (Display)           | To (ESP32C6)             |
| -------------- | -------------- |
| GND | GND |
| VCC | 3V3 |
| SCL | D8 (GPIO19) |
| SDA | D10 (GPIO18) |
| RST | D5 (GPIO23) |
| DC |  D3 (GPIO21) |
| CS |  D4 (GPIO22) |

**Buttons**
- leg orientation doesnt matter (but make sure to solder the opposite legs)

| From (Button)           | To (ESP32C6)             |
| -------------- | -------------- |
| left btn - 1. leg | D1 (GPIO1) |
| left btn - 2. leg | GND |
| |  |
| right btn - 1. leg | D2 (GPIO2) |
| right btn - 2. leg | GND |

**Buzzer**

| From (Buzzer)           | To (ESP32C6)             |
| -------------- | -------------- |
| red cable | D0 (GPIO0) |
| black cable | GND |

### Step 3: 3D Printing

*   Print the case and button caps
*   You can choose the version with holes (much cooler) or the normal one without holes

### Step 4: Final Assembly

*   Put the buttons with caps into the holes on top of the case
*   Now solder the buttons according to the prefboard according to the diagram   
*   Slide the prefboard into the case and put in the antenna and buzzer
*   Put in the 2 screws to hold the two parts of the case together

### Step 5: Software flashing

*   Connect the board to your pc
*   Clone this repository
*   In Arduino IDE open the `code.ino` file
*   Make sure you have installed all the libraries and edited the `User_Setup.h` file
*   Click the upload button in the arduino ide to compile and flash the firmware
    * The ide might tell you that you have run out of memeory on the board
        *   go to: `Tools->Partition Scheme` and select `Huge APP`

## Contributing

- Contributions are welcome! Please feel free to submit a pull request.
- Adding new features is very welcome.
