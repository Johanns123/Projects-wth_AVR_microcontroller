//===================================//
/*Definicao de bibliotecas*/
#ifndef LIQUIDCRYSTAL
#define LIQUIDCRYSTAL

#define ATMEGA328P

#ifdef ATMEGA328P
/* ========================================================================= */
/* --- Frequ�ncia de Clock --- */
#define    F_CPU    16000000									          /* configurado para 16MHz */


/* ========================================================================= */
/* --- Bibliotecas Auxiliares --- */
#include <avr/io.h>										                  /* biblioteca de entradas e sa�das padr�o */
#include <util/delay.h>													  /* biblioteca com as fun��es de delay */


/* ========================================================================= */
/* --- Macros --- */
#define   set_bit(reg,bit)		(reg |= (1<<bit))		   /* seta um bit de determinado registrador */
#define   clr_bit(reg,bit)		(reg &= ~(1<<bit))		   /* limpa um bit de determinado registrador */
#define   test_bit(reg,bit)      (reg &   (1<<bit))         /* l� um bit de determinado registrador */
#define   DELAY_MS(ms)          _delay_ms(ms);
#define   DELAY_US(us)          _delay_us(us);

#endif
//===================================//
/*Definicao da classe*/

class liquidCrystal
{
    public:
        /*Construtor*/
        liquidCrystal(volatile uint8_t *reg, volatile uint8_t *dir, uint8_t pin_RS, uint8_t pin_EN, uint8_t pin_D4, uint8_t pin_D5, uint8_t pin_D6, uint8_t pin_D7);
        /* ========================================================================= */
        /* --- Prot�tipo das Fun��es --- */
        void disp_number(unsigned num, char row, char col);        /* converte um inteiro de at� 5 d�gitos para exibir no display, remove zeros � esquerda */
        void disp_wr_po(unsigned char chr, char row, char col);    /* fun��o para escrever caracteres no LCD na posi��o indicada */
        void disp_text(char *str, char row, char col);             /* fun��o para escrever uma string no LCD */
        void disp_write(unsigned char chr);                        /* fun��o para escrever caracteres no LCD */
        void disp_cmd(unsigned char cmd);                          /* fun��o para enviar comandos para o LCD*/
        void disp_init(void);                                      /* fun��o para inicializar o LCD */
        void disp_clear(void);                                     /* fun��o para limpar o LCD */

    private:
        volatile uint8_t *dir_,*reg_;
        uint8_t pin_RS_,  pin_EN_,  pin_D4_, pin_D5_, pin_D6_, pin_D7_; 
        void send_nibble(unsigned char nib, char rsel);            /* envia cada nibble separadamente e gera pulso em enable */

};
#endif


/* ============================================================================

                                       _
                                      / \
                                     |oo >      <-- (Hello, liquid crystal display!)
                                     _\=/_
                    ___         #   /  _  \   #
                   /<> \         \\//|/.\|\\//
                 _|_____|_        \/  \_/  \/
                | | === | |          |\ /|
                |_|  0  |_|          \_ _/
                 ||  0  ||           | | |
                 ||__*__||           | | |
                |* \___/ *|          []|[]
                /=\ /=\ /=\          | | |
________________[_]_[_]_[_]_________/_]_[_\_______________________________


============================================================================ */
/* --- Final do Arquivo lcd.h --- */