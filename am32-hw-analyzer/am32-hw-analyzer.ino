#include "config.h"
#include "defs.h"

#include <SoftwareSerial.h>

#include "master_pin_list.h"
#include "hw_pin_lists.h"

SoftwareSerial** ser_list  = NULL;  // contains a list of SoftwareSerial instances
SoftwareSerial*  printer   = NULL;  // this will point to one of the serial ports that's actually working
uint32_t rc_input = PIN_NOT_EXIST;  // which pin is RC input
uint16_t rc_pin_cnt = 0;            // number of potential RC input pins
uint32_t printer_pin;               // used to prevent ADC reading of a serial pin

uint32_t remaining_pins[32];        // all pins except the ones used for the phases
uint32_t remaining_adcs[32];        // all pins remaining that are ADC capable

phase_group_t* phase_groups = NULL; // will list all phase pin groups
uint16_t phase_groups_cnt   = 0;    // number of potential groups
uint32_t phase_groups_sz    = 0;    // size of list in bytes, used for memset
uint8_t  results_invalid    = 0;    // marks the test as invalid if something goes wrong

uint16_t state_machine = STATEMACH_WAITING_START;

void setup()
{
    rc_pin_cnt = count_tbl(all_rc);
    ser_list = (SoftwareSerial**)malloc(sizeof(SoftwareSerial*) * rc_pin_cnt);
    for (uint8_t i = 0; i < rc_pin_cnt; i++)
    {
        uint32_t p = all_rc[i];
        if (p == END_OF_LIST)
        {
            break;
        }
        ser_list[i] = new SoftwareSerial(p, p);
        ser_list[i]->begin(SERIAL_BAUD_RATE);
    }

    // remove pins from the list, to make searching easier later
    memcpy(remaining_pins, all_pins, sizeof(all_pins));
    filter_out((uint32_t*)remaining_pins, all_hin);
    filter_out((uint32_t*)remaining_pins, all_lin);
    filter_out((uint32_t*)remaining_pins, all_fb );

    // prepare the list of phase pin groups
    phase_groups_cnt = count_tbl(all_hin);
    phase_groups_sz = sizeof(phase_group_t) * phase_groups_cnt;
    phase_groups = (phase_group_t*)malloc(phase_groups_sz);
    memset(phase_groups, END_OF_LIST, phase_groups_sz);

    #ifdef ASSUME_PA15_LED
    // according to AM32 source code, this is permanently a LED
    pinMode(PA15, OUTPUT);
    digitalWrite(PA15, LOW);
    filter_out_one((uint32_t*)remaining_pins, PA15);
    #endif
}

