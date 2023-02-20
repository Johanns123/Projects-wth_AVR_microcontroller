//===================================//
/*Definicao de bibliotecas*/
#include "keyboard.hpp"

//===================================//
/*Definicao da classe*/
keyboard::keyboard(volatile uint8_t *dir, volatile uint8_t *port, volatile uint8_t *reg_read, uint8_t pin, uint8_t pull_up_pin)
{
    pin_ = pin;
    out_ = port;
    dir_ = dir;
    reg_read_ = reg_read;
    *dir_  &=  ~(1 << pin_);
    if(pull_up_pin)
        *out_  |= (1 << pin_);
    else
        *out_  &= ~(1 << pin_);
}

uint8_t keyboard::read_key()
{
    if(*reg_read_ & (1<<pin_))
        return 1;

    else
        return 0;
}

uint8_t keyboard::read_key_falling()
{
    static uint8_t memory_button = 1, button = 0, output = 0;

    button = keyboard::read_key();

    if(button < memory_button)
        output = 1;

    else
        output =  0;

    memory_button = button;
    return output;
}