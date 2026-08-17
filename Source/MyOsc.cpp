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
}

void MyOsc::setFrequency(float newFrequency)
{
    targetPitch = newFrequency;
}

void MyOsc::setGain(float newGain)
{
    if (newGain != targetGain)
    {
        targetGain = newGain;
        smoothedGain.reset(sampleRate, rampUpSpeed);
        smoothedGain.setCurrentAndTargetValue(0.0001f);
        smoothedGain.setTargetValue(targetGain);
    }
}

void MyOsc::setRampUpSpeed(float newRampUpSpeed)
{
    if (newRampUpSpeed != rampUpSpeed)
    {
        smoothedGain.reset(sampleRate, static_cast<double>(newRampUpSpeed));
        rampUpSpeed = newRampUpSpeed;
    }
}

void MyOsc::setState(State newState)
{
    currentState = newState;
}

void MyOsc::updateGain()
{
    smoothedGain.reset(sampleRate, rampUpSpeed);
    smoothedGain.setCurrentAndTargetValue(0.0001f);
    smoothedGain.setTargetValue(targetGain);
}