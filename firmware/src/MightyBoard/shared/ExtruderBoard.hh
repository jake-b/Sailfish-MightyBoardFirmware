/*
 * Copyright 2010 by Adam Mayer	 <adam@makerbot.com>
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

#ifndef MIGHTYBOARD_EXTRUDER_HH_
#define MIGHTYBOARD_EXTRUDER_HH_

#include "Configuration.hh"
#include "Thermocouple.hh"
#include "HeatingElement.hh"
#include "Heater.hh"
#include "EepromMap.hh"
#include "CoolingFan.hh"

#if defined(HAS_VIKI_INTERFACE)
#include "VikiInterface.hh"  //Needed to #define INDICATOR capabilities
#elif defined(HAS_VIKI2_INTERFACE)
#include "Viki2Interface.hh" //Needed to #define INDICATOR capabilities
#endif

#if defined(USE_THERMOCOUPLE_DUAL)
#define THERMOCOUPLE_CLASS		DualThermocouple
#define THERMOCOUPLE_TYPE		uint8_t
#include "ThermocoupleDual.hh"
#else
#define THERMOCOUPLE_CLASS		Thermocouple
#define THERMOCOUPLE_TYPE		Pin
#endif

class ExtruderHeatingElement : public HeatingElement {

public:
     ExtruderHeatingElement(uint8_t id);
     void setHeatingElement(uint8_t value);
     uint8_t heater_id;
#if defined(HAS_VIKI_INTERFACE) || defined(HAS_VIKI2_INTERFACE)
     bool oldLEDstate;
#endif
};

class ExtruderBoard {

public:
     /**
      * @param slave_id_in
      * @param HeaterPin_In
      * @param FanPin_In
      * @param thermocouple_channel
      * @param eeprom_base Start of the chunk of EEPROM memory
      *        containing extruder settings
      * @return
      */
     ExtruderBoard(uint8_t slave_id_in, Pin HeaterPin_In, Pin FanPin_In,
		   THERMOCOUPLE_TYPE thermocouple_channel, uint16_t eeprom_base);
private:
     THERMOCOUPLE_CLASS extruder_thermocouple;
     ExtruderHeatingElement extruder_element;
     Heater extruder_heater;
     CoolingFan coolingFan;
     uint8_t slave_id;
     Pin Heater_Pin;
     uint8_t* eeprom_base;
     bool is_disabled;
#if (defined(HAS_VIKI_INTERFACE) || defined(HAS_VIKI2_INTERFACE)) && defined(HAS_TOOL_INDICATOR)
     uint8_t active_heaters;
#endif

public:
     void reset();

     void disable(bool state);

     void runExtruderSlice();

     void setFan(uint8_t on);

     Heater& getExtruderHeater() { return extruder_heater; }

     uint8_t getSlaveID() { return slave_id; }
};

#endif // MIGHTYBOARD_EXTRUDER_HH
