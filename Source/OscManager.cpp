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

void OscManager::updateSettings(const ParamSettings& settings) 
{
    addFeedback = settings.addFeedback;

    osc1.setFrequency(settings.osc1Freq);
    osc1.setRampUpSpeed(settings.osc1rampUpSpeedSeconds);
    osc1.setGain(settings.osc1Gain);
}

void OscManager::setOscillatorsState(State newState)
{
    // for osc : oscs
    osc1.setState(newState);
}
