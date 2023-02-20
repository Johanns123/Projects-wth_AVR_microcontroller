/*
 * File:   main.c
 * Author: johannamorim
 *
 * Created on 8 de Agosto de 2021, 21:23
 */




//defini��es das bibliotecas
//========================
#define F_CPU 16000000        //define a frequencia do uC para 16MHz
#include <avr/io.h>           //Biblioteca geral dos AVR
#include <avr/interrupt.h>    //Biblioteca de interrup��o
#include <stdio.h>            //Bilioteca do C
#include <util/delay.h>       //Biblioteca geradora de atraso
//Defini��es das macros
//====================
#define botao1  PB0
#define botao2  PB1
#define botao3  PB2

//vari�veis de comando para os registradores
#define set_bit(y,bit) (y|=(1<<bit)) //coloca em 1 o bit x da vari�vel Y
#define clr_bit(y,bit) (y&=~(1<<bit)) //coloca em 0 o bit x da vari�vel Y
#define cpl_bit(y,bit) (y^=(1<<bit)) //troca o estado l�gico do bit x da vari�vel Y
#define tst_bit(y,bit) (y&(1<<bit)) //retorna 0 ou 1 conforme leitura do bit
//==============================================================

/*Estrutura*/
struct perif
{
    int flag1, flag2, valor;
}periferal;

//Vari�veis globais
//================

//Prot�tipo das fun��es
//================

ISR(PCINT0_vect) {

    if (!tst_bit(PINB, botao1)) //bot�o1 pressionado?
    {
        periferal.flag1 = 1;
        
    }

    if (tst_bit(PINB, botao1) && periferal.flag1)
    {
        cpl_bit(PORTD, PD2);
        periferal.flag1 = 0;
        periferal.valor = 1;
        
    }
    
    if (!tst_bit(PINB, botao2)) //bot�o2 pressionado?
    {
        periferal.flag2 = 1;

    }

    if (tst_bit(PINB, botao2) && periferal.flag2)
    {
        periferal.flag2 = 0;
        periferal.valor = 2;
    }
    

}

void main() {
    DDRD = 0xff; //Todo PORTD como sa�da
    PORTD = 0x00; //inicia todos do PORTD em LOW
    DDRB &= ~(1 << botao1) &~(1 << botao2) &~(1 << botao3); //PORTB como entrada
    PORTB = 0x03; //resistores de pull-up internos desabilitados

    PCICR = 0x01; //Habilita interrup��o do PCINT0
    PCMSK0 = 0x03; //Habilita PCINT0 e PCINT1 e PCINT2

    sei();

    while (1) {
        char display [] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07
            , 0x7F, 0x6F, 0x77, 0x7F, 0x39, 0x3F, 0x79, 0x71};

        for (int i = 0; i < 16; i++) {
            switch (periferal.valor) {
                case 0:
                    PORTD = display[i];
                    
                case 1: //imprime n�meros primos em hexa  
                    switch (i) {
                        case 2:
                            PORTD = display[2];
                            break;

                        case 3:
                            PORTD = display[3];
                            break;

                        case 5:
                            PORTD = display[5];
                            break;

                        case 7:
                            PORTD = display[7];
                            break;

                        case 11:
                            PORTD = display[11];
                            break;

                        case 13:
                            PORTD = display[13];
                            break;

                    }
                    break;

                case 2://imprime n�meros n�o primos
                    switch (i) {
                        case 0:
                            PORTD = display[0];
                            break;

                        case 1:
                            PORTD = display[1];
                            break;

                        case 4:
                            PORTD = display[4];
                            break;

                        case 6:
                            PORTD = display[6];
                            break;

                        case 8:
                            PORTD = display[8];
                            break;

                        case 9:
                            PORTD = display[9];
                            break;

                        case 10:
                            PORTD = display[10];
                            break;

                        case 12:
                            PORTD = display[12];
                            break;

                        case 14:
                            PORTD = display[14];
                            break;

                        case 15:
                            PORTD = display[15];
                            break;

                    }


            }
            _delay_ms(500);
        }

        periferal.valor = 0;

    }
}