void loop()
{
    static uint32_t tick = 0;      // state machine sub counter
    static uint32_t last_time = 0; // used to time state transitions
    static uint32_t hftick = 0;    // used to toggle pins at high frequency
    uint32_t now = millis();

    hftick++;

    if (state_machine == STATEMACH_WAITING_START)
    {
        if ((now - last_time) >= 1000)
        {
            // have to swap between different pins because only one SoftwareSerial instance can be listening at a time

            last_time = now;

            printer = ser_list[tick];
            printer_pin = all_rc[tick];
            ser_list[tick]->print("RC input is ");
            print_pin_name(all_rc[tick]);
            ser_list[tick]->println(" - send \\n (or ENTER key) to continue");
            ser_list[tick]->listen(); // this is only listening for 1 second
            tick++;
            if (tick >= rc_pin_cnt) { // end of list
                tick = 0;
            }
        }

        // which ever one gets the enter key (Arduino IDE, press SEND button with newline enabled)
        // will be the one that's valid
        for (uint8_t i = 0; i < rc_pin_cnt; i++)
        {
            if (ser_list[i]->available() > 0)
            {
                char c = ser_list[i]->read();
                if (c == '\n') // if enter key (or sent \n)
                {
                    rc_input = all_rc[i];
                    printer_pin = rc_input;
                    filter_out_one((uint32_t*)remaining_pins, rc_input); // remove the RC input pin from potential searched pins
                    printer = ser_list[i];
                    state_machine = STATEMACH_TEST_PHASE_START;
                    // deactivate all other SoftwareSerial instances
                    for (uint8_t j = 0; j < rc_pin_cnt; j++)
                    {
                        if (i != j) {
                            ser_list[j]->end();
                            delete ser_list[j];
                        }
                    }
                    free(ser_list);
                    break;
                }
            }
        }
    }
    else if (state_machine == STATEMACH_TEST_PHASE_START)
    {
        printer->println("test continuing");
        printer->print("pin list: ");
        print_all_pins(remaining_pins);
        // prep for testing, all pins output low
        set_all_pins(all_hin, OUTPUT, LOW);
        set_all_pins(all_lin, OUTPUT, LOW);
        set_all_pins(all_fb,  INPUT,  LOW);
        state_machine = STATEMACH_TEST_PHASE_A;
        tick = 0;
        printer->println("started to test HIN pins");

    }
    else if (state_machine == STATEMACH_TEST_PHASE_A)
    {
        uint32_t p = all_hin[tick];
        if (p == END_OF_LIST)
        {
            set_all_pins(all_hin, OUTPUT, HIGH);
            set_all_pins(all_lin, OUTPUT, LOW);
            printer->println("started to test LIN pins");
            state_machine = STATEMACH_TEST_PHASE_B;
            tick = 0;
            delay(500); // cool down in case of HW mistake
            return;
        }
        uint16_t adc_res[32];

        pulse_all_phases(10, 20); // for charge pump
        set_all_pins(all_hin, OUTPUT, LOW);

        // pulse each HIN pin, see if ADC readings change
        pulse_and_measure(p, 100, 200, adc_res);
        //print_adc_res(adc_res);

        if (test_for_motor_windings_hin(adc_res) && results_invalid == 0)
        {
            printer->println("WARNING: readings make no sense, is the motor connected? please make sure the motor is disconnected.");
            results_invalid = 1;
        }

        uint32_t fb = pick_max(adc_res);

        phase_groups[tick].hin = p;
        phase_groups[tick].fb = fb;

        tick++;
        delay(500); // cool down in case of HW mistake
    }
    else if (state_machine == STATEMACH_TEST_PHASE_B)
    {
        uint32_t p = all_lin[tick];
        if (p == END_OF_LIST)
        {
            set_all_pins(all_hin, OUTPUT, LOW);
            set_all_pins(all_lin, OUTPUT, LOW);
            state_machine = STATEMACH_TEST_REPORT_PHASES;
            tick = 0;
            return;
        }
        uint16_t adc_res[32];

        pulse_all_phases(10, 20); // for charge pump
        set_all_pins(all_hin, OUTPUT, LOW);

        // pulse each LIN pin, see if ADC readings change
        pulse_and_measure(p, 2, 200, adc_res);
        //print_adc_res(adc_res);

        if (test_for_motor_windings_lin(adc_res) && results_invalid <= 1)
        {
            printer->println("WARNING: readings make no sense, is the motor connected? please make sure the motor is disconnected.");
            results_invalid = 2;
        }

        uint32_t fb = pick_min(adc_res);

        // match the FB pin with an existing group, so we can save the LIN pin in the correct group
        for (int i = 0; i < phase_groups_cnt; i++)
        {
            phase_group_t* pg = &(phase_groups[i]);
            if (pg->fb == fb)
            {
                pg->lin = p;
            }
        }
        tick++;
        delay(500); // cool down in case of HW mistake
    }
    else if (state_machine == STATEMACH_TEST_REPORT_PHASES)
    {
        printer->println("phase pin matching results:");
        for (uint8_t i = 0; i < phase_groups_cnt; i++)
        {
            phase_group_t* pg = &(phase_groups[i]);
            printer->printf("\t%d : HIN->", (i + 1));
            print_pin_name(pg->hin);
            printer->print(" , LIN->");
            print_pin_name(pg->lin);
            printer->print(" , FB->");
            print_pin_name(pg->fb);
            printer->println();
        }
        printer->println("end of list");

        if (results_invalid)
        {
            printer->println("WARNING: these results may not be valid!!!");
        }

        #ifdef ASSUME_ADC_PINS
        test_for_analog_pins();
        #endif

        state_machine = STATEMACH_CMD_PROMPT;
    }
    else if (state_machine == STATEMACH_CMD_PROMPT)
    {
        while (printer->available() > 0) { printer->read(); } // clear input buffer
        printer->println("enter command key, one of: V (voltage monitor), C (current sensor monitor), L (LED search)");
        printer->listen();
        state_machine = STATEMACH_CMD_WAIT;
    }
    else if (state_machine == STATEMACH_CMD_WAIT)
    {
        if (printer->available() > 0)
        {
            // handle command keys
            char c = printer->read();
            if (c == 'V' || c == 'v')
            {
                printer->println("monitoring ADCs, X to quit");
                printer->listen();
                state_machine = STATEMACH_SEARCH_VOLTAGE;
                tick = 0;
                last_time = now;
            }
            else if (c == 'C' || c == 'c')
            {
                printer->println("monitoring ADCs while attempting to draw current, please connect motor, X to quit");
                printer->listen();
                state_machine = STATEMACH_SEARCH_CURRENTSENSOR;
                tick = 0;
                last_time = now;
            }
            else if (c == 'L' || c == 'l')
            {
                printer->println("searching LED pin, X to quit, P to pause, G to continue, < and > to nav");
                printer->println("if you see the LED actually blink, then press 'P', and use '<' or '>' if you missed it");
                printer->listen();
                state_machine = STATEMACH_SEARCH_LED;
                tick = 0;
                last_time = now;
            }
            else if (c == '\r' || c == '\n' || c == '\0' || c == ' ' || c == '\t')
            {
                // do nothing
            }
            else
            {
                printer->println("invalid cmd key");
                printer->listen();
                state_machine = STATEMACH_CMD_PROMPT;
            }
        }
    }
    else if (state_machine == STATEMACH_SEARCH_VOLTAGE)
    {
        // monitor all ADC readings to see fluctuations
        // works best with a variable voltage power supply

        if (check_search_exit()) {
            return;
        }
        if ((now - last_time) >= 1000)
        {
            last_time = now;
            print_all_adc();
            printer->println();
            printer->listen();
            tick++;
        }

        //set_all_pins(all_hin, OUTPUT, ((hftick % 2) == 0) ? HIGH : LOW);
        //set_all_pins(all_lin, OUTPUT, ((hftick % 2) == 0) ? LOW : HIGH);
    }
    else if (state_machine == STATEMACH_SEARCH_CURRENTSENSOR)
    {
        // intentionally cause current draw and monitor all ADC readings to see fluctuations

        if (check_search_exit()) {
            return;
        }
        if ((now - last_time) >= 100)
        {
            last_time = now;

            uint8_t tm = tick % 16;
            if (tm == 0)
            {
                pinMode(phase_groups[0].hin, OUTPUT);
                pinMode(phase_groups[0].lin, OUTPUT);
                pinMode(phase_groups[1].hin, OUTPUT);
                pinMode(phase_groups[1].lin, OUTPUT);
                // attempt to draw power
                digitalWrite(phase_groups[0].hin, LOW);
                analogWrite (phase_groups[0].lin, 200);
                analogWrite (phase_groups[1].hin, 200);
                digitalWrite(phase_groups[1].lin, LOW);
            }
            else if (tm == 2)
            {
                // attempt to draw power
                analogWrite (phase_groups[0].hin, 200);
                digitalWrite(phase_groups[0].lin, LOW);
                digitalWrite(phase_groups[1].hin, LOW);
                analogWrite (phase_groups[1].lin, 200);
            }
            else if (tm == 4)
            {
                // cool down, see if current drops
                digitalWrite(phase_groups[0].hin, LOW);
                digitalWrite(phase_groups[0].lin, LOW);
                digitalWrite(phase_groups[1].hin, LOW);
                digitalWrite(phase_groups[1].lin, LOW);
            }
            else if (tm == 5)
            {
                // cool down, see if current drops
                pinMode(phase_groups[0].hin, INPUT);
                pinMode(phase_groups[0].lin, INPUT);
                pinMode(phase_groups[1].hin, INPUT);
                pinMode(phase_groups[1].lin, INPUT);
            }

            if ((tm % 2) != 0)
            {
                print_all_adc();
                printer->println();
                printer->listen();
            }
            tick++;
        }
    }
    else if (state_machine == STATEMACH_SEARCH_LED)
    {
        static uint8_t led_continue = 1;
        if (printer->available() > 0)
        {
            char c = printer->read();
            if (c == 'X' || c == 'x')
            {
                printer->println("search exit");
                printer->listen();
                state_machine = STATEMACH_CMD_PROMPT;
                return;
            }
            else if (c == 'P' || c == 'p')
            {
                // press 'P' to pause the advancement
                led_continue = 0;
            }
            else if (c == 'G' || c == 'g')
            {
                // press 'P' to pause the advancement
                led_continue = 1;
            }
            else if (c == '<')
            {
                // previous
                tick = (tick == 0) ? 0 : (tick - 1);
                led_continue = 0; // pause
            }
            else if (c == '>')
            {
                // next
                tick++;
                if (remaining_pins[tick / 2] == END_OF_LIST)
                {
                    tick = 0;
                }
                led_continue = 0; // pause
            }
        }
        if ((now - last_time) >= 2000)
        {
            // every two seconds, another pin will be blinked
            // the user needs to watch the LED and also watch the serial terminal for the text

            last_time = now;
            uint32_t p = remaining_pins[tick / 2];
            if (p == END_OF_LIST)
            {
                printer->println("LED search looping");
                tick = 0;
                return;
            }
            printer->print("LED pin ");
            print_pin_name(p);
            printer->print(" blink ");
            if ((tick % 2) == 0) {
                printer->println("HIGH");
            }
            else {
                printer->println("LOW");
            }
            printer->listen();
            pinMode(p, OUTPUT);
            digitalWrite(p, (tick % 2) == 0 ? HIGH : LOW);
            delay(100); // short blink just in case we are driving something we shouldn't be
            pinMode(p, INPUT);
            digitalWrite(p, (tick % 2) == 0 ? LOW : HIGH);
            if (led_continue != 0) {
                tick++;
            }
            return;
        }
    }
}
