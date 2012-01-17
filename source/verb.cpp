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
: AudioEffectX (audioMaster, 1, kNumParams)	// 1 program, 1 parameter only
{
	setNumInputs (2);		// stereo in
	setNumOutputs (2);		// stereo out
	setUniqueID ('pkvr');	// identify
	canProcessReplacing ();	// supports replacing output
	canDoubleReplacing ();	// supports double precision processing

	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
	max_resonators = num_resonators = 500;
	resonators = new Resonator[num_resonators];
	for(int i = 0; i < max_resonators; i++){
		resonators[i].set_sr(getSampleRate());
		resonators[i].set_params(10 * i, 1.f);	
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
	
	switch(index){
		case kNumResonators:
			num_resonators = value * max_resonators;
			break;
		case kDecayTime:
			decay_time = 10 * value;
			for(int i = 0; i < max_resonators; i++){
				resonators[i].set_q(value * 10.f);	
			}
			break;
	}
}

//-----------------------------------------------------------------------------------------
float Springverb::getParameter (VstInt32 index)
{
	switch(index){
		case kNumResonators:
			return (float)num_resonators / (float)max_resonators;
		case kDecayTime:
			return decay_time / 10.f;
	}
}

//-----------------------------------------------------------------------------------------
void Springverb::getParameterName (VstInt32 index, char* label)
{
	switch (index) {
		case kNumResonators:
			vst_strncpy (label, "Resonators", kVstMaxParamStrLen);
			break;
		case kDecayTime:
			vst_strncpy (label, "Decay time", kVstMaxParamStrLen);			
			break;
	}
}

//-----------------------------------------------------------------------------------------
void Springverb::getParameterDisplay (VstInt32 index, char* text)
{
	switch(index){
		case kNumResonators:	
			int2string(num_resonators, text, kVstMaxParamStrLen);
			break;
		case kDecayTime:
			float2string(decay_time, text, kVstMaxParamStrLen);
			break;
	}
}

//-----------------------------------------------------------------------------------------
void Springverb::getParameterLabel (VstInt32 index, char* label)
{
	switch(index){
		case kNumResonators:
			vst_strncpy (label, "#", kVstMaxParamStrLen);
			break;
		case kDecayTime:
			vst_strncpy (label, "%", kVstMaxParamStrLen);
			break;
	}
}

//------------------------------------------------------------------------
bool Springverb::getEffectName (char* name)
{
	vst_strncpy (name, "pucktronix.reverb", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool Springverb::getProductString (char* text)
{
	vst_strncpy (text, "pucktronix.reverb", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool Springverb::getVendorString (char* text)
{
	vst_strncpy (text, "pucktronix", kVstMaxVendorStrLen);
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

}
