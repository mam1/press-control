#ifndef PRESS_CONTROL_H_
#define PRESS_CONTROL_H_

/* version */
#define _MAJOR_VERSION_system	6
#define _MINOR_VERSION_system	8

/* foot switch pins */
#define _DOWN_SWITCH		0
#define _UP_SWITCH			1

/* solenoid control pins */
#define _EXTEND_SOLENOID	18
#define _RETRACT_SOLENOID	19
#define _SPLUSE				200		// length of time to engage solenoid on pneumatic valve

/* switch pins */
#define _DIP_0				2
#define _DIP_1				3
#define _DIP_2				4
#define _DIP_3				5
#define _DIP_4				6
#define _DIP_5				7
#define _DIP_6				16
#define _DIP_7				17

/* VGA disable pin */
#define _STATUS_LED_BUS_MUX	15

/* ram control  */
void up(void);
void down(void);  

/* cogs */
void set_dwell();					// Forward declaration, cog code
void watch_up_switch();				// Forward declaration, cog code  
void watch_down_switch();				// Forward declaration, cog code 

/* miscelianous */
#define _EXTENDED 			1
#define _RETRACTED 			0 
#define _BOUNCE 			100  

#define CHECK_MSEC  5   // Read hardware every 5 msec
#define PRESS_MSEC  10  // Stable time before registering pressed
#define RELEASE_MSEC    100 // Stable time before registering released
          

#endif /* PRESS_CONTROL_H_ */