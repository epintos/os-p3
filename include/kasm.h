/*********************************************
kasm.h

************************************************/

#include "defs.h"


unsigned int    _read_msw();

void            _lidt (IDTR *idtr);

void		_mascaraPIC1 (byte mascara);  /* Escribe mascara de PIC1 */
void		_mascaraPIC2 (byte mascara);  /* Escribe mascara de PIC2 */

void		_Cli(void);        /* Deshabilita interrupciones  */
void		_Sti(void);	 /* Habilita interrupciones  */

void		_int_08_hand();      /* Timer tick */
void		_int_09_hand();      
void		_int_80_hand();
void		_int_0_hand();
void		_int_05_hand();
void		_int_06_hand();
void		_int_B_hand();
void		_int_C_hand();
void		_int_D_hand();
void		_int_E_hand();
params *		_wrapper_int_80();

void		_call_int_08();
void		_Halt();
void		_restart_PC();
void 		_out();
int			_inw();
int			_outw();
int 		_in();
unsigned char _inb();
void		_outb();
void		_activatePaging();
void		_setPageDir();
void		 _printASM();
unsigned int _readCR2();
unsigned int _getESP();

void		_debug (void);
