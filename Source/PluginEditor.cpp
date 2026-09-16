/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//==============================================================================
FeedbackerAudioProcessorEditor::FeedbackerAudioProcessorEditor(FeedbackerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    gainSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow),
    gainSliderAttachment(audioProcessor.apvts, SynthVolumeParam::id, gainSlider),
    thresholdSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow),
    thresholdSliderAttachment(audioProcessor.apvts, TriggerThresholdParam::id, thresholdSlider),
    rampUpSpeedSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow),
    rampUpSpeedSliderAttachment(audioProcessor.apvts, RampUpSpeedParam::id, rampUpSpeedSlider),

    osc1FreqSlider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextBoxAbove),
    osc1FreqSliderAttachment(audioProcessor.apvts, Osc1FrequencyParam::id, osc1FreqSlider),
    osc2FreqSlider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextBoxAbove),
    osc2FreqSliderAttachment(audioProcessor.apvts, Osc2FrequencyParam::id, osc2FreqSlider),
    osc3FreqSlider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextBoxAbove),
    osc3FreqSliderAttachment(audioProcessor.apvts, Osc3FrequencyParam::id, osc3FreqSlider),
    osc4FreqSlider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextBoxAbove),
    osc4FreqSliderAttachment(audioProcessor.apvts, Osc4FrequencyParam::id, osc4FreqSlider),

    osc1HoldTimeSlider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextBoxAbove),
    osc1HoldTimeSliderAttachment(audioProcessor.apvts, Osc1HoldTimeParam::id, osc1HoldTimeSlider),
    osc2HoldTimeSlider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextBoxAbove),
    osc2HoldTimeSliderAttachment(audioProcessor.apvts, Osc2HoldTimeParam::id, osc2HoldTimeSlider),
    osc3HoldTimeSlider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextBoxAbove),
    osc3HoldTimeSliderAttachment(audioProcessor.apvts, Osc3HoldTimeParam::id, osc3HoldTimeSlider),
    osc4HoldTimeSlider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextBoxAbove),
    osc4HoldTimeSliderAttachment(audioProcessor.apvts, Osc4HoldTimeParam::id, osc4HoldTimeSlider),

    osc1BypassButton("Bypass Note 1"),
    osc1BypassButtonAttachment(audioProcessor.apvts, Osc1BypassParam::id, osc1BypassButton),
    osc2BypassButton("Bypass Note 2"),
    osc2BypassButtonAttachment(audioProcessor.apvts, Osc2BypassParam::id, osc2BypassButton),
    osc3BypassButton("Bypass Note 3"),
    osc3BypassButtonAttachment(audioProcessor.apvts, Osc3BypassParam::id, osc3BypassButton),
    osc4BypassButton("Bypass Note 4"),
    osc4BypassButtonAttachment(audioProcessor.apvts, Osc4BypassParam::id, osc4BypassButton)
{
    for (Component* comp : getComps())
    {
        addAndMakeVisible(comp);
    }
    
    rampUpSpeedLabel.setText("Ramp Up Speed", juce::NotificationType::dontSendNotification);
    thresholdLabel.setText("Threshold", juce::NotificationType::dontSendNotification);
    gainLabel.setText("Feedback Gain", juce::NotificationType::dontSendNotification);
    note1Label.setText("Note 1", juce::NotificationType::dontSendNotification);
    note2Label.setText("Note 2", juce::NotificationType::dontSendNotification);
    note3Label.setText("Note 3", juce::NotificationType::dontSendNotification);
    note4Label.setText("Note 4", juce::NotificationType::dontSendNotification);

    rampUpSpeedLabel.setJustificationType(juce::Justification::centredTop);
    thresholdLabel.setJustificationType(juce::Justification::centredTop);
    gainLabel.setJustificationType(juce::Justification::centredTop);
    note4Label.setJustificationType(juce::Justification::centredTop);
    note1Label.setJustificationType(juce::Justification::centredTop);
    note2Label.setJustificationType(juce::Justification::centredTop);
    note3Label.setJustificationType(juce::Justification::centredTop);


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
}

