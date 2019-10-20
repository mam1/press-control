/*
    code to control a pneumatic press

    runs on a Parallax C3

    10/16/2017             created mam
    11/8/2017      v-4.0   installed
    5/6/2018       v-6.0   removed pauses where possible leds controlled directly by switches
    5/11/2018      v-6.1   improve response to up switch
    5/12/2018      v-6.5   add locks for shared memory
    5/13/2018      v-6.6   reverse high order bit on binary input
    6/5/2018       v-6.7   add debounce code
    7/22/2018      v-6.8   remove all commented out code, reformat, add comments
    7/22/2018      v-6.9   move state change variables from shared memory to cog memory
                           change solenoid pulse from 200ms to 300ms
                           remove pause in main loop
    7/10/2019      v-6.10  fix bug in up switch debounce code
    10/19/2019     v-6.11  extend length of pulse on the solenoid valve to 1000ms
                           add trace code output to terminal

*/

#include "simpletools.h"        // Library include
#include "press control.h"

/* shared memory */
static volatile int         dwell;                  // dwell time, shared between cogs
static volatile int         lock_dwell;             // memory lock
static volatile int         ds_state, us_state;     // debounced state of switch

int main()
{
    int         timer;                        // accumulator for dwell
    int         *cog1, *cog2, *cog3;           // cog ids

    /* initializations */
    high(_STATUS_LED_BUS_MUX);                // free up vga io pins */
    ds_state = 0;                             // set initial down switch state
    us_state = 0;                             // set initial up switch state
    up();                                     // retract ram
    lock_dwell = 0;                           // clear shared memory lock
    pause(100);

    printf("press control version %i.%i starting\n\n", _MAJOR_VERSION_system, _MINOR_VERSION_system);
#if _DEBUG == 1   
    printf("up switch state = %i, down switch state = %i\n", us_state, ds_state );
#endif
    cog1 = cog_run(watch_up_switch, 128);    // start cog to monitor up switch
    cog2 = cog_run(watch_down_switch, 128);  // start cog to monitor down switch
    cog3 = cog_run(set_dwell, 128);          // start cog to monitor dwell setting switches
    pause(100);

    /* main loop */
    while (1)                        // loop forever
    {
#if _DEBUG == 1
        pause(1000);
        printf("up switch state = %i, down switch state = %i\n", us_state, ds_state );
#endif
        if (us_state)                // test up switch
            up();                    // retract ram
        else if (ds_state)           // test down switch
        {
            while (lock_dwell);      // wait for lock to free

#if _DEBUG == 1
            timer = 50;               // set dewell time
#else
            timer = dwell;            // set dwell time froms switchwes
#endif

            down();                  // extend ram
            while (timer > 0)        // loop until counter hits zero
            {
                pause(100);          // Wait
                timer -= 1;          // decrement time count
                if (us_state) {       // check state of up switch
#if _DEBUG == 1

                    printf("%s\n", "up switch tested on\n");
#endif
                    timer = 0;       // force loop exit
                }
            }
            up();                    // retract ram
        }
    }
}

/* retract ram */
void up()
{
#if _DEBUG == 1
    printf("%s\n", "pulse retract solenoid" );
#endif
    high(_RETRACT_SOLENOID);         // Set I/O pin high
    pause(_SPLUSE);                  // Wait
    low(_RETRACT_SOLENOID);          // Set I/O pin low
    return;
}

/* extend ram */
void down()
{
#if _DEBUG == 1
    printf("%s\n", "pulse extend solenoid" );
#endif
    high(_EXTEND_SOLENOID);           // Set I/O pin high
    pause(_SPLUSE);                   // wait
    low(_EXTEND_SOLENOID);            // Set I/O pin low
    return;
}

/********************  Cog code *************************************/

/* watch dwell time switches */
void set_dwell(void)
{
    static int      tswitch[8] = {_DIP_0, _DIP_1, _DIP_2, _DIP_3, _DIP_4, _DIP_5, _DIP_6, _DIP_7};
    int             value, i;

    while (1)
    {
        value = 0;
        for (i = 0; i < 8; i++)        // convert binary witch settings to decimal seconds
            if (input(tswitch[i]))
                value += (int)pow(2, i);
        lock_dwell = 1;                //set memory lock
        dwell = value * 10;            // convert to .1 seconds and load shared memory
        lock_dwell = 0;                // free memory lock
        pause(CHECK_MSEC);             // Wait
    }
}

/* watch down switch */
void watch_down_switch(void)
{
    int                  ds_change;
    static uint8_t       Count = RELEASE_MSEC / CHECK_MSEC;
    int                  RawState;

    while (1)
    {
        ds_change = 0;
        RawState = input(_DOWN_SWITCH);    // read down switch
        if (RawState == ds_state)
        {
            // Set the timer which will allow a change from the current state.
            if (ds_state)
                Count = RELEASE_MSEC / CHECK_MSEC;
            else
                Count = PRESS_MSEC / CHECK_MSEC;
        }
        else
        {
            // Key has changed - wait for new state to become stable.
            if (--Count == 0)
            {
                // Timer expired - accept the change.
                ds_state = RawState;
                ds_change = 1;
                // And reset the timer.
                if (ds_state)
                    Count = RELEASE_MSEC / CHECK_MSEC;
                else
                    Count = PRESS_MSEC / CHECK_MSEC;
            }
        }
        pause(CHECK_MSEC);
    }
}

/* watch up switch */
void watch_up_switch(void)
{
    int                  us_change;
    static uint8_t       Count = RELEASE_MSEC / CHECK_MSEC;
    int                  RawState;

    while (1)
    {
        us_change = 0;
        RawState = input(_UP_SWITCH);    // read up switch
        if (RawState == us_state)
        {
            // Set the timer which will allow a change from the current state.
            if (us_state)
                Count = RELEASE_MSEC / CHECK_MSEC;
            else
                Count = PRESS_MSEC / CHECK_MSEC;
        }
        else
        {
            // Key has changed - wait for new state to become stable.
            if (--Count == 0) {
                // Timer expired - accept the change.
                us_state = RawState;
                us_change = 1;
                // And reset the timer.
                if (us_state)
                    Count = RELEASE_MSEC / CHECK_MSEC;
                else
                    Count = PRESS_MSEC / CHECK_MSEC;
            }
        }
        pause(CHECK_MSEC);
    }
}
