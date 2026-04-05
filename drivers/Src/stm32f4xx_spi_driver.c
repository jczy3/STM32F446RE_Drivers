/*
 * stm32f4xx_spi_driver.c
 *
 *  Created on: Mar 25, 2026
 *      Author: yaboy
 */
#include "stm32f446xx_spi_driver.h"
/**
	* @fn			 : GPIO_PeriClockControl
	* @brief  		 : This function enables or disables peripheral clock for the given GPIO port
	* @param  *GPIOx : GPIO Port Base address
	* @param  EnorDi : ENABLE or DISABLE macros
	* @retval None
*/
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
		if (pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}
		else if (pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}
		else if (pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}
		else if (pSPIx == SPI4)
		{
			SPI4_PCLK_EN();
		}
	}
	else
	{
		if (pSPIx == SPI1)
		{
			SPI1_PCLK_DI();
		}
		else if (pSPIx == SPI2)
		{
			SPI2_PCLK_DI();
		}
		else if (pSPIx == SPI3)
		{
			SPI3_PCLK_DI();
		}
		else if (pSPIx == SPI4)
		{
			SPI4_PCLK_DI();
		}
	}
}

/**
	* @fn			 : GPIO_PeriClockControl
	* @brief  		 : This function enables or disables peripheral clock for the given GPIO port
	* @param  *GPIOx : GPIO Port Base address
	* @param  EnorDi : ENABLE or DISABLE macros
	* @retval None
*/
void SPI_Init(SPI_Handle_t *pSPIHandle)
{

	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	uint32_t tempreg = 0;

	//peripheral clock enable
	// configure device mode
	tempreg |= (pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR);

	//configure bus config
	if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		//clear bidi mode
		tempreg &= ~(1 << SPI_CR1_BIDI_MODE);
	}
	else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		//set bidi mode
		tempreg |= (1 << SPI_CR1_BIDI_MODE);
	}
	else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RX)
	{
		//clear bidi mode
		tempreg &= ~(1 << SPI_CR1_BIDI_MODE);
		//RXONLY bit must be set
		tempreg |= (1 << SPI_CR1_RX_ONLY);
	}

	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

	tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;

	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;

	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	pSPIHandle->pSPIx->CR1 = tempreg;
}

/**
	* @fn			 : GPIO_PeriClockControl
	* @brief  		 : This function enables or disables peripheral clock for the given GPIO port
	* @param  *GPIOx : GPIO Port Base address
	* @param  EnorDi : ENABLE or DISABLE macros
	* @retval None
*/
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	if (pSPIx == SPI1)
	{
		SPI1_REG_RESET();
	}
	else if (pSPIx == SPI2)
	{
		SPI2_REG_RESET();
	}
	else if (pSPIx == SPI3)
	{
		SPI3_REG_RESET();
	}
	else if (pSPIx == SPI4)
	{
		SPI4_REG_RESET();
	}
}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/**
	* @fn			 : GPIO_PeriClockControl
	* @brief  		 : This function enables or disables peripheral clock for the given GPIO port
	* @param  *GPIOx : GPIO Port Base address
	* @param  EnorDi : ENABLE or DISABLE macros
	* @retval None
	* @Note			 : This is a blocking call (polling)
*/
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while (Len > 0)
	{
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

		if ((pSPIx->CR1 & (1 << SPI_CR1_DFF)))
		{
			pSPIx->DR = *((uint16_t*)pTxBuffer);
			Len--;
			Len--;
			(uint16_t*)pTxBuffer++;
		}
		else
		{
			pSPIx->DR = *pTxBuffer;
			Len--;
			pTxBuffer++;
		}
	}
}

/**
	* @fn			 : GPIO_PeriClockControl
	* @brief  		 : This function enables or disables peripheral clock for the given GPIO port
	* @param  *GPIOx : GPIO Port Base address
	* @param  EnorDi : ENABLE or DISABLE macros
	* @retval None
*/
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
	while (Len > 0)
	{
		while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

		if ((pSPIx->CR1 & (1 << SPI_CR1_DFF)))
		{
			// 16 bit DFF
			// Load the data from the DR to RxBuffer address
			*((uint16_t*)pRxBuffer) = pSPIx->DR;
			Len--;
			Len--;
			(uint16_t*)pRxBuffer++;
		}
		else
		{
			// 8 bit DFF
			*(pRxBuffer) = pSPIx->DR;
			Len--;
			pRxBuffer++;
		}
	}
}

/**
	* @fn			 : GPIO_PeriClockControl
	* @brief  		 : This function enables or disables peripheral clock for the given GPIO port
	* @param  *GPIOx : GPIO Port Base address
	* @param  EnorDi : ENABLE or DISABLE macros
	* @retval None
*/
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);

/**
	* @fn			 : GPIO_PeriClockControl
	* @brief  		 : This function enables or disables peripheral clock for the given GPIO port
	* @param  *GPIOx : GPIO Port Base address
	* @param  EnorDi : ENABLE or DISABLE macros
	* @retval None
*/
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

/**
	* @fn			 : GPIO_PeriClockControl
	* @brief  		 : This function enables or disables peripheral clock for the given GPIO port
	* @param  *GPIOx : GPIO Port Base address
	* @param  EnorDi : ENABLE or DISABLE macros
	* @retval None
*/
void SPI_IRQHandling(SPI_Handle_t *pHandle);

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}

void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
		pSPIx->CR2 |= (1 << SPI_CR2_SSOE);
	}
	else
	{
		pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);
	}
}
