// ====================================================================================
//
//       Author  :   Bas van den Bergh
//       Filename:   main.cpp
//       Part of :   IPASS
//
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// =====================================================================================
#include "BMP280.hpp"


void print(double temp, double pressure, hwlib::i2c_bus & bus){
    hwlib::wait_ms( 1000 );
    auto oled    = hwlib::glcd_oled(bus, 0x3c );
    auto font    = hwlib::font_default_8x8();
    auto display = hwlib::terminal_from( oled, font );
    display
            << "\f" << "BMP_280"
            << "\n" << "================="
            << "\n"
            << "\n" << "Temprature: " << int(temp) << " C"
            << "\n"
            << "\n" << "================="
            << "\n"
            << "\n" << "Pressure:" << int(pressure) << " P"
            << hwlib::flush;
    hwlib::cout << "De tempratuur is: " << int(temp) << hwlib::endl;
    hwlib::cout << "De Luchtdruk is: " << int(pressure) << hwlib::endl;
}

int main(){
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);
    hwlib::i2c_bus_bit_banged_scl_sda bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);

    BMP280 bmp280(bus);


    for (;;) {
        double temp = bmp280.calculateTemp();
        double pressure = bmp280.calculatePressure();
        print(temp, pressure, bus);
        hwlib::wait_ms( 1000 );
    }

}
