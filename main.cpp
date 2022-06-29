#include "BMP280.hpp"




int main(){
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);
    hwlib::i2c_bus_bit_banged_scl_sda bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);

    BMP280 bmp280(bus);

    for (;;) {
        bmp280.warmte();
        bmp280.press();
        bmp280.regenkans();
        bmp280.print();
        hwlib::wait_ms( 1000 );
    }

}
