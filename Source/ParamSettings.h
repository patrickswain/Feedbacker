/*
  ==============================================================================

    ParamSettings.h
    Created: 12 Aug 2026 7:28:47pm
    Author:  Patrick

  ==============================================================================
*/

#pragma once

struct ParamSettings
{
    float gain;
    float rampUpSpeed;
    int numSamples;
    bool addFeedback;

    float osc1Freq;
    float osc1HoldTime;
    float osc1NoteChangeTime;
    bool osc1Bypass;

    float osc2Freq;
    float osc2HoldTime;
    float osc2NoteChangeTime;
    bool osc2Bypass;

    float osc3Freq;
    float osc3HoldTime;
    float osc3NoteChangeTime;
    bool osc3Bypass;

    float osc4Freq;
    float osc4HoldTime;
    float osc4NoteChangeTime;
    bool osc4Bypass;
};

enum State
{ 
    IDLE, 
    RAMP_UP, 
    HOLD_NOTE, 
    NOTE_CHANGE 
};