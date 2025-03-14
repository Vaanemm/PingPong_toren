#include "adcMeasurement.h"
#include "mcc_generated_files/pwm/pwm5.h"
#include "controller.h"
#include "ledstrip.h"
#include "mcc_generated_files/system/system.h"

static int hoogte_bal;
static int setpoint = 500;
static int dutycycle = 0; //PWM5 heeft 10 bits resolutie -> neem uint16_t als type
int integral_action = 0;

static float integraal = 0;
static float before = 30;
float tijd = 0.1;

static float kp = 1.2375;
static float ki = 0.5;
static float kd = 0.1;
static bool ledGeraakt = false;
static bool moetStoppen = false;

void controller(void) {
    hoogte_bal = getHoogtesensor(); //resultaat van ADC
    //setpoint = getPotentiometer(); //je kan je setpoint ook ergens anders zetten (dan moet deze lijn uiteraard weg)

    int error = setpoint - hoogte_bal;
    int proportional_action = error * kp;
    int integral_action = integral_action + 0.03*error;
    float differential_action = (hoogte_bal - before)/tijd;
    int result = proportional_action + ki*integral_action + kd*differential_action;
    if (result<0){
        result = 0;
    }
    if (result>1023){
        result = 1023;
    }

    dutycycle = result;
    before = hoogte_bal;

    pwm_fan_LoadDutyValue(dutycycle); //output PWM signaal
}

//getters
uint16_t getSetpoint(void) {return setpoint;}
uint16_t getDutycycle(void) {return dutycycle;}
float getKp(void) {return kp;}
float getKi(void) {return ki;}
float getKd(void) {return kd;}
bool getLedGeraakt(void) {return ledGeraakt;}
bool getMoetStoppen(void) {return moetStoppen;}
int buiten(void) {
    if (moetStoppen == true){
        return 0;
    }else{
        return 1;
    }
}


//setters
//void setSetpoint(uint16_t new_setpoint) {setpoint = new_setpoint;}
void setDutycycle(uint16_t new_dutycycle) {dutycycle = new_dutycycle;}
//void setKp(float new_kp) {kp = new_kp;}
//void setKi(float new_ki) {ki = new_ki;}
//void setKd(float new_kd) {kd = new_kd;}
void setLedGeraakt(void) {
    if (ledGeraakt == true){
        ledGeraakt = false;
    }else{
        ledGeraakt = true;
    }
}
void intruder(void){
    moetStoppen = true;
}

void nobody(void){
    moetStoppen = false;
}
//void setTarget(float new_target) {target = new_target;}
