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
        std::string af = addFeedback ? "true" : "false";
        DBG("State in manager = " << currentState);
        DBG("addFeedback in manager = " << af);
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

    bool addFeedback = false;
    
    State currentState = IDLE;
    
    int rampUpTime = 0;
    int rampUpTimeCurrent = 0; //(increment in samples)

    int holdTime;
    int holdTimeCurrent;//(increment in samples)

    int noteChangeTime;
    int noteChangeTimeCurrent;//(increment in samples)
};