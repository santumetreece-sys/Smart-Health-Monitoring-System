# Smart Health Monitoring System

## 📌 Project Overview

This project implements a health monitoring system using the **LPC2129 ARM7 microcontroller**. It measures heart rate (BPM), estimates blood oxygen saturation (SpO₂) using the MAX30102 sensor, measures temperature using the LM35 sensor, and stores the readings in an AT24C256 I²C EEPROM.

The project focuses on register-level embedded C programming, peripheral interfacing, I²C communication, ADC conversion, and non-volatile data storage.

## 🎯 Objectives

- Measure heart rate in beats per minute (BPM).
- Estimate blood oxygen saturation (SpO₂).
- Measure temperature in degrees Celsius.
- Store measurement data in external EEPROM.
- Develop peripheral drivers using register-level programming.
- Understand sensor interfacing and I²C communication.

## 🛠️ Hardware Components

| Component | Purpose |
|---|---|
| LPC2129 | Main microcontroller |
| MAX30102 | Heart rate and SpO₂ sensing |
| LM35 | Temperature measurement |
| AT24C256 EEPROM | Non-volatile data storage |
| Power supply | Provides regulated power |
| Pull-up resistors | Support I²C communication |

## ⚙️ System Architecture

```text
       MAX30102 Sensor
        (RED + IR)
             |
             | I²C
             |
       +-------------+
       |   LPC2129   |
       |  ARM7 MCU   |
       +-------------+
         |         |
       I²C         ADC
         |          |
    +----------+  +------+
    | AT24C256 |  | LM35 |
    |  EEPROM  |  | Temp |
    +----------+  +------+
```

## 🔌 Communication Interfaces

- **I²C:** Communication between the LPC2129 and MAX30102.
- **I²C:** Reading and writing measurement data to AT24C256 EEPROM.
- **ADC:** Reading the analog output of the LM35 temperature sensor.
- **UART:** Optional serial debugging and displaying measurement values.

## 📊 Measurement Formulas

### 1. Heart Rate (BPM)

Heart rate is calculated from the time interval between consecutive detected heartbeat peaks.

\[
BPM = \frac{60 \times F_s}{N}
\]

Where:

- \(F_s\) = Sampling frequency in samples per second.
- \(N\) = Number of samples between consecutive heartbeat peaks.

For a sampling frequency of 100 Hz:

\[
BPM = \frac{6000}{N}
\]

### 2. SpO₂ Estimation

The ratio of ratios is calculated using the red and infrared signals:

\[
R = \frac{AC_{RED}/DC_{RED}}{AC_{IR}/DC_{IR}}
\]

A suitable calibration equation is required to convert this ratio into an SpO₂ estimate.

### 3. Temperature (LM35)

The LM35 produces an analog voltage proportional to temperature.

\[
Temperature\ (^\circ C) = \frac{V_{OUT}\ (mV)}{10}
\]

The LPC2129 ADC converts the output voltage into a digital value before calculating temperature.

## 💾 EEPROM Data Storage

The AT24C256 provides 256 Kbit (32 KB) of non-volatile memory.

The project uses separate memory locations for each measurement.

| Measurement | EEPROM Address | Example |
|---|---|---:|
| Temperature (°C) | `0x0000` | 36 |
| SpO₂ (%) | `0x0001` | 98 |
| Heart rate (BPM) | `0x0002` | 75 |

Each measurement is stored as a single byte when its value fits within the range 0–255.

## 💻 Software and Tools

- Embedded C
- Keil µVision
- LPC2129 register-level programming
- I²C peripheral programming
- ADC peripheral programming
- UART debugging

## 🚀 Implementation Steps

1. Initialize the LPC2129 peripherals.
2. Configure I²C communication.
3. Initialize the MAX30102 and acquire red and infrared samples.
4. Process the sensor signals to estimate BPM and SpO₂.
5. Read the LM35 output using the ADC.
6. Calculate the temperature in degrees Celsius.
7. Write the measurement values to AT24C256 EEPROM.
8. Read back the stored values to verify data retention.

## 📈 Expected Results

- Heart rate output in BPM.
- Estimated SpO₂ percentage.
- Temperature output in degrees Celsius.
- Successful storage and retrieval of measurement data from EEPROM.

Actual accuracy depends on sensor signal quality, signal processing, calibration, and hardware implementation.

## ⚠️ Limitations

- MAX30102 measurements can be affected by movement, sensor placement, and ambient light.
- Accurate SpO₂ estimation requires appropriate calibration.
- LM35 temperature readings depend on ADC configuration and sensor accuracy.
- EEPROM writes require completion time and appropriate I²C error handling.
- This project is intended for learning and prototyping, not medical diagnosis.

## 🔮 Future Enhancements

- Add Wi-Fi connectivity for cloud data storage.
- Build a graphical dashboard for real-time monitoring.
- Add timestamped measurement logs.
- Improve signal filtering and peak detection.
- Implement EEPROM error handling and data validation.

## 👨‍💻 Author

**Santosh Metre**

Embedded Systems | ARM7 | LPC2129 | Embedded C | Peripheral Interfacing

## 📄 Disclaimer

This project is intended for educational and prototyping purposes only. It is not a certified medical device and must not be used as the sole basis for medical decisions.
