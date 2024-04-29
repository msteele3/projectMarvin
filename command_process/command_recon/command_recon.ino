#include "Audio.h"
#include "CloudSpeechClient.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

//const char *ssid2 = "";
const char *ssid2 = "Wh";
int test = 0;
//const char *password2 = ""; 
const char *password2 = "";
const String goveeKey = "-9a09-b12b1f254a51";


// Define the input pin and output pin
const int inputPin = 13;   // Adjust as necessary
const int outputPin = 18;  // Output pin definition
const int statePin = 33;

// State definitions
enum State {
  IDLE,
  RECORDING
};

// Current state
State currentState = IDLE;

// Function declarations
void recordAudio();
void sendPostRequest(int colorValue);

void setup() {
  pinMode(inputPin, INPUT);
  pinMode(outputPin, OUTPUT);
  pinMode(statePin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Setup complete.");
  //changeLightColor("65280");


}

void loop() {
  int inputState = digitalRead(inputPin);

  switch (currentState) {
    case IDLE:
      digitalWrite(outputPin, LOW);
      if (inputState == HIGH) {
        currentState = RECORDING;
        recordAudio();
        digitalWrite(statePin, HIGH);
        delay(200);
        digitalWrite(statePin, LOW);
        currentState = IDLE;
      } else {
          if(test % 10 == 0) {
    Serial.println("waiting");
  }
      }
      break;

    case RECORDING:
      // This block is reserved for potential future use
      break;
  }
  delay(500);
  test++;
}

void recordAudio() {
  digitalWrite(outputPin, HIGH);
  Serial.println("\r\nRecord start!\r\n");
  Audio* audio = new Audio(ICS43434);
  audio->Record();
  digitalWrite(outputPin, LOW);
  Serial.println("Recording Completed. Now Processing...");
  CloudSpeechClient* cloudSpeechClient = new CloudSpeechClient(USE_APIKEY);
  String transcription = cloudSpeechClient->Transcribe(audio);
  delete cloudSpeechClient;
  delete audio;
  processCommand(transcription);

}

void processCommand(const String& command) {
  Serial.println(command);
  if (command.indexOf("party mode") != -1) {
    changeLightColor("16711808");

  } else if (command.indexOf("chill mode") != -1) {
    changeLightColor("8421504");

  } else if (command.indexOf("lights green") != -1) {
    changeLightColor("65280");

  } else if (command.indexOf("lights blue") != -1) {
    changeLightColor("255");
  } else if (command.indexOf("lights red") != -1) {
    changeLightColor("16711680");
    Serial.println("Command red.");
  } else if (command.indexOf("lights off") != -1) {
  } else {
    Serial.println("Command not recognized.");
  }
}


void changeLightColor(String color) {
  Serial.println("testing govee");
  HTTPClient http;
   WiFi.begin(ssid2, password2);
  while (WiFi.status() != WL_CONNECTED) delay(1000);
  http.begin("https://openapi.api.govee.com/router/api/v1/device/control");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Govee-api-key", "c474098-9");
  String httpRequestData = "{\"requestId\": \"uuid\", \"payload\": {\"sku\": \"H7021\", \"device\": \"D9:35:CD:30:38:35:63:1F\", \"capability\": {\"type\": \"devices.capabilities.color_setting\", \"instance\": \"colorRgb\", \"value\": ";
  httpRequestData += color; //this should be replaced with a light color param
  httpRequestData += "}}}";
  int httpCode = http.POST(httpRequestData);

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString(); 
    Serial.println(payload);
  } else {
    Serial.println(httpCode);
    Serial.printf("[HTTP] PUT request failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();


}