cmake_minimum_required(VERSION 3.22)

project(FEEDBACKER VERSION 0.0.1)

#************** Download juce if needed *****************
set(LOCAL_JUCE_PATH "$ENV{HOME}/users/Patrick/documents/JUCE")

if (NOT EXISTS ${LOCAL_JUCE_PATH})
    message(STATUS "JUCE download not found. Cloning from github.")
    include(FetchContent)
    FetchContent_Declare(
        JUCE
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG 8.0.4
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        FIND_PACKAGE_ARGS 8.0.4 
    )
    FetchContent_MakeAvailable(JUCE)
else()
    message(STATUS "Local JUCE download found from ${LOCAL_JUCE_PATH}")
    set(JUCE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(JUCE_BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
    add_subdirectory(${LOCAL_JUCE_PATH} JUCE)
endif()

if(COMMAND juce_add_plugin)
    message(STATUS "JUCE loaded successfully")
else()
    message(FATAL_ERROR "JUCE was not loaded")
endif()

#************** shared files used by app and unit tests *****************
add_library(SharedFiles STATIC
    Source/RampingValues.cpp
)

target_include_directories(SharedFiles PUBLIC Source)

target_link_libraries(SharedFiles
    PUBLIC
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
)

#************** Main App *****************
if(APPLE)
    set(PLUGIN_FORMATS AU VST3 Standalone)
else()
    set(PLUGIN_FORMATS VST3 Standalone)
endif()

juce_add_plugin(Feedbacker
    COMPANY_NAME "A company's name"
    NEEDS_MIDI_INPUT FALSE               
    NEEDS_MIDI_OUTPUT FALSE              
    IS_MIDI_EFFECT FALSE                 
    PLUGIN_MANUFACTURER_CODE Nam3               
    PLUGIN_CODE Cod3                                                                          
    FORMATS ${PLUGIN_FORMATS}        
    PRODUCT_NAME "Feedbacker"         
    NEEDS_WEB_BROWSER FALSE                     
    NEEDS_CURL FALSE
)                           


#juce_generate_juce_header(Feedbacker)

target_sources(Feedbacker
    PRIVATE
        Source/PluginEditor.cpp
        Source/PluginProcessor.cpp
        Source/MyOsc.cpp
        Source/OscManager.cpp        
)


target_compile_definitions(Feedbacker
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(Feedbacker
    PRIVATE
        SharedFiles
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

#************** Console app for unit tests *****************
juce_add_console_app(UnitTestApp
    PRODUCT_NAME "UnitTestApp"
)

target_sources(UnitTestApp PRIVATE
    Tests/Main.cpp
    Tests/RampingValuesTest.cpp
)

target_link_libraries(UnitTestApp 
    PRIVATE
        SharedFiles
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags
)

enable_testing()
add_test(NAME UnitTestApp COMMAND UnitTestApp)