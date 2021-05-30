#include <Arduino.h>
#include <elapsedMillis.h>

#define PIN_UART_RX     1
#define PIN_PSU_ON      2
#define PIN_PANEL_PWR   3
#define PIN_SYS_LED_P1  4
#define PIN_NIC_LED_P   5
#define PIN_HDD_LED_P   6
#define PIN_BOARD_PWR   7
#define PIN_FAN_PWM     9
#define PIN_PWR_LED_GP_RN   12
#define PIN_PWR_LED_GN_RP   13
#define PIN_SYS_LED_G_N     19
#define PIN_SYS_LED_R_N     20
#define PIN_SYS_LED_P2      21

#define PWR_STATE_OFF           0   //PSU powered off
#define PWR_STATE_STARTUP       1   //PSU starting up, power button is still held
#define PWR_STATE_ON            2   //PSU powered up, button no longer held
#define PWR_STATE_SHUTDOWN_TMR  3   //PSU powered up, shutdown timer is counting
#define PWR_STATE_SHUTDOWN      4   //PSU shut down, button is still held
#define PWR_STATE_BOARD_OFF     5   //board shut down, PSU still on

#define BLINK_STATE_READY   0   //LED is not blinking
#define BLINK_STATE_ON      1   //LED is off
#define BLINK_STATE_OFF     2   //LED is on

#define UART_STATE_OK       0
#define UART_STATE_ERR      1

#define UART_WATCHDOG_TIMEOUT   10000

uint8_t power_state = PWR_STATE_OFF;
_Bool uart_state = UART_STATE_OK;

int blink_chance = 0;

elapsedMillis event_timer;
elapsedMillis uart_watchdog;
elapsedMillis blink_timer;

void setup() {

    // Configure PWM on pin 9 (and 10) @ 25 kHz.
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
    TCCR1A = _BV(COM1A1)
           | _BV(COM1B1)
           | _BV(WGM11);
    TCCR1B = _BV(WGM13)
           | _BV(CS10);
    ICR1   = 320;

    pinMode(PIN_UART_RX, INPUT);
    pinMode(PIN_PSU_ON, INPUT);
    pinMode(PIN_PANEL_PWR, INPUT_PULLUP);
    pinMode(PIN_SYS_LED_P1, OUTPUT);
    pinMode(PIN_NIC_LED_P, OUTPUT);
    pinMode(PIN_HDD_LED_P, OUTPUT);
    pinMode(PIN_BOARD_PWR, INPUT);
    pinMode(PIN_FAN_PWM, OUTPUT);
    pinMode(PIN_PWR_LED_GP_RN, OUTPUT);
    pinMode(PIN_PWR_LED_GN_RP, OUTPUT);
    pinMode(PIN_SYS_LED_G_N, OUTPUT);
    pinMode(PIN_SYS_LED_R_N, OUTPUT);
    pinMode(PIN_SYS_LED_P2, OUTPUT);

    digitalWrite(PIN_SYS_LED_P1, HIGH);
    digitalWrite(PIN_NIC_LED_P, LOW);
    digitalWrite(PIN_HDD_LED_P, LOW);
    analogWrite(PIN_FAN_PWM, 0);
    digitalWrite(PIN_PWR_LED_GP_RN, LOW);
    digitalWrite(PIN_PWR_LED_GN_RP, LOW);
    digitalWrite(PIN_SYS_LED_G_N, LOW);
    digitalWrite(PIN_SYS_LED_R_N, HIGH);
    digitalWrite(PIN_SYS_LED_P2, HIGH);

    Serial.setTimeout(50);
}

