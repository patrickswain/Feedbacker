cmake_minimum_required(VERSION 3.22)

project(FEEDBACKER VERSION 0.0.1)

set(LOCAL_JUCE_PATH "$ENV{HOME}/users/Patrick/documents/JUCE")

if (NOT EXISTS ${LOCAL_JUCE_PATH})
    message(STATUS "JUCE download not found. Cloning from github.")
    #include(FetchContent)
    #FetchContent_Declare(
    #    JUCE
    #    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
     #   GIT_TAG 8.0.4
    #    GIT_SHALLOW TRUE
    #    GIT_PROGRESS TRUE
    #    FIND_PACKAGE_ARGS 8.0.4 
    #)
    #FetchContent_MakeAvailable(JUCE)
else()
    message(STATUS "Local JUCE download found from ${LOCAL_JUCE_PATH}")
    set(JUCE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(JUCE_BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
    add_subdirectory(${LOCAL_JUCE_PATH} JUCE)
endif()

juce_add_plugin(Feedbacker
    # VERSION ...                               # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    # COMPANY_NAME ...                          # Specify the name of the plugin's author
    IS_SYNTH FALSE                       # Is this a synth or an effect?
    NEEDS_MIDI_INPUT FALSE               # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE              # Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                 # Is this plugin a MIDI effect?
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
    # COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE Patr               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE cOde                            # A unique four-character plugin id with exactly one upper-case character
                                                # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    FORMATS AU VST3 Standalone                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "Feedbacker"         # The name of the final executable, which can differ from the target name
    NEEDS_WEB_BROWSER FALSE                     # Set to TRUE if you need to use webviews
    NEEDS_CURL FALSE)                           # Set to TRUE if you need to use curl


juce_generate_juce_header(Feedbacker)

target_sources(Feedbacker
    PRIVATE
        Source/PluginEditor.cpp
        Source/PluginProcessor.cpp
        Source/MyOsc.cpp
        Source/OscManager.cpp
        Source/RampingValues.cpp
)


target_compile_definitions(Feedbacker
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(Feedbacker
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)