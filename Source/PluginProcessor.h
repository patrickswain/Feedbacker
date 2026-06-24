/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct TriggerThresholdParam
{
    static constexpr auto id = "triggerThreshold";
    static constexpr auto name = "Trigger Threshold (db)";
    inline static const juce::NormalisableRange<float> range = { -30.0f, 0.0f };
    static constexpr float defaultValue = { -20.0f };
};

struct SynthVolumeParam
{
    static constexpr auto id = "synthVolume";
    static constexpr auto name = "Synth Volume";
    inline static const juce::NormalisableRange<float> range = { 0.0f, 1.0f };;
    static constexpr float defaultValue = { 0.5f };
};

struct SynthFrequencyParam
{
    static constexpr auto id = "synthFrequency";
    static constexpr auto name = "Synth Freq";
    static juce::StringArray choices()
    {
        return { "200", "500", "1000", "1500", "2000" };
    }
    static constexpr float defaultValue = 0;
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

    float rampUpSpeed = 0.0f;
    //==============================================================================
    // For Trigger
    bool addFeedback = false;
    float triggerThreshold = 0.0f;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FeedbackerAudioProcessor)
};
