/*hw11-Line Following 장애물 회피 과제1*/
#include <SoftwareSerial.h>
#include <Ultrasonic.h>
#include <TimerOne.h>

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

// 인터럽트 설정
const char F = 1; // forward
const char B = 2; // backward
const char R = 3; // right
const char L = 4; // left
const char S = 5; // stop
const char A = 6; // avoid obstacle
int command; //인터럽트 모드

#define CW  1 
#define CCW 0
#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1

const byte PWMA = 3;      // PWM control (speed) for motor A
const byte PWMB = 11;     // PWM control (speed) for motor B
const byte DIRA = 12;     // Direction control for motor A
const byte DIRB = 13;     // Direction control for motor B

void setup() {
  Timer1.initialize(50000);
  Timer1.attachInterrupt( timerIsr );
  Serial.begin(9600);
  mySerial.begin(9600);
  setupArdumoto();
}

void loop() {
  //Bluetooth comm
  communication();
  
  //read
  if(commandblue==1){
   motionControl(command);
  }else{
    robotStop();
  }
}

//call-back 함수
void timerIsr()
{
    readSensors();  // read all sensors
    sendCommand();  // send motion control command to application context
    //Serial.println(command);
}
//command function
void sendCommand(){
    // 왼쪽, 오른쪽 센서 측정 컬러: sensorLeft sensorRight 

    if (distance < cutDistance&&distance!=0){//인터럽트 주기가 빨라서 쓰레기값 출력 되는 경우 방지
      command = A; //장애물 회피
    }
    else if ( sensorLeft == BLACK && sensorRight == BLACK){
      command = F; // 전진
    }
    else if ( sensorLeft == WHITE && sensorRight == BLACK )  {
      command = R;  // 우회전
    }
     else if  (sensorLeft == BLACK && sensorRight == WHITE) { 
      command = L;   // 좌회전
    }
    else if ( sensorLeft == WHITE && sensorRight == WHITE ) {
      command = S; // 정지
    }
    else {
    }
    //Serial.println(command);
}

// CONTROL : motion control ( 로봇 모션 제어)
void motionControl(int command){
  Serial.println(command);
  switch(command)
    {
      case F:
      robotForward(100);     
      break;
      
      case B:
      robotBackward(100);
      break;
      
      case R:
      robotRight(100,100);
      delay(100);
      break;
      
      case L:
      robotLeft(100,100);
      delay(100);
      break;
      
      case S:
      robotStop();
      break;

      case A:   // 장애물 회피 모션 정의
      robotAvoidance();

      default :
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
  //밝으면 값 커짐
  sensorRight = colorFinder(sensor0);
  sensorLeft = colorFinder(sensor1);
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
//장애물과 만났을 때
void robotAvoidance()
{
   robotStop();
    robotRight(100,100); // 45도 
    delay(300);
    robotForward(100);  // 20 cm 
    delay(1700);
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
      //Serial.println(commandblue);
      Serial.println(commandblue);
      Serial.print("power:");
      Serial.println(power);
    }
  }
}
