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
    struct OscillatorControls 
    {
        OscillatorControls(juce::AudioProcessorValueTreeState& apvts,
            const juce::String& freqParamId,
            const juce::String& holdTimeParamId,
            const juce::String& bypassParamId,
            const juce::String& noteLabel)
            : freqSlider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextBoxAbove),
            holdTimeSlider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextBoxAbove),
            bypassButton("Bypass"),
            freqSliderAttachment(apvts, freqParamId, freqSlider),
            holdTimeSliderAttachment(apvts, holdTimeParamId, holdTimeSlider),
            bypassButtonAttachment(apvts, bypassParamId, bypassButton)

        {
            label.setText(noteLabel, juce::NotificationType::dontSendNotification);
            label.setJustificationType(juce::Justification::centredTop);
        }

        juce::Slider freqSlider, holdTimeSlider;
        juce::ToggleButton bypassButton;
        juce::Label label;

        juce::AudioProcessorValueTreeState::SliderAttachment freqSliderAttachment, holdTimeSliderAttachment;        
        juce::AudioProcessorValueTreeState::ButtonAttachment bypassButtonAttachment;
        
        std::vector<juce::Component*> getComponents()
        {
            return { &freqSlider, &holdTimeSlider, &bypassButton, &label };
        }

        void layoutComponents(juce::Rectangle<int> area, int labelHeight)
        {
            bypassButton.setBounds(area.removeFromRight(area.getWidth() / 4));
            label.setBounds(area.removeFromTop(labelHeight));
            freqSlider.setBounds(area.removeFromTop(area.getHeight() / 2));
            holdTimeSlider.setBounds(area);
        }
    };

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FeedbackerAudioProcessor& audioProcessor;

    static constexpr int labelHeight = 20;

    juce::Slider gainSlider, thresholdSlider, rampUpSpeedSlider; // knobs

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment gainSliderAttachment,
        thresholdSliderAttachment,
        rampUpSpeedSliderAttachment;
        
    juce::Label rampUpSpeedLabel, thresholdLabel, gainLabel;

    static constexpr int numOscillators = 4;
    std::array<OscillatorControls, numOscillators> oscillators;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FeedbackerAudioProcessorEditor)
};


