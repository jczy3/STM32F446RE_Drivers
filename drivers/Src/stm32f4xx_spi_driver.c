/*
 * stm32f4xx_spi_driver.c
 *
 *  Created on: Mar 25, 2026
 *      Author: yaboy
 */

/**
	* @fn			 : GPIO_PeriClockControl
	* @brief  		 : This function enables or disables peripheral clock for the given GPIO port
	* @param  *GPIOx : GPIO Port Base address
	* @param  EnorDi : ENABLE or DISABLE macros
	* @retval None
*/
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/**
	* @fn			 : GPIO_PeriClockControl
	* @brief  		 : This function enables or disables peripheral clock for the given GPIO port
	* @param  *GPIOx : GPIO Port Base address
	* @param  EnorDi : ENABLE or DISABLE macros
	* @retval None
*/
void SPI_Init(SPI_RegDef_t *pSPIHandle);

/**
	* @fn			 : GPIO_PeriClockControl
	* @brief  		 : This function enables or disables peripheral clock for the given GPIO port
	* @param  *GPIOx : GPIO Port Base address
	* @param  EnorDi : ENABLE or DISABLE macros
	* @retval None
*/
void SPI_DeInit(SPI_RegDef_t *pSPIx);

/**
	* @fn			 : GPIO_PeriClockControl
	* @brief  		 : This function enables or disables peripheral clock for the given GPIO port
	* @param  *GPIOx : GPIO Port Base address
	* @param  EnorDi : ENABLE or DISABLE macros
	* @retval None
*/
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);

/**
	* @fn			 : GPIO_PeriClockControl
	* @brief  		 : This function enables or disables peripheral clock for the given GPIO port
	* @param  *GPIOx : GPIO Port Base address
	* @param  EnorDi : ENABLE or DISABLE macros
	* @retval None
*/
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

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

