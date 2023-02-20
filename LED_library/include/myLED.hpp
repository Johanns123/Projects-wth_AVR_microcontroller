//===================================//
/*Definicao de bibliotecas*/
#ifndef MYLED
#define MYLED

#include <avr/io.h>
#include <util/delay.h>

//===================================//
/*Definicao da classe*/

class my_LED
{
    public:
        my_LED(volatile uint8_t *dir, volatile uint8_t *out, uint8_t pin);

        void set_high(uint16_t t_on);
        void set_low(uint16_t t_off);
        void set_duty(uint16_t time_ms, uint16_t duty);
        void blynk_led(void);
        void led_state_and_time(uint8_t state, uint16_t time_ms);
        void led_state(uint8_t state);
        void led_toggle(void);
        void led_toggle_ms(uint16_t time_ms);

    private:
        uint8_t pin_;
        uint16_t t_on_, t_off_, duty_;
        volatile uint8_t *out_, *dir_;
};
#endif