FeedbackerAudioProcessorEditor::~FeedbackerAudioProcessorEditor()
{
}

//==============================================================================
void FeedbackerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.setColour (juce::Colours::white);
    //g.setFont (juce::FontOptions (15.0f));
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void FeedbackerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds();

    auto leftGroup = bounds.removeFromLeft(bounds.getWidth() / 3);
    auto gainArea = leftGroup.removeFromTop(leftGroup.getHeight() / 3);
    auto thresholdArea = leftGroup.removeFromTop(leftGroup.getHeight() / 2);
    auto rampUpSpeedArea = leftGroup;
    gainLabel.setBounds(gainArea.removeFromBottom(labelHeight));
    gainSlider.setBounds(gainArea);

    thresholdLabel.setBounds(thresholdArea.removeFromBottom(labelHeight));
    thresholdSlider.setBounds(thresholdArea);

    rampUpSpeedLabel.setBounds(rampUpSpeedArea.removeFromBottom(labelHeight));
    rampUpSpeedSlider.setBounds(rampUpSpeedArea);
    //rampUpSpeedLabel.setBounds(leftGroup.removeFromTop(labelHeight));

    // Note 1
    auto note1Area = bounds.removeFromTop(bounds.getHeight() / 4);
    osc1BypassButton.setBounds(note1Area.removeFromRight(note1Area.getWidth() / 4));
    note1Label.setBounds(note1Area.removeFromTop(labelHeight));
    osc1FreqSlider.setBounds(note1Area.removeFromTop(note1Area.getHeight() / 2));
    osc1HoldTimeSlider.setBounds(note1Area);
    
    // Note 2
    auto note2Area = bounds.removeFromTop(bounds.getHeight() / 3);
    osc2BypassButton.setBounds(note2Area.removeFromRight(note2Area.getWidth() / 4));
    note2Label.setBounds(note2Area.removeFromTop(labelHeight));
    osc2FreqSlider.setBounds(note2Area.removeFromTop(note2Area.getHeight() / 2));
    osc2HoldTimeSlider.setBounds(note2Area);

    // Note 3
    auto note3Area = bounds.removeFromTop(bounds.getHeight() / 2);
    osc3BypassButton.setBounds(note3Area.removeFromRight(note3Area.getWidth() / 4));
    note3Label.setBounds(note3Area.removeFromTop(labelHeight));
    osc3FreqSlider.setBounds(note3Area.removeFromTop(note3Area.getHeight() / 2));
    osc3HoldTimeSlider.setBounds(note3Area);

    // Note 4
    auto note4Area = bounds;
    osc4BypassButton.setBounds(note4Area.removeFromRight(note4Area.getWidth() / 4));
    note4Label.setBounds(note4Area.removeFromTop(labelHeight));
    osc4FreqSlider.setBounds(note4Area.removeFromTop(note4Area.getHeight() / 2));
    osc4HoldTimeSlider.setBounds(note4Area);
}

std::vector<juce::Component*> FeedbackerAudioProcessorEditor::getComps()
{
    return
    {
        &gainSlider,
        &thresholdSlider,
        &rampUpSpeedSlider,

        &osc1FreqSlider,
        &osc2FreqSlider,
        &osc3FreqSlider,
        &osc4FreqSlider,

        &osc1HoldTimeSlider,
        &osc2HoldTimeSlider,
        &osc3HoldTimeSlider,
        &osc4HoldTimeSlider,

        &osc1BypassButton,
        &osc2BypassButton,
        &osc3BypassButton,
        &osc4BypassButton,

        &rampUpSpeedLabel, 
        &thresholdLabel, 
        &gainLabel, 
        &note1Label, 
        &note2Label, 
        &note3Label, 
        &note4Label
    };
}