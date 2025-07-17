#pragma once
#ifndef VBDriveArduino_vbdrive_arduino_h
#define VBDriveArduino_vbdrive_arduino_h

#include "stm32_def.h"
#include "stm32g4xx_hal_fdcan.h"
#include "Arduino.h"

#define STM32_G
#define HAL_FDCAN_MODULE_ENABLED

#define LED1 PB2
#define LED2 PB10

// i2c3
#define pinSDA PC9
#define pinSCL PC8

namespace VBDrive {
class CanFD {
public:
    CanFD() : hfdcan1() {};
    FDCAN_HandleTypeDef* get_hfdcan() {
        return &hfdcan1;
    }

    void init();
    void write_default_params();
    void write_default_params_classic();
    void default_start();
    void apply_config();

private:
    FDCAN_HandleTypeDef hfdcan1;
};
}

void SystemClock_Config();

#endif
