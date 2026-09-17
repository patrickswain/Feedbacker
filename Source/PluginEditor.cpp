/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"
//==============================================================================
FeedbackerLookAndFeel::FeedbackerLookAndFeel()
{
    imageStrip = juce::ImageCache::getFromMemory(BinaryData::hise_Knob_medium_png, BinaryData::hise_Knob_medium_pngSize);

    jassert(imageStrip.isValid());

    if (imageStrip.getHeight() < imageStrip.getWidth())
    {
        isHorizontal = true;
        numFrames = imageStrip.getWidth() / imageStrip.getHeight();
    }
    else
    {
        isHorizontal = false;
        numFrames = imageStrip.getHeight() / imageStrip.getWidth();
    }


}

void FeedbackerLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    
    if (!imageStrip.isValid() || numFrames <= 0)
    {
        return;
    }
    //auto radius = (float)juce::jmin(width, height) / 2;
    //auto centerX = x + (width / 2);
    //auto centerY = y + (height / 2);
    //auto diameter = radius * 2.0f;
    //auto startingX = centerX - radius;
    //auto startingY = centerY - radius;

    //g.drawEllipse(startingX, startingY, diameter, diameter, 2.0f);
    int currentFrame = 0;

    int sourceX = 0;
    int sourceY = 0;
    int frameWidth = 0;
    int frameHeight = 0;
    juce::Image croppedImage;

    currentFrame = juce::jlimit(0, numFrames - 1, 
                    (int)std::floor(sliderPosProportional * numFrames));

    // if position is 0.9
    // 5 frames wide of 20 pixels each (100 pixels wide)

    if (isHorizontal)
    {
        sourceX = currentFrame * frameWidth;
        sourceY = 0;
        frameWidth = imageStrip.getWidth() / numFrames;
        frameHeight = imageStrip.getHeight();
        croppedImage = imageStrip.getClippedImage(juce::Rectangle<int>(currentFrame * frameWidth, 0, frameWidth, frameHeight));
    }
    else
    {
        sourceX = 0;
        sourceY = currentFrame * frameHeight;
        frameWidth = imageStrip.getWidth();
        frameHeight = imageStrip.getHeight() / numFrames;
        croppedImage = imageStrip.getClippedImage(juce::Rectangle<int>(0, currentFrame * frameHeight, frameWidth, frameHeight));
    }

    g.drawImage(croppedImage, x, y, width, height, sourceX, sourceY, frameWidth, frameHeight, false);
    
}

void FeedbackerLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle style, juce::Slider& slider)
{

}

//==============================================================================
FeedbackerAudioProcessorEditor::FeedbackerAudioProcessorEditor(FeedbackerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    gainSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow),
    gainSliderAttachment(audioProcessor.apvts, SynthVolumeParam::id, gainSlider),
    thresholdSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow),
    thresholdSliderAttachment(audioProcessor.apvts, TriggerThresholdParam::id, thresholdSlider),
    rampUpSpeedSlider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow),
    rampUpSpeedSliderAttachment(audioProcessor.apvts, RampUpSpeedParam::id, rampUpSpeedSlider), 
    oscillators
    {
        OscillatorControls{audioProcessor.apvts, Osc1FrequencyParam::id, Osc1HoldTimeParam::id, Osc1BypassParam::id, "Note 1"},
        OscillatorControls{audioProcessor.apvts, Osc2FrequencyParam::id, Osc2HoldTimeParam::id, Osc2BypassParam::id, "Note 2"},
        OscillatorControls{audioProcessor.apvts, Osc3FrequencyParam::id, Osc3HoldTimeParam::id, Osc3BypassParam::id, "Note 3"},
        OscillatorControls{audioProcessor.apvts, Osc4FrequencyParam::id, Osc4HoldTimeParam::id, Osc4BypassParam::id, "Note 4"}
    }
{

    setLookAndFeel(&feedbackerLookAndFeel);

    for (Component* comp : getComps())
    {
        addAndMakeVisible(comp);
        //comp->setLookAndFeel(&feedbackerLookAndFeel);
    }
    
    

    rampUpSpeedLabel.setText("Ramp Up Speed", juce::NotificationType::dontSendNotification);
    thresholdLabel.setText("Threshold", juce::NotificationType::dontSendNotification);
    gainLabel.setText("Feedback Gain", juce::NotificationType::dontSendNotification);

    rampUpSpeedLabel.setJustificationType(juce::Justification::centredTop);
    thresholdLabel.setJustificationType(juce::Justification::centredTop);
    gainLabel.setJustificationType(juce::Justification::centredTop);


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
}

FeedbackerAudioProcessorEditor::~FeedbackerAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
    //for (auto* comps : getComps())
    //{
    //    comps->setLookAndFeel(nullptr);
    //}
}

//==============================================================================
void FeedbackerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

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

    int layoutHeight = bounds.getHeight() / numOscillators;
    for (auto& osc : oscillators)
    {
        osc.layoutComponents(bounds.removeFromTop(layoutHeight), labelHeight);
    }
}

std::vector<juce::Component*> FeedbackerAudioProcessorEditor::getComps()
{
    std::vector<juce::Component*> comps = { &gainSlider, &thresholdSlider, &rampUpSpeedSlider,
                                            &gainLabel, &thresholdLabel, &rampUpSpeedLabel };

    for (auto& osc : oscillators)
    {
        auto oscComponents = osc.getComponents();
        comps.insert(comps.end(), oscComponents.begin(), oscComponents.end());
    }
    
    return comps;

}