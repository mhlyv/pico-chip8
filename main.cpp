#include <iostream>
#include "pico/stdlib.h"

int main()
{
    stdio_init_all();
    constexpr unsigned int led_pin = PICO_DEFAULT_LED_PIN;

    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    for (;;)
    {
        std::cout << "ON\n";
        gpio_put(led_pin, 1);
        sleep_ms(500);

        std::cout << "OFF\n";
        gpio_put(led_pin, 0);
        sleep_ms(500);
    }
}
