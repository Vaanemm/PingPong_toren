#include "mcc_generated_files/spi/mssp1.h"
#include "mcc_generated_files/timer/tmr6.h"
#include "adcMeasurement.h"
#include "controller.h"
#include <math.h>
#include "mcc_generated_files/system/system.h"





void sendLedstripStartFrame(void) {
    //de eerste 4 bytes moeten allemaal 0 zijn
    SPI1_ByteExchange(0x00);
    SPI1_ByteExchange(0x00);
    SPI1_ByteExchange(0x00);
    SPI1_ByteExchange(0x00);
}

void sendLedstripEndFrame(void) {
    //de laaste 4 bytes moeten allemaal 0xFF zijn
    SPI1_ByteExchange(0xFF);
    SPI1_ByteExchange(0xFF);
    SPI1_ByteExchange(0xFF);
    SPI1_ByteExchange(0xFF);
}

void sendLedstripFrame(uint8_t red, uint8_t green, uint8_t blue, uint8_t intensity) {
    SPI1_ByteExchange(0xE0 | intensity); //de eerste 3 bits moeten 1 zijn, daarna volgen 5 bits voor de helderheid
    //0xE0 = 0b1110 0000; '|' is de bitwise logische 'or' operatie
    SPI1_ByteExchange(blue); //8 bits voor de blauwe kleur
    SPI1_ByteExchange(green); //8 bits voor de groene kleur
    SPI1_ByteExchange(red); //8 bits voor de rode kleur
}

#define NUMBER_OF_LEDS 60
#define STEP 2

enum states {
    GREEN_UP, RED_DOWN, BLUE_UP, GREEN_DOWN, RED_UP, BLUE_DOWN
};

enum count {
    UP, DOWN
};

void updateLedstripAnimation(void) {
    
    static uint8_t counter = 0;
    static uint8_t intensity = 0;
    static uint8_t i = 0;
    /*
    //we voegen static toe om de waarden van de variabelen over de functiecalls te behouden
    static enum count direction = UP;
    static uint8_t led_run = 0;
    static enum states change_color = GREEN_UP;
    static uint8_t blue = 0x00, green = 0x00, red = 0xFF;


    switch (direction) {
        case UP:
            if (led_run < NUMBER_OF_LEDS - 1) {
                //sendLedstripFrame(0x00, 0xFF, 0x00, 1);
                led_run++;
            } else {
                direction = DOWN;
            }
            break;
        case DOWN:
            if (led_run > 0) {
                //sendLedstripFrame(0x00, 0x00, 0xFF, 1);
                led_run--;
            } else {
                direction = UP;
            }
            break;
    }
  

    switch (change_color) {
        case GREEN_UP:
            if (green < 0xFF) {
                green += STEP;
            } else {
                change_color = RED_DOWN;
            }
            break;
        case RED_DOWN:
            if (red > 0x00) {
                red -= STEP;
            } else {
                change_color = BLUE_UP;
            }
            break;
        case BLUE_UP:
            if (blue < 0xFF) {
                blue += STEP;
            } else {
                change_color = GREEN_DOWN;
            }
            break;
        case GREEN_DOWN:
            if (green > 0x00) {
                green -= STEP;
            } else {
                change_color = RED_UP;
            }
            break;
        case RED_UP:
            if (red < 0xFF) {
                red += STEP;
            } else {
                change_color = BLUE_DOWN;
            }
            break;
        case BLUE_DOWN:
            if (blue > 0x00) {
                blue -= STEP;
            } else {
                change_color = GREEN_UP;
            }
            break;
    }

    */
//    static float conv = (60/850);
    
// uint16_t hoogte_bal_teConverteren = getHoogtesensor();
//       float hoogte_bal = (60/850)*hoogte_bal_teConverteren;  
    //start frame: eerst laten we weten dat we data gaan doorsturen
    sendLedstripStartFrame();
    
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    
    
    
    bool ledGeraakt=true;
    for (uint8_t led = 0; led < NUMBER_OF_LEDS; led++) { //dan sturen we de waarde van alle leds door.         
        if (ledGeraakt==true){
            sendLedstripFrame(0xFF, 0x00, 0x00, 0x05);
           //_delay_ms(100);
            sendLedstripFrame(0x00, 0x00, 0xFF, 0x05);
        //  __delay_ms(100);
            sendLedstripFrame(0x00, 0xFF, 0x00, 0x05);
            
        }else{
            uint16_t hoogtEbal = getHoogtesensor();
            //float rood_hoogte_verschil = 450.0 - hoogtEbal
            //float rood_fout = round( (rood_hoogte_verschil)/125);
            float hoogtebal_teConverteren = hoogtEbal * (60.0 / 1023.0)-4.5   ;

            uint16_t setPoint = getSetpoint();
            //int8_t blauw_fout = round( (525 - setPoint)/65);
            float setpoint_teConverteren = setPoint * (60.0 / 1023.0)-4.5  ;

            uint16_t hoogtebal = round(hoogtebal_teConverteren);
            uint16_t setpoint = round(setpoint_teConverteren);
    //        if (led == led_run) {
    //           sendLedstripFrame(red, green, blue, 0x05);
    //        }
            if (led == hoogtebal ){
                sendLedstripFrame(0xFF, 0x00, 0x00, 0x05);
            } else {
                if (led == setpoint) {
                sendLedstripFrame(0x00, 0x00, 0xFF, 0x05);
                } else {
                    sendLedstripFrame(0x00, 0xFF, 0x00, 0x02);
            }
            }
        }
            
       
     //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       
        
    }
    //stop frame: uiteindelijk laten we weten dat we alle data hebben doorgestuurd
    sendLedstripEndFrame();
     
}

void initLedstrip(void) {
    ledstrip_Open(HOST_CONFIG);
    tmr_ledstrip_OverflowCallbackRegister(updateLedstripAnimation);
}
