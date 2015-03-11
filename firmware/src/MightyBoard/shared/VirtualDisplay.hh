/* VirtualDisplay
 * 
 * This is a base class for control of a HD44780-based LCD display.
 * It should be subclassed to provide specific implementation of the
 * communication routines for specific hardware.
 * 
 * For example, the standard OEM MBI hardware uses a shift register to
 * send data to the LCD display.  Other hardware might use I2C to do
 * accomplish the same thing.
 * 
 * This base class contains the initialization and convenience methods
 * that are similar for all LCD displays.  These methods rely on the
 * subclass' implementaiton of the low level communication routines
 * such as send, writeSerial, write4bits, and pulseEnable.
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

#ifndef VIRTUAL_DISPLAY_HH
#define VIRTUAL_DISPLAY_HH

#include <stdint.h>
#include <avr/pgmspace.h>
#include "Pin.hh"

// Custom chars
#define LCD_CUSTOM_CHAR_DOWN 0
#define LCD_CUSTOM_CHAR_EXTRUDER_NORMAL 2
#define LCD_CUSTOM_CHAR_EXTRUDER_HEATING 3
#define LCD_CUSTOM_CHAR_PLATFORM_NORMAL 4
#define LCD_CUSTOM_CHAR_PLATFORM_HEATING 5
#define LCD_CUSTOM_CHAR_FOLDER 6 // Must not be 0
#define LCD_CUSTOM_CHAR_RETURN 7 // Must not be 0

#define LCD_CUSTOM_CHAR_DEGREE 0xdf // MAY ALSO BE 0xdf
#define LCD_CUSTOM_CHAR_UP 0x5e     // ^
#define LCD_CUSTOM_CHAR_RIGHT 0x7e // right pointing arrow (0x7f is left pointing)

// Font sizes
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// TODO:  make variable names for rs, rw, e places in the output vector

class VirtualDisplay {
public:
  
  /** Added by MakerBot Industries to support storing strings in flash **/
  void writeInt(uint16_t value, uint8_t digits);
  void moveWriteInt(uint8_t col, uint8_t row, uint16_t value, uint8_t digits);
  void writeInt32(uint32_t value, uint8_t digits);
  void writeFloat(float value, uint8_t decimalPlaces,
                  uint8_t rightJustifyToCol);

  void writeString(char message[]);

  /** Display the given line until a newline or null is encountered.
   * Returns a pointer to the first character not displayed.
   */
  char *writeLine(char *message);

  void writeFromPgmspace(const prog_uchar message[]);
  void moveWriteFromPgmspace(uint8_t col, uint8_t row,
                             const prog_uchar message[]);
														 
														 
  // Virtual functions
  virtual void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS) = 0;

  virtual void clear() = 0;
  virtual void home() = 0;

  virtual void homeCursor() = 0;      // faster version of home()
  virtual void clearHomeCursor() = 0; // clear() and homeCursor() combined
  virtual void noDisplay() = 0;
  virtual void display() = 0;
  virtual void noBlink() = 0;
  virtual void blink() = 0;
  virtual void noCursor() = 0;
  virtual void cursor() = 0;
  virtual void scrollDisplayLeft() = 0;
  virtual void scrollDisplayRight() = 0;
  virtual void leftToRight() = 0;
  virtual void rightToLeft() = 0;
  virtual void autoscroll() = 0;
  virtual void noAutoscroll() = 0;

  virtual void createChar(uint8_t, uint8_t[]) = 0;
  virtual void setCursor(uint8_t, uint8_t) = 0;
  virtual void setRow(uint8_t) = 0;
  virtual void setCursorExt(int8_t col, int8_t row) = 0;

  virtual void write(uint8_t) = 0;

};

#endif // VIRTUAL_DISPLAY_HH
