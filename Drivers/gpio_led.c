#include "gpio_led.h"


#define GPIO_LED_MODER    (*(volatile uint32_t*)(GPIOI_BASE + 0x00))
#define GPIO_LED_BSRR     (*(volatile uint32_t*)(GPIOI_BASE + 0x18))


#define SYST_CSR          (*(volatile uint32_t *)0xE000E010U)
#define SYST_RVR          (*(volatile uint32_t *)0xE000E014U)
#define SYST_CVR          (*(volatile uint32_t *)0xE000E018U)

#define SYST_ENABLE       (1U << 0)
#define SYST_CLKSOURCE    (1U << 2)      /* 1 = core clock */
#define SYST_COUNTFLAG    (1U << 16)



void GPIO_LED_Init(uint32_t pin)
{
    /* Enable GPIOI peripheral clock */
    RCC_AHB4ENR |= (1U << GPIOI_LED_RCC_BIT);
    (void)RCC_AHB4ENR;

    GPIO_LED_BSRR = (1U << pin);                 /* drive high first = LED off */
    /* Set pin to general-purpose output mode */
    GPIO_LED_MODER &= ~(3U << (pin * 2));
    GPIO_LED_MODER |=  (1U << (pin * 2));
}

void GPIO_LED_On(uint32_t pin)
{
  GPIO_LED_BSRR = (1U << (pin + 16U));
}

void GPIO_LED_Off(uint32_t pin)
{
      GPIO_LED_BSRR = (1U << pin);
}

// void GPIO_Delay(volatile uint32_t count)
// {
//     while (count--) { __asm__("nop"); }
// }

void GPIO_Delay_ms(uint32_t ms)
{
    static uint8_t inited = 0U;

    if (!inited)
    {
        SYST_RVR = (CPU_CLK_HZ / 1000U) - 1U;     /* 1 ms period */
        SYST_CVR = 0U;
        SYST_CSR = SYST_CLKSOURCE | SYST_ENABLE;  /* core clock, polling only */
        inited = 1U;
    }

    (void)SYST_CSR;                               /* reading CSR clears COUNTFLAG */
    while (ms--)
    {
        while ((SYST_CSR & SYST_COUNTFLAG) == 0U) { }
    }
}