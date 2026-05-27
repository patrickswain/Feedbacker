/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct ParamIDs
{
    static constexpr auto triggerThreshold = "triggerThreshold";
};

struct ParamNames
{
    static constexpr auto triggerThreshold = "Trigger Threshold";
};

struct ParamSettings
{
    float triggerThreshold = { 0.5f };
};

struct ParamRanges
{
    inline static const juce::NormalisableRange<float> triggerThreshold = { 0.0f, 1.5f };
};

ParamSettings getParamSettings(juce::AudioProcessorValueTreeState& apvts);
//==============================================================================
/**
*/
class FeedbackerAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    FeedbackerAudioProcessor();
    ~FeedbackerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts;
    //==============================================================================
    void updateAngleDelta();
    
private:
    //==============================================================================
    // For Sine Wave
    double currentSampleRate = 0.0;
    double currentAngle = 0.0;
    double angleDelta = 0.0;
    double oscFrequency = 440.0;
    double oscLevel = 0.5;
    //==============================================================================
    // For Trigger
    bool addFeedback = false;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FeedbackerAudioProcessor)
};
