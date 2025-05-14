# RC_CAR_LED

## 영상링크

## 회로 사진

![KakaoTalk_20250513_141750271_01](https://github.com/user-attachments/assets/82267823-d57e-43f0-a1ee-30a509f0f832)
CH1 - A0 - 밝기 조절
CH2 - A1 - 색상 변경
CH5 - A2 - on/off 버튼

![KakaoTalk_20250513_141750271](https://github.com/user-attachments/assets/58ccaad0-fa1b-4995-ace9-d87351cf9971)
LED
---


## 코드 설명
![image](https://github.com/user-attachments/assets/e2d2ed5b-92c9-48e3-ac76-92f93ed2bb09)
![image](https://github.com/user-attachments/assets/9fc0d122-a2f6-4a08-b143-d0b0205f4244)
![image](https://github.com/user-attachments/assets/20b2dc2b-a109-4079-a523-64be50a63b3e)
![image](https://github.com/user-attachments/assets/f78e9845-1bd6-458d-8083-bd1adf527914)
![image](https://github.com/user-attachments/assets/5f014206-6a17-44fb-9c1f-bd84bc6de9a9)
`...python
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
      analogWrite(redPin, bright);
      analogWrite(yellowPin, 0);
      analogWrite(greenPin, 0);
    } else if (pwm < 1700) {
      analogWrite(redPin, 0);
      analogWrite(yellowPin, bright);
      analogWrite(greenPin, 0);
    } else {
      analogWrite(redPin, 0);
      analogWrite(yellowPin, 0);
      analogWrite(greenPin, bright);
    }
  } else {
    analogWrite(redPin, 0);
    analogWrite(yellowPin, 0);
    analogWrite(greenPin, 0);
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
...`
