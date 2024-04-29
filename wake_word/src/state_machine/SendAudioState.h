#ifndef _send_audio_state_h_
#define _send_audio_state_h_

#include "States.h"

class I2SSampler;
class WiFiClient;
class HTTPClient;
class IndicatorLight;
class Speaker;
class IntentProcessor;
class WitAiChunkedUploader;

class SendAudioState : public State
{
private:
    I2SSampler *m_sample_provider;
    unsigned long m_start_time;
    unsigned long m_elapsed_time;
    int m_last_audio_position;
    Speaker *m_speaker;




public:
    SendAudioState(I2SSampler *sample_provider, Speaker *speaker);
    void enterState();
    bool run();
    void exitState();
};

#endif