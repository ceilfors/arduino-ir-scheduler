/*
 * Turns on Dyson Heater on a certain time
 */

#include <IRremote.h>
#include <DS3231.h>
#include <Wire.h>

DS3231 Clock;
IRsend irsend;

bool h12=false;
bool PM=false;
bool turnedOn = false;
bool debugAlwaysSend = false;

// On the Zero and others we switch explicitly to SerialUSB
#if defined(ARDUINO_ARCH_SAMD)
#define Serial SerialUSB
#endif

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Wire.begin();
  Serial.begin(9600);
  Clock.setClockMode(false);    // set to 24h

  // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ " from " __DATE__));
  Serial.print(F("Ready to send IR signals at pin "));
  Serial.println(IR_SEND_PIN);

  // Uncomment to set real clock, comment back and upload once set!
  // Clock.setHour(9);
  // Clock.setMinute(10);
  // Clock.setSecond(0);
  
  // Uncomment to set test clock
  // Clock.setHour(5);
  // Clock.setMinute(29);
  // Clock.setSecond(50);
}

void loop() {
  byte hour = Clock.getHour(h12, PM);
  byte minute = Clock.getMinute();
  byte second = Clock.getSecond();
  printTime(hour, minute, second);
  if (debugAlwaysSend || (!turnedOn && hour == 5 && minute == 30)) {
    sendDysonSignal();
    turnedOn = true;
  } else if (hour == 4) {
    turnedOn = false;
  }
  delay(10000);
}

void sendDysonSignal() {
  Serial.println("Sending Dyson IR signal");
  int khz = 38; // 38kHz carrier frequency for the NEC protocol
  unsigned int DYSON_POWER_SIGNAL[33] = {2200,750, 700,750, 700,800, 700,1400, 700,1400, 700,750, 700,750, 700,700, 700,800, 700,750, 750,750, 700,750, 700,750, 700,750, 650,750, 700,750, 700};  // UNKNOWN 4C73CC7
  irsend.sendRaw(DYSON_POWER_SIGNAL, 33, khz);
}

void printTime(byte hour, byte minute, byte second) {
  Serial.print("Current time: ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);
}
