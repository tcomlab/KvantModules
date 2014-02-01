/*
 * AD7792.c
 *
 *  Created on: 30.10.2012
 *      Author: TCom
 */

#include "main.h"
#include "SPI.h"
#include "AD7792.h"


//Начальная инициализация АЦП:
void ADC_Init(void)
{
//Сброс АЦП. 32 клока с Dout=1
	CS_0;
	spi_transaction(0xFF);
	spi_transaction(0xFF);
	spi_transaction(0xFF);
	spi_transaction(0xFF);
	delay_ms(1); //Пауза
	CS_1;
	//while (Din == 1)
	//	; //Ожидание завершения процедуры сброса

	delay_ms(100);

//Настройка внутреностей
//IO REG
	CS_0;
	spi_transaction(0x28); //28h
	CS_1;
	delay_us(5);
	CS_0;
	spi_transaction(0); //Опора отключенна
	CS_1;
}


//Выбор канала АЦП:
void ADC_configure_chanel(unsigned char Ch)
//Ch - номер канала, от 1 до 3 - входы, 4 - внутрений датчик температуры
{
//Config REG
	CS_0;
	spi_transaction(0x10); // 10h
	CS_1;
	delay_us(5);
// ch Ch
// __ * ___
	CS_0;
	if (Ch == 1)
	{
		spi_transaction(0x90);
		spi_transaction(0x4E); //GAIN=64=18.28mV; Vbias=-AN1; ref=internal; buf=on
	}
	if (Ch == 2)
	{
		spi_transaction(0x91);
		spi_transaction(0x4E); //GAIN=64=18.28mV; Vbias=-AN2; ref=internal; buf=on
	}
	if (Ch == 3)
	{
		spi_transaction(0x92);
		spi_transaction(0x4E); //GAIN=64=18.28mV ref=internal; buf=on
	}
	if (Ch == 4)
	{
		spi_transaction(0x96);
		spi_transaction(0x4F); //GAIN=1/6 ref=1.17V
	}
	CS_1;
	delay_us(5);
}

//Самокалибровка:
void ADC_Calibrate(void)
{
//Mode REG
	CS_0;
	spi_transaction(0x08); //08h
	CS_1;
	delay_us(5);
	CS_0;
	spi_transaction(0x0F);
	spi_transaction(0x80); //Калибровка нуля
	CS_1;
	delay_s(1); //while(Din==1); //Ждем завершения самокалибровки
//Mode REG
	CS_0;
	spi_transaction(0x08); //08h
	CS_1;
	delay_us(5);
	CS_0;
	spi_transaction(0x0F);
	spi_transaction(0xA0); //Калибровка шкалы
	CS_1;
	delay_s(1); //while(Din==1); //Ждем завершения самокалибровки
	delay_us(5);
}

//Запуск непрерывного преобразования:
void ADC_Start_Continuous_Conversion_Mode(void)
{
//Mode REG
	CS_0;
	spi_transaction(0x08); //08h
	CS_1;
	delay_us(5);
	CS_0;
	spi_transaction(0xF);
	spi_transaction(0x00); //0009h //Cont. conv.
	CS_1;
	delay_us(5);
}

//Чтения результатов преобразования из АЦП:
int ADC_Read_Data(void)
{
//Ждем флага окончания преобразования
	delay_s(1); //while(Din==1);
//Читаема
//DATA REG
	CS_0;
	spi_transaction(0x58); //58h
	CS_1;
	delay_us(5);
	CS_0;
	int res = (spi_transaction(0x00) << 8) | (spi_transaction(0x00));
	CS_1;
	return res;
}
/*
 Перевод АЦП в ждущий режим:
 void ADC_Shut_Down(void)
 {
	 //Mode REG
	 SendSPI(0b00001000,8); //08h
	 delay_us(5);
	 SendSPI(0b0100000000001111,16); //0009h //Cont. conv.
	 delay_us(5);
 }
 */
/*
Главная функция получения температуры:
void RefreshAllData(void)
{
float T4f;
// Настраиваем канал 1
ADC_configure_chanel(1);
// Делаем самокалибровку
ADC_Calibrate();
// Запускаем на преобразование
ADC_Start_Continuous_Conversion_Mode();
// Делаем паузу на 250 мсек. Мы не спешим, пускай все "устаканится"
delay_ms(250);
// Считываем показания канала 1 - Т1
T1_RAW=ADC_Read_Data()-0x8000;

// Настраиваем канал 2
ADC_configure_chanel(2);
// Делаем самокалибровку
ADC_Calibrate();
// Запускаем на преобразование
ADC_Start_Continuous_Conversion_Mode();
// Делаем паузу на
delay_ms(250);
// Считываем показания канала 2 - Т2
T2_RAW=ADC_Read_Data()-0x8000;

// Настраиваем канал 3
ADC_configure_chanel(3);
// Делаем самокалибровку
ADC_Calibrate();
// Запускаем на преобразование
ADC_Start_Continuous_Conversion_Mode();
// Делаем паузу
delay_ms(250);
// Считываем показания канала 3 - Т3
T3_RAW=ADC_Read_Data()-0x8000;

//Был замечен глюк. Если каждый раз переключатся на измерение температуры через встроенный датчик, то в это время отключается источник подтягивающего напряжения и конденсаторы в цепочке фильтра на входе успевают немного разрядится, что вызывает появление отрицательного напряжения на дифференциальном входе и как следствие - заметного снижения отображаемой температуры. Поэтом внутреннюю температуру измеряет редко.
if (NeedT4refresh++>10)
{
NeedT4refresh=0;
// Настраиваем канал ТЕМПЕРАТУРА
ADC_configure_chanel(4);
// Делаем самокалибровку
ADC_Calibrate(); // ??? нужна ли она?
// Запускаем на преобразование
ADC_Start_Continuous_Conversion_Mode();
// Делаем паузу
delay_ms(255);
// Считываем показания канала ТЕМПЕРАТУРА - Т4
T4_RAW=ADC_Read_Data();

ADC_configure_chanel(1); //После работы со встроенным термодатчиком
delay_s(3); //делаем паузу на восстановление заряда конденсаторов.
}
*/
