#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

int main() {
    stdio_usb_init();

    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    while (true) {
        uint16_t raw = adc_read();
        float voltage = raw * 3.3f / (1 << 12);
        float temperature = 27.0f - (voltage - 0.706f) / 0.001721f;
        printf("Temperature: %.2f C\n", temperature);
        sleep_ms(1000);
    }

    return 0;
}
