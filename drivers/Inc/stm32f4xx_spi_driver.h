/*
 * stm32f4xx_spi_driver.h
 *
 *  Created on: Mar 25, 2026
 *      Author: yaboy
 */

#ifndef INC_STM32F4XX_SPI_DRIVER_H_
#define INC_STM32F4XX_SPI_DRIVER_H_

#include "stm32f446xx.h"

/*
 * Configuration Structure for SPIx peripheral
 */

typedef struct
{
	uint8_t SPI_DeviceMode;				/* Possible values from @ */
	uint8_t SPI_BusConfig;				/* Possible values from @   */
	uint8_t SPI_SclkSpeed;				/* Possible values from @   */
	uint8_t SPI_DFF;					/* Possible values from @    */
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;
} SPI_Config_t;

/*
 * Handle Structure for SPIx peripheral
 */

typedef struct
{
	SPI_RegDef_t *pSPIx;		/* This holds the base address of SPIx peripheral */
	SPI_Config_t SPIConfig; 	/*  */

} SPI_Handle_t;

/*****************************************************
 *  				SPI Driver APIs
 *  **************************************************/

/*
 * Peripheral Clock setup
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void SPI_Init(SPI_RegDef_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

/*
 * Data Send and Receive (3 methodologies, polling/blocking based, interrupt/nonblocking based, and DMA based)
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

/*
 * IRQ Configuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

/*
 * Other Peripheral Control APIs
 */

#endif /* INC_STM32F4XX_SPI_DRIVER_H_ */
