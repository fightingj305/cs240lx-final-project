
/******************************************************************************/
// Start Script for STM32F411
// Lowkey ripped from the SSCP F429 script
/******************************************************************************/

        MODULE  ?cstartup

/******************************************************************************/
// Stack
/******************************************************************************/

        SECTION CSTACK:DATA:NOROOT(3)

/******************************************************************************/
// Vector table
/******************************************************************************/

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit

        PUBLIC  __vector_table

__vector_table

        DCD     sfe(CSTACK)
        DCD     Reset_Handler

        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     MemManage_Handler
        DCD     BusFault_Handler
        DCD     UsageFault_Handler
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     SVC_Handler
        DCD     DebugMon_Handler
        DCD     0
        DCD     PendSV_Handler
        DCD     SysTick_Handler

        ; External IRQs (STM32F411)
        DCD     WWDG_IRQHandler
        DCD     PVD_IRQHandler
        DCD     TAMP_STAMP_IRQHandler
        DCD     RTC_WKUP_IRQHandler
        DCD     FLASH_IRQHandler
        DCD     RCC_IRQHandler
        DCD     EXTI0_IRQHandler
        DCD     EXTI1_IRQHandler
        DCD     EXTI2_IRQHandler
        DCD     EXTI3_IRQHandler
        DCD     EXTI4_IRQHandler
        DCD     DMA1_Stream0_IRQHandler
        DCD     DMA1_Stream1_IRQHandler
        DCD     DMA1_Stream2_IRQHandler
        DCD     DMA1_Stream3_IRQHandler
        DCD     DMA1_Stream4_IRQHandler
        DCD     DMA1_Stream5_IRQHandler
        DCD     DMA1_Stream6_IRQHandler
        DCD     ADC_IRQHandler
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     EXTI9_5_IRQHandler
        DCD     TIM1_BRK_TIM9_IRQHandler
        DCD     TIM1_UP_TIM10_IRQHandler
        DCD     TIM1_TRG_COM_TIM11_IRQHandler
        DCD     TIM1_CC_IRQHandler
        DCD     TIM2_IRQHandler
        DCD     TIM3_IRQHandler
        DCD     TIM4_IRQHandler
        DCD     I2C1_EV_IRQHandler
        DCD     I2C1_ER_IRQHandler
        DCD     I2C2_EV_IRQHandler
        DCD     I2C2_ER_IRQHandler
        DCD     SPI1_IRQHandler
        DCD     SPI2_IRQHandler
        DCD     USART1_IRQHandler
        DCD     USART2_IRQHandler
        DCD     0
        DCD     EXTI15_10_IRQHandler
        DCD     RTC_Alarm_IRQHandler
        DCD     OTG_FS_WKUP_IRQHandler
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     DMA1_Stream7_IRQHandler
        DCD     0
        DCD     SDIO_IRQHandler
        DCD     TIM5_IRQHandler
        DCD     SPI3_IRQHandler
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     DMA2_Stream0_IRQHandler
        DCD     DMA2_Stream1_IRQHandler
        DCD     DMA2_Stream2_IRQHandler
        DCD     DMA2_Stream3_IRQHandler
        DCD     DMA2_Stream4_IRQHandler
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     OTG_FS_IRQHandler
        DCD     DMA2_Stream5_IRQHandler
        DCD     DMA2_Stream6_IRQHandler
        DCD     DMA2_Stream7_IRQHandler
        DCD     USART6_IRQHandler
        DCD     I2C3_EV_IRQHandler
        DCD     I2C3_ER_IRQHandler
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     FPU_IRQHandler
        DCD     0
        DCD     0
        DCD     SPI4_IRQHandler

/******************************************************************************/
// Reset Handler
/******************************************************************************/

        SECTION .text:CODE:REORDER:NOROOT(2)

        PUBLIC Reset_Handler

Reset_Handler

        BL      SystemInit
        LDR     R0, =__iar_program_start
        BX      R0

/******************************************************************************/
// Core exception handlers
/******************************************************************************/

        PUBWEAK NMI_Handler
NMI_Handler
        B       .

        PUBWEAK HardFault_Handler
HardFault_Handler
        B       .

        PUBWEAK MemManage_Handler
MemManage_Handler
        B       .

        PUBWEAK BusFault_Handler
BusFault_Handler
        B       .

        PUBWEAK UsageFault_Handler
UsageFault_Handler
        B       .

        PUBWEAK SVC_Handler
SVC_Handler
        BX      LR

        PUBWEAK DebugMon_Handler
DebugMon_Handler
        BX      LR

        PUBWEAK PendSV_Handler
PendSV_Handler
        BX      LR

        PUBWEAK SysTick_Handler
SysTick_Handler
        BX      LR

//===============================
// Default IRQ Handlers
//===============================

        PUBWEAK WWDG_IRQHandler
WWDG_IRQHandler
        B       .

        PUBWEAK PVD_IRQHandler
PVD_IRQHandler
        B       .

        PUBWEAK TAMP_STAMP_IRQHandler
TAMP_STAMP_IRQHandler
        B       .

        PUBWEAK RTC_WKUP_IRQHandler
RTC_WKUP_IRQHandler
        B       .

        PUBWEAK FLASH_IRQHandler
FLASH_IRQHandler
        B       .

        PUBWEAK RCC_IRQHandler
RCC_IRQHandler
        B       .

        PUBWEAK EXTI0_IRQHandler
