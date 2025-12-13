#include <Keypad.h>
#include <esp_now.h>
#include <WiFi.h>

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// LED pins
const int GREEN_LED = 2;
const int RED_LED = 4;
const int BLUE_LED = 5;

// Buzzer pin
const int BUZZER = 15;

// ESP-NOW
// Replace this with your actual Display MAC!
uint8_t receiverMAC[] = {0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x01};

typedef struct struct_message {
  char key;
  int score;
  bool gameOver;
  bool newGame;
} struct_message;

struct_message myData;

// Forward declaration
void OnDataRecv(const esp_now_recv_info* info, const uint8_t* incomingData, int len);

void setup() {
  Serial.begin(115200);
  Serial.print("Controller MAC: ");
  Serial.println(WiFi.macAddress());

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (!esp_now_is_peer_exist(receiverMAC)) {
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("Failed to add peer");
      return;
    }
  }

  esp_now_register_recv_cb(OnDataRecv);

  startupSequence();
}

// NEW CALLBACK SIGNATURE!
void OnDataRecv(const esp_now_recv_info* info, const uint8_t* incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  updateLEDs(myData.score, myData.gameOver);
  if (myData.gameOver) {
    gameOverSound();
  } else if (myData.score > 0) {
    scoreSound();
  }
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    myData.key = key;
    myData.score = 0;
    myData.gameOver = false;
    myData.newGame = (key == 'A');
    esp_err_t result = esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData));
    if (result != ESP_OK) {
      Serial.print("Send failed: ");
      Serial.println(result);
    }
    buttonFeedback(key);
  }
  delay(50);
}

void buttonFeedback(char key) {
  digitalWrite(BLUE_LED, HIGH);
  tone(BUZZER, 1000, 100);
  delay(150);
  digitalWrite(BLUE_LED, LOW);
}

void updateLEDs(int score, bool gameOver) {
  if (gameOver) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  } else if (score > 100) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  } else {
    digitalWrite(GREEN_LED, score > 0 ? HIGH : LOW);
    digitalWrite(RED_LED, LOW);
  }
}

void scoreSound() {
  tone(BUZZER, 1500, 200);
}

void gameOverSound() {
  for (int i = 0; i < 3; i++) {
    tone(BUZZER, 800, 300);
    delay(400);
  }
}

void startupSequence() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
    tone(BUZZER, 1200, 200);
    delay(300);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    delay(200);
  }
}