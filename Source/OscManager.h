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
        
        if (debugcounter % 20) // limit dbg writes
        {
            std::string af = addFeedback ? "true" : "false";
            DBG("State in manager = " << currentState);
            DBG("addFeedback in manager = " << af);
        }
        debugcounter++;

        switch (currentState)
        {
        case IDLE:
            // Switch to Ramp up
            if (addFeedback)
            {
                currentState = RAMP_UP;
                setOscillatorsState(RAMP_UP); 
                rampUpTimeCurrent = 0;
            }
            break;
        case RAMP_UP:
            // Switch to idle
            if (!addFeedback)
            {
                currentState = IDLE;
                setOscillatorsState(IDLE);
                break;
            }

            rampUpTimeCurrent += numSamples;

            // Switch to Hold Note
            if (rampUpTimeCurrent > rampUpTime)
            {
                currentState = HOLD_NOTE;
                setOscillatorsState(HOLD_NOTE);
                holdTimeCurrent = 0;
                break;
            }
            break;
        case HOLD_NOTE:
            osc1.setFrequency(dummyHoldFrequency);

            // Switch to idle
            if (!addFeedback)
            {
                currentState = IDLE;
                setOscillatorsState(IDLE);
                rampUpTimeCurrent = 0;
                break;
            }

            holdTimeCurrent += numSamples; // May need to go after next if statement

            // Switch to Note Change
            //if (holdTimeCurrent > holdTime)
            //{
            //    currentState = NOTE_CHANGE;
            //    setOscillatorsState(NOTE_CHANGE);
            //    noteChangeTimeCurrent = 0;
            //}
            break;
        case NOTE_CHANGE:
            //osc1.setFrequency(dummyTestFrequency);
            //dummyTestFrequency += 50.0;

            //// Switch to idle
            //if (!addFeedback)
            //{
            //    currentState = IDLE;
            //    setOscillatorsState(IDLE);
            //}

            //noteChangeTimeCurrent += numSamples;

            //// Switch to Hold Note
            //if (noteChangeTimeCurrent >= noteChangeTime)
            //{
            //    currentState = HOLD_NOTE;
            //    setOscillatorsState(HOLD_NOTE);
            //    holdTimeCurrent = 0;
            //}
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
    int numSamples = 0;
    
    MyOsc osc1;

    bool addFeedback = false;
    
    State currentState = IDLE;
    
    int rampUpTime = 0;
    int rampUpTimeCurrent = 0; //(increment in samples)

    int holdTime = 2 * static_cast<int>(sampleRate);
    int holdTimeCurrent; //(increments in samples)

    int noteChangeTime = 2 * static_cast<int>(sampleRate);
    int noteChangeTimeCurrent = 0; //(increments in samples)
    float dummyTestFrequency = 500.0f;
    float dummyHoldFrequency = 440.0f;
    int debugcounter = 0;
};