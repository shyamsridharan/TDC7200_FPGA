      // Connections:
//
// TDC7200      Pro-Mini
//  1 Enable      4
//  2 Trigg       -
//  3 Start       3
//  4 Stop        5
//  5 Clock       8            8Mhz clock output configured in Pro Mini Fuse settings
//  6 Nc          -
//  7 Gnd         Gnd
//  8 Intb        2
//  9 Dout        12
// 10 Din         11
// 11 Csb         10
// 12 Sclk        13
// 13 Vreg        -           Via 1uF to Gnd
// 14 Vdd         Vcc 3.3V        
#include <avr/power.h>
#include "TDC7200.h"
#include "Streaming.h"

#define PIN_TDC7200_INT       (2)
#define PIN_TDC7200_ENABLE    (4)
#define PIN_TDC7200_STOP      (5)
#define PIN_TDC7200_START     (6)

#define PIN_TDC7200_SPI_CS    (10)
#define TDC7200_CLOCK_FREQ_HZ (8000000)

static TDC7200 tof(PIN_TDC7200_ENABLE, PIN_TDC7200_SPI_CS, TDC7200_CLOCK_FREQ_HZ);

#define NUM_STOPS (1)

void setup()
{

  #if F_CPU == 8000000
  //Serial.println(F_CPU);
  delay(1000);
  clock_prescale_set(clock_div_2);
  #endif
  
    Serial.begin(115200);
    //Serial.println(F("-- Starting TDC7200 test --"));
    while (not tof.begin())
    {
        Serial.println(F("Failed to init TDC7200"));
        delay(1000);
    }

    pinMode(PIN_TDC7200_INT, INPUT_PULLUP);     // active low (open drain)

    digitalWrite(PIN_TDC7200_START, LOW);
    pinMode(PIN_TDC7200_START, OUTPUT);

    digitalWrite(PIN_TDC7200_STOP, LOW);
    pinMode(PIN_TDC7200_STOP, OUTPUT);

    if (not tof.setupMeasurement( 10,         // cal2Periods
                                  1,          // avgCycles
                                  NUM_STOPS,  // numStops
                                  1))        // mode
    {
        Serial.println(F("Failed to setup measurement"));
        while (1);
    }
}

//64bit unsigned int to char array conversion
static void ui64toa(uint64_t v, char * buf, uint8_t base)
{ 
  int idx = 0;
  uint64_t w = 0;
  while (v > 0)
  {
    w = v / base;
    buf[idx++] = (v - w * base) + '0';
    v = w;
  }
  buf[idx] = 0;
  // reverse char array
  for (int i = 0, j = idx - 1; i < idx / 2; i++, j--)
  {
    char c = buf[i];
    buf[i] = buf[j];
    buf[j] = c;
  }
}

void loop()
{
  static uint16_t pulseUs =0;
  uint8_t stop = 1;
  int iter;
  unsigned long x; 
  int timeint=30000;      
  while(Serial.available()<0); //check for command from python serial
  int temp=Serial.parseInt();
  if(temp==8){ 
  uint64_t time_1,time_2;  
  tof.startMeasurement();     
  while (digitalRead(PIN_TDC7200_INT) == HIGH); //wait for interrupt to go high
        if (tof.readMeasurement(stop,time_1))
        { 
          char buff[40];            
          ui64toa(time_1, buff, 10);          
          Serial<<buff<<","<<endl;            //output the first value in serial buffer as a 64 bit unsigned int
              
        } 
  x=millis();  
  while((millis()- x)<timeint){    
  tof.startMeasurement();
  while (digitalRead(PIN_TDC7200_INT) == HIGH);
  if (tof.readMeasurement(stop,time_2))
       {
           int32_t dif=time_1-time_2;        //output remaining values after subtracting from the first value. The difference is a 32 bit signed int. 
           Serial<<dif<<","<<endl;           
        } 
            
    } 
  Serial.print('@'); 
  Serial.println();
  }
 
}
