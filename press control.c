/*
	code to control a pneumatic press

	runs on a Parallax C3

		 		10/16/2017 	- created mam 
		 		11/8/2017 	- v-4.0 installed 
		 		5/6/2018 	- v-6.0 removed pauses where possible
		 							leds controlled directly by switches

*/

#include "simpletools.h"					// Library include
#include "press control.h"

/* shared memory */
volatile int 		dwell;					// dwell time in seconds, shared between cogs
volatile int 		ram_state;				// 0-RETRACTED, 1-EXTENDED

int main()
{
	int timer;								// accumulator for dwell
	int 		down_switch;				// switch state

/* initializations */ 1
	high(_STATUS_LED_BUS_MUX);				// free up vga io pins */
	up();									// retract ram
	printf("press control version %i.%i starting\n\n", _MAJOR_VERSION_system, _MINOR_VERSION_system);
	printf("start dwell monitor cog\n");
	cog_run(set_dwell, 128);     			// start cog to monitor dwell setting switches
	printf("start switch monitor cog\n");
	cog_run(watch_up_switch, 128);     		// start cog to monitor foot switch input
 	pause(100);
   
/* main loop - watch down switch */
	while (1)								// wait for an extend command
	{
		down_switch = input(_DOWN_SWITCH);  // check down switch
		else if (down_switch)				// test switch
		{
			timer = dwell;
			down();							// extend ram
			while ((timer >= 0) & (ram_state == _EXTENDED))
			{
				pause(1);					// Wait 1 ms
				timer -= 1;					// decrement time count
			}
			if(ram_state == _EXTENDED)
				up();						// retract ram
		}
	}
}

/* retract ram */
void up()
{
	high(_RETRACT_SOLENOID);
	ram_state = _RETRACTED;					// Set I/O pin high
	pause(100);								// Wait
	low(_RETRACT_SOLENOID);					// Set I/O pin low
	return;
}

/* extend ram */
void down()
{
	high(_EXTEND_SOLENOID);					// Set I/O pin high
	ram_state = _EXTENDED;				
	pause(100);								// Wait
	low(_EXTEND_SOLENOID);					// Set I/O pin low
	return;
}

/********************  Cog code *************************************/
/* watch switches */
void watch_up_switch(void)
{
	int 		up_switch;					// switch state
	while (1)
	{
		up_switch = input(_UP_SWITCH);
		if (up_switch)
			if (ram_state == _EXTENDED)
				up();
		pause(10);							// 
	}
}

/* let user set dwell time */
void set_dwell(void)
{
	static int 		tswitch[8] = {_DIP_0, _DIP_1, _DIP_2, _DIP_3, _DIP_4, _DIP_5, _DIP_6, _DIP_7};
	int 			value, i;

	while (1)
	{
		value = 0;
		for (i = 0; i < 8; i++)				// convert binary witch settings to decimal seconds
		{
			if (input(tswitch[i]))	
				value += (int)pow(2, i);				
		}
		dwell = value * 1000;				// convert to milliseconds
		pause(10);							// Wait
	}
}
