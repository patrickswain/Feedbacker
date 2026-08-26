/*
  ==============================================================================

    OscManager.cpp
    Created: 12 Aug 2026 1:05:49pm
    Author:  Patrick

  ==============================================================================
*/

#include "OscManager.h"


OscManager::OscManager()
{
    //oscs.push_back(osc1);
    //oscs.push_back(osc2);
}

OscManager::~OscManager()
{
    //oscs.clear();
}

void OscManager::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    samplesPerBlock = spec.maximumBlockSize;

    // update individual oscillators
    osc1.prepare(spec);
    osc2.prepare(spec);
}

void OscManager::updateSettings(const ParamSettings& settings) 
{
    numSamples = settings.numSamples;
    addFeedback = settings.addFeedback;

    osc1.setFrequency(settings.osc1Freq);
    osc1.setRampUpSpeed(settings.osc1rampUpSpeedSeconds);

    osc2.setFrequency(settings.osc1Freq + 200.0f);
    osc2.setRampUpSpeed(settings.osc1rampUpSpeedSeconds);

    if (osc1Gain != settings.osc1Gain) // prevent resetting values during note change
    {
        osc1.setGain(settings.osc1Gain);
        osc2.setGain(osc1Gain / 10.0); // Half volume in db
        osc1Gain = settings.osc1Gain;
    }

    
    rampUpTime = settings.osc1rampUpSpeedSeconds * static_cast<int>(sampleRate); // For state changing timing
    holdTime = 2 * static_cast<int>(sampleRate);
    noteChangeTime = 2 * static_cast<int>(sampleRate);

}

void OscManager::setOscillatorsState(State newState)
{    
    osc1.setState(newState);
    osc2.setState(newState);
}

void OscManager::setIdle()
{
    currentState = IDLE;
    setOscillatorsState(IDLE);
}

void OscManager::setRampUp()
{
    currentState = RAMP_UP;
    setOscillatorsState(RAMP_UP);
    rampUpTimeCurrent = 0;
}

void OscManager::setHoldNote()
{
    currentState = HOLD_NOTE;
    setOscillatorsState(HOLD_NOTE);
    holdTimeCurrent = 0;
}

void OscManager::setNoteChange()
{
    currentState = NOTE_CHANGE;
    setOscillatorsState(NOTE_CHANGE);
    noteChangeTimeCurrent = 0;
    if (secondNote) // replace this with reading from files
    {
        osc1.setRampUpSpeed(noteChangeTime);
        osc1.setGain(osc1Gain);

        osc2.setRampUpSpeed(noteChangeTime);
        osc2.setGain(osc1Gain / 10);

    }
    else
    {
        osc1.setRampUpSpeed(noteChangeTime);
        osc1.setGain(osc1Gain / 10);

        osc2.setRampUpSpeed(noteChangeTime);
        osc2.setGain(osc1Gain);

    }
    secondNote = secondNote ? false : true; // toggle which note
}