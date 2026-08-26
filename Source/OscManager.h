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
                setRampUp();
            }
            break;
        case RAMP_UP:
            // Switch to idle
            if (!addFeedback)
            {
                setIdle();
                break;
            }

            rampUpTimeCurrent += numSamples;

            // Switch to Hold Note
            if (rampUpTimeCurrent >= rampUpTime)
            {
                setHoldNote();
                break;
            }
            break;
        case HOLD_NOTE:
            // Switch to idle
            if (!addFeedback)
            {
                setIdle();
                break;
            }

            holdTimeCurrent += numSamples; // May need to go after next if statement

            // Switch to Note Change
            if (holdTimeCurrent >= holdTime)
            {
                setNoteChange();
                break;
            }
            break;
        case NOTE_CHANGE:
            // Switch to idle
            if (!addFeedback)
            {
                setIdle();
                break;
            }

            noteChangeTimeCurrent += numSamples;

            // Switch to Hold Note
            if (noteChangeTimeCurrent >= noteChangeTime)
            {
                setHoldNote();
            }           
            break;
        default:
            break;
        }

        osc1.process(context);
        osc2.process(context);
    }

    void updateSettings(const ParamSettings& settings);
    void setOscillatorsState(State state);
    void setIdle();
    void setRampUp();
    void setHoldNote();
    void setNoteChange();
 
private:
    double sampleRate;
    int samplesPerBlock;
    int numSamples = 0;
    
    std::vector<MyOsc> oscs;
    MyOsc osc1, osc2;
    float osc1Gain = 0.0f;
    float osc2Gain = 0.0f;

    bool secondNote = false;
    bool addFeedback = false;
    
    State currentState = IDLE;
    
    int rampUpTime = 0;
    int rampUpTimeCurrent = 0; //(increment in samples)

    int holdTime; // set in prepare bc needs sample rate (needs to be updated when these are parameters)
    int holdTimeCurrent = 0; //(increments in samples)

    int noteChangeTime; // set in prepare bc needs sample rate (needs to be updated when these are parameters)
    int noteChangeTimeCurrent = 0; //(increments in samples)

    int debugcounter = 0;
};