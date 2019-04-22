/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AapCourseworkAudioProcessor::AapCourseworkAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                  .withInput  ("Input",  AudioChannelSet::stereo(), true)
#endif
                  .withOutput ("Output", AudioChannelSet::stereo(), true)
#endif
                  )
#endif
{
    
    // Add Input Combo Box
    addParameter(inputSelect = new AudioParameterChoice("inputSelect", "Input Select", {"- Select", "Stereo", "Mid-Side"}, 0));
    
    // Add Output Combo Box
    addParameter(outputSelect = new AudioParameterChoice("outputSelect", "Output Select", {"- Select", "Stereo", "Mid-Side"}, 0));
    
    // Add Stereo Width Slider
    addParameter(stereoWidth = new AudioParameterFloat("stereoWidth", "Stereo Width", 0.0f, 2.0f, 1.0f));
    
    // Phase Inversion
    addParameter(invertPhase = new AudioParameterBool("invertPhase", "Phase Inversion", false));
}

AapCourseworkAudioProcessor::~AapCourseworkAudioProcessor()
{
}

//==============================================================================
const String AapCourseworkAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AapCourseworkAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool AapCourseworkAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool AapCourseworkAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double AapCourseworkAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AapCourseworkAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int AapCourseworkAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AapCourseworkAudioProcessor::setCurrentProgram (int index)
{
}

const String AapCourseworkAudioProcessor::getProgramName (int index)
{
    return {};
}

void AapCourseworkAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AapCourseworkAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}


void AapCourseworkAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations

bool AapCourseworkAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void AapCourseworkAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)


{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        
        
        auto* channelDataLeft = buffer.getWritePointer(0);
        auto* channelDataRight = buffer.getWritePointer(1);
        
        
        for(int i = 0; i < buffer.getNumSamples(); i++) {
            
            auto inputChoice = inputSelect->getIndex();
            auto outputChoice = outputSelect->getIndex();
            auto width = stereoWidth->get();
            auto invertChoice = invertPhase->get();
            
            
            // Combination select Stereo input, Stereo Output
            if (inputChoice == 1 && outputChoice == 1) {
                
                // Input - Encoding MS to Stereo
                auto xSide = 0.5f * (channelDataLeft[i] - channelDataRight[i]);
                auto xMid = 0.5f * (channelDataLeft[i] + channelDataRight[i]);
                
                // Width
                xSide = width * xSide;
                xMid = (2.0f - width) * xMid;
                
                // Phase
                auto phase = *invertPhase ? -1.0f : 1.0f;
                
                // Output - Decoding MS to Stereo
                channelDataLeft[i] = (xMid + xSide);
                channelDataRight[i] = (xMid - xSide);
                
                
            }
            
            // Combination select Stereo input, Mid-Side Output
            else if (inputChoice == 1 && outputChoice == 2){
                
                // Input - Encoding MS to Stereo
                auto xSide = 0.5f * (channelDataLeft[i] - channelDataRight[i]);
                auto xMid = 0.5f * (channelDataLeft[i] + channelDataRight[i]);
                
                // Width
                xSide = width * xSide;
                xMid = (2.0f - width) * xMid;
                
                // Phase
                auto phase = *invertPhase ? -1.0f : 1.0f;
                
                // Output - Decoding Stereo to MS
                xMid = channelDataLeft[i];
                xSide = channelDataRight[i];
                
                
            }
            
            
            // Combination select Mid-Side input, Stereo Output
            else if (inputChoice == 2 && outputChoice == 1){
                
                // Input - Encoding Stereo to MS
                auto xMid = channelDataLeft[i];
                auto xSide = channelDataRight[i];
                
                // Width
                xSide = width * xSide;
                xMid = (2.0f - width) * xMid;
                
                // Phase
                auto phase = *invertPhase ? -1.0f : 1.0f;
                
                // Output - Decoding MS to Stereo
                channelDataLeft[i] = (xMid + xSide);
                channelDataRight[i] = (xMid - xSide);
                
                
            }
            
            
            // Combination select Mid-Side input, Mid-Side Output
            else if (inputChoice == 2 && outputChoice == 2){
                
                // Input - Encoding MS to Stereo
                auto xMid = channelDataLeft[i];
                auto xSide = channelDataRight[i];
                
                // Width
                xSide = width * xSide;
                xMid = (2.0f - width) * xMid;
                
                // Phase
                auto phase = *invertPhase ? -1.0f : 1.0f;
                
                // Output - Decoding Stereo to MS
                xMid = channelDataLeft[i];
                xSide = channelDataRight[i];
                
            }
            
        }
        
    }
    
}


//==============================================================================
bool AapCourseworkAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AapCourseworkAudioProcessor::createEditor()
{
    return new GenericAudioProcessorEditor(this);
}

//==============================================================================
void AapCourseworkAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    MemoryOutputStream stream(destData, true);
    stream.writeFloat(*stereoWidth);
    stream.writeInt(*inputSelect);
    stream.writeInt(*outputSelect);
    stream.writeBool(*invertPhase);
}

void AapCourseworkAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    MemoryInputStream stream(data, static_cast<size_t> (sizeInBytes), false);
    stereoWidth->setValueNotifyingHost(stereoWidth->getNormalisableRange().convertTo0to1(stream.readFloat()));
    inputSelect->setValueNotifyingHost(inputSelect->getNormalisableRange().convertTo0to1(stream.readInt()));
    outputSelect->setValueNotifyingHost(outputSelect->getNormalisableRange().convertTo0to1(stream.readInt()));
    invertPhase->setValueNotifyingHost(invertPhase->getNormalisableRange().convertTo0to1(stream.readBool()));
    
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AapCourseworkAudioProcessor();
}

