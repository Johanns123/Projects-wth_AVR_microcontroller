#include "myLED.hpp"
#include "keyboard.hpp"

void setup(void);
void loop(void);

keyboard button1(&DDRD, &PORTD, &PIND, PD0, PULL_UP_ENABLE);
my_LED LED1(&DDRD, &PORTD, PD2);
my_LED LED2(&DDRB, &PORTB, PB0);

int main()
{
  setup();
  while(1)  loop();
  return 0;
}


void setup() 
{
}

void loop() 
{
  static uint8_t bt1 = 0;

  bt1 = button1.read_key_falling();
  if(bt1)
    LED1.led_toggle();

}