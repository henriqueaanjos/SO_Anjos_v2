//INCLUDES
//EX. -> #include "libraries/SPI/SPI.h"

//VARIAVÉIS
//EX. -> int SOAnjos::startProgram = 0;

//FUNCOES
//EX. -> void SOAnjos::paintScreen(uint16_t color){}

// #include "Arduino.h"


// #define INCLUDE_GAMEPAD_MODULE
// #define INCLUDE_TERMINAL_MODULE

#include "SOAnjos.h"
#include "libraries/Adafruit_ILI9341/Adafruit_ILI9341.h"
#include "libraries/Adafruit_GFX_Library/Adafruit_GFX.h"
#include "libraries/SPI/SPI.h"
#include "libraries/XPT2046_Touchscreen/XPT2046_Touchscreen.h"
#include "libraries/OneButton/src/OneButton.h"
#include "libraries/NewTone/src/NewTone.h"
#include "libraries/SoftWire/src/SoftWire.h"
#include "libraries/Bounce2/src/Bounce2.h"

uint8_t SOAnjos::MotorFlag1 = 0;
uint8_t SOAnjos::MotorFlag2 = 0;
uint8_t SOAnjos::MotorFlag3 = 0;
uint8_t SOAnjos::MotorFlag4 = 0;

uint8_t SOAnjos::CodeFlag1 = 0;	
uint8_t SOAnjos::CodeFlag2 = 0;
uint8_t SOAnjos::CodeFlag3 = 0;
uint8_t SOAnjos::CodeFlag4 = 0;

unsigned long SOAnjos::MotorCodeCnt1 = 0;	
unsigned long SOAnjos::MotorCodeCnt2 = 0;
unsigned long SOAnjos::MotorCodeCnt3 = 0;
unsigned long SOAnjos::MotorCodeCnt4 = 0;

int SOAnjos::sliderValueM1 = 0;
int SOAnjos::sliderValueM2 = 0;
int SOAnjos::sliderValueM3 = 0;
int SOAnjos::sliderValueM4 = 0;

Bounce SOAnjos::bounceP1 = Bounce();
Bounce SOAnjos::bounceP2 = Bounce();
Bounce SOAnjos::bounceP3 = Bounce();
Bounce SOAnjos::bounceP4 = Bounce();
Bounce SOAnjos::bounceP5 = Bounce();
Bounce SOAnjos::bounceP6 = Bounce();
Bounce SOAnjos::bounceP7 = Bounce();
Bounce SOAnjos::bounceP8 = Bounce();
Bounce SOAnjos::bounceM1 = Bounce();
Bounce SOAnjos::bounceM2 = Bounce();
Bounce SOAnjos::bounceM3 = Bounce();
Bounce SOAnjos::bounceM4 = Bounce();

Adafruit_ILI9341 SOAnjos::tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
XPT2046_Touchscreen SOAnjos::ts = XPT2046_Touchscreen(TOUCH_CS);

OneButton enterButton(49, true);  // Botão Enter da placa v1 e V2
OneButton backButton(48, true); // Botão Enter da placa v1 e V2
OneButton plusButton(47, true);   // Botão Enter da placa v1 e V2
OneButton lessButton(38, true);  // Botão Enter da placa v1 e V2

int SOAnjos::screen = 0;
int SOAnjos::option = 0;

int SOAnjos::startProgram = 0;
int SOAnjos::setupProgram = 0;
int SOAnjos::buzzerActivate = 0;

int16_t xCordinatesButton [5];
int16_t yCordinatesButton [5]; 

int sensorSelected = 0;

AsyncDelay delay_3s;

//CONSTRUTOR
SOAnjos::SOAnjos(){}

// -----------------------------------------------------------------------------------
// --------------------------------- MOTORES -----------------------------------------
// -----------------------------------------------------------------------------------

uint8_t SOAnjos::getDigitalPin(uint8_t pin){  //PRIVATE
	pinMode(pin,INPUT_PULLUP);
	return digitalRead(pin);
}

void SOAnjos::getCode1Interrupt(){   //PRIVATE
	uint8_t m_ISR,m_Dir;	
	m_ISR = getDigitalPin(2);	
	m_Dir = getDigitalPin(A8);	
	if(m_ISR^m_Dir)			
		MotorCodeCnt1++;	
	else
		MotorCodeCnt1--;	 
} 

void SOAnjos::getCode2Interrupt(){    //PRIVATE
	uint8_t m_ISR,m_Dir;	
	m_ISR = getDigitalPin(3);	
	m_Dir = getDigitalPin(A9);	
	if(m_ISR^m_Dir)			
		MotorCodeCnt2++;	
	else
		MotorCodeCnt2--;	
}

void SOAnjos::getCode3Interrupt(){    //PRIVATE
	uint8_t m_ISR,m_Dir;	
	m_ISR = getDigitalPin(18);	
	m_Dir = getDigitalPin(A10);	
	if(m_ISR^m_Dir)			
		MotorCodeCnt3++;	
	else
		MotorCodeCnt3--;	
}

void SOAnjos::getCode4Interrupt(){    //PRIVATE
	uint8_t m_ISR,m_Dir;	
	m_ISR = getDigitalPin(19);	
	m_Dir = getDigitalPin(A11);	
	if(m_ISR^m_Dir)			
		MotorCodeCnt4++;	
	else
		MotorCodeCnt4--;	
}  

void SOAnjos::setCodeInit(uint8_t interrupt){	//PRIVATE
	switch(interrupt){
		case m1: 
            attachInterrupt(0, getCode1Interrupt, CHANGE);
            break;
		case m2: 
            attachInterrupt(1, getCode2Interrupt, CHANGE);
            break;
		case m3:
            attachInterrupt(5, getCode3Interrupt, CHANGE); 
            break;
		case m4: 
            attachInterrupt(4, getCode4Interrupt, CHANGE);
            break;
		default:
            break;		
	}
}

void SOAnjos::setMotorPin(byte motor, int value){  //PUBLIC
    if(value > 100) value = 100;
    else if(value < -100) value = -100;

    switch (motor){
        case M1:
            if (MotorFlag1 == 0){
                MotorFlag1 = 1;
                pinMode(PWMA1, OUTPUT);
                pinMode(PWMA2, OUTPUT);             
            }
            break;
        case M2:
            if (MotorFlag2 == 0){
                MotorFlag2 = 1;
                pinMode(PWMB1, OUTPUT);
                pinMode(PWMB2, OUTPUT);             
            }
            break;
        case M3:
            if (MotorFlag3 == 0){
                MotorFlag3 = 1;
                pinMode(PWMC1, OUTPUT);
                pinMode(PWMC2, OUTPUT);           
            }
            break;
        case M4:
            if (MotorFlag4 == 0){
                MotorFlag4 = 1;
                pinMode(PWMD1, OUTPUT);
                pinMode(PWMD2, OUTPUT);           
            }
            break;
        default:
            break;
    }
    switch (motor){
        case M1:
            if (value == 0){
                analogWrite(PWMA1, 255);
                analogWrite(PWMA2, 255);
            }else if (value>0){
                analogWrite(PWMA1, map(value,0,100,0,255));
                analogWrite(PWMA2, 0);
            }else if (value<0){
                analogWrite(PWMA2, map(value,-100,0,255,0));
                analogWrite(PWMA1, 0); 
            }
            break;
        case M2:
            if (value == 0){
                analogWrite(PWMB1, 255);
                analogWrite(PWMB2, 255);
            }else if (value>0){
                analogWrite(PWMB1, map(value,0,100,0,255));
                analogWrite(PWMB2, 0);
            }else if (value<0){
                analogWrite(PWMB2, map(value,-100,0,255,0));
                analogWrite(PWMB1, 0);
            }
            break;
        case M3:
            if (value == 0){
                analogWrite(PWMC1, 255);
                analogWrite(PWMC2, 255);
            }
            else if (value>0){
                analogWrite(PWMC1, map(value,0,100,0,255));
                analogWrite(PWMC2, 0);
            }else if (value<0){
                analogWrite(PWMC2, map(value,-100,0,255,0));
                analogWrite(PWMC1, 0);
            }
            break;
        case M4:
            if (value == 0){
                analogWrite(PWMD1, 255);
                analogWrite(PWMD2, 255);
            }else if (value>0){
                analogWrite(PWMD1, map(value,0,100,0,255));
                analogWrite(PWMD2, 0);
            }else if (value<0){
                analogWrite(PWMD2, map(value,-100,0,255,0));
                analogWrite(PWMD1, 0);
            }
            break;
        default:
            break;
    }
}

long SOAnjos::getCodePin(uint8_t code){	  //PUBLIC
	if(CodeFlag1==0 && code==m1){
		CodeFlag1 = 1;
		setCodeInit(code);		
	}else if(CodeFlag2==0 && code==m2){
		CodeFlag2 = 1;
		setCodeInit(code);				
	}else if(CodeFlag3==0 && code==m3){
		CodeFlag3 = 1;
		setCodeInit(code);				
	}else if(CodeFlag4==0 && code==m4){
		CodeFlag4 = 1;
		setCodeInit(code);				
	}
	switch(code){
		case m1: 
            return	MotorCodeCnt1;
		case m2: 
            return	MotorCodeCnt2;
		case m3: 
            return	MotorCodeCnt3;
		case m4: 
            return	MotorCodeCnt4;
		default:
            break;
	}
}

void SOAnjos::clearCodePin(uint8_t code){  //PUBLIC
    switch(code){
		case m1: 
            MotorCodeCnt1 = 0;
            break; 
		case m2: 
            MotorCodeCnt2 = 0; 
            break; 
		case m3: 
            MotorCodeCnt3 = 0; 
            break; 
		case m4: 
            MotorCodeCnt4 = 0; 
            break; 
		default:
            break;		
	}
}

// -----------------------------------------------------------------------------------
// ----------------------------------- TELA ------------------------------------------
// -----------------------------------------------------------------------------------

uint16_t SOAnjos::hexToRGB565Bitwise(unsigned long rgb){  //PRIVATE
    uint16_t R_ = (rgb >> 16) & 0xFF;
    uint16_t G_ = (rgb >> 8) & 0xFF;
    uint16_t B_ = (rgb ) & 0xFF;

    uint16_t ret = ((R_ & 0xF8) << 8) |
                   ((G_ & 0xFC) << 3) |
                   (B_ >> 3);

    return (ret);
}

void SOAnjos::paintScreen(char* color){  //PUBLIC
    unsigned long colorValue = hexToRGB565Bitwise(strtoul(color + 1, NULL, 16));  // +1 para pular o '#'
    tft.fillRect(0,32,240,281,colorValue);
}

