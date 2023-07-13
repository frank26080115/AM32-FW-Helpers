// check if the user wants to end a search mode
// if returned true, then do a state transition
bool check_search_exit()
{
    if (printer->available() > 0) {
        char c = printer->read();
        if (c == 'X' || c == 'x') {
            printer->println("search exit");
            printer->listen();
            state_machine = STATEMACH_CMD_PROMPT;
            return true;
        }
    }
    return false;
}

// set all pins a certain way
void set_all_pins(const uint32_t* lst, uint8_t mode, uint8_t pol)
{
    for (uint8_t i = 0; ; i++)
    {
        uint32_t p = lst[i];
        if (p == END_OF_LIST)
        {
            return;
        }
        pinMode(p, mode);
        if (mode == OUTPUT) {
            if (pol == 123) {
                analogWrite(p, pol);
            }
            else {
                digitalWrite(p, pol);
            }
        }
    }
}

// set all pins a certain way, except for one
void set_all_pins_except(const uint32_t* lst, uint32_t not_me, uint8_t mode, uint8_t pol)
{
    for (uint8_t i = 0; ; i++)
    {
        uint32_t p = lst[i];
        if (p == END_OF_LIST)
        {
            return;
        }
        if (p == not_me) {
            continue;
        }
        pinMode(p, mode);
        if (mode == OUTPUT) {
            digitalWrite(p, pol);
        }
    }
}

void pulse_all_phases(uint32_t dly, uint32_t cnt)
{
    for (uint8_t i = 0; i < cnt; i++)
    {
        set_all_pins(all_lin, OUTPUT, 123);
        set_all_pins(all_hin, OUTPUT, LOW);
        delay(dly);
        set_all_pins(all_lin, OUTPUT, LOW);
        set_all_pins(all_hin, OUTPUT, 123);
        delay(dly);
    }
}

// pulse a pin, measure all FB pins ADC values after the pulse
void pulse_and_measure(uint32_t outpin, uint32_t dly, uint8_t pol, uint16_t* res)
{
    if (pol != HIGH && pol != LOW)
    {
        analogWrite(outpin, pol);
    }
    else
    {
        digitalWrite(outpin, pol == LOW ? LOW : HIGH);
    }
    delay(dly);
    for (uint8_t i = 0; ; i++)
    {
        uint32_t p = all_fb[i];
        if (p == END_OF_LIST)
        {
            break;
        }
        int16_t x = analogRead(PINTRANSLATE(p));
        res[i] = x;
    }
    digitalWrite(outpin, pol == LOW ? HIGH : LOW);
}

// pick the FB pin that reported a value that's very different from the average
uint32_t pick_different(const uint16_t* adc_buf)
{
    uint32_t x = PIN_NOT_EXIST;
    uint16_t max_val = 0;
    uint32_t avg_sum = 0;
    uint32_t cnt = 0;

    for (uint8_t i = 0; ; i++)
    {
        uint32_t p = all_fb[i];
        if (p == END_OF_LIST)
        {
            break;
        }
        uint16_t v = adc_buf[i];
        avg_sum += v;
        cnt += 1;
    }
    avg_sum /= cnt;

    for (uint8_t i = 0; ; i++)
    {
        uint32_t p = all_fb[i];
        if (p == END_OF_LIST)
        {
            break;
        }
        uint16_t v = adc_buf[i];
        uint16_t diff = (v > avg_sum) ? (v - avg_sum) : (avg_sum - v);
        if (diff > max_val) {
            max_val = diff;
            x = p;
        }
    }
    return x;
}

// pick the FB pin that reported a value that's higher than the rest
uint32_t pick_max(const uint16_t* adc_buf)
{
    uint32_t x = PIN_NOT_EXIST;
    uint16_t max_val = 0;
    for (uint8_t i = 0; ; i++)
    {
        uint32_t p = all_fb[i];
        if (p == END_OF_LIST)
        {
            return x;
        }
        uint16_t v = adc_buf[i];
        if (v > max_val)
        {
            x = p;
            max_val = v;
        }
    }
    return x;
}

// pick the FB pin that reported a value that's lower than the rest
uint32_t pick_min(const uint16_t* adc_buf)
{
    uint32_t x = PIN_NOT_EXIST;
    int16_t min_val = -1;
    for (uint8_t i = 0; ; i++)
    {
        uint32_t p = all_fb[i];
        if (p == END_OF_LIST)
        {
            return x;
        }
        uint16_t v = adc_buf[i];
        if (v < min_val || min_val < 0)
        {
            x = p;
            min_val = v;
        }
    }
    return x;
}

// count number of entries in a pin table
uint16_t count_tbl(const uint32_t* input_list)
{
    for (int i = 0; ; i++)
    {
        uint32_t p = input_list[i];
        if (p == END_OF_LIST)
        {
            return i;
        }
    }
    return 0;
}

