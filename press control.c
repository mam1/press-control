/*
	code to control a pneumatic press

	runs on a Parallax C3

		 		created - mam 10/16/2017
*/

#include "simpletools.h"					// Library include
#include "press control.h"


/* shared memory */
volatile int 		dwell;					// dwell time in seconds, shared between cogs
volatile int 		kill_switch ;			// switch state, shared between cogs
volatile int 		up_switch;				// switch state, shared between cogs
volatile int 		down_switch;			// switch state, shared between cogs

int main()
{
	int timer;								// accumulator for dwell

	pause(1000);							// wait 1 second
	printf("press control version %i.%i starting\n\n", _MAJOR_VERSION_system, _MINOR_VERSION_system);
	printf("test extend ");
	down();
	printf("test retract ");
	up();
	printf("start switch monitor cog\n");
	cog_run(watch_switches, 128);     		// start cog to monitor swiwtch input
	printf("start dewll monitor cog\n");
	cog_run(set_dwell, 128);     			// start cog to monitor DIP input

	while (1)								// main loop
	{
		if (!down_switch)					// test switch
		{
			timer = 0;
			down();							// extend ram
			timer = dwell;
			while (timer > 0)
			{
				pause(100);				// Wait 0.1 second
				timer -= .1;			// increment time count
			}
			up();							// retract ram
		}
		pause(100);							// Wait 0.1 second before repeat
		// printf("dwell<%i>, kill<%i>, up<%i>, down<%i>\r",dwell,kill_switch,up_switch,down_switch);
	}
}

/* retract ram */
void up()
{
	printf("<<--- ram retract\n");
	high(_RETRACT_SOLENOID);				// Set I/O pin high
	pause(1000);								 // Wait 1/10 second
	low(_RETRACT_SOLENOID);					// Set I/O pin low
	return;
}

/* extend ram */
void down()
{
	printf("--->> ram extend\n");
	high(_EXTEND_SOLENOID);					// Set I/O pin high
	pause(1000);								// Wait 1/10 second
	low(_EXTEND_SOLENOID);					// Set I/O pin low
	return;
}

/* watch switches */
void watch_switches(void)
{
	while (1)
	{
		kill_switch = input(_KILL_SWITCH);
		if (!kill_switch)
		{
			high(_RETRACT_SOLENOID);		    // Set I/O pin high
			pause(1000);						         // Wait 1/10 second
			low(_RETRACT_SOLENOID);				  // Set I/O pin low;
		}
		up_switch = input(_UP_SWITCH);
		if (!up_switch)
		{
			high(_RETRACT_SOLENOID);		    // Set I/O pin high
			pause(1000);						         // Wait 1/10 second
			low(_RETRACT_SOLENOID);				  // Set I/O pin low;
		}
		down_switch = input(_DOWN_SWITCH);
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
			if (input(dip_switch[i]))	// reverse pin order
				value += (int)pow(2, i);
		}
		dwell = value;						// set value of dwell, shared between cogs
		pause(100);						// Wait .1 second
	}
}





