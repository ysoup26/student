/*hw13 아두이노 라즈베리 파이 통신*/
bool sendReady=0;
int count=0; //측정 횟수

//color sensor sett
int left_sensor_value; // 전면 기준으로 왼쪽 광센서 측정치
int right_sensor_value; // 전면 기준으로 오른쪽 광센서 측정치
#define sensorLeft 1
#define sensorRight 0

void setup() {
  Serial.begin(9600);
  while(!Serial){
    ;
  }
}

void loop() {
  if(Serial.available()>0){ //값이 들어왔을때
    if(Serial.read()=='A'){// 그 값이 A라면
      sensor();
      count++;
      sendReady=1;
    }
  }
  //inform send
  if(sendReady){
    Serial.write(count);
    Serial.write(left_sensor_value);
    Serial.write(right_sensor_value);
  }
  sendReady=0;
}
//센서 측정 & mapping
void sensor(){ 
  left_sensor_value=1023-analogRead(sensorLeft);
  left_sensor_value=map(left_sensor_value,0,1023,0,255);
  right_sensor_value=1023-analogRead(sensorRight);
  right_sensor_value=map(right_sensor_value,0,1023,0,255);
}
