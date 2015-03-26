/* VikiInterface
 *
 * This is an implementation of the communciation routines for the
 * "Visual Kinetic control Interface" or Viki2.
 *
 * http://www.panucatt.com/product_p/vikilcd.htm
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

 // Credits:
 // The LCD display code was created by referencing to two exsiting implementations.
 // u8glib - https://code.google.com/p/u8glib/ 
 //          (New BSD License)
 // Adafruit's st7565 implementation - https://github.com/adafruit/ST7565-LCD 
 //                                    (GNU Lesser General Public)

#include "Viki2Interface.hh"
#include "Configuration.hh"

#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include "TWI.hh"
#include "fontdata.h"
#ifdef HAS_VIKI2_INTERFACE

static bool initializationComplete = false; // Only init() once
static micros_t ButtonDelay;
static uint8_t previousButton; ///< state of the button pin from the previous scan

// Encoder support
static int8_t encClicks, encDir, encTurning;

Viki2Interface::Viki2Interface() {
  previousButton = 0;

  // Encoder state
  encDir = 0;
  encTurning = 0;
  encClicks = 0;

  init();
}

void Viki2Interface::init() {
  // Only initialize once.  Due to multiple inheritance, its possible
  // that multiple people will request an initialization.
  if (initializationComplete)
    return;

  // Configure encoder GPIO pins
  VIKI_ENC_DDR_A  &= ~(VIKI_ENC_MASK_A); // Set pin as input
  VIKI_ENC_PORT_A |=   VIKI_ENC_MASK_A;  // Enable pullup
  VIKI_ENC_DDR_B  &= ~(VIKI_ENC_MASK_B); // Set pin as input
  VIKI_ENC_PORT_B |=   VIKI_ENC_MASK_B;  // Enable pullup

	// Configure button input
  VIKI_CENTER_BUTTON_DDR &= ~(VIKI_CENTER_BUTTON_MASK); // Set pin as input

	// Configure display control lines (outputs)
	VIKI_DISPLAY_CS_DDR |= VIKI_DISPLAY_CS_MASK;
	VIKI_DISPLAY_A0_DDR |= VIKI_DISPLAY_A0_MASK;	

  // Configure LED Ring outputs
  VIKI_DISPLAY_BLUE_LED_DDR |= VIKI_DISPLAY_BLUE_LED_MASK;
  VIKI_DISPLAY_RED_LED_DDR  |= VIKI_DISPLAY_RED_LED_MASK;

  // Defaut the indicators to OFF
  _indicators = 0;

  initializationComplete = true;
}

void Viki2Interface::setRedLED(bool state) {
  if (state)
    VIKI_DISPLAY_RED_LED_PORT |= VIKI_DISPLAY_RED_LED_MASK;
  else
    VIKI_DISPLAY_RED_LED_PORT &= ~VIKI_DISPLAY_RED_LED_MASK;
}

void Viki2Interface::setBlueLED(bool state) {
  if (state)
    VIKI_DISPLAY_BLUE_LED_PORT |= VIKI_DISPLAY_BLUE_LED_MASK;
  else
    VIKI_DISPLAY_BLUE_LED_PORT &= ~VIKI_DISPLAY_BLUE_LED_MASK;
}

void Viki2Interface::setToolIndicator(uint8_t toolID, bool state) {
  char mask = (toolID ? VIKI2_EXTRUDER2_INDICATOR_MASK : VIKI2_EXTRUDER1_INDICATOR_MASK);
  if (state)
    _indicators |= mask;
  else
    _indicators &= ~mask;

  char c = state?LCD_CUSTOM_CHAR_EXTRUDER_HEATING:LCD_CUSTOM_CHAR_EXTRUDER_NORMAL;
  if (toolID == 0) {
    drawGlyph(c,  1, 123, false); 
  } else {
    drawGlyph(c,  4, 123, false); 
  }

}

void Viki2Interface::setHBPIndicator(bool state) {
  if (state)
    _indicators |= VIKI2_HBP_INDICATOR_MASK;
  else 
    _indicators &= ~VIKI2_HBP_INDICATOR_MASK;

  char c = state?LCD_CUSTOM_CHAR_PLATFORM_HEATING:LCD_CUSTOM_CHAR_PLATFORM_NORMAL;
  drawGlyph(c,  7, 123, false); 
}

void Viki2Interface::setCoolingFanIndicator(bool state) {
  if (state)
    _indicators |= VIKI2_HBP_INDICATOR_MASK;
  else
    _indicators &= ~VIKI2_HBP_INDICATOR_MASK;

  char c = state?LCD_CUSTOM_CHAR_FAN:LCD_CUSTOM_CHAR_FAN_OFF;
  drawGlyph(c,  6, 123, false);    
}

void Viki2Interface::setBuzzer(bool state) { return; }

/*********** ButtonArray implementation *******/

