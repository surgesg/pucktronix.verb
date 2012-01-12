//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:27 $
//
// Category     : VST 2.x SDK Samples
// Filename     : again.cpp
// Created by   : Steinberg Media Technologies
// Description  : Stereo plugin which applies Gain [-oo, 0dB]
//
// � 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#include "verb.h"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new Springverb (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
Springverb::Springverb (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, 1)	// 1 program, 1 parameter only
{
	setNumInputs (2);		// stereo in
	setNumOutputs (2);		// stereo out
	setUniqueID ('Gain');	// identify
	canProcessReplacing ();	// supports replacing output
	canDoubleReplacing ();	// supports double precision processing

	fGain = 1.f;			// default to 0 dB
	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
	num_resonators = 500;
	resonators = new Resonator[num_resonators];
	for(int i = 0; i < num_resonators; i++){
		resonators[i].set_params(4 + i, -0.95);	
	}
}

//-------------------------------------------------------------------------------------------------------
Springverb::~Springverb ()
{
	// nothing to do here
}

//-------------------------------------------------------------------------------------------------------
void Springverb::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void Springverb::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void Springverb::setParameter (VstInt32 index, float value)
{
	fGain = value;
}

//-----------------------------------------------------------------------------------------
float Springverb::getParameter (VstInt32 index)
{
	return fGain;
}

//-----------------------------------------------------------------------------------------
void Springverb::getParameterName (VstInt32 index, char* label)
{
	vst_strncpy (label, "Gain", kVstMaxParamStrLen);
}

//-----------------------------------------------------------------------------------------
void Springverb::getParameterDisplay (VstInt32 index, char* text)
{
	dB2string (fGain, text, kVstMaxParamStrLen);
}

//-----------------------------------------------------------------------------------------
void Springverb::getParameterLabel (VstInt32 index, char* label)
{
	vst_strncpy (label, "dB", kVstMaxParamStrLen);
}

//------------------------------------------------------------------------
bool Springverb::getEffectName (char* name)
{
	vst_strncpy (name, "Gain", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool Springverb::getProductString (char* text)
{
	vst_strncpy (text, "Gain", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool Springverb::getVendorString (char* text)
{
	vst_strncpy (text, "Steinberg Media Technologies", kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 Springverb::getVendorVersion ()
{ 
	return 1000; 
}

//-----------------------------------------------------------------------------------------
void Springverb::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	float output_sample = 0;
	float input_sample = 0;
    while (--sampleFrames >= 0)
    {
		output_sample = 0;
		input_sample = *in1++;
		for(int i = 0; i < num_resonators; i++){
			output_sample += resonators[i].process(input_sample);	
		}
        (*out1++) = output_sample * (1.f / num_resonators);
        (*out2++) = output_sample * (1.f / num_resonators); 
    }
}

//-----------------------------------------------------------------------------------------
void Springverb::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	double dGain = fGain;

    while (--sampleFrames >= 0)
    {
        (*out1++) = (*in1++) * dGain;
        (*out2++) = (*in2++) * dGain;
    }
}