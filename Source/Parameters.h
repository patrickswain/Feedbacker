#pragma once

// ************* Global params *********************
struct TriggerThresholdParam
{
    static constexpr auto id = "triggerThreshold";
    static constexpr auto name = "Trigger Threshold (db)";
    inline static const juce::NormalisableRange<float> range = { -100.0f, -5.0f };
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
    inline static const juce::NormalisableRange<float> range = { 0.0f, 0.80f };
    static constexpr float defaultValue = { 0.04f };
};

// ************* Note Frequencies *********************
struct Osc1FrequencyParam
{
    static constexpr auto id = "osc1Frequency";
    static constexpr auto name = "Note 1 Frequency";
    inline static const juce::NormalisableRange<float> range = { 40.0f, 5000.0f };;
    static constexpr float defaultValue = { 440.0f };
};

struct Osc2FrequencyParam
{
    static constexpr auto id = "osc2Frequency";
    static constexpr auto name = "Note 2 Frequency";
    inline static const juce::NormalisableRange<float> range = { 40.0f, 5000.0f };;
    static constexpr float defaultValue = { 880.0f };
};

struct Osc3FrequencyParam
{
    static constexpr auto id = "osc3Frequency";
    static constexpr auto name = "Note 3 Frequency";
    inline static const juce::NormalisableRange<float> range = { 40.0f, 5000.0f };;
    static constexpr float defaultValue = { 1200.0f };
};

struct Osc4FrequencyParam
{
    static constexpr auto id = "osc4Frequency";
    static constexpr auto name = "Note 4 Frequency";
    inline static const juce::NormalisableRange<float> range = { 40.0f, 5000.0f };;
    static constexpr float defaultValue = { 1600.0f };
};

// ************* Hold times *********************
struct Osc1HoldTimeParam
{
    static constexpr auto id = "osc1HoldTimeParam";
    static constexpr auto name = "Note 1 Hold Time (ms)";
    inline static const juce::NormalisableRange<float> range = { 50.0f, 4000.0f };;
    static constexpr float defaultValue = { 1000.0f };
};

struct Osc2HoldTimeParam
{
    static constexpr auto id = "osc2HoldTimeParam";
    static constexpr auto name = "Note 2 Hold Time (ms)";
    inline static const juce::NormalisableRange<float> range = { 50.0f, 4000.0f };;
    static constexpr float defaultValue = { 1000.0f };
};

struct Osc3HoldTimeParam
{
    static constexpr auto id = "osc3HoldTimeParam";
    static constexpr auto name = "Note 3 Hold Time (ms)";
    inline static const juce::NormalisableRange<float> range = { 50.0f, 4000.0f };;
    static constexpr float defaultValue = { 1000.0f };
};

struct Osc4HoldTimeParam
{
    static constexpr auto id = "osc4HoldTimeParam";
    static constexpr auto name = "Note 4 Hold Time (ms)";
    inline static const juce::NormalisableRange<float> range = { 50.0f, 4000.0f };;
    static constexpr float defaultValue = { 1000.0f };
};

// ************* Bypasses *********************
struct Osc1BypassParam
{
    static constexpr auto id = "osc1BypassParam";
    static constexpr auto name = "Note 1 Bypass";
    static constexpr bool defaultValue = false;
};

struct Osc2BypassParam
{
    static constexpr auto id = "osc2BypassParam";
    static constexpr auto name = "Note 2 Bypass";
    static constexpr bool defaultValue = false;
};

struct Osc3BypassParam
{
    static constexpr auto id = "osc3BypassParam";
    static constexpr auto name = "Note 3 Bypass";
    static constexpr bool defaultValue = false;
};

struct Osc4BypassParam
{
    static constexpr auto id = "osc4BypassParam";
    static constexpr auto name = "Note 4 Bypass";
    static constexpr bool defaultValue = false;
};