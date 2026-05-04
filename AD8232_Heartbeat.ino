/*
  AD8232 Heart Rate Monitor — plotter-friendly version

  Reads the analog ECG signal from a SparkFun AD8232 module and streams
  raw 10-bit ADC values (0-1023) over the serial port at 9600 baud.
  Watch the live waveform with Tools -> Serial Plotter in the Arduino IDE.

  Wiring:
    AD8232 OUTPUT -> Arduino A0
    AD8232 LO+    -> Arduino D10
    AD8232 LO-    -> Arduino D11
    AD8232 3.3V   -> Arduino 3.3V   (NOT 5V)
    AD8232 GND    -> Arduino GND
    AD8232 SDN    -> leave unconnected

  No external libraries required.

  License: MIT
*/

void setup() {
  // Open the serial port so the IDE can read our data.
  Serial.begin(9600);

  // The AD8232 pulls these pins HIGH when an electrode loses contact.
  pinMode(10, INPUT); // LO+
  pinMode(11, INPUT); // LO-
}

void loop() {
  // If a lead is off, output 0 so the plotter shows a flat line at the bottom.
  // Otherwise output the raw ECG value (0-1023).
  if ((digitalRead(10) == 1) || (digitalRead(11) == 1)) {
    Serial.println(0);
  } else {
    Serial.println(analogRead(A0));
  }

  // ~2 ms gives a smooth waveform in the Serial Plotter (500 samples/sec).
  delay(2);
}
