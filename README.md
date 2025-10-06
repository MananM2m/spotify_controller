# Spotify Controller (LCD + HC-SR04 + Serial)

Microcontroller-driven Spotify controller that:

* Shows **track title** and **artist** on a 16×2 LCD
* Uses an **HC-SR04** proximity sensor to **toggle play/pause**
* Uses **A0** (e.g., a potentiometer or joystick) to **skip next/previous**
* Talks to a Python helper over **USB serial** that controls Spotify via the Web API

> Firmware: Arduino (parallel 4-bit LCD)
> Host: Python script using `spotipy`

---

## ✨ Features

* Physical control over Spotify playback
* Real-time song and artist display on LCD
* Hand gesture detection using HC-SR04
* Analog input (A0) for track skipping
* Serial communication between Arduino and Python

---

## 🧱 Project Structure

```
.
├─ spotify.py    # Python helper: serial bridge + Spotify Web API control
└─ spotify.ino   # Arduino firmware: LCD UI + HC-SR04 + analog control
```

---

## 🔌 Hardware & Wiring

### Required Components

* Arduino Uno / Nano (or compatible board)
* 16×2 LCD (HD44780, 4-bit mode)
* HC-SR04 ultrasonic distance sensor
* Potentiometer or joystick on A0
* Jumper wires, breadboard, USB cable

### Pin Map

**LCD (LiquidCrystal in 4-bit mode)**

| LCD Signal      | Arduino Pin                 |
| --------------- | --------------------------- |
| RS              | **12**                      |
| EN              | **11**                      |
| D4              | **5**                       |
| D5              | **4**                       |
| D6              | **3**                       |
| D7              | **2**                       |
| VSS/VDD/RW      | GND / 5V / GND              |
| VO (contrast)   | Pot middle pin (10kΩ)       |
| A/K (backlight) | 5V (through resistor) / GND |

**HC-SR04 Sensor**

| HC-SR04 | Arduino Pin |
| ------- | ----------- |
| Echo    | **10**      |
| Trig    | **9**       |
| VCC/GND | 5V / GND    |

**Controls**

| Function            | Arduino Pin / Input                     |
| ------------------- | --------------------------------------- |
| Pause (hand <15 cm) | HC-SR04 distance                        |
| Next Track          | `analogRead(A0) < 200`                  |
| Previous Track      | `analogRead(A0) > 800`                  |
| Spare Inputs        | Pins **8** and **7** (currently unused) |

---

## 💻 Software Requirements

### Python (Host)

* Python 3.10+
* `spotipy`
* `pyserial`

```bash
python -m venv .venv
source .venv/bin/activate   # Windows: .venv\Scripts\activate
pip install spotipy pyserial
```

### Arduino (Microcontroller)

* Arduino IDE
* Built-in **LiquidCrystal** library
* **SR04** library *(install via Library Manager)*

---

## 🔐 Spotify Setup

Before running the controller, you need to link it with your Spotify account using a **temporary access token**.

### Steps

1. Go to the [Spotify Developer Dashboard](https://developer.spotify.com/dashboard/).
2. Log in and create a new app (or use an existing one).
3. Generate an **Access Token** using the **OAuth 2.0 Tool**.

   * Include the following **scopes**:

     * `user-modify-playback-state`
     * `user-read-playback-state`
     * `user-read-currently-playing`
4. Copy the token and paste it into your `spotify.py`:

   ```python
   ACCESS_TOKEN = 'your_spotify_access_token'
   ```
5. Replace the serial port name with your Arduino’s port (check Arduino IDE → Tools → Port):

   ```python
   ser = serial.Serial('/dev/cu.usbmodem1201', 9600, timeout=1)
   ```

   Example:

   * macOS/Linux: `/dev/cu.usbmodemXXXX` or `/dev/ttyUSB0`
   * Windows: `COM3`, `COM4`, etc.

> ⚠️ **Note:** This token will eventually expire. You can generate a new one anytime.
> For a permanent solution, integrate SpotifyOAuth (token refresh) later.

---

## 🚀 Running the Project

### 1️⃣ Upload the Arduino Firmware

1. Open `spotify.ino` in the **Arduino IDE**.
2. Select the correct **Board** and **Port** under the **Tools** menu.
3. Click **Upload** to flash the code.
4. Once uploaded, open the **Serial Monitor** (set to 9600 baud) to verify output.

---

### 2️⃣ Run the Python Controller

In your terminal, navigate to the project directory and run:

```bash
python spotify.py
```

This script will:

* Connect to your Arduino via Serial
* Continuously read playback commands from it
* Update the LCD with the current Spotify track

---

### 3️⃣ Control Spotify

Use your connected hardware to control playback:

* 👋 **Wave your hand (<15 cm)** over the HC-SR04 → toggles **Play/Pause**
* 🎚 **Move analog input A0 high (joystick or similar input) (>800)** → skips to the **Previous track**
* 🎚 **Move analog input A0 low (<200)** → skips to the **Next track**
* 📟 LCD display updates in real time, showing:

  ```
  Song Title
  Artist Name
  ```

---

## 🔁 Serial Communication Protocol

| Direction            | Message Example | Description                  |                                                    |
| -------------------- | --------------- | ---------------------------- | -------------------------------------------------- |
| **Arduino → Python** | `pause`         | Toggle playback (pause/play) |                                                    |
|                      | `prev`          | Go to previous track         |                                                    |
|                      | `next`          | Go to next track             |                                                    |
| **Python → Arduino** | `SongTitle      | ArtistName`                  | Updates LCD with the current song (each ≤16 chars) |

Messages are newline-terminated (`\n`).
The Arduino parses everything before and after `|` as two LCD lines.

---

## 🧩 How It Works

### 🖥️ `spotify.py`

* Opens a Serial connection to the Arduino
* Listens for commands (`pause`, `prev`, `next`)
* Uses the [Spotify Web API](https://developer.spotify.com/documentation/web-api/) via `spotipy`
* Sends the current song title and artist (truncated to 16 characters each) back to the Arduino
* Continuously updates the LCD every second

### 🔧 `spotify.ino`

* Initializes the 16×2 LCD via `LiquidCrystal` (pins 12, 11, 5, 4, 3, 2)
* Reads distance from the HC-SR04 (Trig: 9, Echo: 10)
* Reads analog input A0 to determine direction for next/previous
* Sends serial commands to Python and displays the current track info

---


## 🧭 Future Improvements

* Add **volume control** via another potentiometer
* Replace analog input with **rotary encoder**
* Integrate **album art** on OLED display
* Support **Bluetooth (ESP32)** for wireless control
* Add **SpotifyOAuth** token refresh to make it permanent

---

## 💡 Tips for Expansion

* The LCD and SR04 can be swapped for I²C OLED displays or touch sensors.
* You can easily adapt this for a Raspberry Pi setup using the same serial logic.
* Add `sp.current_user_playing_track()['progress_ms']` for a playback progress bar.

---
