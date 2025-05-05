/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FetzerAudioProcessorEditor::FetzerAudioProcessorEditor(FetzerAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setLookAndFeel(&skVibes);

    vibeParams = skVibes.aspectRatio(50, 14, 9);
    setSize(vibeParams->width, vibeParams->height);

    volumeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "VOLUME", mVolumeSlider);
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "GAIN", mGainSlider);
    trebleSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "TREBLE", mTrebleSlider);
    bassSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "BASS", mBassSlider);
    shapeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "MID", mShapeSlider);
    voiceSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "VOICE", mVoiceSlider);

    fet1ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.apvts, "STAGE1", fetButton1);
    fet2ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.apvts, "STAGE2", fetButton2);
    fet3ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.apvts, "STAGE3", fetButton3);

    skVibes.createDivisions();

    skVibes.setupSKSliders(allKnobs, allLabels, labelText);
    skVibes.setupSKTitle(&titleLabel);
    skVibes.setupSKHiddenButtons(buttons);

    panelPos =
    {
        skVibes.drawOnCentre(0, 0),
        skVibes.drawOnCentre(7,0),
        skVibes.drawOnCentre(0,4),
        skVibes.drawOnCentre(7,4),
    };

    unitSize = skVibes.getUnitSize();

    knobSize = skVibes.getKnobSize();
    buttonSize = knobSize * 0.75;

    for (int i = 0; i < allKnobs.size(); i++)
    {
        addAndMakeVisible(allKnobs[i]);
        addAndMakeVisible(allLabels[i]);
    }
    
    knobPos =
    {
        skVibes.drawOnIntersection(2,2, allKnobs[0]),
        skVibes.drawOnIntersection(5,2, allKnobs[1]),
        skVibes.drawOnIntersection(9,2, allKnobs[2]),
        skVibes.drawOnIntersection(12,2, allKnobs[3]),
        skVibes.drawOnIntersection(2,7, allKnobs[4]),
        skVibes.drawOnIntersection(5,7, allKnobs[5])
    };

    allKnobs[4]->addListener(this);
    allKnobs[5]->addListener(this);

    for (int i = 0; i < buttons.size(); i++)
    {
        buttons[i]->setSize(buttonSize, buttonSize);
        addAndMakeVisible(buttons[i]);
    }

    buttonPos =
    {
        skVibes.drawOnIntersection(9, 6, buttons[0]),
        skVibes.drawOnHorizontal(10, 6, buttons[1]),
        skVibes.drawOnIntersection(12, 6, buttons[2])
    };

    for (int i = 0; i < buttons.size(); i++)
    {
        if (i == 0)
        {
            skVibes.setupJFET(buttons[i], buttonPos[i], buttonSize, buttonSize, i, true);
        }

        else
        {
            skVibes.setupJFET(buttons[i], buttonPos[i], buttonSize, buttonSize, i, false);
        }    
    }

    addAndMakeVisible(voiceLabel);
    voiceLabel.setText("400", juce::dontSendNotification);
    voiceLabel.setFont(skVibes.getPluginFont().boldened().withHeight(24.0f));
    voiceLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colour(0xff000000));
    voiceLabel.setJustificationType(juce::Justification::centred);
    voiceLabel.setSize(unitSize * 2, unitSize);
    voiceLabelPos = skVibes.drawOnVertical(2, 5, &voiceLabel);
    
    addAndMakeVisible(titleLabel);
    titlePos = skVibes.drawOnHorizontal(7, 7);
    titleLabel.setText("n e t z e r_2.0", juce::NotificationType::dontSendNotification);

    pixels = skVibes.setupPixelShape(skVibes.getGridTopLeft(4, 5).x, skVibes.getGridTopLeft(4, 5).y, 100, 50);

    updateFrequencyLabel(allKnobs[4]);
    updateFrequencyLabel(allKnobs[5]);
}

FetzerAudioProcessorEditor::~FetzerAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void FetzerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setColour(juce::Colour(SKVibes::skIBMlavendar));
    juce::Rectangle<float> bgPanel(0.0, 0.0, getWidth(), getHeight());
    g.fillRect(bgPanel);

    for (int i = 0; i < panelPos.size(); i++)
    {
        if (i != 2)
        {
            skVibes.drawPanel(g, panelPos[i], unitSize * 6, unitSize * 3);
        }

        else
        {
            skVibes.drawPanel(g, panelPos[i], unitSize * 6, unitSize * 4);
        }
    }

    for (int i = 0; i < knobPos.size(); i++)
    {
        allKnobs[i]->setBounds(knobPos[i].x, knobPos[i].y, knobSize, knobSize);
        allLabels[i]->setBounds(knobPos[i].x, knobPos[i].y + knobSize, knobSize, 25);
    }

    for (int i = 0; i < buttons.size(); i++)
    {
        buttons[i]->setBounds(buttonPos[i].x, buttonPos[i].y, buttonSize, buttonSize);
        skVibes.drawJFET(g, i, buttons[i]->getToggleState());
    }

    voiceLabel.setBounds(voiceLabelPos.x, voiceLabelPos.y, unitSize * 2, unitSize);
    voiceLabel.setText(voiceFreq, juce::dontSendNotification);

    skVibes.drawPixelShape(g, pixels, control);

    titleLabel.setBounds(titlePos.x, titlePos.y, skVibes.getTitleSize()[0], skVibes.getTitleSize()[1]);
}

void FetzerAudioProcessorEditor::resized()
{
}

void FetzerAudioProcessorEditor::updateFrequencyLabel(juce::Slider* slider)
{
    double sliderVal = slider->getValue();

    if (slider == &mShapeSlider)
    {
        control = skVibes.lerp(pixels->border.getY() - 10.0, pixels->border.getBottom() + 10.0, (1.0 - sliderVal));
        repaint();
    }

    if (slider == &mVoiceSlider)
    {
        //scale to Hz values and send to label
        double scaled = (sliderVal * 2000) + 400;
        int scaledVal = juce::roundToInt(scaled);
        std::string hz = " Hz";
        voiceFreq = std::to_string(scaledVal) + hz;

        repaint();
    }
}

void FetzerAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    updateFrequencyLabel(slider);
}

