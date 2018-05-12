/*
	code to control a pneumatic press

	runs on a Parallax C3

	10/16/2017 	created mam
	11/8/2017 	v-4.0 installed
	5/6/2018 	v-6.0 removed pauses where possible leds controlled directly by switches
    5/11/2018 	v-6.1 improve response to up swithch
    5/12/2018   v-6.5 add locks for shared memory

*/

#include "simpletools.h"		// Library include
#include "press control.h"

/* shared memory */
static volatile int 		dwell;		// dwell time, shared between cogs
static volatile int 		ram_state;	// 0-RETRACTED, 1-EXTENDED
static volatile int   lock_dwell, lock_ram_state;

int main()
{
	int timer;							// accumulator for dwell
	int 		down_switch;			// switch state
	int 		*cog1;
	int 		*cog2;

	/* initializations */
	high(_STATUS_LED_BUS_MUX);	// free up vga io pins */
	up();						// retract ram
	lock_dwell = 0;
	lock_ram_state = 0;
	pause(100);
	printf("press control version %i.%i starting\n\n", _MAJOR_VERSION_system, _MINOR_VERSION_system);
	printf("start dwell monitor cog\n");
	cog1 = cog_run(set_dwell, 128);			// start cog to monitor dwell setting switches
	pause(100);				     			
	printf("dwell set to %d seconds\n", dwell);
	printf("start switch monitor cog\n");
	cog2 = cog_run(watch_up_switch, 128);   // start cog to monitor up switch input

	/* main loop - watch down switch */
	while (1)								// wait for an extend command
	{
		down_switch = input(_DOWN_SWITCH);  // check down switch
		if (down_switch)					// test switch
		{
			while (lock_dwell); 			// wait for lock to free
			timer = dwell;
			while (lock_ram_state); 		// wait for lock to free
			down();							// extend ram
			while (timer >= 0)
			{
				pause(100);					// Wait
				timer -= 1;					// decrement time count
				if (ram_state == _RETRACTED)
					timer = -1;
			}
			up();							// retract ram
		}
		pause(100);
	}
}

/* retract ram */
void up()
{
	if (ram_state == _RETRACTED)
		return;
	ram_state = _RETRACTED;
	high(_RETRACT_SOLENOID);				// Set I/O pin high
	pause(_SPLUSE);							// Wait
	low(_RETRACT_SOLENOID);					// Set I/O pin low
	return;
}

/* extend ram */
void down()
{
	if (ram_state == _RETRACTED)
	{
		ram_state = _EXTENDED;
		high(_EXTEND_SOLENOID);					// Set I/O pin high
		pause(_SPLUSE);							// wait
		low(_EXTEND_SOLENOID);					// Set I/O pin low
	}
	return;
}

/********************  Cog code *************************************/
/* watch switches */
void watch_up_switch(void)
{
	int 		up_switch;				// switch state
	while (1)
	{
		up_switch = input(_UP_SWITCH);	// read up switch
		if (up_switch)
		{
			lock_ram_state = 1;			// set memory lock
			high(_RETRACT_SOLENOID);	// Set I/O pin high
			pause(_SPLUSE);				// Wait
			low(_RETRACT_SOLENOID);		// Set I/O pin low
			pause(500);
			ram_state = _RETRACTED;
			lock_ram_state = 0;			// free memory lock
		}
		pause(100);						// wait
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
		lock_dwell = 1;				//set memory lock
		for (i = 0; i < 8; i++)		// convert binary witch settings to decimal seconds
			if (input(tswitch[i]))
				value += (int)pow(2, i);
		dwell = value * 10;			// convert to .1 seconds
		lock_dwell = 0;				// free memory lock
		pause(100);					// Wait
	}
}