#ifndef GPIO_H
#define GPIO_H
#include <stdint.h>

#define CPU_CLK_HZ        64000000U      /* H747 default after reset: HSI 64 MHz. Change if you set up a PLL. */

/* GPIO base address (AHB4 bus) */
#define RCC_AHB4ENR (*(volatile uint32_t *)0x580244E0UL)
 
/* GPIOC base address (AHB4 bus) */
#define GPIOC_BASE      0x58020800UL
#define GPIOI_BASE      0x58022000UL


/* RCC AHB4ENR bit position for GPIOC */ /* GPIOC enable bit (A=0, B=1, C=2) */
#define GPIOC_BTN_RCC_BIT 2U 
#define GPIOI_LED_RCC_BIT 8U



/*BUTTON*/
void GPIO_Button_Init(void);
uint32_t GPIO_Button_Read(void);

/*LED*/
/* LED pin definitions (per UM2411 / BSP header, GPIOI) */
#define LED_GREEN_PIN   12U
#define LED_ORANGE_PIN  13U
#define LED_RED_PIN     14U
#define LED_BLUE_PIN    15U
void GPIO_LED_Init(uint32_t pin);
void GPIO_LED_On(uint32_t pin);
void GPIO_LED_Off(uint32_t pin);
void GPIO_Delay_ms(uint32_t ms);

/* USART */
void     USART_Init(uint32_t baud);
void     USART_Write_Char(char c);
void     USART_Write_String(const char *s);
uint32_t USART_Rx_Ready(void);      /* 1 = a byte is waiting */
char     USART_Read_Char(void);     /* waits until a byte arrives */


#endif /* GPIO_H */