/*
  ==============================================================================

    MyOsc.cpp
    Created: 10 Aug 2026 7:51:22pm
    Author:  Patrick

  ==============================================================================
*/

#include "MyOsc.h"
#include <math.h>

MyOsc::MyOsc()
{
    maintone.initialise([](float x) {return sin(x);}, lookupTableSize);
    pitchLfo.initialise([](float x) {return 0.5 * sin(x);}, lookupTableSize);
}

MyOsc::~MyOsc()
{

}

void MyOsc::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    samplesPerBlock = spec.maximumBlockSize;

    maintone.prepare(spec);
    maintone.setFrequency(440.0f);

    pitchLfo.prepare(spec);
    pitchLfo.setFrequency(pitchLfoRate);
}

void MyOsc::setFrequency(float newFrequency)
{
    if (newFrequency != targetPitch)
    {
        targetPitch = newFrequency;
        maintone.setFrequency(targetPitch);
    }
}

void MyOsc::setGain(float newGain) // Used for OscManager to set
{
    if (newGain != targetGain) // revent reseting when settings are constanly updated
    {
        targetGain = newGain;
        //smoothedGain.reset(sampleRate, rampUpSpeed);
        smoothedGain.setCurrentAndTargetValue(currentGain);
        smoothedGain.setTargetValue(targetGain);
    }
}

void MyOsc::updateGain() // Used to reset smoothed gain when in idle state
{    
        smoothedGain.reset(sampleRate, rampUpSpeed);
        smoothedGain.setCurrentAndTargetValue(0.0001f);
        smoothedGain.setTargetValue(targetGain);    
}

void MyOsc::setRampUpSpeed(float newRampUpSpeed)
{
    if (newRampUpSpeed != rampUpSpeed) // prevent reseting since settings are constanly updated
    {
        rampUpSpeed = static_cast<double>(newRampUpSpeed);
        smoothedGain.reset(sampleRate, rampUpSpeed);
    }
}

void MyOsc::setState(State newState)
{
    currentState = newState;
}

