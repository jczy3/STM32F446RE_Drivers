#include "stm32f446xx.h"
#include <string.h>

//PB15 - SPI2_MOSI 	CH0
//PB14 - SPI2_MISO	CH3
//PB13 - SPI2_SCK	CH1
//PB12 - SPI2_NSS	CH2
//ALT Functionality Mode (AF) : 5

//command codes
#define COMMAND_LED_CTRL			0x50
#define COMMAND_SENSOR_READ			0x51
#define COMMAND_LED_READ			0x52
#define COMMAND_PRINT				0x53
#define COMMAND_ID_READ				0x54

#define LED_ON			1
#define LED_OFF			0

//arduino analog pins
#define ANALOG_PIN0		0
#define ANALOG_PIN1		1
#define ANALOG_PIN2		2
#define ANALOG_PIN3		3
#define ANALOG_PIN4		4

#define LED_PIN			9


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
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

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

uint8_t SPI_VerifyResponse(uint8_t ackbyte)
{
	if (ackbyte == 0xF5)
	{
		// ack
		return 1;
	}
	return 0;
}
int main (void)
{

	uint8_t dummy_write = 0xFF;
	uint8_t dummy_read;
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
		//Wait until the button is pressed
		while (GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13));

		delay();

		// Enable SPI2 peripheral
		SPI_PeripheralControl(SPI2, ENABLE);

		//1. CMD_LED_CTRL	<pin no(1)>		<value(1)>

		uint8_t commandCode = COMMAND_LED_CTRL;
		uint8_t ackbyte;
		uint8_t args[2];

		//Send command
		SPI_SendData(SPI2, &commandCode, 1);

		//Do a dummy read to clear the RXNE
		SPI_ReceiveData(SPI2, &dummy_read, 1);

		//Send a dummy byte to fetch the response from the slave
		SPI_SendData(SPI2, &dummy_write, 1);

		//Read the ackbyte received in the RxBuffer
		SPI_ReceiveData(SPI2, &ackbyte, 1);

		if(SPI_VerifyResponse(ackbyte))
		{
			//send arguments
			args[0] = LED_PIN;
			args[1] = LED_ON;
			SPI_SendData(SPI2, args, 2);
		}

		//2. CMD_SENSOR_READ		<analog pin number(1)>

		//Wait until the button is pressed
		while (GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13));

		delay();

		commandCode = COMMAND_SENSOR_READ;

		//Send command
		SPI_SendData(SPI2, &commandCode, 1);

		//Do a dummy read to clear the RXNE
		SPI_ReceiveData(SPI2, &dummy_read, 1);

		//Send a dummy byte to fetch the response from the slave
		SPI_SendData(SPI2, &dummy_write, 1);

		//Read the ackbyte received in the RxBuffer
		SPI_ReceiveData(SPI2, &ackbyte, 1);

		if(SPI_VerifyResponse(ackbyte))
		{
			//send arguments
			args[0] = ANALOG_PIN0;
			SPI_SendData(SPI2, args, 1);

			//Do a dummy read to clear the RXNE
			SPI_ReceiveData(SPI2, &dummy_read, 1);

			delay();

			//Send a dummy byte to fetch the response from the slave
			SPI_SendData(SPI2, &dummy_write, 1);

			uint8_t analog_read;
			SPI_ReceiveData(SPI2, &analog_read, 1);
		}


		// Confirm SPI is not busy
		while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

		// Disable SPI2 peripheral
		SPI_PeripheralControl(SPI2, DISABLE);
	}

	return 0;
}
