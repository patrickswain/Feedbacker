/*
  ==============================================================================

    OscManager.h
    Created: 12 Aug 2026 1:06:02pm
    Author:  Patrick

  ==============================================================================
*/

#pragma once

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
        
        //if (debugcounter % 20) // limit dbg writes
        //{
        //    //std::string af = addFeedback ? "true" : "false";
        //    DBG("State in manager = " << currentState);
        //    //DBG("addFeedback in manager = " << af);
        //}
        //debugcounter++;

        switch (currentState)
        {
        case IDLE:
            // Switch to Ramp up
            if (settings.addFeedback)
            {
                setRampUp();
                break;
            }
            break;
        case RAMP_UP:
            // Switch to idle
            if (!settings.addFeedback)
            {
                setIdle();
                break;
            }            

            // Switch to Hold Note
            if (rampUpTimeCurrent >= rampUpTime)
            {
                setHoldNote();
                break;
            }

            rampUpTimeCurrent += settings.numSamples;

            break;
        case HOLD_NOTE:
            // Switch to idle
            if (!settings.addFeedback)
            {
                setIdle();
                break;
            }            

            // Switch to Note Change
            if (holdTimeCurrent >= holdTime)
            {
                setNoteChange();
                break;
            }
            DBG("Hold time current = " << holdTimeCurrent << " . Total hold time = " << holdTime);
            holdTimeCurrent += settings.numSamples; // May need to go after next if statement
            break;
        case NOTE_CHANGE:
            // Switch to idle
            if (!settings.addFeedback)
            {
                setIdle();
                break;
            }

            // Switch to Hold Note
            if (noteChangeTimeCurrent >= noteChangeTime)
            {
                setHoldNote();
                break;
            }           

            noteChangeTimeCurrent += settings.numSamples;
            break;
        default:
            break;
        }

        if (!settings.osc1Bypass)
        {
            osc1.process(context);
        }
        if (!settings.osc2Bypass)
        {
            osc2.process(context);
        }
        if (!settings.osc3Bypass)
        {
            osc3.process(context);
        }
        if (!settings.osc4Bypass)
        {
            osc4.process(context);
        }
        
    }

    enum Note {
        NOTE_1,
        NOTE_2,
        NOTE_3,
        NOTE_4
    };
    void updateSettings(const ParamSettings& settings);
    void setOscillatorsState(State state);
    void setIdle();
    void setRampUp();
    void setHoldNote();
    void setNoteChange();
    void updateOscillatorNoteChange(float rampTime, Note currentNote);
    
private:
    
    double sampleRate;
    int samplesPerBlock;
    
    MyOsc osc1, osc2, osc3, osc4;
    std::array<MyOsc*, 4> oscs;

    ParamSettings settings;
    bool secondNote = false;

    State currentState = IDLE;
    Note note = NOTE_1;

    int rampUpTime = 0;
    int rampUpTimeCurrent = 0;

    int holdTime = 0;
    int holdTimeCurrent = 0;

    int noteChangeTime = 0;
    int noteChangeTimeCurrent = 0;


    int debugcounter = 0;
};