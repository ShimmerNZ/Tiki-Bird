#include <PololuMaestro.h>
#include "Arduino.h"
#include "SoftwareSerial.h"
#include <DFMiniMp3.h>


class Mp3Notify
{
public:
  static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action)
  {
    if (source & DfMp3_PlaySources_Sd) 
    {
        Serial.print("SD Card, ");
    }
    if (source & DfMp3_PlaySources_Usb) 
    {
        Serial.print("USB Disk, ");
    }
    if (source & DfMp3_PlaySources_Flash) 
    {
        Serial.print("Flash, ");
    }
    Serial.println(action);
  }
  static void OnError(uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
  }
  static void OnPlayFinished(DfMp3_PlaySources source, uint16_t track)
  {
    Serial.print("Play finished for #");
    Serial.println(track);  
  }
  static void OnPlaySourceOnline(DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "online");
  }
  static void OnPlaySourceInserted(DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "inserted");
  }
  static void OnPlaySourceRemoved(DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "removed");
  }
};

SoftwareSerial maestroSerial(11, 10);
SoftwareSerial mySoftwareSerial(9, 8); // RX, TX
DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(mySoftwareSerial);
void printDetail(uint8_t type, int value);
MicroMaestro maestro(maestroSerial);
const int MOTION_PIN = 12;
long randNumber;
#define LED 7

void waitMilliseconds(uint16_t msWait)
{
  uint32_t start = millis();
  
  while ((millis() - start) < msWait)
  {
    // calling mp3.loop() periodically allows for notifications 
    // to be handled without interrupts
    mp3.loop(); 
    delay(1);
  }
}

void setup()
{
  maestroSerial.begin(9600);
  mySoftwareSerial.begin(9600);
  randomSeed(analogRead(0));
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  mp3.begin();
  pinMode(MOTION_PIN, INPUT);
  Serial.println();
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  uint16_t volume = mp3.getVolume();
  mp3.setVolume(20);
  Serial.print("volume ");
  Serial.println(volume);
  randomSeed(analogRead(0));
  
  uint16_t count = mp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
  Serial.print("files ");
  Serial.println(count);
  
  Serial.println("starting...");
  digitalWrite(LED, HIGH);
}

void loop()
{
  int proximity = digitalRead(MOTION_PIN);
  if (proximity == HIGH) // If the sensor's output goes low, motion is detected
  {
    
    randNumber = random(5);
    //randNumber = 0;
    Serial.println(randNumber);
    if (randNumber == 0)
    {
      Serial.println(F("Sequence 1"));
      maestro.restartScript(0);
      mp3.setVolume(20);
      delay(200);
      mp3.playMp3FolderTrack(1);
      delay(3000);
    }
    if (randNumber == 1)
    {
      Serial.println(F("Sequence 2 - Tiki Chorus Singing"));
      maestro.restartScript(1);
      mp3.setVolume(25);
      delay(250);
      mp3.playMp3FolderTrack(2);
      delay(3000);
    }   
    if (randNumber == 2)
    {
      Serial.println(F("Sequence 3 - Short no Singing"));
      maestro.restartScript(2);
      mp3.setVolume(25);
      delay(200);
      mp3.playMp3FolderTrack(3);
      delay(3000);
    }
    if (randNumber == 3)
    {
      Serial.println(F("Sequence 4 - Singing Bird Long"));
      maestro.restartScript(3);
      mp3.setVolume(25);
      delay(2800);
      mp3.playMp3FolderTrack(4);
      delay(3000);
    }                                                
    if (randNumber > 3)
    {
      Serial.println(F("Sequence 5 - Michelle You're Fired"));
      mp3.setVolume(28);
      maestro.restartScript(4);
      delay(2050);
      mp3.playMp3FolderTrack(5); 
      delay(3000);
      mp3.setVolume(10);
    }


  }

}                                          
