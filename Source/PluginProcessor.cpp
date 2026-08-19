/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
FeedbackerAudioProcessor::FeedbackerAudioProcessor() : apvts(*this, nullptr, "Parameters", createParameterLayout())
#ifndef JucePlugin_PreferredChannelConfigurations
     , AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    
}

FeedbackerAudioProcessor::~FeedbackerAudioProcessor()
{
}

//==============================================================================
const juce::String FeedbackerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FeedbackerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FeedbackerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FeedbackerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FeedbackerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FeedbackerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FeedbackerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FeedbackerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FeedbackerAudioProcessor::getProgramName (int index)
{
    return {};
}

void FeedbackerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FeedbackerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;

    oscManager.prepare(spec);

    //updateGain();
}

void FeedbackerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FeedbackerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FeedbackerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    int numSamples = buffer.getNumSamples();
    settings.numSamples = numSamples;

    // update values to user inputs
    // At this point basically just gets the threshold bc the rest are sent to osc manager
    getParamSettings(apvts);

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Processing
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        float rmsLevel = buffer.getRMSLevel(channel, 0, numSamples);
        rmsLevel = juce::Decibels::gainToDecibels(rmsLevel);
        if (rmsLevel < triggerThreshold)
        {
            settings.addFeedback = true;
        }
        else
        {
            settings.addFeedback = false;
        }

        oscManager.updateSettings(settings);
                          
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
            
        if (channel == 0)
        {
            oscManager.process(context);
        }
 
    }
}

//==============================================================================
bool FeedbackerAudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FeedbackerAudioProcessor::createEditor()
{
    return new FeedbackerAudioProcessorEditor (*this);
}

//==============================================================================
void FeedbackerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FeedbackerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}


void FeedbackerAudioProcessor::updateGain()
{
    //smoothedGain.reset(currentSampleRate, settings.osc1rampUpSpeedSeconds);
    //smoothedGain.setCurrentAndTargetValue(0.0001f);
    //smoothedGain.setTargetValue(oscLevel);
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FeedbackerAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout FeedbackerAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(TriggerThresholdParam::id, TriggerThresholdParam::name, TriggerThresholdParam::range, TriggerThresholdParam::defaultValue));    
    layout.add(std::make_unique<juce::AudioParameterFloat>(SynthVolumeParam::id, SynthVolumeParam::name, SynthVolumeParam::range, SynthVolumeParam::defaultValue));
    layout.add(std::make_unique<juce::AudioParameterFloat>(RampUpSpeedParam::id, RampUpSpeedParam::name, RampUpSpeedParam::range, RampUpSpeedParam::defaultValue));
    layout.add(std::make_unique<juce::AudioParameterFloat>(SynthFrequencyParam::id, SynthFrequencyParam::name, SynthFrequencyParam::range, SynthFrequencyParam::defaultValue));

    return layout;
}

void FeedbackerAudioProcessor::getParamSettings(juce::AudioProcessorValueTreeState& apvts)
{

    // Feedback on/off logic
    triggerThreshold = apvts.getRawParameterValue(TriggerThresholdParam::id)->load();
    
    // Note choice
    settings.osc1Freq = apvts.getRawParameterValue(SynthFrequencyParam::id)->load();

    // Volume ramp up
    float rampUpSpeedMs = apvts.getRawParameterValue(RampUpSpeedParam::id)->load();
    settings.osc1rampUpSpeedSeconds = rampUpSpeedMs / 1000.00;
    settings.osc1Gain = apvts.getRawParameterValue(SynthVolumeParam::id)->load();//static_cast<double>(apvts.getRawParameterValue(SynthVolumeParam::id)->load());
}