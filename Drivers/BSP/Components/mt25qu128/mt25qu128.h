/**
  ******************************************************************************
  * @file    MT25QU128.h
  * @author  MCD Application Team
  * @brief   This file contains all the description of the MT25QU128 QSPI memory.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MT25QU128_H
#define __MT25QU128_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @addtogroup MT25QU128
  * @{
  */

/** @defgroup MT25QU128_Exported_Types
  * @{
  */
   
/**
  * @}
  */ 

/** @defgroup MT25QU128_Exported_Constants
  * @{
  */
   
/** 
  * @brief  MT25QU128 Configuration  
  */  
#define MT25QU128_FLASH_SIZE                0x1000000  /* 128 MBits => 16MBytes */
#define MT25QU128_BLOCK_SIZE                0x10000   /* 255 blocks of 64KBytes */
#define MT25QU128_SUBBLOCK_SIZE             0x8000    /* 512 blocks of 32KBytes */
#define MT25QU128_SECTOR_SIZE               0x1000    /* 4096 sectors of 4kBytes */
#define MT25QU128_PAGE_SIZE                 0x100     /* 32768 pages of 256 bytes */

#define MT25QU128_DUMMY_CYCLES_READ         8
#define MT25QU128_DUMMY_CYCLES_READ_QUAD    4
#define MT25QU128_DUMMY_CYCLES_2READ        2
#define MT25QU128_DUMMY_CYCLES_4READ        4

#define MT25QU128_ALT_BYTES_PE_MODE         0xA5
#define MT25QU128_ALT_BYTES_NO_PE_MODE      0xAA

#define MT25QU128_CHIP_ERASE_MAX_TIME       240000
#define MT25QU128_BLOCK_ERASE_MAX_TIME      3500
#define MT25QU128_SUBBLOCK_ERASE_MAX_TIME   3000
#define MT25QU128_SECTOR_ERASE_MAX_TIME     240

/** 
  * @brief  MT25QU128 Commands  
  */  
/* Read Operations */
#define READ_CMD                             0x03
#define FAST_READ_CMD                        0x0B
#define QUAD_OUT_READ_CMD                    0x6B
#define QUAD_INOUT_READ_CMD                  0xEB

/* Program Operations */
#define PAGE_PROG_CMD                        0x02
#define QUAD_PAGE_PROG_CMD                   0x38

/* Erase Operations */
#define SECTOR_ERASE_CMD                     0x20
#define SUBBLOCK_ERASE_CMD                   0x52
#define BLOCK_ERASE_CMD                      0xD8
#define CHIP_ERASE_CMD                       0x60
#define CHIP_ERASE_CMD_2                     0xC7

#define PROG_ERASE_RESUME_CMD                0x7A
#define PROG_ERASE_RESUME_CMD_2              0x30
#define PROG_ERASE_SUSPEND_CMD               0x75
#define PROG_ERASE_SUSPEND_CMD_2             0xB0

/* Identification Operations */
#define READ_ID_CMD                          0x9F
#define READ_ELECTRONIC_ID_CMD               0xAB
#define READ_ELEC_MANUFACTURER_DEVICE_ID_CMD 0x90
#define READ_SERIAL_FLASH_DISCO_PARAM_CMD    0x5A

/* Write Operations */
#define WRITE_ENABLE_CMD                     0x06
#define WRITE_DISABLE_CMD                    0x04

/* Register Operations */
#define READ_STATUS_REG_CMD                  0x05
#define READ_CFG_REG_CMD                     0x15
#define WRITE_STATUS_CFG_REG_CMD             0x01

#define READ_SEC_REG_CMD                     0x2B
#define WRITE_SEC_REG_CMD                    0x2F

/* Power Down Operations */
#define DEEP_POWER_DOWN_CMD                  0xB9

/* Burst Operations */
#define SET_BURST_LENGTH_CMD                 0xC0

/* One-Time Programmable Operations */
#define ENTER_SECURED_OTP_CMD                0xB1
#define EXIT_SECURED_OTP_CMD                 0xC1

/* No Operation */
#define NO_OPERATION_CMD                     0x00

/* Reset Operations */
#define RESET_ENABLE_CMD                     0x66
#define RESET_MEMORY_CMD                     0x99
#define RELEASE_READ_ENHANCED_CMD            0xFF

/** 
  * @brief  MT25QU128 Registers  
  */ 
/* Status Register */
#define MT25QU128_SR_WIP                    ((uint8_t)0x01)    /*!< Write in progress */
#define MT25QU128_SR_WEL                    ((uint8_t)0x02)    /*!< Write enable latch */
#define MT25QU128_SR_BP                     ((uint8_t)0x3C)    /*!< Block protect */
#define MT25QU128_SR_QE                     ((uint8_t)0x40)    /*!< Quad enable */
#define MT25QU128_SR_SRWD                   ((uint8_t)0x80)    /*!< Status register write disable */

/* Configuration Register 1 */
#define MT25QU128_CR1_TB                    ((uint8_t)0x08)    /*!< Top / bottom */

/* Configuration Register 2 */
#define MT25QU128_CR2_LH_SWITCH             ((uint8_t)0x02)    /*!< Low power / high performance switch */

/* Security Register */
#define MT25QU128_SECR_SOI                  ((uint8_t)0x01)    /*!< Secured OTP indicator */
#define MT25QU128_SECR_LDSO                 ((uint8_t)0x02)    /*!< Lock-down secured OTP */
#define MT25QU128_SECR_PSB                  ((uint8_t)0x04)    /*!< Program suspend bit */
#define MT25QU128_SECR_ESB                  ((uint8_t)0x08)    /*!< Erase suspend bit */
#define MT25QU128_SECR_P_FAIL               ((uint8_t)0x20)    /*!< Program fail flag */
#define MT25QU128_SECR_E_FAIL               ((uint8_t)0x40)    /*!< Erase fail flag */

/**
  * @}
  */
  
/** @defgroup MT25QU128_Exported_Functions
  * @{
  */ 
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __MT25QU128_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
