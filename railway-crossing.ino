#include <Servo.h>
#include "RTClib.h"

Servo myservo;
RTC_PCF8523 rtc;

// Pin Setting //
const int speakerPin = 2;
const int servoPin = 3;
const int ledPin_1 = 7;
const int ledPin_2 = 10;
const int ledPin_In = 9;
const int ledPin_Out = 8;

// Timing Setting //
const int marginSec_In   = 150;
const int marginSec_Out = 60;
const long duration_In = 60000;
const long duration_Out = 90000;
const long delay_Servo = 10000;
const int speed_Servo = 60;
const int speed_Siren = 310;

// Siren Setting //
const int vol = 10;
const int delayTime = 200;

// Initialize //
unsigned long lastMsec; 
unsigned long lastMsec_Servo;
unsigned long lastMsec_Siren;
unsigned long lastMsec_debug; //debug
unsigned long delay_Start;
int last_now_In;
int last_now_Out;
long total_duration;
long arrow_duration_In;
long arrow_duration_Out;
unsigned long lastTime;
int pos;
int flag;
int ledState_1 = LOW;
int ledState_2 = LOW;
int ledState_In = LOW;
int ledState_Out = LOW;

// Time Table //
char* Weekday_Table_In[]={
  "4:59",
  "5:16",  "5:28", "5:36", "5:46", "5:56",
  "6:2", "6:14", "6:20", "6:23", "6:26", "6:30", "6:34", "6:37", "6:40", "6:46", "6:48", "6:52", "6:57",
  "7:0", "7:5", "7:8", "7:14", "7:18", "7:22", "7:24", "7:29", "7:38", "7:44", "7:48", "7:52", "7:54", "7:59",
  "8:4", "8:8", "8:11", "8:19", "8:21", "8:30", "8:34", "8:39", "8:41", "8:49",
  "9:0", "9:5", "9:9", "9:11", "9:21", "9:31", "9:41", "9:51",
  "10:1", "10:11", "10:13", "10:21", "10:31", "10:41", "10:47", "10:50",
  "11:1", "11:11", "11:22", "11:31", "11:41", "11:47", "11:50",
  "12:1", "12:11", "12:22", "12:31", "12:41", "12:47", "12:50",
  "13:1", "13:11", "13:22", "13:31", "13:41", "13:47", "13:50",
  "14:1", "14:11", "14:22", "14:31", "14:41", "14:47", "14:50",
  "15:1", "15:12", "15:22", "15:31", "15:41", "15:47", "15:50",
  "16:0", "16:9", "16:17", "16:20", "16:29", "16:33", "16:38", "16:42", "16:46", "16:50", "16:56",
  "17:1", "17:8", "17:12", "17:16", "17:20", "17:25", "17:35", "17:43", "17:46", "17:53", "17:57",
  "18:1", "18:6", "18:10", "18:13", "18:16", "18:22", "18:25", "18:35", "18:43", "18:46", "18:50", "18:53",
  "19:2", "19:6", "19:13", "19:16", "19:25", "19:29", "19:32", "19:37", "19:44", "19:46", "19:55",
  "20:4", "20:9", "20:13", "20:16", "20:25", "20:29", "20:39", "20:44", "20:46", "20:57",
  "21:7", "21:14", "21:17", "21:29", "21:33", "21:39", "21:45", "21:48", "21:56",
  "22:2", "22:11", "22:14", "22:17", "22:25", "22:37", "22:42", "22:47", "22:55",
  "23:9", "23:16", "23:25"
}; //170

