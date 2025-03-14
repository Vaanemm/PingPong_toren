#include <stdlib.h> //voor printf

#include "mcc_generated_files/uart/eusart.h"
#include "adcMeasurement.h"
#include "UART.h"
#include "mcc_generated_files/pwm/pwm5.h"
#include "controller.h"
#include "ledstrip.h"

bool mustPrintLogs = true;

#define EUART_READ_LINE_BUFFER_LENGTH 20
char line[EUART_READ_LINE_BUFFER_LENGTH];

//Lees een lijn uit de console

char* readLine(void) {
    static uint8_t index = 0;
    while (1) {
        if (!EUSART_IsRxReady()) {
            line[index] = 0;
            return line + index; //return empty string
        }
        if (index >= EUART_READ_LINE_BUFFER_LENGTH - 1) {
            while (EUSART_IsRxReady()) {
                EUSART_Read();
            }
            printf("input buffer overflow\r\n");
            index = 0;
            line[index] = 0;
            return line;
        }

        line[index] = EUSART_Read();
        if (line[index] == '\n') {//end of command
            line[index + 1] = 0;
            index = 0;
            return line;
        }
        index++;
    }
}

void printLogs(void) {

    //Schrijf naar de COM poort
    if (mustPrintLogs) {
        //printf("hoogte: %u", getHoogtesensor());
        //printf(", setpoint: %u", getSetpoint());
        //printf("d%u", getDutycycle());
        //printf(", target: %u", getTargdet());
        printf("t%u", getTarget());
        printf("p%u", buiten());
        //Dit zijn dure prints qua geheugen (zo'n 5 %). Als je microcontroller
        //volgeraakt, kan je de printFloat uitschakelen
        //printf(", kp: "); printFloat(getKp());
        //printf(", ki: "); printFloat(getKi());
        //printf(", kd: "); printFloat(getKd());
        printf("\r\n");
    }

}

void uartHandler(void) {
    char *str = readLine();
    switch (*str) { //*str is het eerste karakter van de gelezen lijn
        case 0: //Geen input
            break;
        case 'G':
        case 'g':
            setLedGeraakt();
            break;
        case 'X':
        case 'x':
            intruder();
            break;
        case 'Z':
        case 'z':
            nobody();
            break;
        /*case 'S': //Verander setpoint
        case 's':
            //str+1 omdat str het eerste karakter (s in dit geval) is, de cijfers komen erna
            setSetpoint((uint16_t) atoi(str + 1));
            printf("changed setpoint\r\n");
            break;*/
            /*
        case 'P': //Verander proportion versterking
        case 'p':
            //str+1 omdat str het eerste karakter (p in dit geval) is, de cijfers komen erna
            setKp(str2float(str + 1));
            printf("changed kp\r\n");
            break;
        case 'I': //Verander integrerende versterking
        case 'i':
            setKi(str2float(str + 1));
            printf("changed ki\r\n");
            break;
        case 'D':
        case 'd':
            setKd(str2float(str+1));
            printf("changed kd\r\n");
            break;
        case 'L': //Toggle logs
        case 'l':
            mustPrintLogs = !mustPrintLogs;
            printf("logs toggled\r\n");
            break;*/
        default:
            //printf("Unkown command: %s\r\n", str);
            //je kan de %s weghalen om zo'n 4% geheugen te besparen van de microcontroller
            printf("Unkown command\r\n");
    };
}

//Print een float met 3 cijfers na de komma
void printFloat(float number) {
    int8_t integer_part = (int8_t) number;
    uint16_t fractional_part = (uint16_t) ((number - integer_part) * 1000);
    printf("%i.%03u", integer_part, fractional_part);
    //Voor 4 cijfers:
    //uint16_t fractional_part = (uint16_t) ((number - integer_part) * 10000);
    //printf("%i.%04u", integer_part, fractional_part);
}

//Vorm karakters (chars) om naar een float
float str2float(char *str) {
    float number = 0;
    bool decimal_separator = false;
    uint8_t multiplier;
    while (1) {
        char digit = *str++; //lees het volgende karakter
        if ('0' <= digit && digit <= '9') {
            number *= 10;
            number += digit - '0'; //chars hebben een ASCII code
            multiplier++;
        } else if (digit == ',' || digit == '.') {
            decimal_separator = true;
            multiplier = 0;
        } else {
            break;
        }
    }
    //nu is number het getal zonder komma, dus die moeten we toevoegen
    for (uint8_t i = 0; decimal_separator && i < multiplier; i++) {
        number /= 10;
    }
    return number;
}