void Viki2Interface::scanButtons() {
  if (buttonPressWaiting ||
      (buttonTimeout.isActive() && !buttonTimeout.hasElapsed()))
    return;

  uint8_t newButton = VIKI_CENTER_BUTTON_PIN & VIKI_CENTER_BUTTON_MASK;

  // Encoder
  bool encA = (VIKI_ENC_PIN_A & VIKI_ENC_MASK_A) == 0;
  bool encB = (VIKI_ENC_PIN_B & VIKI_ENC_MASK_B) == 0;
	
  if ( !encA && !encB ) {
       // We are at a detent
       if ( encTurning ) {
         // We were turning AND this wasn't a bounce: accumulate it
         encClicks += encDir;
         encTurning = 0;
       }
       // Wipe this info out.  If we merely registered a bounce,
       // then encTurning was not true and so we didn't accumulate a turn.
       encDir = 0;
  }
  else if ( encA && encB ) {
       // We're at the half way point
       // This means that we're turning and isn't just bounce
       encTurning = 1;
  }
  else if ( !encTurning ) {
       // This is a bounce OR we've started to turn
       // Note direction
       encDir = encA ? -1 : 1;
  }

  buttonTimeout.clear();

  // Process actual button presses before encoder info
  // That is, we give more weight to a button press, even
  // if it happens whilst turning the encoder wheel.
  if (newButton != previousButton) {
    if (!buttonPressWaiting && newButton == 0) {  //active low
        	buttonPress = CENTER;
					buttonPressWaiting = true;
					buttonTimeout.start(ButtonDelay);
    }
  } else {
	  if ( encClicks != 0 ) {
	       if ( encClicks > 0 ) 
					 buttonPress = DOWN;
				 else 
					 buttonPress = UP;
	       encClicks = 0;
 				 buttonPressWaiting = true;
	  }
  }

  previousButton = newButton;
}

bool Viki2Interface::getButton(ButtonName &button) {
  bool buttonValid;
  uint8_t buttonNumber;

  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    buttonValid = buttonPressWaiting;
    buttonNumber = buttonPress;
    buttonPressWaiting = false;
  }

  if (buttonValid) {
    button = (ButtonName)(buttonNumber);
  }

  return buttonValid;
}

void Viki2Interface::clearButtonPress() { previousButton = 0; }

bool Viki2Interface::isButtonPressed(ButtonArray::ButtonName button) { return 0; }

void Viki2Interface::setButtonDelay(micros_t delay) { ButtonDelay = delay; }

void Viki2Interface::setLED(bool on) { setBlueLED(on); }

