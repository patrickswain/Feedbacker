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
    static constexpr auto synthFrequency = "synthFrequency";
    static constexpr auto synthVolume = "synthVolume";
};

struct ParamNames
{
    static constexpr auto triggerThreshold = "Trigger Threshold (db)";
    static constexpr auto synthFrequency = "Synth Freq";
    static constexpr auto synthVolume = "Synth Volume";
};

struct ParamSettings
{
    float triggerThreshold = { 0.5f };
    int synthFrequency = { 1000 };
    //float synthFrequency = { 1000.0f };
    float synthVolume = { 0.5f };
};

struct ParamRanges
{
    inline static const juce::NormalisableRange<float> triggerThreshold = { -30.0f, 0.0f };
    //inline static const juce::NormalisableRange<float> synthFrequency = { 200.0f, 5000.0f };
    inline static const juce::NormalisableRange<float> synthVolume = { 0.0f, 1.0f };
};


struct ParamDefaultValues
{
    static constexpr float triggerThreshold = { -20.0f };
    static constexpr int synthFrequency = { 1000 };
    //static constexpr float synthFrequency = { 1000.0f };
    static constexpr float synthVolume = { 0.5f };
};


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
    void getParamSettings(juce::AudioProcessorValueTreeState& apvts);
    void updateAngleDelta();
    
private:
    //==============================================================================
    // For Sine Wave
    double currentSampleRate = 0.0;
    double currentAngle = 0.0;
    double angleDelta = 0.0;
    double oscFrequency = 440.0;
    double oscLevel = 0.25;
    //==============================================================================
    // For Trigger
    bool addFeedback = false;
    float triggerThreshold = 0.0f;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FeedbackerAudioProcessor)
};
