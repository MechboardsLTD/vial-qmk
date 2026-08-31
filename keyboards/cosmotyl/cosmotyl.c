#include "quantum.h"

void keyboard_pre_init_kb(void)
{
    gpio_set_pin_output(GP11);
    gpio_write_pin_high(GP11);
    keyboard_pre_init_user();
}