void Viki2Interface::begin(uint8_t cols, uint8_t rows, uint8_t charsize) {

  // Initialize the SPI bus.  Note that these settings are the same as lib_sd used
  // to access the SD Card.  Note that the f_OSC bits are omitted as we want to defer
  // to lib_sd to set the clock speed.  We'll set/restore f_OSC if necessary at time
  // of use.
  SPCR &= ~(1 << SPIE);  // Clear SPI Interrupt Enable Bit
  SPCR |=  (1 << SPE);   // Set SPI Enable bit
  SPCR &= ~(1 << DORD);  // Set Data Order bit: MSB first
  SPCR |=  (1 << MSTR);  // Set Master mode bit
  SPCR &= ~(1 << CPOL);  // Clear Clock Polarity bit: SCK low when idle
  SPCR &= ~(1 << CPHA);  // ClearClock Phase bit: sample on rising SCK edge

  st7565_enable();

  st7565_command(0x040);  /* set display start line */
  st7565_command(CMD_SET_ADC_NORMAL);   /* ADC set to normal.*/
  st7565_command(CMD_SET_COM_REVERSE);  /* common output mode: set scan direction reverse operation (flip display)*/
  st7565_command(CMD_SET_DISP_NORMAL);  /* display normal, bit val 0: LCD pixel off. */
  st7565_command(CMD_SET_BIAS_9);       /* LCD bias 1/9 */
  st7565_command(CMD_SET_POWER_CONTROLLER | 
                    CMD_SET_POWER_CONTROLLER_BOOSTER_ON   | 
                    CMD_SET_POWER_CONTROLLER_REGULATOR_ON |
                    CMD_SET_POWER_CONTROLLER_FOLLOWER_ON);  /* all power  control circuits on */
  st7565_command(CMD_SET_BOOSTER_MODE_SELECT);          /* set booster ratio to */
  st7565_command(CMD_SET_BOOSTER_2x3x4x);               /* 2x, 3x, 4x  (4x) */
  st7565_command(CMD_SET_V0_VOLTAGE_REGULATOR_INTERNAL_RESISTOR_RATIO + 7); /* set V0 voltage resistor ratio to large */
  st7565_command(CMD_SET_CONTRAST_MODE_SELECT);         /* set contrast */
  st7565_command(0x008);                                /* contrast: 0x008 is a good value for NHD C12864, Nov 2012: User reports that 0x1a is much better */
  st7565_command(CMD_SET_SLEEP_MODE_SELECT);            /* sleep mode select */
  st7565_command(0x000);                                /* disable */
  st7565_command(CMD_DISPLAY_ON);                       /* display on */
  _delay_ms(100);         /* delay 100 ms */

  // Blink the display to indicate the initalization has completed.
  st7565_command(CMD_SET_ALLPTS_ON);  /* display all points, ST7565 */
  _delay_ms(200);         /* delay 200 ms */
  st7565_command(CMD_SET_ALLPTS_NORMAL);  /* normal display */

  st7565_disable();

  clear();

  //Blink the indicators
  setToolIndicator(0, true);
  setToolIndicator(1, true);
  setHBPIndicator(true);
  setCoolingFanIndicator(true);
  _delay_ms(500);
  setToolIndicator(0, false);
  setToolIndicator(1, false);
  setHBPIndicator(false);
  setCoolingFanIndicator(false);

	
}

void Viki2Interface::clear() { 

  uint8_t page, col;
  
  st7565_enable();
  for(page = 0; page < LCD_CONTROLLER_PAGES; page++) {
    st7565_command(CMD_SET_PAGE | page);
    st7565_command(CMD_SET_COLUMN_LOWER | 0x00);
    st7565_command(CMD_SET_COLUMN_UPPER | 0x00);
    for(col = 0; col < 132; col++) {
      st7565_data(0);
    }     
  }
  st7565_disable();

  drawIndicators();
}

void Viki2Interface::drawIndicators() {
  // Draw the dividing line
  verticalLine(121);

  // Draw the extruder labels
  drawGlyph(LCD_CUSTOM_CHAR_1,  0, 123, false);
  drawGlyph(LCD_CUSTOM_CHAR_2,  3, 123, false);

  // Draw the indicators, use the current state.
  setToolIndicator(0, _indicators & VIKI2_EXTRUDER1_INDICATOR_MASK);
  setToolIndicator(1, _indicators & VIKI2_EXTRUDER2_INDICATOR_MASK);
  setHBPIndicator(_indicators & VIKI2_HBP_INDICATOR_MASK);
  setCoolingFanIndicator(_indicators & VIKI2_COOLING_FAN_INDICATOR_MASK);
}

void Viki2Interface::home() {
	setCursor(0,0);
}

void Viki2Interface::homeCursor() { 
	home();
}

void Viki2Interface::clearHomeCursor() {
	clear();
	home();
}

void Viki2Interface::noDisplay() {

}

void Viki2Interface::display() {
	
}

void Viki2Interface::noBlink() {
	
}

void Viki2Interface::blink() {
	
}

void Viki2Interface::noCursor() {
	
}

void Viki2Interface::cursor() {
	
}

void Viki2Interface::scrollDisplayLeft() {
	
}

void Viki2Interface::scrollDisplayRight() {
	
}

void Viki2Interface::leftToRight() {
	
}

void Viki2Interface::rightToLeft() {
	
}

void Viki2Interface::autoscroll(){
	
}

void Viki2Interface::noAutoscroll() {
	
}

void Viki2Interface::createChar(uint8_t, uint8_t[]) {
	
}

void Viki2Interface::setCursor(uint8_t col, uint8_t row) {
  _cursorx = col; _cursory=row;
}

void Viki2Interface::setRow(uint8_t row) {
  _cursory = row;
  _cursorx = 0;
}

void Viki2Interface::setCursorExt(int8_t col, int8_t row) {
  if (col >= 0) _cursorx=col;
  if (row >= 0) _cursory=row;
}

