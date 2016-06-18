#include "DS18B20.h"

static void OW_DIR_IN(){

	gpio_ds.GPIO_Mode = GPIO_Mode_IN;
	gpio_ds.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio_ds.GPIO_OType = GPIO_OType_OD;
	gpio_ds.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOE, &gpio_ds);

}

static void OW_DIR_OUT(){

	gpio_ds.GPIO_Mode = GPIO_Mode_OUT;
	gpio_ds.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_ds.GPIO_OType = GPIO_OType_PP;
	gpio_ds.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOE, &gpio_ds);

}

//Funkcja resetujaca linie 1 - Wire
static uint8_t ow_reset(void)
{
	GPIO_ResetBits(GPIOE, GPIO_Pin_5);

	OW_DIR_OUT();

	Delay_us(480);

	//GPIO_SetBits(GPIOE, GPIO_Pin_5);

	OW_DIR_IN();

	Delay_us(480);

	return 0;
}

//Funkcja wysy�ajaca bit danych
static uint8_t ow_bit_write(uint8_t b){

	if (b){

		GPIO_ResetBits(GPIOE, GPIO_Pin_5);
		OW_DIR_OUT();
		Delay_us(10);

		GPIO_SetBits(GPIOE, GPIO_Pin_5);

		Delay_us(55);
		OW_DIR_IN();

	} else {

		GPIO_ResetBits(GPIOE, GPIO_Pin_5);
		OW_DIR_OUT();
		Delay_us(65);

		OW_DIR_IN();

		Delay_us(5);
		OW_DIR_IN();
	}

	return b;
}

static uint8_t ow_bit_read(){

	uint8_t b = 0;

	GPIO_ResetBits(GPIOE, GPIO_Pin_5);
	OW_DIR_OUT();
	Delay_us(3);

	OW_DIR_IN();
	Delay_us(10);

	if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5)){

		b = 1;

	}

	Delay_us(50);

	return b;

}

//Funkcja wysy�ajaca bajt danych
static uint8_t ow_byte_wr(uint8_t b)
{
	for(int i=0;i<8;i++)
	{
		ow_bit_write(b & 0x01);
		b >>= 1;

	}
	return b;
}

//Funkcja odczytujaca bajt danych
static uint8_t ow_byte_rd(void)
{
	uint8_t i=8, byte = 0;
	while (i--){
		byte >>= 1;
		byte |= ow_bit_read() << 7;
	}

	return byte;
}

//Funkcja inicjuje tablic� sensor�w ich adresami
void DS18B20_init(sDS18B20 s[])
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	gpio_ds.GPIO_Pin = GPIO_Pin_5;
	gpio_ds.GPIO_Mode = GPIO_Mode_OUT;
	gpio_ds.GPIO_OType = GPIO_OType_PP;
	gpio_ds.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_ds.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOE, &gpio_ds);

	s[0].addr[0]=40;
	s[0].addr[1]=255;
	s[0].addr[2]=123;
	s[0].addr[3]=57;
	s[0].addr[4]=163;
	s[0].addr[5]=21;
	s[0].addr[6]=4;
	s[0].addr[7]=238;
	s[1].addr[0]=40;
	s[1].addr[1]=255;
	s[1].addr[2]=185;
	s[1].addr[3]=26;
	s[1].addr[4]=0;
	s[1].addr[5]=22;
	s[1].addr[6]=1;
	s[1].addr[7]=207;
}

//Funkcja odczytujaca temperature ze wszystkich czujnik�w
uint8_t DS18B20_temp(sDS18B20 s[])
{
	uint16_t temp1, temp2;
	ow_reset();
	ow_byte_wr(0xCC);
	ow_byte_wr(0x44);
	Delay_ms(750);
	for(int i=0;i<SENSNUMBER;i++)
	{
		ow_reset();
		ow_byte_wr(0xCC);
//		for(int j=0;j<8;j++)
//			ow_byte_wr(s[i].addr[j]);
		ow_byte_wr(0xBE);
		temp1 = ow_byte_rd();		//lsb
		temp2 = ow_byte_rd()<<8;	//msb
		ow_reset();
		s[i].temp=(float)(temp1+temp2)/16;
	}

	return 1;

}

void DS18B20_onetemp(sDS18B20 s[], uint8_t num, uint8_t prec)
{
	uint16_t temp1, temp2;
	ow_reset();
	ow_byte_wr(0x55);
	for(int j=0;j<8;j++)
		ow_byte_wr(s[num].addr[j]);
	ow_byte_wr(0x4E);
	ow_byte_wr(0xff);
	ow_byte_wr(0xff);
	switch (prec)
	{
		case 9:
			ow_byte_wr(0x1f);
			break;
		case 10:
			ow_byte_wr(0x3f);
			break;
		case 11:
			ow_byte_wr(0x5f);
			break;
		case 12:
			ow_byte_wr(0x7f);
			break;
		default:
			break;
	}
	ow_reset();
	ow_byte_wr(0x55);
	for(int j=0;j<8;j++)
		ow_byte_wr(s[num].addr[j]);
	ow_byte_wr(0x44);
	switch (prec)
	{
		case 9:
			Delay_ms(750/8);
			break;
		case 10:
			Delay_ms(750/4);
			break;
		case 11:
			Delay_ms(750/2);
			break;
		case 12:
			Delay_ms(750);
			break;
		default:
			break;
	}
	ow_reset();
	ow_byte_wr(0x55);
	for(int j=0;j<8;j++)
		ow_byte_wr(s[num].addr[j]);
	ow_byte_wr(0xBE);
	temp1 = ow_byte_rd();		//lsb
	temp2 = ow_byte_rd()<<8;	//msb
	ow_reset();
	s[num].temp=(temp1+temp2)/16;
}

void DS18B20_readaddress(void)
{
	ow_reset();
	ow_byte_wr(0x33);
	for(int i=0;i<8;i++){

		UART_Puts(itoa(ow_byte_rd(), buff, 10));
		UART_Puts(" ");
	}
	ow_reset();
}