char* Weekday_Table_Out[]={
  "5:18",  "5:47",
  "6:9", "6:23", "6:34", "6:42", "6:53",
  "7:0", "7:11", "7:17", "7:21", "7:31", "7:36", "7:38", "7:42", "7:51",
  "8:1", "8:6", "8:8", "8:12", "8:22", "8:28", "8:33", "8:39", "8:42",
  "9:1", "9:11", "9:19", "9:23", "9:26", "9:30", "9:34", "9:40", "9:43",
  "10:1", "10:6", "10:8", "10:12", "10:16", "10:22", "10:30", "10:32", "10:41",
  "11:1", "11:11", "11:21", "11:30", "11:32", "11:42", "11:52",
  "12:2", "12:10", "12:13", "12:21", "12:31", "12:41", "12:52",
  "13:1", "13:10", "13:12", "13:21", "13:31", "13:41", "13:52",
  "14:0", "14:10", "14:12", "14:21", "14:31", "14:41", "14:52",
  "15:1", "15:10", "15:12", "15:22", "15:31", "15:41", "15:51",
  "16:1", "16:10", "16:12", "16:22", "16:34", "16:42", "16:51",
  "17:0", "17:4", "17:14", "17:26", "17:32", "17:34", "17:38", "17:46", "17:50",
  "18:4", "18:6", "18:11", "18:16", "18:25", "18:33", "18:37", "18:41", "18:46",
  "19:4", "19:7", "19:11", "19:15", "19:25", "19:34", "19:36", "19:41", "19:46",
  "20:4", "20:7", "20:17", "20:21", "20:27", "20:34", "20:38", "20:45", "20:52",
  "21:4", "21:8", "21:15", "21:25", "21:32", "21:35", "21:37", "21:46", "21:49",
  "22:4", "22:6", "22:12", "22:25", "22:29", "22:32", "22:35", "22:38", "22:47",
  "23:2", "23:7", "23:11", "23:17", "23:27", "23:31", "23:36", "23:46", "23:54",
  "0:3", "0:6", "0:15", "0:23", "0:28", "0:39", "0:53" 
}; //155

char* Weekend_Table_In[]={
  "4:59",
  "5:16",  "5:28", "5:36", "5:46", "5:56",
  "6:7", "6:16", "6:24", "6:35", "6:38", "6:45", "6:54", "6:59",
  "7:6", "7:13", "7:16", "7:23", "7:32", "7:38", "7:46", "7:53", "7:58",
  "8:7", "8:12", "8:15", "8:23", "8:29", "8:38", "8:46", "8:52", "8:58",
  "9:7", "9:13", "9:15", "9:22", "9:30", "9:40", "9:49", "9:51",
  "10:1", "10:11", "10:15", "10:20", "10:22", "10:30", "10:41", "10:47", "10:50",
  "11:0", "11:11", "11:21", "11:30", "11:41", "11:47", "11:50",
  "12:1", "12:12", "12:22", "12:31", "12:41", "12:47", "12:50",
  "13:1", "13:11", "13:22", "13:31", "13:41", "13:47", "13:50",
  "14:1", "14:11", "14:22", "14:31", "14:41", "14:47", "14:50",
  "15:1", "15:12", "15:22", "15:30", "15:41", "15:48", "15:50", "15:55",
  "16:0", "16:9", "16:17", "16:20", "16:25", "16:32", "16:41", "16:48", "16:51", "16:58",
  "17:2", "17:11", "17:19", "17:21", "17:28", "17:32", "17:40", "17:48", "17:50", "17:57",
  "18:5", "18:10", "18:18", "18:21", "18:28", "18:32", "18:40", "18:48", "18:51", "18:57",
  "19:5", "19:10", "19:18", "19:21", "19:29", "19:40", "19:48", "19:51", "19:58",
  "20:4", "20:14", "20:16", "20:21", "20:31", "20:40", "20:48", "20:51",
  "21:5", "21:13", "21:16", "21:27", "21:38", "21:45", "21:48", "21:57",
  "22:9", "22:16", "22:21", "22:30", "22:38", "22:48", "22:56",
  "23:9", "23:25"
}; //149

