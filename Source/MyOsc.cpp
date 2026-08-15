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
    targetGain = newGain;
    DBG("Ramp up in myosc = " << rampUpSpeed);
    smoothedGain.reset(sampleRate, rampUpSpeed);
    smoothedGain.setCurrentAndTargetValue(0.0001f);
    smoothedGain.setTargetValue(targetGain);

}

void MyOsc::setRampUpSpeed(float newRampUpSpeed)
{
    smoothedGain.reset(sampleRate, static_cast<double>(newRampUpSpeed));
}

void MyOsc::setState(State newState)
{
    currentState = newState;
}
