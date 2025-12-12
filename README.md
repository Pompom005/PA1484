# Weather Data Visualization and Interaction on ESP32

## Introduction

This project is a touchscreen-based weather application built for an ESP32-T4-S3 LilyGO device. It fetches weather data from SMHI’s Open Data API and visualizes it on the device using a menu-/screen-driven interface.

The main goals are to:
- Fetch forecast and historical weather data from SMHI.
- Show the data in a simple, readable graphical format on the ESP32 display.
- Let the user navigate between screens using touch gestures (swipe).
- Let the user interact with the data (e.g., scroll through historical data using a slider).
- Provide a settings screen where the user can select city and weather parameter, and optionally store defaults that persist across restarts.

## Getting started

### Prerequisites

Software:
- Visual Studio Code
- PlatformIO IDE extension for VS Code

Hardware:
- ESP32-T4-S3 LilyGO device (touchscreen)
- USB cable for flashing and serial monitor
- A WiFi network that is **not** eduroam (the board does not work with eduroam)

### Installation

1. Clone this repository.
2. Open the repository folder in Visual Studio Code.
3. Install the **PlatformIO IDE** extension in VS Code.
4. Open `project/project.ino`.

> Important: Only modify code inside `project/project.ino`. Changing other files may break the provided framework.

### WiFi configuration

Add your WiFi SSID and password in `project/project.ino` (look for the WiFi configuration section).

## Building and running

### Build & upload to the ESP32

1. Connect the ESP32 to your computer via USB.
2. In VS Code + PlatformIO:
   - Build (✔)
   - Upload (→)
   - (Optional) Monitor serial output (plug icon)

If the port cannot be found or the USB device keeps flashing, enter upload mode manually:
1. Hold **BOOT**
2. While holding **BOOT**, press and release **RST**
3. Release **BOOT**
4. Upload again
5. Press **RST** to exit download mode

### Startup procedure (expected behavior)

- On boot, a boot screen is shown with the program version and group number for a short time.
- After the boot screen, the user can swipe left/right to navigate between the main screens.

### How to operate (touchscreen)

- **Swipe left/right** to change screen.
- Use on-screen UI elements such as:
  - Slider (for navigating historical data)
  - Dropdown menus (for choosing city/parameter)
  - Buttons (reset to default / set current as default)

## Features

Checklist of user stories (mark what your team implemented):

- [ ] **US1.1C:** Show a starting screen with current program version and group number.
- [ ] **US1.3:** Provide a screen to view weather forecast data.
- [ ] **US1.2C:** Show a 7-day forecast for the selected city (12:00 each day) with temperature and weather condition symbols.
- [ ] **US2.1:** Navigate between screens by sliding a finger over the touchscreen.
- [ ] **US3.1:** Provide a screen to view historical weather data.
- [ ] **US3.2D:** Show latest-months historical hourly data for the selected parameter/city, with a slider to scroll from oldest to newest.
- [ ] **US4.1:** Provide a single settings screen to configure both city and weather parameter.
- [ ] **US4.2B:** Select between four weather parameters: temperature (1), humidity (6), wind speed (4), air pressure (9).
- [ ] **US4.3B:** Select between five cities: Karlskrona (65090), Stockholm (97400), Göteborg (72420), Malmö (53300), Kiruna (180940).
- [ ] **US4.4:** Reset the selected city and weather parameter to default using a button.
- [ ] **US4.5:** Set the default city/parameter to the current selection using a button.
- [ ] **US4.6:** Store default city/parameter so they persist after restart.
- [ ] **US5.1:** Provide a screen to view weather forecast for all of Sweden.
- [ ] **US5.2:** Show a Sweden “Landskap” temperature map with color-coded zones and a looped 24h hourly forecast animation.
