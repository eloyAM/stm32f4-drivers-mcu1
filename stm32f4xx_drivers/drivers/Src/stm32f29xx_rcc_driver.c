#include "stm32f429xx_rcc_driver.h"

uint16_t AHB_PreScaler[8] = {2, 4, 8, 16, 64, 128, 256, 512};
uint8_t APB_PreScaler[4] = {2, 4, 8, 16};


// TODO Not implemented, we are not using PLL clock
uint32_t RCC_GetPLLOutputClock(void)
{
	return 0;
}

uint32_t RCC_GetPCLK1Value(void)
{
	uint32_t pckl1, SystemClk;
	uint8_t clksrc, temp, ahbp, apb1p;
	clksrc = ((RCC->CFGR >> 2) & 0x3);
	switch (clksrc) {
		case 0: SystemClk = 16000000; break;
		case 1: SystemClk =  8000000; break;
		case 2: SystemClk = RCC_GetPLLOutputClock(); break;
	}
	// For AHB
	temp = ((RCC->CFGR >> 4) & 0xF);
	if (temp < 8) {
		ahbp = 1;
	} else {
		ahbp = AHB_PreScaler[temp-8];
	}
	// For APB1
	temp = ((RCC->CFGR >> 10) & 0x7);
	if (temp < 8) {
		apb1p = 1;
	} else {
		apb1p = APB_PreScaler[temp-4];
	}

	pckl1 = (SystemClk / ahbp) / apb1p;

	return pckl1;
}


uint32_t RCC_GetPCLK2Value(void)
{
	uint32_t pclk2, SystemClk = 0, temp;

	uint8_t clksrc = ((RCC->CFGR >> 2) & 0x3);

	uint8_t ahbp, apb2p;

	switch (clksrc) {
		case 0: SystemClk = 16000000; break;
		case 1: SystemClk =  8000000; break;
		case 2: SystemClk = RCC_GetPLLOutputClock(); break;
	}

	/* AHBP */
	temp = ((RCC->CFGR >> 4) & 0xF);
	if (temp < 8) {
		ahbp = 1;
	} else {
		ahbp = AHB_PreScaler[temp-8];
	}

	/* APB2 */
	temp = ((RCC->CFGR >> 13) & 0x7);
	if (temp < 4) {
		apb2p = 1;
	} else {
		apb2p = APB_PreScaler[temp-4];
	}

	pclk2 = (SystemClk / ahbp) / apb2p;

	return pclk2;
}
