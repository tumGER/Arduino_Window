// EUPL v1.2+, git@tomger.eu

#include <SimpleDHT.h>
#include <Stepper.h>

// for DHT11, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT11 = 2;
bool isClosed = true;
const int SPU = 2048;
const int FvDg = SPU/360*45; // 45°

SimpleDHT11 dht11;
Stepper Motor(SPU, 8, 9, 10, 11); // Init motor

void setup() {
  Serial.begin(9600);

  Motor.setSpeed(5); // 5 RPM
}

void loop() {
 
  // start working...
  // Serial.println("=================================");
  // Serial.println("Sample DHT11...");
  
  // read with raw sample data.
  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  if (dht11.read(pinDHT11, &temperature, &humidity, data)) {
    // Serial.print("Read DHT11 failed");
    return;
  }
  
  // Serial.print("Sample RAW Bits: ");
  for (int i = 0; i < 40; i++) {
    // Serial.print((int)data[i]);
    if (i > 0 && ((i + 1) % 4) == 0) {
      // Serial.print(' ');
    }
  }
  // Serial.println("");
  
  if ((temperature >= 25 || humidity <= 44) && !isClosed) {
    Serial.print("It's too hot, starting motor! \n)");

    Motor.step(FvDg);
    delay(1000);

    isClosed = true;
  } else if ((temperature <= 24 || humidity >= 43) && isClosed) {
    Serial.print("It's too cold! \n");

    Motor.step(-1*FvDg);
    delay(1000);

    isClosed = false;
  }    
  // Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, \n");
  Serial.print((int)humidity); Serial.println(" %");
  
  // DHT11 sampling rate is 1HZ.
  delay(1000);
}