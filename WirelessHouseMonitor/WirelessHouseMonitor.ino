char devise[2] = {"0"}; //Devise no
# define SLEEP 10 // leanth of time to sleep in minits

#include "DHT.h"
#include <VirtualWire.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

DHT dht;
char reading[12];
volatile int wakeCount = 0;

void setup()
{
  dht.setup(0); // data pin 0
  
  MCUSR &= ~(1<<WDRF);
  WDTCR |= (1<<WDCE) | (1<<WDE);
  WDTCR = 1<<WDP0 | 1<<WDP3;
  WDTCR |= _BV(WDIE);
  
  vw_set_ptt_inverted(true);  // Required by the RF module
  vw_setup(2000);            // bps connection speed
  vw_set_tx_pin(1);         // Arduino pin to connect the receiver data pin


  //send startup
  char sensor[3] = {"!#"};
  strcat(reading, sensor);
  strcat(reading, devise);
  vw_send((uint8_t *)reading, strlen(reading));
  vw_wait_tx();        // We wait to finish sending the message
  delay(200);         // We wait to send the message again     

  pinMode(2, INPUT);
}
 
void loop(){

  char sensor[4] = {"_#_"};
  
  if(wakeCount == (SLEEP*60)/8) {
  //if(wakeCount == 2) { //For testing only

    float humid = dht.getHumidity();
    dtostrf(humid, 3, 1, reading);
    sensor[1] = 'H';
    strcat(reading, sensor);
    strcat(reading, devise);
    //Message to send:
    //const char *msg = reading;
    vw_send((uint8_t *)reading, strlen(reading));
    
    vw_wait_tx();        // We wait to finish sending the message
    delay(200);         // We wait to send the message again      

    
    
    float temp = dht.getTemperature();
    dtostrf(temp, 3, 1, reading);
    sensor[1] = 'T';
    strcat(reading, sensor);
    strcat(reading, devise);
    //Message to send:
    //const char *msg = reading;
    vw_send((uint8_t *)reading, strlen(reading));
    
    vw_wait_tx();        // We wait to finish sending the message
    delay(200);         // We wait to send the message again     
    

    float light = (analogRead(1)/1024.0)*100;
    dtostrf(light, 3, 1, reading);
    sensor[1] = 'L';
    strcat(reading, sensor);
    strcat(reading, devise);

    //Message to send:
    //const char *msg = reading;
    vw_send((uint8_t *)reading, strlen(reading));
    
    vw_wait_tx();        // We wait to finish sending the message
    delay(200);         // We wait to send the message again  
    
    
    wakeCount = 0;
  } 
  
  goToSleep();         
}

ISR(WDT_vect) {
  wakeCount = wakeCount + 1;
}

void goToSleep(void) {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  
  sleep_disable();
  
  power_all_enable();
}


