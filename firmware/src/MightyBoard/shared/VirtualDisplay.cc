/* Virutal Display
 *
 * This is a base class for a generic LCD display.
 * It should be subclassed to provide specific implementation of the
 * communication routines for specific hardware.
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
#include "VirtualDisplay.hh"
#include "Configuration.hh"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <util/delay.h>


/*********** mid level commands, for sending data/cmds */


void VirtualDisplay::writeInt(uint16_t value, uint8_t digits) {

    if(digits > 5)
        digits = 5;
    writeInt32(value, digits);
}

void VirtualDisplay::moveWriteInt(uint8_t col, uint8_t row, uint16_t value, uint8_t digits) {
     setCursor(col, row);
     writeInt(value, digits);
}

void VirtualDisplay::writeInt32(uint32_t value, uint8_t digits) {

     uint32_t currentDigit = 1;
     uint32_t nextDigit;
     bool nonzero_seen = false;

     if ( digits > 9 )
	  digits = 9;

     for (uint8_t i = digits; i; i--)
	  currentDigit *= 10;

     for (uint8_t i = digits; i; i--) {
	  nextDigit = currentDigit / 10;
	  char c;
	  int8_t d = (value % currentDigit) / nextDigit;
	  if ( nonzero_seen || d != 0 || i == 1) {
	       c = d + '0';
	       nonzero_seen = true;
	  }
	  else
	       c = ' ';
	  write(c);
	  currentDigit = nextDigit;
     }
}

//From: http://www.arduino.cc/playground/Code/PrintFloats
//tim [at] growdown [dot] com   Ammended to write a float to lcd
//If rightJusityToCol = 0, the number is left justified, i.e. printed from the
//current cursor position.  If it's non-zero, it's right justified to end at rightJustifyToCol column.

#define MAX_FLOAT_STR_LEN 20

void VirtualDisplay::writeFloat(float value, uint8_t decimalPlaces, uint8_t rightJustifyToCol) {
        // this is used to cast digits
        int digit;
        float tens = 0.1;
        int tenscount = 0;
        int i;
        float tempfloat = value;
	uint8_t p = 0;
	char str[MAX_FLOAT_STR_LEN + 1];

        // make sure we round properly. this could use pow from <math.h>, but doesn't seem worth the import
        // if this rounding step isn't here, the value  54.321 prints as 54.3209

        // calculate rounding term d:   0.5/pow(10,decimalPlaces)
        float d = 0.5;
        if (value < 0) d *= -1.0;

        // divide by ten for each decimal place
        for (i = decimalPlaces; i; i--) d/= 10.0;

        // this small addition, combined with truncation will round our values properly
        tempfloat +=  d;

        // first get value tens to be the large power of ten less than value
        // tenscount isn't necessary but it would be useful if you wanted to know after this how many chars the number will take

        tempfloat = fabsf(tempfloat);
        while ((tens * 10.0) <= tempfloat) {
                tens *= 10.0;
                tenscount += 1;
        }

        // write out the negative if needed
        if (value < 0) str[p++] = '-';

        if (tenscount == 0) str[p++] = '0';

        for (i = tenscount; i; i--) {
                digit = (int) (tempfloat/tens);
                str[p++] = digit + '0';
                tempfloat = tempfloat - ((float)digit * tens);
                tens /= 10.0;
        }

        // if no decimalPlaces after decimal, stop now and return
        if (decimalPlaces > 0) {
		// otherwise, write the point and continue on
		str[p++] = '.';

		// now write out each decimal place by shifting digits one by one into the ones place and writing the truncated value
		for (i = decimalPlaces; i; i--) {
			tempfloat *= 10.0;
			digit = (int) tempfloat;
			str[p++] = digit+'0';
			// once written, subtract off that digit
			tempfloat = tempfloat - (float) digit;
		}
	}

	str[p] = '\0';

	if ( rightJustifyToCol ) {
		setCursorExt(rightJustifyToCol - p, -1);
	}
	writeString(str);
}

char* VirtualDisplay::writeLine(char* message) {
	char* letter = message;
	while (*letter != 0 && *letter != '\n') {
		//INTERFACE_RLED.setValue(true);
		write(*letter);
		letter++;

	}
	return letter;
}

void VirtualDisplay::writeString(char message[]) {
	char* letter = message;
	while (*letter != 0) {
		write(*letter);
		letter++;
	}
}

void VirtualDisplay::writeFromPgmspace(const prog_uchar message[]) {
	char letter;
	while ((letter = pgm_read_byte(message++)))
		write(letter);
}

void VirtualDisplay::moveWriteFromPgmspace(uint8_t col, uint8_t row,
						const prog_uchar message[]) {
	setCursor(col, row);
	writeFromPgmspace(message);
}