void SOAnjos::printText(char* text, char* color, int x, int y, int textSize){
    unsigned long colorValue = hexToRGB565Bitwise(strtoul(color + 1, NULL, 16));  // +1 para pular o '#'
    tft.setCursor(x, y);
    tft.setTextColor(colorValue);
    tft.setTextSize(textSize);
    tft.setTextWrap(false);
    tft.print(text);
}

void SOAnjos::printCenteredText(String text, char*  color, int x, int y, int w, int h, int textSize){
    int boxCenterX = (x + x + w)/2;
    int boxCenterY = (y + y + h)/2;

    int16_t xText, yText;
    uint16_t wText, hText;
    
    unsigned long colorValue = hexToRGB565Bitwise(strtoul(color + 1, NULL, 16));  // +1 para pular o '#'

    tft.getTextBounds(text,0,0,&xText, &yText, &wText, &hText); 
    int textCenterX = (xText + wText)/2;
    int textCenterY = (yText + hText)/2;

    tft.setCursor(boxCenterX - textCenterX, boxCenterY - textCenterY);
    tft.setTextColor(colorValue);
    tft.setTextSize(textSize);
    tft.setTextWrap(false);
    tft.print(text);
}

void SOAnjos::printText(String text, uint16_t color, int x, int y, int textSize){
    tft.setCursor(x, y);
    tft.setTextColor(color);
    tft.setTextSize(textSize);
    tft.setTextWrap(false);
    tft.print(text);
}

void SOAnjos::printCenteredText(String text, uint16_t color, int x, int y, int w, int h, int textSize){
    int boxCenterX = (x + x + w)/2;
    int boxCenterY = (y + y + h)/2;

    int16_t xText, yText;
    uint16_t wText, hText;

    tft.getTextBounds(text,0,0,&xText, &yText, &wText, &hText); 
    int textCenterX = (xText + wText)/2;
    int textCenterY = (yText + hText)/2;

    tft.setCursor(boxCenterX - textCenterX, boxCenterY - textCenterY);
    tft.setTextColor(color);
    tft.setTextSize(textSize);
    tft.setTextWrap(false);
    tft.print(text);
}

void SOAnjos::drawRect(boolean filled, int x, int y, int w, int h, char*  color){
    unsigned long colorValue = hexToRGB565Bitwise(strtoul(color + 1, NULL, 16));  // +1 para pular o '#'
    if(filled){
        tft.fillRect(x,y,w,h,colorValue);
    }else{
        tft.drawRect(x,y,w,h,colorValue);
    }
}

void SOAnjos::drawRoundRect(boolean filled, int x, int y, int w, int h, int r, char*  color){
    unsigned long colorValue = hexToRGB565Bitwise(strtoul(color + 1, NULL, 16));  // +1 para pular o '#'
    if(filled){
        tft.fillRoundRect(x,y,w,h,r,colorValue);
    }else{
        tft.drawRoundRect(x,y,w,h,r,colorValue);
    }
}

void SOAnjos::drawCircle(boolean filled, int x, int y, int r, char*  color){
    unsigned long colorValue = hexToRGB565Bitwise(strtoul(color + 1, NULL, 16));  // +1 para pular o '#'
    if(filled){
        tft.fillCircle(x,y,r, colorValue);
    }else{
        tft.drawCircle(x,y,r, colorValue);
    }
}

void SOAnjos::drawLine(int x0, int y0, int x1, int y1, char*  color){
    unsigned long colorValue = hexToRGB565Bitwise(strtoul(color + 1, NULL, 16));  // +1 para pular o '#'
    tft.drawLine(x0,y0,x1,y1,colorValue);
}

void SOAnjos::drawTriangle(boolean filled, int x0, int y0, int x1, int y1, int x2, int y2, char* color){
    unsigned long colorValue = hexToRGB565Bitwise(strtoul(color + 1, NULL, 16));  // +1 para pular o '#'
    if(filled){
        tft.fillTriangle(x0,y0,x1,y1,x2,y2, colorValue);
    }else{
        tft.drawTriangle(x0,y0,x1,y1,x2,y2, colorValue);
    }
}

// -----------------------------------------------------------------------------------
// --------------------------------- SENSORES ----------------------------------------
// -----------------------------------------------------------------------------------

int SOAnjos::getSoundSensor(byte port){ //
    int value;
    switch(port){
        case 1:
            pinMode(P1, INPUT);
            value = analogRead(P1);
            return value - 20;
        case 2:
            pinMode(P2, INPUT);
            return analogRead(P2);
        case 3:
            pinMode(P3, INPUT);
            return analogRead(P3);
        case 4:
            pinMode(P4, INPUT);
            return analogRead(P4);
        case 5:
            pinMode(P5, INPUT);
            value = analogRead(P5);
            return value - 80;
        case 6:
            pinMode(P6, INPUT);
            value = analogRead(P6);
            return value - 100;
        case 7:
            pinMode(P7, INPUT);
            value = analogRead(P7);
            return value - 130;
        case 8:
            pinMode(P8, INPUT);
            value = analogRead(P8);
            return value - 90;
        case 9:  //M1
            pinMode(PM1, INPUT);
            value = analogRead(PM1);
            return value - 135;
        case 10: //M2
            pinMode(PM2, INPUT);
            value = analogRead(PM2);
            return value - 140;
        case 11: //M3
            pinMode(PM3, INPUT);
            value = analogRead(PM3);
            return value - 150;
        case 12: //M4
            pinMode(PM4, INPUT);
            value = analogRead(PM4);
            return value - 155;
    }
}

int SOAnjos::getTouchSensor(byte port){
    int value;
    switch(port){
        case 1: 
            value = analogRead(P1);
            return value >= 550 && value <= 700 ? 1 :0;
        case 2: 
            return analogRead(P2) >= 600 ? 1 :0;
        case 3: 
            return analogRead(P3) >= 600 ? 1 :0;
        case 4: 
            return analogRead(P4) >= 600 ? 1 :0;
        case 5: 
            value = analogRead(P5);
            return value >= 640 && value <= 700 ? 1 :0;
        case 6: 
            value = analogRead(P6);
            return value >= 640 && value <= 700 ? 1 :0;
        case 7: 
            value = analogRead(P7);
            return value >= 640 && value <= 700 ? 1 :0;
        case 8: 
            value = analogRead(P8);
            return value >= 640 && value <= 700 ? 1 :0;
        case 9:  //M1
            value = analogRead(PM1);
            return value >= 640 && value <= 700 ? 1 :0;
        case 10: //M2
            value = analogRead(PM2);
            return value >= 640 && value <= 700 ? 1 :0;
        case 11: //M3
            value = analogRead(PM3);
            return value >= 640 && value <= 700 ? 1 :0;
        case 12: //M4
            value = analogRead(PM4);
            return value >= 640 && value <= 700 ? 1 :0;
    }
}

int SOAnjos::getUltrassonicSensor(byte port){
    unsigned long currentTime = millis();
    int sda, scl;
    switch(port){
        case 5:
            sda = P5;
            scl = 4;
            break;
        case 6:
            sda = P6;
            scl = 53;
            break;
        case 7:
            sda = P7;
            scl = 28;
            break;
        case 8:
            sda = P8;
            scl = 29;
            break;
        case 9:
            sda = PM1;
            scl = 2;
            break;
        case 10:
            sda = PM2;
            scl = 3;
            break;
        case 11:
            sda = PM3;
            scl = 18;
            break;
        case 12:
            sda = PM4;
            scl = 19;
            break;
        default:
            return 0;
    }
    SoftwareSerial ultrasonic(sda, scl);
    ultrasonic.begin(57600);
    ultrasonic.listen();
    ultrasonic.write(0x01);
    delay(30);
    if (ultrasonic.available()) {
        return ultrasonic.read();
    }
    return 0;
}

int SOAnjos::getInfraredSensor(byte port, byte channel) { //
    unsigned long _previousTime = 0;
    const unsigned long _interval = 35; // Intervalo de 5 segundos
    unsigned long currentTime = millis();

    byte receivedByte;
    byte tresPrimeirosBits;
    byte cincoUltimosBits;

    int sda, scl;
    switch(port){
        case 5:
            sda = P5;
            scl = 4;
            break;
        case 6:
            sda = P6;
            scl = 53;
            break;
        case 7:
            sda = P7;
            scl = 28;
            break;
        case 8:
            sda = P8;
            scl = 29;
            break;
        case 9:
            sda = PM1;
            scl = 2;
            break;
        case 10:
            sda = PM2;
            scl = 3;
            break;
        case 11:
            sda = PM3;
            scl = 18;
            break;
        case 12:
            sda = PM4;
            scl = 19;
            break;
        default:
            return 0;
    }
    SoftwareSerial infrared(sda, scl);
    infrared.begin(57600);
    if (currentTime - _previousTime >= _interval) {
        _previousTime = currentTime;
        infrared.listen();
        infrared.write(0x08);
        delay(5);
        if (infrared.available()) {
            receivedByte = infrared.read();
        }
    }
    tresPrimeirosBits = receivedByte & 0b00000111;
    cincoUltimosBits = (receivedByte >> 3) & 0b00011111;
    if(channel == 1 && cincoUltimosBits==4){
        return tresPrimeirosBits;
    }else if(channel == 2 && cincoUltimosBits==8){
        return tresPrimeirosBits;
    }else if(channel == 3 && cincoUltimosBits==12){
        return tresPrimeirosBits;
    }else if(channel == 4 && cincoUltimosBits==16){
        return tresPrimeirosBits;
    }else{
        return 0;
    }
}

int SOAnjos::getTemperatureSensor(byte port){ //
    static byte temperatura[2]={0,0};
    static char swTxBuffer[16];
    static char swRxBuffer[16];
    int sda, scl;
    switch(port){
        case 1:
            sda = P1;
            scl = 22;
            break;
        case 2:
            sda = P2;
            scl = 23;
            break;
        case 3:
            sda = P3;
            scl = 24;
            break;
        case 4:
            sda = P4;
            scl = 25;
            break;
        case 5:
            sda = P5;
            scl = 4;
            break;
        case 6:
            sda = P6;
            scl = 53;
            break;
        case 7:
            sda = P7;
            scl = 28;
            break;
        case 8:
            sda = P8;
            scl = 29;
            break;
        case 9:
            sda = PM1;
            scl = 2;
            break;
        case 10:
            sda = PM2;
            scl = 3;
            break;
        case 11:
            sda = PM3;
            scl = 18;
            break;
        case 12:
            sda = PM4;
            scl = 19;
            break;
        default:
            sda = P1;
            scl = 22;
    }
    SoftWire temperature(sda, scl);
    temperature.setTxBuffer(swTxBuffer, sizeof(swTxBuffer));
    temperature.setRxBuffer(swRxBuffer, sizeof(swRxBuffer));
    temperature.setDelay_us(8);
    temperature.setTimeout_ms(10);
    temperature.begin();
    temperature.beginTransmission(0x4c); 
    temperature.write(0x00); 
    temperature.endTransmission();    
    temperature.requestFrom(0x4c, 2);    
    for(byte i=0;i<=2;i++){
        if (temperature.available()>1) { 
            temperatura[i] = temperature.read();  
        }else{ 
            temperatura[i] = 0;
        } 
    }
    return temperatura[0];
}

