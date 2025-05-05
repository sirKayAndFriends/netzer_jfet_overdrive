/*
  ==============================================================================

    jfet.h
    Created: 26 Apr 2025 4:59:18pm
    Author:  jwh93

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class JFET
{
public:
    JFET();

    void setGain(double amount);

    juce::dsp::AudioBlock<float> processBlock(juce::dsp::AudioBlock<float> block, int numChannels);

private:

    double gain;
};
