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
}

