#ifndef PRESS_CONTROL_H_
#define PRESS_CONTROL_H_

/* version */
#define _MAJOR_VERSION_system    6
#define _MINOR_VERSION_system    11

/* foot switch pins */
#define _DOWN_SWITCH          0
#define _UP_SWITCH            1

/* solenoid control IO pins */
#define _EXTEND_SOLENOID      18
#define _RETRACT_SOLENOID     19

/* dwell switch IO pins */
#define _DIP_0                2
#define _DIP_1                3
#define _DIP_2                4
#define _DIP_3                5
#define _DIP_4                6
#define _DIP_5                7
#define _DIP_6                16
#define _DIP_7                17

/* VGA disable IO pin */
#define _STATUS_LED_BUS_MUX   15    // make the VGA pins available for gneral use

/* ram control  */
void up(void);                      // Forward declaration
void down(void);                    // Forward declaration

/* cogs */
void set_dwell();                   // Forward declaration, cog code
void watch_up_switch();             // Forward declaration, cog code  
void watch_down_switch();           // Forward declaration, cog code 

/* miscelianous */
#define _EXTENDED             1
#define _RETRACTED            0 
#define _SPLUSE               1000   // length of time to engage solenoid on pneumatic valve
#define CHECK_MSEC            5      // Read hardware every 5 msec
#define PRESS_MSEC            10     // Stable time before registering pressed
#define RELEASE_MSEC          100    // Stable time before registering released
          

#endif /* PRESS_CONTROL_H_ */