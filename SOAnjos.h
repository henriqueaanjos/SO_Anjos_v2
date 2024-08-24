#ifndef SOANJOS_H
#define SOANJOS_H

//INCLUDES
//EX. -> #include "libraries/SPI/SPI.h"
#include <SoftwareSerial.h>
#include "libraries/Adafruit_GFX_Library/Adafruit_GFX.h"
#include "libraries/Adafruit_ILI9341/Adafruit_ILI9341.h"
#include "libraries/SPI/SPI.h"
#include "libraries/XPT2046_Touchscreen/XPT2046_Touchscreen.h"
#include "libraries/OneButton/src/OneButton.h"
#include "libraries/NewTone/src/NewTone.h"
#include "libraries/SoftWire/src/SoftWire.h"
#include "libraries/Bounce2/src/Bounce2.h"


#include "libraries/Adafruit_GFX_Library/Fonts/FreeMono9pt7b.h"
#include "libraries/Adafruit_GFX_Library/Fonts/FreeSansBold12pt7b.h"
#include "libraries/Adafruit_GFX_Library/Fonts/FreeSerif12pt7b.h"
#include "libraries/Adafruit_GFX_Library/Fonts/FreeSerifBold9pt7b.h"
#include "libraries/Adafruit_GFX_Library/Fonts/FreeSerifBoldItalic9pt7b.h"
#include "libraries/Adafruit_GFX_Library/Fonts/FreeSerifItalic12pt7b.h"

// #include "pins_arduino.h"
// #include <SoftwareSerial.h>
// #include "libraries/SoftWire/src/SoftWire.h"
// #include "libraries/OneWire/OneWireMod.h"

// #include "libraries/TFT_eWidget/src/TFT_eWidget.h"
// #include "libraries/AsyncDelay/src/AsyncDelay.h"
// #include "libraries/MPU6050_light-1.2.1/src/MPU6050_light.h"
// #include "libraries/Bounce2-master/src/Bounce2.h"
// #include "libraries/TimerThree/TimerThree.h"
// #include "libraries/OneButton/src/OneButton.h"
// #include "libraries/NewTone/src/NewTone.h"
//#include "libraries/Dabble/src/Dabble.h"

//DEFINES
//EX. -> #define P1 A3



#define M1 10  //Porta Motor M1
#define M2 11  //Porta Motor M2
#define M3 12  //Porta Motor M3
#define M4 13  //Porta Motor M4

#define m1 1 		
#define m2 2
#define m3 3
#define m4 4 

#define PWMA1  9
#define PWMA2  10

#define PWMB1  12
#define PWMB2  11

#define PWMC1  13
#define PWMC2  46

#define PWMD1  45
#define PWMD2  44

#define TFT_BACKGROUND 3
#define TFT_CS 34
#define TFT_DC 33
#define TOUCH_CS 43

#define TS_MINX 1
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

#define BUTTON_W 95 // Largura
#define BUTTON_H 85 // Altura

#define FSBI9 &FreeSerifBoldItalic9pt7b
#define FM9 &FreeMono9pt7b
#define FSB9 &FreeSerifBold9pt7b
#define FSSB12 &FreeSansBold12pt7b
#define FS12 &FreeSerif12pt7b
#define FSI12 &FreeSerifItalic12pt7b
#define FF0 NULL //ff0 reserved for GLCD

#define P1 A3
#define P2 A4
#define P3 A5
#define P4 A6
#define P5 A15
#define P6 A12
#define P7 A13
#define P8 A14
#define PM1 A8
#define PM2 A9
#define PM3 A10
#define PM4 A11

#define MPU6050_ADDRESS 0x68       // Endereço I2C do MPU6050
#define MPU6050_PWR_MGMT_1 0x6B    // Registrador de gerenciamento de energia
#define MPU6050_ACCEL_XOUT_H 0x3B  // Registradores para leitura da aceleração
#define MPU6050_GYRO_XOUT_H 0x43   // Registradores para leitura dos giros

#define COR_ADRESS 0x39

#define blackThreshold 30
#define whiteThreshold 100

class SOAnjos{
    //VARIAVÉIS
    //EX. -> static int startProgram;
    //FUNCOES
    //EX. -> static void paintScreen(uint16_t color);
    public:
        //CONSTRUTOR
        SOAnjos();
        
