#include <Arduino.h>
#include <Wire.h>
#include <stdio.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "GameConfig.h"

namespace {
Adafruit_SSD1306 display(
    GameConfig::DISPLAY_WIDTH,
    GameConfig::DISPLAY_HEIGHT,
    &Wire,
    GameConfig::OLED_RESET_PIN);

unsigned long countdownStartedAt = 0;
int16_t lastDisplayedSeconds = -1;

bool hardwareTestLedsOn = false;
unsigned long lastHardwareTestBlinkAt = 0;
unsigned long lastHardwareTestDrawAt = 0;

template <typename T, size_t N>
constexpr size_t arraySize(const T (&)[N]) {
  return N;
}

uint8_t secondsRemaining() {
  const unsigned long durationMs =
      static_cast<unsigned long>(GameConfig::COUNTDOWN_SECONDS) * 1000UL;
  const unsigned long elapsedMs = millis() - countdownStartedAt;

  if (elapsedMs >= durationMs) {
    return 0;
  }

  return static_cast<uint8_t>((durationMs - elapsedMs + 999UL) / 1000UL);
}

void drawCenteredCountdown(uint8_t seconds) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("COUNTDOWN"));
  display.drawLine(
      0,
      10,
      GameConfig::DISPLAY_WIDTH - 1,
      10,
      SSD1306_WHITE);

  if (seconds == 0) {
    display.setTextSize(2);
    display.setCursor(22, 28);
    display.print(F("TIME UP"));
  } else {
    char countdownText[4];
    snprintf(countdownText, sizeof(countdownText), "%02u", seconds);

    int16_t x = 0;
    int16_t y = 0;
    uint16_t width = 0;
    uint16_t height = 0;
    display.setTextSize(4);
    display.getTextBounds(countdownText, 0, 0, &x, &y, &width, &height);

    const int16_t cursorX =
        static_cast<int16_t>((GameConfig::DISPLAY_WIDTH - width) / 2);
    const int16_t cursorY = 24;
    display.setCursor(cursorX, cursorY);
    display.print(countdownText);

    display.setTextSize(1);
    display.setCursor(42, 56);
    display.print(F("seconds"));
  }

  display.display();
}

void printHighLow(uint8_t pin) {
  display.print(digitalRead(pin) == HIGH ? F("HIGH") : F("LOW"));
}

void drawHardwareTestState() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.print(F("HARDWARE TEST"));
  display.drawLine(
      0,
      10,
      GameConfig::DISPLAY_WIDTH - 1,
      10,
      SSD1306_WHITE);

  for (size_t index = 0; index < arraySize(GameConfig::BUTTON_PINS); ++index) {
    const int16_t y = 14 + static_cast<int16_t>(index * 10);

    display.setCursor(0, y);
    display.print(F("B"));
    display.print(index + 1);
    display.print(F(" D"));
    display.print(GameConfig::BUTTON_PINS[index]);
    display.print(F(" "));
    printHighLow(GameConfig::BUTTON_PINS[index]);

    display.setCursor(66, y);
    display.print(F("C"));
    display.print(index + 1);
    display.print(F(" D"));
    display.print(GameConfig::CABLE_PINS[index]);
    display.print(F(" "));
    printHighLow(GameConfig::CABLE_PINS[index]);
  }

  display.setCursor(0, 56);
  display.print(F("LEDs "));
  display.print(hardwareTestLedsOn ? F("ON") : F("OFF"));

  display.display();
}

void writeAllHardwareTestLeds(bool on) {
  for (size_t index = 0; index < arraySize(GameConfig::LED_PINS); ++index) {
    digitalWrite(GameConfig::LED_PINS[index], on ? HIGH : LOW);
  }
}

void setupHardwareTestMode() {
  for (size_t index = 0; index < arraySize(GameConfig::LED_PINS); ++index) {
    pinMode(GameConfig::LED_PINS[index], OUTPUT);
  }

  for (size_t index = 0; index < arraySize(GameConfig::BUTTON_PINS); ++index) {
    pinMode(GameConfig::BUTTON_PINS[index], INPUT_PULLUP);
  }

  for (size_t index = 0; index < arraySize(GameConfig::CABLE_PINS); ++index) {
    pinMode(GameConfig::CABLE_PINS[index], INPUT_PULLUP);
  }

  writeAllHardwareTestLeds(false);
}

void updateHardwareTestMode() {
  const unsigned long now = millis();

  if (now - lastHardwareTestBlinkAt >= 500UL) {
    lastHardwareTestBlinkAt = now;
    hardwareTestLedsOn = !hardwareTestLedsOn;
    writeAllHardwareTestLeds(hardwareTestLedsOn);
  }

  if (now - lastHardwareTestDrawAt >= 100UL) {
    lastHardwareTestDrawAt = now;
    drawHardwareTestState();
  }
}

void beep() {
  if (!GameConfig::BUZZER_ENABLED) {
    return;
  }

  tone(
      GameConfig::BEEPER_PIN,
      GameConfig::BEEP_FREQUENCY_HZ,
      GameConfig::BEEP_DURATION_MS);
}
} // namespace

void setup() {
  if (GameConfig::BUZZER_ENABLED) {
    pinMode(GameConfig::BEEPER_PIN, OUTPUT);
  }

  if (GameConfig::HARDWARE_TEST_MODE) {
    setupHardwareTestMode();
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, GameConfig::OLED_I2C_ADDRESS)) {
    while (true) {
      beep();
      delay(1000);
    }
  }

  if (GameConfig::HARDWARE_TEST_MODE) {
    drawHardwareTestState();
    return;
  }

  countdownStartedAt = millis();
}

void loop() {
  if (GameConfig::HARDWARE_TEST_MODE) {
    updateHardwareTestMode();
    delay(10);
    return;
  }

  const uint8_t seconds = secondsRemaining();

  if (seconds != lastDisplayedSeconds) {
    lastDisplayedSeconds = seconds;
    drawCenteredCountdown(seconds);

    if (seconds > 0) {
      beep();
    }
  }

  delay(10);
}
