#ifndef PRESS_CONTROL_H_
#define PRESS_CONTROL_H_

/* version */
#define _MAJOR_VERSION_system	1
#define _MINOR_VERSION_system	0

/* switch pins */
#define _DOWN_SWITCH		18
#define _UP_SWITCH			17
#define _KILL_SWITCH		16

/* solenoid control pins */
#define _EXTEND_SOLENOID	0
#define _RETRACT_SOLENOID	1

/* DIP switch pins */
#define _DIP_0				2
#define _DIP_1				3
#define _DIP_2				4
#define _DIP_3				5
#define _DIP_4				6
#define _DIP_5				7
#define _DIP_6				8
#define _DIP_7				9

/* ram control  */
void up(void);
void down(void);  

/* cogs */
void set_dwell();							// Forward declaration, cog code
void watch_switches();						// Forward declaration, cog code                  

#endif /* PRESS_CONTROL_H_ */