int SOAnjos::getColorSensor(byte port, byte mode){
    uint8_t  clearL,redL,greenL,blueL,lastColor = 7;
    static char swTxBuffer[16];
    static char swRxBuffer[16];

    int sda, scl;
    switch(port){
        case 1:
            sda = P1;
            scl = 22;
            break;
        case 5:
            sda = P5;
            scl = 4;
            break;
        case 7:
            sda = P7;
            scl = 28;
            break;
        case 8:
            sda = P8;
            scl = 29;
            break;
        case 9:
            sda = PM1;
            scl = 2;
            break;
        case 10:
            sda = PM2;
            scl = 3;
            break;
        case 11:
            sda = PM3;
            scl = 18;
            break;
        case 12:
            sda = PM4;
            scl = 19;
            break;
        default:
            return 7;
    }
    
    SoftWire color(sda, scl);
    color.setTxBuffer(swTxBuffer, sizeof(swTxBuffer));
    color.setRxBuffer(swRxBuffer, sizeof(swRxBuffer));
    color.setTimeout_ms(2);

    color.beginTransmission(0x39);   
    color.write(0x92);                     
    color.endTransmission();

    color.beginTransmission(0x39);   
    color.write(0x81);                     
    color.write(0xFF);                      
    color.endTransmission();               

    color.beginTransmission(0x39);   
    color.write(0x8F);                     
    color.write(0x02);                     
    color.endTransmission();                

    color.beginTransmission(0x39);   
    color.write(0x80);                     
    color.write(0x0B);                     
    color.endTransmission();  

    color.beginTransmission(0x39);   
    color.write(0x94);                     
    color.endTransmission();              

    color.requestFrom(0x39, 1);      
    
    if(color.available()){                 
        clearL = color.read();
    }

    color.beginTransmission(0x39);   
    color.write(0x96);                     
    color.endTransmission();               

    color.requestFrom(0x39, 1);      
    
    if(color.available()){                 
        redL = color.read();
    }

    color.beginTransmission(0x39);   
    color.write(0x98);                     
    color.endTransmission();               

    color.requestFrom(0x39, 1);      
    
    if(color.available()){                 
        greenL = color.read();
    }

    color.beginTransmission(0x39);   
    color.write(0x9A);                     
    color.endTransmission();               

    color.requestFrom(0x39, 1);      
    if(color.available()){                 
        blueL = color.read();
    }
    switch(mode){
        case 0x00:
            if (clearL < blackThreshold) {
                lastColor = 7;
            }else if (clearL > whiteThreshold) {
                lastColor = 6; 
            }else if (greenL > redL && greenL > blueL) {
                if (redL > blueL) {
                    lastColor = 4; // Yellow
                } else {
                    lastColor = 2; // Green
                }  
            } else if (blueL > redL && blueL > greenL) {
                lastColor = 3;   
            } else if (redL > blueL && greenL > blueL) {
                lastColor = 4; // Yellow  
            } else if (redL > blueL && greenL < blueL) {
                lastColor = 1;    
            } else {
                lastColor = 8; // Unknown or Orange    
            }
            return lastColor;
        case 0x01:
            return clearL;
        case 0x02:
            return redL;
        case 0x03:
            return greenL;
        case 0x04:
            return blueL;
    }
}

int16_t SOAnjos::getGyroscopeSensor(byte port, uint8_t mode){ //
    char swTxBuffer[16];
    char swRxBuffer[16];
    int16_t accelX, accelY, accelZ; 
    int16_t gyroX, gyroY, gyroZ;

    int sda, scl;
    switch(port){
        case 1:
            sda = P1;
            scl = 22;
            break;
        case 2:
            sda = P2;
            scl = 23;
            break;
        case 3:
            sda = P3;
            scl = 24;
            break;
        case 4:
            sda = P4;
            scl = 25;
            break;
        case 5:
            sda = P5;
            scl = 4;
            break;
        case 7:
            sda = P7;
            scl = 28;
            break;
        case 8:
            sda = P8;
            scl = 29;
            break;
        case 9:
            sda = PM1;
            scl = 2;
            break;
        case 10:
            sda = PM2;
            scl = 3;
            break;
        case 11:
            sda = PM3;
            scl = 18;
            break;
        case 12:
            sda = PM4;
            scl = 19;
            break;
        default:
            return 0;
    }
    SoftWire gyroscope(sda, scl);
    gyroscope.setTxBuffer(swTxBuffer, sizeof(swTxBuffer));
    gyroscope.setRxBuffer(swRxBuffer, sizeof(swRxBuffer));
    gyroscope.begin();
    gyroscope.setTimeout_ms(5);

    gyroscope.beginTransmission(MPU6050_ADDRESS);
    gyroscope.write(MPU6050_PWR_MGMT_1);
    gyroscope.write(0x00); // Ativa o MPU6050 (desativa o modo de suspensão)
    gyroscope.endTransmission();

    // Leitura dos valores da aceleração
    gyroscope.beginTransmission(MPU6050_ADDRESS);
    gyroscope.write(MPU6050_ACCEL_XOUT_H);
    gyroscope.endTransmission(false);
    gyroscope.requestFrom(MPU6050_ADDRESS, 6, true);

    accelX = (gyroscope.read() << 8) | gyroscope.read();
    accelY = (gyroscope.read() << 8) | gyroscope.read();
    accelZ = (gyroscope.read() << 8) | gyroscope.read();

    // Leitura dos valores dos giros
    gyroscope.beginTransmission(MPU6050_ADDRESS);
    gyroscope.write(MPU6050_GYRO_XOUT_H);
    gyroscope.endTransmission(false);
    gyroscope.requestFrom(MPU6050_ADDRESS, 6, true);

    gyroX = (gyroscope.read() << 8) | gyroscope.read();
    gyroY = (gyroscope.read() << 8) | gyroscope.read();
    gyroZ = (gyroscope.read() << 8) | gyroscope.read();

    accelX *= -1;
    accelY *= -1;

    accelX = map(accelX,16300,-16300,90,-90);
    accelY = map(accelY,16300,-16300,90,-90);
    switch(mode){
        case 0:
            return accelX;
        case 1:
            return accelY;
        case 2:
            return accelZ;
        case 3:
            return gyroX;
        case 4:
            return gyroY;
        case 5:
            return gyroZ;
    }
}

// -----------------------------------------------------------------------------------
// ----------------------------------- SOM -------------------------------------------
// -----------------------------------------------------------------------------------

void SOAnjos::makeSound(int frequency, int time){
    NewTone(26, frequency, time);
}
// -----------------------------------------------------------------------------------
// ---------------------------------- BOTÕES -----------------------------------------
// -----------------------------------------------------------------------------------
typedef void (*buttonActionFunc)();
typedef void (*buttonActionLongPressFunc)(void *oneButton);

void SOAnjos::defineButtonAction(int button, int state, buttonActionFunc func, buttonActionLongPressFunc longPressFunc){
  switch(button){
      case 2:
          switch(state){
              case 1:
                  enterButton.attachClick(func);
                  break;
              case 2:
                  enterButton.attachDoubleClick(func);
                  break;
              case 3:
                  enterButton.attachLongPressStart(longPressFunc, &enterButton);
                  break;
              case 4:
                  enterButton.attachDuringLongPress(longPressFunc, &enterButton);
                break;
          }
          break;
      case 3:
          switch(state){
              case 1:
                  plusButton.attachClick(func);
                  break;
              case 2:
                  plusButton.attachDoubleClick(func);
                  break;
              case 3:
                  plusButton.attachLongPressStart(longPressFunc, &plusButton);
                  break;
              case 4:
                  plusButton.attachDuringLongPress(longPressFunc, &plusButton);
                  break;
          }
          break;
      case 4:
          switch(state){
              case 1:
                  lessButton.attachClick(func);
                  break;
              case 2:
                  lessButton.attachDoubleClick(func);
                  break;
              case 3:
                  lessButton.attachLongPressStart(longPressFunc, &lessButton);
                  break;
              case 4:
                  lessButton.attachDuringLongPress(longPressFunc, &lessButton);
                  break;
          }
          break;
  }
}

void SOAnjos::handleEnterButtonPress(){
    int time = buzzerActivate == 1 ? 120 : 0;
    switch(screen){
        case 0:  //pageMenu 
            if(option > 0 && option < 6){
                NewTone(26,659,time);
                screen = option;
                switch(option){
                    case 1: 
                        pageProgram();
                        break;
                    case 2:
                        pageMotor();
                        break;
                    case 3:
                        pageApp();
                        break;
                    case 4:
                        pageSensor();
                        break;
                    case 5:
                        pageConfig();
                        break;
                }
            }else{
                NewTone(26,250,time);
            }
            break;
        case 1:  //pageProgram
            break;
        case 2:  //pageMotor
            break;
        case 3:  //pageApp
            break;
        case 4:  //pageSensor
            break;
        case 5:  //pageConfig
            break;
        default:
            NewTone(26,250,time);
            break;
    } 
}

void SOAnjos::handleBackButtonPress(){
    int time = buzzerActivate == 1 ? 120 : 0;
    switch(screen){
        case 0:  //pageMenu
            NewTone(26,250,time);
            break;
        // case 1:  //pageProgram
        //     break;
        // case 2:  //pageMotor
        //     break;
        // case 3:  //pageApp
        //     break;
        // case 4:  //pageSensor
        //     break;
        // case 5:  //pageConfig
        //     break;
        default:
            NewTone(26,659,time);
            delay(100);
            screen = 0;
            pageMenu();
            
            setMotorPin(M1, 0);
            setMotorPin(M2, 0);
            setMotorPin(M3, 0);
            setMotorPin(M4, 0);
            clearCodePin(m1);
            clearCodePin(m2);
            clearCodePin(m3);
            clearCodePin(m4);
            
            
            break;
    }
    startProgram = 0;
    setupProgram = 0;
}

void SOAnjos::handleLessButtonPress(){
    int time = buzzerActivate == 1 ? 120 : 0;
    switch(screen){
        case 0:  //pageMenu
            if(option < 5){
                NewTone(26,659,time);
                option++;
                updateButtonPreselect();
            }else{
                NewTone(26,250,time);
            }
            break;
        case 1:  //pageProgram
            break;
        case 2:  //pageMotor
            break;
        case 3:  //pageApp
            break;
        case 4:  //pageSensor
            sensorSelected --;
            sensorSelected = sensorSelected >= 0 ? sensorSelected : 6;
            break;
        case 5:  //pageConfig
            if(option < 1){
                NewTone(26,659,time);
                option++;
                updateButtonPreselect();
            }else{
                NewTone(26,250,time);
            }
            break;
        default:
            NewTone(26,250,time);
            break;
    }
}

