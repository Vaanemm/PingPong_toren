#include "mcc_generated_files/adc/adc.h"
#include "adcMeasurement.h"
volatile uint16_t ADC_potentiometer;
volatile uint16_t ADC_potentiometer, ADC_hoogtesensor, ADC_target, ADC_target2, ADC_target3, ADC_target4;

void adcConversionDoneHandler(void) {
    /* static zorgt dat de waarde behouden blijft over functiecalls heen, en dat
     * enkel binnen de functie de waarde aangepast kan worden
     * mogelijke channels staan in Header Files > MCC Generated Files > adc > adc.h
     */
    static adc_channel_t new_channel = potentiometer;
    switch (new_channel) {
        case potentiometer:
            ADC_potentiometer = ADC_GetConversionResult();
            new_channel = hoogtesensor;
            break;
        case hoogtesensor:
            ADC_hoogtesensor = ADC_GetConversionResult();
            new_channel = target1;
            break;
        case target1:
            ADC_target = ADC_GetConversionResult();
            new_channel = target2;
            break;
        case target2:
            ADC_target2 = ADC_GetConversionResult();
            new_channel = target3;
            break;  
        case target3:
            ADC_target3 = ADC_GetConversionResult();
            new_channel = potentiometer;
            break;  
        case target4:
            ADC_target4 = ADC_GetConversionResult();
            new_channel = potentiometer;
            break;   
    }
    ADC_SelectChannel(new_channel);
}

void initAdcMultiplexer(void){
    ADC_SelectChannel(potentiometer);
    ADC_SetInterruptHandler(adcConversionDoneHandler);
}

uint16_t getPotentiometer(void){
    return ADC_potentiometer;
}

uint16_t getHoogtesensor(void) {
    return ADC_hoogtesensor;
}

uint16_t getTarget(void) {
    return ADC_target;
}

uint16_t getTarget2(void) {
    return ADC_target2;
}

uint16_t getTarget3(void) {
    return ADC_target3;
}

uint16_t getTarget4(void) {
    return ADC_target4;
}
