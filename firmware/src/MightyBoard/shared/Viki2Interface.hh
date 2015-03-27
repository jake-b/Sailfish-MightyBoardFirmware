/* Viki2Interface
 *
 * This is an implementation of the communciation routines for the
 * "Visual Kinetic control Interface" or Viki.
 *
 * http://www.panucatt.com/product_p/vikilcd2.htm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#ifndef VIKI2_INTERFACE_HH
#define VIKI2_INTERFACE_HH

#include "Configuration.hh"
#include <stdint.h>
#include <avr/pgmspace.h>
#include "Pin.hh"
#include "LiquidCrystalSerial.hh"
#include "ButtonArray.hh"

// Capabilities of the VIKI Display.  (HBP and tool LEDs, etc)
#define HAS_HBP_INDICATOR 1
#define HAS_TOOL_INDICATOR 1
#define HAS_COOLING_FAN_INDICATOR 1
#define HAS_NO_SIDEWAYS_ARROWS 1

//#define LCD_CONTROLLER_WIDTH_IN_PIXELS 132
#define LCD_CONTROLLER_PAGES 8

#define CMD_SET_ADC_NORMAL  0xA0
#define CMD_SET_ADC_REVERSE 0xA1

#define CMD_SET_COM_NORMAL  0xC0
#define CMD_SET_COM_REVERSE  0xC8

#define CMD_SET_DISP_NORMAL 0xA6
#define CMD_SET_DISP_REVERSE 0xA7

#define CMD_DISPLAY_OFF   0xAE
#define CMD_DISPLAY_ON    0xAF

#define CMD_SET_ALLPTS_NORMAL 0xA4
#define CMD_SET_ALLPTS_ON  0xA5

#define CMD_SET_BIAS_9 0xA2 
#define CMD_SET_BIAS_7 0xA3

#define CMD_SET_BOOSTER_MODE_SELECT  0xF8
#define CMD_SET_BOOSTER_2x3x4x  0
#define CMD_SET_BOOSTER_5  1
#define CMD_SET_BOOSTER_6  3

#define CMD_SET_V0_VOLTAGE_REGULATOR_INTERNAL_RESISTOR_RATIO 0b00100000

#define CMD_SET_CONTRAST_MODE_SELECT 0x81

#define CMD_SET_SLEEP_MODE_SELECT 0xAC

#define CMD_SET_POWER_CONTROLLER 0x28
#define CMD_SET_POWER_CONTROLLER_BOOSTER_ON   0b100
#define CMD_SET_POWER_CONTROLLER_REGULATOR_ON 0b010
#define CMD_SET_POWER_CONTROLLER_FOLLOWER_ON  0b001

#define CMD_SET_PAGE  0xB0
#define CMD_SET_COLUMN_UPPER  0x10
#define CMD_SET_COLUMN_LOWER  0x00

#define CMD_RMW  0xE0

#ifdef  LCD_CUSTOM_CHAR_DEGREE
#undef LCD_CUSTOM_CHAR_DEGREE
#define LCD_CUSTOM_CHAR_DEGREE 1
#endif

#define VIKI2_HBP_INDICATOR_MASK         (1<<0)
#define VIKI2_EXTRUDER1_INDICATOR_MASK   (1<<1)
#define VIKI2_EXTRUDER2_INDICATOR_MASK   (1<<2)
#define VIKI2_COOLING_FAN_INDICATOR_MASK (1<<3)

class Viki2Interface : public VirtualDisplay, public ButtonArray {

public:
  Viki2Interface();
	
  // Both LCD and ButtonArray will call init()
  void init();

  void setLED(bool on);  
  void setToolIndicator(uint8_t toolID, bool state);
  void setHBPIndicator(bool state);
  void setCoolingFanIndicator(bool state);
  void setBuzzer(bool state);

  // Viki Specific LEDs
  void setRedLED(bool state);
  void setBlueLED(bool state);
  
  // ButtonArray public routines
  void scanButtons();
  bool getButton(ButtonName &button);
  void clearButtonPress();
  void setButtonDelay(micros_t delay);
  bool isButtonPressed(ButtonArray::ButtonName button);

private:
	
  void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);

  void clear();
  void home();

  void homeCursor();      // faster version of home()
  void clearHomeCursor(); // clear() and homeCursor() combined
  void noDisplay();
  void display();
  void noBlink();
  void blink();
  void noCursor();
  void cursor();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void leftToRight();
  void rightToLeft();
  void autoscroll();
  void noAutoscroll();

  void drawGlyph(uint8_t b, uint8_t row, uint8_t xcol, bool overline);
  void verticalLine(uint8_t x);

  void drawIndicators();

  void createChar(uint8_t, uint8_t[]);
  void setCursor(uint8_t, uint8_t);
  void setRow(uint8_t);
  void setCursorExt(int8_t col, int8_t row);

  // Low level communciations
  void write(uint8_t);	
  void st7565_command(uint8_t c);
  void st7565_data(uint8_t c);
  void st7565_write(uint8_t c);

  void st7565_enable();
  void st7565_disable();

  //cursor position
  uint8_t _cursorx;
  uint8_t _cursory;
  uint8_t _indicators;

  uint8_t _saveSPIConfig;
};



#endif // STANDARD_LIQUID_CRYSTAL_HH
