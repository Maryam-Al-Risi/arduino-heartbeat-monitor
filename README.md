# arduino-heartbeat-monitor
A beginner-friendly ECG / heartbeat monitor using Arduino Uno and the AD8232 sensor module — view your live heart waveform in the Arduino Serial Plotter.
# Arduino Heartbeat Monitor (AD8232 ECG)

A simple, working ECG / heart-rate monitor built with an Arduino Uno and the SparkFun AD8232 single-lead heart-rate sensor module. Reads the live ECG waveform and streams it over the serial port so you can watch your heartbeat in real time using the Arduino IDE's built-in Serial Plotter — no extra software required.

This is a beginner-friendly project. If you've never touched an Arduino before, the wiring is five jumper wires and the code is fewer than 20 lines.

## What it does

- Reads the analog ECG signal from the AD8232 sensor
- Detects when an electrode falls off (lead-off detection)
- Streams raw 10-bit ADC values (0–1023) to the computer at 9600 baud
- Lets you visualise your heartbeat as a live waveform in the Serial Plotter

## Hardware required

| Item | Notes |
|---|---|
| Arduino Uno (or compatible) | Any 5 V Arduino works |
| SparkFun AD8232 ECG module | The one with a 3.5 mm TRRS jack |
| 3-electrode biomedical cable | Usually included with the AD8232 |
| 3 × disposable gel electrode pads | Replace if old / dry — fresh pads make a huge difference |
| 5 × jumper wires | Male-to-female works well |
| USB cable | To power the Arduino and stream data |

## Wiring

| AD8232 pin | Arduino pin |
|---|---|
| GND | GND |
| 3.3V | 3.3V (NOT 5V — AD8232 is a 3.3 V chip) |
| OUTPUT | A0 |
| LO+ | D10 |
| LO− | D11 |
| SDN | leave unconnected |

Snap the cable into the audio jack on the AD8232 board and clip the three electrodes to the pads on your skin.

## Electrode placement

Three pads, one cable, three colours:

| Colour | Label | Where on body |
|---|---|---|
| Red | RA (Right Arm) | Upper right chest, just below the collarbone |
| Yellow | LA (Left Arm) | Upper left chest, just below the collarbone |
| Green | RL (Right Leg / reference) | Lower right side of the abdomen, or right ankle |

Press each pad firmly. If the gel feels dry, replace it — that single fix solves most "I see only noise" problems.

## Software setup

1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Plug your Arduino into your computer with the USB cable.
3. In the IDE: **Tools → Board** → pick your board (e.g. Arduino Uno).
4. **Tools → Port** → pick the COM / USB port your Arduino shows up on.
5. Open `AD8232_Heartbeat/AD8232_Heartbeat.ino` from this repository.
6. Click the Upload button (the right-facing arrow).
7. Wait for "Done uploading."

No external libraries needed. The AD8232 is purely analog — `analogRead()` is all you need.

## Viewing the heartbeat

After uploading:

- **Tools → Serial Plotter** (or `Ctrl + Shift + L`)
- Set baud to **9600** (bottom-right of the plotter window)
- Sit still

Within a few seconds you should see the classic ECG waveform scroll across the screen — flat baseline with sharp upward spikes once per heartbeat.

If you'd rather see the raw numbers, use **Tools → Serial Monitor** (`Ctrl + Shift + M`) at 9600 baud.

## How the code works

```cpp
void setup() {
  Serial.begin(9600);
  pinMode(10, INPUT);   // LO+ lead-off flag
  pinMode(11, INPUT);   // LO− lead-off flag
}

void loop() {
  if ((digitalRead(10) == 1) || (digitalRead(11) == 1)) {
    Serial.println(0);
  } else {
    Serial.println(analogRead(A0));
  }
  delay(2);
}
```

Line by line:

- `Serial.begin(9600)` — opens the serial connection between the Arduino and your computer at 9600 bits per second.
- `pinMode(10, INPUT)` and `pinMode(11, INPUT)` — configures pins 10 and 11 as inputs so we can read the AD8232's lead-off flags. These pins go HIGH when an electrode loses contact with the skin.
- Inside `loop()`, the `if` checks both lead-off pins. If either is HIGH, contact is bad, so we print `0`. That makes the Serial Plotter show a flat line at the bottom — easy visual cue that something is wrong.
- Otherwise, `analogRead(A0)` reads the voltage on pin A0, returning an integer between 0 and 1023 (Arduino's 10-bit analog-to-digital converter). That number is the live ECG signal.
- `Serial.println(...)` sends that one number to the computer followed by a newline. The Serial Plotter reads each line as one sample and draws it.
- `delay(2)` waits 2 milliseconds between samples, giving roughly 500 samples per second — plenty of resolution to see clean QRS spikes.

## Troubleshooting

| Symptom | Likely cause |
|---|---|
| Flat line at the very top of the plotter (railed at ~1023) | Powered AD8232 from 5 V instead of 3.3 V, or bad electrode contact |
| Flat line near 0 | Lead-off detection is firing — electrode not making skin contact |
| Noisy signal but no clear spikes | Old / dry electrode pads — replace them |
| No data at all | Wrong COM port, wrong baud rate, or sketch didn't upload |
| Heartbeat appears upside-down (spikes go down) | RA and LA electrodes are swapped — physically swap them, or invert in code with `Serial.println(1023 - analogRead(A0));` |
| 50/60 Hz hum | Stay away from laptop chargers and AC mains; sit still |

## Safety

This project is for educational purposes only. It is **not a medical device** and must not be used to diagnose any condition.

- Do not connect this circuit to a person while it is connected to mains-powered equipment. Run the Arduino from a USB power bank or laptop on battery only when electrodes are on a person.
- Keep the wires and breadboard well clear of liquids.
- If you are concerned about your heart, see a doctor and use a real medical-grade ECG.

## Future improvements

- Add a heart-rate calculator (count R-peaks per minute)
- Save the ECG to an SD card with a timestamp
- Send the data over Bluetooth or Wi-Fi to a phone
- Add a small OLED display to show BPM live
- Build the analog amplifier stage for a stronger signal

## License

MIT — see [LICENSE](LICENSE).

## Repository structure

```
arduino-heartbeat-monitor/
├── README.md                       <- this file (project description)
├── LICENSE                         <- MIT licence
├── .gitignore                      <- files Git should ignore
└── AD8232_Heartbeat/
    └── AD8232_Heartbeat.ino        <- the Arduino sketch
```

The Arduino IDE requires the `.ino` file to live inside a folder of the same name, so the sketch sits in its own subfolder. Open the `.ino` file (the IDE will open the parent folder automatically).

## Author

Built as a beginner electronics / Arduino project. First public repository — feedback welcome via Issues.
