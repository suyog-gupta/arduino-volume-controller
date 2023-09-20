// REFER: https://gitlab.com/-/snippets/1690600

#include <IRremote.h>
#define transmitter_pin 3

int IRpin = 11;
IRrecv irrecv(IRpin);

int soundSensor = A0;
int lowCounter = 0;
int volDownOpCounter = 0;
double aggregateVol = 0;
double avgVolume = 0;
int loopCounter = 0;
int spikeCounter = 0;
bool controllerEnabled = true;

void setup() {
  pinMode(soundSensor, INPUT);
  IrSender.begin(transmitter_pin);
  irrecv.enableIRIn();  // Start the receiver
  IrReceiver.begin(IRpin, ENABLE_LED_FEEDBACK);
  Serial.begin(9600);
}

void loop() {
  loopCounter++;
  aggregateVol = aggregateVol + analogRead(soundSensor);

  if (IrReceiver.decode()) {

    String irNum = String(IrReceiver.decodedIRData.command, HEX);
    Serial.print("  Received IR Signal : ");
    Serial.println(irNum);

    if (irNum.equals("10")) {
      Serial.print("  Received signal to disable the controller: ");
      Serial.println(irNum);
      controllerEnabled = false;
    }
    if (irNum.equals("11")) {
      Serial.print("  Received signal to enable the controller: ");
      Serial.println(irNum);
      controllerEnabled = true;
    }
    IrReceiver.resume();
  }


// if (loopCounter > 90000) {
//   spikeCounter = 0;
//   lowCounter = 0;
// }

if (loopCounter > 5000) {
  avgVolume = aggregateVol / 5;

  if (avgVolume > 70000) {
    Serial.print("  Avg Volume reading higher side: ");
    Serial.println(avgVolume);
    spikeCounter++;
  }

  if (avgVolume > 18000 && avgVolume < 32000) {
    lowCounter++;
    //Serial.print("  Avg Volume reading: ");
    //Serial.println(avgVolume);
    //Serial.print("  Low Counter: ");
    //Serial.println(lowCounter);
  }

  loopCounter = 0;
  aggregateVol = 0;
}

if (spikeCounter > 3 && controllerEnabled) {
  volDownOpCounter++;
  Serial.print("  Lowering down the volume.. Low Counter: ");
  Serial.print(lowCounter);
  Serial.println();
  spikeCounter = 0;
  IrSender.sendLG(551534655, 32);
}

if (lowCounter > 50 && controllerEnabled) {
  Serial.print("  Bumping up the volume.. Low Counter: ");
  Serial.print(lowCounter);
  Serial.print("  Loop Counter: ");
  Serial.print(loopCounter);
  IrSender.sendLG(551502015, 32);
  lowCounter = 0;
  Serial.println();
}
}
