/*
 * AD7792.c
 *
 *  Created on: 30.10.2012
 *      Author: TCom
 */

#include "main.h"
#include "SPI.h"
#include "AD7792.h"


//��������� ������������� ���:
void ADC_Init(void)
{
//����� ���. 32 ����� � Dout=1
	CS_0;
	spi_transaction(0xFF);
	spi_transaction(0xFF);
	spi_transaction(0xFF);
	spi_transaction(0xFF);
	delay_ms(1); //�����
	CS_1;
	//while (Din == 1)
	//	; //�������� ���������� ��������� ������

	delay_ms(100);

//��������� ������������
//IO REG
	CS_0;
	spi_transaction(0x28); //28h
	CS_1;
	delay_us(5);
	CS_0;
	spi_transaction(0); //����� ����������
	CS_1;
}


//����� ������ ���:
void ADC_configure_chanel(unsigned char Ch)
//Ch - ����� ������, �� 1 �� 3 - �����, 4 - ��������� ������ �����������
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

//��������������:
void ADC_Calibrate(void)
{
//Mode REG
	CS_0;
	spi_transaction(0x08); //08h
	CS_1;
	delay_us(5);
	CS_0;
	spi_transaction(0x0F);
	spi_transaction(0x80); //���������� ����
	CS_1;
	delay_s(1); //while(Din==1); //���� ���������� ��������������
//Mode REG
	CS_0;
	spi_transaction(0x08); //08h
	CS_1;
	delay_us(5);
	CS_0;
	spi_transaction(0x0F);
	spi_transaction(0xA0); //���������� �����
	CS_1;
	delay_s(1); //while(Din==1); //���� ���������� ��������������
	delay_us(5);
}

//������ ������������ ��������������:
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

//������ ����������� �������������� �� ���:
int ADC_Read_Data(void)
{
//���� ����� ��������� ��������������
	delay_s(1); //while(Din==1);
//�������
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
 ������� ��� � ������ �����:
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
������� ������� ��������� �����������:
void RefreshAllData(void)
{
float T4f;
// ����������� ����� 1
ADC_configure_chanel(1);
// ������ ��������������
ADC_Calibrate();
// ��������� �� ��������������
ADC_Start_Continuous_Conversion_Mode();
// ������ ����� �� 250 ����. �� �� ������, ������ ��� "�����������"
delay_ms(250);
// ��������� ��������� ������ 1 - �1
T1_RAW=ADC_Read_Data()-0x8000;

// ����������� ����� 2
ADC_configure_chanel(2);
// ������ ��������������
ADC_Calibrate();
// ��������� �� ��������������
ADC_Start_Continuous_Conversion_Mode();
// ������ ����� ��
delay_ms(250);
// ��������� ��������� ������ 2 - �2
T2_RAW=ADC_Read_Data()-0x8000;

// ����������� ����� 3
ADC_configure_chanel(3);
// ������ ��������������
ADC_Calibrate();
// ��������� �� ��������������
ADC_Start_Continuous_Conversion_Mode();
// ������ �����
delay_ms(250);
// ��������� ��������� ������ 3 - �3
T3_RAW=ADC_Read_Data()-0x8000;

//��� ������� ����. ���� ������ ��� ������������ �� ��������� ����������� ����� ���������� ������, �� � ��� ����� ����������� �������� �������������� ���������� � ������������ � ������� ������� �� ����� �������� ������� ����������, ��� �������� ��������� �������������� ���������� �� ���������������� ����� � ��� ��������� - ��������� �������� ������������ �����������. ������ ���������� ����������� �������� �����.
if (NeedT4refresh++>10)
{
NeedT4refresh=0;
// ����������� ����� �����������
ADC_configure_chanel(4);
// ������ ��������������
ADC_Calibrate(); // ??? ����� �� ���?
// ��������� �� ��������������
ADC_Start_Continuous_Conversion_Mode();
// ������ �����
delay_ms(255);
// ��������� ��������� ������ ����������� - �4
T4_RAW=ADC_Read_Data();

ADC_configure_chanel(1); //����� ������ �� ���������� �������������
delay_s(3); //������ ����� �� �������������� ������ �������������.
}
*/
