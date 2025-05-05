/*
  ==============================================================================

    jfet.cpp
    Created: 26 Apr 2025 4:59:11pm
    Author:  jwh93

  ==============================================================================
*/

#include "jfet.h"

JFET::JFET()
{
    gain = 0.5;
}

void JFET::setGain(double amount)
{
    gain = amount;
}

juce::dsp::AudioBlock<float> JFET::processBlock(juce::dsp::AudioBlock<float> block, int numChannels)
{
    for (int sample = 0; sample < block.getNumSamples(); sample++)
    {
        for (int channel = 0; channel < numChannels; channel++)
        {
            float* chunk = block.getChannelPointer(channel);

            chunk[sample] = chunk[sample] * gain;

            if (chunk[sample] <= -0.999987)
            {
                chunk[sample] = -0.570127;
            }

            else if (chunk[sample] > -0.999987 && chunk[sample] <= -0.725519)
            {
                chunk[sample] = 0.0016571 * pow(chunk[sample], 3.0) + 0.0049712 * pow(chunk[sample], 2.0) + 0.54213 * chunk[sample] - 0.031660;
            }

            else if (chunk[sample] > -0.725519 && chunk[sample] <= -0.243818)
            {
                chunk[sample] = 0.090556 * pow(chunk[sample], 3.0) + 0.19846 * pow(chunk[sample], 2.0) + 0.68252 * chunk[sample] + 0.0022901;
            }

            else if (chunk[sample] > -0.243818 && chunk[sample] <= 0.486152)
            {
                chunk[sample] = -0.29546 * pow(chunk[sample], 3.0) - 0.083888 * pow(chunk[sample], 2.0) + 0.61367 * chunk[sample] - 0.0033050;
            }

            else if (chunk[sample] > 0.486152 && chunk[sample] <= 0.7324)
            {
                chunk[sample] = -0.026740 * pow(chunk[sample], 3.0) - 0.47580 * pow(chunk[sample], 2.0) + 0.80420 * chunk[sample] - 0.034181;
            }

            else if (chunk[sample] > 0.7324 && chunk[sample] <= 0.931615)
            {
                chunk[sample] = 1.4407 * pow(chunk[sample], 3.0) - 3.7 * pow(chunk[sample], 2.0) + 3.1656 * chunk[sample] - 0.61068;
            }

            else if (chunk[sample] > 0.931615 && chunk[sample] <= 0.99833)
            {
                chunk[sample] = -1.6311 * pow(chunk[sample], 3.0) + 4.8852 * pow(chunk[sample], 2.0) - 4.8325 * chunk[sample] + 1.8730;
            }

            else if (chunk[sample] > 0.99833)
            {
                chunk[sample] = 0.29405;
            }

            chunk[sample] *= -1.0f;
        }
    }
    return block;
}
