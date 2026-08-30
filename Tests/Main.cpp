/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "RampingValuesTest.h"


//class MyFirstUnitTest final : public juce::UnitTest
//{
//public:
//    MyFirstUnitTest() : juce::UnitTest("MyFirstUnitTest") {}
//
//    void runTest()
//    {
//        beginTest("My first test");
//        {
//            expect(true);
//        }
//    }
//};
//
//static MyFirstUnitTest myUnitTest;

//==============================================================================
int main (int argc, char* argv[])
{

    juce::UnitTestRunner runner;
    runner.runAllTests();
    //runner.runTestsInCategory("DSP");

    return 0;
}
