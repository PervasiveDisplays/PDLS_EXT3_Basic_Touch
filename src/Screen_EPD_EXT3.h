///
/// @file Screen_EPD_EXT3.h
/// @brief Driver for Pervasive Displays iTC monochrome touch-screens and EXT3 board
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @n @b B-T-F
/// * Edition: Basic
/// * Family: Touch 2.70-P 3.70-P
/// * Update: Fast
/// * Feature: none
///
/// @n Supported screens
/// * 2.70"-Touch reference xTP270PGH0x
/// * 3.70"-Touch reference xTP370PGH0x
///
/// @author Rei Vilo
/// @date 22 Aug 2022
/// @version 541
///
/// @copyright (c) Rei Vilo, 2010-2022
/// @copyright Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
///
/// The highView Library Suite is shared under the Creative Commons licence Attribution-ShareAlike 4.0 International (CC BY-SA 4.0).
///
/// * Basic edition: for hobbyists and for basic usage
/// @n Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
///
/// @see https://creativecommons.org/licenses/by-sa/4.0/
///
/// @n Consider the advanced or commercial editions for professionals or organisations and for commercial usage
///

// SDK
#if defined(ENERGIA) // LaunchPad specific
#include "Energia.h"
#else // Arduino general
#include "Arduino.h"
#endif // SDK

// Configuration
#include "hV_Configuration.h"

#if (hV_CONFIGURATION_RELEASE < 530)
#error Required hV_CONFIGURATION_RELEASE 530
#endif // hV_CONFIGURATION_RELEASE

#ifndef SCREEN_EPD_EXT3_RELEASE
///
/// @brief Library release number
///
#define SCREEN_EPD_EXT3_RELEASE 541

// Other libraries
#include "SPI.h"
#include "Wire.h"
#include "hV_Screen_Buffer.h"

#if (hV_SCREEN_BUFFER_RELEASE < 507)
#error Required hV_SCREEN_BUFFER_RELEASE 507
#endif // hV_SCREEN_BUFFER_RELEASE

///
/// @brief Touch events
/// @{
#define TOUCH_EVENT_NONE 0 ///< no event
#define TOUCH_EVENT_PRESS 1 ///< press event
#define TOUCH_EVENT_RELEASE 2 ///< release event
#define TOUCH_EVENT_MOVE 3 ///< move event
/// @}
///

// Objects
//
///
/// @brief Class for Pervasive Displays iTC monochome touch screens
/// @details Screen controllers
/// * LCD: proprietary, SPI
/// * touch: proprietary, I2C
/// * fonts: no fonts
///
/// @note All commands work on the frame-buffer,
/// to be displayed on screen with flush()
///
class Screen_EPD_EXT3_Fast final : public hV_Screen_Buffer
{
  public:
    ///
    /// @brief Constructor with default pins
    /// @param eScreen_EPD_EXT3 size and model of the e-screen
    /// @param board board configuration
    /// @note To be used with begin() with no parameter
    ///
    Screen_EPD_EXT3_Fast(eScreen_EPD_EXT3_t eScreen_EPD_EXT3, pins_t board);

    ///
    /// @brief Initialisation
    /// @note Frame-buffer generated internally, not suitable for FRAM
    /// @warning begin() initialises SPI and I2C
    ///
    void begin();

    ///
    /// @brief Who Am I
    /// @return Who Am I string
    ///
    String WhoAmI();

    ///
    /// @brief Clear the screen
    /// @param colour default = white
    /// @note Clear next frame-buffer
    ///
    void clear(uint16_t colour = myColours.white);

    ///
    /// @brief Invert screen
    /// @param flag true to invert, false for normal screen
    ///
    void invert(bool flag);

    ///
    /// @brief Update the display, fast update
    /// @note Send the frame-buffer to the screen and refresh the screen
    ///
    void flush();

    ///
    /// @brief Regenerate the panel
    /// @details white-to-black-to-white cycle to reduce ghosting
    ///
    void regenerate();

    ///
    /// @brief Draw pixel
    /// @param x1 point coordinate, x-axis
    /// @param y1 point coordinate, y-axis
    /// @param colour 16-bit colour
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    void point(uint16_t x1, uint16_t y1, uint16_t colour);