char* Weekend_Table_Out[]={
  "5:18",  "5:47",
  "6:5", "6:26", "6:38", "6:54",
  "7:6", "7:16", "7:26", "7:37", "7:46", "7:53",
  "8:0", "8:3", "8:10", "8:16", "8:21", "8:31", "8:34", "8:41", "8:47", "8:52",
  "9:1", "9:10", "9:16", "9:22", "9:31", "9:33", "9:42", "9:52", "9:59",
  "10:3", "10:12", "10:22", "10:31", "10:33", "10:41", "10:49", "10:53",
  "11:1", "11:9", "11:12", "11:21", "11:30", "11:40", "11:42", "11:52",
  "12:2", "12:10", "12:12", "12:21", "12:31", "12:41", "12:52",
  "13:1", "13:10", "13:12", "13:21", "13:31", "13:41", "13:52",
  "14:0", "14:10", "14:12", "14:21", "14:31", "14:41", "14:52",
  "15:1", "15:10", "15:12", "15:23", "15:31", "15:41", "15:53",
  "16:1", "16:9", "16:11", "16:22", "16:32", "16:43", "16:52",
  "17:0", "17:2", "17:11", "17:22", "17:30", "17:32", "17:40", "17:49", "17:55",
  "18:0", "18:2", "18:8", "18:15", "18:21", "18:30", "18:32", "18:40", "18:50", "18:54",
  "19:0", "19:3", "19:8", "19:14", "19:22", "19:30", "19:32", "19:41", "19:49", "19:55",
  "20:0", "20:4", "20:8", "20:13", "20:23", "20:30", "20:33", "20:42", "20:49",
  "21:1", "21:3", "21:12", "21:22", "21:30", "21:35", "21:40", "21:50",
  "22:0", "22:5", "22:9", "22:19", "22:30", "22:33", "22:41", "22:47", "22:59",
  "23:2", "23:8", "23:17", "23:24", "23:29", "23:33", "23:40", "23:50", 
  "0:0", "0:11", "0:22", "0:36", "0:53"
}; //150

// Function //
void bar_down(){
  unsigned long currentMsec_Servo = millis();
  if (currentMsec_Servo - lastMsec_Servo >= speed_Servo) {
    lastMsec_Servo = currentMsec_Servo;
    if (pos < 106) {
      pos += 1;
      myservo.write(pos);
    }
  }
}

void siren(){
  int kankan[]={2,2,2,2,1,0,0,0,1,1,2,2,1,1,1,0,1,1,1,1,1,1,1,1,2,1,1,1,0,0,1,1,2,2,2,1,0,0,0,0};
  unsigned long currentMsec_Siren = millis();
  if (currentMsec_Siren - lastMsec_Siren >= speed_Siren) {
    lastMsec_Siren = currentMsec_Siren;
    if (ledState_1 == LOW) {
      ledState_1 = HIGH;
      ledState_2 = LOW;
    } else {
      ledState_1 = LOW;
      ledState_2 = HIGH;
    }
    digitalWrite(ledPin_1, ledState_1);
    digitalWrite(ledPin_2, ledState_2);
    
    for(int i=0; i<6; i++){
      for(int j=0; j<40; j++){
        pwm(speakerPin, 64000, (18*i+12)*kankan[j]*vol);
        delayMicroseconds(delayTime);
      }
      if (currentMsec_Siren - delay_Start >= delay_Servo) {
        bar_down();
      }
    }
    for(int i=0; i<10; i++){
      for(int j=0; j<40; j++){
        pwm(speakerPin, 64000, 120*kankan[j]*vol);
        delayMicroseconds(delayTime);
      }
      if (currentMsec_Siren - delay_Start >= delay_Servo) {
        bar_down();
      }
    }
    for(int i=0; i<8; i++){
      for(int j=0; j<40; j++){
        pwm(speakerPin, 64000, (120-10*i)*kankan[j]*vol);
        delayMicroseconds(delayTime);
      }
      if (currentMsec_Siren - delay_Start >= delay_Servo) {
        bar_down();
      }
    }
    for(int i=0; i<40; i++){
      for(int j=0; j<40; j++){
        pwm(speakerPin, 64000, (40-i)*kankan[j]*vol);
        delayMicroseconds(delayTime);
      }
      if (currentMsec_Siren - delay_Start >= delay_Servo) {
        bar_down();
      }
    }
  }
}

