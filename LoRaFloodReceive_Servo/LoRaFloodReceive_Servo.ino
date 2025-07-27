#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h> // Driver Library for the LCD Module

int RED_LED = 8;
int YELLOW_LED = 5;
int GREEN_LED = 4;
int buzzer = 3;
int servoPin1 = A0;
int servoPin2 = A1;

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,16,2); // Adjust to (0x27,20,4) for 20x4 LCD

// LoRaWAN NwkSKey, network session key
// This is the default Semtech key, which is used by the early prototype TTN
// network.
static const u1_t PROGMEM APPEUI[8]={0x67, 0x45, 0x23, 0x91, 0x78, 0x56, 0x34, 0x12};
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}
// LoRaWAN AppSKey, application session key
// This is the default Semtech key, which is used by the early prototype TTN
// network.
static const u1_t PROGMEM DEVEUI[8]= {0x33, 0x44, 0x06, 0xD0, 0x7E, 0xD5, 0xB3, 0x70};
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}

// LoRaWAN end-device address (DevAddr)
static const u1_t PROGMEM APPKEY[16] = {0xDD, 0xAA, 0xA9, 0x12, 0xE5, 0x4E, 0xC6, 0x6F, 0x17, 0x7D, 0xE1, 0xF3, 0xBD, 0x0D, 0x91, 0x44};
void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}

//uint8_t Data;
static uint8_t mydata[2] = {0x00,0x00};
static osjob_t sendjob;

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 30;

void playBuzzer(){
  tone(buzzer,200,500); //tone function syntax: tone(pin, pitch, frequency)
  delay(500);
}

void input()
{
    mydata[1] = 1;
}

// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 9,
    .dio = {2, 6, 7},
};

void receive(){
if (LMIC.txrxFlags & TXRX_ACK)
Serial.println(F("Received ack"));

  if (LMIC.dataLen) {
  Serial.println(F("Received "));
  //for (int i = 0; i < LMIC.dataLen; i += 2) {
      byte distData = LMIC.frame[LMIC.dataBeg];
      byte rainData = LMIC.frame[LMIC.dataBeg + 1];
  //}
  
        Serial.print("DistCM: ");
        Serial.print(distData);
        Serial.println(" cm");
        Serial.print("Rain%: ");
        Serial.print(rainData);
        Serial.println(" cm");

        lcd.clear();
        lcd.setCursor(4,1);
        lcd.print("Rain: ");
        lcd.print(rainData);
        lcd.print("%");

        //Condition when Flood height is beyond 200 cm. 
        if(distData > 200){
          //lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("!Danger!");
          lcd.setCursor(11,0);
          lcd.print(distData);
          lcd.print("Cm");

          digitalWrite(RED_LED,HIGH);
          digitalWrite(YELLOW_LED,LOW);
          digitalWrite(GREEN_LED,LOW);
          digitalWrite(servoPin1, LOW);
          digitalWrite(servoPin2, HIGH);
          playBuzzer();	//Activate the Buzzer
          delay(1000);
        }
        
      //Condition when Flood height sits between 110 cm and 200 cm.
        else if ((distData <= 200) && (distData > 100)){
          //lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("!Risk!");
          lcd.setCursor(11,0);
          lcd.print(distData);
          lcd.print("Cm");
          digitalWrite(RED_LED,LOW);
          digitalWrite(YELLOW_LED,HIGH);
          digitalWrite(GREEN_LED,LOW);
          digitalWrite(servoPin1, HIGH);
          digitalWrite(servoPin2, LOW);
          noTone(buzzer);
          delay(1000);
        }
        
      //Condition for Flood height < 100 cm.
        else if((distData < 100) && (distData > 0)){
          //lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("!Safe!");
          lcd.setCursor(11,0);
          lcd.print(distData);
          lcd.print("Cm");
          digitalWrite(RED_LED,LOW);
          digitalWrite(YELLOW_LED,LOW);
          digitalWrite(GREEN_LED,HIGH);
          digitalWrite(servoPin1, LOW);
          digitalWrite(servoPin2, LOW);
          noTone(buzzer);
          delay(1000);
        }
        else{
          //lcd.clear();
          lcd.setCursor(1,0);
          lcd.print("...Loading!...");
          } 
    }
  }

void onEvent (ev_t ev) {

    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            break;
        case EV_RFU1:
            Serial.println(F("EV_RFU1"));
            break;
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            receive();
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            break;
         default:
            Serial.println(F("Unknown event"));
            break;
    }
}

void do_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) 
    {
        Serial.println(F("OP_TXRXPEND, not sending"));
    }
    else 
    {
        input();
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, mydata, sizeof(mydata), 0);
        Serial.println(F("Packet queued"));
    }
    // Next TX is scheduled after TX_COMPLETE event.
}

void setup() {

    Serial.begin(9600);
    pinMode(RED_LED,OUTPUT);
    pinMode(YELLOW_LED,OUTPUT);
    pinMode(GREEN_LED,OUTPUT);
    pinMode(buzzer,OUTPUT);
    pinMode(servoPin1, OUTPUT);
    pinMode(servoPin2, OUTPUT);

    // Initiate the LCD and turn on the backlight
    lcd.init();          // Initiate the LCD module
    lcd.backlight();     // Turn on the backlight

    Serial.println(F("Starting"));

    #ifdef VCC_ENABLE
    // For Pinoccio Scout boards
    pinMode(VCC_ENABLE, OUTPUT);
    digitalWrite(VCC_ENABLE, HIGH);
    delay(1000);
    #endif

    // LMIC init
    os_init();

    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    // Disable link check validation
    LMIC_setLinkCheckMode(0);

    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;

    // Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
    LMIC_setDrTxpow(DR_SF7,14);

    // Start job
    do_send(&sendjob);
}

void loop() {
    os_runloop_once();
}
