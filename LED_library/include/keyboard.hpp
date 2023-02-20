//===================================//
/*Definicao de bibliotecas*/
#ifndef KEYBOARD
#define KEYBOARD

#include <avr/io.h>
#include <util/delay.h>

#define PULL_UP_ENABLE  1
#define PULL_UP_DISABLE  0

//===================================//
/*Definicao da classe*/

class keyboard
{
    public:
        keyboard(volatile uint8_t *dir, volatile uint8_t *port, volatile uint8_t *reg_read, uint8_t pin, uint8_t pull_up_pin);

        uint8_t read_key(void);
        uint8_t read_key_falling(void);

    private:
        uint8_t pin_;
        volatile uint8_t *out_, *dir_, *reg_read_;
};
#endif