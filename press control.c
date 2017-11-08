/*
	code to control a pneumatic press

	runs on a Parallax C3

		 		created - mam 10/16/2017
*/

#include "simpletools.h"					// Library include
#include "press control.h"


/* shared memory */
volatile int 		dwell;					// dwell time in seconds, shared between cogs
volatile int 		ram_state;				// 0-RETRACTED, 1-EXTENDED

int main()
{
	int timer;								// accumulator for dwell
	int 		down_switch;			// switch state

/* initializations */
	pause(200);								// wait 1 second
	printf("press control version %i.%i starting\n\n", _MAJOR_VERSION_system, _MINOR_VERSION_system);
	up();
	printf("start switch monitor cog\n");
	cog_run(watch_up_switch, 128);     		// start cog to monitor swiwtch input
	printf("start dewll monitor cog\n");
	cog_run(set_dwell, 128);     			// start cog to monitor DIP input

/* main loop - watch down switch */
	while (1)								// wait for an extend command
	{
		down_switch = input(_DOWN_SWITCH);
		if (down_switch)					// test switch
		{
			timer = dwell;
			down();							// extend ram
			while ((timer >= 0) & (ram_state == _EXTENDED))
			{
				pause(100);				// Wait 0.1 second
				timer -= 1;			// decrement time count
			}
			if(ram_state == _EXTENDED)
				up();							// retract ram
			pause(100);
		}
		pause(100);							// Wait 0.1 second before repeat
	}
}

/* retract ram */
void up()
{
	printf("<<--- ram retract\n");
	high(_RETRACT_SOLENOID);				// Set I/O pin high
	pause(100);								 // Wait 1/10 second
	low(_RETRACT_SOLENOID);					// Set I/O pin low
	ram_state = _RETRACTED;
	return;
}

/* extend ram */
void down()
{
	printf("--->> ram extend dwell %i\n", dwell);
	high(_EXTEND_SOLENOID);					// Set I/O pin high
	pause(100);								// Wait 1/10 second
	low(_EXTEND_SOLENOID);					// Set I/O pin low
	ram_state = _EXTENDED;
	return;
}

/* watch switches */
void watch_up_switch(void)
{
	int 		up_switch;				// switch state
	while (1)
	{
		up_switch = input(_UP_SWITCH);
		if (up_switch)
			if (ram_state == _EXTENDED)
				up();
		pause(100);						// Wait .1 second
	}
}

/* let user set a new dwell time */
void set_dwell(void)
{
	int 			dip_switch[8] = {_DIP_0, _DIP_1, _DIP_2, _DIP_3, _DIP_4, _DIP_5, _DIP_6, _DIP_7};
	int 			value, i;

	while (1)
	{
		value = 0;
		for (i = 0; i < 8; i++)				// convert DIP switch setting to decimal number
		{
			if (input(dip_switch[i]))	
				value += (int)pow(2, i);
		}
		dwell = value * 10;						// set value of dwell, shared between cogs
		pause(100);						// Wait .1 second
	}
}





