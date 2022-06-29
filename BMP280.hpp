// ====================================================================================
//
//       Author  :   Bas van den Bergh
//       Filename:   BMP280.hpp
//       Part of :   IPASS
//
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// =====================================================================================
#include "hwlib.hpp"

#ifndef BMP280_HPP
#define BMP280_HPP

class BMP280{
private:
    double var1;
    double var2;
    double var3;
    double var4;
    double T_fine;
    double p;
    uint16_t dig_T1;
    int16_t dig_T2 ;
    int16_t dig_T3 ;
    uint16_t dig_P1;
    int16_t dig_P2 ;
    int16_t dig_P3 ;
    int16_t dig_P4 ;
    int16_t dig_P5 ;
    int16_t dig_P6 ;
    int16_t dig_P7 ;
    int16_t dig_P8 ;
    int16_t dig_P9 ;
    hwlib::i2c_bus& bus;
    int regenprocent;
    static const uint_fast8_t address = 0x77;
    static const uint_fast8_t control = 0xF4;
    static const uint8_t modes = 0b1010011;
    static  const uint_fast8_t filter = 0xF5;
    uint8_t resultstemp[3];
    uint8_t digtemp[6];
    uint32_t totaaltemp = 0x00;
    uint8_t resultspress[3];
    uint8_t digpress[18];
    uint32_t totaalpress = 0x00;

    /// \brief
   /// Cunstuctor
   /// \details
   /// In onze contructor geven we de bus door.
   /// En we schrijven de register ardesses in een array genaamd digtemp voor temprature en digpress voor de pressure.
   /// Hier mee daan we 16 bit variable maken waar mee we later gaan rekenen.
   /// Deze waardes zijn signed en unsigned.
public:
    BMP280(hwlib::i2c_bus & bus);
    /// \brief
    /// Standaard schrijft de control, modes en de filter.
    /// \details
    /// We schrijven de control, modes en de filter een keer uit voor communicatie met de chip.
    void standaard();

    /// \brief
   /// write
    /// \details
    /// write schrijft de schrijft alle adressen die je wilt in een array aan de hand van een hext decimaal.
    void write(int content);
    /// \brief
    /// read
    /// \details
    /// read leest alle array's die jij aan geeft uit aan de hand van de naam en grote
    void read(uint8_t* array, int array_size);
    /// \brief
    /// Tempratur meten in void warmte.
    /// \details
    /// We schrijven de  msb, lsb ,xlsb in een array door write aan te roepen.
    /// We lezen de array uit aan de hand van de read met 2 parameters de array en de array grote.
    /// Daarna maken we een 20 bit waarde door ze aan elkaar te plakken en op te schuiven.
    /// Met de 20 bit waarde en de register adresses kunnen we gaan rekenen.
    /// De daadwerkelijke tempratuur komt er uit als variable T_fine.
    void warmte();
    /// \brief
    /// Pressure meten in void press.
    /// \details
    /// We schrijven de  msb, lsb ,xlsb in een array door write aan te roepen.
    /// We lezen de array uit aan de hand van de read met 2 parameters de array en de array grote.
    /// Daarna maken we een 20 bit waarde door ze aan elkaar te plakken en op te schuiven.
    /// Met de 20 bit waarde en de register adresses en T_fine van void warmte kunnen we gaan rekenen.
    /// De daadwerkelijke pressure komt eruit als variable p.
    void press();
    /// \brief
    /// Het uit lezen van de pressure en temprature en deze waardes printen in void Print.
    /// \details
    /// We defineren eerst oled met de bus en zijn adress.
    /// Daarna defineren we de font met 8x8.
    /// En als laatste defineren we display met 2 parameters oled en font.
    /// Hierna schrijven we de dingen die we willen laten zien op de display.
    /// Die flushen we naar de oled en word het geprint.
    void print();
};

#endif // BMP280_HPP
