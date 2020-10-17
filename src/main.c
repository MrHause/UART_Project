/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"
#include "UART_stm32.h"
#include "ring_buffer.h"
#include "commands.h"
#include "ADC_stm32.h"

volatile uint32_t timer_ms = 0;
volatile uint8_t command_end = 0;

Ring_buffer_type RX_Buffer;

static void GPIO_Config();
static void GPIO_Config2();
static void set_pin(uint8_t state);

void SysTick_Handler()
{
    if (timer_ms) {
        timer_ms--;
    }
}

void delay_ms(int time)
{
    timer_ms = time;
    while(timer_ms > 0){};
}

			
int main(void)
{
	GPIO_Config();
	ADC_Config();

	SysTick_Config( SystemCoreClock / 1000 );

	UART_Config( USART2, GPIOA, GPIO_Pin_2, GPIO_Pin_3 ); //Configure USART2
	UART_Config_receive_IRQ(USART2, USART2_IRQn);		//enable receive interrupt

	Ring_buffer_init(&RX_Buffer, 32);

	//char *str = "witam Pana \r\n";
	//char element;

	while(1){
		/*
		//it reads when sth is in the buff and echo it
		if( !Ring_buffer_is_sth_to_read( &RX_Buffer ) ){
			element = Ring_buffer_get_item( &RX_Buffer);
			send_char( USART2, element );
		}
		*/

		if( command_end == 1 && !Ring_buffer_is_sth_to_read( &RX_Buffer ) ){
			Commands_list command = command_analyse( &RX_Buffer );
			command_execute( command );
			command_end = 0;
		}

		//send_string( USART2, str );
		//GPIO_SetBits(GPIOA, GPIO_Pin_5);
		//delay_ms(1000);
		//GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		//delay_ms(1000);
	}
}
void USART2_IRQHandler(void)
{
    /* RXNE handler */
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
    	char sign = USART_ReceiveData(USART2);
    	if( sign == 0x0A || sign == 0x0D )
    		command_end = 1;

    	Ring_buffer_add_item(&RX_Buffer, sign);
    	USART_ClearITPendingBit(USART2, USART_IT_RXNE); //reset interupt flag
    }

    /* ------------------------------------------------------------ */
    /* Other USART1 interrupts handler can go here ...             */
}

static void GPIO_Config(){
	GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE );
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_5;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &gpio);
}

static void GPIO_Config2(){
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRL |= ( 0x03UL << 20);
}
static void set_pin(uint8_t state){
	if( state )
		GPIOA->BSRR=(1<<5);
	else
		GPIOA->BSRR=( (1<<5) << 16 );
}
