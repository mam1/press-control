/*
	code to control a pneumatic press

	runs on a Parallax C3

		 		create - mam 10/16/2017
*/

#include "simpletools.h"                   // Library include
#include "press control.h"

void set_dwell();                          // Forward declaration
void save_dwell();                         // Forward declaration
void up();                                 // Forward declaration
void down();                               // Forward declaration
volatile int 		dwell;				   // dwell time in seconds, shared between cogs

int main()                                 // Main function
{
	int 			down_switch;
	int 			up_switch;
	int 			kill_switch;

	int timer;								// accumulator for dwell

	pause(100);
	printf("press control version %i.%i starting\n\n", _MAJOR_VERSION_system, _MINOR_VERSION_system);

	dwell = ee_getInt(_EEPROM_BASE);		// read dwell from EEPROM 
	cog_run(set_dwell, 128); 				// start cog to monitor user input                    
	pause(1000);
	while (1)
	{
		kill_switch = input(_KILL_SWITCH);
		if (kill_switch)
		{
			up();
			return 0;
		}
		up_switch = input(_UP_SWITCH);
		if (up_switch)
		{
			up();
		}
		down_switch = 1;
		if (down_switch)
		{
			timer = 0;
			down();
			printf("dwell set to %i seconds|n",dwell);
			while (timer < dwell)
			{
				kill_switch = input(_KILL_SWITCH);
				if (kill_switch)
				{
					up();
					return 0;
				}
				up_switch = input(_UP_SWITCH);
				if (up_switch)
	            	exit;
				else
				{
					pause(100);             // Wait 0.1 second
					timer += .1;			// increment time count
				}			
			}
			up();
		}
		pause(100);                          // Wait 0.1 second before repeat
	}
}

/* retract ram */
void up()
{
	high(_RETRACT_SOLENOID);                  // Set I/O pin high
    pause(100);                               // Wait 1/10 second
    low(_RETRACT_SOLENOID);                   // Set I/O pin low
    pause(100);                               // Wait another 1/10 second
}

/* extend ram */
void down()
{
    high(_EXTEND_SOLENOID);                   // Set I/O pin high
    pause(100);                               // Wait 1/10 second
    low(_EXTEND_SOLENOID);                    // Set I/O pin low
    pause(100);                               // Wait another 1/10 second
}

/* let user set a new dwell time*/
void set_dwell()
{
	while(1){
		dwell = 225;
		ee_putInt(dwell, eeprom_addr);		  // save dwell to EEPROM 
		pause(10000);
	}
}






