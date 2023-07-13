#ifndef _DEFS_H_
#define _DEFS_H_

#include <stdint.h>
#include "config.h"

#define PIN_NOT_EXIST 0xFFFFFFFF
#define END_OF_LIST   0xFFFFFFFF

#define PINTRANSLATE(x) (x)
//#define PINTRANSLATE(x) digitalPinToAnalogInput(x)
//#define PINTRANSLATE(x) analogPinTranslate(x)

// state machine state names
enum
{
    STATEMACH_WAITING_START,      // output text on serial port, wait for user to respond
    STATEMACH_TEST_PHASE_START,   // start checking phase pins
    STATEMACH_TEST_PHASE_A,       // checking all HIN and FB pins
    STATEMACH_TEST_PHASE_B,       // checking all LIN pins
    STATEMACH_TEST_REPORT_PHASES, // print out the result of the test
    STATEMACH_CMD_PROMPT,         // ask the user to input a command
    STATEMACH_CMD_WAIT,           // wait for user to give a command
    STATEMACH_SEARCH_VOLTAGE,        // simply print out ADC readings
    STATEMACH_SEARCH_CURRENTSENSOR,  // make the motor draw current, and see if any ADC readings change
    STATEMACH_SEARCH_LED,            // blink LED pins until user sees it blinking
};

typedef struct
{
    // every phase has a HIN pin, a LIN pin, and a FB pin
    uint32_t hin;
    uint32_t lin;
    uint32_t fb;
}
phase_group_t;

#endif
