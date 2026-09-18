/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"
#include "Parameters.h"

//==============================================================================

class ImageStrip
{
public:
    ImageStrip();
    ~ImageStrip();
    void loadImage(const char* file, size_t filesize);
    juce::Image getCroppedFrame(float sliderPosProportional);
    void drawImage(juce::Graphics& g, juce::Image croppedFrame, juce::Rectangle<float> bounds);

private:
    juce::Image image;
    int numFrames;
    bool isHorizontal = false;
};

//==============================================================================

struct ImageStripLookAndFeel : juce::LookAndFeel_V4
{
    ImageStripLookAndFeel();    
    void drawRotarySlider(juce::Graphics& g,
        int 	x,
        int 	y,
        int 	width,
        int 	height,
        float 	sliderPosProportional,
        float 	rotaryStartAngle,
        float 	rotaryEndAngle,
        juce::Slider& slider)override;
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle style, juce::Slider& slider) override;
    void drawLinearSliderBackground(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle style, juce::Slider& slider) override;
    ImageStrip rotaryKnobStrip;
    ImageStrip sliderStrip;
    ImageStrip toggleButtonStrip;
};

//==============================================================================
struct OscillatorControls
{
    OscillatorControls(juce::AudioProcessorValueTreeState& apvts,
        const juce::String& freqParamId,
        const juce::String& holdTimeParamId,
        const juce::String& bypassParamId,
        const juce::String& noteLabel);

    ~OscillatorControls();
    void layoutComponents(juce::Rectangle<int> area, int labelHeight);
    std::vector<juce::Component*> getComponents();

    //ImageStripLookAndFeel imageStripLookAndFeel;

    juce::Slider freqSlider, holdTimeSlider;
    juce::ToggleButton bypassButton;
    juce::Label label;

    juce::AudioProcessorValueTreeState::SliderAttachment freqSliderAttachment, holdTimeSliderAttachment;
    juce::AudioProcessorValueTreeState::ButtonAttachment bypassButtonAttachment;

};
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
    ImageStripLookAndFeel imageStripLookAndFeel;

    static constexpr int labelHeight = 20;
    static constexpr int numOscillators = 4;

    juce::Slider gainSlider, thresholdSlider, rampUpSpeedSlider; // knobs

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment gainSliderAttachment,
        thresholdSliderAttachment,
        rampUpSpeedSliderAttachment;
        
    juce::Label rampUpSpeedLabel, thresholdLabel, gainLabel;

    std::array<OscillatorControls, numOscillators> oscillators;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FeedbackerAudioProcessorEditor)
};


