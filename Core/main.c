#include "gpio.h"

void check_led(void) 
{
    GPIO_LED_Init(LED_GREEN_PIN);
    while (1)
    {
        GPIO_LED_On(LED_GREEN_PIN);
        GPIO_Delay_ms(500U);
        GPIO_LED_Off(LED_GREEN_PIN);
        GPIO_Delay_ms(500U);
    }
}

void check_button(void) 
{
  GPIO_LED_Init(LED_BLUE_PIN);
   GPIO_LED_Init(LED_RED_PIN);
    GPIO_Button_Init();
    while (1)
    {
        if (GPIO_Button_Read())
        {
          GPIO_LED_On(LED_BLUE_PIN);
          GPIO_LED_Off(LED_RED_PIN);
        }
        else
        {
          GPIO_LED_Off(LED_BLUE_PIN);
          GPIO_LED_On(LED_RED_PIN);
        }
    }
}
void check_usart()
{
    GPIO_LED_Init(LED_BLUE_PIN);
    USART_Init(115200U);

    USART_Write_String("Hello from STM32H747I\n");

    while (1)
    {
        if (USART_Rx_Ready())
        {
            char c = USART_Read_Char();
            USART_Write_Char(c);              /* echo back */
            GPIO_LED_On(LED_BLUE_PIN);
            GPIO_Delay_ms(50);
            GPIO_LED_Off(LED_BLUE_PIN);
        }
    }
}
int main(void) {

  //check_led();
  //check_button();
  check_usart();
}