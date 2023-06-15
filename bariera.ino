#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define servoPin1 10
#define servoPin2 A1
#define trigPin1 6
#define echoPin1 7

#define trigPin2 8
#define echoPin2 9

#define trigPin3 A3
#define echoPin3 A2

#define trigPin4 5
#define echoPin4 4

#define rosu 3
#define galben 2
#define verde 11

long duration, distance, intrare1, intrare2,iesire1,iesire2;
LiquidCrystal_I2C lcd_1(0x27, 20, 4);

Servo servo1, servo2;

char incomingByte = '0';
char sentByte = '0';

int nr_locuri = 5;

bool checkValue(long value){

  if(value >= 0.0 && value <= 500.0){
    return true;
  }
  return false;
}

long getSensorDistance(int trigPin, int echoPin){

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2)/29.1;

  return distance;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);

  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);

  pinMode(rosu, OUTPUT);
  pinMode(galben, OUTPUT);
  pinMode(verde, OUTPUT);


  //LCD
  lcd_1.init();
  lcd_1.clear();
  lcd_1.backlight();
  //lcd_1.begin(20, 4);//numarul de coloane si linii

  //servomotor
  // Serial.begin(9600);
  servo1.attach(servoPin1);
  servo1.write(180);

  servo2.attach(servoPin2);
  servo2.write(180);
}

void loop() {

  // put your main code here, to run repeatedly:
  intrare1 = getSensorDistance(trigPin1, echoPin1);
  intrare2 = getSensorDistance(trigPin2, echoPin2);
  iesire1 = getSensorDistance(trigPin3, echoPin3);
  iesire2 = getSensorDistance(trigPin4, echoPin4);

  // print_text(intrare1, intrare2, iesire1, iesire2);
  print_locuri_libere(nr_locuri);

  //Serial.println("We got here!");

 if(checkValue(intrare1)){
    bariera_intrare(intrare1, intrare2);
  }

  if(checkValue(iesire1)){
    bariera_iesire(iesire1, iesire2);
  }
    //bariera_intrare(intrare1, intrare2);
    //bariera_iesire(iesire1, iesire2);

}

void bariera_intrare(long intrare1, long intrare2){

  if( nr_locuri > 0 ){
    if(intrare1 > 3.0 && intrare1 <= 12.0){

      sentByte = '1';
      Serial.write(sentByte);
      // while(!Serial.available());
      while( Serial.available()== 0 );
      incomingByte = Serial.read();
      lcd_1.setCursor(0, 2);
      lcd_1.print(incomingByte);
      sentByte = '0';
      Serial.write(sentByte);

      if(incomingByte == '1'){
        digitalWrite(rosu,LOW);
        // Serial.println("Bariera ridicata pentru intare!");
        digitalWrite(galben,HIGH);
        servo1.write(90);
        incomingByte = '0';
        delay(1000);
        digitalWrite(galben,LOW);
        digitalWrite(verde,HIGH);
        // while(intrare1 > 0.0 && intrare1 <= 12.0){
        //   intrare1 = getSensorDistance(trigPin1, echoPin1);
        // }
        delay(1000);
      }
    }

    if( intrare2 > 3.0 && intrare2 <= 12.0){
      // Serial.write('0');
      digitalWrite(verde,LOW);
      // Serial.println("Bariera coborata dupa intrare!");
      servo1.write(180);
      nr_locuri--;
      digitalWrite(galben,HIGH);
      delay(1500);
      digitalWrite(galben,LOW);
      digitalWrite(rosu,HIGH);
    }
    // else{
    //   delay(1000);
    //   servo1.write(180);
    // }

  }
  else{
    servo1.write(180);
  }
}


void bariera_iesire(long iesire1, long iesire2){

  if(nr_locuri < 10){
    if(iesire1 > 3.0 && iesire1 <= 12.0){
      // Serial.println("Bariera ridicata pentru iesire!");
      servo2.write(90);
      delay(1200);
    }
    
    if( iesire2 > 3.0 && iesire2 <= 12.0){
      // Serial.println("Bariera coborata dupa iesire!");
      servo2.write(180);
      nr_locuri++;
      delay(1200);
    }
  }
  
}

void print_locuri_libere(int nr_locuri){

  lcd_1.clear();
  if (nr_locuri > 0){
    lcd_1.setCursor(0, 1);
    lcd_1.print(nr_locuri);
    lcd_1.print(" loc. disponibile");
  }
  else{
    lcd_1.setCursor(0, 1);
    lcd_1.print("Parcare plina!");
    delay(500);
    digitalWrite(verde, LOW);
    digitalWrite(galben, LOW);
    digitalWrite(rosu, HIGH);
  }
}


void print_text(long firstSensor, long secondSensor, long threeSensor,long fourSensor){
  Serial.println();
  Serial.print("L1:");
  Serial.print(firstSensor);
  Serial.print(" cm");
  Serial.print(" R1:");
  Serial.print(secondSensor);
  Serial.print(" cm");
  Serial.print(" L2:");
  Serial.print(threeSensor);
  Serial.print(" cm");
  Serial.print(" R2:");
  Serial.print(fourSensor);
  Serial.println(" cm");

}

