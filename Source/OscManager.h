/*
  ==============================================================================

    OscManager.h
    Created: 12 Aug 2026 1:06:02pm
    Author:  Patrick

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MyOsc.h"
#include "ParamSettings.h"

class OscManager
{
public:
    OscManager();
    ~OscManager();

    void prepare(const juce::dsp::ProcessSpec& spec);

    template <typename ProcessContext>
    void process(const ProcessContext& context)
    {
        switch (currentState)
        {
        case IDLE:
            if (addFeedback)
            {
                currentState = RAMP_UP;
                setOscillatorsState(RAMP_UP); 
            }
            break;
        case RAMP_UP:
            if (!addFeedback)
            {
                currentState = IDLE;
                setOscillatorsState(IDLE);
            }
            break;
        case HOLD_NOTE:
            break;
        case NOTE_CHANGE:
            break;
        default:
            break;
        }

        osc1.process(context);
    }

    void updateSettings(const ParamSettings& settings);
    void setOscillatorsState(State state);

 
private:
    double sampleRate;
    int samplesPerBlock;
    

    MyOsc osc1;
    
    //Oscillator Osc1;
    //Oscillator Osc2;
    //vector oscillators
    bool addFeedback;
    
    State currentState = RAMP_UP;
    
    float rampUpTime;
    float rampUpTimeCurrent; //(increment in samples)

    float holdTime;
    float holdTimeCurrent;//(increment in samples)

    float noteChangeTime;
    float noteChangeTimeCurrent;//(increment in samples)
};