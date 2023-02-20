#include "liquidCrystal.hpp"
/* ========================================================================= */
/* --- Desenvolvimento das Fun��es do LCD --- */

liquidCrystal::liquidCrystal(volatile uint8_t *reg, volatile uint8_t *dir, uint8_t pin_RS, uint8_t pin_EN, uint8_t pin_D4, uint8_t pin_D5, uint8_t pin_D6, uint8_t pin_D7):
dir_(dir),
reg_(reg),
pin_RS_(pin_RS),
pin_EN_(pin_EN),
pin_D4_(pin_D4),
pin_D5_(pin_D5),
pin_D6_(pin_D6),
pin_D7_(pin_D7)
{
    *dir_ |= (1<<pin_RS_) | (1<<pin_EN_) | (1<<pin_D4_) | (1<<pin_D5_),
    (1<<pin_D6_), (1<<pin_D7_);
}

/* ========================================================================= */
/* --- disp_number --- */
/* Converte um inteiro para exibir no display, remove zeros � esquerda */
void liquidCrystal::disp_number(unsigned num, char row, char col)
{
  char  dem, mil, cen, dez, uni;               /* vari�veis para c�lculo de cada d�gito */
  short  no_zero = 0;                          /* vari�vel local para limpeza de zeros � esquerda */

  dem = (char)(num/10000);                     /* calcula dezenas de milhares */
  mil = (char)(num%10000/1000);                /* calcula milhares */
  cen = (char)(num%1000/100);                  /* calcula centenas */
  dez = (char)(num%100/10);                    /* calcula dezenas */
  uni = (char)(num%10);                        /* calcula unidades */

  if(!dem && !no_zero)                         /* d�gito das dezenas de milhares � zero e no_zero est� limpa? */
    liquidCrystal::disp_wr_po(' ',row,col);                   /* sim, imprime um espa�o em branco */
  else                                         /* n�o... */
  {
    liquidCrystal::disp_wr_po(dem+0x30,row,col);              /* imprime o d�gito calculado */
    no_zero = 1;                               /* seta no_zero */
  } /* end else */

  if(!mil && !no_zero)                         /* d�gito dos milhares � zero e no_zero est� limpa? */
    liquidCrystal::disp_write(' ');                           /* sim, imprime um espa�o em branco */
  else                                         /* n�o... */
  {
    liquidCrystal::disp_write(mil+0x30);                      /* imprime o d�gito calculado */
    no_zero = 1;                               /* seta no_zero */
  } /* end else */

  if(!cen && !no_zero)                         /* d�gito das centenas � zero e no_zero est� limpa? */
    liquidCrystal::disp_write(' ');                           /* sim, imprime um espa�o em branco */
  else                                         /* n�o... */
  {
    liquidCrystal::disp_write(cen+0x30);                      /* imprime o d�gito calculado */
    no_zero = 1;                               /* seta no_zero */
  } /* end else */

  if(!dez && !no_zero)                         /* d�gito das dezenas � zero e no_zero est� limpa? */
    liquidCrystal::disp_write(' ');                           /* sim, imprime um espa�o em branco */
  else                                         /* n�o... */
  {
    liquidCrystal::disp_write(dez+0x30);                      /* imprime o d�gito calculado */
    no_zero = 1;                               /* seta no_zero */
  } /* end else */

  liquidCrystal::disp_write(uni+0x30);                        /* imprime d�gito calculado */


} /* end disp_number */


/* ========================================================================= */
/* --- disp_wr_po --- */
/* fun��o para escrever caracteres no LCD na posi��o indicada */
void liquidCrystal::disp_wr_po(unsigned char chr, char row, char col)
{
  if(!row)                                                    /* linha 0? */
  {                                                           /* sim */
    liquidCrystal::disp_cmd(0x80|col);                        /* envia comando para posicionar na coluna correta */
    liquidCrystal::disp_write(chr);                           /* escreve o caractere */
  } /* end if */
  else                                                        /* sen�o... */
  {                                                           /* linha 1 */
    liquidCrystal::disp_cmd(0xC0|col);                        /* envia comando para posicionar linha e coluna corretas */
    liquidCrystal::disp_write(chr);                           /* escreve o caractere */
  } /* end else */

} /* end disp_write */


/* ========================================================================= */
/* --- disp_text --- */
/* fun��o para escrever uma string no LCD */
void liquidCrystal::disp_text(char *str, char row, char col)
{
  register int i;                                             /* vari�vel local para itera��es */

  for(i=0; str[i]!='\0';i++,col++)                            /* executa at� encontrar o caractere nulo */
    liquidCrystal::disp_wr_po(str[i],row,col);                /* imprime caractere atual da string passada como par�metro */
} /* end disp_text */


