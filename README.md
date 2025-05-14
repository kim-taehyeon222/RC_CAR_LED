# RC_CAR_LED

3색 led가 없어서 red, yellow, green led 3개를 이용하여 구성했습니다.

CH1 : LED의 밝기를 조절
CH2 : pulse에 따라 red, yellow, green led 색상 변경
CH5 : on/off 버튼튼

## 영상링크

## 회로 사진

![KakaoTalk_20250513_141750271_01](https://github.com/user-attachments/assets/82267823-d57e-43f0-a1ee-30a509f0f832)
| 채널 | 핀 | 역할 |
|-----|----|------|
| CH1 | A0 | 밝기 조절 |
| CH2 | A1 | 색상 변경 |
| CH5 | A2 | on/off 버튼|

![KakaoTalk_20250513_141750271](https://github.com/user-attachments/assets/58ccaad0-fa1b-4995-ace9-d87351cf9971)

| LED | 핀 |
|-----|----|
| RED | 9 | 
| YELLOW | 10 |
| GREEN | 11 |


## 코드 설명

```C++
#include <Arduino.h>
#include <PinChangeInterrupt.h>

#define NEUTRAL_THROTTLE 1500

// 채널 핀
#define CH1 A0  // 밝기
#define CH2 A1  // 색상
#define CH5 A2  // on/off

// LED 핀
const int redPin = 9;
const int yellowPin = 10;
const int greenPin = 11;

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

  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
}

// CH1 Interrupt: Brightness
void changeBrightness() {
  if (digitalRead(CH1)) startCH1 = micros();
  else {
    pwmCH1 = micros() - startCH1; // CH1 pulse 측정
    newCH1 = true;
  }
}

// CH2 Interrupt: Color
void changeColor() {
  if (digitalRead(CH2)) startCH2 = micros();
  else {
    pwmCH2 = micros() - startCH2; // CH2 pulse 측정
    newCH2 = true;
  }
}

// CH5 Interrupt: On/Off
void onOff() {
  if (digitalRead(CH5)) startCH5 = micros();
  else {
    pwmCH5 = micros() - startCH5; // CH5 pulse 측정정
    newCH5 = true;
  }
}

// LED 색상 변경
void turnOnLed(int pwm, int bright) {
  if (ledOn) { // CH5 on/off 버튼이 on이면 실행
    if (pwm < 1300) { // pulse가 1300이하면 red on
      analogWrite(redPin, bright);
      analogWrite(yellowPin, 0);
      analogWrite(greenPin, 0);
    } else if (pwm < 1700) { // pulse가 1300 ~ 1700이면 yellow on
      analogWrite(redPin, 0);
      analogWrite(yellowPin, bright);
      analogWrite(greenPin, 0);
    } else { // pulse가 1700이상이면 green on
      analogWrite(redPin, 0);
      analogWrite(yellowPin, 0);
      analogWrite(greenPin, bright);
    }
  } else { // CH5 on/off 버튼이 off이면 실행 x
    analogWrite(redPin, 0);
    analogWrite(yellowPin, 0);
    analogWrite(greenPin, 0);
  }
}

void loop() {
  int brightness = map(pwmCH1, 1000, 2000, 0, 255); // CH1: Brightness - 0~255 매핑

  if (newCH5) {
    ledOn = (pwmCH5 > 1500);  // CH5: ON/OFF 1500이상이면 on
    newCH5 = false;
  }

  turnOnLed(pwmCH2, brightness);

  newCH1 = newCH2 = false;
}
```