void SOAnjos::handlePlusButtonPress(){
    int time = buzzerActivate == 1 ? 120 : 0;
    switch(screen){
        case 0:  //pageMenu
            if(option > 0){
                NewTone(26,659,time);
                option--;
                updateButtonPreselect();
            }else if(option == 0){
                NewTone(26,659,time);
                option++;
                updateButtonPreselect();
            }else{
                NewTone(26,250,time);
            }
            break;
        case 1:  //pageProgram
            break;
        case 2:  //pageMotor
            break;
        case 3:  //pageApp
            break;
        case 4:  //pageSensor
            sensorSelected ++;
            sensorSelected = sensorSelected <= 6 ? sensorSelected : 0;
            break;
        case 5:  //pageConfig
            if(option > 0){
                NewTone(26,659,time);
                option--;
                updateButtonPreselect();
            }else if(option == 0){
                NewTone(26,659,time);
                option++;
                updateButtonPreselect();
            }else{
                NewTone(26,250,time);
            }
            break;
        default:
            NewTone(26,250,time);
            break;
    }
}

// -----------------------------------------------------------------------------------
// ------------------------------------ SO -------------------------------------------
// -----------------------------------------------------------------------------------

void SOAnjos::pageMenu(){
    option = 0;
    if (screen == 0){
        tft.fillScreen(TFT_BACKGROUND);
        tft.setFont(FM9);
        printText("CITTIUS", ILI9341_WHITE, 10, 20, 1);
        tft.fillRoundRect(0, 25, 240, 2, 0, 0x0A0A);

        uint16_t x = (tft.width() - BUTTON_W) / 8;
        uint16_t y = tft.height() / 3 - BUTTON_H + 30;

        tft.drawRoundRect(x,y, BUTTON_W, BUTTON_H, 20, 0x0A0A);
        tft.drawRoundRect(x-1,y-1, BUTTON_W+2, BUTTON_H+2, 20, 0x0A0A);
        printCenteredText("PROGRAMA", ILI9341_YELLOW,x,y, BUTTON_W, BUTTON_H,1);
        xCordinatesButton[0] = x;
        yCordinatesButton[0] = y;

        y = tft.height() / 3 + 50;

        tft.drawRoundRect(x,y, BUTTON_W, BUTTON_H, 20, 0x0A0A);
        tft.drawRoundRect(x-1,y-1, BUTTON_W+2, BUTTON_H+2, 20, 0x0A0A);
        printCenteredText("MOTOR", ILI9341_YELLOW,x,y, BUTTON_W, BUTTON_H,1);
        xCordinatesButton[1] = x;
        yCordinatesButton[1] = y;

        x = tft.height() / 3 + 20;

        tft.drawRoundRect(x,y, BUTTON_W, BUTTON_H, 20, 0x0A0A);
        tft.drawRoundRect(x-1,y-1, BUTTON_W+2, BUTTON_H+2, 20, 0x0A0A);
        printCenteredText("SENSOR", ILI9341_YELLOW,x,y, BUTTON_W, BUTTON_H,1);
        xCordinatesButton[2] = x;
        yCordinatesButton[2] = y;

        y = tft.height() / 3 - 55;

        tft.drawRoundRect(x,y, BUTTON_W, BUTTON_H, 20, 0x0A0A);
        tft.drawRoundRect(x-1,y-1, BUTTON_W+2, BUTTON_H+2, 20, 0x0A0A);
        printCenteredText("APP", ILI9341_YELLOW,x,y, BUTTON_W, BUTTON_H,1);
        xCordinatesButton[3] = x;
        yCordinatesButton[3] = y;

        tft.drawRoundRect(20,250, 200, 55, 20, 0x0A0A);
        tft.drawRoundRect(19,249, 202, 57, 20, 0x0A0A);
        printCenteredText("CONFIG", ILI9341_YELLOW,20,250, 200, 55, 1);
        xCordinatesButton[4] = 20;
        yCordinatesButton[4] = 250;
        
    }
}

void SOAnjos::pageProgram(){
    tft.fillScreen(TFT_BACKGROUND);
    printText("PROGRAMA", ILI9341_WHITE, 10, 20, 1);
    tft.fillRoundRect(0, 25, 240, 2, 0, 0x0A0A);
}

void SOAnjos::pageMotor(){
    tft.fillScreen(TFT_BACKGROUND);
    printText("MOTORES", ILI9341_WHITE, 10, 20, 1);
    tft.fillRoundRect(0, 25, 240, 2, 0, 0x0A0A);
    uint16_t w, h, x, y;
    w = tft.width() - 20;
    h = (tft.height() - 75)/4 +10;
    x = 15;
    y = 3 + ((tft.height() -90)/4);
    //------------- M1 ------------------
    tft.setFont(FSB9);
    tft.drawRect(10,28, w, h, 0x0A0A); // Draw rectangle outline
    printCenteredText("M1", ILI9341_WHITE, 0, 33, tft.width(),20,1);

    drawSlider(x, y , tft.width() - 30, 15, 50);
    printText("Encoder:", ILI9341_WHITE, x + 120, y + 27, 1);
    printText(String(getCodePin(m1)), ILI9341_WHITE, x + 175, y + 27, 1);
    y = y + 2 + h;
    //------------- M2 ------------------
    tft.setFont(FSB9);
    tft.drawRect(10,30 + h, w, h, 0x0A0A); // Draw rectangle outline
    printCenteredText("M2", ILI9341_WHITE, 0, 35 + h, tft.width(),20,1);

    drawSlider(x, y, tft.width() - 30, 15, 50);
    printText("Encoder:", ILI9341_WHITE, x + 120, y + 27, 1);
    printText(String(getCodePin(m2)), ILI9341_WHITE, x + 175, y + 27, 1);

    y = y + 2 + h;
    //------------- M3 ------------------
    tft.setFont(FSB9);
    tft.drawRect(10,32 + (2 * h), w, h, 0x0A0A); // Draw rectangle outline
    printCenteredText("M3", ILI9341_WHITE, 0, 37 + (2 * h), tft.width(),20,1);

    drawSlider(x, y, tft.width() - 30, 15, 50);
    printText("Encoder:", ILI9341_WHITE, x + 120, y + 27, 1);
    printText(String(getCodePin(m3)), ILI9341_WHITE, x + 175, y + 27, 1);

    y = y + 2 + h;
    //------------- M4 ------------------
    tft.setFont(FSB9);
    tft.drawRect(10,34 + (3 * h), w, h, 0x0A0A); // Draw rectangle outline
    printCenteredText("M4", ILI9341_WHITE, 0, 39 + (3 * h), tft.width(),20,1);

    drawSlider(x, y, tft.width() - 30, 15, 50);
    printText("Encoder:", ILI9341_WHITE, x + 120, y + 27, 1);
    printText(String(getCodePin(m4)), ILI9341_WHITE, x + 175, y + 27, 1);
}

void SOAnjos::pageMotorUpdateValues(){
    uint16_t w, h, x, y;
    w = tft.width() - 20;
    h = (tft.height() - 75)/4 +10;
    x = 15;
    y = 3 + ((tft.height() -90)/4);

    printText("Encoder:", ILI9341_WHITE, x + 120, y + 27, 1);
    tft.fillRect(x+175, y+27, 30, 8, TFT_BACKGROUND);
    printText(String(getCodePin(m1)), ILI9341_WHITE, x + 175, y + 27, 1);

    y = y + 2 + h;

    printText("Encoder:", ILI9341_WHITE, x + 120, y + 27, 1);
    tft.fillRect(x+175, y+27, 30, 8, TFT_BACKGROUND);
    printText(String(getCodePin(m2)), ILI9341_WHITE, x + 175, y + 27, 1);

    y = y + 2 + h;

    printText("Encoder:", ILI9341_WHITE, x + 120, y + 27, 1);
    tft.fillRect(x+175, y+27, 30, 8, TFT_BACKGROUND);
    printText(String(getCodePin(m3)), ILI9341_WHITE, x + 175, y + 27, 1);

    y = y + 2 + h;

    printText("Encoder:", ILI9341_WHITE, x + 120, y + 27, 1);
    tft.fillRect(x+175, y+27, 30, 8, TFT_BACKGROUND);
    printText(String(getCodePin(m4)), ILI9341_WHITE, x + 175, y + 27, 1);
}

void SOAnjos::drawSlider(int x, int y, int w, int h, int value){
    int sliderWidth = w;
    int sliderHeight = h;
    int sliderX = x;
    int sliderY = y;

    // Desenha o fundo do slider
    tft.fillRect(x-2, sliderY-10, sliderWidth+3, 10, TFT_BACKGROUND);
    tft.fillRoundRect(sliderX, sliderY, sliderWidth, sliderHeight, 5,ILI9341_YELLOW);
    tft.fillRect(x-2, sliderY+sliderHeight, sliderWidth+3, 10, TFT_BACKGROUND);

    // Calcula a largura do valor do slider
    int fillWidth = map(value, 0, 100, 15, sliderWidth-2);

    // Desenha o valor do slider
    tft.fillRoundRect(fillWidth-2, sliderY-10, 20, 35, 5, ILI9341_WHITE);
    tft.drawLine(fillWidth+8, sliderY-10, fillWidth+8, sliderY+24, ILI9341_RED);

    // Desenha o texto do valor
    tft.setFont(FF0);
    printText("Potencia:", ILI9341_WHITE, x , y + 27, 1);
    int power = map(value, 0, 100, -100, 100);
    tft.fillRect(x+59, y+26, 30, 8, TFT_BACKGROUND);
    printText(String(power), ILI9341_WHITE, x + 60, y + 27, 1);
    printText("%", ILI9341_WHITE,x + 78, y + 27, 1);
}

void SOAnjos::pageConfig(){
    tft.fillScreen(TFT_BACKGROUND);
    printText("CONFIG", ILI9341_WHITE, 10, 20, 1);
    tft.fillRoundRect(0, 25, 240, 2, 0, 0x0A0A);

    tft.fillRoundRect(xCordinatesButton[0], yCordinatesButton[0], BUTTON_W, BUTTON_H, 20, buzzerActivate == 1 ? ILI9341_GREEN : ILI9341_RED);
    tft.drawRoundRect(xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H, 20, 0x0A0A);
    tft.drawRoundRect(xCordinatesButton[0]-1,yCordinatesButton[0]-1, BUTTON_W+2, BUTTON_H+2, 20, 0x0A0A);
    printCenteredText("SOM", buzzerActivate == 1 ? TFT_BACKGROUND : ILI9341_WHITE,xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H,1);
}

