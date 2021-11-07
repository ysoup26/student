/*hw11-Line Following 장애물 회피 과제1*/
#include <SoftwareSerial.h>
#include <Ultrasonic.h>

//bluetooth 연결
int blueRx=4;
int blueTx=5;
unsigned char commandReadyFlag=0;
unsigned char commandblue,power;
SoftwareSerial mySerial(blueRx,blueTx); //소프트 웨어 시리얼통신 포트 생성

//color sensor sett
#define BLACK 0
#define WHITE 1
int sensor0,sensor1; //sensor reading 0~1023
int sensorLeft,sensorRight;// 왼-1 오-0

//ultrasonic sett
char trig=9;
char echo=8;
int distance;
int cutDistance=10; //장애물과의 거리 설정
Ultrasonic ultrasonic(trig,echo); // ultrasonic 객체 생성

#define CW  1 
#define CCW 0
#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1

const byte PWMA = 3;      // PWM control (speed) for motor A
const byte PWMB = 11;     // PWM control (speed) for motor B
const byte DIRA = 12;     // Direction control for motor A
const byte DIRB = 13;     // Direction control for motor B

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  setupArdumoto();
}

void loop() {
  //Bluetooth comm
  communication();
  //read
  if(commandblue==1){
    readSensors();
    //Serial.println(distance);
    if(distance<cutDistance){
      robotAvoidance();
    }else{
      robotControl(sensorLeft,sensorRight);
    }
  }else{
    robotStop();
  }
}
//모든 센서 측정
void readSensors(){
  readCSensors();
  readUltraSensor(); 
}
//컬러 센서 측정-센서 정면 기준
void readCSensors(){
  sensor0=1023-analogRead(0);
  sensor1=1023-analogRead(1);
}
char colorFinder(int sensorValue){
  char color;
  const int THRESHOLD=800;
  if(sensorValue>THRESHOLD)
    color=WHITE;
  else
    color=BLACK;
  return color;
}
// 초음파 센서 측정
void readUltraSensor(){
   distance = ultrasonic.read();  // read distance in cm
}
//로봇 컨트롤
void robotControl(int sensorLeft,int sensorRight){
  sensorRight = colorFinder(sensor0);
  sensorLeft = colorFinder(sensor1);
  if(sensorLeft==BLACK && sensorRight==BLACK){
    robotForward(100);
  }else if(sensorLeft==BLACK && sensorRight==WHITE){
    robotLeft(100,100);
    delay(100);
  }else if(sensorLeft==WHITE && sensorRight==BLACK){
    robotRight(100,100);
    delay(100);
  }else
    robotStop();
}
//초기화 및 세팅
void setupArdumoto()
{
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}
//장애물과 만났을 때//배터리수치에 따라 회전방향, 속도 달라짐
void robotAvoidance()
{
   robotStop();
    robotRight(100,100); // 45도 
    delay(300);
    robotForward(100);  // 20 cm 
    delay(1800);
    robotLeft(100,100);  // 75 deg
    delay(550); 
    robotForward(100);
      //원래 줄로
    delay(1200);
}
/*로봇 주행*/
void driveArdumoto(byte motor, byte dir, byte spd)
{
  if(motor == MOTOR_LEFT)
  {
    digitalWrite(DIRA, dir);
    analogWrite(PWMA, spd);
  }
  else if(motor == MOTOR_RIGHT)
  {
    digitalWrite(DIRB, dir);
    analogWrite(PWMB, spd);
  }  
}
//전진 //초음파가 앞면이니 수정
void robotForward(int velocity)
{
  driveArdumoto(MOTOR_LEFT, CW, velocity);
  driveArdumoto(MOTOR_RIGHT, CCW, velocity);
}
//후진
void robotBackward(int velocity)
{
  driveArdumoto(MOTOR_LEFT, CCW, velocity);
  driveArdumoto(MOTOR_RIGHT, CW, velocity);
}
//우회전
void robotRight(int velocity1,int velocity2)
{
  driveArdumoto(MOTOR_LEFT, CCW, velocity1);
  driveArdumoto(MOTOR_RIGHT, CCW, velocity2);
}
//좌회전
void robotLeft(int velocity1,int velocity2)
{
  driveArdumoto(MOTOR_LEFT, CW, velocity1);
  driveArdumoto(MOTOR_RIGHT, CW, velocity2);
}
//로봇 정지
void stopArdumoto(byte motor)
{
  driveArdumoto(motor, 0, 0);
}
void robotStop()
{
  stopArdumoto(MOTOR_LEFT);
  stopArdumoto(MOTOR_RIGHT);
}
//Bluetooth communication 
void communication(){
  if(mySerial.available()>=4){//버퍼에 들어온 갯수-4바이트
    //robotForward(50);
    Serial.println("data arrived ");
    unsigned char buffer[4];
    //read data form buffer and save to array
    for(char i=0;i<4;i++){
      buffer[i]=mySerial.read();
    }
    if(buffer[0]==255&&buffer[3]==100){//약속함
      commandblue=buffer[1];
      power=buffer[2];
      Serial.print("command:");
      Serial.println(commandblue);
      Serial.print("power:");
      Serial.println(power);
    }
  }
}
