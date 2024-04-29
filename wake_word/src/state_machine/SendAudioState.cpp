#include <Arduino.h>
#include <ArduinoJson.h>
#include "I2SSampler.h"
#include "RingBuffer.h"
#include "SendAudioState.h"
#include "IndicatorLight.h"
#include "Speaker.h"
#include "../config.h"
#include <string.h>
#include <HTTPClient.h>
#include <driver/dac.h>

#define WINDOW_SIZE 320
#define STEP_SIZE 160
#define POOLING_SIZE 6
#define AUDIO_LENGTH 16000

SendAudioState::SendAudioState(I2SSampler *sample_provider, Speaker *speaker)
{
    // save the sample provider for use later
    m_sample_provider = sample_provider;
    m_speaker = speaker;
}

void SendAudioState::enterState()
{
    Serial.println("testing enter state");
    m_start_time = millis();
    m_elapsed_time = 0;
    m_last_audio_position = -1;
    

    uint32_t free_ram = esp_get_free_heap_size();
    Serial.printf("Free ram before connection %d\n", free_ram);
    Serial.println("Ready for action");
    free_ram = esp_get_free_heap_size();
    Serial.printf("Free ram after connection %d\n", free_ram);
    
}

void SendAudioState::exitState()
{
    // clean up the speech recognizer client as it takes up a lot of RAM
    uint32_t free_ram = esp_get_free_heap_size();
    Serial.printf("Free ram after request %d\n", free_ram);
}




bool SendAudioState::run()
{

      // Configure pin 2 as an input
    pinMode(2, INPUT);
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
    digitalWrite(23,HIGH);
    delay(2000);
    digitalWrite(23,LOW);

    // Continuously check the state of pin 2
    while (digitalRead(2) == LOW)
    {
        // Pin 2 is LOW, so continue running
        delay(100); // Add a small delay to prevent the loop from running too fast
    }

    // Pin 2 is HIGH, so we're done
    digitalWrite(5, LOW);
    return true;
}
  



//  HTTPClient http;

//     // Specify the URL
//     http.begin("https://.coindesk.com/v1/bpi/currentprice.json");

//     // Send the request
//     int httpCode = http.GET();

//     // Check the returning code
//     if (httpCode > 0) {
//         // Get the request response payload
//         String payload = http.getString();

//         // Parse JSON
//         DynamicJsonDocument doc(1024);
//         deserializeJson(doc, payload);

//         // Print JSON to Serial
//         serializeJsonPretty(doc, Serial);
//         Serial.println();
//     } else {
//         Serial.println("Error on HTTP request");
//     }

//     // Close connection
//     http.end();





// bool SendAudioState::run()
// {
//     dac_output_enable(DAC_CHANNEL_1);
//     Serial.println("testing run state");
//     // Define the frequency and duration of the sine wave
//     float frequency = 440.0; // 440 Hz (A4)
//     float duration = 1.0;   // 1 second

//     // Calculate the number of samples to generate
//     int numSamples = duration * AUDIO_LENGTH / 16;

//     // Calculate the step size for each sample
//     float stepSize = 2 * M_PI * frequency / AUDIO_LENGTH * 16;

//     // Generate and output the sine wave
  
//     // Turn off for 1 second
//     dac_output_voltage(DAC_CHANNEL_1, 0);
//     delay(1000);

//     // Turn on for 2 seconds
//     for (int i = 0; i < numSamples * 2; i++)
//     {
//         // Calculate the current sample value
//         float sampleValue = sin(i * stepSize);

//         // Scale the sample value to the DAC range (0-255)
//         uint8_t dacValue = (sampleValue + 1) * 127;

//         // Output the sample value to the DAC
//         dac_output_voltage(DAC_CHANNEL_1, dacValue);

//         // Delay between samples to control the playback speed
//         delayMicroseconds(1000);
//     }

//     // Print "done" to Serial
//     Serial.println("done");

//     return true;
// }
