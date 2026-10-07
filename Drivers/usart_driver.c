#include "gpio.h"

/* ---- Clocks (RCC) ---- */
#define RCC_AHB4ENR      (*(volatile uint32_t*)0x580244E0UL)
#define RCC_APB2ENR      (*(volatile uint32_t*)0x580244F0UL)
#define GPIOA_RCC_BIT    0U          /* GPIOA enable bit in AHB4ENR */
#define USART1_RCC_BIT   4U          /* USART1 enable bit in APB2ENR */

/* ---- GPIOA registers (PA9 = TX, PA10 = RX) ---- */
#define GPIOA_BASE       0x58020000UL
#define GA_MODER         (*(volatile uint32_t*)(GPIOA_BASE + 0x00))
#define GA_PUPDR         (*(volatile uint32_t*)(GPIOA_BASE + 0x0C))
#define GA_AFRH          (*(volatile uint32_t*)(GPIOA_BASE + 0x24))  /* pins 8..15 */
#define TX_PIN           9U
#define RX_PIN           10U
#define UART_AF          7U          /* AF7 = USART1 */

/* ---- USART1 registers ---- */
#define USART1_BASE      0x40011000UL
#define U_CR1            (*(volatile uint32_t*)(USART1_BASE + 0x00))
#define U_BRR            (*(volatile uint32_t*)(USART1_BASE + 0x0C))
#define U_ISR            (*(volatile uint32_t*)(USART1_BASE + 0x1C))
#define U_RDR            (*(volatile uint32_t*)(USART1_BASE + 0x24))
#define U_TDR            (*(volatile uint32_t*)(USART1_BASE + 0x28))

#define CR1_UE           (1U << 0)   /* USART enable */
#define CR1_RE           (1U << 2)   /* receiver enable */
#define CR1_TE           (1U << 3)   /* transmitter enable */
#define ISR_RXNE         (1U << 5)   /* receive data ready */
#define ISR_TXE          (1U << 7)   /* transmit register empty */

#define USART_CLK_HZ     64000000U   /* assumed USART1 clock after reset: 64 MHz. Verify. */

void USART_Init(uint32_t baud)
{
    /* 1. Power on GPIOA and USART1 */
    RCC_AHB4ENR |= (1U << GPIOA_RCC_BIT);
    RCC_APB2ENR |= (1U << USART1_RCC_BIT);
    (void)RCC_APB2ENR;                               /* short wait */

    /* 2. PA9, PA10 -> alternate function mode (10) */
    GA_MODER &= ~((3U << (TX_PIN * 2U)) | (3U << (RX_PIN * 2U)));
    GA_MODER |=  ((2U << (TX_PIN * 2U)) | (2U << (RX_PIN * 2U)));

    /* 3. Choose AF7 for both pins (4 bits per pin, pin 8 starts at bit 0) */
    GA_AFRH &= ~((0xFU << ((TX_PIN - 8U) * 4U)) | (0xFU << ((RX_PIN - 8U) * 4U)));
    GA_AFRH |=  ((UART_AF << ((TX_PIN - 8U) * 4U)) | (UART_AF << ((RX_PIN - 8U) * 4U)));

    /* 4. Pull-up on RX so the line does not float (01) */
    GA_PUPDR &= ~(3U << (RX_PIN * 2U));
    GA_PUPDR |=  (1U << (RX_PIN * 2U));

    /* 5. Set up the USART: 8 data bits, no parity, 1 stop bit (all defaults) */
    U_CR1 = 0U;                                      /* off while configuring */
    U_BRR = (USART_CLK_HZ + (baud / 2U)) / baud;     /* baud rate divider, rounded */
    U_CR1 = CR1_TE | CR1_RE | CR1_UE;                /* TX on, RX on, USART on */
}

void USART_Write_Char(char c)
{
    while ((U_ISR & ISR_TXE) == 0U) { }              /* wait until TDR is free */
    U_TDR = (uint32_t)(uint8_t)c;
}

void USART_Write_String(const char *s)
{
    while (*s != '\0')
    {
        if (*s == '\n')
        {
            USART_Write_Char('\r');                  /* terminals need CR + LF */
        }
        USART_Write_Char(*s);
        s++;
    }
}

uint32_t USART_Rx_Ready(void)
{
    return ((U_ISR & ISR_RXNE) != 0U) ? 1U : 0U;
}

char USART_Read_Char(void)
{
    while ((U_ISR & ISR_RXNE) == 0U) { }             /* wait for a byte */
    return (char)(U_RDR & 0xFFU);                    /* reading RDR clears RXNE */
}