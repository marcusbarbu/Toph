#define MAX_INTESITY 255
#define MED_INTENSITY 122
#define BASE_INTENSITY 40

const bool DEBUG = false;

volatile bool signalStart = false;
volatile bool timeReady = false;
volatile uint16_t roundTrip = 0;

//portMasks: 0x04 : PortD
//           0x02 : PortC
//           0x01 : PortB

const uint8_t customMOSI = 0x08;//pb3
const uint8_t customCLK = 0x10;//pb4

struct Module{
    uint8_t sensorEchoMask;
    uint8_t sensorTriggerMask;
    uint8_t sensorPortMask;
    
    uint8_t csPinMask;
    uint8_t csPortMask;  //for now must be 0x01

    uint16_t farThreshold;
    uint16_t medThreshold;
};

const int numModules = 3;
int currentModuleIndex = 0;

Module modules[numModules];

void initTimer();
void startTimer();
void stopTimer();

void initModule(Module mod);
    void initSensor(uint8_t trigMask, uint8_t echoMask, uint8_t portMask);
    void initBuzzer(uint8_t pinMask, uint8_t portMask);

int getTimeToTarget(int moduleIndex);
    void triggerModule(Module mod);

void setBuzzerStrength(int module, uint8_t power);
    uint8_t getOutputStrength(uint16_t rawTime);
    void sendWithSPIOnPortB(uint16_t value, uint8_t csLoc, uint8_t mosiLoc, uint8_t sclkLoc, int interval, bool final);

void setup() {
    if(DEBUG){
        Serial.begin(9600);
    }
    //module 0: echo PD2, trigger PD3, cs PB2
    modules[0] = {.sensorEchoMask = 0x04, .sensorTriggerMask = 0x08, .sensorPortMask = 0x04, .csPinMask = 0x04, .csPortMask = 0x01, .farThreshold=25600, .medThreshold=12800};
    //module 1: echo PD4, trigger PD5, cs PB5
    modules[1] = {.sensorEchoMask = 0x10, .sensorTriggerMask = 0x20, .sensorPortMask = 0x04, .csPinMask = 0x20, .csPortMask = 0x01, .farThreshold=25600, .medThreshold=12800};
    //module 2: echo PD6, trigger PD7, cs PB0
    modules[2] = {.sensorEchoMask = 0x40, .sensorTriggerMask = 0x80, .sensorPortMask = 0x04, .csPinMask = 0x01, .csPortMask = 0x01, .farThreshold=25600, .medThreshold=12800};
    
    for (int i=0; i < numModules; i++){
        initModule(modules[i]);
    }
    initTimer();
    DDRB |= customMOSI;
    DDRB |= customCLK;
}

void loop() {
    while(currentModuleIndex < numModules){
        if (DEBUG){
            Serial.print("attempting module");
            Serial.println(currentModuleIndex);
        }
        uint16_t time = getTimeToTarget(currentModuleIndex);
        if (DEBUG){
            Serial.print("Time: ");
            Serial.println(time);
        } 
        setBuzzerStrength(currentModuleIndex, time);
        currentModuleIndex++;
    }
    currentModuleIndex = 0;
    delay(20);
}

ISR(PCINT2_vect){
    cli();
    Module mod = modules[currentModuleIndex];
    if ((0x04 & mod.sensorPortMask) == mod.sensorPortMask){
        if((PIND & mod.sensorEchoMask) == mod.sensorEchoMask){
            startTimer();
            signalStart = true;
        }
        else{
            if (signalStart){
                stopTimer();
                roundTrip = TCNT1; //TimerCount1
                timeReady = true;
            }
        }
    }
    sei();
}

ISR(PCINT1_vect){
    cli();
    if (DEBUG){
        Serial.println("PCINT1");
    }
    Module mod = modules[currentModuleIndex];
    if (0x02 & mod.sensorPortMask){
        if((PINC & mod.sensorEchoMask) == mod.sensorEchoMask){
            startTimer();
            signalStart = true;
        }
        else{
            if (signalStart){
                stopTimer();
                roundTrip = TCNT1; //TimerCount1
                timeReady = true;
            }
        }
    }
    sei();
}

ISR(PCINT0_vect){
    cli();
    if (DEBUG){
        Serial.println("PCINT0");
    }
    Module mod = modules[currentModuleIndex];
    if (0x01 & mod.sensorPortMask){
        if((PIND & mod.sensorEchoMask) == mod.sensorEchoMask){
            startTimer();
            signalStart = true;
        }
        else{
            if (signalStart){
                stopTimer();
                roundTrip = TCNT1; //TimerCount1
                timeReady = true;
            }
        }
    }
    sei();
}

void sendWithSPIOnPortB(uint16_t value, uint8_t csLoc, uint8_t mosiLoc, uint8_t sclkLoc, int interval, bool final){
    Serial.println(value, HEX);
    Serial.println(csLoc, HEX);
    Serial.println(mosiLoc, HEX);
    Serial.println(sclkLoc, HEX);
    PORTB &= ~(mosiLoc);
    PORTB &= ~(sclkLoc);
    delayMicroseconds(interval);
    PORTB &= ~(csLoc);
    delayMicroseconds(interval);
    //MSB first, send a full 2Byte word at once 
    for (size_t i = 0; i < 16; i++){
        if (value & (0x8000 >> i)) {
            PORTB |= mosiLoc; 
        }
        else {
            PORTB &= ~(mosiLoc);
        }
        delayMicroseconds(interval/3); //wait so that the change and the clock don't occur at the same time
        PORTB |= sclkLoc;
        delayMicroseconds(interval);
        PORTB &= ~(sclkLoc);
        delayMicroseconds(interval);

    }
    if (final) {PORTB |= csLoc;} //if it's a standalone transmission, pull the chip select back up
}