void loop() {
    
/*********************************************
 * handle system power
 *********************************************/
    switch(power_state)
    {

        case PWR_STATE_OFF:
        {
            if( (digitalRead(PIN_PANEL_PWR) == LOW) || (PIN_BOARD_PWR == HIGH) )
            {
                power_state = PWR_STATE_STARTUP;
                pinMode(PIN_PSU_ON, OUTPUT);
                digitalWrite(PIN_PSU_ON, LOW);

                //turn on the GREEN power LED
                pinMode(PIN_PWR_LED_GP_RN, OUTPUT);
                pinMode(PIN_PWR_LED_GN_RP, OUTPUT);
                digitalWrite(PIN_PWR_LED_GN_RP, LOW);
                digitalWrite(PIN_PWR_LED_GP_RN, HIGH);

                event_timer = 0;
            }
        } break;
        case PWR_STATE_STARTUP:
        {
            //after button is released, wait 5 seconds for startup
            if( (digitalRead(PIN_PANEL_PWR) == HIGH) && (event_timer > 5000) )
            {
                power_state = PWR_STATE_ON;
            }
        } break;
        case PWR_STATE_ON:
        {
            if(digitalRead(PIN_PANEL_PWR) == LOW)
            {
                event_timer = 0;
                power_state = PWR_STATE_SHUTDOWN_TMR;
            }
            else if(digitalRead(PIN_BOARD_PWR) == LOW)
            {
                event_timer = 0;
                power_state = PWR_STATE_BOARD_OFF;
            }
        } break;
        case PWR_STATE_SHUTDOWN_TMR:
        {
            if(digitalRead(PIN_PANEL_PWR) == LOW)
            {
                //button was held for 3 seconds, shut down
                if( event_timer > 3000 )
                {
                    //tri-state the power pin
                    power_state = PWR_STATE_SHUTDOWN;
                    pinMode(PIN_PSU_ON, INPUT);

                    //turn on the RED power LED
                    digitalWrite(PIN_PWR_LED_GP_RN, LOW);
                    digitalWrite(PIN_PWR_LED_GN_RP, HIGH);

                    //disconnect UART
                    Serial.end();
                    pinMode(PIN_UART_RX, INPUT);
                }
            }
            else
            {
                power_state = PWR_STATE_ON;
            }
        } break;
        case PWR_STATE_SHUTDOWN:
        {
            //wait until button is released
            if(digitalRead(PIN_PANEL_PWR) == HIGH)
            {
                power_state = PWR_STATE_OFF;
            }
        } break;
        case PWR_STATE_BOARD_OFF:
        {
            if(digitalRead(PIN_BOARD_PWR) == LOW)
            {
                //board was off for 3 seconds, shut down
                if( event_timer > 3000 )
                {
                    //tri-state the power pin
                    power_state = PWR_STATE_SHUTDOWN;
                    pinMode(PIN_PSU_ON, INPUT);

                    //turn on the RED power LED
                    digitalWrite(PIN_PWR_LED_GP_RN, LOW);
                    digitalWrite(PIN_PWR_LED_GN_RP, HIGH);

                    //disconnect UART
                    Serial.end();
                    pinMode(PIN_UART_RX, INPUT);
                }
            }
            else
            {
                power_state = PWR_STATE_ON;
            }
        } break;
        default:
            break;
    }

/*********************************************
 * blink Eth0 LED
 *********************************************/
    // 100 megabit blinks fastest
    #define BLINK_MAX_CHANCE    100000000
    #define BLINK_MILLIS        30

    static uint8_t blink_state = BLINK_STATE_READY;

    if( (power_state == PWR_STATE_ON) &&
        (uart_state == UART_STATE_OK) )
    {
        switch(blink_state)
        {
            case BLINK_STATE_READY:
            {
                // blink the eth0 LED randomly, with a frequency based
                // on the network traffic
                if( (blink_chance != 0) &&
                    ( (random(0,BLINK_MAX_CHANCE) < blink_chance) || 
                    (random(0,2000) == 0) ) 
                    )
                {
                    blink_state = BLINK_STATE_ON;
                    blink_timer = 0;
                    digitalWrite(PIN_NIC_LED_P, HIGH);
                }
            } break;
            case BLINK_STATE_ON:
            {
                if(blink_timer > BLINK_MILLIS)
                {
                    blink_state = BLINK_STATE_OFF;
                    blink_timer = 0;
                    digitalWrite(PIN_NIC_LED_P, LOW);
                }
            } break;
            case BLINK_STATE_OFF:
            {
                if(blink_timer > (BLINK_MILLIS/2))
                {
                    blink_state = BLINK_STATE_READY;
                }
            } break;
            default:
            {
                    blink_state = BLINK_STATE_READY;
                    blink_timer = 0;
                    digitalWrite(PIN_NIC_LED_P, LOW);
            }
        }
    }

    #undef BLINK_MAX_CHANCE 
    #undef BLINK_TICKS         

/*********************************************
 * handle UART commands
 *********************************************/
    #define UART_IDENTIFIER_IDX  0
    #define UART_CMD_IDX         9

    /* enable/reset UART */

    // keep UART off until board is powered up
    if(power_state != PWR_STATE_ON)
    {
        uart_watchdog = 0;
    }
    // no commands received in timeout period, reset UART
    else if(uart_watchdog > UART_WATCHDOG_TIMEOUT)
    {
        Serial.end();
        Serial.begin(115200);
        uart_watchdog = 0;

        uart_state = UART_STATE_ERR;

        //set warning LED
        digitalWrite(PIN_SYS_LED_G_N, HIGH);
        digitalWrite(PIN_SYS_LED_R_N, LOW);
    }

    /* monitor the console output */

    if (Serial.available() > 0) {

        String command;
        int argument = 0;

        // monitor the serial port for the identifier string
        command = Serial.readStringUntil('\n');

        if(command.substring(UART_IDENTIFIER_IDX, UART_CMD_IDX)
            .equals("SimonSays"))
        {
            // reset the watchdog timer
            uart_watchdog = 0;

            uart_state = UART_STATE_OK;

            // system OK
            digitalWrite(PIN_SYS_LED_R_N, HIGH);
            digitalWrite(PIN_SYS_LED_G_N, LOW);

            // parse the command
            command.remove(UART_IDENTIFIER_IDX, UART_CMD_IDX);
            command.trim();

            int arg_idx = command.lastIndexOf(' ')+1;
            if(arg_idx > 0)
            {
                argument = command.substring(arg_idx).toInt();
                command.remove(arg_idx);
                command.trim();
            }

        }

        /* commands */

        // change the fan speed
        if(command.equals("SET FAN"))
        {
            analogWrite(PIN_FAN_PWM, argument);
        }

        // blink the eth0 LED
        else if(command.equals("SET NET LED"))
        {
            blink_chance = argument;
        }
    }

    #undef UART_IDENTIFIER_IDX
    #undef UART_CMD_IDX
}