EXTI0_IRQHandler
        B       .

        PUBWEAK EXTI1_IRQHandler
EXTI1_IRQHandler
        B       .

        PUBWEAK EXTI2_IRQHandler
EXTI2_IRQHandler
        B       .

        PUBWEAK EXTI3_IRQHandler
EXTI3_IRQHandler
        B       .

        PUBWEAK EXTI4_IRQHandler
EXTI4_IRQHandler
        B       .

        PUBWEAK DMA1_Stream0_IRQHandler
DMA1_Stream0_IRQHandler
        B       .

        PUBWEAK DMA1_Stream1_IRQHandler
DMA1_Stream1_IRQHandler
        B       .

        PUBWEAK DMA1_Stream2_IRQHandler
DMA1_Stream2_IRQHandler
        B       .

        PUBWEAK DMA1_Stream3_IRQHandler
DMA1_Stream3_IRQHandler
        B       .

        PUBWEAK DMA1_Stream4_IRQHandler
DMA1_Stream4_IRQHandler
        B       .

        PUBWEAK DMA1_Stream5_IRQHandler
DMA1_Stream5_IRQHandler
        B       .

        PUBWEAK DMA1_Stream6_IRQHandler
DMA1_Stream6_IRQHandler
        B       .

        PUBWEAK ADC_IRQHandler
ADC_IRQHandler
        B       .

        PUBWEAK EXTI9_5_IRQHandler
EXTI9_5_IRQHandler
        B       .

        PUBWEAK TIM1_BRK_TIM9_IRQHandler
TIM1_BRK_TIM9_IRQHandler
        B       .

        PUBWEAK TIM1_UP_TIM10_IRQHandler
TIM1_UP_TIM10_IRQHandler
        B       .

        PUBWEAK TIM1_TRG_COM_TIM11_IRQHandler
TIM1_TRG_COM_TIM11_IRQHandler
        B       .

        PUBWEAK TIM1_CC_IRQHandler
TIM1_CC_IRQHandler
        B       .

        PUBWEAK TIM2_IRQHandler
TIM2_IRQHandler
        B       .

        PUBWEAK TIM3_IRQHandler
TIM3_IRQHandler
        B       .

        PUBWEAK TIM4_IRQHandler
TIM4_IRQHandler
        B       .

        PUBWEAK I2C1_EV_IRQHandler
I2C1_EV_IRQHandler
        B       .

        PUBWEAK I2C1_ER_IRQHandler
I2C1_ER_IRQHandler
        B       .

        PUBWEAK I2C2_EV_IRQHandler
I2C2_EV_IRQHandler
        B       .

        PUBWEAK I2C2_ER_IRQHandler
I2C2_ER_IRQHandler
        B       .

        PUBWEAK SPI1_IRQHandler
SPI1_IRQHandler
        B       .

        PUBWEAK SPI2_IRQHandler
SPI2_IRQHandler
        B       .

        PUBWEAK USART1_IRQHandler
USART1_IRQHandler
        B       .

        PUBWEAK USART2_IRQHandler
USART2_IRQHandler
        B       .

        PUBWEAK EXTI15_10_IRQHandler
EXTI15_10_IRQHandler
        B       .

        PUBWEAK RTC_Alarm_IRQHandler
RTC_Alarm_IRQHandler
        B       .

        PUBWEAK OTG_FS_WKUP_IRQHandler
OTG_FS_WKUP_IRQHandler
        B       .

        PUBWEAK DMA1_Stream7_IRQHandler
DMA1_Stream7_IRQHandler
        B       .

        PUBWEAK SDIO_IRQHandler
SDIO_IRQHandler
        B       .

        PUBWEAK TIM5_IRQHandler
TIM5_IRQHandler
        B       .

        PUBWEAK SPI3_IRQHandler
SPI3_IRQHandler
        B       .

        PUBWEAK DMA2_Stream0_IRQHandler
DMA2_Stream0_IRQHandler
        B       .

        PUBWEAK DMA2_Stream1_IRQHandler
DMA2_Stream1_IRQHandler
        B       .

        PUBWEAK DMA2_Stream2_IRQHandler
DMA2_Stream2_IRQHandler
        B       .

        PUBWEAK DMA2_Stream3_IRQHandler
DMA2_Stream3_IRQHandler
        B       .

        PUBWEAK DMA2_Stream4_IRQHandler
DMA2_Stream4_IRQHandler
        B       .

        PUBWEAK OTG_FS_IRQHandler
OTG_FS_IRQHandler
        B       .

        PUBWEAK DMA2_Stream5_IRQHandler
DMA2_Stream5_IRQHandler
        B       .

        PUBWEAK DMA2_Stream6_IRQHandler
DMA2_Stream6_IRQHandler
        B       .

        PUBWEAK DMA2_Stream7_IRQHandler
DMA2_Stream7_IRQHandler
        B       .

        PUBWEAK USART6_IRQHandler
USART6_IRQHandler
        B       .

        PUBWEAK I2C3_EV_IRQHandler
I2C3_EV_IRQHandler
        B       .

        PUBWEAK I2C3_ER_IRQHandler
I2C3_ER_IRQHandler
        B       .

        PUBWEAK FPU_IRQHandler
FPU_IRQHandler
        B       .

        PUBWEAK SPI4_IRQHandler
SPI4_IRQHandler
        B       .

/******************************************************************************/

        END