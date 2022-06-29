// ====================================================================================
//
//       Author  :   Bas van den Bergh
//       Filename:   BMP280.cpp
//       Part of :   IPASS
//
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// =====================================================================================
#include "BMP280.hpp"


BMP280::BMP280(hwlib::i2c_bus & bus) :
bus( bus ) {
    standaard();
    write(0x88);
    read(digtemp, 6);
    write(0x8E);
    read(digpress, 18);
    dig_T1 = digtemp[0] | (digtemp[1] << 8);
    dig_T2 = (int16_t)(digtemp[2] | (digtemp[3] << 8));
    dig_T3 = (int16_t)(digtemp[4] | (digtemp[5] << 8));
    dig_P1 = digpress[0] | (digpress[1] << 8);
    dig_P2 = (int16_t)(digpress[2] | (digpress[3] << 8));
    dig_P3 = (int16_t)(digpress[4] | (digpress[5] << 8));
    dig_P4 = (int16_t)(digpress[6] | (digpress[7] << 8));
    dig_P5 = (int16_t)(digpress[8] | (digpress[9] << 8));
    dig_P6 = (int16_t)(digpress[10] | (digpress[11] << 8));
    dig_P7 = (int16_t)(digpress[12] | (digpress[13] << 8));
    dig_P8 = (int16_t)(digpress[14] | (digpress[15] << 8));
    dig_P9 = (int16_t)(digpress[16] | (digpress[17] << 8));

}
void BMP280::standaard(){
    hwlib::i2c_write_transaction wtrans = ((hwlib::i2c_bus*)(&bus))->write(address);
    wtrans.write(control);
    wtrans.write(modes);
    wtrans.write(filter);
}

void BMP280::write(int content) {
    hwlib::i2c_write_transaction wtrans = ((hwlib::i2c_bus*)(&bus))->write(address);
    wtrans.write(content);
}

void BMP280::read(uint8_t *array, int array_size){
    hwlib::i2c_read_transaction rtrans = ((hwlib::i2c_bus*)(&bus))->read(address);
    rtrans.read(array, array_size);
}


    void BMP280::warmte(){

            write(0xFA);
            read(resultstemp, 3);
            totaaltemp = 0;
            uint32_t newresulttemp = resultstemp[2] >> 4;
            totaaltemp = (resultstemp[0] | totaaltemp) << 8;
            totaaltemp = (totaaltemp | resultstemp[1]) << 4;
            totaaltemp = totaaltemp | newresulttemp;

            var1 = (((double) totaaltemp) / 16384.0 - ((double) dig_T1) / 1024.0) * ((double) dig_T2);
            var2 = ((((double) dig_T1) / 131072.0 - ((double) dig_T1) / 8192) *
                    (((double) totaaltemp) / 131072.0 - ((double) dig_T1) / 8192.0)) * ((double) dig_T3);
            T_fine = (var1 + var2) / 5120.0;
    }

    void BMP280::press(){
            write(0xF7);
            read(resultspress, 3);
            totaalpress = 0;
            uint32_t newresultpressure = resultspress[2] >> 4;
            totaalpress = (resultspress[0] | totaalpress) << 8;
            totaalpress = (totaalpress | resultspress[1]) << 4;
            totaalpress = totaalpress | newresultpressure;


        var3 = ((double)T_fine/2.0) - 64000.0;

        var4 = var3 * var3 * ((double)dig_P6) / 32768.0;
        var4 = var4 + var3 * ((double)dig_P5) *2.0;
        var4 = (var4/4.0) + (((double)dig_P4) * 65536.0);

        var3 = (((double)dig_P3) * var3 * var3 / 524288.0 + ((double)dig_P2) * var3) / 524288.0;
        var3 = (1.0 + var3 / 32768.0)* ((double)dig_P1);
        if(var3 == 0 ){
            p = 0;
        }
        p = 1048576.0 - (double)totaalpress;
        p = (p - (var4 / 4096.0)) * 6250.0 / var3;
        var3 = ((double)dig_P9) * p * p / 2147483648.0;
        var4 = p * ((double)dig_P8) / 32768.0;
        p = p + (var3 + var4 + ((double) dig_P7)) / 16.0;


    }

    void BMP280::print(){

        hwlib::wait_ms( 1000 );
        auto oled    = hwlib::glcd_oled(bus, 0x3c );
        auto font    = hwlib::font_default_8x8();
        auto display = hwlib::terminal_from( oled, font );
        display
            << "\f" << "BMP_280"
            << "\n" << "================="
            << "\n"
            << "\n" << "Temprature: " << int(T_fine) << " C"
            << "\n"
            << "\n" << "================="
            << "\n"
            << "\n" << "Pressure:" << int(p) << " P"
            << hwlib::flush;
        hwlib::cout << "De tempratuur is: " << int(T_fine) << hwlib::endl;
        hwlib::cout << "De Luchtdruk is: " << int(p) << hwlib::endl;
    }

