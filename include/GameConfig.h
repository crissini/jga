#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <Arduino.h>

namespace GameConfig {
constexpr uint8_t BEEPER_PIN = 2; // D2

constexpr uint8_t STARTING_LIVES = 3;
constexpr uint8_t COUNTDOWN_SECONDS = 30;
constexpr uint16_t BEEP_FREQUENCY_HZ = 1200;
constexpr uint16_t BEEP_DURATION_MS = 80;

constexpr uint8_t DISPLAY_WIDTH = 128;
constexpr uint8_t DISPLAY_HEIGHT = 64;
constexpr uint8_t OLED_I2C_ADDRESS = 0x3C;
constexpr int8_t OLED_RESET_PIN = -1;
} // namespace GameConfig

#endif
