#ifndef PRESS_CONTROL_H_
#define PRESS_CONTROL_H_

/* version */
#define _MAJOR_VERSION_system	5
#define _MINOR_VERSION_system	0

/* foot switch pins */
#define _DOWN_SWITCH		0
#define _UP_SWITCH			1

/* solenoid control pins */
#define _EXTEND_SOLENOID	18
#define _RETRACT_SOLENOID	19
#define _SPLUSE				100

/* switch pins */
#define _DIP_0				2
#define _DIP_1				3
#define _DIP_2				4
#define _DIP_3				5
#define _DIP_4				6
#define _DIP_5				7
#define _DIP_6				16
#define _DIP_7				17

// /* LED pins */
// #define _LED_1				16
// #define _LED_2				17
// #define _LED_3				18
// #define _LED_4				19
// #define _LED_5				20
// #define _LED_6				21
// #define _LED_7				22
// #define _LED_8				23

/* VGA disable pin */
#define _STATUS_LED_BUS_MUX		15

/* cycle leds */
void cycle(void);

/* ram control  */
void up(void);
void down(void);  

/* cogs */
void set_dwell();							// Forward declaration, cog code
void watch_up_switch();						// Forward declaration, cog code   

/* miscelianous */
#define _EXTENDED 			1
#define _RETRACTED 			0              

#endif /* PRESS_CONTROL_H_ */