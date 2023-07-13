/*
use this code to generate
int main()
{
    int i, j;
    for (i = 'A'; i <= 'Z'; i++)
    {
        for (j = 0; j < 16; j++)
        {
            printf("        #ifdef P%c%u\r\n", i, j);
            printf("            case P%c%u: printer->print(\"P%c%u\"); break;\r\n", i, j, i, j);
            printf("        #endif\r\n");
        }
    }
    return 0;
}
*/

void print_pin_name(uint32_t p)
{
    switch (p)
    {
#ifdef PA0
            case PA0: printer->print("PA0"); break;
        #endif
        #ifdef PA1
            case PA1: printer->print("PA1"); break;
        #endif
        #ifdef PA2
            case PA2: printer->print("PA2"); break;
        #endif
        #ifdef PA3
            case PA3: printer->print("PA3"); break;
        #endif
        #ifdef PA4
            case PA4: printer->print("PA4"); break;
        #endif
        #ifdef PA5
            case PA5: printer->print("PA5"); break;
        #endif
        #ifdef PA6
            case PA6: printer->print("PA6"); break;
        #endif
        #ifdef PA7
            case PA7: printer->print("PA7"); break;
        #endif
        #ifdef PA8
            case PA8: printer->print("PA8"); break;
        #endif
        #ifdef PA9
            case PA9: printer->print("PA9"); break;
        #endif
        #ifdef PA10
            case PA10: printer->print("PA10"); break;
        #endif
        #ifdef PA11
            case PA11: printer->print("PA11"); break;
        #endif
        #ifdef PA12
            case PA12: printer->print("PA12"); break;
        #endif
        #ifdef PA13
            case PA13: printer->print("PA13"); break;
        #endif
        #ifdef PA14
            case PA14: printer->print("PA14"); break;
        #endif
        #ifdef PA15
            case PA15: printer->print("PA15"); break;
        #endif
        #ifdef PB0
            case PB0: printer->print("PB0"); break;
        #endif
        #ifdef PB1
            case PB1: printer->print("PB1"); break;
        #endif
        #ifdef PB2
            case PB2: printer->print("PB2"); break;
        #endif
        #ifdef PB3
            case PB3: printer->print("PB3"); break;
        #endif
        #ifdef PB4
            case PB4: printer->print("PB4"); break;
        #endif
        #ifdef PB5
            case PB5: printer->print("PB5"); break;
        #endif
        #ifdef PB6
            case PB6: printer->print("PB6"); break;
        #endif
        #ifdef PB7
            case PB7: printer->print("PB7"); break;
        #endif
        #ifdef PB8
            case PB8: printer->print("PB8"); break;
        #endif
        #ifdef PB9
            case PB9: printer->print("PB9"); break;
        #endif
        #ifdef PB10
            case PB10: printer->print("PB10"); break;
        #endif
        #ifdef PB11
            case PB11: printer->print("PB11"); break;
        #endif
        #ifdef PB12
            case PB12: printer->print("PB12"); break;
        #endif
        #ifdef PB13
            case PB13: printer->print("PB13"); break;
        #endif
        #ifdef PB14
            case PB14: printer->print("PB14"); break;
        #endif
        #ifdef PB15
            case PB15: printer->print("PB15"); break;
        #endif
        #ifdef PC0
            case PC0: printer->print("PC0"); break;
        #endif
        #ifdef PC1
            case PC1: printer->print("PC1"); break;
        #endif
        #ifdef PC2
            case PC2: printer->print("PC2"); break;
        #endif
        #ifdef PC3
            case PC3: printer->print("PC3"); break;
        #endif
        #ifdef PC4
            case PC4: printer->print("PC4"); break;
        #endif
        #ifdef PC5
            case PC5: printer->print("PC5"); break;
        #endif
        #ifdef PC6
            case PC6: printer->print("PC6"); break;
        #endif
        #ifdef PC7
            case PC7: printer->print("PC7"); break;
        #endif
        #ifdef PC8
            case PC8: printer->print("PC8"); break;
        #endif
        #ifdef PC9
            case PC9: printer->print("PC9"); break;
        #endif
        #ifdef PC10
            case PC10: printer->print("PC10"); break;
        #endif
        #ifdef PC11
            case PC11: printer->print("PC11"); break;
        #endif
        #ifdef PC12
            case PC12: printer->print("PC12"); break;
        #endif
        #ifdef PC13
            case PC13: printer->print("PC13"); break;
        #endif
        #ifdef PC14
            case PC14: printer->print("PC14"); break;
        #endif
        #ifdef PC15
            case PC15: printer->print("PC15"); break;
        #endif
        #ifdef PD0
            case PD0: printer->print("PD0"); break;
        #endif
        #ifdef PD1
            case PD1: printer->print("PD1"); break;
        #endif
        #ifdef PD2
            case PD2: printer->print("PD2"); break;
        #endif
        #ifdef PD3
            case PD3: printer->print("PD3"); break;
        #endif
        #ifdef PD4
            case PD4: printer->print("PD4"); break;
        #endif
        #ifdef PD5
            case PD5: printer->print("PD5"); break;
        #endif
        #ifdef PD6
            case PD6: printer->print("PD6"); break;
        #endif
        #ifdef PD7
            case PD7: printer->print("PD7"); break;
        #endif
        #ifdef PD8
            case PD8: printer->print("PD8"); break;
        #endif
        #ifdef PD9
            case PD9: printer->print("PD9"); break;
        #endif
        #ifdef PD10
            case PD10: printer->print("PD10"); break;
        #endif
        #ifdef PD11
            case PD11: printer->print("PD11"); break;
        #endif
        #ifdef PD12
            case PD12: printer->print("PD12"); break;
        #endif
        #ifdef PD13
            case PD13: printer->print("PD13"); break;
        #endif
        #ifdef PD14
            case PD14: printer->print("PD14"); break;
        #endif
        #ifdef PD15
            case PD15: printer->print("PD15"); break;
        #endif
        #ifdef PE0
            case PE0: printer->print("PE0"); break;
        #endif
        #ifdef PE1
            case PE1: printer->print("PE1"); break;
        #endif
        #ifdef PE2
            case PE2: printer->print("PE2"); break;
        #endif
        #ifdef PE3
            case PE3: printer->print("PE3"); break;
        #endif
        #ifdef PE4
            case PE4: printer->print("PE4"); break;
        #endif
        #ifdef PE5
            case PE5: printer->print("PE5"); break;
        #endif
        #ifdef PE6
            case PE6: printer->print("PE6"); break;
        #endif
        #ifdef PE7
            case PE7: printer->print("PE7"); break;
        #endif
        #ifdef PE8
            case PE8: printer->print("PE8"); break;
        #endif
        #ifdef PE9
            case PE9: printer->print("PE9"); break;
        #endif
        #ifdef PE10
            case PE10: printer->print("PE10"); break;
        #endif
        #ifdef PE11
            case PE11: printer->print("PE11"); break;
        #endif
        #ifdef PE12
            case PE12: printer->print("PE12"); break;
        #endif
        #ifdef PE13
            case PE13: printer->print("PE13"); break;
        #endif
        #ifdef PE14
            case PE14: printer->print("PE14"); break;
        #endif
        #ifdef PE15
            case PE15: printer->print("PE15"); break;
        #endif
        #ifdef PF0
            case PF0: printer->print("PF0"); break;
        #endif
        #ifdef PF1
            case PF1: printer->print("PF1"); break;
        #endif
        #ifdef PF2
            case PF2: printer->print("PF2"); break;
        #endif
        #ifdef PF3
            case PF3: printer->print("PF3"); break;
        #endif
        #ifdef PF4
            case PF4: printer->print("PF4"); break;
        #endif
        #ifdef PF5
            case PF5: printer->print("PF5"); break;
        #endif
        #ifdef PF6
            case PF6: printer->print("PF6"); break;
        #endif
        #ifdef PF7
            case PF7: printer->print("PF7"); break;
        #endif
        #ifdef PF8
            case PF8: printer->print("PF8"); break;
        #endif
        #ifdef PF9
            case PF9: printer->print("PF9"); break;
        #endif
        #ifdef PF10
            case PF10: printer->print("PF10"); break;
        #endif
        #ifdef PF11
            case PF11: printer->print("PF11"); break;
        #endif
        #ifdef PF12
            case PF12: printer->print("PF12"); break;
        #endif
        #ifdef PF13
            case PF13: printer->print("PF13"); break;
        #endif
        #ifdef PF14
            case PF14: printer->print("PF14"); break;
        #endif
        #ifdef PF15
            case PF15: printer->print("PF15"); break;
        #endif
        #ifdef PG0
            case PG0: printer->print("PG0"); break;
        #endif
        #ifdef PG1
            case PG1: printer->print("PG1"); break;
        #endif
        #ifdef PG2
            case PG2: printer->print("PG2"); break;
        #endif
        #ifdef PG3
            case PG3: printer->print("PG3"); break;
        #endif
        #ifdef PG4
            case PG4: printer->print("PG4"); break;
        #endif
        #ifdef PG5
            case PG5: printer->print("PG5"); break;
        #endif
        #ifdef PG6
            case PG6: printer->print("PG6"); break;
        #endif
        #ifdef PG7
            case PG7: printer->print("PG7"); break;
        #endif
        #ifdef PG8
            case PG8: printer->print("PG8"); break;
        #endif
        #ifdef PG9
            case PG9: printer->print("PG9"); break;
        #endif
        #ifdef PG10
            case PG10: printer->print("PG10"); break;
        #endif
        #ifdef PG11
            case PG11: printer->print("PG11"); break;
        #endif
        #ifdef PG12
            case PG12: printer->print("PG12"); break;
        #endif
        #ifdef PG13
            case PG13: printer->print("PG13"); break;
        #endif
        #ifdef PG14
            case PG14: printer->print("PG14"); break;
        #endif
        #ifdef PG15
            case PG15: printer->print("PG15"); break;
        #endif
        #ifdef PH0
            case PH0: printer->print("PH0"); break;
        #endif
        #ifdef PH1
            case PH1: printer->print("PH1"); break;
        #endif
        #ifdef PH2
            case PH2: printer->print("PH2"); break;
        #endif
        #ifdef PH3
            case PH3: printer->print("PH3"); break;
        #endif
        #ifdef PH4
            case PH4: printer->print("PH4"); break;
        #endif
        #ifdef PH5
            case PH5: printer->print("PH5"); break;
        #endif
        #ifdef PH6
            case PH6: printer->print("PH6"); break;
        #endif
        #ifdef PH7
            case PH7: printer->print("PH7"); break;
        #endif
        #ifdef PH8
            case PH8: printer->print("PH8"); break;
        #endif
        #ifdef PH9
            case PH9: printer->print("PH9"); break;
        #endif
        #ifdef PH10
            case PH10: printer->print("PH10"); break;
        #endif
        #ifdef PH11
            case PH11: printer->print("PH11"); break;
        #endif
        #ifdef PH12
            case PH12: printer->print("PH12"); break;
        #endif
        #ifdef PH13
            case PH13: printer->print("PH13"); break;
        #endif
        #ifdef PH14
            case PH14: printer->print("PH14"); break;
        #endif
        #ifdef PH15
            case PH15: printer->print("PH15"); break;
        #endif
        #ifdef PI0
            case PI0: printer->print("PI0"); break;
        #endif
        #ifdef PI1
            case PI1: printer->print("PI1"); break;
        #endif
        #ifdef PI2
            case PI2: printer->print("PI2"); break;
        #endif
        #ifdef PI3
            case PI3: printer->print("PI3"); break;
        #endif
        #ifdef PI4
            case PI4: printer->print("PI4"); break;
        #endif
        #ifdef PI5
            case PI5: printer->print("PI5"); break;
        #endif
        #ifdef PI6
            case PI6: printer->print("PI6"); break;
        #endif
        #ifdef PI7
            case PI7: printer->print("PI7"); break;
        #endif
        #ifdef PI8
            case PI8: printer->print("PI8"); break;
        #endif
        #ifdef PI9
            case PI9: printer->print("PI9"); break;
        #endif
        #ifdef PI10
            case PI10: printer->print("PI10"); break;
        #endif
        #ifdef PI11
            case PI11: printer->print("PI11"); break;
        #endif
        #ifdef PI12
            case PI12: printer->print("PI12"); break;
        #endif
        #ifdef PI13
            case PI13: printer->print("PI13"); break;
        #endif
        #ifdef PI14
            case PI14: printer->print("PI14"); break;
        #endif
        #ifdef PI15
            case PI15: printer->print("PI15"); break;
        #endif
        #ifdef PJ0
            case PJ0: printer->print("PJ0"); break;
        #endif
        #ifdef PJ1
            case PJ1: printer->print("PJ1"); break;
        #endif
        #ifdef PJ2
            case PJ2: printer->print("PJ2"); break;
        #endif
        #ifdef PJ3
            case PJ3: printer->print("PJ3"); break;
        #endif
        #ifdef PJ4
            case PJ4: printer->print("PJ4"); break;
        #endif
        #ifdef PJ5
            case PJ5: printer->print("PJ5"); break;
        #endif
        #ifdef PJ6
            case PJ6: printer->print("PJ6"); break;
        #endif
        #ifdef PJ7
            case PJ7: printer->print("PJ7"); break;
        #endif
        #ifdef PJ8
            case PJ8: printer->print("PJ8"); break;
        #endif
        #ifdef PJ9
            case PJ9: printer->print("PJ9"); break;
        #endif
        #ifdef PJ10
            case PJ10: printer->print("PJ10"); break;
        #endif
        #ifdef PJ11
            case PJ11: printer->print("PJ11"); break;
        #endif
        #ifdef PJ12
            case PJ12: printer->print("PJ12"); break;
        #endif
        #ifdef PJ13
            case PJ13: printer->print("PJ13"); break;
        #endif
        #ifdef PJ14
            case PJ14: printer->print("PJ14"); break;
        #endif
        #ifdef PJ15
            case PJ15: printer->print("PJ15"); break;
        #endif
        #ifdef PK0
            case PK0: printer->print("PK0"); break;
        #endif
        #ifdef PK1
            case PK1: printer->print("PK1"); break;
        #endif
        #ifdef PK2
            case PK2: printer->print("PK2"); break;
        #endif
        #ifdef PK3
            case PK3: printer->print("PK3"); break;
        #endif
        #ifdef PK4
            case PK4: printer->print("PK4"); break;
        #endif
        #ifdef PK5
            case PK5: printer->print("PK5"); break;
        #endif
        #ifdef PK6
            case PK6: printer->print("PK6"); break;
        #endif
        #ifdef PK7
            case PK7: printer->print("PK7"); break;
        #endif
        #ifdef PK8
            case PK8: printer->print("PK8"); break;
        #endif
        #ifdef PK9
            case PK9: printer->print("PK9"); break;
        #endif
        #ifdef PK10
            case PK10: printer->print("PK10"); break;
        #endif
        #ifdef PK11
            case PK11: printer->print("PK11"); break;
        #endif
        #ifdef PK12
            case PK12: printer->print("PK12"); break;
        #endif
        #ifdef PK13
            case PK13: printer->print("PK13"); break;
        #endif
        #ifdef PK14
            case PK14: printer->print("PK14"); break;
        #endif
        #ifdef PK15
            case PK15: printer->print("PK15"); break;
        #endif
        #ifdef PL0
            case PL0: printer->print("PL0"); break;
        #endif
        #ifdef PL1
            case PL1: printer->print("PL1"); break;
        #endif
        #ifdef PL2
            case PL2: printer->print("PL2"); break;
        #endif
        #ifdef PL3
            case PL3: printer->print("PL3"); break;
        #endif
        #ifdef PL4
            case PL4: printer->print("PL4"); break;
        #endif
        #ifdef PL5
            case PL5: printer->print("PL5"); break;
        #endif
        #ifdef PL6
            case PL6: printer->print("PL6"); break;
        #endif
        #ifdef PL7
            case PL7: printer->print("PL7"); break;
        #endif
        #ifdef PL8
            case PL8: printer->print("PL8"); break;
        #endif
        #ifdef PL9
            case PL9: printer->print("PL9"); break;
        #endif
        #ifdef PL10
            case PL10: printer->print("PL10"); break;
        #endif
        #ifdef PL11
            case PL11: printer->print("PL11"); break;
        #endif
        #ifdef PL12
            case PL12: printer->print("PL12"); break;
        #endif
        #ifdef PL13
            case PL13: printer->print("PL13"); break;
        #endif
        #ifdef PL14
            case PL14: printer->print("PL14"); break;
        #endif
        #ifdef PL15
            case PL15: printer->print("PL15"); break;
        #endif
        #ifdef PM0
            case PM0: printer->print("PM0"); break;
        #endif
        #ifdef PM1
            case PM1: printer->print("PM1"); break;
        #endif
        #ifdef PM2
            case PM2: printer->print("PM2"); break;
        #endif
        #ifdef PM3
            case PM3: printer->print("PM3"); break;
        #endif
        #ifdef PM4
            case PM4: printer->print("PM4"); break;
        #endif
        #ifdef PM5
            case PM5: printer->print("PM5"); break;
        #endif
        #ifdef PM6
            case PM6: printer->print("PM6"); break;
        #endif
        #ifdef PM7
            case PM7: printer->print("PM7"); break;
        #endif
        #ifdef PM8
            case PM8: printer->print("PM8"); break;
        #endif
        #ifdef PM9
            case PM9: printer->print("PM9"); break;
        #endif
        #ifdef PM10
            case PM10: printer->print("PM10"); break;
        #endif
        #ifdef PM11
            case PM11: printer->print("PM11"); break;
        #endif
        #ifdef PM12
            case PM12: printer->print("PM12"); break;
        #endif
        #ifdef PM13
            case PM13: printer->print("PM13"); break;
        #endif
        #ifdef PM14
            case PM14: printer->print("PM14"); break;
        #endif
        #ifdef PM15
            case PM15: printer->print("PM15"); break;
        #endif
        #ifdef PN0
            case PN0: printer->print("PN0"); break;
        #endif
        #ifdef PN1
            case PN1: printer->print("PN1"); break;
        #endif
        #ifdef PN2
            case PN2: printer->print("PN2"); break;
        #endif
        #ifdef PN3
            case PN3: printer->print("PN3"); break;
        #endif
        #ifdef PN4
            case PN4: printer->print("PN4"); break;
        #endif
        #ifdef PN5
            case PN5: printer->print("PN5"); break;
        #endif
        #ifdef PN6
            case PN6: printer->print("PN6"); break;
        #endif
        #ifdef PN7
            case PN7: printer->print("PN7"); break;
        #endif
        #ifdef PN8
            case PN8: printer->print("PN8"); break;
        #endif
        #ifdef PN9
            case PN9: printer->print("PN9"); break;
        #endif
        #ifdef PN10
            case PN10: printer->print("PN10"); break;
        #endif
        #ifdef PN11
            case PN11: printer->print("PN11"); break;
        #endif
        #ifdef PN12
            case PN12: printer->print("PN12"); break;
        #endif
        #ifdef PN13
            case PN13: printer->print("PN13"); break;
        #endif
        #ifdef PN14
            case PN14: printer->print("PN14"); break;
        #endif
        #ifdef PN15
            case PN15: printer->print("PN15"); break;
        #endif
        #ifdef PO0
            case PO0: printer->print("PO0"); break;
        #endif
        #ifdef PO1
            case PO1: printer->print("PO1"); break;
        #endif
        #ifdef PO2
            case PO2: printer->print("PO2"); break;
        #endif
        #ifdef PO3
            case PO3: printer->print("PO3"); break;
        #endif
        #ifdef PO4
            case PO4: printer->print("PO4"); break;
        #endif
        #ifdef PO5
            case PO5: printer->print("PO5"); break;
        #endif
        #ifdef PO6
            case PO6: printer->print("PO6"); break;
        #endif
        #ifdef PO7
            case PO7: printer->print("PO7"); break;
        #endif
        #ifdef PO8
            case PO8: printer->print("PO8"); break;
        #endif
        #ifdef PO9
            case PO9: printer->print("PO9"); break;
        #endif
        #ifdef PO10
            case PO10: printer->print("PO10"); break;
        #endif
        #ifdef PO11
            case PO11: printer->print("PO11"); break;
        #endif
        #ifdef PO12
            case PO12: printer->print("PO12"); break;
        #endif
        #ifdef PO13
            case PO13: printer->print("PO13"); break;
        #endif
        #ifdef PO14
            case PO14: printer->print("PO14"); break;
        #endif
        #ifdef PO15
            case PO15: printer->print("PO15"); break;
        #endif
        #ifdef PP0
            case PP0: printer->print("PP0"); break;
        #endif
        #ifdef PP1
            case PP1: printer->print("PP1"); break;
        #endif
        #ifdef PP2
            case PP2: printer->print("PP2"); break;
        #endif
        #ifdef PP3
            case PP3: printer->print("PP3"); break;
        #endif
        #ifdef PP4
            case PP4: printer->print("PP4"); break;
        #endif
        #ifdef PP5
            case PP5: printer->print("PP5"); break;
        #endif
        #ifdef PP6
            case PP6: printer->print("PP6"); break;
        #endif
        #ifdef PP7
            case PP7: printer->print("PP7"); break;
        #endif
        #ifdef PP8
            case PP8: printer->print("PP8"); break;
        #endif
        #ifdef PP9
            case PP9: printer->print("PP9"); break;
        #endif
        #ifdef PP10
            case PP10: printer->print("PP10"); break;
        #endif
        #ifdef PP11
            case PP11: printer->print("PP11"); break;
        #endif
        #ifdef PP12
            case PP12: printer->print("PP12"); break;
        #endif
        #ifdef PP13
            case PP13: printer->print("PP13"); break;
        #endif
        #ifdef PP14
            case PP14: printer->print("PP14"); break;
        #endif
        #ifdef PP15
            case PP15: printer->print("PP15"); break;
        #endif
        #ifdef PQ0
            case PQ0: printer->print("PQ0"); break;
        #endif
        #ifdef PQ1
            case PQ1: printer->print("PQ1"); break;
        #endif
        #ifdef PQ2
            case PQ2: printer->print("PQ2"); break;
        #endif
        #ifdef PQ3
            case PQ3: printer->print("PQ3"); break;
        #endif
        #ifdef PQ4
            case PQ4: printer->print("PQ4"); break;
        #endif
        #ifdef PQ5
            case PQ5: printer->print("PQ5"); break;
        #endif
        #ifdef PQ6
            case PQ6: printer->print("PQ6"); break;
        #endif
        #ifdef PQ7
            case PQ7: printer->print("PQ7"); break;
        #endif
        #ifdef PQ8
            case PQ8: printer->print("PQ8"); break;
        #endif
        #ifdef PQ9
            case PQ9: printer->print("PQ9"); break;
        #endif
        #ifdef PQ10
            case PQ10: printer->print("PQ10"); break;
        #endif
        #ifdef PQ11
            case PQ11: printer->print("PQ11"); break;
        #endif
        #ifdef PQ12
            case PQ12: printer->print("PQ12"); break;
        #endif
        #ifdef PQ13
            case PQ13: printer->print("PQ13"); break;
        #endif
        #ifdef PQ14
            case PQ14: printer->print("PQ14"); break;
        #endif
        #ifdef PQ15
            case PQ15: printer->print("PQ15"); break;
        #endif
        #ifdef PR0
            case PR0: printer->print("PR0"); break;
        #endif
        #ifdef PR1
            case PR1: printer->print("PR1"); break;
        #endif
        #ifdef PR2
            case PR2: printer->print("PR2"); break;
        #endif
        #ifdef PR3
            case PR3: printer->print("PR3"); break;
        #endif
        #ifdef PR4
            case PR4: printer->print("PR4"); break;
        #endif
        #ifdef PR5
            case PR5: printer->print("PR5"); break;
        #endif
        #ifdef PR6
            case PR6: printer->print("PR6"); break;
        #endif
        #ifdef PR7
            case PR7: printer->print("PR7"); break;
        #endif
        #ifdef PR8
            case PR8: printer->print("PR8"); break;
        #endif
        #ifdef PR9
            case PR9: printer->print("PR9"); break;
        #endif
        #ifdef PR10
            case PR10: printer->print("PR10"); break;
        #endif
        #ifdef PR11
            case PR11: printer->print("PR11"); break;
        #endif
        #ifdef PR12
            case PR12: printer->print("PR12"); break;
        #endif
        #ifdef PR13
            case PR13: printer->print("PR13"); break;
        #endif
        #ifdef PR14
            case PR14: printer->print("PR14"); break;
        #endif
        #ifdef PR15
            case PR15: printer->print("PR15"); break;
        #endif
        #ifdef PS0
            case PS0: printer->print("PS0"); break;
        #endif
        #ifdef PS1
            case PS1: printer->print("PS1"); break;
        #endif
        #ifdef PS2
            case PS2: printer->print("PS2"); break;
        #endif
        #ifdef PS3
            case PS3: printer->print("PS3"); break;
        #endif
        #ifdef PS4
            case PS4: printer->print("PS4"); break;
        #endif
        #ifdef PS5
            case PS5: printer->print("PS5"); break;
        #endif
        #ifdef PS6
            case PS6: printer->print("PS6"); break;
        #endif
        #ifdef PS7
            case PS7: printer->print("PS7"); break;
        #endif
        #ifdef PS8
            case PS8: printer->print("PS8"); break;
        #endif
        #ifdef PS9
            case PS9: printer->print("PS9"); break;
        #endif
        #ifdef PS10
            case PS10: printer->print("PS10"); break;
        #endif
        #ifdef PS11
            case PS11: printer->print("PS11"); break;
        #endif
        #ifdef PS12
            case PS12: printer->print("PS12"); break;
        #endif
        #ifdef PS13
            case PS13: printer->print("PS13"); break;
        #endif
        #ifdef PS14
            case PS14: printer->print("PS14"); break;
        #endif
        #ifdef PS15
            case PS15: printer->print("PS15"); break;
        #endif
        #ifdef PT0
            case PT0: printer->print("PT0"); break;
        #endif
        #ifdef PT1
            case PT1: printer->print("PT1"); break;
        #endif
        #ifdef PT2
            case PT2: printer->print("PT2"); break;
        #endif
        #ifdef PT3
            case PT3: printer->print("PT3"); break;
        #endif
        #ifdef PT4
            case PT4: printer->print("PT4"); break;
        #endif
        #ifdef PT5
            case PT5: printer->print("PT5"); break;
        #endif
        #ifdef PT6
            case PT6: printer->print("PT6"); break;
        #endif
        #ifdef PT7
            case PT7: printer->print("PT7"); break;
        #endif
        #ifdef PT8
            case PT8: printer->print("PT8"); break;
        #endif
        #ifdef PT9
            case PT9: printer->print("PT9"); break;
        #endif
        #ifdef PT10
            case PT10: printer->print("PT10"); break;
        #endif
        #ifdef PT11
            case PT11: printer->print("PT11"); break;
        #endif
        #ifdef PT12
            case PT12: printer->print("PT12"); break;
        #endif
        #ifdef PT13
            case PT13: printer->print("PT13"); break;
        #endif
        #ifdef PT14
            case PT14: printer->print("PT14"); break;
        #endif
        #ifdef PT15
            case PT15: printer->print("PT15"); break;
        #endif
        #ifdef PU0
            case PU0: printer->print("PU0"); break;
        #endif
        #ifdef PU1
            case PU1: printer->print("PU1"); break;
        #endif
        #ifdef PU2
            case PU2: printer->print("PU2"); break;
        #endif
        #ifdef PU3
            case PU3: printer->print("PU3"); break;
        #endif
        #ifdef PU4
            case PU4: printer->print("PU4"); break;
        #endif
        #ifdef PU5
            case PU5: printer->print("PU5"); break;
        #endif
        #ifdef PU6
            case PU6: printer->print("PU6"); break;
        #endif
        #ifdef PU7
            case PU7: printer->print("PU7"); break;
        #endif
        #ifdef PU8
            case PU8: printer->print("PU8"); break;
        #endif
        #ifdef PU9
            case PU9: printer->print("PU9"); break;
        #endif
        #ifdef PU10
            case PU10: printer->print("PU10"); break;
        #endif
        #ifdef PU11
            case PU11: printer->print("PU11"); break;
        #endif
        #ifdef PU12
            case PU12: printer->print("PU12"); break;
        #endif
        #ifdef PU13
            case PU13: printer->print("PU13"); break;
        #endif
        #ifdef PU14
            case PU14: printer->print("PU14"); break;
        #endif
        #ifdef PU15
            case PU15: printer->print("PU15"); break;
        #endif
        #ifdef PV0
            case PV0: printer->print("PV0"); break;
        #endif
        #ifdef PV1
            case PV1: printer->print("PV1"); break;
        #endif
        #ifdef PV2
            case PV2: printer->print("PV2"); break;
        #endif
        #ifdef PV3
            case PV3: printer->print("PV3"); break;
        #endif
        #ifdef PV4
            case PV4: printer->print("PV4"); break;
        #endif
        #ifdef PV5
            case PV5: printer->print("PV5"); break;
        #endif
        #ifdef PV6
            case PV6: printer->print("PV6"); break;
        #endif
        #ifdef PV7
            case PV7: printer->print("PV7"); break;
        #endif
        #ifdef PV8
            case PV8: printer->print("PV8"); break;
        #endif
        #ifdef PV9
            case PV9: printer->print("PV9"); break;
        #endif
        #ifdef PV10
            case PV10: printer->print("PV10"); break;
        #endif
        #ifdef PV11
            case PV11: printer->print("PV11"); break;
        #endif
        #ifdef PV12
            case PV12: printer->print("PV12"); break;
        #endif
        #ifdef PV13
            case PV13: printer->print("PV13"); break;
        #endif
        #ifdef PV14
            case PV14: printer->print("PV14"); break;
        #endif
        #ifdef PV15
            case PV15: printer->print("PV15"); break;
        #endif
        #ifdef PW0
            case PW0: printer->print("PW0"); break;
        #endif
        #ifdef PW1
            case PW1: printer->print("PW1"); break;
        #endif
        #ifdef PW2
            case PW2: printer->print("PW2"); break;
        #endif
        #ifdef PW3
            case PW3: printer->print("PW3"); break;
        #endif
        #ifdef PW4
            case PW4: printer->print("PW4"); break;
        #endif
        #ifdef PW5
            case PW5: printer->print("PW5"); break;
        #endif
        #ifdef PW6
            case PW6: printer->print("PW6"); break;
        #endif
        #ifdef PW7
            case PW7: printer->print("PW7"); break;
        #endif
        #ifdef PW8
            case PW8: printer->print("PW8"); break;
        #endif
        #ifdef PW9
            case PW9: printer->print("PW9"); break;
        #endif
        #ifdef PW10
            case PW10: printer->print("PW10"); break;
        #endif
        #ifdef PW11
            case PW11: printer->print("PW11"); break;
        #endif
        #ifdef PW12
            case PW12: printer->print("PW12"); break;
        #endif
        #ifdef PW13
            case PW13: printer->print("PW13"); break;
        #endif
        #ifdef PW14
            case PW14: printer->print("PW14"); break;
        #endif
        #ifdef PW15
            case PW15: printer->print("PW15"); break;
        #endif
        #ifdef PX0
            case PX0: printer->print("PX0"); break;
        #endif
        #ifdef PX1
            case PX1: printer->print("PX1"); break;
        #endif
        #ifdef PX2
            case PX2: printer->print("PX2"); break;
        #endif
        #ifdef PX3
            case PX3: printer->print("PX3"); break;
        #endif
        #ifdef PX4
            case PX4: printer->print("PX4"); break;
        #endif
        #ifdef PX5
            case PX5: printer->print("PX5"); break;
        #endif
        #ifdef PX6
            case PX6: printer->print("PX6"); break;
        #endif
        #ifdef PX7
            case PX7: printer->print("PX7"); break;
        #endif
        #ifdef PX8
            case PX8: printer->print("PX8"); break;
        #endif
        #ifdef PX9
            case PX9: printer->print("PX9"); break;
        #endif
        #ifdef PX10
            case PX10: printer->print("PX10"); break;
        #endif
        #ifdef PX11
            case PX11: printer->print("PX11"); break;
        #endif
        #ifdef PX12
            case PX12: printer->print("PX12"); break;
        #endif
        #ifdef PX13
            case PX13: printer->print("PX13"); break;
        #endif
        #ifdef PX14
            case PX14: printer->print("PX14"); break;
        #endif
        #ifdef PX15
            case PX15: printer->print("PX15"); break;
        #endif
        #ifdef PY0
            case PY0: printer->print("PY0"); break;
        #endif
        #ifdef PY1
            case PY1: printer->print("PY1"); break;
        #endif
        #ifdef PY2
            case PY2: printer->print("PY2"); break;
        #endif
        #ifdef PY3
            case PY3: printer->print("PY3"); break;
        #endif
        #ifdef PY4
            case PY4: printer->print("PY4"); break;
        #endif
        #ifdef PY5
            case PY5: printer->print("PY5"); break;
        #endif
        #ifdef PY6
            case PY6: printer->print("PY6"); break;
        #endif
        #ifdef PY7
            case PY7: printer->print("PY7"); break;
        #endif
        #ifdef PY8
            case PY8: printer->print("PY8"); break;
        #endif
        #ifdef PY9
            case PY9: printer->print("PY9"); break;
        #endif
        #ifdef PY10
            case PY10: printer->print("PY10"); break;
        #endif
        #ifdef PY11
            case PY11: printer->print("PY11"); break;
        #endif
        #ifdef PY12
            case PY12: printer->print("PY12"); break;
        #endif
        #ifdef PY13
            case PY13: printer->print("PY13"); break;
        #endif
        #ifdef PY14
            case PY14: printer->print("PY14"); break;
        #endif
        #ifdef PY15
            case PY15: printer->print("PY15"); break;
        #endif
        #ifdef PZ0
            case PZ0: printer->print("PZ0"); break;
        #endif
        #ifdef PZ1
            case PZ1: printer->print("PZ1"); break;
        #endif
        #ifdef PZ2
            case PZ2: printer->print("PZ2"); break;
        #endif
        #ifdef PZ3
            case PZ3: printer->print("PZ3"); break;
        #endif
        #ifdef PZ4
            case PZ4: printer->print("PZ4"); break;
        #endif
        #ifdef PZ5
            case PZ5: printer->print("PZ5"); break;
        #endif
        #ifdef PZ6
            case PZ6: printer->print("PZ6"); break;
        #endif
        #ifdef PZ7
            case PZ7: printer->print("PZ7"); break;
        #endif
        #ifdef PZ8
            case PZ8: printer->print("PZ8"); break;
        #endif
        #ifdef PZ9
            case PZ9: printer->print("PZ9"); break;
        #endif
        #ifdef PZ10
            case PZ10: printer->print("PZ10"); break;
        #endif
        #ifdef PZ11
            case PZ11: printer->print("PZ11"); break;
        #endif
        #ifdef PZ12
            case PZ12: printer->print("PZ12"); break;
        #endif
        #ifdef PZ13
            case PZ13: printer->print("PZ13"); break;
        #endif
        #ifdef PZ14
            case PZ14: printer->print("PZ14"); break;
        #endif
        #ifdef PZ15
            case PZ15: printer->print("PZ15"); break;
        #endif
        default: printer->print("P??"); break;
    }
}
