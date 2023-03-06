#include <avr/io.h>
#include <util/delay.h>

#define SR_DAT   PB0
#define SR_LATCH PB1
#define SR_CLK   PB2

void setup(void);
void loop(void);
void shift_reg(uint8_t word);
void latch(void);

int main(void)
{
  setup();
  while(1)  loop();
}

void setup() {
  DDRB  |= (1 << SR_DAT) | (1 << SR_CLK) | (1 << SR_LATCH);
  PORTB &= ~((1 << SR_DAT) | (1 << SR_CLK) | (1 << SR_LATCH));
}

void loop() {
  static uint16_t contador = 0;
  if(contador < 256)
  {
    shift_reg(contador);
    latch();
    _delay_ms(10);
    shift_reg(0);
    _delay_ms(10);  
  }

  else
  {
    shift_reg(contador>>8);
    latch();
    _delay_ms(10);
    shift_reg(contador);
    _delay_ms(10); 
  }
  contador++;
}

void shift_reg(uint8_t word)
{
  for(int i = 7; i >= 0; i--)
  {
    if(word & (1<<i))
    {
      PORTB |= (1 << SR_DAT);
    }
    else
    {
      PORTB &= ~(1 << SR_DAT);
    }
    //_delay_ms(1);
    PORTB |= (1<<SR_CLK);
    //_delay_ms(1);
    PORTB &= ~(1<<SR_CLK);
    //_delay_ms(1);
  }
}

void latch(void)
{
  PORTB |= (1<< SR_LATCH);
  PORTB &= ~(1 << SR_LATCH);
}