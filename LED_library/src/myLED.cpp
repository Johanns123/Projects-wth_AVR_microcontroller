//===================================//
/*Definicao de bibliotecas*/
#include "myLED.hpp"

//===================================//
/*Definicao da classe*/
my_LED::my_LED(volatile uint8_t *dir, volatile uint8_t *out, uint8_t pin)
{
    pin_ = pin;
    out_ = out;
    dir_ = dir;
    *dir_  |=  (1 << pin_);
    *out_  &= ~(1 << pin_);
}

void my_LED::set_high(uint16_t t_on)
{
    t_on_ = t_on;
}

void my_LED::set_low(uint16_t t_off)
{
    t_off_ = t_off;
}

void my_LED::set_duty(uint16_t time_ms, uint16_t duty)
{
    t_on_ = time_ms * ((float)duty/100.0);
    t_off_ = time_ms - t_on_;
}

void my_LED::blynk_led(void)
{
    *out_ |= (1 << pin_);
    for(uint16_t i = 0; i <  t_on_; i++)
        _delay_ms(1);

    *out_ &= ~(1 << pin_);
    for(uint16_t i = 0; i <  t_off_; i++)
        _delay_ms(1);
}

void my_LED::led_state_and_time(uint8_t state, uint16_t time_ms)
{
    if(state)
    {
       *out_ |= (1 << pin_);
    }

    else
    {
        *out_ &= ~(1 << pin_);
    }

    for(uint16_t i = 0; i <  time_ms; i++)
    _delay_ms(1); 
}
void my_LED::led_toggle(void)
{
    *out_ ^= (1 << pin_);
}

void my_LED::led_toggle_ms(uint16_t time_ms)
{
    *out_ ^= (1 << pin_);
    for(uint16_t i = 0; i < time_ms; i++)
        _delay_ms(1); 
}

void my_LED::led_state(uint8_t state)
{
    if(state)
    {
       *out_ |= (1 << pin_);
    }

    else
    {
        *out_ &= ~(1 << pin_);
    }
}