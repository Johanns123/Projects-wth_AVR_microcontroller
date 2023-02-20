#include "liquidCrystal.hpp"
void setup(void);
void loop(void);

liquidCrystal LCD(&PORTD, &DDRD, PD2, PD3, PD4, PD5, PD6, PD7);

int main()
{
  setup();
  while(1)  loop();
  return 0;
}


void setup() 
{
  LCD.disp_init();
  LCD.disp_text("IHM com classe!", 0, 0);
}

void loop() 
{
  
}