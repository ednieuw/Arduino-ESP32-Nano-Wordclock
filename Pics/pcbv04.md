# Bill of Materials — Nano ESP32 Wordclock PCB (V04)

Extracted from `PCB/Nano_ESP32-PCB_V04.fzz` (the embedded Fritzing XML instance list), from the [Arduino-ESP32-Nano-Wordclock](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock) repo.

| Qty | Component | Value / Spec | Designators |
|---|---|---|---|
| 1 | Arduino Nano ESP32 | — | main board |
| 4 | Resistor | 470 Ω, ±5% | R5, R18, R19, R20 |
| 3 | Resistor | 10 kΩ, ±5% | R13, R21, R22 |
| 1 | Electrolytic capacitor | 1000 µF (1 mF), 16 V | unlabeled |
| 1 | Electrolytic capacitor | 10 µF | C1 |
| 2 | Diode | 1N5817 (label) | near J1/J2 |
| 1 | LED, 3 mm | Red (633 nm) | — |
| 1 | LED, 3 mm | Yellow (592 nm) | — |
| 1 | IC | labeled "74AHCT125" | — |
| 2 | Screw terminal, 2-pin, 3.5 mm pitch | — | J1, J2 |
| 1 | Female pin header, 6-pin, 0.1" | DS3231 RTC module (GND, 5V, SDA, SCL wired to Arduino I2C) | RTC-clock |
| 2 | Female pin header, 6-pin, 0.1" | **Spare — wired only to each other, not connected to anything else** | RTCclock2, RTCclock9 |
| 1 | Female pin header, 5-pin, 0.1" | Rotary encoder connector | Rotary |
| 1 | Female pin header, 4-pin, 0.1" | Arduino GPIO (pins 28–30) + LED — despite the name, not RTC-related | RTCclock11 |
| 1 | Female pin header, 3-pin, 0.1" | DCF77 receiver connector | DCF |
| 1 | Female pin header, 3-pin, 0.1" | 3.3 V regulator (GND, 5V, input-GND) | AMS1117 3v3 |
| 1 | Female pin header, 3-pin, 0.1" | 3.3 V / GND breakout, mirrors AMS1117 3v3 header | 5V Out GND |
| 1 | Female pin header, 3-pin, 0.1" | All 3 pins tied to GND | RTCclock3 |
| 1 | Female pin header, 3-pin, 0.1" | All 3 pins tied to 5V | RTCclock12 |
| 1 | Female pin header, 3-pin, 0.1" | All 3 pins tied to GND | RTCclock13 |
| 1 | Female pin header, 3-pin, 0.1" | Isolated — only tied to one diode leg, not on main GND/5V rails | RTCclock4 |
| 1 | Female pin header, 3-pin, 0.1" | GND, GND, and one leg of R5 | RTCclock6 |
| 1 | Female pin header, 2-pin, 0.1" | Duplicate tap of a Rotary signal + GND | RTCclock14 |
| 1 | Female pin header, 2-pin, 0.1" | Tap of a 74AHCT125 IC pin + GND | RTCclock15 |
| 1 | Male pin header, 2-pin, 0.1" | LDR (light sensor) connector | LDR |
| 1 | Male pin header, 3-pin, 0.1" | Arduino TX/RX + GND | TX-RX |

## Notes / things to verify

- **Diodes** are labeled "1N5817" on the board, but the underlying Fritzing generic part's "part number" metadata field says "1N4001". These are different diode types (1N5817 is a Schottky, 1N4001 is a standard rectifier) — worth checking the actual silkscreen/datasheet reference before ordering.
- **The IC** is labeled "74AHCT125" but is built from Fritzing's generic "74xx08" part (whose metadata part number is "74HC08", a quad 2-input AND gate — not the same function as a 74x125 buffer). This looks like a label/underlying-part mismatch in the Fritzing file; confirm which chip is actually intended from the schematic/README.
- The headers labeled "5V Out GND" and "AMS1117 3v3" suggest an external AMS1117 3.3V regulator module connects via header rather than being soldered directly on this board.
- Header purposes above come from tracing the actual copper nets (wires + vias) in the PCB view back to the Arduino pins, GND/5V rails, or other components — not just from part names. Several headers are named "RTCclock#" but aren't actually part of the RTC circuit (see table).
- Excluded from this list as non-physical PCB layout elements: 255 wire traces, 49 vias, 4 mounting holes, 48 copper-fill/ground-plane regions, the board outline, and logo/silkscreen text.

