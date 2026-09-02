/*
  ==============================================================================

    OscManager.cpp
    Created: 12 Aug 2026 1:05:49pm
    Author:  Patrick

  ==============================================================================
*/

#include "OscManager.h"


OscManager::OscManager() : oscs {&osc1, &osc2, &osc3, &osc4}
{

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
    osc3.prepare(spec);
    osc4.prepare(spec);
}

void OscManager::updateSettings(const ParamSettings& newSettings) 
{
    rampUpTime = newSettings.rampUpSpeed;
    holdTime = newSettings.osc1HoldTime;
    noteChangeTime = newSettings.osc1NoteChangeTime;

    osc1.setFrequency(newSettings.osc1Freq);
    osc2.setFrequency(newSettings.osc2Freq);
    osc3.setFrequency(newSettings.osc3Freq);
    osc4.setFrequency(newSettings.osc4Freq);

    if (newSettings.rampUpSpeed != settings.rampUpSpeed)
    {
        osc1.setRampUpSpeed(newSettings.rampUpSpeed);
        osc2.setRampUpSpeed(newSettings.rampUpSpeed);
        osc3.setRampUpSpeed(newSettings.rampUpSpeed);
        osc4.setRampUpSpeed(newSettings.rampUpSpeed);
    }   

    if (newSettings.gain != settings.gain)
    {
        osc1.setGain(newSettings.gain);
        osc2.setGain(newSettings.gain * juce::Decibels::decibelsToGain(-6.0f));
        osc3.setGain(newSettings.gain * juce::Decibels::decibelsToGain(-6.0f));
        osc4.setGain(newSettings.gain * juce::Decibels::decibelsToGain(-6.0f));
    }

    settings = newSettings;

}

void OscManager::setOscillatorsState(State newState)
{    
    osc1.setState(newState);
    osc2.setState(newState);
    osc3.setState(newState);
    osc4.setState(newState);
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
    DBG("Set from setRampUp()");
    osc1.setGain(settings.gain);
    osc2.setGain(settings.gain * juce::Decibels::decibelsToGain(-18.0f));
    osc3.setGain(settings.gain * juce::Decibels::decibelsToGain(-18.0f));
    osc4.setGain(settings.gain * juce::Decibels::decibelsToGain(-18.0f));
}

void OscManager::setHoldNote()
{
    currentState = HOLD_NOTE;
    setOscillatorsState(HOLD_NOTE);
    holdTimeCurrent = 0;
}

void OscManager::updateOscillatorNoteChange(float rampTime, Note currentNote)
{
    float smallerGain = settings.gain * juce::Decibels::decibelsToGain(-18.0f);
    for (int i = 0; i < 4; i++)
    {
        oscs[i]->setRampUpSpeed(rampTime);
        if (i == currentNote)
        {
            oscs[i]->setGain(settings.gain);
        }
        else
        {
            oscs[i]->setGain(smallerGain);
        }
    }
}
void OscManager::setNoteChange()
{
    currentState = NOTE_CHANGE;
    setOscillatorsState(NOTE_CHANGE);
    noteChangeTimeCurrent = 0;
    DBG("Set from setNoteChange");

    // Determines which note to switch to, not current note
    switch(note)
    {
    case NOTE_1:
        updateOscillatorNoteChange(settings.osc2NoteChangeTime, NOTE_2);
        note = NOTE_2;
        break;
    case NOTE_2:
        updateOscillatorNoteChange(settings.osc3NoteChangeTime, NOTE_3);
        note = NOTE_3;
        break;
    case NOTE_3:
        updateOscillatorNoteChange(settings.osc4NoteChangeTime, NOTE_4);
        note = NOTE_4;
        break;
    case NOTE_4:
        updateOscillatorNoteChange(settings.osc1NoteChangeTime, NOTE_1);
        note = NOTE_1;
        break;
    }
    

    //if (secondNote) // replace this with reading from files
    //{
    //    // noteChangeTime = 
    //    osc1.setRampUpSpeed(settings.rampUpSpeed);
    //    osc1.setGain(settings.gain);

    //    osc2.setRampUpSpeed(settings.rampUpSpeed);
    //    osc2.setGain(settings.gain * juce::Decibels::decibelsToGain(-18.0f));
    //}
    //else
    //{
    //    osc1.setRampUpSpeed(settings.rampUpSpeed);
    //    osc1.setGain(settings.gain * juce::Decibels::decibelsToGain(-18.0f));

    //    osc2.setRampUpSpeed(settings.rampUpSpeed);
    //    osc2.setGain(settings.gain);

    //}
    //secondNote = secondNote ? false : true; // toggle which note
}