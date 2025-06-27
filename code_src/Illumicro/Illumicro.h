#ifndef Illumicro_h
#define Illumicro_h

#include <Arduino.h>
#include <Wire.h>

/**
 * @file Illumicro.h
 * @brief A library for controlling a 13x9 RGB LED matrix with the IS31FL3741A driver.
 *
 * Provides functions to set LED brightness, push buffered frames, set maximum current per LED,
 * and run demo animations. Uses ILM139C mapping for accurate LED addressing.
 */
class Illumicro {
public:
  /**
   * @brief Constructor for Illumicro.
   * @param addr I2C address of the IS31FL3741A (default 0x30).
   */
  Illumicro(uint8_t addr = 0x30);

  /**
   * @brief Initialize the LED driver with default settings (GCC = 127).
   */
  void begin();

  /**
   * @brief Set the brightness of a specific LED.
   * @param row Row index (0-8).
   * @param col Column index (0-12).
   * @param color Color index (0=Blue, 1=Green, 2=Red).
   * @param pwm PWM value (0-255).
   */
  void setLED(uint8_t row, uint8_t col, uint8_t color, uint8_t pwm);

  /**
   * @brief Set the global current control (GCC) value.
   * @param gcc GCC value (0-255, default 127).
   */
  void setGCC(uint8_t gcc);

  /**
   * @brief Set the maximum current for all LEDs as a percentage using SL.
   * @param percent Desired current percentage (0-100).
   * @param r_iset R_ISET resistor value in ohms (e.g., 2550).
   */
  void setGlobalMaxCurrentPercent(uint8_t percent, uint16_t r_iset);

  /**
   * @brief Set the maximum current for a specific LED.
   * @param row Row index (0-8).
   * @param col Column index (0-12).
   * @param color Color index (0=Blue, 1=Green, 2=Red).
   * @param current_mA Desired current in mA.
   * @param r_iset R_ISET resistor value in ohms.
   */
  void setLEDMaxCurrent(uint8_t row, uint8_t col, uint8_t color, float current_mA, uint16_t r_iset);

  /**
   * @brief Push a buffered frame to the driver.
   * @param frame 3D array of PWM values [row][col][color] (9x13x3).
   */
  void pushFrame(uint8_t frame[9][13][3]);

  /**
   * @brief Get the PWM register address for a specific LED.
   * @param row Row index (0-8).
   * @param col Column index (0-12).
   * @param color Color index (0=Blue, 1=Green, 2=Red).
   * @return Register address.
   */
  uint8_t getRegisterAddress(uint8_t row, uint8_t col, uint8_t color);

  /**
   * @brief Run a color wave demo animation (~10 seconds).
   */
  void demo();

  /**
   * @brief Run a breathing demo animation (~20 seconds).
   */
  void demoBreathing();

private:
  uint8_t _addr; ///< I2C address of the driver
  uint8_t _gcc; ///< Current GCC value
  void unlock(); ///< Unlock the command register
  void selectPage(uint8_t page); ///< Select the I2C page
  void writeReg(uint8_t reg, uint8_t val); ///< Write to a register
  void writeBlock(uint8_t page, uint8_t startAddr, const uint8_t* data, uint16_t len); ///< Write multiple registers in chunks
  void writeTwoPages(uint8_t first_page, uint8_t value); ///< Fill two pages with a single value
  void hsvToRgb(uint8_t h, uint8_t s, uint8_t v, uint8_t &r, uint8_t &g, uint8_t &b); ///< Convert HSV to RGB
  static const uint8_t ILM139C[9][39]; ///< LED mapping table
};

#endif
