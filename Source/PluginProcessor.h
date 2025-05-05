/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "skClasses/skFilters.h"
#include "skClasses/antiAlias.h"
#include "skClasses/jfet.h"


//==============================================================================
/**
*/
class FetzerAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    FetzerAudioProcessor();
    ~FetzerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void updateParameters();
    
    juce::dsp::AudioBlock<float> gainScale(juce::dsp::AudioBlock<float> block, int numChannels);

    juce::AudioProcessorValueTreeState apvts;

private:
    //==============================================================================

    double volume;
    double overdriveGain;
    double trebleGain;
    double bassGain;
    double midGain;
    double midVoice;

    bool fet1State, fet2State, fet3State;

    std::unique_ptr<SKAntiAlias> aaf;
    
    std::unique_ptr<skFilters> inputHP;
    std::unique_ptr<skFilters> inputLP;
    std::unique_ptr<skFilters> gainStage1LP;
    std::unique_ptr<skFilters> gainStage2LP;
    std::unique_ptr<skFilters> gainStage3LP;
    std::unique_ptr<skFilters> outputFilter;
    std::unique_ptr<skFilters> trebleHS;
    std::unique_ptr<skFilters> bassLS;
    std::unique_ptr<skFilters> midPK;

    std::unique_ptr<JFET> jfet1;
    std::unique_ptr<JFET> jfet2;
    std::unique_ptr<JFET> jfet3;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FetzerAudioProcessor)
};
