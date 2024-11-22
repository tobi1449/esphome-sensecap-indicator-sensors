//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
// Copyright (c) 2024 tobi149
// SPDX-License-Identifier: MIT
//


#include "sensecap_indicator_sensors.h"
#include "esphome/core/log.h"
#include "COBS.h"

#define PACKETMARKER 0

#define PKT_TYPE_SENSOR_SCD41_CO2 0XB2
#define PKT_TYPE_SENSOR_SHT41_TEMP 0XB3
#define PKT_TYPE_SENSOR_SHT41_HUMIDITY 0XB4
#define PKT_TYPE_SENSOR_TVOC_INDEX 0XB5
#define PKT_TYPE_SENSOR_SCD41_TEMP 0XB6
#define PKT_TYPE_SENSOR_SCD41_HUMIDITY 0XB7

#define PKT_TYPE_CMD_SHUTDOWN 0xA3
#define PKT_TYPE_CMD_ALTITUDE 0xA4

namespace esphome {
namespace sensecap_indicator_sensors {

static const char *const TAG = "sensecap_indicator_sensors";

void SenseCapIndicatorSensorComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "SenseCap Indicator Sensors:");
  LOG_SENSOR("  ", "CO2", this->co2_sensor_);
  LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
  LOG_SENSOR("  ", "Humidity", this->humidity_sensor_);
  ESP_LOGCONFIG(TAG, "  Altitude: %i", this->altitude_);
  this->check_uart_settings(115200);
}

float SenseCapIndicatorSensorComponent::get_setup_priority() const { return setup_priority::DATA; }

void SenseCapIndicatorSensorComponent::loop() {
  if (this->available() == 0) {
    return;
  }

  while (this->available() != 0) {
    uint8_t data;
    this->read_byte(&data);

    if (data == PACKETMARKER) {
      uint8_t decodeBuffer[receiveBufferIndex_];

      size_t numDecoded = COBS::decode(receiveBuffer_,
                                       receiveBufferIndex_,
                                       decodeBuffer);

      // clear the index here so that the callback function can call update() if needed and receive more data
      receiveBufferIndex_ = 0;
      recieveBufferOverflow_ = false;

      this->on_packet_received(decodeBuffer, numDecoded);
    } else {
      if ((receiveBufferIndex_ + 1) < receiveBufferSize_) {
        receiveBuffer_[receiveBufferIndex_++] = data;
      } else {
        // The buffer will be in an overflowed state if we write
        // so set a buffer overflowed flag.
        recieveBufferOverflow_ = true;
      }
    }
  }
}
void SenseCapIndicatorSensorComponent::on_shutdown() {
  uint8_t payload = {PKT_TYPE_CMD_SHUTDOWN};
  this->send(payload, sizeof(payload));
}

void SenseCapIndicatorSensorComponent::setup() {
  ESP_LOGD(TAG, "Setting up SenseCap Indicator Sensors ...");

  if (altitude_ > 0) {
    this->send(PKT_TYPE_CMD_ALTITUDE, altitude_);
  }
}

void SenseCapIndicatorSensorComponent::on_packet_received(const uint8_t *buffer, size_t size) {
  float dataval;
  switch (buffer[0]) {
  case PKT_TYPE_SENSOR_SCD41_CO2:
    memcpy(&dataval, &buffer[1], sizeof(float));
    ESP_LOGD(TAG, "CO2 Level: %.4f", dataval);
    if (this->co2_sensor_ != nullptr) {
      this->co2_sensor_->publish_state(dataval);
    }
    break;
  case PKT_TYPE_SENSOR_SHT41_TEMP:
    memcpy(&dataval, &buffer[1], sizeof(float));
    ESP_LOGD(TAG, "Temperature: %.2f", dataval);
    if (this->temperature_sensor_ != nullptr) {
      this->temperature_sensor_->publish_state(dataval);
    }
    break;
  case PKT_TYPE_SENSOR_SHT41_HUMIDITY:
    memcpy(&dataval, &buffer[1], sizeof(float));
    ESP_LOGD(TAG, "Humidity: %.2f", dataval);
    if (this->humidity_sensor_ != nullptr) {
      this->humidity_sensor_->publish_state(dataval);
    }
    break;
  case PKT_TYPE_SENSOR_TVOC_INDEX:
    memcpy(&dataval, &buffer[1], sizeof(float));
    ESP_LOGD(TAG, "TVOC INDEX: %.4f", dataval);
    if (this->tvoc_sensor_ != nullptr) {
      this->tvoc_sensor_->publish_state(dataval);
    }
    break;
  case PKT_TYPE_SENSOR_SCD41_TEMP:
    memcpy(&dataval, &buffer[1], sizeof(float));
    ESP_LOGD(TAG, "Internal Temperature: %.2f", dataval);
    if (this->internal_temperature_sensor_ != nullptr) {
      this->internal_temperature_sensor_->publish_state(dataval);
    }
    break;
  case PKT_TYPE_SENSOR_SCD41_HUMIDITY:
    memcpy(&dataval, &buffer[1], sizeof(float));
    ESP_LOGD(TAG, "Internal Humidity: %.2f", dataval);
    if (this->internal_humidity_sensor_ != nullptr) {
      this->internal_humidity_sensor_->publish_state(dataval);
    }
    break;
  default:
    break;
  }
}

void SenseCapIndicatorSensorComponent::send(const uint8_t *buffer, size_t size) {
  if (this->available() == 0) {
    return;
  }

  uint8_t encodeBuffer[COBS::getEncodedBufferSize(size)];

  size_t numEncoded = COBS::encode(buffer, size, encodeBuffer);
  this->write_array(encodeBuffer, numEncoded);
  this->write_byte(PACKETMARKER);
}

void SenseCapIndicatorSensorComponent::send(const uint8_t command, const uint16_t payload) {
  uint8_t data_buf[32] = {0};
  int index = 0;

  data_buf[0] = command;
  index++;

  memcpy(&data_buf[1], &payload, sizeof(uint16_t));
  index += sizeof(uint16_t);

  this->send(data_buf, index);
}

} // namespace sensecap_indicator_sensors
} // namespace esphome