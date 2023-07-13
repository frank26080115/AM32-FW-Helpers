#ifndef _HW_PIN_LISTS_H_
#define _HW_PIN_LISTS_H_

#include "config.h"
#include "defs.h"
#include <Arduino.h>

// list the potential candidates for pins

#if defined(ARDUINO_GENERIC_F051K6HX) || defined(ARDUINO_GENERIC_F051K6TX) || defined(ARDUINO_GENERIC_F051K6UX) || defined(ARDUINO_GENERIC_F051K6YX) || defined(ARDUINO_GENERIC_F051K8HX) || defined(ARDUINO_GENERIC_F051K8TX) || defined(ARDUINO_GENERIC_F051K8UX) || defined(ARDUINO_GENERIC_F051K8YX) \
 || defined(ARDUINO_GENERIC_F051T6HX) || defined(ARDUINO_GENERIC_F051T6TX) || defined(ARDUINO_GENERIC_F051T6UX) || defined(ARDUINO_GENERIC_F051T6YX) || defined(ARDUINO_GENERIC_F051T8HX) || defined(ARDUINO_GENERIC_F051T8TX) || defined(ARDUINO_GENERIC_F051T8UX) || defined(ARDUINO_GENERIC_F051T8YX) \
 || defined(ARDUINO_GENERIC_F051C6HX) || defined(ARDUINO_GENERIC_F051C6TX) || defined(ARDUINO_GENERIC_F051C6UX) || defined(ARDUINO_GENERIC_F051C6YX) || defined(ARDUINO_GENERIC_F051C8HX) || defined(ARDUINO_GENERIC_F051C8TX) || defined(ARDUINO_GENERIC_F051C8UX) || defined(ARDUINO_GENERIC_F051C8YX) \
 || defined(ARDUINO_GENERIC_F051R6HX) || defined(ARDUINO_GENERIC_F051R6TX) || defined(ARDUINO_GENERIC_F051R6UX) || defined(ARDUINO_GENERIC_F051R6YX) || defined(ARDUINO_GENERIC_F051R8HX) || defined(ARDUINO_GENERIC_F051R8TX) || defined(ARDUINO_GENERIC_F051R8UX) || defined(ARDUINO_GENERIC_F051R8YX) \

static const uint32_t all_rc[] = {
    PA2,
    PB4,
    END_OF_LIST,
};

static const uint32_t all_hin[] = {
    PA10,
    PA9,
    PA8,
    END_OF_LIST,
};

static const uint32_t all_lin[] = {
    PB1,
    PB0,
    PA7,
    END_OF_LIST,
};

static const uint32_t all_fb[] = {
    PA5,
    PA0,
    PA4,
    END_OF_LIST,
};

#ifdef ASSUME_ADC_PINS
static const uint32_t assumed_adcs[] = {
    PA3, PA6,
    END_OF_LIST,
};
#endif

#elif defined(ARDUINO_GENERIC_F031C6PX) || defined(ARDUINO_GENERIC_F031C6TX) || defined(ARDUINO_GENERIC_F031C6UX) || defined(ARDUINO_GENERIC_F031C6YX) || defined(ARDUINO_GENERIC_F031C6PX) || defined(ARDUINO_GENERIC_F031C6TX) || defined(ARDUINO_GENERIC_F031C6UX) || defined(ARDUINO_GENERIC_F031C6YX)

static const uint32_t all_rc[] = {
    PA2,
    END_OF_LIST,
};

static const uint32_t all_hin[] = {
    PA10,
    PA9,
    PA8,
    END_OF_LIST,
};

static const uint32_t all_lin[] = {
    PB14,
    PB13,
    PB15,
    END_OF_LIST,
};

static const uint32_t all_fb[] = {
    PF0,
    PF1,
    PA5,
    END_OF_LIST,
};

#ifdef ASSUME_ADC_PINS
static const uint32_t assumed_adcs[] = {
    PA5, PA7,
    END_OF_LIST,
};
#endif

#elif  defined(ARDUINO_GENERIC_G071E8IX) || defined(ARDUINO_GENERIC_G071E8TX) || defined(ARDUINO_GENERIC_G071E8UX) || defined(ARDUINO_GENERIC_G071E8YX) || defined(ARDUINO_GENERIC_G071EBIX) || defined(ARDUINO_GENERIC_G071EBTX) || defined(ARDUINO_GENERIC_G071EBUX) || defined(ARDUINO_GENERIC_G071EBYX) \
    || defined(ARDUINO_GENERIC_G071G8IX) || defined(ARDUINO_GENERIC_G071G8TX) || defined(ARDUINO_GENERIC_G071G8UX) || defined(ARDUINO_GENERIC_G071G8YX) || defined(ARDUINO_GENERIC_G071GBIX) || defined(ARDUINO_GENERIC_G071GBTX) || defined(ARDUINO_GENERIC_G071GBUX) || defined(ARDUINO_GENERIC_G071GBYX) \
    || defined(ARDUINO_GENERIC_G071K8IX) || defined(ARDUINO_GENERIC_G071K8TX) || defined(ARDUINO_GENERIC_G071K8UX) || defined(ARDUINO_GENERIC_G071K8YX) || defined(ARDUINO_GENERIC_G071KBIX) || defined(ARDUINO_GENERIC_G071KBTX) || defined(ARDUINO_GENERIC_G071KBUX) || defined(ARDUINO_GENERIC_G071KBYX) \
    || defined(ARDUINO_GENERIC_G071C8IX) || defined(ARDUINO_GENERIC_G071C8TX) || defined(ARDUINO_GENERIC_G071C8UX) || defined(ARDUINO_GENERIC_G071C8YX) || defined(ARDUINO_GENERIC_G071CBIX) || defined(ARDUINO_GENERIC_G071CBTX) || defined(ARDUINO_GENERIC_G071CBUX) || defined(ARDUINO_GENERIC_G071CBYX) \
    || defined(ARDUINO_GENERIC_G071R8IX) || defined(ARDUINO_GENERIC_G071R8TX) || defined(ARDUINO_GENERIC_G071R8UX) || defined(ARDUINO_GENERIC_G071R8YX) || defined(ARDUINO_GENERIC_G071RBIX) || defined(ARDUINO_GENERIC_G071RBTX) || defined(ARDUINO_GENERIC_G071RBUX) || defined(ARDUINO_GENERIC_G071RBYX) \

static const uint32_t all_rc[] = {
    PA6,
    PB4,
    END_OF_LIST,
};

static const uint32_t all_hin[] = {
    PA10,
    PA9,
    PA8,
    END_OF_LIST,
};

static const uint32_t all_lin[] = {
    PB1,
    PB0,
    PA7,
    END_OF_LIST,
};

static const uint32_t all_fb[] = {
    PB7,
    PB3,
    PA2,
    END_OF_LIST,
};

#ifdef ASSUME_ADC_PINS
static const uint32_t assumed_adcs[] = {
    PA5, PA6,
    END_OF_LIST,
};
#endif

#else

#error unsupported MCU

#endif

#endif
