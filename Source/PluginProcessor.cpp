/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
FetzerAudioProcessor::FetzerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
    aaf = std::make_unique<SKAntiAlias>();
    
    inputHP = std::make_unique<skFilters>(highpass);
    inputLP = std::make_unique<skFilters>(lowpass);
    gainStage1LP = std::make_unique<skFilters>(lowpass);
    gainStage2LP = std::make_unique<skFilters>(lowpass);
    gainStage3LP = std::make_unique<skFilters>(lowpass);
    outputFilter = std::make_unique<skFilters>(lowpass);
    trebleHS = std::make_unique<skFilters>(highshelf);
    bassLS = std::make_unique<skFilters>(lowshelf);
    midPK = std::make_unique<skFilters>(peak);

    jfet1 = std::make_unique<JFET>();
    jfet2 = std::make_unique<JFET>();
    jfet3 = std::make_unique<JFET>();
}

FetzerAudioProcessor::~FetzerAudioProcessor()
{
}

//==============================================================================
const juce::String FetzerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FetzerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FetzerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FetzerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FetzerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FetzerAudioProcessor::getNumPrograms()
{
    return 1;
}

int FetzerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FetzerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FetzerAudioProcessor::getProgramName (int index)
{
    return {};
}

void FetzerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FetzerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();

    aaf->setup(samplesPerBlock, getSampleRate());

    inputHP->calc(50.0, 1.0, 0.707, getSampleRate());
    inputLP->calc(3500.0, 1.0, 0.707, getSampleRate());
    gainStage1LP->calc(1000.0, 1.0, 0.707, getSampleRate() * 4.0);
    gainStage2LP->calc(1000.0, 1.0, 0.707, getSampleRate() * 4.0);
    gainStage3LP->calc(1000.0, 1.0, 0.707, getSampleRate() * 4.0);
    outputFilter->calc(3000.0, 1.0, 0.707, getSampleRate());

    updateParameters();
}

void FetzerAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FetzerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void FetzerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    buffer.copyFrom(1, 0, buffer.getReadPointer(0), buffer.getNumSamples());
    auto inputBlock = juce::dsp::AudioBlock<float>(buffer);
    juce::dsp::AudioBlock<float> overBlock;

    updateParameters();

    inputBlock = inputHP->processBlock(inputBlock, 2);

    inputBlock = inputLP->processBlock(inputBlock, 2);

    inputBlock = midPK->processBlock(inputBlock, 2);

    overBlock = aaf->processUp(inputBlock);

    overBlock = jfet1->processBlock(overBlock, 2);
    overBlock = gainStage1LP->processBlock(overBlock, 2);

    if (fet2State)
    {
        overBlock = jfet2->processBlock(overBlock, 2);
        overBlock = gainStage2LP->processBlock(overBlock, 2);
    }

    if (fet3State)
    {
        overBlock = jfet3->processBlock(overBlock, 2);
        overBlock = gainStage3LP->processBlock(overBlock, 2);
    }

    aaf->processDown(overBlock, inputBlock);

    inputBlock = outputFilter->processBlock(inputBlock, 2);

    inputBlock = trebleHS->processBlock(inputBlock, 2);

    inputBlock = bassLS->processBlock(inputBlock, 2);

    inputBlock = gainScale(inputBlock, 2);
}

juce::dsp::AudioBlock<float> FetzerAudioProcessor::gainScale(juce::dsp::AudioBlock<float> block, int numChannels)
{
    for (int sample = 0; sample < block.getNumSamples(); sample++)
    {
        for (int channel = 0; channel < numChannels; channel++)
        {
            float* data = block.getChannelPointer(channel);

            data[sample] *= volume;
        }
    }

    return block;
}


void FetzerAudioProcessor::updateParameters()
{
    volume = apvts.getRawParameterValue("VOLUME")->load();
    overdriveGain = apvts.getRawParameterValue("GAIN")->load() * 50.0 + 0.1;
    trebleGain = apvts.getRawParameterValue("TREBLE")->load() * 48.0 - 24.0 ;
    bassGain = apvts.getRawParameterValue("BASS")->load() * 48.0 - 24.0;
    midGain = apvts.getRawParameterValue("MID")->load() * 48.0 - 24.0;
    midVoice = apvts.getRawParameterValue("VOICE")->load() * 2000.0 + 400.0;

    fet1State = apvts.getRawParameterValue("STAGE1")->load();
    fet2State = apvts.getRawParameterValue("STAGE2")->load();
    fet3State = apvts.getRawParameterValue("STAGE3")->load();

    trebleHS->calc(2000.0, trebleGain, 1.0, getSampleRate());
    bassLS->calc(250.0, bassGain, 1.0, getSampleRate());
    midPK->calc(midVoice, midGain, 0.707, getSampleRate());

    jfet1->setGain(overdriveGain);
    jfet2->setGain(overdriveGain * 0.25);
    jfet3->setGain(overdriveGain * 0.25);
}

juce::AudioProcessorValueTreeState::ParameterLayout FetzerAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME", "volume", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "gain", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("TREBLE", "treble", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("BASS", "bass", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MID", "mid", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("VOICE", "voice", 0.0f, 1.0f, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterBool>("STAGE1", "stage1", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>("STAGE2", "stage2", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("STAGE3", "stage3", false));

    return { params.begin(), params.end() };
}

//==============================================================================
bool FetzerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FetzerAudioProcessor::createEditor()
{
    return new FetzerAudioProcessorEditor (*this);
}

//==============================================================================
void FetzerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void FetzerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FetzerAudioProcessor();
}