// Main //
void setup(){
  Serial.begin(57600); //Debug
  rtc.begin();
  myservo.attach(3);
  for (int i = 106; i >= 10; i -= 1) {
    myservo.write(i);
    delay(15);
  }
  pinMode(speakerPin, OUTPUT);
  pinMode(ledPin_1, OUTPUT);
  pinMode(ledPin_2, OUTPUT);
  pinMode(ledPin_In, OUTPUT);
  pinMode(ledPin_Out, OUTPUT);
}

void loop(){
  DateTime now = rtc.now();
  DateTime now_In = now.unixtime() + marginSec_In;
  DateTime now_Out = now.unixtime() + marginSec_Out;
  String currentTime_In = String(now_In.hour()) + ":" + String(now_In.minute());
  String currentTime_Out = String(now_Out.hour()) + ":" + String(now_Out.minute());
  
  if (now.unixtime() != lastTime){
    lastTime = now.unixtime();
    if ((now.dayOfTheWeek() != 0) && (now.dayOfTheWeek() != 6)){
      for(int i=0; i<=169; i++){
        if (currentTime_In == Weekday_Table_In[i]){
          if (now_In.minute() != last_now_In){
            last_now_In = now_In.minute();
            total_duration = duration_In + 1000;
            arrow_duration_In = duration_In;
            Serial.print(Weekday_Table_In[i]);
            Serial.println("// In Start");
          }
        }
      }
      for(int i=0; i<=154; i++){
        if (currentTime_Out == Weekday_Table_Out[i]){
          if (now_Out.minute() != last_now_Out){
            last_now_Out = now_Out.minute();
            total_duration = duration_Out + 1000;
            arrow_duration_Out = duration_Out;
            Serial.print(Weekday_Table_Out[i]);
            Serial.println("// Out Start");
          }
        }
      }
    } else {
      for(int i=0; i<=148; i++){
        if (currentTime_In == Weekend_Table_In[i]){
          if (now_In.minute() != last_now_In){
            last_now_In = now_In.minute();
            total_duration = duration_In + 1000;
            arrow_duration_In = duration_In;
            Serial.print(Weekend_Table_In[i]);
            Serial.println("// In Start");
          }
        }
      }
      for(int i=0; i<=149; i++){
        if (currentTime_Out == Weekend_Table_Out[i]){
          if (now_Out.minute() != last_now_Out){
            last_now_Out = now_Out.minute();
            total_duration = duration_Out + 1000;
            arrow_duration_Out = duration_Out;
            Serial.print(Weekend_Table_Out[i]);
            Serial.println("// Out Start");
          }
        }
      }
    }
  }


  unsigned long currentMsec = millis();
  
  if (total_duration > 0){
    total_duration = total_duration - (currentMsec - lastMsec);
  } else { total_duration = 0; }
  if (arrow_duration_In > 0){
    arrow_duration_In = arrow_duration_In - (currentMsec - lastMsec);
  } else { arrow_duration_In = 0; }
  if (arrow_duration_Out > 0){
    arrow_duration_Out = arrow_duration_Out - (currentMsec - lastMsec);
  } else { arrow_duration_Out = 0; }

  // debug
  if (currentMsec - lastMsec_debug >= 1000){
    lastMsec_debug = currentMsec;
    Serial.print("Total=");
    Serial.print(total_duration);
    Serial.print("/In=");
    Serial.print(arrow_duration_In);
    Serial.print("/Out=");
    Serial.println(arrow_duration_Out);
  }
  //

  lastMsec = currentMsec;
  
  if (arrow_duration_In > 0){
    digitalWrite(ledPin_In, HIGH);
  } else {
    digitalWrite(ledPin_In, LOW);
  }
  if (arrow_duration_Out > 0){
    digitalWrite(ledPin_Out, HIGH);
  } else {
    digitalWrite(ledPin_Out, LOW);
  }

  if (total_duration > 0){
    if (flag == 0){
      delay_Start = currentMsec;
      flag = 1;
    }
    siren();
  } else {
    digitalWrite(ledPin_1, LOW);
    digitalWrite(ledPin_2, LOW);
    if (pos != 11) {
      for (int i = 106; i >= 11; i -= 1) {
      myservo.write(i);
      delay(15);
      }
    pos = 11;
    flag = 0;
    }
  }
}
