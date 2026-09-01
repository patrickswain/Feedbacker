/*
  ==============================================================================

    RampingValues.h
    Created: 26 Aug 2026 4:09:42pm
    Author:  Patrick

  ==============================================================================
*/

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

class RampingValues
{
public:
    enum RampType
    {
        HannFunction,
        Linear,
        DecibelLinear,
        DecibelHann
    };
    void setRampingType(RampType type);
    void setGainAndSpeed(float currentGain, float targetGain, float rampUpSpeedInSamples);    
    float getNextValue();

private:
    double sampleRate = 48000.0;
    float gainDifference = 0.0f;
    float startingGain = 0.0f;
    float targetGain = 0.0f;
    float startingDB = 0.0f;
    float targetDB = 0.0f;
    int currentStep = 0;
    int totalSteps = 0;
    RampType rampType = Linear;
};