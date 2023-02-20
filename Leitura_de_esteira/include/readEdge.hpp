/* ===========================================================================

	Leitura de Entradas digitais de acordo com a borda gerada.

    ARQUIVO readEdge.hpp
    
    MCU:   Atmega328p
    Clock: 16MHz Cristal Externo

    Compilador: Microchip Studio 7.0.2542
    Autor: Dr. Eng. Wagner Rambo
    Data de cria��o: Abril de 2022
    �ltima atualiza��o: 16/04/2022

============================================================================ */


#ifndef READEDGE_HPP_
#define READEDGE_HPP_

// ============================================================================ 
// --- Clock de MCU ---
#define  F_CPU  16000000UL


// ============================================================================
// --- Bibliotecas ---
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>


// ============================================================================
// --- Classe ---
class readEdge
{
  public:
  
    readEdge(volatile uint8_t *dir, volatile uint8_t *reg, const uint8_t pin, const bool action_edge);
	
    void firstEdge(void (*callback)());
    void seconEdge(void (*callback)());
    void run(void);
    void fallling_rising_edge_hardware(void);
  
  private:	
  
  uint8_t pin_;
  volatile uint8_t *dir_, *reg_;
	bool    in_st, read_input;
	
	void(*firstE)();
	void(*seconE)();


}; //end class


#endif // READEDGE_HPP_


/* ============================================================================

                                       _
                                      / \
                                     |oo >      <-- (Woww, advanced application!)
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
/* --- Final do Arquivo readEdge.hpp --- */