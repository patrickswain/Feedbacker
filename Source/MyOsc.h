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
        auto* buffer = outputBlock.getChannelPointer(0);
        
        switch (currentState)
        {
        case IDLE:
            currentGain = 0.0f;
            updateGain();
            break;
        case RAMP_UP:

            for (size_t i = 0; i < numSamples; ++i)
            {
                buffer[i] += maintone.processSample(0) * currentGain; //* lfo gain
                currentGain = smoothedGain.getNextValue();
            }
            break;
        case HOLD_NOTE:
            for (size_t i = 0; i < numSamples; ++i)
            {
                buffer[i] += maintone.processSample(0) * currentGain; //* lfo gain
                currentGain = smoothedGain.getNextValue();
                //if (!bypassPitchLfo)
                //{
                //    maintone.setFrequency(targetPitch + (pitchLfo.processSample(0) * pitchLfoDepth));
                //}
                
            }
            break;
        case NOTE_CHANGE:
            for (size_t i = 0; i < numSamples; ++i)
            {             
                buffer[i] += maintone.processSample(0) * currentGain; //* lfo gain  
                currentGain = smoothedGain.getNextValue();
            }
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
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedGain;

    State currentState = IDLE;
    
    size_t lookupTableSize = 1024;
    juce::dsp::Oscillator<float> maintone;
    juce::dsp::Oscillator<float> pitchLfo;
    float pitchLfoRate = 0.5f; // Rate in seconds
    float pitchLfoDepth = 20.f; // range of Hz that pitch lfo affects (if note pitch is 440 and depth is 10, new pitch will be between 435 and 445)
    bool bypassPitchLfo = true;
    
    //juce::dsp::Oscillator<float> volumelfo;

};