void SOAnjos::pageSensor(){
    tft.fillScreen(TFT_BACKGROUND);
    printText("SENSORES", ILI9341_WHITE, 10, 20, 1);
    tft.fillRoundRect(0, 25, 240, 2, 0, 0x0A0A);

    int w = (tft.width() - 26)/4; 
    int x = 10;
    int y = 30;
    int spaceY = ((tft.height()-35)- (w*4))/3;

    printCenteredText("↑",  ILI9341_YELLOW, 0, 10, tft.width(), tft.height(), 1);
    printCenteredText("ULTRASOM", ILI9341_YELLOW, 0, 20, tft.width(), tft.height(), 1);
    printCenteredText("↓",  ILI9341_YELLOW, 0, 30, tft.width(), tft.height(), 1);

    tft.drawRoundRect(x,y, w, w, 10, 0x0A0A);
    tft.drawRoundRect(x -1, y -1, w+2, w+2, 10, 0x0A0A);
    printCenteredText("P1", ILI9341_WHITE, x, y-10,w, w, 1);
    int value1 = 0;
    printCenteredText(String(value1), ILI9341_YELLOW, x, y+15, w, w, 1);

    x = x + 2 + w;
    tft.drawRoundRect(x, 30, w, w, 10, 0x0A0A);
    tft.drawRoundRect(x -1, y -1, w+2, w+2, 10, 0x0A0A);
    printCenteredText("P2", ILI9341_WHITE, x, y-10,w, w, 1);
    int value2 = getUltrassonicSensor(2);
    printCenteredText(String(value2), ILI9341_YELLOW, x, y+15, w, w, 1);

    x = x + 2 + w;
    tft.drawRoundRect(x, 30, w, w, 10, 0x0A0A);
    tft.drawRoundRect(x -1, y -1, w+2, w+2, 10, 0x0A0A);
    printCenteredText("P3", ILI9341_WHITE, x, y-10,w, w, 1);
    int value3 = getUltrassonicSensor(3);
    printCenteredText(String(value3), ILI9341_YELLOW, x, y+15, w, w, 1);

    x = x + 2 + w;
    tft.drawRoundRect(x, 30, w, w, 10, 0x0A0A);
    tft.drawRoundRect(x -1, y -1, w+2, w+2, 10, 0x0A0A);
    printCenteredText("P4", ILI9341_WHITE, x, y-10,w, w, 1);
    int value4 = getUltrassonicSensor(4);
    printCenteredText(String(value4), ILI9341_YELLOW, x, y+15, w, w, 1);

    y = y + spaceY + w;
    tft.drawRoundRect(x, 30, w, w, 10, 0x0A0A);
    tft.drawRoundRect(x -1, y -1, w+2, w+2, 10, 0x0A0A);
    printCenteredText("P7", ILI9341_WHITE, x, y-10,w, w, 1);
    int value7 = getUltrassonicSensor(7);
    printCenteredText(String(value7), ILI9341_YELLOW, x, y+15, w, w, 1);

    x = 10;
    tft.drawRoundRect(x, 30, w, w, 10, 0x0A0A);
    tft.drawRoundRect(x -1, y -1, w+2, w+2, 10, 0x0A0A);
    printCenteredText("P5", ILI9341_WHITE, x, y-10,w, w, 1);
    int value5 = getUltrassonicSensor(5);
    printCenteredText(String(value7), ILI9341_YELLOW, x, y+15, w, w, 1);

    y = y + spaceY + w;
    tft.drawRoundRect(x, 30, w, w, 10, 0x0A0A);
    tft.drawRoundRect(x -1, y -1, w+2, w+2, 10, 0x0A0A);
    printCenteredText("P6", ILI9341_WHITE, x, y-10,w, w, 1);
    int value6 = getUltrassonicSensor(6);
    printCenteredText(String(value7), ILI9341_YELLOW, x, y+15, w, w, 1);

    x = (2 + w)*3 + x;
    tft.drawRoundRect(x, 30, w, w, 10, 0x0A0A);
    tft.drawRoundRect(x -1, y -1, w+2, w+2, 10, 0x0A0A);
    printCenteredText("P8", ILI9341_WHITE, x, y-10,w, w, 1);
    int value8 = getUltrassonicSensor(8);
    printCenteredText(String(value8), ILI9341_YELLOW, x, y+15, w, w, 1);

    y = y + spaceY + w;
    x = 10;
    tft.drawRoundRect(x,y, w, w, 10, 0x0A0A);
    tft.drawRoundRect(x -1, y -1, w+2, w+2, 10, 0x0A0A);
    printCenteredText("M1", ILI9341_WHITE, x, y-10,w, w, 1);
    int value9 = 0;
    printCenteredText(String(value9), ILI9341_YELLOW, x, y+15, w, w, 1);

    x = x + 2 + w;
    tft.drawRoundRect(x, 30, w, w, 10, 0x0A0A);
    tft.drawRoundRect(x -1, y -1, w+2, w+2, 10, 0x0A0A);
    printCenteredText("M2", ILI9341_WHITE, x, y-10,w, w, 1);
    int value10 = getUltrassonicSensor(10);
    printCenteredText(String(value10), ILI9341_YELLOW, x, y+15, w, w, 1);

    x = x + 2 + w;
    tft.drawRoundRect(x, 30, w, w, 10, 0x0A0A);
    tft.drawRoundRect(x -1, y -1, w+2, w+2, 10, 0x0A0A);
    printCenteredText("M3", ILI9341_WHITE, x, y-10,w, w, 1);
    int value11 = getUltrassonicSensor(11);
    printCenteredText(String(value11), ILI9341_YELLOW, x, y+15, w, w, 1);

    x = x + 2 + w;
    tft.drawRoundRect(x, 30, w, w, 10, 0x0A0A);
    tft.drawRoundRect(x -1, y -1, w+2, w+2, 10, 0x0A0A);
    printCenteredText("M4", ILI9341_WHITE, x, y-10,w, w, 1);
    int value12 = getUltrassonicSensor(12);
    printCenteredText(String(value12), ILI9341_YELLOW, x, y+15, w, w, 1);
}

