#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/interrupt.h>

#define SR_DAT   PB0
#define SR_LATCH PB1
#define SR_CLK   PB2

/* ========================================================================= */
/* --- Constantes --- */
#define    RS       0x01                       /* endereço do pino register select */
#define    EN       0x02                       /* endereço do pino de enable */
#define NUM_SHIFT_REGISTERS 2  // Quantidade de shift registers em cascata

enum OPERATOON_MODE
{
  BOTH, ONLY_FIRST, ONLY_SECOND
};

/* ========================================================================= */
/* --- Protótipo das Funções --- */
void lcdSER_write(char chr);                   /* função para enviear caracteres para o LCD */
void lcdSER_cmd(unsigned char cmd);            /* função para enviar comandos para o LCD */
void lcdSER_init();                            /* função para inicializar LCD no modo de 4 bits */
void lcdSER_clear();                           /* função para limpeza do LCD */
void setup(void);
void loop(void);
void shift_reg(uint8_t word);
void latch(void);
void shiftOutCascade(uint8_t data_second_shift, uint8_t data_first_shift, uint8_t mode) ;

//static uint8_t contador = 0;

ISR(TIMER0_OVF_vect)
{
  TCNT0 = 0x06;
  static uint16_t contador0 = 1, contador1 = 1, data = 0, i = 0;

  if(contador0 < 250) contador0++;

  else
  {
    contador0 = 1;
    data ^= 1;
    shiftOutCascade((data)<<i, 0, ONLY_SECOND);
  }

  if(contador1 < 500) contador1++;

  else
  {
    contador1 = 1;
    i++;
    if(i > 7) i = 0;
  }
}

int main(void)
{
  setup();
  while(1)  loop();
}

void setup() {
  DDRB  |= (1 << SR_DAT) | (1 << SR_CLK) | (1 << SR_LATCH);
  PORTB &= ~((1 << SR_DAT) | (1 << SR_CLK) | (1 << SR_LATCH));
  TCCR0B |= (1<<CS00) | (1<<CS01);
  TCNT0 = 0x06; //contagem de 1ms
  TIMSK0 = 1;
  sei(); 
  lcdSER_init();                               /* inicializa LCD */
  lcdSER_write('W');                           /* envia 'W' */
  lcdSER_write('R');                           /* envia 'R' */
  lcdSER_write(' ');                           /* envia ' ' */
  lcdSER_write('K');                           /* envia 'K' */
  lcdSER_write('i');                           /* envia 'i' */
  lcdSER_write('t');                           /* envia 't' */
  lcdSER_write('s');                           /* envia 's' */
  _delay_ms(500);                               /* aguarda */
  lcdSER_clear();                              /* limpa LCD */
}

void loop() {

  lcdSER_write('H');                         /* envia 'H' */
  _delay_ms(250);                             /* aguarda */
  lcdSER_write('e');                         /* envia 'e' */
  _delay_ms(250);                             /* aguarda */
  lcdSER_write('l');                         /* envia 'l' */
  _delay_ms(250);                             /* aguarda */
  lcdSER_write('l');                         /* envia 'l' */
  _delay_ms(250);                             /* aguarda */
  lcdSER_write('o');                         /* envia 'o' */
  _delay_ms(250);                             /* aguarda */
  lcdSER_write('!');                         /* envia '!' */
  _delay_ms(250);                             /* aguarda */
  lcdSER_clear();                            /* limpa LCD */
  _delay_ms(250);                             /* aguarda */
  
  //shiftOutCascade(contador, 0, ONLY_SECOND);
  //contador++;
}

