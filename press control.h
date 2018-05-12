#ifndef PRESS_CONTROL_H_
#define PRESS_CONTROL_H_

/* version */
#define _MAJOR_VERSION_system	6
#define _MINOR_VERSION_system	4

/* foot switch pins */
#define _DOWN_SWITCH		0
#define _UP_SWITCH			1

/* solenoid control pins */
#define _EXTEND_SOLENOID	18
#define _RETRACT_SOLENOID	19
#define _SPLUSE				200

/* switch pins */
#define _DIP_0				17
#define _DIP_1				16
#define _DIP_2				7
#define _DIP_3				6
#define _DIP_4				5
#define _DIP_5				4
#define _DIP_6				3
#define _DIP_7				2

/* VGA disable pin */
#define _STATUS_LED_BUS_MUX	15

/* ram control  */
void up(void);
void down(void);  

/* cogs */
void set_dwell();					// Forward declaration, cog code
void watch_up_switch();				// Forward declaration, cog code   

/* miscelianous */
#define _EXTENDED 			1
#define _RETRACTED 			0              

#endif /* PRESS_CONTROL_H_ */