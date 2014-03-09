/* ###################################################################
 **     Filename    : ProcessorExpert.c
 **     Project     : ProcessorExpert
 **     Processor   : MKL25Z128VLK4
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2014-03-04, 16:57, # CodeGen: 0
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file ProcessorExpert.c
 ** @version 01.01
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup ProcessorExpert_module ProcessorExpert module documentation
 **  @{
 */
/* MODULE ProcessorExpert */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "HIDM1.h"
#include "Tx2.h"
#include "CsIO1.h"
#include "IO1.h"
#include "CS1.h"
#include "MMA1.h"
#include "GI2C1.h"
#include "WAIT1.h"
#include "I2C1.h"
#include "USB1.h"
#include "USB0.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "stdlib.h"

static const int MOVEMENT_SCALE = 1320;
static const int X_THRESHOLD = 1500;
static const int Y_THRESHOLD = 1500;

/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
char* itoa(int value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) {
		*result = '\0';
		return result;
	}

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ =
				"zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35
						+ (tmp_value - value * base)];
	} while (value);

	// Apply negative sign
	if (tmp_value < 0)
		*ptr++ = '-';
	*ptr-- = '\0';
	while (ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/
	/* Write your code here */
	/* For example: for(;;) { } */
	MMA1_Enable();
	static int16_t xValue = 0, yValue = 0, zValue = 0;
	while (1) {
		xValue = MMA1_GetX(); // X value and Y value are reversed 
		yValue = MMA1_GetY(); // Accelerometer orientation thing
		zValue = MMA1_GetZ() * 0.5 + zValue * 0.5; 

//		printf("X : %i, Y: %i, Z: %i", xValue, yValue, zValue);
		/* Write your code here ... */

		bool justClicked = FALSE;
		HIDM1_App_Task();
		
		if (zValue > 21000 && !justClicked) {
			HIDM1_Click(1);
			justClicked = TRUE;
			continue;
		}
		if (zValue < 1000)
			justClicked = FALSE;
		HIDM1_App_Task();
		
		if (xValue > X_THRESHOLD) {
			xValue -= X_THRESHOLD;
		} else if (xValue < -X_THRESHOLD) {
			xValue += X_THRESHOLD;
		} else {
			xValue = 0;
		}

		if (yValue > Y_THRESHOLD) {
			yValue -= Y_THRESHOLD;
		} else if (yValue < -Y_THRESHOLD) {
			yValue += Y_THRESHOLD;
		} else {
			yValue = 0;
		}

		HIDM1_Move(xValue / MOVEMENT_SCALE, -yValue / MOVEMENT_SCALE);


	}
	/* Write your code here ... */

	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
	/*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
#ifdef PEX_RTOS_START
	PEX_RTOS_START(); /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
#endif
	/*** End of RTOS startup code.  ***/
	/*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
	for (;;) {
	}
	/*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END ProcessorExpert */
/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.3 [05.08]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */
