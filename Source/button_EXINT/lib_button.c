#include "button.h"
#include "LPC17xx.h"

/**
 * @brief  Function that initializes Buttons
 */
void BUTTON_init(void) {

	LPC_PINCON->PINSEL4 &= ~(0xFF << 20);
	LPC_GPIO2->FIODIR   &= ~(1 << 10); // INT0
  LPC_GPIO2->FIODIR   &= ~(1 << 11); // KEY1
  LPC_GPIO2->FIODIR   &= ~(1 << 12); // KEY2
}
