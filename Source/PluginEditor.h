/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "skClasses/skVibes.h"

//==============================================================================
/**
*/
class FetzerAudioProcessorEditor : public juce::AudioProcessorEditor,
                                   public juce::Slider::Listener

{
public:
    FetzerAudioProcessorEditor (FetzerAudioProcessor&);
    ~FetzerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;
    void updateFrequencyLabel(juce::Slider* slider);


private:

    SKVibes skVibes;

    double unitSize;
    double knobSize, buttonSize;
    float control;

    SKVibes::windowParams* vibeParams;
    SKVibes::visualParams vParams;
    SKVibes::panelParams pParams;
    SKVibes::pixelParams* pixels;

    juce::Slider mVolumeSlider, mGainSlider, mTrebleSlider, mBassSlider, mShapeSlider, mVoiceSlider;

    std::vector<juce::Slider*> allKnobs
    {
        &mTrebleSlider,
        &mBassSlider,
        &mGainSlider,
        &mVolumeSlider,
        &mVoiceSlider,
        &mShapeSlider
    };

    std::vector<juce::Point<float>> knobPos;

    juce::Label gain, treble, shape, voice, bass, level;

    std::vector<juce::Label*> allLabels
    {
        &treble,
        &bass,
        &gain,
        &level,
        &voice,
        &shape
    };

    std::vector<juce::String> labelText
    {
        "Treble",
        "Bass",
        "Gain",
        "Level",
        "Voice",
        "Shape",
    };

    juce::ToggleButton fetButton1, fetButton2, fetButton3;

    std::vector<juce::ToggleButton*> buttons
    {
        &fetButton1,
        &fetButton2,
        &fetButton3
    };

    std::vector<juce::Point<float>> buttonPos;

    juce::Label titleLabel;
    juce::Point<float> titlePos;

    juce::Point<float> voiceLabelPos;
    juce::Label voiceLabel;
    std::string voiceFreq;

    std::vector<juce::Point<float>> panelPos;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trebleSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bassSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> shapeSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> voiceSliderAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> fet1ButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> fet2ButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> fet3ButtonAttachment;

    FetzerAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FetzerAudioProcessorEditor)
};
