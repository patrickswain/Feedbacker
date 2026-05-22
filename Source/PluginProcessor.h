/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct ParamIDs
{
    static constexpr auto a0 = "a0";
    static constexpr auto a1 = "a1";
    static constexpr auto b1 = "b1";
};

struct ParamNames
{
    static constexpr auto a0 = "a0";
    static constexpr auto a1 = "a1";
    static constexpr auto b1 = "b1";
};

struct ParamSettings
{
    float a0 = { 0.5f };
    float a1 = { 0.5f };
    float b1 = { 0.5f };
};

struct ParamRanges
{
    inline static const juce::NormalisableRange<float> a0 = { -1.0f, 1.0f };
    inline static const juce::NormalisableRange<float> a1 = { -1.0f, 1.0f };
    inline static const juce::NormalisableRange<float> b1 = { -1.5f, 1.5f };
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
private:

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FeedbackerAudioProcessor)
};
