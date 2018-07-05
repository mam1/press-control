p/*
	code to control a pneumatic press

	runs on a Parallax C3

	10/16/2017 	created mam
	11/8/2017 	v-4.0 installed
	5/6/2018 	v-6.0 removed pauses where possible leds controlled directly by switches
    5/11/2018 	v-6.1 improve response to up swithch
    5/12/2018   v-6.5 add locks for shared memory
    5/13/2018   v-6.6 revers high order bit on binary input
    6/5/2018	v-6.7 add debounce code

*/

#include "simpletools.h"		// Library include
#include "press control.h"

/* shared memory */
static volatile int 		dwell;		// dwell time, shared between cogs
static volatile int 		ram_state;	// 0-RETRACTED, 1-EXTENDED
static volatile int   		lock_dwell, lock_ram_state;
static volatile int   		dswitch, uswitch;

int main()
{
	int timer;							// accumulator for dwell
	int 		*cog1;
	int 		*cog2;
	int 		*cog3;
	int 		*cog4;

	/* initializations */
	high(_STATUS_LED_BUS_MUX);	// free up vga io pins */
	up();						// retract ram
	lock_dwell = 0;
	pause(100);
	printf("press control version %i.%i starting\n\n", _MAJOR_VERSION_system, _MINOR_VERSION_system);

	cog1 = cog_run(watch_up_switch, 128);	// start cog to monitor up switch
	cog2 = cog_run(watch_down_switch, 128);	// start cog to monitor ddown switch
	cog3 = cog_run(set_dwell, 128);			// start cog to monitor dwell setting switches
	pause(100);

	/* main loop */
	while (1)								
		if (uswitch)				// test up switch
			up();					// retract ram
		else if (dswitch)			// test down switch
		{
			while (lock_dwell); 	// wait for lock to free
			timer = dwell;			// set dwell time
			down();					// extend ram
			while (timer > 0)
			{
				pause(100);			// Wait
				timer -= 1;			// decrement time count
				if (uswitch)
					timer = 0;
			}
			up();							// retract ram
		}
		pause(100);
	}
}

/* retract ram */
void up()
{

	high(_RETRACT_SOLENOID);				// Set I/O pin high
	pause(_SPLUSE);							// Wait
	low(_RETRACT_SOLENOID);					// Set I/O pin low
	return;
}

/* extend ram */
void down()
{
	high(_EXTEND_SOLENOID);					// Set I/O pin high
	pause(_SPLUSE);							// wait
	low(_EXTEND_SOLENOID);					// Set I/O pin low
	return;
}

/********************  Cog code *************************************/

/* watch up switch */
void watch_up_switch(void)
{
	int 		up_switch;				// current switch state
	int 		pup_switch;				// previous switch state
	int 		acc;					// bounce accumulator

	while (1)
	{
		up_switch = input(_UP_SWITCH);	// read up switch
		if(up_switch == pup_switch)		// no change in state
			acc += 1;
		else
		{
			acc -= 1;
			pup_switch = up_switch;
		}

		if(acc > _BOUNCE)
		{
			uswitch = TRUE;
			acc = 0;
		}
		else
			uswitch = FALSE;
	}
}

/* watch down switch */
void watch_down_switch(void)
{
	int 		down_switch;			// current switch state
	int 		pdown_switch;			// previous switch state
	int 		acc;					// bounce accumulator

	while (1)
	{
		down_switch = input(_DOWN_SWITCH);	// read down switch
		if(down_switch == pdown_switch)
			acc += 1;
		else
		{
			acc -= 1;
			pdown_switch = down_switch;
		}

		if(acc > _BOUNCE)
		{
			dswitch = TRUE;
			acc = 0;
		}
		else
			dswitch = FALSE;
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