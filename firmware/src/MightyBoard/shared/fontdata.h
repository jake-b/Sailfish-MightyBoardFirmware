/* Font Data
 *
 * This is a represenation of the font used on the standard HD44780 used
 * commonly with Mightyboards.
 * 
 * It was generated using bdfe (https://github.com/achilikin/bdfe)
 * with slight modifications to trim out.  You can find 
 * the modified version of bdfe here:
 * 				https://github.com/jake-b/bdfe
 *
 *       ./bdfe all dropempty droplast HD44780.bdf
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
 
#include <avr/io.h>
#include <avr/pgmspace.h> 
 
#ifndef __LCDFONT_H__
#define __LCDFONT_H__

#define LCD_FONT_WIDTH 6
#define LCD_FONT_HEIGHT 8

#define LCD_FONT_DATA_WIDTH 5 

#define LCD_START_CHAR_ASCII_VALUE 0x20
#define LCD_END_CHAR_ASCII_VALUE   0x7F
#define LCD_FONT_START_STANDARD_CHAR_RANGE 0xD

#define LCD_CUSTOM_CHAR_UNKNOWN_GLYPH 0x08
#define LCD_CUSTOM_CHAR_FAN     0x09
#define LCD_CUSTOM_CHAR_FAN_OFF 0x0A
#define LCD_CUSTOM_CHAR_1 0x0B
#define LCD_CUSTOM_CHAR_2 0x0C

const uint8_t font[] PROGMEM = { 
//     0 ' ' |01234567|
 0x10, //   0|   #    |
 0x20, //   1|  #     |
 0x40, //   2| #      |
 0x20, //   3|  #     |
 0x10, //   4|   #    |

//     1 ' ' |01234567|
 0x07, //   0|     ###|
 0x05, //   1|     # #|
 0x07, //   2|     ###|
 0x00, //   3|        |
 0x00, //   4|        |

//     2 ' ' |01234567|
 0x07, //   0|     ###|
 0x3E, //   1|  ##### |
 0x46, //   2| #   ## |
 0x3E, //   3|  ##### |
 0x07, //   4|     ###|

//     3 ' ' |01234567|
 0x07, //   0|     ###|
 0x3E, //   1|  ##### |
 0x7E, //   2| ###### |
 0x3E, //   3|  ##### |
 0x07, //   4|     ###|

//     4 ' ' |01234567|
 0x60, //   0| ##     |
 0x20, //   1|  #     |
 0x20, //   2|  #     |
 0x20, //   3|  #     |
 0x60, //   4| ##     |

//     5 ' ' |01234567|
 0x6A, //   0| ## # # |
 0x75, //   1| ### # #|
 0x60, //   2| ##     |
 0x6A, //   3| ## # # |
 0x75, //   4| ### # #|

//     6 ' ' |01234567|
 0x00, //   0|        |
 0x7F, //   1| #######|
 0x3E, //   2|  ##### |
 0x1C, //   3|   ###  |
 0x08, //   4|    #   |

//     7 ' ' |01234567|
 0x44, //   0| #   #  |
 0x4E, //   1| #  ### |
 0x55, //   2| # # # #|
 0x44, //   3| #   #  |
 0x3C, //   4|  ####  |

//     8 ' ' |01234567|
 0x55, //   0| # # # #|
 0x00, //   1|        |
 0x41, //   2| #     #|
 0x00, //   3|        |
 0x55, //   4| # # # #|

//     9 ' ' |01234567|
 0x64, //   0| ##  #  |
 0x2C, //   1|  # ##  |
 0x10, //   2|   #    |
 0x68, //   3| ## #   |
 0x4C, //   4| #  ##  |

//    10 ' ' |01234567|
 0x28, //   0|  # #   |
 0x44, //   1| #   #  |
 0x00, //   2|        |
 0x44, //   3| #   #  |
 0x28, //   4|  # #   |

//    11 ' ' |01234567|
 0x00, //   0|        |
 0x48, //   1| #  #   |
 0x7C, //   2| #####  |
 0x40, //   3| #      |
 0x00, //   4|        |

//    12 ' ' |01234567|
 0x00, //   0|        |
 0x64, //   1| ##  #  |
 0x54, //   2| # # #  |
 0x48, //   3| #  #   |
 0x00, //   4|        |

//    32 ' ' |01234567|
 0x00, //   0|        |
 0x00, //   1|        |
 0x00, //   2|        |
 0x00, //   3|        |
 0x00, //   4|        |

//    33 '!' |01234567|
 0x00, //   0|        |
 0x00, //   1|        |
 0x5F, //   2| # #####|
 0x00, //   3|        |
 0x00, //   4|        |

//    34 '"' |01234567|
 0x00, //   0|        |
 0x07, //   1|     ###|
 0x00, //   2|        |
 0x07, //   3|     ###|
 0x00, //   4|        |

//    35 '#' |01234567|
 0x14, //   0|   # #  |
 0x7F, //   1| #######|
 0x14, //   2|   # #  |
 0x7F, //   3| #######|
 0x14, //   4|   # #  |

//    36 '$' |01234567|
 0x24, //   0|  #  #  |
 0x2A, //   1|  # # # |
 0x7F, //   2| #######|
 0x2A, //   3|  # # # |
 0x12, //   4|   #  # |

//    37 '%' |01234567|
 0x23, //   0|  #   ##|
 0x13, //   1|   #  ##|
 0x08, //   2|    #   |
 0x64, //   3| ##  #  |
 0x62, //   4| ##   # |

//    38 '&' |01234567|
 0x36, //   0|  ## ## |
 0x49, //   1| #  #  #|
 0x55, //   2| # # # #|
 0x22, //   3|  #   # |
 0x50, //   4| # #    |

//    39 ''' |01234567|
 0x00, //   0|        |
 0x05, //   1|     # #|
 0x03, //   2|      ##|
 0x00, //   3|        |
 0x00, //   4|        |

//    40 '(' |01234567|
 0x00, //   0|        |
 0x1C, //   1|   ###  |
 0x22, //   2|  #   # |
 0x41, //   3| #     #|
 0x00, //   4|        |

//    41 ')' |01234567|
 0x00, //   0|        |
 0x41, //   1| #     #|
 0x22, //   2|  #   # |
 0x1C, //   3|   ###  |
 0x00, //   4|        |

//    42 '*' |01234567|
 0x14, //   0|   # #  |
 0x08, //   1|    #   |
 0x3E, //   2|  ##### |
 0x08, //   3|    #   |
 0x14, //   4|   # #  |

//    43 '+' |01234567|
 0x08, //   0|    #   |
 0x08, //   1|    #   |
 0x3E, //   2|  ##### |
 0x08, //   3|    #   |
 0x08, //   4|    #   |

//    44 ',' |01234567|
 0x00, //   0|        |
 0x50, //   1| # #    |
 0x30, //   2|  ##    |
 0x00, //   3|        |
 0x00, //   4|        |

//    45 '-' |01234567|
 0x08, //   0|    #   |
 0x08, //   1|    #   |
 0x08, //   2|    #   |
 0x08, //   3|    #   |
 0x08, //   4|    #   |

//    46 '.' |01234567|
 0x00, //   0|        |
 0x60, //   1| ##     |
 0x60, //   2| ##     |
 0x00, //   3|        |
 0x00, //   4|        |

//    47 '/' |01234567|
 0x20, //   0|  #     |
 0x10, //   1|   #    |
 0x08, //   2|    #   |
 0x04, //   3|     #  |
 0x02, //   4|      # |

//    48 '0' |01234567|
 0x3E, //   0|  ##### |
 0x51, //   1| # #   #|
 0x49, //   2| #  #  #|
 0x45, //   3| #   # #|
 0x3E, //   4|  ##### |

//    49 '1' |01234567|
 0x00, //   0|        |
 0x42, //   1| #    # |
 0x7F, //   2| #######|
 0x40, //   3| #      |
 0x00, //   4|        |

//    50 '2' |01234567|
 0x42, //   0| #    # |
 0x61, //   1| ##    #|
 0x51, //   2| # #   #|
 0x49, //   3| #  #  #|
 0x46, //   4| #   ## |

//    51 '3' |01234567|
 0x21, //   0|  #    #|
 0x41, //   1| #     #|
 0x45, //   2| #   # #|
 0x4B, //   3| #  # ##|
 0x31, //   4|  ##   #|

//    52 '4' |01234567|
 0x18, //   0|   ##   |
 0x14, //   1|   # #  |
 0x12, //   2|   #  # |
 0x7F, //   3| #######|
 0x10, //   4|   #    |

//    53 '5' |01234567|
 0x27, //   0|  #  ###|
 0x45, //   1| #   # #|
 0x45, //   2| #   # #|
 0x45, //   3| #   # #|
 0x39, //   4|  ###  #|

//    54 '6' |01234567|
 0x3C, //   0|  ####  |
 0x4A, //   1| #  # # |
 0x49, //   2| #  #  #|
 0x49, //   3| #  #  #|
 0x30, //   4|  ##    |

//    55 '7' |01234567|
 0x01, //   0|       #|
 0x71, //   1| ###   #|
 0x09, //   2|    #  #|
 0x05, //   3|     # #|
 0x03, //   4|      ##|

//    56 '8' |01234567|
 0x36, //   0|  ## ## |
 0x49, //   1| #  #  #|
 0x49, //   2| #  #  #|
 0x49, //   3| #  #  #|
 0x36, //   4|  ## ## |

//    57 '9' |01234567|
 0x06, //   0|     ## |
 0x49, //   1| #  #  #|
 0x49, //   2| #  #  #|
 0x29, //   3|  # #  #|
 0x1E, //   4|   #### |

//    58 ':' |01234567|
 0x00, //   0|        |
 0x66, //   1| ##  ## |
 0x66, //   2| ##  ## |
 0x00, //   3|        |
 0x00, //   4|        |

//    59 ';' |01234567|
 0x00, //   0|        |
 0x56, //   1| # # ## |
 0x36, //   2|  ## ## |
 0x00, //   3|        |
 0x00, //   4|        |

//    60 '<' |01234567|
 0x08, //   0|    #   |
 0x14, //   1|   # #  |
 0x22, //   2|  #   # |
 0x41, //   3| #     #|
 0x00, //   4|        |

//    61 '=' |01234567|
 0x14, //   0|   # #  |
 0x14, //   1|   # #  |
 0x14, //   2|   # #  |
 0x14, //   3|   # #  |
 0x14, //   4|   # #  |

//    62 '>' |01234567|
 0x00, //   0|        |
 0x41, //   1| #     #|
 0x22, //   2|  #   # |
 0x14, //   3|   # #  |
 0x08, //   4|    #   |

//    63 '?' |01234567|
 0x02, //   0|      # |
 0x01, //   1|       #|
 0x51, //   2| # #   #|
 0x09, //   3|    #  #|
 0x06, //   4|     ## |

//    64 '@' |01234567|
 0x32, //   0|  ##  # |
 0x49, //   1| #  #  #|
 0x79, //   2| ####  #|
 0x41, //   3| #     #|
 0x3E, //   4|  ##### |

//    65 'A' |01234567|
 0x7E, //   0| ###### |
 0x09, //   1|    #  #|
 0x09, //   2|    #  #|
 0x09, //   3|    #  #|
 0x7E, //   4| ###### |

//    66 'B' |01234567|
 0x7F, //   0| #######|
 0x49, //   1| #  #  #|
 0x49, //   2| #  #  #|
 0x49, //   3| #  #  #|
 0x36, //   4|  ## ## |

//    67 'C' |01234567|
 0x3E, //   0|  ##### |
 0x41, //   1| #     #|
 0x41, //   2| #     #|
 0x41, //   3| #     #|
 0x22, //   4|  #   # |

//    68 'D' |01234567|
 0x7F, //   0| #######|
 0x41, //   1| #     #|
 0x41, //   2| #     #|
 0x41, //   3| #     #|
 0x3E, //   4|  ##### |

//    69 'E' |01234567|
 0x7F, //   0| #######|
 0x49, //   1| #  #  #|
 0x49, //   2| #  #  #|
 0x49, //   3| #  #  #|
 0x41, //   4| #     #|

//    70 'F' |01234567|
 0x7F, //   0| #######|
 0x09, //   1|    #  #|
 0x09, //   2|    #  #|
 0x09, //   3|    #  #|
 0x01, //   4|       #|

//    71 'G' |01234567|
 0x3E, //   0|  ##### |
 0x41, //   1| #     #|
 0x49, //   2| #  #  #|
 0x49, //   3| #  #  #|
 0x7A, //   4| #### # |

//    72 'H' |01234567|
 0x7F, //   0| #######|
 0x08, //   1|    #   |
 0x08, //   2|    #   |
 0x08, //   3|    #   |
 0x7F, //   4| #######|

//    73 'I' |01234567|
 0x00, //   0|        |
 0x41, //   1| #     #|
 0x7F, //   2| #######|
 0x41, //   3| #     #|
 0x00, //   4|        |

//    74 'J' |01234567|
 0x20, //   0|  #     |
 0x40, //   1| #      |
 0x41, //   2| #     #|
 0x3F, //   3|  ######|
 0x01, //   4|       #|

//    75 'K' |01234567|
 0x7F, //   0| #######|
 0x08, //   1|    #   |
 0x14, //   2|   # #  |
 0x22, //   3|  #   # |
 0x41, //   4| #     #|

//    76 'L' |01234567|
 0x7F, //   0| #######|
 0x40, //   1| #      |
 0x40, //   2| #      |
 0x40, //   3| #      |
 0x40, //   4| #      |

//    77 'M' |01234567|
 0x7F, //   0| #######|
 0x02, //   1|      # |
 0x04, //   2|     #  |
 0x02, //   3|      # |
 0x7F, //   4| #######|

//    78 'N' |01234567|
 0x7F, //   0| #######|
 0x04, //   1|     #  |
 0x08, //   2|    #   |
 0x10, //   3|   #    |
 0x7F, //   4| #######|

//    79 'O' |01234567|
 0x3E, //   0|  ##### |
 0x41, //   1| #     #|
 0x41, //   2| #     #|
 0x41, //   3| #     #|
 0x3E, //   4|  ##### |

//    80 'P' |01234567|
 0x7F, //   0| #######|
 0x09, //   1|    #  #|
 0x09, //   2|    #  #|
 0x09, //   3|    #  #|
 0x06, //   4|     ## |

//    81 'Q' |01234567|
 0x3E, //   0|  ##### |
 0x41, //   1| #     #|
 0x51, //   2| # #   #|
 0x21, //   3|  #    #|
 0x5E, //   4| # #### |

//    82 'R' |01234567|
 0x7F, //   0| #######|
 0x09, //   1|    #  #|
 0x19, //   2|   ##  #|
 0x29, //   3|  # #  #|
 0x46, //   4| #   ## |

//    83 'S' |01234567|
 0x26, //   0|  #  ## |
 0x49, //   1| #  #  #|
 0x49, //   2| #  #  #|
 0x49, //   3| #  #  #|
 0x32, //   4|  ##  # |

//    84 'T' |01234567|
 0x01, //   0|       #|
 0x01, //   1|       #|
 0x7F, //   2| #######|
 0x01, //   3|       #|
 0x01, //   4|       #|

//    85 'U' |01234567|
 0x3F, //   0|  ######|
 0x40, //   1| #      |
 0x40, //   2| #      |
 0x40, //   3| #      |
 0x3F, //   4|  ######|

//    86 'V' |01234567|
 0x1F, //   0|   #####|
 0x20, //   1|  #     |
 0x40, //   2| #      |
 0x20, //   3|  #     |
 0x1F, //   4|   #####|

//    87 'W' |01234567|
 0x3F, //   0|  ######|
 0x40, //   1| #      |
 0x30, //   2|  ##    |
 0x40, //   3| #      |
 0x3F, //   4|  ######|

//    88 'X' |01234567|
 0x63, //   0| ##   ##|
 0x14, //   1|   # #  |
 0x08, //   2|    #   |
 0x14, //   3|   # #  |
 0x63, //   4| ##   ##|

//    89 'Y' |01234567|
 0x07, //   0|     ###|
 0x08, //   1|    #   |
 0x70, //   2| ###    |
 0x08, //   3|    #   |
 0x07, //   4|     ###|

//    90 'Z' |01234567|
 0x61, //   0| ##    #|
 0x51, //   1| # #   #|
 0x49, //   2| #  #  #|
 0x45, //   3| #   # #|
 0x43, //   4| #    ##|

//    91 '[' |01234567|
 0x00, //   0|        |
 0x7F, //   1| #######|
 0x41, //   2| #     #|
 0x41, //   3| #     #|
 0x00, //   4|        |

//    92 '\' |01234567|
 0x01, //   0|       #|
 0x06, //   1|     ## |
 0x08, //   2|    #   |
 0x30, //   3|  ##    |
 0x40, //   4| #      |

//    93 ']' |01234567|
 0x00, //   0|        |
 0x41, //   1| #     #|
 0x41, //   2| #     #|
 0x7F, //   3| #######|
 0x00, //   4|        |

//    94 '^' |01234567|
 0x04, //   0|     #  |
 0x02, //   1|      # |
 0x01, //   2|       #|
 0x02, //   3|      # |
 0x04, //   4|     #  |

//    95 '_' |01234567|
 0x00, //   0|        |
 0x00, //   1|        |
 0x00, //   2|        |
 0x00, //   3|        |
 0x00, //   4|        |

//    96 '`' |01234567|
 0x00, //   0|        |
 0x01, //   1|       #|
 0x02, //   2|      # |
 0x04, //   3|     #  |
 0x00, //   4|        |

//    97 'a' |01234567|
 0x20, //   0|  #     |
 0x54, //   1| # # #  |
 0x54, //   2| # # #  |
 0x54, //   3| # # #  |
 0x78, //   4| ####   |

//    98 'b' |01234567|
 0x7F, //   0| #######|
 0x48, //   1| #  #   |
 0x44, //   2| #   #  |
 0x44, //   3| #   #  |
 0x38, //   4|  ###   |

//    99 'c' |01234567|
 0x38, //   0|  ###   |
 0x44, //   1| #   #  |
 0x44, //   2| #   #  |
 0x44, //   3| #   #  |
 0x20, //   4|  #     |

//   100 'd' |01234567|
 0x38, //   0|  ###   |
 0x44, //   1| #   #  |
 0x44, //   2| #   #  |
 0x48, //   3| #  #   |
 0x7F, //   4| #######|

//   101 'e' |01234567|
 0x38, //   0|  ###   |
 0x54, //   1| # # #  |
 0x54, //   2| # # #  |
 0x54, //   3| # # #  |
 0x18, //   4|   ##   |

//   102 'f' |01234567|
 0x08, //   0|    #   |
 0x7E, //   1| ###### |
 0x09, //   2|    #  #|
 0x01, //   3|       #|
 0x02, //   4|      # |

//   103 'g' |01234567|
 0x0C, //   0|    ##  |
 0x52, //   1| # #  # |
 0x52, //   2| # #  # |
 0x52, //   3| # #  # |
 0x3E, //   4|  ##### |

//   104 'h' |01234567|
 0x7F, //   0| #######|
 0x08, //   1|    #   |
 0x04, //   2|     #  |
 0x04, //   3|     #  |
 0x78, //   4| ####   |

//   105 'i' |01234567|
 0x00, //   0|        |
 0x44, //   1| #   #  |
 0x7D, //   2| ##### #|
 0x40, //   3| #      |
 0x00, //   4|        |

//   106 'j' |01234567|
 0x00, //   0|        |
 0x20, //   1|  #     |
 0x40, //   2| #      |
 0x44, //   3| #   #  |
 0x3D, //   4|  #### #|

//   107 'k' |01234567|
 0x7F, //   0| #######|
 0x10, //   1|   #    |
 0x28, //   2|  # #   |
 0x44, //   3| #   #  |
 0x00, //   4|        |

//   108 'l' |01234567|
 0x00, //   0|        |
 0x41, //   1| #     #|
 0x7F, //   2| #######|
 0x40, //   3| #      |
 0x00, //   4|        |

//   109 'm' |01234567|
 0x7C, //   0| #####  |
 0x04, //   1|     #  |
 0x78, //   2| ####   |
 0x04, //   3|     #  |
 0x78, //   4| ####   |

//   110 'n' |01234567|
 0x7C, //   0| #####  |
 0x08, //   1|    #   |
 0x04, //   2|     #  |
 0x04, //   3|     #  |
 0x78, //   4| ####   |

//   111 'o' |01234567|
 0x38, //   0|  ###   |
 0x44, //   1| #   #  |
 0x44, //   2| #   #  |
 0x44, //   3| #   #  |
 0x38, //   4|  ###   |

//   112 'p' |01234567|
 0x7C, //   0| #####  |
 0x14, //   1|   # #  |
 0x14, //   2|   # #  |
 0x14, //   3|   # #  |
 0x08, //   4|    #   |

//   113 'q' |01234567|
 0x0C, //   0|    ##  |
 0x14, //   1|   # #  |
 0x14, //   2|   # #  |
 0x18, //   3|   ##   |
 0x7C, //   4| #####  |

//   114 'r' |01234567|
 0x7C, //   0| #####  |
 0x08, //   1|    #   |
 0x04, //   2|     #  |
 0x04, //   3|     #  |
 0x08, //   4|    #   |

//   115 's' |01234567|
 0x48, //   0| #  #   |
 0x54, //   1| # # #  |
 0x54, //   2| # # #  |
 0x54, //   3| # # #  |
 0x24, //   4|  #  #  |

//   116 't' |01234567|
 0x04, //   0|     #  |
 0x3F, //   1|  ######|
 0x44, //   2| #   #  |
 0x40, //   3| #      |
 0x20, //   4|  #     |

//   117 'u' |01234567|
 0x3C, //   0|  ####  |
 0x40, //   1| #      |
 0x40, //   2| #      |
 0x20, //   3|  #     |
 0x7C, //   4| #####  |

//   118 'v' |01234567|
 0x1C, //   0|   ###  |
 0x20, //   1|  #     |
 0x40, //   2| #      |
 0x20, //   3|  #     |
 0x1C, //   4|   ###  |

//   119 'w' |01234567|
 0x3C, //   0|  ####  |
 0x40, //   1| #      |
 0x30, //   2|  ##    |
 0x40, //   3| #      |
 0x3C, //   4|  ####  |

//   120 'x' |01234567|
 0x44, //   0| #   #  |
 0x28, //   1|  # #   |
 0x10, //   2|   #    |
 0x28, //   3|  # #   |
 0x44, //   4| #   #  |

//   121 'y' |01234567|
 0x0C, //   0|    ##  |
 0x50, //   1| # #    |
 0x50, //   2| # #    |
 0x50, //   3| # #    |
 0x3C, //   4|  ####  |

//   122 'z' |01234567|
 0x44, //   0| #   #  |
 0x64, //   1| ##  #  |
 0x54, //   2| # # #  |
 0x4C, //   3| #  ##  |
 0x44, //   4| #   #  |

//   123 '{' |01234567|
 0x00, //   0|        |
 0x08, //   1|    #   |
 0x36, //   2|  ## ## |
 0x41, //   3| #     #|
 0x00, //   4|        |

//   124 '|' |01234567|
 0x00, //   0|        |
 0x00, //   1|        |
 0xFF, //   2|########|
 0x00, //   3|        |
 0x00, //   4|        |

//   125 '}' |01234567|
 0x00, //   0|        |
 0x41, //   1| #     #|
 0x36, //   2|  ## ## |
 0x08, //   3|    #   |
 0x00, //   4|        |

//   126 '~' |01234567|
 0x08, //   0|    #   |
 0x08, //   1|    #   |
 0x2A, //   2|  # # # |
 0x1C, //   3|   ###  |
 0x08, //   4|    #   |

//   127 ' ' |01234567|
 0x08, //   0|    #   |
 0x1C, //   1|   ###  |
 0x2A, //   2|  # # # |
 0x08, //   3|    #   |
 0x08, //   4|    #   |

};

#endif