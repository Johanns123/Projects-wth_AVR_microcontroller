/*
 * File:   main.c
 * Author: johan
 *
 * Created on 28 de Maio de 2021, 13:41
 */

//este c�digo PWM foi feito no timer1, ou seja, � um pwm de 10 bits. Este c�digo foi feito com base nas conex�es do pro mini
//Para utilizar no Celta caido, ser� necess�rio fazer pequenas altera��es nos registradores.


#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define set_bit(y,bit) (y|=(1<<bit))
#define clr_bit(y,bit) (y&=~(1<<bit)


void setDuty_1(int duty);    //Seleciona o duty cycle na sa�da digital  3
void setFreq(char option);           //Seleciona a frequ�ncia de opera��o do PWM
void setDuty_2(int duty);

int main(void) {
    DDRB = 0b00000110;  //Habilita PB1 e PB2 como sa�da
    PORTB = 0b00000000; //PORTB inicializa desligado e sa�das sem pull up
    
    TCCR1A = 0xA2;                 //Configura opera��o em fast PWM, utilizando registradores OCR1x para compara��o
                         
    setFreq(3);                    //Seleciona op��o para frequ�ncia
        
    while (1) {
        for(int i = 0; i < 1024; i++ ){
            setDuty_1(i);
            setDuty_2(i);
            _delay_ms(10);
        }

    }
}

void setDuty_1(int duty)    
{
   
   OCR1B = duty;

} //end setDuty_pin3

void setDuty_2(int duty)   //MotorB
{
   
   OCR1A = duty;    //valores de 0 - 1023

} //end setDuty_pin3

void setFreq(char option)
{
  /*
  TABLE:
  
      option  frequency (as frequ�ncias no timer 1 s�o menores do que as frequ�ncias nos timers 0 e 2)
        
        1      16    kHz
        2       2    kHz
        3     250     Hz
        4     62,5    Hz
        5     15,6    Hz
  */
   TCCR1B = option;
  

} //end setFrequency


