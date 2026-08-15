/*
  ==============================================================================

    MyOsc.h
    Created: 10 Aug 2026 7:51:05pm
    Author:  Patrick

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ParamSettings.h"

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
        auto&& outBlock = context.getOutputBlock();

        auto len = outBlock.getNumSamples();
        auto numChannels = outBlock.getNumChannels();
        //auto inputChannels = inBlock.getNumChannels();

        maintone.setFrequency(targetPitch);
        
        auto* dst = outBlock.getChannelPointer(0);

        for (size_t i = 0; i < len; ++i)
            dst[i] += maintone.processSample(0);
       

        switch (currentState)
        {
        case IDLE:
            break;
        case RAMP_UP:

            for (size_t i = 0; i < len; ++i)
            {
                dst[i] += maintone.processSample(0) * currentGain; //* lfo gain
            }
            currentGain = smoothedGain.getNextValue();
            break;
        case HOLD_NOTE:
            break;
        case NOTE_CHANGE:
            break;
        default:
            break;
        }
        //currentPitch = targetPitch * pitchLfo
        //maintone.setFrequency(targetPitch);
        //maintone.process(context);
    }

    void setFrequency(float newFrequency);
    void setGain(float newGain);
    void setRampUpSpeed(float newSpeed);
    void setState(State newState);



private:
    double sampleRate;
    int samplesPerBlock;

    float currentPitch;
    float targetPitch;

    double rampUpSpeed = 0.0; // Set from osc manager
    float currentGain = 0.0f;
    float targetGain = 0.0f;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> smoothedGain;

    State currentState = IDLE;
    
    juce::dsp::Oscillator<float> maintone;
    size_t lookupTableSize = 1024;

    //juce::dsp::Oscillator<float> pitchlfo;
    //juce::dsp::Oscillator<float> volumelfo;

};