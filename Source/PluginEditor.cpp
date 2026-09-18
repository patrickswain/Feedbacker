/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"
//==============================================================================
ImageStrip::ImageStrip()
{

}
ImageStrip::~ImageStrip()
{

}
void ImageStrip::loadImage(const char* file, size_t filesize)
{
    image = juce::ImageCache::getFromMemory(file, filesize);

    if (!image.isValid())
    {
        return;
    }

    
    isHorizontal = (image.getWidth() > image.getHeight()) ? true : false;
    numFrames = isHorizontal ? (image.getWidth() / image.getHeight()) : (image.getHeight() / image.getWidth());
}

juce::Image ImageStrip::getCroppedFrame(float sliderPosProportional)
{
    if (!image.isValid() || numFrames < 1)
    {
        return {};
    }
    int index = juce::jlimit(0, numFrames - 1, (int)std::floor(sliderPosProportional * (numFrames - 1)));
    int initialX;
    int initialY;
    int frameWidth = isHorizontal ? image.getWidth() / numFrames : image.getWidth();
    int frameHeight = isHorizontal ? image.getHeight() : image.getHeight() / numFrames;
    if (isHorizontal)
    {
        initialX = index * frameWidth;
        initialY = 0;
    }
    else
    {
        initialX = 0;
        initialY = index * frameHeight;
    }

    auto cropArea = juce::Rectangle<int>(initialX, initialY, frameWidth, frameHeight);

    return image.getClippedImage(cropArea);
}
void ImageStrip::drawImage(juce::Graphics& g, juce::Image croppedFrame, juce::Rectangle<float> bounds)
{
    g.drawImage(croppedFrame, bounds);
}
//==============================================================================
ImageStripLookAndFeel::ImageStripLookAndFeel()
{
    rotaryKnobStrip.loadImage(BinaryData::hise_Knob_medium_png, BinaryData::hise_Knob_medium_pngSize);
    sliderStrip.loadImage(BinaryData::Slider_png, BinaryData::Slider_pngSize);
    toggleButtonStrip.loadImage(BinaryData::ToggleButton_mn_png, BinaryData::ToggleButton_mn_pngSize);
}
void ImageStripLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    auto croppedFrame = rotaryKnobStrip.getCroppedFrame(sliderPosProportional);
    rotaryKnobStrip.drawImage(g, croppedFrame, juce::Rectangle<int>(x, y, width, height).toFloat());
}

void ImageStripLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle style, juce::Slider& slider)
{
    
    float sliderProportion;
    float sliderMin = (float)slider.getBounds().getX();//(float)slider.getMaximum();
    float sliderMax = sliderMin + (float)slider.getBounds().getWidth();//(float)slider.getMinimum();
    if (sliderMax != sliderMin)
    {
        sliderProportion = juce::jlimit(0.0f, 1.0f, sliderPos / (sliderMax - sliderMin));
    }
    else
    {
        sliderProportion = 0;
    }
    DBG("Slider min = " + std::to_string(sliderMin) + ". Slider max = " + std::to_string(sliderMax) + ". current position = " + std::to_string(sliderPos));
    DBG("bounds = " + slider.getBounds().toString());
    DBG("Slider Proportion = " + std::to_string(sliderProportion));
    auto croppedFrame = sliderStrip.getCroppedFrame(sliderProportion);
    sliderStrip.drawImage(g, croppedFrame, juce::Rectangle<int>(x, y, width, height).toFloat());
}

void ImageStripLookAndFeel::drawLinearSliderBackground(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle style, juce::Slider& slider)
{
    // no-op — suppress default background entirely for this test
}
//==============================================================================

OscillatorControls::OscillatorControls(juce::AudioProcessorValueTreeState& apvts,
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

OscillatorControls::~OscillatorControls()
{
    bypassButton.setLookAndFeel(nullptr);
    holdTimeSlider.setLookAndFeel(nullptr);
    freqSlider.setLookAndFeel(nullptr);
}

std::vector<juce::Component*> OscillatorControls::getComponents()
{
    return { &freqSlider, &holdTimeSlider, &bypassButton, &label };
}

void OscillatorControls::layoutComponents(juce::Rectangle<int> area, int labelHeight)
{
    bypassButton.setBounds(area.removeFromRight(area.getWidth() / 4));
    label.setBounds(area.removeFromTop(labelHeight));
    freqSlider.setBounds(area.removeFromTop(area.getHeight() / 2));
    holdTimeSlider.setBounds(area);
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

    for (Component* comp : getComps())
    {
        addAndMakeVisible(comp);
        comp->setLookAndFeel(&imageStripLookAndFeel);
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
    for (auto* comps : getComps())
    {
        comps->setLookAndFeel(nullptr);
    }
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