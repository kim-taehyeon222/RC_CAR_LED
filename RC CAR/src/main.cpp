#include <Arduino.h>
#include <PinChangeInterrupt.h>

#define NEUTRAL_THROTTLE 1500

// 채널 핀
#define CH1 A0  // 밝기
#define CH2 A1  // 색상
#define CH5 A2  // on/off

// LED 핀
const int LED_R = 9;
const int LED_G = 10;
const int LED_B = 11;

// PWM 저장
volatile unsigned long startCH1, startCH2, startCH5;
volatile int pwmCH1 = NEUTRAL_THROTTLE;
volatile int pwmCH2 = NEUTRAL_THROTTLE;
volatile int pwmCH5 = NEUTRAL_THROTTLE;
volatile bool newCH1 = false, newCH2 = false, newCH5 = false;

bool ledOn = true;

void setup() {
  Serial.begin(9600);

  pinMode(CH1, INPUT_PULLUP);
  pinMode(CH2, INPUT_PULLUP);
  pinMode(CH5, INPUT_PULLUP);

  attachPCINT(digitalPinToPCINT(CH1), changeBrightness, CHANGE);
  attachPCINT(digitalPinToPCINT(CH2), changeColor, CHANGE);
  attachPCINT(digitalPinToPCINT(CH5), onOff, CHANGE);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
}

// CH1 Interrupt: Brightness
void changeBrightness() {
  if (digitalRead(CH1)) startCH1 = micros();
  else {
    pwmCH1 = micros() - startCH1;
    newCH1 = true;
  }
}

// CH2 Interrupt: Color
void changeColor() {
  if (digitalRead(CH2)) startCH2 = micros();
  else {
    pwmCH2 = micros() - startCH2;
    newCH2 = true;
  }
}

// CH5 Interrupt: On/Off
void onOff() {
  if (digitalRead(CH5)) startCH5 = micros();
  else {
    pwmCH5 = micros() - startCH5;
    newCH5 = true;
  }
}

// LED 색상 변경
void turnOnLed(int pwm, int bright) {
  if (ledOn) {
    if (pwm < 1300) {
      analogWrite(LED_R, bright);
      analogWrite(LED_G, 0);
      analogWrite(LED_B, 0);
    } else if (pwm < 1700) {
      analogWrite(LED_R, 0);
      analogWrite(LED_G, bright);
      analogWrite(LED_B, 0);
    } else {
      analogWrite(LED_R, 0);
      analogWrite(LED_G, 0);
      analogWrite(LED_B, bright);
    }
  } else {
    analogWrite(LED_R, 0);
    analogWrite(LED_G, 0);
    analogWrite(LED_B, 0);
  }
}

void loop() {
  int brightness = map(pwmCH1, 1000, 2000, 0, 255); // CH1: Brightness - 0~255 매핑

  if (newCH5) {
    ledOn = (pwmCH5 > 1500);  // CH5: ON/OFF
    newCH5 = false;
  }

  turnOnLed(pwmCH2, brightness);

  newCH1 = newCH2 = false;
}
