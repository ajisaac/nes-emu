#ifndef AUDIO_H
#define AUDIO_H

class Audio {
public:
    // stream audioProviderStream
    // f64 sampleRate
    // int outputChannels
    // channel channel

    Audio() {
        // make channel
    }

    ~Audio() {
        // destroy audio resources
    }

    void start() {
        // set up audio streaming etc, probably do this in the constructor
    }

    void stop() {
        // do stuff in the destructor
    }

    void callback() {
        // not sure
    }
};

#endif //AUDIO_H
