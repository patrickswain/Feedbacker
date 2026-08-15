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
    float osc1Gain;
    float osc1Freq;
    float osc1rampUpSpeed;
    bool addFeedback;
};

enum State
{ 
    IDLE, 
    RAMP_UP, 
    HOLD_NOTE, 
    NOTE_CHANGE 
};