void setBuzzerStrength(int module, uint16_t rawTime){
    if (DEBUG){
        Serial.print("setting buzzer strength for module ");
        Serial.println(module); 
    }
    uint16_t payload = 0x0000;
    uint16_t controlMask = 0x3000; //write to DAC, Unbuffered, 1x Gain, Active Mode
    uint8_t val = getOutputStrength(rawTime); 
    payload |= val;
    payload = payload << 4;
    payload |= controlMask;
    cli();
    sendWithSPIOnPortB(payload, modules[module].csPinMask, customMOSI, customCLK, 16, true);
    sei();
}

uint8_t getOutputStrength(uint16_t rawTime){
    uint16_t bottom = modules[currentModuleIndex].medThreshold;
    uint16_t top = modules[currentModuleIndex].farThreshold; 
    
    if (rawTime <= bottom){
        return MAX_INTESITY;
    }
    else if (rawTime <= top){
        return MED_INTENSITY;
    }
    else {
        return BASE_INTENSITY;
    }
}

int getTimeToTarget(int moduleIndex){
    signalStart = false;
    triggerModule(modules[moduleIndex]);
    while(!timeReady && (TCNT1 & 0xFFFF)!= 0xFFFF);
    Serial.println(roundTrip, HEX);
    timeReady = false;
    return roundTrip;
}

void triggerModule(Module mod){
    if (DEBUG){
        Serial.print("triggering Module ");
        Serial.println(mod.sensorTriggerMask);
    }
    if (mod.sensorPortMask & 0x01){
        PORTB &= ~(mod.sensorTriggerMask); //pull the trigger low
        delayMicroseconds(2);
        PORTB |= mod.sensorTriggerMask; //start the trigger pulse
        delayMicroseconds(10);
        PORTB &= ~(mod.sensorTriggerMask); //end the trigger pulse
    }
    else if (mod.sensorPortMask & 0x02){
        PORTC &= ~(mod.sensorTriggerMask); //pull the trigger low
        delayMicroseconds(2);
        PORTC |= mod.sensorTriggerMask; //start the trigger pulse
        delayMicroseconds(10);
        PORTC &= ~(mod.sensorTriggerMask); //end the trigger pulse
    }
    else if (mod.sensorPortMask & 0x04){
        PORTD &= ~(mod.sensorTriggerMask); //pull the trigger low
        delayMicroseconds(2);
        PORTD |= mod.sensorTriggerMask; //start the trigger pulse
        delayMicroseconds(10);
        PORTD &= ~(mod.sensorTriggerMask); //end the trigger pulse
    }
    
}

void initModule(Module mod){
    initSensor(mod.sensorTriggerMask, mod.sensorEchoMask, mod.sensorPortMask);
    initBuzzer(mod.csPinMask, mod.csPortMask);
    if (DEBUG){
        Serial.println("initialized module successfully");
        
    }
}

void initTimer(){
    //TCCR1A [COM1A1 | COM1A0 | COM1B1 | COM1B0 | ---- | ---- | WGM11 | WGM10 ]
    TCCR1A = 0x00; //use it as a normal timer
    //TCCR1B [ ICNC1 | ICES1 | ---- | WGM13 | WGM12 | CS12 | CS11 | CS10 ]
    TCCR1B = 0x00; //use it as a normal timer but it's not started
}

void initSensor(uint8_t trigMask, uint8_t echoMask, uint8_t portMask){
    PCICR |= portMask; //turn on the pin change interrupt for the port

    if (portMask & 0x01){
        if (DEBUG){
            Serial.println("Adding interrupt to PCMSK0");
        }
        PCMSK0 |= echoMask; //allow interrupt on the echo pin
        DDRB |= trigMask; //set trigger pin to output 
        DDRB &= ~(echoMask); //set echo pin to input
    }
    else if (portMask & 0x02){
        if (DEBUG){
            Serial.println("Adding interrupt to PCMSK1");
        }
        PCMSK1 |= echoMask;
        DDRC |= trigMask; 
        DDRC &= ~(echoMask);
    }
    else if (portMask & 0x04){
        if (DEBUG){
            Serial.println("Adding interrupt to PCMSK2");
        }
        PCMSK2 |= echoMask;
        DDRD |= trigMask; 
        DDRD &= ~(echoMask);
    }
}

void initBuzzer(uint8_t pinMask, uint8_t portMask){
    if (portMask & 0x01){
        DDRB |= pinMask; //set chip select as output
        PORTB |= pinMask; //pull chip select high
    }
    else if (portMask & 0x02){
        DDRC |= pinMask;
        PORTC |= pinMask;
    }
    else if (portMask & 0x04){
        DDRD |= pinMask;
        PORTD |= pinMask;
    }
}

void startTimer(){
    TCNT1 = 0x0000;
    TCCR1B |= 0x01;
}

void stopTimer(){
    TCCR1B &= ~(0x01);
}
