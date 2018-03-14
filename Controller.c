/*
 * finalproject.c
 *
 * Created: 2/25/2018 4:55:20 PM
 * Author : Jon
 */ 

#include <avr/io.h>
#include "usart_ATmega1284.h"
#include "timer.h"
#include "io.c"
#include "nokia5110.h"
#include "nokia5110.c"
#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#define button (~PINA) & 0x2
#define numTasks 1


unsigned int ADCInput = 0;
unsigned int globalMax = 0;
unsigned int globalMin = 1024;
unsigned char On = 0;

enum soundStates {soundStart, Microphone, Aux, Off, Hold} state;

unsigned char x;
unsigned char y;
unsigned char toSend = 0;
unsigned char sendTimer = 0;

task soundTask;


void ADC_init()
{
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}


unsigned int sampleSound(unsigned int ADCInput)
{
	unsigned char sampleTime = 0;
	unsigned int p2p = 0;
	unsigned int  sampleMax = 0; //Initial high is 287 Highest seen is 515
	unsigned int  sampleMin = 1024; //Initial low is 271 Lowest seen is 128
	unsigned char temp[10];
	unsigned char temp2[10];

	while(sampleTime != 50)
	{
		if (ADCInput < 1024)
		{
			p2p += ADC;
			if (ADCInput > globalMax)
			{
				globalMax = ADCInput;
				itoa(globalMax, temp, 10);
				
				nokia_lcd_set_cursor(0,0);
				nokia_lcd_write_string("Max: ",1);
				nokia_lcd_write_string(temp, 1);
				nokia_lcd_render();
				
			}
			if (ADCInput < globalMin)
			{
				globalMin = ADCInput;


				itoa(globalMin, temp2, 10);
				
				nokia_lcd_clear();
				nokia_lcd_set_cursor(0,0);
				nokia_lcd_write_string("Max: ",1);
				nokia_lcd_write_string(temp, 1);
				nokia_lcd_set_cursor(0,20);
				nokia_lcd_write_string("Min: ",1);
				nokia_lcd_write_string(temp2, 1);
				nokia_lcd_render();
				
			}
		}
		sampleTime++;
	}
	p2p = p2p/50;
	return (p2p);
}



unsigned char setLevels(unsigned int input)
{
	unsigned char temp[10];
	unsigned char value = 15;
	
	if (input >= 282)
	{
		value = 14;
	}
	if (input > 286)
	{
		value = 13;
	}
	if (input > 290)
	{
		value = 12;
	}
	if (input > 294)
	{
		value = 11;
	}
	if (input > 298)
	{
		value = 10;
	}
	if (input > 302)
	{
		value = 9;
	}
	if (input > 306)
	{
		value = 8;
	}
	if (input > 310)
	{
		value = 7;
	}
	if (input > 314)
	{
		value = 6;
	}
	if (input > 318)
	{
		value = 5;
	}
	if (input > 322)
	{
		value = 4;
	}
	if (input > 326)
	{
		value = 3;
	}
	if (input > 330)
	{
		value = 2;
	}
	if (input > 334)
	{
		value = 1;
	}
	if (input > 338)
	{
		value = 0;
	}

	return value;
}

void microSend()
{
	if(USART_IsSendReady(0))
	{
		unsigned int sound = sampleSound(ADC);
		toSend = setLevels(sound);
		
		if (ADC < 1024)
		{
			USART_Send(toSend,0);
		}
	}
}
	
void checkInput()
{
	switch (state)
	{
		case soundStart:
			state = Microphone;
			On = 1;
			break;
		case Microphone:
			state = Microphone;
			microSend();
			if (button)
			{
				state = Hold;
			}
			break;
		case Aux:
			state = Microphone;
			break;
		case Off:
			On = 0;
			nokia_lcd_clear();
			nokia_lcd_set_cursor(0,25);
			nokia_lcd_write_string("Off",3);
			nokia_lcd_render();
			globalMin = 1024;
			globalMax = 0;
			if (button)
			{
				state = Hold;
			}
			break;
		case Hold:
			if(button)
			{
				
			}
			else
			{
				if(!On)
				{
					state = soundStart;
				}
				else
				{
					state = Off;
				}
			}
	}
	return;
}


	
char nthdigit(int x, int n)
{
	while (n--) {
		x /= 10;
	}
	return (x % 10) + '0';
}


void auxSend()
{
	LCD_Cursor(1);
}

void sendData()
{
	switch(state)
	{
		case soundStart:
			break;
		case Microphone:
			microSend();
			//auxSend();
			break;
		case Aux:
			auxSend();
			break;
		case Off:
			nokia_lcd_clear();
			nokia_lcd_render();
			globalMin = 1024;
			globalMax = 0;
			break;
		case Hold:
			break;
	}
}

int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFf; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	//DDRC = 0xFF; PORTC = 0x00;
	
	initUSART(0);
	ADC_init();
	TimerSet(10);
	TimerOn();
	nokia_lcd_init();
	nokia_lcd_power(1);
	
	soundTask.state = soundStart;
	soundTask.elapsedTime = 0;
	soundTask.period = 1;
	soundTask.TickFct = &checkInput;
	
	task *tasks[] = {&soundTask};


    while (1) 
    {	
		for (int i = 0; i < numTasks; i++)
		{
			if(tasks[i]->elapsedTime == tasks[i]->period)
			{
				tasks[i]->state = (tasks[i]->TickFct(tasks[i]->state));
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime++;
		}
    }
}

