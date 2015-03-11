/* VikiInterface
 *
 * This is an implementation of the communciation routines for the
 * "Visual Kinetic control Interface" or Viki.
 *
 * http://www.panucatt.com/product_p/vikilcd.htm
 * *
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

#ifndef STANDARD_LIQUID_CRYSTAL_HH
#define STANDARD_LIQUID_CRYSTAL_HH

#include "Configuration.hh"
#include <stdint.h>
#include <avr/pgmspace.h>
#include "Pin.hh"
#include "LiquidCrystalSerial.hh"
#include "ButtonArray.hh"
#include <u8glib/u8g.h>

#define EMULATED_SCREEN_BUFFER_SIZE (LCD_SCREEN_WIDTH*LCD_SCREEN_HEIGHT)

class Viki2Interface : public VirtualDisplay, public ButtonArray {

public:
  Viki2Interface();

	u8g_t u8g;	
	
  // Both LCD and ButtonArray will call init()
  void init();

  // LCD Public routines
  bool hasI2CDisplay();
  void setToolLED(uint8_t toolID, bool state);
  void setHBPLED(bool state);
  void setBuzzer(bool state);
  
  // ButtonArray public routines
  void scanButtons();
  bool getButton(ButtonName &button);
  void clearButtonPress();
  void setButtonDelay(micros_t delay);
  bool isButtonPressed(ButtonArray::ButtonName button);
  void setLED(bool on);
	
	void draw();

private:
  bool has_i2c_lcd;
	
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

  void createChar(uint8_t, uint8_t[]);
  void setCursor(uint8_t, uint8_t);
  void setRow(uint8_t);
  void setCursorExt(int8_t col, int8_t row);

  void write(uint8_t);

  uint8_t _xcursor;
  uint8_t _ycursor;

	uint8_t _fontHeight;
	uint8_t _fontWidth;
	const u8g_fntpgm_uint8_t* _font;
	
	char screenBuffer[EMULATED_SCREEN_BUFFER_SIZE];
	
};



#endif // STANDARD_LIQUID_CRYSTAL_HH
