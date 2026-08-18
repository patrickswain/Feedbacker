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
        auto&& outputBlock = context.getOutputBlock();

        auto numSamples = outputBlock.getNumSamples();
        auto numChannels = outputBlock.getNumChannels();
        //auto inputChannels = inBlock.getNumChannels();
        auto* buffer = outputBlock.getChannelPointer(0);
        maintone.setFrequency(targetPitch);
        
        

        switch (currentState)
        {
        case IDLE:
            currentGain = 0.0f;
            updateGain();
            //DBG("current gain in process = " << currentGain);
            break;
        case RAMP_UP:

            for (size_t i = 0; i < numSamples; ++i)
            {
                //DBG("currentGain in process = " << currentGain);
                buffer[i] += maintone.processSample(0) * currentGain; //* lfo gain
                currentGain = smoothedGain.getNextValue();
            }
            break;
        case HOLD_NOTE:
            break;
        case NOTE_CHANGE:
            break;
        default:
            break;
        }
 
    }

    void setFrequency(float newFrequency);
    void setGain(float newGain);
    void updateGain();
    void setRampUpSpeed(float newSpeed);
    void setState(State newState);


private:
    double sampleRate = 48000;
    int samplesPerBlock = 512;

    float currentPitch = 440.0f;
    float targetPitch = 440.0f;

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