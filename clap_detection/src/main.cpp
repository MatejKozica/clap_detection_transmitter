#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10

const byte address[6] = {"1node"};

RF24 radio(CE_PIN, CSN_PIN);

int dataToSend;
int SoundSensor = A4; // LM393 Sound Sensor Digital Pin D0 connected to analog pin 4
unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 1000;

void send();
void setup()
{
  Serial.begin(9600); // initialize serial
  Serial.println(F("SimpleTx Starting"));
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(111);
  radio.setPALevel(RF24_PA_MAX);
  radio.setRetries(3, 5); // delay, count
  radio.openWritingPipe(address);

  randomSeed(analogRead(0));

  pinMode(SoundSensor, INPUT);
}
void loop()
{
  dataToSend = analogRead(SoundSensor);

  if (dataToSend >= 800)
  {
    currentMillis = millis();
    if (currentMillis - prevMillis >= txIntervalMillis)
    {
      send();
      prevMillis = millis();
    }
  }
}

void send()
{

  bool rslt;
  rslt = radio.write(&dataToSend, sizeof(dataToSend));
  // Always use sizeof() as it gives the size as the number of bytes.
  // For example if dataToSend was an int sizeof() would correctly return 2

  Serial.print("Data Sent ");
  Serial.print(dataToSend);

  if (rslt)
  {
    Serial.println("  Acknowledge received");
  }
  else
  {
    Serial.println("  Tx failed");
  }
}