void SOAnjos::pageSensorUpdateValues(){
    if(screen == 4){
        int v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12;
        String nameSensor;
        int w = (tft.width() - 26)/4; 
        int x = 10;
        int y = 30;
        int spaceY = ((tft.height()-35)- (w*4))/3;
        switch(sensorSelected){
            case 0: // ULTRASSONICO
                nameSensor = "ULTRASOM";
                v1 = getUltrassonicSensor(1);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v2 = getUltrassonicSensor(2);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v3 = getUltrassonicSensor(3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v4 = getUltrassonicSensor(4);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v5 = getUltrassonicSensor(5);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v6 = getUltrassonicSensor(6);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v7 = getUltrassonicSensor(7);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v8 = getUltrassonicSensor(8);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v9 = getUltrassonicSensor(9);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v10 = getUltrassonicSensor(10);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v11 = getUltrassonicSensor(11);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v12 = getUltrassonicSensor(12);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                break;
            case 1: // TOQUE
                nameSensor = "TOQUE";
                v1 = getTouchSensor(1);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v2 = getTouchSensor(2);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v3 = getTouchSensor(3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v4 = getTouchSensor(4);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v5 = getTouchSensor(5);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v6 = getTouchSensor(6);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v7 = getTouchSensor(7);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v8 = getTouchSensor(8);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v9 = getTouchSensor(9);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v10 = getTouchSensor(10);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v11 = getTouchSensor(11);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v12 = getTouchSensor(12);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                break;
            case 2: // SOM
                nameSensor = "SOM";
                v1 = getSoundSensor(1);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v2 = getSoundSensor(2);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v3 = getSoundSensor(3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v4 = getSoundSensor(4);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v5 = getSoundSensor(5);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v6 = getSoundSensor(6);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v7 = getSoundSensor(7);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v8 = getSoundSensor(8);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v9 = getSoundSensor(9);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v10 = getSoundSensor(10);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v11 = getSoundSensor(11);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v12 = getSoundSensor(12);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                break;
            case 3: // COR
                nameSensor = "COR";
                v1 = getColorSensor(1, 0);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v2 = getColorSensor(2, 0);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v3 = getColorSensor(3, 0);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v4 = getColorSensor(4, 0);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v5 = getColorSensor(5, 0);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                // v6 = getColorSensor(6, 0);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v6 = 7;
                v7 = getColorSensor(7, 0);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v8 = getColorSensor(8, 0);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v9 = getColorSensor(9, 0);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v10 = getColorSensor(10, 0);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v11 = getColorSensor(11, 0);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v12 = getColorSensor(12, 0);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                break;
            case 4: // TEMPERATURA
                nameSensor = "TEMPERATURA";
                v1 = getTemperatureSensor(1);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v2 = getTemperatureSensor(2);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v3 = getTemperatureSensor(3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v4 = getTemperatureSensor(4);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v5 = getTemperatureSensor(5);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                // v6 = getTemperatureSensor(6);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v6 = 0;
                v7 = getTemperatureSensor(7);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();            
                v8 = getTemperatureSensor(8);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v9 = getTemperatureSensor(9);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v10 = getTemperatureSensor(10);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v11 = getTemperatureSensor(11);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v12 = getTemperatureSensor(12);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                break;
            case 5: // INFRAVERMELHO
                nameSensor = "INFRAVER.";
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v1 = getInfraredSensor(1,3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v2 = getInfraredSensor(2,3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v3 = getInfraredSensor(3,3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v4 = getInfraredSensor(4,3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v5 = getInfraredSensor(5,3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v6 = getInfraredSensor(6,3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v7 = getInfraredSensor(7,3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v8 = getInfraredSensor(8,3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v9 = getInfraredSensor(9,3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v10 = getInfraredSensor(10,3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v11 = getInfraredSensor(11,3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v12 = getInfraredSensor(12,3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                break;
            case 6: // GIROSCOPIO
                nameSensor = "GIROSC.";
                v1 = getGyroscopeSensor(1, 3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v2 = getGyroscopeSensor(2, 3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v3 = getGyroscopeSensor(3, 3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v4 = getGyroscopeSensor(4, 3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v5 = getGyroscopeSensor(5, 3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v6 = getGyroscopeSensor(6, 3);
                // v6 = 0;
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v7 = getGyroscopeSensor(7, 3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v8 = getGyroscopeSensor(8, 3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v9 = getGyroscopeSensor(9, 3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v10 = getGyroscopeSensor(10, 3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v11 = getGyroscopeSensor(11, 3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                v12 = getGyroscopeSensor(12, 3);
                enterButton.tick(); // Habilita evento do botão Enter
                backButton.tick();
                plusButton.tick();
                lessButton.tick();
                break;
        }
        if(screen == 4){
        printCenteredText("^",  ILI9341_YELLOW, 0, -10, tft.width(), tft.height(), 1);
        tft.fillRect(tft.width()/2 - 60, tft.height()/2 + 5, 130, 17, TFT_BACKGROUND);
        printCenteredText(nameSensor, ILI9341_YELLOW, 0, 20, tft.width(), tft.height(), 1);
        printCenteredText("v",  ILI9341_YELLOW, 0, 50, tft.width(), tft.height(), 1);

        // -------------------- P1 --------------------------
        if(sensorSelected == 1){
            tft.fillRoundRect(x, y, w, w, 10, v1 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText("P1", v1 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, v1 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText(String(v1), v1 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y+15, w, w, 1);
        } else if(sensorSelected == 3){
            switch(v1){
                case 1:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_RED);
                    printCenteredText("P1",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_RED );
                    printCenteredText(String(v1), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 2:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_GREEN);
                    printCenteredText("P1",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_GREEN );
                    printCenteredText(String(v1), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 3:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLUE);
                    printCenteredText("P1",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLUE);
                    printCenteredText(String(v1), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 4:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_YELLOW);
                    printCenteredText("P1",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_YELLOW );
                    printCenteredText(String(v1), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 5:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_ORANGE);
                    printCenteredText("P1",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_ORANGE );
                    printCenteredText(String(v1), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 6:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_WHITE);
                    printCenteredText("P1",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_WHITE );
                    printCenteredText(String(v1), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 7: 
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLACK);
                    printCenteredText("P1",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLACK );
                    printCenteredText(String(v1), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
            }
        }else{
            tft.fillRoundRect(x, y, w, w, 10, TFT_BACKGROUND);
            printCenteredText("P1", ILI9341_WHITE, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, TFT_BACKGROUND);
            printCenteredText(String(v1), ILI9341_YELLOW, x, y+15, w, w, 1);
        }
        
        x = x + 2 + w;
        // -------------------- P2 --------------------------
        if(sensorSelected == 1){
            tft.fillRoundRect(x, y, w, w, 10, v2 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText("P2", v2 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, v2 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText(String(v2), v2 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y+15, w, w, 1);
        } else if(sensorSelected == 3){
            switch(v2){
                case 1:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_RED);
                    printCenteredText("P2",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_RED );
                    printCenteredText(String(v2), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 2:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_GREEN);
                    printCenteredText("P2",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_GREEN );
                    printCenteredText(String(v2), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 3:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLUE);
                    printCenteredText("P2",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLUE);
                    printCenteredText(String(v2), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 4:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_YELLOW);
                    printCenteredText("P2",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_YELLOW );
                    printCenteredText(String(v2), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 5:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_ORANGE);
                    printCenteredText("P2",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_ORANGE );
                    printCenteredText(String(v2), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 6:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_WHITE);
                    printCenteredText("P2",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_WHITE );
                    printCenteredText(String(v2), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 7: 
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLACK);
                    printCenteredText("P2",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLACK );
                    printCenteredText(String(v2), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
            }
        }else{
            tft.fillRoundRect(x, y, w, w, 10, TFT_BACKGROUND);
            printCenteredText("P2", ILI9341_WHITE, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, TFT_BACKGROUND);
            printCenteredText(String(v2), ILI9341_YELLOW, x, y+15, w, w, 1);
        }
        
        x = x + 2 + w;

        // -------------------- P3 --------------------------
        if(sensorSelected == 1){
            tft.fillRoundRect(x, y, w, w, 10, v3 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText("P3", v3 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, v3 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText(String(v3), v3 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y+15, w, w, 1);
        } else if(sensorSelected == 3){
            switch(v3){
                case 1:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_RED);
                    printCenteredText("P3",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_RED );
                    printCenteredText(String(v3), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 2:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_GREEN);
                    printCenteredText("P3",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_GREEN );
                    printCenteredText(String(v3), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 3:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLUE);
                    printCenteredText("P3",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLUE);
                    printCenteredText(String(v3), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 4:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_YELLOW);
                    printCenteredText("P3",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_YELLOW );
                    printCenteredText(String(v3), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 5:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_ORANGE);
                    printCenteredText("P3",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_ORANGE );
                    printCenteredText(String(v3), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 6:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_WHITE);
                    printCenteredText("P3",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_WHITE );
                    printCenteredText(String(v3), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 7: 
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLACK);
                    printCenteredText("P3",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLACK );
                    printCenteredText(String(v3), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
            }
        }else{
            tft.fillRoundRect(x, y, w, w, 10, TFT_BACKGROUND);
            printCenteredText("P3", ILI9341_WHITE, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, TFT_BACKGROUND);
            printCenteredText(String(v3), ILI9341_YELLOW, x, y+15, w, w, 1);
        }
        

        x = x + 2 + w;

        // -------------------- P4 --------------------------
        if(sensorSelected == 1){
            tft.fillRoundRect(x, y, w, w, 10, v4 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText("P4", v4 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, v4 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText(String(v4), v4 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y+15, w, w, 1);
        } else if(sensorSelected == 3){
            switch(v4){
                case 1:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_RED);
                    printCenteredText("P4",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_RED );
                    printCenteredText(String(v4), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 2:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_GREEN);
                    printCenteredText("P4",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_GREEN );
                    printCenteredText(String(v4), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 3:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLUE);
                    printCenteredText("P4",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLUE);
                    printCenteredText(String(v4), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 4:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_YELLOW);
                    printCenteredText("P4",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_YELLOW );
                    printCenteredText(String(v4), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 5:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_ORANGE);
                    printCenteredText("P4",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_ORANGE );
                    printCenteredText(String(v4), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 6:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_WHITE);
                    printCenteredText("P4",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_WHITE );
                    printCenteredText(String(v4), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 7: 
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLACK);
                    printCenteredText("P4",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLACK );
                    printCenteredText(String(v4), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
            }
        }else{
            tft.fillRoundRect(x, y, w, w, 10, TFT_BACKGROUND);
            printCenteredText("P4", ILI9341_WHITE, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, TFT_BACKGROUND);
            printCenteredText(String(v4), ILI9341_YELLOW, x, y+15, w, w, 1);
        }
        

        y = y + spaceY + w;
        // -------------------- P7 --------------------------
        if(sensorSelected == 1){
            tft.fillRoundRect(x, y, w, w, 10, v7 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText("P7", v7 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, v7 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText(String(v7), v7 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y+15, w, w, 1);
        } else if(sensorSelected == 3){
            switch(v7){
                case 1:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_RED);
                    printCenteredText("P7",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_RED );
                    printCenteredText(String(v7), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 2:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_GREEN);
                    printCenteredText("P7",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_GREEN );
                    printCenteredText(String(v7), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 3:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLUE);
                    printCenteredText("P7",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLUE);
                    printCenteredText(String(v7), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 4:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_YELLOW);
                    printCenteredText("P7",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_YELLOW );
                    printCenteredText(String(v7), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 5:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_ORANGE);
                    printCenteredText("P7",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_ORANGE );
                    printCenteredText(String(v7), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 6:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_WHITE);
                    printCenteredText("P7",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_WHITE );
                    printCenteredText(String(v7), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 7: 
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLACK);
                    printCenteredText("P7",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLACK );
                    printCenteredText(String(v7), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
            }
        }else{
            tft.fillRoundRect(x, y, w, w, 10, TFT_BACKGROUND);
            printCenteredText("P7", ILI9341_WHITE, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, TFT_BACKGROUND);
            printCenteredText(String(v7), ILI9341_YELLOW, x, y+15, w, w, 1);
        }
        

        x = 10;
        // -------------------- P5 --------------------------
        if(sensorSelected == 1){
            tft.fillRoundRect(x, y, w, w, 10, v5 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText("P5", v5 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, v5 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText(String(v5), v5 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y+15, w, w, 1);
        } else if(sensorSelected == 3){
            switch(v5){
                case 1:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_RED);
                    printCenteredText("P5",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_RED );
                    printCenteredText(String(v5), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 2:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_GREEN);
                    printCenteredText("P5",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_GREEN );
                    printCenteredText(String(v5), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 3:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLUE);
                    printCenteredText("P5",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLUE);
                    printCenteredText(String(v5), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 4:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_YELLOW);
                    printCenteredText("P5",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_YELLOW );
                    printCenteredText(String(v5), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 5:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_ORANGE);
                    printCenteredText("P5",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_ORANGE );
                    printCenteredText(String(v5), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 6:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_WHITE);
                    printCenteredText("P5",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_WHITE );
                    printCenteredText(String(v5), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 7: 
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLACK);
                    printCenteredText("P5",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLACK );
                    printCenteredText(String(v5), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
            }
        }else{
            tft.fillRoundRect(x, y, w, w, 10, TFT_BACKGROUND);
            printCenteredText("P5", ILI9341_WHITE, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, TFT_BACKGROUND);
            printCenteredText(String(v5), ILI9341_YELLOW, x, y+15, w, w, 1);
        }
        

        y = y + spaceY + w;
        // -------------------- P6 --------------------------
        if(sensorSelected == 1){
            tft.fillRoundRect(x, y, w, w, 10, v6 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText("P6", v6 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, v6 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText(String(v6), v6 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y+15, w, w, 1);
        } else if(sensorSelected == 3){
            switch(v6){
                case 1:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_RED);
                    printCenteredText("P6",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_RED );
                    printCenteredText(String(v6), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 2:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_GREEN);
                    printCenteredText("P6",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_GREEN );
                    printCenteredText(String(v6), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 3:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLUE);
                    printCenteredText("P6",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLUE);
                    printCenteredText(String(v6), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 4:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_YELLOW);
                    printCenteredText("P6",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_YELLOW );
                    printCenteredText(String(v6), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 5:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_ORANGE);
                    printCenteredText("P6",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_ORANGE );
                    printCenteredText(String(v6), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 6:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_WHITE);
                    printCenteredText("P6",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_WHITE );
                    printCenteredText(String(v6), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 7: 
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLACK);
                    printCenteredText("P6",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLACK );
                    printCenteredText(String(v6), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
            }
        }else{
            tft.fillRoundRect(x, y, w, w, 10, TFT_BACKGROUND);
            printCenteredText("P6", ILI9341_WHITE, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, TFT_BACKGROUND);
            printCenteredText(String(v6), ILI9341_YELLOW, x, y+15, w, w, 1);
        }
        
        x = (2 + w)*3 + x;
        // -------------------- P8 --------------------------
        if(sensorSelected == 1){
            tft.fillRoundRect(x, y, w, w, 10, v8 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText("P8", v8 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, v8 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText(String(v8), v8 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y+15, w, w, 1);
        } else if(sensorSelected == 3){
            switch(v8){
                case 1:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_RED);
                    printCenteredText("P8",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_RED );
                    printCenteredText(String(v8), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 2:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_GREEN);
                    printCenteredText("P8",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_GREEN );
                    printCenteredText(String(v8), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 3:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLUE);
                    printCenteredText("P8",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLUE);
                    printCenteredText(String(v8), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 4:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_YELLOW);
                    printCenteredText("P8",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_YELLOW );
                    printCenteredText(String(v8), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 5:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_ORANGE);
                    printCenteredText("P8",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_ORANGE );
                    printCenteredText(String(v8), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 6:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_WHITE);
                    printCenteredText("P8",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_WHITE );
                    printCenteredText(String(v8), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 7: 
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLACK);
                    printCenteredText("P8",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLACK );
                    printCenteredText(String(v8), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
            }
        }else{
            tft.fillRoundRect(x, y, w, w, 10, TFT_BACKGROUND);
            printCenteredText("P8", ILI9341_WHITE, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, TFT_BACKGROUND);
            printCenteredText(String(v8), ILI9341_YELLOW, x, y+15, w, w, 1);
        }
        
        y = y + spaceY + w;
        x = 10;
        // -------------------- M1 --------------------------
        if(sensorSelected == 1){
            tft.fillRoundRect(x, y, w, w, 10, v9 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText("M1", v9 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, v9 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText(String(v9), v9 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y+15, w, w, 1);
        } else if(sensorSelected == 3){
            switch(v9){
                case 1:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_RED);
                    printCenteredText("M1",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_RED );
                    printCenteredText(String(v9), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 2:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_GREEN);
                    printCenteredText("M1",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_GREEN );
                    printCenteredText(String(v9), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 3:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLUE);
                    printCenteredText("M1",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLUE);
                    printCenteredText(String(v9), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 4:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_YELLOW);
                    printCenteredText("M1",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_YELLOW );
                    printCenteredText(String(v9), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 5:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_ORANGE);
                    printCenteredText("M1",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_ORANGE );
                    printCenteredText(String(v9), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 6:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_WHITE);
                    printCenteredText("M1",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_WHITE );
                    printCenteredText(String(v9), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 7: 
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLACK);
                    printCenteredText("M1",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLACK );
                    printCenteredText(String(v9), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
            }
        }else{
            tft.fillRoundRect(x, y, w, w, 10, TFT_BACKGROUND);
            printCenteredText("M1", ILI9341_WHITE, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, TFT_BACKGROUND);
            printCenteredText(String(v9), ILI9341_YELLOW, x, y+15, w, w, 1);
        }
        
        x = x + 2 + w;
        // -------------------- M2 --------------------------
        if(sensorSelected == 1){
            tft.fillRoundRect(x, y, w, w, 10, v10 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText("M2", v10 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, v10 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText(String(v10), v10 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y+15, w, w, 1);
        } else if(sensorSelected == 3){
            switch(v10){
                case 1:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_RED);
                    printCenteredText("M2",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_RED );
                    printCenteredText(String(v10), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 2:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_GREEN);
                    printCenteredText("M2",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_GREEN );
                    printCenteredText(String(v10), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 3:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLUE);
                    printCenteredText("M2",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLUE);
                    printCenteredText(String(v10), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 4:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_YELLOW);
                    printCenteredText("M2",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_YELLOW );
                    printCenteredText(String(v10), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 5:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_ORANGE);
                    printCenteredText("M2",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_ORANGE );
                    printCenteredText(String(v10), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 6:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_WHITE);
                    printCenteredText("M2",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_WHITE );
                    printCenteredText(String(v10), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 7: 
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLACK);
                    printCenteredText("M2",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLACK );
                    printCenteredText(String(v10), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
            }
        }else{
            tft.fillRoundRect(x, y, w, w, 10, TFT_BACKGROUND);
            printCenteredText("M2", ILI9341_WHITE, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, TFT_BACKGROUND);
            printCenteredText(String(v10), ILI9341_YELLOW, x, y+15, w, w, 1);
        }
        

        x = x + 2 + w;
        // -------------------- M3 --------------------------
        if(sensorSelected == 1){
            tft.fillRoundRect(x, y, w, w, 10, v11 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText("M3", v11 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, v11 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText(String(v11), v11 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y+15, w, w, 1);
        } else if(sensorSelected == 3){
            switch(v11){
                case 1:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_RED);
                    printCenteredText("M3",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_RED );
                    printCenteredText(String(v11), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 2:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_GREEN);
                    printCenteredText("M3",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_GREEN );
                    printCenteredText(String(v11), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 3:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLUE);
                    printCenteredText("M3",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLUE);
                    printCenteredText(String(v11), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 4:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_YELLOW);
                    printCenteredText("M3",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_YELLOW );
                    printCenteredText(String(v11), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 5:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_ORANGE);
                    printCenteredText("M3",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_ORANGE );
                    printCenteredText(String(v11), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 6:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_WHITE);
                    printCenteredText("M3",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_WHITE );
                    printCenteredText(String(v11), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 7: 
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLACK);
                    printCenteredText("M3",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLACK );
                    printCenteredText(String(v11), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
            }
        }else{
            tft.fillRoundRect(x, y, w, w, 10, TFT_BACKGROUND);
            printCenteredText("M3", ILI9341_WHITE, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, TFT_BACKGROUND);
            printCenteredText(String(v11), ILI9341_YELLOW, x, y+15, w, w, 1);
        }
        
        x = x + 2 + w;
    // -------------------- M4 --------------------------
        if(sensorSelected == 1){
            tft.fillRoundRect(x, y, w, w, 10, v12 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText("M4", v12 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, v12 == 0 ? ILI9341_RED : ILI9341_GREEN);
            printCenteredText(String(v12), v12 == 0 ? ILI9341_WHITE : TFT_BACKGROUND, x, y+15, w, w, 1);
        } else if(sensorSelected == 3){
            switch(v12){
                case 1:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_RED);
                    printCenteredText("M4",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_RED );
                    printCenteredText(String(v12), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 2:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_GREEN);
                    printCenteredText("M4",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_GREEN );
                    printCenteredText(String(v12), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 3:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLUE);
                    printCenteredText("M4",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLUE);
                    printCenteredText(String(v12), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
                case 4:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_YELLOW);
                    printCenteredText("M4",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_YELLOW );
                    printCenteredText(String(v12), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 5:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_ORANGE);
                    printCenteredText("M4",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_ORANGE );
                    printCenteredText(String(v12), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 6:
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_WHITE);
                    printCenteredText("M4",  TFT_BACKGROUND, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_WHITE );
                    printCenteredText(String(v12), TFT_BACKGROUND , x, y+15, w, w, 1);
                    break;
                case 7: 
                    tft.fillRoundRect(x, y, w, w, 10, ILI9341_BLACK);
                    printCenteredText("M4",  ILI9341_WHITE, x, y-10,w, w, 1);
                    tft.fillRect(x, y+30, w, 12,ILI9341_BLACK );
                    printCenteredText(String(v12), ILI9341_WHITE , x, y+15, w, w, 1);
                    break;
            }
        }else{
            tft.fillRoundRect(x, y, w, w, 10, TFT_BACKGROUND);
            printCenteredText("M4", ILI9341_WHITE, x, y-10,w, w, 1);
            tft.fillRect(x, y+30, w, 12, TFT_BACKGROUND);
            printCenteredText(String(v12), ILI9341_YELLOW, x, y+15, w, w, 1);
        }
        }
    }
}

void SOAnjos::pageApp(){
    tft.fillScreen(TFT_BACKGROUND);
    printText("APP DABBLE", ILI9341_WHITE, 10, 20, 1);
    tft.fillRoundRect(0, 25, 240, 2, 0, 0x0A0A);

    tft.fillTriangle(100, 100, 140, 100, 120, 60, ILI9341_DARKGREY);
    tft.fillTriangle(20, 160, 60, 140, 60, 180, ILI9341_DARKGREY);
    tft.fillTriangle(220, 160, 180, 140, 180, 180, ILI9341_DARKGREY);
    tft.fillTriangle(100, 220, 140, 220, 120, 260, ILI9341_DARKGREY);

    tft.fillTriangle(100, 100, 140, 100, 120, 60, ILI9341_RED);
    tft.fillTriangle(20, 160, 60, 140, 60, 180, ILI9341_RED);
    tft.fillTriangle(220, 160, 180, 140, 180, 180, ILI9341_RED);
    tft.fillTriangle(100, 220, 140, 220, 120, 260, ILI9341_RED);
}

void SOAnjos::updateButtonPreselect(){
    switch(screen){
        case 0:
            uint16_t x = (tft.width() - BUTTON_W) / 8;
            uint16_t y = tft.height() / 3 - BUTTON_H + 30;
            switch(option){
                case 0: /// --------------- NENHUM ----------------
                    tft.fillRoundRect(xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("PROGRAMA", ILI9341_YELLOW,xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("MOTOR", ILI9341_YELLOW,xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("APP", ILI9341_YELLOW,xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("SENSOR", ILI9341_YELLOW,xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(20,250, 200, 55, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(20,250, 200, 55, 20, 0x0A0A);
                    printCenteredText("CONFIG", ILI9341_YELLOW,20,250, 200, 55, 1);
                    break;
                case 1: /// --------------- PROGRAMA ----------------
                    tft.fillRoundRect(xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H, 20, ILI9341_YELLOW);
                    tft.drawRoundRect(xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("PROGRAMA", TFT_BACKGROUND,xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H,1); 

                    tft.fillRoundRect(xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("MOTOR", ILI9341_YELLOW,xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("APP", ILI9341_YELLOW,xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("SENSOR", ILI9341_YELLOW,xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(20,250, 200, 55, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(20,250, 200, 55, 20, 0x0A0A);
                    printCenteredText("CONFIG", ILI9341_YELLOW,20,250, 200, 55, 1);
                    break;
                case 2:/// --------------- MOTOR ----------------
                    tft.fillRoundRect(xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("PROGRAMA", ILI9341_YELLOW,xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H, 20, ILI9341_YELLOW);
                    tft.drawRoundRect(xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("MOTOR", TFT_BACKGROUND,xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("APP", ILI9341_YELLOW,xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("SENSOR", ILI9341_YELLOW,xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(20,250, 200, 55, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(20,250, 200, 55, 20, 0x0A0A);
                    printCenteredText("CONFIG", ILI9341_YELLOW,20,250, 200, 55, 1);
                    break;
                case 3:/// --------------- APP ----------------
                    tft.fillRoundRect(xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("PROGRAMA", ILI9341_YELLOW,xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("MOTOR", ILI9341_YELLOW,xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H, 20, ILI9341_YELLOW);
                    tft.drawRoundRect(xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("APP", TFT_BACKGROUND,xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("SENSOR", ILI9341_YELLOW,xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(20,250, 200, 55, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(20,250, 200, 55, 20, 0x0A0A);
                    printCenteredText("CONFIG", ILI9341_YELLOW,20,250, 200, 55, 1);
                    break;
                case 4: /// --------------- SENSOR ----------------
                    tft.fillRoundRect(xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("PROGRAMA", ILI9341_YELLOW,xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("MOTOR", ILI9341_YELLOW,xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("APP", ILI9341_YELLOW,xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H, 20, ILI9341_YELLOW);
                    tft.drawRoundRect(xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("SENSOR", TFT_BACKGROUND,xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(20,250, 200, 55, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(20,250, 200, 55, 20, 0x0A0A);
                    printCenteredText("CONFIG", ILI9341_YELLOW,20,250, 200, 55, 1);
                    break;
                case 5: /// --------------- CONFIG ----------------
                    tft.fillRoundRect(xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("PROGRAMA", ILI9341_YELLOW,xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("MOTOR", ILI9341_YELLOW,xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("APP", ILI9341_YELLOW,xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("SENSOR", ILI9341_YELLOW,xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H,1);

                    tft.fillRoundRect(20,250,200,55, 20, ILI9341_YELLOW);
                    tft.drawRoundRect(20,250,200,55, 20, 0x0A0A);
                    printCenteredText("CONFIG", TFT_BACKGROUND,20,250,200,55,1);
                    break;
            }
            break;
   }
}

void SOAnjos::initSetup(){  //PUBLIC
    Serial.begin(9600);

    // pinMode(A1, OUTPUT); // pino do RFID como saida digital
    // pinMode(A2, OUTPUT);
    // digitalWrite(A1, 0x01); // pino do RFID para Alta para elevar CS do chip
    // digitalWrite(A2, 0x01); // pino do RFID para Alta para elevar CS do chip

    bounceP1.attach(A3, INPUT);
    bounceP1.interval(40);

    bounceP2.attach(A4, INPUT);
    bounceP2.interval(40);

    bounceP3.attach(A5, INPUT);
    bounceP3.interval(40);

    bounceP4.attach(A6, INPUT);
    bounceP4.interval(40);

    bounceP5.attach(A15, INPUT);
    bounceP5.interval(40);

    bounceP6.attach(A12, INPUT);
    bounceP6.interval(40);

    bounceP7.attach(A13, INPUT);
    bounceP7.interval(40);

    bounceP8.attach(A14, INPUT);
    bounceP8.interval(40);

    bounceM1.attach(A8, INPUT);
    bounceM1.interval(40);

    bounceM2.attach(A9, INPUT);
    bounceM2.interval(40);

    bounceM3.attach(A10, INPUT);
    bounceM3.interval(40);

    bounceM4.attach(A11, INPUT);
    bounceM4.interval(40);

    Wire.begin();
    delay(200);

    tft.begin();
    tft.setRotation(2);
    tft.fillScreen(ILI9341_BLACK);
    ts.begin();
    ts.setRotation(2);
    while (!Serial && (millis() <= 1000));
    pageMenu();
    enterButton.attachClick(handleEnterButtonPress);       // Simples Click no botão enter chama a função 1
    backButton.attachClick(handleBackButtonPress);         // Simples Click no botão enter chama a função 1
    plusButton.attachClick(handlePlusButtonPress);                // Simples Click no botão enter chama a função 1
    lessButton.attachClick(handleLessButtonPress);              // Simples Click no botão enter chama a função 1
}

void SOAnjos::updateLoop(){  //PUBLIC
    boolean istouched = ts.touched();
    
    int time = buzzerActivate == 1 ? 120 : 0;
    if (istouched) {
        TS_Point p = ts.getPoint();
        p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
        p.y = map(p.y, TS_MAXY, TS_MINY, 0, tft.height());
        switch (screen){
            case 0:
                if((p.x > xCordinatesButton[0] && p.x < xCordinatesButton[0] + BUTTON_W) && (p.y > yCordinatesButton[0] && p.y < yCordinatesButton[0] + BUTTON_H)){
                    NewTone(26,659,time);
                    option = 1;
                    updateButtonPreselect();
                    delay(200);
                    tft.fillRoundRect(xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("PROGRAMA", ILI9341_YELLOW,xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H,1);
                    screen = option;
                    startProgram = 1;
                    pageProgram();
                }else if((p.x > xCordinatesButton[1] && p.x < xCordinatesButton[1] + BUTTON_W) && (p.y > yCordinatesButton[1] && p.y < yCordinatesButton[1] + BUTTON_H)){
                    NewTone(26,659,time);
                    option = 2;
                    updateButtonPreselect();
                    delay(200);
                    tft.fillRoundRect(xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("MOTOR", ILI9341_YELLOW,xCordinatesButton[1],yCordinatesButton[1], BUTTON_W, BUTTON_H,1);
                    screen = option;
                    pageMotor();
                }else if((p.x > xCordinatesButton[2] && p.x < xCordinatesButton[2] + BUTTON_W) && (p.y > yCordinatesButton[2] && p.y < yCordinatesButton[2] + BUTTON_H)){
                    NewTone(26,659,time);
                    option = 4;
                    updateButtonPreselect();
                    delay(200);
                    tft.fillRoundRect(xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("SENSOR", ILI9341_YELLOW,xCordinatesButton[2],yCordinatesButton[2], BUTTON_W, BUTTON_H,1);
                    screen = option;
                    pageSensor();
                }else if((p.x > xCordinatesButton[3] && p.x < xCordinatesButton[3] + BUTTON_W) && (p.y > yCordinatesButton[3] && p.y < yCordinatesButton[3] + BUTTON_H)){
                    NewTone(26,659,time);
                    option = 3;
                    updateButtonPreselect();
                    delay(200);
                    tft.fillRoundRect(xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    printCenteredText("APP", ILI9341_YELLOW,xCordinatesButton[3],yCordinatesButton[3], BUTTON_W, BUTTON_H,1);
                    screen = option;
                    pageApp();
                }else if((p.x > xCordinatesButton[4] && p.x < xCordinatesButton[4] + 200) && (p.y > yCordinatesButton[4] && p.y < yCordinatesButton[4] + 55)){
                    NewTone(26,659,time);
                    option = 5;
                    updateButtonPreselect();
                    delay(200);
                    tft.fillRoundRect(20,250, 200, 55, 20, TFT_BACKGROUND);
                    tft.drawRoundRect(20,250, 200, 55, 20, 0x0A0A);
                    printCenteredText("CONFIG", ILI9341_YELLOW,20,250, 200, 55, 1);
                    screen = option;
                    pageConfig();
                }
                break;
            case 2:
                uint16_t w, h, x, y;
                w = tft.width() - 20;
                h = (tft.height() - 75)/4 +10;
                x = 15;
                y = 3 + ((tft.height() -90)/4);
                if(p.x >= x && p.x <= tft.width() - x && p.y >= y-10 && p.y <= y+25){
                    sliderValueM1 = map(p.x, x, tft.width() - x, 0, 100);
                    drawSlider(x, y, tft.width() - 30, 15, sliderValueM1);
                    printText("Encoder:", ILI9341_WHITE, x + 120, y + 27, 1);
                    tft.fillRect(x+175, y+27, 30, 8, TFT_BACKGROUND);
                    printText(String(getCodePin(m1)), ILI9341_WHITE, x + 175, y + 27, 1);
                    sliderValueM1 = map(sliderValueM1, 0,100, -100, 100);
                    setMotorPin(M1, sliderValueM1);
                }
                y = y + 2 + h;
                if(p.x >= x && p.x <= tft.width() - x && p.y >= y-10 && p.y <= y+25){
                    sliderValueM2 = map(p.x, x, tft.width()-x, 0, 100);
                    drawSlider(x, y, tft.width() - 30, 15, sliderValueM2);
                    printText("Encoder:", ILI9341_WHITE, x + 120, y + 27, 1);
                    tft.fillRect(x+175, y+27, 30, 8, TFT_BACKGROUND);
                    printText(String(getCodePin(m2)), ILI9341_WHITE, x + 175, y + 27, 1);
                    sliderValueM2 = map(sliderValueM2, 0,100, -100, 100);
                    setMotorPin(M2, sliderValueM2);
                }
                y = y + 2 + h;
                if(p.x >= x && p.x <= tft.width() -x && p.y >= y-10 && p.y <= y+25){
                    sliderValueM3 = map(p.x, x, tft.width()-x, 0, 100);
                    drawSlider(x, y, tft.width() - 30, 15, sliderValueM3);
                    printText("Encoder:", ILI9341_WHITE, x + 120, y + 27, 1);
                    tft.fillRect(x+175, y+27, 30, 8, TFT_BACKGROUND);
                    printText(String(getCodePin(m2)), ILI9341_WHITE, x + 175, y + 27, 1);
                    sliderValueM3 = map(sliderValueM3, 0,100, -100, 100);
                    setMotorPin(M3, sliderValueM3);
                }
                y = y + 2 + h;
                if(p.x >= x && p.x <= tft.width() -x && p.y >= y-10 && p.y <= y+25){
                    sliderValueM4 = map(p.x, x, tft.width()-x, 0, 100);
                    drawSlider(x, y, tft.width() - 30, 15, sliderValueM4);
                    printText("Encoder:", ILI9341_WHITE, x + 120, y + 27, 1);
                    tft.fillRect(x+175, y+27, 30, 8, TFT_BACKGROUND);
                    printText(String(getCodePin(m2)), ILI9341_WHITE, x + 175, y + 27, 1);
                    sliderValueM4 = map(sliderValueM4, 0,100, -100, 100);
                    setMotorPin(M4, sliderValueM4);
                }
                break;
            case 4:
                
                break;
            case 5:
                if((p.x > xCordinatesButton[0] && p.x < xCordinatesButton[0] + BUTTON_W) && (p.y > yCordinatesButton[0] && p.y < yCordinatesButton[0] + BUTTON_H)){
                    NewTone(26,659,time);
                    buzzerActivate = buzzerActivate == 0 ? 1 : 0;
                    tft.fillRoundRect(xCordinatesButton[0], yCordinatesButton[0], BUTTON_W, BUTTON_H, 20, buzzerActivate == 1 ? ILI9341_GREEN : ILI9341_RED);
                    tft.drawRoundRect(xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H, 20, 0x0A0A);
                    tft.drawRoundRect(xCordinatesButton[0]-1,yCordinatesButton[0]-1, BUTTON_W+2, BUTTON_H+2, 20, 0x0A0A);
                    printCenteredText("SOM", buzzerActivate == 1 ? TFT_BACKGROUND : ILI9341_WHITE ,xCordinatesButton[0],yCordinatesButton[0], BUTTON_W, BUTTON_H,1);
                    delay(100);
                }

                
                break;

            default:
                break;
        }
    }
    if(screen == 2){
        pageMotorUpdateValues();
    }
    if(screen == 4){
        pageSensorUpdateValues();
    }
    enterButton.tick(); // Habilita evento do botão Enter
    backButton.tick();
    plusButton.tick();
    lessButton.tick();
}