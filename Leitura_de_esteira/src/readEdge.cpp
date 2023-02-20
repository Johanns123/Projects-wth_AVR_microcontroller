/* ===========================================================================

	Leitura de Entradas digitais de acordo com a borda gerada.

    ARQUIVO readEdge.cpp
    
    MCU:   Atmega328p
    Clock: 16MHz Cristal Externo

    Compilador: Microchip Studio 7.0.2542
    Autor: Dr. Eng. Wagner Rambo
    Data de cria��o: Abril de 2022
    �ltima atualiza��o: 16/04/2022

============================================================================ */


// ============================================================================
// --- Bibliotecas ---
#include "readEdge.hpp"


// ============================================================================
// --- Desenvolvimento das Fun��es ---


// ============================================================================
// --- Construtor readEdge ---
readEdge::readEdge(volatile uint8_t *dir, volatile uint8_t *reg,const uint8_t pin, const bool action_edge):
dir_(dir),
reg_(reg),
pin_(pin),
in_st(action_edge)
{
  *dir_ &= ~(1<<pin_);	
  read_input = *reg_ &(1<<pin_);
	
}


// ============================================================================
// --- firstEdge (detec��o da primeira borda) ---
void readEdge::firstEdge(void (*callback)())
{
  firstE = callback;	
  
}


// ============================================================================
// --- seconEdge (detec��o da segunda borda) ---
void readEdge::seconEdge(void (*callback)())
{
	seconE = callback;
	
}


// ============================================================================
// --- run ---
void readEdge::run()
{
  bool a_input;
  
  a_input = PINB&(1<<pin_);	
  
  if(a_input != read_input)
  {
    if(a_input == in_st)
	{
    if(firstE != NULL)
    firstE();
		   
	}
	else
	{
	  if(seconE != NULL)
		seconE();

	}
	  
	read_input = a_input;  
	  
  } //end if
	
} //end run

void readEdge::fallling_rising_edge_hardware()
{
  if(firstE != NULL)
    firstE();
}

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
/* --- Final do Arquivo readEdge.cpp --- */