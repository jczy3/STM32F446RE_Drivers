#include "stm32f446xx.h"
#include <string.h>

//PB15 - SPI2_MOSI 	CH0
//PB14 - SPI2_MISO
//PB13 - SPI2_SCK	CH1
//PB12 - SPI2_NSS	CH2
//ALT Functionality Mode (AF) : 5

void delay(void)
{
	for (uint32_t i = 0; i < 500000/2; ++i);
}

void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SPI2_SCK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//SPI2_MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//SPI2_MISO
	/*SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins); */

	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);

}

void SPI2_Inits(void)
{
	SPI_Handle_t SPI2handle;
	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI;							// Hardware SM enabled for NSS pin

	SPI_Init(&SPI2handle);
}


void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GPIOBtn;
	GPIOBtn.pGPIOx = GPIOC;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PU;

	GPIO_Init(&GPIOBtn);
}
int main (void)
{
	char user_data[] = "I have always been active my whole life. My parents put me into sports as soon as I was a toddler, playing baseball, basketball. From this, my love for sports branched out to tennis, and later cross country and wrestling.";
	GPIO_ButtonInit();

	// Initialize the GPIO pins to behave as SPI2 pins
	SPI2_GPIOInits();

	// Initialize SPI2 peripheral parameters
	SPI2_Inits();

	/*
	 * Making SSOE 1 does NSS output enable.	(1 for non multimaster mode)
	 * The NSS pin is automatically managed by the hardware.
	 * When SPE = 1, NSS is pulled to low
	 * When SPE = 0, NSS is pulled to high
	 */

	SPI_SSOEConfig(SPI2, ENABLE);

	while(1)
	{
		while (GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13));

		delay();

		// Enable SPI2 peripheral
		SPI_PeripheralControl(SPI2, ENABLE);

		// First send length information
		uint8_t dataLen = strlen(user_data);
		SPI_SendData(SPI2, &dataLen, 1);

		// Send the data
		SPI_SendData(SPI2, (uint8_t*)user_data, strlen(user_data));

		// Confirm SPI is not busy
		while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

		// Disable SPI2 peripheral
		SPI_PeripheralControl(SPI2, DISABLE);
	}

	return 0;
}
