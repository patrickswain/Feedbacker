/*
  ==============================================================================

    RampingValuesTest.h
    Created: 27 Aug 2026 1:00:03am
    Author:  Patrick

  ==============================================================================
*/

#pragma once

#include <juce_core/juce_core.h>
#include "RampingValues.h"

class RampingValuesTest : juce::UnitTest
{
public:

    RampingValuesTest();
    void testSettingFunction();
    void testBeforeAndAfterAscending();
    void testBeforeAndAfterDescending();

    void testAscending();
    void testDescending();
    void testHolding();
    void testRampFromZero();
    void runTest() override;
    
private:
    RampingValues rv;
    double sampleRate = 48000;
    float currentGain;
    float targetGain;
    float epsilon = 0.00001f;
    int numSamples;
};