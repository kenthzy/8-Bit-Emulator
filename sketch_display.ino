#include <BluetoothSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

BluetoothSerial SerialBT;
Servo myServo;
Adafruit_SSD1306 display(128, 64, &Wire, -1);

int servoPin = 23;
int angle = 90;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Servo");
  myServo.attach(servoPin);
  myServo.write(angle);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
    while(1);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print("Angle: ");
  display.print(angle);
  display.display();

  Serial.println("Ready for Bluetooth control!");
}

void loop() {
  if (SerialBT.available()) {
    String data = SerialBT.readStringUntil('\n');
    angle = data.toInt();
    if (angle >= 0 && angle <= 180) {
      myServo.write(angle);
      updateDisplay(angle);
      Serial.println("Angle: " + String(angle));
    }
  }
}

void updateDisplay(int a) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.print("Angle: ");
  display.print(a);
  display.display();
}
