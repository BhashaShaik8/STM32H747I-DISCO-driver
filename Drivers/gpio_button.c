#include "gpio.h"

#define BTN_MODER       (*(volatile uint32_t*)(GPIOC_BASE + 0x00))
#define BTN_PUPDR       (*(volatile uint32_t*)(GPIOC_BASE + 0x0C))
#define BTN_IDR         (*(volatile uint32_t*)(GPIOC_BASE + 0x10))

/* ---- Board settings: CHECK against the schematic ---- */
#define BTN_PIN         13U                /* PC13 */
#define BTN_ACTIVE_LVL  1U                 /* level read when pressed: 1 = high, 0 = low */

void GPIO_Button_Init(void)
{
    /* 1. Power on port C */
    RCC_AHB4ENR |= (1U << GPIOC_BTN_RCC_BIT);
    (void)RCC_AHB4ENR;

    /* 2. Pin mode = input (00) */
    BTN_MODER &= ~(3U << (BTN_PIN * 2U));

    /* 3. No internal pull-up/pull-down (00) */
    BTN_PUPDR &= ~(3U << (BTN_PIN * 2U));
}

uint32_t GPIO_Button_Read(void)
{
    uint32_t level = (BTN_IDR >> BTN_PIN) & 1U;   /* read the pin: 0 or 1 */

    return (level == BTN_ACTIVE_LVL) ? 1U : 0U;
}