        // ---------------------------- MOTORES --------------------------------
        static void setMotorPin(byte motor, int value);
        static void clearCodePin(uint8_t code);
        static long getCodePin(uint8_t code);

        // ----------------------------- TELA ---------------------------------
        static uint16_t hexToRGB565Bitwise(unsigned long rgb);
        static void paintScreen(char* color);
        static void printText(char* text, char* color, int x, int y,int textSize);
        static void drawRect(boolean filled, int x, int y, int w, int h, char* color);
        static void drawRoundRect(boolean filled, int x, int y, int w, int h, int r, char* color);
        static void drawCircle(boolean filled, int x, int y, int r, char* color);
        static void drawLine(int x0, int y0, int x1, int y1, char* color);
        static void drawTriangle(boolean filled, int x0, int y0, int x1, int y1, int x2, int y2, char* color);
        // ---------------------------- SENSORES --------------------------------
        static int getSoundSensor(byte port);
        static int getTouchSensor(byte port);
        static int getUltrassonicSensor(byte port);
        static int getInfraredSensor(byte port, byte channel);
        static int getTemperatureSensor(byte port);
        static int getColorSensor(byte port, byte mode);
        static int16_t getGyroscopeSensor(byte port, uint8_t mode);
        
        // ----------------------------- BOTOES ---------------------------------
        typedef void (*buttonActionFunc)();
        typedef void (*buttonActionLongPressFunc)(void *oneButton);

        static void defineButtonAction(int button, int state, buttonActionFunc func, buttonActionLongPressFunc longPressFunc);


        // ------------------------------ SOM ----------------------------------
        static void makeSound(int frequency, int time);

        // ------------------------------ SO ----------------------------------
        static void initSetup();
        static void updateLoop();
        static int startProgram;
        static int setupProgram;

    private:
        // ---------------------------- MOTORES --------------------------------
        static uint8_t MotorFlag1;
        static uint8_t MotorFlag2;
        static uint8_t MotorFlag3;
        static uint8_t MotorFlag4;

        static uint8_t CodeFlag1; 	
        static uint8_t CodeFlag2;	
        static uint8_t CodeFlag3; 
        static uint8_t CodeFlag4; 

        static unsigned long MotorCodeCnt1;	
        static unsigned long MotorCodeCnt2;	
        static unsigned long MotorCodeCnt3;
        static unsigned long MotorCodeCnt4;	

        static int sliderValueM1;
        static int sliderValueM2;
        static int sliderValueM3;
        static int sliderValueM4;

        static uint8_t getDigitalPin(uint8_t pin);
        static void getCode1Interrupt();
        static void getCode2Interrupt();
        static void getCode3Interrupt();
        static void getCode4Interrupt();
        static void setCodeInit(uint8_t interrupt);

        // ---------------------------- SENSORES --------------------------------
        static Bounce bounceP1;
        static Bounce bounceP2;
        static Bounce bounceP3;
        static Bounce bounceP4;
        static Bounce bounceP5;
        static Bounce bounceP6;
        static Bounce bounceP7;
        static Bounce bounceP8;
        static Bounce bounceM1;
        static Bounce bounceM2;
        static Bounce bounceM3;
        static Bounce bounceM4;

        // ------------------------------ TELA ----------------------------------
        static Adafruit_ILI9341 tft;
        static XPT2046_Touchscreen ts;
        
        static void printText(String text, uint16_t color, int x, int y,int textSize);
        static void printCenteredText(String text, char* color, int x, int y, int w, int h, int textSize);
        static void printCenteredText(String text, uint16_t color, int x, int y, int w, int h, int textSize);

        // ----------------------------- BOTOES ---------------------------------
        static void handleEnterButtonPress();
        static void handleBackButtonPress();
        static void handlePlusButtonPress();
        static void handleLessButtonPress();
        // ------------------------------- SO -----------------------------------
        static int screen;
        static int option;
        static int buzzerActivate;
        static void pageMenu();
        static void pageSensor();
        static void pageSensorUpdateValues();
        static void pageMotor();
        static void pageMotorUpdateValues();
        static void drawSlider(int x, int y, int w, int h, int value);
        static void pageProgram();
        static void pageConfig();
        static void pageApp();
        static void updateButtonPreselect();

};


#endif