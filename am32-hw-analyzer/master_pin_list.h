#ifndef _MASTER_PIN_LIST_H_
#define _MASTER_PIN_LIST_H_

#include "config.h"
#include "defs.h"
#include <Arduino.h>

// master list of all pins
// canditates will be removed from this list dynamically later in the code

/*
// use this to generate the list
int main()
{
    int i, j;
    for (i = 'A'; i <= 'Z'; i++)
    {
        for (j = 0; j < 16; j++)
        {
            printf("    #ifdef P%c%u\r\n", i, j);
            printf("        P%c%u,\r\n"  , i, j);
            printf("    #endif\r\n");
        }
    }
    return 0;
}
*/

const uint32_t all_pins[] =
{
    #ifdef PA0
           PA0,
    #endif
    #ifdef PA1
           PA1,
    #endif
    #ifdef PA2
           PA2,
    #endif
    #ifdef PA3
           PA3,
    #endif
    #ifdef PA4
           PA4,
    #endif
    #ifdef PA5
           PA5,
    #endif
    #ifdef PA6
           PA6,
    #endif
    #ifdef PA7
           PA7,
    #endif
    #ifdef PA8
           PA8,
    #endif
    #ifdef PA9
           PA9,
    #endif
    #ifdef PA10
           PA10,
    #endif
    #ifdef PA11
           PA11,
    #endif
    #ifdef PA12
           PA12,
    #endif
    #ifdef PA13
           PA13,
    #endif
    #ifdef PA14
           PA14,
    #endif
    #ifdef PA15
           PA15,
    #endif
    #ifdef PB0
           PB0,
    #endif
    #ifdef PB1
           PB1,
    #endif
    #ifdef PB2
           PB2,
    #endif
    #ifdef PB3
           PB3,
    #endif
    #ifdef PB4
           PB4,
    #endif
    #ifdef PB5
           PB5,
    #endif
    #ifdef PB6
           PB6,
    #endif
    #ifdef PB7
           PB7,
    #endif
    #ifdef PB8
           PB8,
    #endif
    #ifdef PB9
           PB9,
    #endif
    #ifdef PB10
           PB10,
    #endif
    #ifdef PB11
           PB11,
    #endif
    #ifdef PB12
           PB12,
    #endif
    #ifdef PB13
           PB13,
    #endif
    #ifdef PB14
           PB14,
    #endif
    #ifdef PB15
           PB15,
    #endif
    #ifdef PC0
           PC0,
    #endif
    #ifdef PC1
           PC1,
    #endif
    #ifdef PC2
           PC2,
    #endif
    #ifdef PC3
           PC3,
    #endif
    #ifdef PC4
           PC4,
    #endif
    #ifdef PC5
           PC5,
    #endif
    #ifdef PC6
           PC6,
    #endif
    #ifdef PC7
           PC7,
    #endif
    #ifdef PC8
           PC8,
    #endif
    #ifdef PC9
           PC9,
    #endif
    #ifdef PC10
           PC10,
    #endif
    #ifdef PC11
           PC11,
    #endif
    #ifdef PC12
           PC12,
    #endif
    #ifdef PC13
           PC13,
    #endif
    #ifdef PC14
           PC14,
    #endif
    #ifdef PC15
           PC15,
    #endif
    #ifdef PD0
           PD0,
    #endif
    #ifdef PD1
           PD1,
    #endif
    #ifdef PD2
           PD2,
    #endif
    #ifdef PD3
           PD3,
    #endif
    #ifdef PD4
           PD4,
    #endif
    #ifdef PD5
           PD5,
    #endif
    #ifdef PD6
           PD6,
    #endif
    #ifdef PD7
           PD7,
    #endif
    #ifdef PD8
           PD8,
    #endif
    #ifdef PD9
           PD9,
    #endif
    #ifdef PD10
           PD10,
    #endif
    #ifdef PD11
           PD11,
    #endif
    #ifdef PD12
           PD12,
    #endif
    #ifdef PD13
           PD13,
    #endif
    #ifdef PD14
           PD14,
    #endif
    #ifdef PD15
           PD15,
    #endif
    #ifdef PE0
           PE0,
    #endif
    #ifdef PE1
           PE1,
    #endif
    #ifdef PE2
           PE2,
    #endif
    #ifdef PE3
           PE3,
    #endif
    #ifdef PE4
           PE4,
    #endif
    #ifdef PE5
           PE5,
    #endif
    #ifdef PE6
           PE6,
    #endif
    #ifdef PE7
           PE7,
    #endif
    #ifdef PE8
           PE8,
    #endif
    #ifdef PE9
           PE9,
    #endif
    #ifdef PE10
           PE10,
    #endif
    #ifdef PE11
           PE11,
    #endif
    #ifdef PE12
           PE12,
    #endif
    #ifdef PE13
           PE13,
    #endif
    #ifdef PE14
           PE14,
    #endif
    #ifdef PE15
           PE15,
    #endif
    #ifdef PF0
           PF0,
    #endif
    #ifdef PF1
           PF1,
    #endif
    #ifdef PF2
           PF2,
    #endif
    #ifdef PF3
           PF3,
    #endif
    #ifdef PF4
           PF4,
    #endif
    #ifdef PF5
           PF5,
    #endif
    #ifdef PF6
           PF6,
    #endif
    #ifdef PF7
           PF7,
    #endif
    #ifdef PF8
           PF8,
    #endif
    #ifdef PF9
           PF9,
    #endif
    #ifdef PF10
           PF10,
    #endif
    #ifdef PF11
           PF11,
    #endif
    #ifdef PF12
           PF12,
    #endif
    #ifdef PF13
           PF13,
    #endif
    #ifdef PF14
           PF14,
    #endif
    #ifdef PF15
           PF15,
    #endif
    #ifdef PG0
           PG0,
    #endif
    #ifdef PG1
           PG1,
    #endif
    #ifdef PG2
           PG2,
    #endif
    #ifdef PG3
           PG3,
    #endif
    #ifdef PG4
           PG4,
    #endif
    #ifdef PG5
           PG5,
    #endif
    #ifdef PG6
           PG6,
    #endif
    #ifdef PG7
           PG7,
    #endif
    #ifdef PG8
           PG8,
    #endif
    #ifdef PG9
           PG9,
    #endif
    #ifdef PG10
           PG10,
    #endif
    #ifdef PG11
           PG11,
    #endif
    #ifdef PG12
           PG12,
    #endif
    #ifdef PG13
           PG13,
    #endif
    #ifdef PG14
           PG14,
    #endif
    #ifdef PG15
           PG15,
    #endif
    #ifdef PH0
           PH0,
    #endif
    #ifdef PH1
           PH1,
    #endif
    #ifdef PH2
           PH2,
    #endif
    #ifdef PH3
           PH3,
    #endif
    #ifdef PH4
           PH4,
    #endif
    #ifdef PH5
           PH5,
    #endif
    #ifdef PH6
           PH6,
    #endif
    #ifdef PH7
           PH7,
    #endif
    #ifdef PH8
           PH8,
    #endif
    #ifdef PH9
           PH9,
    #endif
    #ifdef PH10
           PH10,
    #endif
    #ifdef PH11
           PH11,
    #endif
    #ifdef PH12
           PH12,
    #endif
    #ifdef PH13
           PH13,
    #endif
    #ifdef PH14
           PH14,
    #endif
    #ifdef PH15
           PH15,
    #endif
    #ifdef PI0
           PI0,
    #endif
    #ifdef PI1
           PI1,
    #endif
    #ifdef PI2
           PI2,
    #endif
    #ifdef PI3
           PI3,
    #endif
    #ifdef PI4
           PI4,
    #endif
    #ifdef PI5
           PI5,
    #endif
    #ifdef PI6
           PI6,
    #endif
    #ifdef PI7
           PI7,
    #endif
    #ifdef PI8
           PI8,
    #endif
    #ifdef PI9
           PI9,
    #endif
    #ifdef PI10
           PI10,
    #endif
    #ifdef PI11
           PI11,
    #endif
    #ifdef PI12
           PI12,
    #endif
    #ifdef PI13
           PI13,
    #endif
    #ifdef PI14
           PI14,
    #endif
    #ifdef PI15
           PI15,
    #endif
    #ifdef PJ0
           PJ0,
    #endif
    #ifdef PJ1
           PJ1,
    #endif
    #ifdef PJ2
           PJ2,
    #endif
    #ifdef PJ3
           PJ3,
    #endif
    #ifdef PJ4
           PJ4,
    #endif
    #ifdef PJ5
           PJ5,
    #endif
    #ifdef PJ6
           PJ6,
    #endif
    #ifdef PJ7
           PJ7,
    #endif
    #ifdef PJ8
           PJ8,
    #endif
    #ifdef PJ9
           PJ9,
    #endif
    #ifdef PJ10
           PJ10,
    #endif
    #ifdef PJ11
           PJ11,
    #endif
    #ifdef PJ12
           PJ12,
    #endif
    #ifdef PJ13
           PJ13,
    #endif
    #ifdef PJ14
           PJ14,
    #endif
    #ifdef PJ15
           PJ15,
    #endif
    #ifdef PK0
           PK0,
    #endif
    #ifdef PK1
           PK1,
    #endif
    #ifdef PK2
           PK2,
    #endif
    #ifdef PK3
           PK3,
    #endif
    #ifdef PK4
           PK4,
    #endif
    #ifdef PK5
           PK5,
    #endif
    #ifdef PK6
           PK6,
    #endif
    #ifdef PK7
           PK7,
    #endif
    #ifdef PK8
           PK8,
    #endif
    #ifdef PK9
           PK9,
    #endif
    #ifdef PK10
           PK10,
    #endif
    #ifdef PK11
           PK11,
    #endif
    #ifdef PK12
           PK12,
    #endif
    #ifdef PK13
           PK13,
    #endif
    #ifdef PK14
           PK14,
    #endif
    #ifdef PK15
           PK15,
    #endif
    #ifdef PL0
           PL0,
    #endif
    #ifdef PL1
           PL1,
    #endif
    #ifdef PL2
           PL2,
    #endif
    #ifdef PL3
           PL3,
    #endif
    #ifdef PL4
           PL4,
    #endif
    #ifdef PL5
           PL5,
    #endif
    #ifdef PL6
           PL6,
    #endif
    #ifdef PL7
           PL7,
    #endif
    #ifdef PL8
           PL8,
    #endif
    #ifdef PL9
           PL9,
    #endif
    #ifdef PL10
           PL10,
    #endif
    #ifdef PL11
           PL11,
    #endif
    #ifdef PL12
           PL12,
    #endif
    #ifdef PL13
           PL13,
    #endif
    #ifdef PL14
           PL14,
    #endif
    #ifdef PL15
           PL15,
    #endif
    #ifdef PM0
           PM0,
    #endif
    #ifdef PM1
           PM1,
    #endif
    #ifdef PM2
           PM2,
    #endif
    #ifdef PM3
           PM3,
    #endif
    #ifdef PM4
           PM4,
    #endif
    #ifdef PM5
           PM5,
    #endif
    #ifdef PM6
           PM6,
    #endif
    #ifdef PM7
           PM7,
    #endif
    #ifdef PM8
           PM8,
    #endif
    #ifdef PM9
           PM9,
    #endif
    #ifdef PM10
           PM10,
    #endif
    #ifdef PM11
           PM11,
    #endif
    #ifdef PM12
           PM12,
    #endif
    #ifdef PM13
           PM13,
    #endif
    #ifdef PM14
           PM14,
    #endif
    #ifdef PM15
           PM15,
    #endif
    #ifdef PN0
           PN0,
    #endif
    #ifdef PN1
           PN1,
    #endif
    #ifdef PN2
           PN2,
    #endif
    #ifdef PN3
           PN3,
    #endif
    #ifdef PN4
           PN4,
    #endif
    #ifdef PN5
           PN5,
    #endif
    #ifdef PN6
           PN6,
    #endif
    #ifdef PN7
           PN7,
    #endif
    #ifdef PN8
           PN8,
    #endif
    #ifdef PN9
           PN9,
    #endif
    #ifdef PN10
           PN10,
    #endif
    #ifdef PN11
           PN11,
    #endif
    #ifdef PN12
           PN12,
    #endif
    #ifdef PN13
           PN13,
    #endif
    #ifdef PN14
           PN14,
    #endif
    #ifdef PN15
           PN15,
    #endif
    #ifdef PO0
           PO0,
    #endif
    #ifdef PO1
           PO1,
    #endif
    #ifdef PO2
           PO2,
    #endif
    #ifdef PO3
           PO3,
    #endif
    #ifdef PO4
           PO4,
    #endif
    #ifdef PO5
           PO5,
    #endif
    #ifdef PO6
           PO6,
    #endif
    #ifdef PO7
           PO7,
    #endif
    #ifdef PO8
           PO8,
    #endif
    #ifdef PO9
           PO9,
    #endif
    #ifdef PO10
           PO10,
    #endif
    #ifdef PO11
           PO11,
    #endif
    #ifdef PO12
           PO12,
    #endif
    #ifdef PO13
           PO13,
    #endif
    #ifdef PO14
           PO14,
    #endif
    #ifdef PO15
           PO15,
    #endif
    #ifdef PP0
           PP0,
    #endif
    #ifdef PP1
           PP1,
    #endif
    #ifdef PP2
           PP2,
    #endif
    #ifdef PP3
           PP3,
    #endif
    #ifdef PP4
           PP4,
    #endif
    #ifdef PP5
           PP5,
    #endif
    #ifdef PP6
           PP6,
    #endif
    #ifdef PP7
           PP7,
    #endif
    #ifdef PP8
           PP8,
    #endif
    #ifdef PP9
           PP9,
    #endif
    #ifdef PP10
           PP10,
    #endif
    #ifdef PP11
           PP11,
    #endif
    #ifdef PP12
           PP12,
    #endif
    #ifdef PP13
           PP13,
    #endif
    #ifdef PP14
           PP14,
    #endif
    #ifdef PP15
           PP15,
    #endif
    #ifdef PQ0
           PQ0,
    #endif
    #ifdef PQ1
           PQ1,
    #endif
    #ifdef PQ2
           PQ2,
    #endif
    #ifdef PQ3
           PQ3,
    #endif
    #ifdef PQ4
           PQ4,
    #endif
    #ifdef PQ5
           PQ5,
    #endif
    #ifdef PQ6
           PQ6,
    #endif
    #ifdef PQ7
           PQ7,
    #endif
    #ifdef PQ8
           PQ8,
    #endif
    #ifdef PQ9
           PQ9,
    #endif
    #ifdef PQ10
           PQ10,
    #endif
    #ifdef PQ11
           PQ11,
    #endif
    #ifdef PQ12
           PQ12,
    #endif
    #ifdef PQ13
           PQ13,
    #endif
    #ifdef PQ14
           PQ14,
    #endif
    #ifdef PQ15
           PQ15,
    #endif
    #ifdef PR0
           PR0,
    #endif
    #ifdef PR1
           PR1,
    #endif
    #ifdef PR2
           PR2,
    #endif
    #ifdef PR3
           PR3,
    #endif
    #ifdef PR4
           PR4,
    #endif
    #ifdef PR5
           PR5,
    #endif
    #ifdef PR6
           PR6,
    #endif
    #ifdef PR7
           PR7,
    #endif
    #ifdef PR8
           PR8,
    #endif
    #ifdef PR9
           PR9,
    #endif
    #ifdef PR10
           PR10,
    #endif
    #ifdef PR11
           PR11,
    #endif
    #ifdef PR12
           PR12,
    #endif
    #ifdef PR13
           PR13,
    #endif
    #ifdef PR14
           PR14,
    #endif
    #ifdef PR15
           PR15,
    #endif
    #ifdef PS0
           PS0,
    #endif
    #ifdef PS1
           PS1,
    #endif
    #ifdef PS2
           PS2,
    #endif
    #ifdef PS3
           PS3,
    #endif
    #ifdef PS4
           PS4,
    #endif
    #ifdef PS5
           PS5,
    #endif
    #ifdef PS6
           PS6,
    #endif
    #ifdef PS7
           PS7,
    #endif
    #ifdef PS8
           PS8,
    #endif
    #ifdef PS9
           PS9,
    #endif
    #ifdef PS10
           PS10,
    #endif
    #ifdef PS11
           PS11,
    #endif
    #ifdef PS12
           PS12,
    #endif
    #ifdef PS13
           PS13,
    #endif
    #ifdef PS14
           PS14,
    #endif
    #ifdef PS15
           PS15,
    #endif
    #ifdef PT0
           PT0,
    #endif
    #ifdef PT1
           PT1,
    #endif
    #ifdef PT2
           PT2,
    #endif
    #ifdef PT3
           PT3,
    #endif
    #ifdef PT4
           PT4,
    #endif
    #ifdef PT5
           PT5,
    #endif
    #ifdef PT6
           PT6,
    #endif
    #ifdef PT7
           PT7,
    #endif
    #ifdef PT8
           PT8,
    #endif
    #ifdef PT9
           PT9,
    #endif
    #ifdef PT10
           PT10,
    #endif
    #ifdef PT11
           PT11,
    #endif
    #ifdef PT12
           PT12,
    #endif
    #ifdef PT13
           PT13,
    #endif
    #ifdef PT14
           PT14,
    #endif
    #ifdef PT15
           PT15,
    #endif
    #ifdef PU0
           PU0,
    #endif
    #ifdef PU1
           PU1,
    #endif
    #ifdef PU2
           PU2,
    #endif
    #ifdef PU3
           PU3,
    #endif
    #ifdef PU4
           PU4,
    #endif
    #ifdef PU5
           PU5,
    #endif
    #ifdef PU6
           PU6,
    #endif
    #ifdef PU7
           PU7,
    #endif
    #ifdef PU8
           PU8,
    #endif
    #ifdef PU9
           PU9,
    #endif
    #ifdef PU10
           PU10,
    #endif
    #ifdef PU11
           PU11,
    #endif
    #ifdef PU12
           PU12,
    #endif
    #ifdef PU13
           PU13,
    #endif
    #ifdef PU14
           PU14,
    #endif
    #ifdef PU15
           PU15,
    #endif
    #ifdef PV0
           PV0,
    #endif
    #ifdef PV1
           PV1,
    #endif
    #ifdef PV2
           PV2,
    #endif
    #ifdef PV3
           PV3,
    #endif
    #ifdef PV4
           PV4,
    #endif
    #ifdef PV5
           PV5,
    #endif
    #ifdef PV6
           PV6,
    #endif
    #ifdef PV7
           PV7,
    #endif
    #ifdef PV8
           PV8,
    #endif
    #ifdef PV9
           PV9,
    #endif
    #ifdef PV10
           PV10,
    #endif
    #ifdef PV11
           PV11,
    #endif
    #ifdef PV12
           PV12,
    #endif
    #ifdef PV13
           PV13,
    #endif
    #ifdef PV14
           PV14,
    #endif
    #ifdef PV15
           PV15,
    #endif
    #ifdef PW0
           PW0,
    #endif
    #ifdef PW1
           PW1,
    #endif
    #ifdef PW2
           PW2,
    #endif
    #ifdef PW3
           PW3,
    #endif
    #ifdef PW4
           PW4,
    #endif
    #ifdef PW5
           PW5,
    #endif
    #ifdef PW6
           PW6,
    #endif
    #ifdef PW7
           PW7,
    #endif
    #ifdef PW8
           PW8,
    #endif
    #ifdef PW9
           PW9,
    #endif
    #ifdef PW10
           PW10,
    #endif
    #ifdef PW11
           PW11,
    #endif
    #ifdef PW12
           PW12,
    #endif
    #ifdef PW13
           PW13,
    #endif
    #ifdef PW14
           PW14,
    #endif
    #ifdef PW15
           PW15,
    #endif
    #ifdef PX0
           PX0,
    #endif
    #ifdef PX1
           PX1,
    #endif
    #ifdef PX2
           PX2,
    #endif
    #ifdef PX3
           PX3,
    #endif
    #ifdef PX4
           PX4,
    #endif
    #ifdef PX5
           PX5,
    #endif
    #ifdef PX6
           PX6,
    #endif
    #ifdef PX7
           PX7,
    #endif
    #ifdef PX8
           PX8,
    #endif
    #ifdef PX9
           PX9,
    #endif
    #ifdef PX10
           PX10,
    #endif
    #ifdef PX11
           PX11,
    #endif
    #ifdef PX12
           PX12,
    #endif
    #ifdef PX13
           PX13,
    #endif
    #ifdef PX14
           PX14,
    #endif
    #ifdef PX15
           PX15,
    #endif
    #ifdef PY0
           PY0,
    #endif
    #ifdef PY1
           PY1,
    #endif
    #ifdef PY2
           PY2,
    #endif
    #ifdef PY3
           PY3,
    #endif
    #ifdef PY4
           PY4,
    #endif
    #ifdef PY5
           PY5,
    #endif
    #ifdef PY6
           PY6,
    #endif
    #ifdef PY7
           PY7,
    #endif
    #ifdef PY8
           PY8,
    #endif
    #ifdef PY9
           PY9,
    #endif
    #ifdef PY10
           PY10,
    #endif
    #ifdef PY11
           PY11,
    #endif
    #ifdef PY12
           PY12,
    #endif
    #ifdef PY13
           PY13,
    #endif
    #ifdef PY14
           PY14,
    #endif
    #ifdef PY15
           PY15,
    #endif
    #ifdef PZ0
           PZ0,
    #endif
    #ifdef PZ1
           PZ1,
    #endif
    #ifdef PZ2
           PZ2,
    #endif
    #ifdef PZ3
           PZ3,
    #endif
    #ifdef PZ4
           PZ4,
    #endif
    #ifdef PZ5
           PZ5,
    #endif
    #ifdef PZ6
           PZ6,
    #endif
    #ifdef PZ7
           PZ7,
    #endif
    #ifdef PZ8
           PZ8,
    #endif
    #ifdef PZ9
           PZ9,
    #endif
    #ifdef PZ10
           PZ10,
    #endif
    #ifdef PZ11
           PZ11,
    #endif
    #ifdef PZ12
           PZ12,
    #endif
    #ifdef PZ13
           PZ13,
    #endif
    #ifdef PZ14
           PZ14,
    #endif
    #ifdef PZ15
           PZ15,
    #endif
    END_OF_LIST,
};

#endif
