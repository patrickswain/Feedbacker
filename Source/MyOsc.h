/*
  ==============================================================================

    MyOsc.h
    Created: 10 Aug 2026 7:51:05pm
    Author:  Patrick

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
class MyOsc
{   
public:
    MyOsc();
    ~MyOsc();

    //void initialize();
    void prepare(const juce::dsp::ProcessSpec& spec);

    template <typename ProcessContext>
    void process(const ProcessContext& context)
    {

        //currentPitch = targetPitch * pitchLfo
        maintone.setFrequency(targetPitch);
        maintone.process(context);
    }

    void setFrequency(float newFrequency);
    void setGain(float newGain);



private:
    double sampleRate;
    int samplesPerBlock;

    float currentPitch;
    float targetPitch;

    float currentGain;
    float targetGain;

    //State currentState;
    
    juce::dsp::Oscillator<float> maintone;
    size_t lookupTableSize = 1024;

    //juce::dsp::Oscillator<float> pitchlfo;
    //juce::dsp::Oscillator<float> volumelfo;

};