    ///
    /// @brief Read pixel colour
    /// @param x1 point coordinate, x-axis
    /// @param y1 point coordinate, y-axis
    /// @return 16-bit colour, bits 15-11 red, bits 10-5 green, bits 4-0 blue
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    uint16_t readPixel(uint16_t x1, uint16_t y1);

    ///
    /// @brief Screen refresh time for the BWR screens
    /// @return Estimated refresh time in seconds
    ///
    uint8_t getRefreshTime();

  protected:
    /// @cond

    ///
    /// @brief General reset
    /// @param ms1 delay after PNLON_PIN, ms
    /// @param ms2 delay after RESET_PIN HIGH, ms
    /// @param ms3 delay after RESET_PIN LOW, ms
    /// @param ms4 delay after RESET_PIN HIGH, ms
    /// @param ms5 delay after CS_PIN CSS_PIN HIGH, ms
    ///
    void _reset(uint32_t ms1, uint32_t ms2, uint32_t ms3, uint32_t ms4, uint32_t ms5);

    // * Virtual =0 compulsory functions
    // Screen-specific
    ///
    /// @brief Send data through SPI
    /// @param index register
    /// @param data data
    /// @param size number of bytes
    /// @note Valid for all except large screens
    ///
    void _sendIndexData(uint8_t index, const uint8_t * data, uint32_t size);

    // Orientation
    ///
    /// @brief Set orientation
    /// @param orientation 1..3, 6, 7
    ///
    void _setOrientation(uint8_t orientation); // compulsory

    ///
    /// @brief Check and orient coordinates, logical coordinates
    /// @param x x-axis coordinate, modified
    /// @param y y-axis coordinate, modified
    /// @return false = success, true = error
    ///
    bool _orientCoordinates(uint16_t & x, uint16_t & y); // compulsory

    // Position

    // Write and Read
    /// @brief Set point
    /// @param x1 x coordinate
    /// @param y1 y coordinate
    /// @param colour 16-bit colour
    /// @n @b More: @ref Colour, @ref Coordinate
    ///
    void _setPoint(uint16_t x1, uint16_t y1, uint16_t colour);

    /// @brief Get point
    /// @param x1 x coordinate
    /// @param y1 y coordinate
    /// @return colour 16-bit colour
    /// @n @b More: @ref Colour, @ref Coordinate
    ///
    uint16_t _getPoint(uint16_t x1, uint16_t y1);

    ///
    /// @brief Convert
    /// @param x1 x-axis coordinate
    /// @param y1 y-axis coordinate
    /// @return index for _newImage[]
    ///
    uint32_t _getZ(uint16_t x1, uint16_t y1);

    ///
    /// @brief Wait for ready
    /// @details Wait for _pin.panelBusy low
    ///
    void _waitBusy();
    void _sendCommand8(uint8_t command);

    // Energy
    // No energy

    // * Other functions specific to the screen
    // Screen independent variables
    uint8_t * _newImage;
    bool _invert;
    uint16_t _widthScreen, _heightScreen;

    // Screen dependent variables
    pins_t _pin;
    eScreen_EPD_EXT3_t _eScreen_EPD_EXT3;
    uint8_t _codeExtra;
    uint8_t _codeSize;
    uint8_t _codeType;
    uint16_t _widthBuffer, _heightBuffer, _depthBuffer, _numberBuffer;
    uint32_t _sizePageColour, _sizeFrame;
    uint8_t _refreshTime;

    // === Touch
#if (TOUCH_MODE != USE_TOUCH_NONE)

    uint8_t _touchAddress;
    uint8_t _touchPrevious;
    uint16_t _touchX, _touchY;

    void _wireWriteRead(uint8_t address, uint8_t * dataWrite, size_t sizeWrite, uint8_t * dataRead = 0, size_t sizeRead = 0);

    void _beginTouch();
    void _getRawTouch(uint16_t & x0, uint16_t & y0, uint16_t & z0, uint16_t & t0); // compulsory
    bool _getInterruptTouch(); // compulsory

#endif // TOUCH_MODE

    /// @endcond
};

#endif // SCREEN_EPD_EXT3_RELEASE
