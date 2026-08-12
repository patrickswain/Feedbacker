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


}

OscManager::~OscManager()
{

}

void OscManager::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    samplesPerBlock = spec.maximumBlockSize;

    // update individual oscillators
    osc1.prepare(spec);
}


