//
// Copyright (c) 2024 tobi149
// SPDX-License-Identifier: MIT
//


#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace sensecap_indicator_sensors {

class SenseCapIndicatorSensorComponent : public Component, public uart::UARTDevice {
 public:
 void set_altitude(const uint16_t altitude) { altitude_ = altitude; }
 void set_co2_sensor(sensor::Sensor *co2_sensor) { co2_sensor_ = co2_sensor; }
 void set_humidity_sensor(sensor::Sensor *humidity_sensor) { humidity_sensor_ = humidity_sensor; }
 void set_internal_humidity_sensor(sensor::Sensor *internal_humidity_sensor) { internal_humidity_sensor_ = internal_humidity_sensor; }
 void set_internal_temperature_sensor(sensor::Sensor *internal_temperature_sensor) { internal_temperature_sensor_ = internal_temperature_sensor; }
 void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
 void set_tvoc_sensor(sensor::Sensor *tvoc_sensor) { tvoc_sensor_ = tvoc_sensor; }

 void dump_config() override;
 float get_setup_priority() const override;
 void loop() override;
 void on_shutdown() override;
 void setup() override;

 protected:
 static const int receiveBufferSize_ = 256;

 void on_packet_received(const uint8_t *buffer, size_t size);
 void send(const uint8_t *buffer, size_t size);
 void send(const uint8_t command);
 void send(const uint8_t command, const uint16_t payload);

 uint16_t altitude_;
 sensor::Sensor *co2_sensor_{nullptr};
 sensor::Sensor *humidity_sensor_{nullptr};
 sensor::Sensor *internal_humidity_sensor_{nullptr};
 sensor::Sensor *internal_temperature_sensor_{nullptr};
 sensor::Sensor *temperature_sensor_{nullptr};
 sensor::Sensor *tvoc_sensor_{nullptr};

 uint8_t receiveBuffer_[receiveBufferSize_];
 size_t receiveBufferIndex_ = 0;
 bool recieveBufferOverflow_ = false;
};

} // namespace sensecap_indicator_sensors
} // namespace esphome