/* ========================================================================= */
/* --- disp_write --- */
/* fun��o para escrever caracteres no LCD */
void liquidCrystal::disp_write(unsigned char chr)
{
  liquidCrystal::send_nibble(chr, 1);                         /* envia o nibble mais significativo do caractere, RS em high */
  chr <<= 4;                                                  /* atualiza chr para enviar nibble menos significativo */
  liquidCrystal::send_nibble(chr, 1);                         /* envia o nibble menos significativo do caractere, RS em high */

} /* end disp_write */


/* ========================================================================= */
/* --- disp_cmd --- */
/* fun��o para enviar comandos para o LCD */
void liquidCrystal::disp_cmd(unsigned char cmd)
{
  liquidCrystal::send_nibble(cmd, 0);                         /* envia o nibble mais significativo do comando, RS em low */
  cmd <<= 4;                                                  /* atualiza chr para enviar nibble menos significativo */
  liquidCrystal::send_nibble(cmd, 0);                         /* envia o nibble menos significativo do comando, RS em low */
   
} /* end disp_cmd */


/* ========================================================================= */
/* --- disp_init --- */
/* fun��o para inicializar o LCD */
void liquidCrystal::disp_init(void)
{
  DELAY_MS(48);                                               /* tempo para estabiliza��o (datasheet recomenda no m�nimo 40ms) */
  liquidCrystal::send_nibble(0x30,0);                         /* protocolo de inicializa��o */
  DELAY_MS(5);                                                /* tempo acima do sugerido pelo datasheet pag.46) */
  liquidCrystal::send_nibble(0x30,0);                         /* protocolo de inicializa��o */
  DELAY_MS(150);                                              /* tempo acima do sugerido pelo datasheet pag.46) */
  liquidCrystal::send_nibble(0x30,0);                         /* protocolo de inicializa��o */
  liquidCrystal::send_nibble(0x20,0);                         /* lcd no modo de 4 bits */
  liquidCrystal::disp_cmd(0x28);                              /* 5x8 pontos por caractere, duas linhas */
  liquidCrystal::disp_cmd(0x0F);                              /* liga display, cursor e blink */
  liquidCrystal::disp_cmd(0x01);                              /* clear LCD */
  liquidCrystal::disp_cmd(0x06);                              /* modo de incremento de endere�o para direita */
  liquidCrystal::disp_clear();                                /* limpa LCD */

} /* end disp_init*/


/* ========================================================================= */
/* --- Limpa LCD --- */
/* fun��o para limpar o LCD */
void liquidCrystal::disp_clear(void)
{
  liquidCrystal::disp_cmd(0x02);                              /* return home */
  liquidCrystal::disp_cmd(0x01);                              /* limpa o display */

} /* end disp_clear */


/* ========================================================================= */
/* --- send_nibble --- */
/* fun��o para envio de cada nibble separadamente e gerar pulso em enable */
void liquidCrystal::send_nibble(unsigned char nib, char rsel)
{
  if((nib>>4)&0x01)                            /* envia bit 4 de comando no barramento */
    set_bit(*reg_,pin_D4_);                    /* . */
  else                                         /* . */
    clr_bit(*reg_,pin_D4_);                    /* . */

  if((nib>>5)&0x01)                            /* envia bit 5 de comando no barramento */
    set_bit(*reg_,pin_D5_);                    /* . */
  else                                         /* . */
    clr_bit(*reg_,pin_D5_);                    /* . */  

  if((nib>>6)&0x01)                            /* envia bit 6 de comando no barramento */
    set_bit(*reg_,pin_D6_);                    /* . */
  else                                         /* . */
    clr_bit(*reg_,pin_D6_);                    /* . */
	
  if((nib>>7)&0x01)                            /* envia bit 7 de comando no barramento */
    set_bit(*reg_,pin_D7_);                    /* . */
  else                                         /* . */
    clr_bit(*reg_,pin_D7_);                    /* . */
 
  
  if(rsel)                                     /* se rsel for verdadeiro */
    set_bit(*reg_,pin_RS_);                    /* envio de dados */
  else                                         /* se rsel for falso */
    clr_bit(*reg_,pin_RS_);                    /* envio de comandos */

  /* -- Pulse Enable -- */
  set_bit(*reg_,pin_EN_);                     /* seta EN */
  DELAY_US(1200);                             /* aguarda 1200�s */
  clr_bit(*reg_,pin_EN_);                     /* limpa EN */
  DELAY_US(1200);                             /* aguarda 1200�s */

} /* end send nibble */


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
/* --- Final do Arquivo lcd.c --- */