/* ========================================================================= */
/* --- Escreve um caractere LCD --- */
void lcdSER_write(char chr)
{

  shiftOutCascade(0, (chr&0xF0)|RS, ONLY_FIRST);
  latch();
  shiftOutCascade(0, (chr&0xF0)|RS|EN, ONLY_FIRST);                 /* seta enable para enviar nibble mais significativo */
  latch();
  _delay_us(1200);                              /* aguarda 1,2ms */
  shiftOutCascade(0, ((chr&0xF0)|RS)&~EN, ONLY_FIRST);                /* clear enable */
  latch();
  _delay_us(1200);                              /* aguarda 1,2ms */
  
  chr <<= 4;                                   /* atualiza chr para enviar nibble menos significativo */

  shiftOutCascade(0, chr|RS, ONLY_FIRST);                           /* seta RS */
  latch();
  shiftOutCascade(0, chr|RS|EN, ONLY_FIRST);                        /* seta enable para enviar nibble menos significativo */
  latch();
  _delay_us(1200);                              /* aguarda 1,2ms */
  shiftOutCascade(0, (chr|RS)&~EN, ONLY_FIRST);                       /* clear enable */
  latch();
  _delay_us(1200);                              /* aguarda 1,2ms */
  
} /* end lcdSER_write */


/* ========================================================================= */
/* --- Envia comandos para LCD --- */
void lcdSER_cmd(unsigned char cmd)
{
  //shift_reg(0b00000100);
  shiftOutCascade(0, cmd&~RS, ONLY_FIRST);                          /* clear RS */
  latch();
  shiftOutCascade(0, cmd|EN, ONLY_FIRST);                           /* seta enable para envio do comando */
  latch();
  _delay_us(1200);                              /* aguarda 1,2ms */
  shiftOutCascade(0, cmd&~EN, ONLY_FIRST);                          /* limpa enable */
  latch();
  _delay_us(1200);                              /* aguarda 1,2ms */

} /* end lcdSER_cmd */


/* ========================================================================= */
/* --- Inicializa LCD no modo 4 bits --- */
void lcdSER_init()
{
  lcdSER_cmd(0x20);                            /* lcd no modo de 4 bits */
  lcdSER_cmd(0x20);                            /* e também */
  lcdSER_cmd(0x20);                            /* 5x8 pontos por caractere */
  lcdSER_cmd(0x00);                            /* liga display e cursor 0h */
  lcdSER_cmd(0xE0);                            /* liga display e cursor Eh */
  lcdSER_cmd(0x00);                            /* modo de incremento de endereço para direita 0h */
  lcdSER_cmd(0x60);                            /* modo de incremento de endereço para direita 6h */
  lcdSER_clear();                              /* limpa LCD */

} /* end lcdSER_init */


/* ========================================================================= */
/* --- Limpa LCD --- */
void lcdSER_clear()
{
  lcdSER_cmd(0x00);                            /* envia 0h e*/
  lcdSER_cmd(0x20);                            /* envia 2h para return home */
  lcdSER_cmd(0x00);                            /* envia 0h e */
  lcdSER_cmd(0x10);                            /* envia 1h para clear lcd */

} /* end lcdSER_clear */

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
  //PORTB &= ~(1 << SR_DAT);
}

void latch(void)
{
  PORTB |= (1<< SR_LATCH);
  _delay_us(200);
  PORTB &= ~(1 << SR_LATCH);
}

void shiftOutCascade(uint8_t data_second_shift, uint8_t data_first_shift, uint8_t mode) 
{
  // Envia os dados para cada shift register em cascata
  static uint8_t data1 = 0, data2 = 0;

  if(mode == BOTH)
  {
    shift_reg(data_second_shift);
    shift_reg(data_first_shift);
    data1 = data_first_shift;
    data2 = data_second_shift;
  }

  else if(mode == ONLY_FIRST)
  {
    shift_reg(data2);
    shift_reg(data_first_shift);
    data1 = data_first_shift;
  }

  else if(mode == ONLY_SECOND)
  {
    shift_reg(data_second_shift);
    shift_reg(data1);
    data2 = data_second_shift;
  }
  // Atualiza os registradores dos shift registers
  latch();
}