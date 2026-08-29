/*
  ==============================================================================

    RampingValues.cpp
    Created: 26 Aug 2026 4:09:54pm
    Author:  Patrick

  ==============================================================================
*/

#include "RampingValues.h"

void RampingValues::setGainAndSpeed(float newStartingGain, float newTargetGain, float rampUpSpeedInSamples)
{
    // difference in gain = amplitude of hann function
    // rampupspeed = numsamples in hann funtion
    currentStep = 0;
    totalSteps = rampUpSpeedInSamples;
    startingGain = newStartingGain;
    targetGain = newTargetGain;
    gainDifference = targetGain - startingGain;
}

/* 
This function doesn't work like juce::smoothedvalue
Use buffer[i] * gain.getNextValue()
instead of
buffer[i] * currentGain; currentGain += gain.getNextValue()
*/
float RampingValues::getNextValue()
{
    if (currentStep > totalSteps)
    {
        return targetGain;
    }

    // 0.5 * ( 1 - (cos((2 * pi * n) / T)) // n is current x axis location, T is total period
    // 0.5 is to convert -1 to 1 cosign to -0.5 to 0.5, 
    // and 1 - is to shift values up to 
    // This function goes up, then down across the period of 0 to T - 1
    // So we need to double the period, to only get ramp up or ramp down
    // and rampup goes 0 to midpoint

    // cos amplitude is double, so have to half the difference of the two gains, and shift the starting point up by half of the distance
    // dont need extra expression for ramping down bc gaindiff will be negative
    return (startingGain + (targetGain - startingGain) * 0.5f * (1.0f - std::cos((2 * juce::MathConstants<float>::pi * currentStep++) / (2 * totalSteps))));
    
}
