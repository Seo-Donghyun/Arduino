#include <Servo.h>

// Arduino pin assignment

#define PIN_LED 9 // LED
#define PIN_POTENTIOMETER 3 // Potentiometer at Pin A3
// Add IR Sensor Definition Here !!!
#define PIN_SERVO 10
#define PIN_IRSENSOR 0 // 적외선 센서
#define DIST_MIN 0 // 최소 거리 (단위 : mm)
#define DIST_MAX 800 // 최대 거리

#define _DUTY_MIN 553  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counter-clockwise position (180 degree)

#define _TARGET_LOW 100
#define _TARGET_HIGH 250

#define LOOP_INTERVAL 200   // Loop Interval (unit: msec)
#define _EMA_ALPHA 0.5 // EMA 알파값

Servo myservo;
unsigned long last_loop_time;   // unit: msec
float dist_prev = DIST_MAX; // 처음에 이전 거리를 정의
float dist_ema = DIST_MAX;

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  
  dist_prev = DIST_MIN;
  Serial.begin(57600);
}

void loop()
{
  float dist;
  unsigned long time_curr = millis();
  long a_value, duty;


  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  
  // Read IR Sensor value !!!
  a_value = analogRead(PIN_IRSENSOR); // 적외선 센서 값 읽기
  // Convert IR sensor value into distance !!!
  dist = (6762.0 / (a_value - 9) - 4.0) * 10.0 - 60.0; // 읽은 값을 거리로 계산

  duty = func(a_value, 347, 202, _DUTY_MIN, _DUTY_MAX);

  if (dist < _TARGET_LOW){
    myservo.writeMicroseconds(_DUTY_MIN);
    digitalWrite(PIN_LED, HIGH);
  }else if(_TARGET_LOW <= dist && dist <= _TARGET_HIGH){
    myservo.writeMicroseconds(duty);
    digitalWrite(PIN_LED, LOW);
  }else if(dist > _TARGET_HIGH){
    myservo.writeMicroseconds(_DUTY_MAX);
    digitalWrite(PIN_LED, HIGH);
  }
  // we need distance range filter here !!!
  if (dist < DIST_MIN) {
    dist = dist_prev;    // Set Lower Value    
  } else if (dist > DIST_MAX) {
    dist = dist_prev;    // Set Higher Value
  } else {    
    ;
  }
  // we need EMA filter here !!!
  dist_ema = _EMA_ALPHA * dist + (1 - _EMA_ALPHA) * dist_ema;

  // print IR sensor value, distnace, duty !!!
  Serial.print("MIN:"); Serial.print(DIST_MIN);
  Serial.print(",IR:"); Serial.print(a_value);
  Serial.print(",dist"); Serial.print(dist);
  Serial.print(",ema:"); Serial.print(dist_ema);
  Serial.print(",servo:"); Serial.print(duty);
  Serial.print(",MAX:"); Serial.print(DIST_MAX);
  Serial.println("");

  dist_prev = dist; // 이전 거리를 조정
  last_loop_time += LOOP_INTERVAL;

}
long func(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
