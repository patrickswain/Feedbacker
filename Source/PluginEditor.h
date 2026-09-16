/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"
#include "Parameters.h"

//==============================================================================
/**
*/
class FeedbackerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FeedbackerAudioProcessorEditor (FeedbackerAudioProcessor&);
    ~FeedbackerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    std::vector<juce::Component*> getComps();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FeedbackerAudioProcessor& audioProcessor;

    int labelHeight = 20;

    juce::Slider gainSlider, thresholdSlider, rampUpSpeedSlider; // knobs
    juce::Slider osc1FreqSlider, 
        osc2FreqSlider, 
        osc3FreqSlider, 
        osc4FreqSlider,
        osc1HoldTimeSlider, 
        osc2HoldTimeSlider, 
        osc3HoldTimeSlider, 
        osc4HoldTimeSlider;

    juce::ToggleButton osc1BypassButton, osc2BypassButton, osc3BypassButton, osc4BypassButton;

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment gainSliderAttachment,
        thresholdSliderAttachment,
        rampUpSpeedSliderAttachment,
        osc1FreqSliderAttachment, 
        osc2FreqSliderAttachment, 
        osc3FreqSliderAttachment, 
        osc4FreqSliderAttachment,
        osc1HoldTimeSliderAttachment, 
        osc2HoldTimeSliderAttachment, 
        osc3HoldTimeSliderAttachment, 
        osc4HoldTimeSliderAttachment;

    using ButtonAttachment = APVTS::ButtonAttachment;

    ButtonAttachment osc1BypassButtonAttachment, 
        osc2BypassButtonAttachment, 
        osc3BypassButtonAttachment, 
        osc4BypassButtonAttachment;

    juce::Label rampUpSpeedLabel, thresholdLabel, gainLabel, note1Label, note2Label, note3Label, note4Label;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FeedbackerAudioProcessorEditor)
};


