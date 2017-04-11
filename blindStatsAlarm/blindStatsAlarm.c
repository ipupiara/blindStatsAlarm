/*
 * blindStatsAlarm.c
 *
 * Created: 11.04.2017 14:59:38
 *  Author: mira
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/atomic.h>


#define alarmSecs   30

int8_t  tickCnt;
int32_t  secsCnt;

int8_t alarmOn;


void initBuzzerTimer()
{
	TCCR0A = (1 << COM0B1) | (1 << WGM00);
	TCCR0B = (1 << WGM02);
	OCR0A =  200;
	OCR0B =  100;    // should together give a squarewave of 5k
	TCNT0 = 0;
	PORTB &= ~(1 << PB0);   // initial value 0
	DDRB |= (1 << DDB0);    // set as ouput
}

void startBuzzerTimer()
{
	TCCR0B |=  (1<< CS01) ;    //  prescaler = 8
}

void stopBuzzerTimer()
{
	TCCR0B &= ~((1<< CS00) | (1<< CS01) | (1 << CS02));
}



void toggleBuzzerAlarm()
{
	if (alarmOn == 0) {
		initBuzzerTimer();
		startBuzzerTimer();
	} else {
		stopBuzzerTimer();
	}
	
}


ISR(TIMER1_COMPA_vect)
{
	++ tickCnt;
	if (tickCnt > 1) {
		tickCnt = 0;
		++ secsCnt;
	}
	if (secsCnt > alarmSecs)  {
		toggleBuzzerAlarm();
	}
}




void initHW()
{
	cli();
	
	// start Timer 1 
	
	tickCnt = 0;
	secsCnt = 0;   
	alarmOn = 0;
	TCCR1 = 1 << CTC1;
	OCR1A = 0xF4;  // counter top value means approx   2 per sec
	GTCCR = 0x00;
	TIMSK  = 1 << OCIE1A;  //  interrupt needed 
	TCNT1 = 0x00 ;

	TCCR1 |=  (1 << CS10) |  (1 << CS11) | (1 << CS12) | (1 << CS13) ;      //   16384 prescaler... about 488 Hz at 8Mhz CLK
	sei();	
}




#include <avr/io.h>

int main(void)
{
	initHW();
    while(1)
    {
        //TODO:: Please write your application code 
    }
}