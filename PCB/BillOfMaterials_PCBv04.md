# Bill of Materials — Nano ESP32 Wordclock PCB (V04)


| Qty | Component | Value / Spec | Designators |
|---|---|---|---|
| 1 | Arduino Nano ESP32 | — | main board |
| 3 | Resistor | 470 Ω, ±5% | Before RGB(W) LED-strip and LED 3mmm |
| 3 | Resistor | 10 kΩ, ±5% | LDR, 2 optional for I2C|
| 1 | Electrolytic capacitor | 1000 µF (1 mF), 16 V | 1mF |
| 1 | Electrolytic capacitor | 10 µF | 10uF |
| 1 | LDR | GL5516 | LDR |
| 1 | Rotary Encoder or Membrane keyboard | KY-034 | Rotary |
| 1 | RTC-module | DS3231 | RTC-clock |
| 1 | RGB(W) LED-strip | ES2812 or SK6812 | LED-strip |
| 2 | Diode | 1N5817 (label) | 1N5817 |
| 1 | LED, 3 mm | Red (633 nm) | D9 |
| 1 | LED, 3 mm | Yellow (592 nm) | D10 |
| 1 | IC | 74AHCT125 | Level shifter |
| 2 | Screw terminal, 2-pin, 3.5 mm pitch | POWER | J1, J2 |
| 2 | Female pin header, 15-pin, 0.1" | Connector for Arduino Nano ESP32 | Nano ESP32 |
| 1 | Female pin header, 6-pin, 0.1" | DS3231 RTC module (GND, 5V, SDA, SCL wired to Arduino I2C) | RTC-clock |
| 1 | Female pin header, 5-pin, 0.1" | Rotary encoder connector | Rotary |
| 1 | Female pin header, 3-pin, 0.1" | DCF77 receiver connector | DCF |
| 1 | Female pin header, 3-pin, 0.1" | optional 3.3 V regulator (GND, 5V, input-GND) | AMS1117 3v3 |
| 1 | Male pin header, 4-pin, 0.1" | For connector 5V to Vin or USB|  |
| 1 | Male pin header, 3-pin, 0.1" | All 3 pins tied to 5V | RTCclock12 |
| 1 | Male pin header, 3-pin, 0.1" | All 3 pins tied to GND | RTCclock13 |
| 1 | Female pin header, 3-pin, 0.1" | GND, GND, and one leg of R5 | RTCclock6 |
| 1 | Female pin header, 2-pin, 0.1" | LDR (light sensor) connector | LDR |
| 1 | Female pin jumper, 2-pin, 0.1" | Select 5V to Vin or to USB 5V | Power shunt|
| 1 | Male pin header, 3-pin, 0.1" | Arduino TX/RX + GND | TX-RX |

## Notes 

- **Diodes** are labeled "1N5817" on the board, but the underlying Fritzing generic part's "part number" metadata field says "1N4001". These are different diode types (1N5817 is a Schottky, 1N4001 is a standard rectifier) 
— "1N4001" has a voltage drop op 0.7V, "1N5817" of 0.45V. The "1N5817" is preferred

