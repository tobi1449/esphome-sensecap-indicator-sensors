# ESPHome external component for SenseCap Indicator sensors
This is an external component for ESPHome that can read the sensor values sent from the RP2040 of a SenseCap Indicator.
The components supports reading temperature and humidity from the external AHT41 sensor, CO² from the internal SCD41 sensor and TVOC from the internal SGP40 sensor.

When the SenseCap Indicator's RP2040 is flashed with [tobi1449/sensecap-indicator-rp2040-extended](https://github.com/tobi1449/sensecap-indicator-rp2040-extended), this component can also read temperature and humidity from the internal SCD41 sensor and set the altitude adjustment for the SCD41.

## Usage
```
uart:
  tx_pin: 19
  rx_pin: 20
  baud_rate: 115200

sensor:
  - platform: sensecap_indicator_sensors
    co2:
      name: "CO2 (SCD41)"
    altitude: 405
    tvoc:
      name: "TVOC (SGP40)"
    temperature:
      name: "Temperature External (AHT41)"
    temperature_internal:
      name: "Temperature Internal (SCD41)"
    humidity:
      name: "Humidity External (AHT41)"
    humidity_internal:
      name: "Humidity Internal (SCD41)"
```