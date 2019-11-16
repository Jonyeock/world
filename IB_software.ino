//////////////////////// 변수 선언 /////////////////////////////

int trig = 3;
int echo = 2;
int RIP = 6;
int S = 5;
int LED = 4;
float avg_H;
/////////////////////////// 함수 선언 부분 ////////////////////////////

float measureDistance(int trig, int echo);
float averageHeight(int trig, int echo, int repeat);

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  pinMode(RIP, INPUT);    // 동작 감지 센서
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(S, OUTPUT);     // 피에조 센서
  pinMode(LED, OUTPUT);
  
  avg_H = averageHeight(trig, echo, 20);  // 처음 전원이 켜졌을때 천장과 바닥사이의 거리 측정(센서값이 변해서 평균값으로 설정)
  Serial.print("averageHeight : ");
  Serial.print(avg_H);
  Serial.println("cm");
  digitalWrite(LED, HIGH);  // 전원이 켜짐과 동시에 LED 불이 들어오도록 setup
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void loop() {
  int val = digitalRead(RIP);   // 동작이 감지되면 1, 동작이 없으면 0

   pinMode(trig, INPUT);    // 초음파 센서가 동작이 감지되었을 경우에만 작동하도록 하기 위해서
   pinMode(echo, OUTPUT);   // trig 핀과 echo 핀의 INPUT OUTPUT 상태를 바꾸어서 동작 정지

  // 경고가 울리는 경우
  // 아동 평균키 최대 130(7세 아동 기준)
  // 바닥까지 초음파 센서범위가 닿을 때 -> 높이에서 변화량이 130보다 작고 동작감지 센서가 1이면 아동
  

  Serial.print("Motionrecognition : ");
  Serial.println(val);
  
  if (val == 1) {
    // 1번의 경우 경고 울림
    pinMode(trig, OUTPUT);    // 동작이 감지 되었으므로 초음파 센서가 다시 정상 작동 하도록 해야함
    pinMode(echo, INPUT);     // trig 핀과 echo 핀의 INPUT OUTPUT 상태를 원상태로 변경
    
    float distance = measureDistance(trig, echo);
    float height = avg_H - distance;
    
    Serial.print("Height : ");
    Serial.print(height);
    Serial.print("cm  ");
    
    if (height > 30 && height < 130) {
      tone(S, 220);
      delay(5000);
    }
  } else {
    // 경고 울리지 않음
    noTone(S);
  }
  delay(200);
}


///////////////////////////// 함수 정의 /////////////////////////////////////

float measureDistance(int trig, int echo){
  digitalWrite(trig, LOW);
  digitalWrite(echo, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
 
  unsigned long duration = pulseIn(echo, HIGH);
  float distance = duration / 29.0 / 2.0;
  return distance;
}

float averageHeight(int trig, int echo, int repeat) {
  float avg_H = 0;
  float sum_H = 0;
  for (int i = 1; i <= repeat; i++){
    digitalWrite(trig, LOW);
    digitalWrite(echo, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
   
    unsigned long duration = pulseIn(echo, HIGH);
    float distance = duration / 29.0 / 2.0;
    sum_H = sum_H + distance;
    avg_H = sum_H / i;
    delay(200);
  }
  return avg_H;
}

