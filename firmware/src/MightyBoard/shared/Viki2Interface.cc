/* VikiInterface
 *
 * This is an implementation of the communciation routines for the
 * "Visual Kinetic control Interface" or Viki.
 *
 * http://www.panucatt.com/product_p/vikilcd.htm
 *
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

#include "Viki2Interface.hh"
#include "Configuration.hh"

#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include "TWI.hh"

static bool initializationComplete = false;
static micros_t ButtonDelay;
static uint8_t previousButton; ///< state of the button pins from the previous

// Encoder support
static int8_t encClicks, encDir, encTurning;

Viki2Interface::Viki2Interface() {
  has_i2c_lcd = false;
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
  VIKI_ENC_DDR_A  &= ~(VIKI_ENC_MASK_A); // Set pin as output
  VIKI_ENC_PORT_A |=   VIKI_ENC_MASK_A;  // Enable pullup
  VIKI_ENC_DDR_B  &= ~(VIKI_ENC_MASK_B); // Set pin as output
  VIKI_ENC_PORT_B |=   VIKI_ENC_MASK_B;  // Enable pullup
	
	// Configure button input
  VIKI_CENTER_BUTTON_DDR &= ~(VIKI_CENTER_BUTTON_MASK); // Set pin as input
//  VIKI_CENTER_BUTTON_PORT |= (VIKI_CENTER_BUTTON_MASK); // Enable pullup

	// Configure display control lines
	VIKI_DISPLAY_CS_DDR |= VIKI_DISPLAY_CS_MASK;
	VIKI_DISPLAY_A0_DDR |= VIKI_DISPLAY_A0_MASK;	
	
  // Turn off the LEDs.  They default on, so they will briefly blink
  // during the initialization process.
  setToolLED(0, false);
  setToolLED(1, false);
  setHBPLED(false);

  has_i2c_lcd = true;
  initializationComplete = true;
}


void Viki2Interface::setToolLED(uint8_t toolID, bool state) {
}

void Viki2Interface::setHBPLED(bool state) {
}

void Viki2Interface::setBuzzer(bool state) {
}

// Return true if we have an LCD connected
bool Viki2Interface::hasI2CDisplay() { return true; }

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

void Viki2Interface::setLED(bool on) { return; }


void Viki2Interface::begin(uint8_t cols, uint8_t rows, uint8_t charsize) {
	// Initialize the display, Hardware SPI
	u8g_InitHWSPI(&u8g, &u8g_dev_st7565_nhd_c12864_hw_spi, VIKI_DISPLAY_CS_PN, VIKI_DISPLAY_A0_PN, U8G_PIN_NONE);

	u8g_SetContrast(&u8g, 32);  //TODO: menu to configure constrast?
	u8g_SetRot180(&u8g);				//Rotate display 180-degrees
	
	// Font and font metrics
	_font = u8g_font_6x12r;
	_fontHeight = 10;
	_fontWidth = 6;
	
	clear();
}

void Viki2Interface::clear() { 
	memset(screenBuffer, ' ', EMULATED_SCREEN_BUFFER_SIZE);
	_xcursor = 0; _ycursor = 0;
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
	_xcursor = col;
	_ycursor = row;
	if (_xcursor >= LCD_SCREEN_WIDTH) {
		_xcursor = 0;
	}
	if (_ycursor >= LCD_SCREEN_HEIGHT) {
		_ycursor = 0;
	}
}

void Viki2Interface::setRow(uint8_t row) {
	setCursor(0, row);
}

void Viki2Interface::setCursorExt(int8_t col, int8_t row) {
	if (col == -1) col = _xcursor;
	if (row == -1) row = _ycursor;
	setCursor(col, row);
}

void Viki2Interface::write(uint8_t byte) {
	screenBuffer[_ycursor*20+_xcursor] = byte;
	_xcursor++;
	if (_xcursor >= LCD_SCREEN_WIDTH) {
		_xcursor = 0;
		_ycursor++;
		if (_ycursor >= LCD_SCREEN_HEIGHT) {
			_ycursor = 0;
		}
	}
}

void Viki2Interface::draw() {
  u8g_FirstPage(&u8g);  
  do {
		for (int y=0; y<LCD_SCREEN_HEIGHT; y++) {
			for (int x=0; x<LCD_SCREEN_WIDTH; x++) {
				char c = screenBuffer[y*LCD_SCREEN_WIDTH+x];
        switch (c) {
          case ' ':
            //Noop
            break;
            
          case LCD_CUSTOM_CHAR_RIGHT:
						u8g_SetFont(&u8g,u8g_font_6x12_75r);
						u8g_DrawGlyph(&u8g, x*(_fontWidth), (y+1)*_fontHeight, 0x37);
            break;
            
          case LCD_CUSTOM_CHAR_UP:
						u8g_SetFont(&u8g,u8g_font_6x12_75r);
						u8g_DrawGlyph(&u8g, x*(_fontWidth), (y+1)*_fontHeight, 0x33);
            break;
            
          case LCD_CUSTOM_CHAR_DOWN:
						u8g_SetFont(&u8g,u8g_font_6x12_75r);
						u8g_DrawGlyph(&u8g, x*(_fontWidth), (y+1)*_fontHeight, 0x3D);
            break;
            
          case LCD_CUSTOM_CHAR_DEGREE:
						u8g_SetFont(&u8g,u8g_font_6x12_75r);
						u8g_DrawGlyph(&u8g, x*(_fontWidth), (y+1)*_fontHeight, 0x22);
            break;
            
          case 0xff:
            u8g_SetFont(&u8g,u8g_font_6x12_75r);  
            u8g_DrawGlyph(&u8g, x*(_fontWidth), (y+1)*_fontHeight, 0x2E);  
            break;
            
          default:
            u8g_SetFont(&u8g,_font);
            u8g_DrawGlyph(&u8g, x*(_fontWidth), (y+1)*_fontHeight, c);
            break;
				} //switch
			} //x loop
		} // y loop 
  } while( u8g_NextPage(&u8g) );
}
