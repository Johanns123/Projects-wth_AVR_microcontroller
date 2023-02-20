/*Bibliotecas e frequ�ncia do uc*/
#define F_CPU 16000000        //define a frequencia do uC para 16MHz
#include <avr/io.h>           //Biblioteca geral dos AVR
#include <avr/interrupt.h>    //Biblioteca de interrup��o
#include <stdio.h>            //Bilioteca do C
#include <util/delay.h>       //Biblioteca geradora de atraso
#include <avr/io.h>
#include <avr/pgmspace.h>   //para o uso do PROGMEM, grava��o de dados na mem�ria flash
#include "ADC.h"

//vari�veis de comando para os registradores
#define set_bit(y,bit) (y|=(1<<bit)) //coloca em 1 o bit x da vari�vel Y
#define clr_bit(y,bit) (y&=~(1<<bit)) //coloca em 0 o bit x da vari�vel Y
#define cpl_bit(y,bit) (y^=(1<<bit)) //troca o estado l�gico do bit x da vari�vel Y
#define tst_bit(y,bit) (y&(1<<bit)) //retorna 0 ou 1 conforme leitura do bit


//mapeamento de hardware
//===============
#define PWM_pin     PB1

//Vari�veis globais
//================
unsigned char pwm = 128, pwm_state = 0;
int ADC_dados;
//Prot�tipo das fun��es
//================
void setup_hardware(void);
void setup(void);
void INT_init(void);
void f_timer0();
void f_timer1();
void f_timer2();
void ADC_maq();
void tempos (char valor, char seq);
void loop();
void display_maq();
void f_timers();
void sequencia1();
void sequencia2();
void f_int_ADC();
void pwm_motor(unsigned adj, unsigned char *pwm);

ISR(TIMER0_OVF_vect)
{   
    if(pwm_state)                /* PWM_pin em n�vel high? */
    {                                         /* sim */
      TCNT0 = pwm;                            /* TMR0L recebe valor atual do pwm1 */
      clr_bit(PORTB, PWM_pin);                /* sa�da PWM1_pin em low */
      pwm_state = 0;
    } /* end if PWM_pin */
    else                                      /* sen�o...*/
    {
      TCNT0 = 256 - pwm;                      /* TMR0L recebe valor m�ximo menos valor do pwm1 */
      set_bit(PORTB, PWM_pin);                /* sa�da PWM1 em high */
      pwm_state = 1;
    } /* end else */

}


ISR(ADC_vect)
{
    ADC_maq();
}



int main(void) {
    setup_hardware();
    INT_init();
    ADC_init();
    sei();
    setup();
    while (1) {
        loop();
    }
}

void setup_hardware()
{
    MCUCR &= 0xef;  //Pull up interno habilitado
    DDRB = 0xff;    //Todo PORTB como sa�da
    PORTB = 0x00;   //inicia todos do PORTD em LOW
    DDRC = 0xf0;    //PC0-PC3 como entrada
    PORTC = 0x0d;   //entradas com pull up e PC1 sem
    DDRD = 0xff;    //Todo PORTD como sa�da
    PORTD = 0x00;   //iniciado em low
    DIDR0 = 0x02;   //desabilita entrada digital de PC1
}

void setup()
{
    
}

void INT_init()
{
    TCCR0B = 0b00000101; //TC0 com prescaler de 1024
    TIMSK0 = 0b00000001; //habilita a interrup��o do TC0

}


void ADC_maq () 
{

    ADC_conv_ch(1);
    ADC_dados = ADC_ler();
    
    
}

void f_timers()
{
    
    
}

void f_timer0()
{
    

}

void loop()
{
    pwm_motor(ADC_dados,&pwm);                     /* atualiza valor do pwm */
}

/* ========================================================================= */
/* --- pwm_motor --- */
/* Atualiza o PWM do motor de acordo com o ajuste */
void pwm_motor(unsigned adj, unsigned char *pwm)
{
  *pwm = adj>>2;                               /* pwm = adj/4 (normaliza para 8 bits) */
  if(*pwm<1)*pwm=1;                            /* fixa o valor 1 como o m�nimo */

} /* end pwm_motor */