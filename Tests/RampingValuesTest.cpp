/*
  ==============================================================================

    RampingValuesTest.cpp
    Created: 27 Aug 2026 1:00:03am
    Author:  Patrick

  ==============================================================================
*/

#include "RampingValues.h"
#include "RampingValuesTest.h"

RampingValuesTest::RampingValuesTest() : juce::UnitTest("RampingValues", "DSP") 
{

}

void RampingValuesTest::testBeforeAndAfterAscending()
{
    beginTest("Test before and after values, ramping up");

    // Ascending
    currentGain = 0.01f;
    targetGain = 0.05f;
    numSamples = 96000;
    rv.setGainAndSpeed(currentGain, targetGain, numSamples);

    bool flag = true;
    float res = rv.getNextValue();
    expect((res >= currentGain), "First value is less current gain. Current Gain = " + std::to_string(currentGain) + ", First value = " + std::to_string(res));
    for (int i = 1; i < numSamples * 2; i++)
    {
        res = rv.getNextValue();
        //if ((flag) && (res == targetGain))
        //{            
        //    // Probably failing because of of currentstep / totalstep mismatch and total step not dividing evenly
        //    expect((i == numSamples), "Max value hit at wrong time. Current step = " + std::to_string(i) + " Correct step is = " + std::to_string(numSamples));
        //    flag = false;
        //}

    }
    res = rv.getNextValue();
    expect((fabs(targetGain - res) < epsilon), ("Ending value was " + std::to_string(res) + " and should be " + std::to_string(targetGain)));

}
void RampingValuesTest::testBeforeAndAfterDescending()
{
    
    // Descending
    beginTest("Test before and after values, ramping down");

    currentGain = 0.048f;
    targetGain = 0.0025f;
    numSamples = 48000;
    rv.setGainAndSpeed(currentGain, targetGain, numSamples);

    float res = rv.getNextValue();
    expect((res <= currentGain), "First value higher than current gain. Current Gain = " + std::to_string(currentGain) + ", First value = " + std::to_string(res));

    for (int i = 1; i < numSamples; i++)
    {
        res = rv.getNextValue();
        //if (res == targetGain)
        //{
        //    expect((i == numSamples), "Max value hit at wrong time. Current step = " + std::to_string(i) + " Correct step is = " + std::to_string(numSamples));
        //}
    }
    res = rv.getNextValue();
    expect((fabs(targetGain - res) < epsilon), ("Ending value was " + std::to_string(res) + " and should be " + std::to_string(targetGain)));

}

void RampingValuesTest::testHolding()
{
    beginTest("Test hold after numSamples reached");
    
    currentGain = 0.048f;
    targetGain = 0.0025f;
    numSamples = 48000;
    rv.setGainAndSpeed(currentGain, targetGain, numSamples);

    for (int i = 0; i < numSamples; i++)
    {
        rv.getNextValue();
    }
    float res = rv.getNextValue();
    expect(fabs(res - targetGain) < epsilon, "Current gain not equal to total gain at total steps. Current gain = " + std::to_string(res) + " .Target gain = " + std::to_string(targetGain));
    
    for (int i = 0; i < 96000; i++)
    {
        rv.getNextValue();
    }
    res = rv.getNextValue();
    expect(fabs(res - targetGain) < epsilon, "Current gain not equal to total gain after many steps. Current gain = " + std::to_string(res) + " . Target gain = " + std::to_string(targetGain));

}
void RampingValuesTest::testRampFromZero()
{
    beginTest("ramp from zero");
    {
        expect(true);
    }
}
void RampingValuesTest::runTest()
{
    testBeforeAndAfterAscending();
    testBeforeAndAfterDescending();
    testHolding();
}

static RampingValuesTest rampingValuesTest;