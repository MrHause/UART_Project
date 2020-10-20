/*
 * TMP102_stm32.c

 *
 *  Created on: 18.10.2020
 *      Author: mrhause
 */
#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"

static const uint8_t TMP102_ADDR = 0x48<<1;

static void TMP102_gpio_config();
static void TMP102_I2C_config();

void TMP102_init(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	TMP102_gpio_config();
	TMP102_I2C_config();
}

static void TMP102_gpio_config(){
	GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; // SCL, SDA
	gpio.GPIO_Mode = GPIO_Mode_AF_OD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);
}

static void TMP102_I2C_config(){
	I2C_InitTypeDef i2c;
	I2C_StructInit(&i2c);
	i2c.I2C_ClockSpeed = 100000;
	i2c.I2C_DutyCycle = I2C_DutyCycle_2;
	i2c.I2C_OwnAddress1 = 0;
	i2c.I2C_Mode = I2C_Mode_I2C;
	I2C_Init(I2C1, &i2c);
	I2C_Cmd(I2C1, ENABLE);
}

float TMP102_get_temp(){
	float temperature;
	uint16_t val;
	uint8_t buf[12];
	buf[0] = 0x00;

	I2C_GenerateSTART(I2C1, ENABLE);
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);

	I2C_Send7bitAddress(I2C1, TMP102_ADDR, I2C_Direction_Transmitter);
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);

	I2C_SendData(I2C1, buf[0]);
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);
	I2C_GenerateSTOP(I2C1, ENABLE);

	I2C_GenerateSTART(I2C1, ENABLE);
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);

	I2C_AcknowledgeConfig(I2C1, ENABLE);
	I2C_Send7bitAddress(I2C1, TMP102_ADDR, I2C_Direction_Receiver);
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS);

	for (uint8_t i = 0; i < 2; i++) {
		while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);
    	buf[i] = I2C_ReceiveData(I2C1);
    }
	I2C_AcknowledgeConfig(I2C1, DISABLE);
    I2C_GenerateSTOP(I2C1, ENABLE);

	val = ((int16_t)buf[0]<<4)|(buf[1]>>4);
	if(val>0x7FF){
		val |= 0xF000;
	}
	temperature = val * 0.0625;

    return temperature;
}