void Viki2Interface::drawGlyph(uint8_t b, uint8_t row, uint8_t col, bool overline) {
    // Simple character mapping.
    if (b == 0xFF) {
      //noop: The code below will generate a solid rectangle for 0xFF
    }  else if (b >= LCD_START_CHAR_ASCII_VALUE &&
                b <= LCD_END_CHAR_ASCII_VALUE) {
       b -= (LCD_START_CHAR_ASCII_VALUE - LCD_FONT_START_STANDARD_CHAR_RANGE);
    }  else if (b > LCD_END_CHAR_ASCII_VALUE) {
       b = LCD_CUSTOM_CHAR_UNKNOWN_GLYPH;
    }

    uint8_t line = overline?0x01:0x00;

    // Start the conversation with the st7565
    st7565_enable();

    // Calculate the page and column to start writing
    st7565_command(CMD_SET_PAGE | row);
    st7565_command(CMD_SET_COLUMN_LOWER | (col & 0x0f) );
    st7565_command(CMD_SET_COLUMN_UPPER | ((col >> 4) & 0x0f));
    st7565_command(CMD_RMW);

    // Write the character.  If were 0xFF, it sa solid rectangle.
    // Otherwise lookup the font data and send it to the display.
    if (b == 0xFF) {
      for (int c=0; c<5; c++) {
         st7565_data(0xFF);
      }
    } else {
      for (int c=0; c<5; c++) {
         st7565_data(pgm_read_byte(font+(b*LCD_FONT_DATA_WIDTH)+c) | line);
      }
    }

    // End the conversation iwth the st7565.
    st7565_disable();
}

void Viki2Interface::write(uint8_t b) {
    uint8_t xcol = (_cursorx * LCD_FONT_WIDTH);
    drawGlyph(b, _cursory, xcol, false);

    // Advance the cursor and wrap as apporopriate.
    _cursorx++;
    if (_cursorx >= LCD_SCREEN_WIDTH) {
      _cursorx=0;
      _cursory++;
    }
    if (_cursory >= LCD_SCREEN_HEIGHT) {
      _cursory=0;
    }
}

void Viki2Interface::verticalLine(uint8_t x) {
      // Start the conversation with the st7565
    st7565_enable();

    for (int page = 0; page < LCD_CONTROLLER_PAGES; page++) {
      st7565_command(CMD_SET_PAGE | page);
      uint8_t xcol = x; //LCD_CONTROLLER_WIDTH_IN_PIXELS - 1 - x;

      st7565_command(CMD_SET_COLUMN_LOWER | (xcol & 0x0f));
      st7565_command(CMD_SET_COLUMN_UPPER | ((xcol >> 4) & 0x0f));
      st7565_command(CMD_RMW);
    
      st7565_data(0xFF);
    }

    // End the conversation iwth the st7565.
    st7565_disable();
}

void Viki2Interface::st7565_write(uint8_t b) {
    // Pilfered from lib_sd.  Probably can consolidate this code.
    uint8_t tries = 0;
    SPDR = b;

    /* wait for byte to be shifted out */
    while(!(SPSR & (1 << SPIF)) && (tries++ < 100))
      _delay_us(1);
    
    SPSR &= ~(1 << SPIF);
}

void Viki2Interface::st7565_enable() {
  // Save the fOSC scaler and 2x mode to restore later.
  _saveSPIConfig = (SPCR << 1) & (SPSR & 0x10);
  
  // Set the fastest SPI speed possible (fOSC/4 x2 = 8 MHz)
  SPCR &= 0b11111100; // set prescaler to fOSC/4
  SPSR |= 0x1;        // enable 2x mode

  VIKI_DISPLAY_CS_PORT &= ~VIKI_DISPLAY_CS_MASK;  // CS Low (assert)
}

void Viki2Interface::st7565_disable() {
  // Restre fOSC scaler and 2x mode bit from where we stored it.
  SPCR |= (SPCR & 0xFC) | ((_saveSPIConfig >> 1) & 0x3);
  SPSR |= (SPCR & 0xFE) | (_saveSPIConfig & 0x1);

  VIKI_DISPLAY_CS_PORT |= VIKI_DISPLAY_CS_MASK; // CS High (de-assert)
}

void Viki2Interface::st7565_command(uint8_t c) {
  VIKI_DISPLAY_A0_PORT &= ~VIKI_DISPLAY_A0_MASK; // A0 LOW
  st7565_write(c);
}

void Viki2Interface::st7565_data(uint8_t c) {
  VIKI_DISPLAY_A0_PORT |= VIKI_DISPLAY_A0_MASK; // A0 High
  st7565_write(c);
}

#endif