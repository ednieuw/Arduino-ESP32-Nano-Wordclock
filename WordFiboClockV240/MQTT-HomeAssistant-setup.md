# Connecting the clock to Home Assistant with MQTT

The clock appears in Home Assistant via MQTT with:

- **Controls:** a Display on/off switch, a Max Brightness slider, a Display Colour Scheme selector, a Random Display selector (off / per minute / per hour) and buttons for LED test, StatusLED on/off and Restart.
- **Sensors:** CPU temperature, LDR light reading and, when a DS3231 module is installed, its temperature.
- **Diagnostics:** WiFi signal strength, uptime, free memory and the three reboot counters. The whole device shows *unavailable* in Home Assistant as soon as the clock goes offline. The clock connects to `homeassistant.local`, port 1883, and logs in with username **mqtt_user** and, as password, **the same password as your WiFi router password** (the one entered in the clock with menu option **B**).

## Setup in Home Assistant

1. Install the Mosquitto broker: **Settings → Add-ons → Add-on Store → Mosquitto broker → Install**, then **Start** (enable *Start on boot*).
2. Add the MQTT integration if it is not added automatically: **Settings → Devices & Services → Add Integration → MQTT**.
3. Create the user the clock logs in with: **Settings → People → Users → Add user**.
   - Username: `mqtt_user`
   - Password: *your WiFi router password* (identical to the password entered in the clock)
   - Tick *Can only log in from the local network*.
4. On the clock, turn MQTT on with menu option **_** and restart the clock with **@**.

The clock now appears in Home Assistant under **Settings → Devices & Services → MQTT** as device *WordClock*, with its buttons, sensors and the colour scheme selector.

## If your MQTT broker uses a different username or password

The clock's MQTT settings are not in the menu; they are set in the sketch. Find these lines near the top of the sketch and change them to match your broker:

```cpp
const char* MQTT_SERVER   = "homeassistant.local";   // or the IP address of your broker, e.g. "192.168.1.10"
const uint16_t MQTT_PORT  = 1883;
const char* MQTT_USER     = "mqtt_user";             // your MQTT username
const char* MQTT_PASSWORD = Mem.Password;            // uses the WiFi password. Replace with your own, e.g. "MySecret123"
```

To use a password that differs from the WiFi password, replace `Mem.Password` with your password between double quotes:

```cpp
const char* MQTT_PASSWORD = "MySecret123";
```

Then compile and upload the sketch again.

> **Note:** as long as `MQTT_PASSWORD = Mem.Password` is used, changing the WiFi password in the clock (menu option **B**) also changes the password the clock uses for MQTT. Keep the `mqtt_user` password in Home Assistant identical to the WiFi password, or set a fixed password as shown above.

## How the MQTT discovery works

Home Assistant does not need any configuration for the clock because the clock announces its own entities. This is called **MQTT Discovery**: for every entity the clock publishes one JSON message to a special config topic

```
homeassistant/<type>/<deviceId>/<key>/config
```

where `<type>` is `sensor`, `button`, `switch`, `number` or `select`, and `<deviceId>` is the BLE name of the clock in lower case (spaces replaced by `_`), for example `wordclock`. Home Assistant listens to `homeassistant/#`, reads the JSON and creates the entity. The JSON never contains measurements — it only tells Home Assistant *where* the data will appear and *how* to send commands. The examples below assume the device id `wordclock`.

### Sensor

Published to `homeassistant/sensor/wordclock/rssi/config`:

```json
{
  "name": "WiFi Signal",
  "state_topic": "wordclock/rssi",
  "unit_of_measurement": "dBm",
  "device_class": "signal_strength",
  "entity_category": "diagnostic",
  "availability_topic": "wordclock/status",
  "state_class": "measurement",
  "unique_id": "wordclock_rssi",
  "device": { "identifiers": ["wordclock"], "name": "WordClock",
              "manufacturer": "EdNieuw", "model": "WordClock" }
}
```

- **state_topic** — Home Assistant subscribes here; whatever the clock publishes there (e.g. `-67`) becomes the sensor value.
- **unit_of_measurement / device_class** — Home Assistant picks the right icon and formatting. `device_class` must come from Home Assistant's fixed list (`temperature`, `signal_strength`, `duration`, `data_size`, …).
- **state_class: measurement** — keeps min/mean/max history in Home Assistant's long-term statistics.
- **entity_category: diagnostic** — moves the entity to the *Diagnostic* box on the device page. Used for WiFi signal, uptime, free heap and the reboot counters.
- **availability_topic** — Home Assistant also watches `wordclock/status`. The clock publishes `online` there after connecting and the broker publishes `offline` (the *Last Will*) when the clock drops off — all entities then show *unavailable* instead of stale values.
- **unique_id** — Home Assistant's bookkeeping key. As long as it stays the same, a republished config updates the *same* entity instead of creating a duplicate. Removing an entity is done by publishing an **empty** retained message to its config topic.
- **device** — all entities with the same `identifiers` are grouped under one device in Home Assistant.

### Button

The simplest entity. Instead of a `state_topic` it has:

```json
"command_topic": "wordclock/cmnd",
"payload_press": "@"
```

Pressing the button in Home Assistant publishes `@` to `wordclock/cmnd`. The clock feeds this into the same command parser used by the serial monitor, BLE and the web page — identical to typing `@`. Fire-and-forget, no state.

### Switch

A button with memory:

```json
"command_topic": "wordclock/cmnd",
"payload_on":  "O1",
"payload_off": "O0",
"state_topic": "wordclock/display",
"state_on":  "ON",
"state_off": "OFF"
```

Two directions: Home Assistant sends `O1`/`O0` as command; the clock reports the real state by publishing `ON`/`OFF` to `wordclock/display`. Because the state is reported and not assumed, the toggle in Home Assistant stays correct even when the display is switched off via the menu or the IR remote.

### Number (slider)

```json
"command_topic": "wordclock/cmnd",
"command_template": "M{{ value }}",
"state_topic": "wordclock/maxbrightness",
"min": 1, "max": 255, "step": 1
```

`{{ value }}` is a template that *Home Assistant* fills in before publishing. Sliding to 200 sends the literal string `M200` — already a valid menu command, so the clock needs no new parsing. The state topic feeds the slider position back.

### Select (dropdown)

```json
"command_topic": "wordclock/cmnd",
"state_topic": "wordclock/randomdisplay",
"options": ["Off", "Per minute", "Per hour"]
```

Home Assistant sends the chosen option back *as text*, e.g. `Per minute`. The clock translates the text to the matching menu command (`~1`, or `Qn` for the colour schemes) before executing it. The state topic must echo the exact option text back, so the clock publishes the option name, not the number.

### Two details that apply to everything

All config messages are published with **retain = true**: the broker stores them, so Home Assistant re-learns all entities immediately after a restart without the clock doing anything. And every entity shares the same `availability_topic` and `device` block, which makes the whole set behave as one device that collectively goes *unavailable* the moment the clock disappears from the network.
