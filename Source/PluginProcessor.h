/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MyOsc.h"
#include "OscManager.h"
#include "ParamSettings.h"


struct TriggerThresholdParam
{
    static constexpr auto id = "triggerThreshold";
    static constexpr auto name = "Trigger Threshold (db)";
    inline static const juce::NormalisableRange<float> range = { -100.0f, 0.0f };
    static constexpr float defaultValue = { -95.0f };
};

struct RampUpSpeedParam
{
    static constexpr auto id = "rampUpSpeed";
    static constexpr auto name = "Ramp Up Speed (milliseconds)";
    inline static const juce::NormalisableRange<float> range = { 50.0f, 4000.0f }; // Double check for overflow problems
    static constexpr float defaultValue = { 1000.0f };
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
    inline static const juce::NormalisableRange<float> range = { 40.0f, 5000.0f };;
    static constexpr float defaultValue = { 440.0f };
};

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
    void updateGain();
private:
    //==============================================================================
    // For Sine Wave
    double currentSampleRate = 0.0;
    double oscFrequency = 440.0;
    double oscLevel = 0.25;

    ParamSettings settings;
    OscManager oscManager;
    
    float triggerThreshold = 0.0f;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FeedbackerAudioProcessor)
};