// remove entries from a list, based on a blacklist
void filter_out(uint32_t* list, const uint32_t* dont_want)
{
    int i, j, k;
    k = 0;
    for (i = 0; ; i++)
    {
        uint32_t p = list[i];
        if (p == END_OF_LIST)
        {
            break;
        }
        bool found = false;
        for (j = 0; ; j++)
        {
            uint32_t d = dont_want[j];
            if (d == END_OF_LIST) {
                break;
            }
            if (d == p) {
                found = true;
                break;
            }
        }
        if (found)
        {
            for (k = i; ; k++)
            {
                uint32_t pnext = list[k + 1];
                list[k] = pnext;
                if (pnext == END_OF_LIST) {
                    break;
                }
            }
            i--;
        }
    }
}

void filter_out_one(uint32_t* list, uint32_t dont_want)
{
    uint32_t tmp[2] = {dont_want, END_OF_LIST};
    filter_out(list, (const uint32_t*)tmp);
}

void filter_only_adcs(uint32_t* list)
{
    for (uint8_t i = 0; ; i++)
    {
        uint32_t p = list[i];
        if (p == END_OF_LIST)
        {
            break;
        }
        if (digitalpinIsAnalogInput(p) == false) {
            filter_out_one(list, p);
            i--;
        }
    }
}

bool pin_is_in_list(uint32_t pin, const uint32_t* list)
{
    for (uint8_t i = 0; ; i++)
    {
        uint32_t p = list[i];
        if (p == END_OF_LIST)
        {
            return false;
        }
        if (p == pin)
        {
            return true;
        }
    }
    return false;
}

void print_adc_res(const uint16_t* adc_buf)
{
    printer->print("ADC res: ");
    for (uint8_t i = 0; ; i++)
    {
        uint32_t p = all_fb[i];
        if (p == END_OF_LIST)
        {
            break;
        }
        uint16_t v = adc_buf[i];
        print_pin_name(p);
        printer->printf(" = %u ; ", v);
    }
    printer->println();
}

void print_all_pins(const uint32_t* lst)
{
    for (uint8_t i = 0; ; i++)
    {
        uint32_t p = lst[i];
        if (p == END_OF_LIST)
        {
            break;
        }
        print_pin_name(p);
        printer->print(" ");
    }
    printer->println("");
}

void print_all_adc()
{
    for (uint8_t i = 0; ; i++)
    {
        uint32_t p = remaining_pins[i];
        if (p == END_OF_LIST)
        {
            break;
        }
        if (p == rc_input) {
            continue;
        }
        if (p == printer_pin) {
            continue;
        }
        if (digitalpinIsAnalogInput(p))
        {
            int16_t x = analogRead(PINTRANSLATE(p));
            print_pin_name(p);
            printer->printf(" = %5d ; ", x);
        }
    }
}

bool test_for_motor_windings_hin(uint16_t* adc_res)
{
    int16_t max_val =  0;
    int16_t min_val = -1;
    for (uint8_t i = 0; ; i++)
    {
        uint32_t p = all_fb[i];
        if (p == END_OF_LIST)
        {
            break;
        }
        uint16_t r = adc_res[i];
        if (r > max_val) {
            max_val = r;
        }
        if (r < min_val || min_val < 0) {
            min_val = r;
        }
    }
    return (max_val - min_val) < 16;
}

bool test_for_motor_windings_lin(uint16_t* adc_res)
{
    int16_t max_val =  0;
    int16_t min_val = -1;
    for (uint8_t i = 0; ; i++)
    {
        uint32_t p = all_fb[i];
        if (p == END_OF_LIST)
        {
            break;
        }
        uint16_t r = adc_res[i];
        if (r > max_val) {
            max_val = r;
        }
        if (r < min_val || min_val < 0) {
            min_val = r;
        }
    }
    return (max_val - min_val) == 0;
}

void test_for_analog_pins()
{
    #ifdef ASSUME_ADC_PINS
    memcpy(remaining_adcs, remaining_pins, sizeof(remaining_adcs));
    filter_only_adcs(remaining_adcs);
    if (pin_is_in_list(assumed_adcs[0], remaining_adcs) && pin_is_in_list(assumed_adcs[1], remaining_adcs))
    {
        uint16_t x = analogRead(PINTRANSLATE(assumed_adcs[0]));
        uint16_t y = analogRead(PINTRANSLATE(assumed_adcs[1]));
        if (x < 8 && y > x &&  y >= 32)
        {
            printer->print("it is likely that ");
            print_pin_name(assumed_adcs[0]);
            printer->print(" is the current sensing pin, and ");
            print_pin_name(assumed_adcs[1]);
            printer->print(" is the voltage sensing pin");
        }
        else if (y < 8 && x > y && x >= 32)
        {
            printer->print("it is likely that ");
            print_pin_name(assumed_adcs[1]);
            printer->print(" is the current sensing pin, and ");
            print_pin_name(assumed_adcs[0]);
            printer->print(" is the voltage sensing pin");
        }
        else
        {
            printer->print("unable to automatically detect the analog sensing pins");
        }
        printer->println(" ");
    }
    #endif
}
