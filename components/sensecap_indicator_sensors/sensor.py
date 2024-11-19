 #
 # Copyright (c) 2024 tobi149
 # SPDX-License-Identifier: MIT
 #


import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_TVOC, DEVICE_CLASS_TEMPERATURE, DEVICE_CLASS_VOLATILE_ORGANIC_COMPOUNDS_PARTS, \
    ICON_RADIATOR, STATE_CLASS_MEASUREMENT, CONF_TEMPERATURE, \
    CONF_HUMIDITY, CONF_CO2, UNIT_PARTS_PER_BILLION, UNIT_PARTS_PER_MILLION, ICON_MOLECULE_CO2, \
    DEVICE_CLASS_CARBON_DIOXIDE, UNIT_CELSIUS, \
    ICON_THERMOMETER, UNIT_PERCENT, ICON_WATER_PERCENT, DEVICE_CLASS_HUMIDITY, CONF_ALTITUDE, CONF_ID

DEPENDENCIES = ["uart"]
CODEOWNERS = ["@tobi1449"]
CONF_TEMPERATURE_INTERNAL = "temperature_internal"
CONF_HUMIDITY_INTERNAL = "humidity_internal"

sensecap_indicator_sensors_ns = cg.esphome_ns.namespace("sensecap_indicator_sensors")
SenseCapIndicatorSensorsComponent = sensecap_indicator_sensors_ns.class_("SenseCapIndicatorSensorComponent",
                                                                         cg.Component, uart.UARTDevice)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(SenseCapIndicatorSensorsComponent),
        cv.Optional(CONF_CO2): sensor.sensor_schema(
            unit_of_measurement=UNIT_PARTS_PER_MILLION,
            icon=ICON_MOLECULE_CO2,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_CARBON_DIOXIDE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            icon=ICON_WATER_PERCENT,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_HUMIDITY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TEMPERATURE_INTERNAL): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_HUMIDITY_INTERNAL): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            icon=ICON_WATER_PERCENT,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_HUMIDITY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TVOC): sensor.sensor_schema(
            unit_of_measurement=UNIT_PARTS_PER_BILLION,
            icon=ICON_RADIATOR,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_VOLATILE_ORGANIC_COMPOUNDS_PARTS,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_ALTITUDE, default=0): cv.int_range(min=0, max=10000),
    }
).extend(uart.UART_DEVICE_SCHEMA)
FINAL_VALIDATE_SCHEMA = uart.final_validate_device_schema(
    "sensecap_indicator_sensors", baud_rate=115200, parity="NONE", require_rx=True
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if co2_config := config.get(CONF_CO2):
        sens = await sensor.new_sensor(co2_config)
        cg.add(var.set_co2_sensor(sens))
    if temperature_config := config.get(CONF_TEMPERATURE):
        sens = await sensor.new_sensor(temperature_config)
        cg.add(var.set_temperature_sensor(sens))
    if humidity_config := config.get(CONF_HUMIDITY):
        sens = await sensor.new_sensor(humidity_config)
        cg.add(var.set_humidity_sensor(sens))
    if temperature_internal_config := config.get(CONF_TEMPERATURE_INTERNAL):
        sens = await sensor.new_sensor(temperature_internal_config)
        cg.add(var.set_internal_temperature_sensor(sens))
    if humidity_internal_config := config.get(CONF_HUMIDITY_INTERNAL):
        sens = await sensor.new_sensor(humidity_internal_config)
        cg.add(var.set_internal_humidity_sensor(sens))
    if tvoc_config := config.get(CONF_TVOC):
        sens = await sensor.new_sensor(tvoc_config)
        cg.add(var.set_tvoc_sensor(sens))
    if altitude_config := config.get(CONF_ALTITUDE):
        cg.add(var.set_altitude(altitude_config))
