/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "MyOsc.h"
#include "OscManager.h"
#include <Parameters.h>
#include "ParamSettings.h"


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
    float decibelsBeforeFeedbackStarts = -50.0f;
    bool firstNoteWasPlayed = true;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FeedbackerAudioProcessor)
};
