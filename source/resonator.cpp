/*
 *  resonator.cpp
 *  vst 2.4 examples
 *
 *  Created by Greg Surges on 1/11/12.
 *  Copyright 2012 Pucktronix. All rights reserved.
 *
 */

#include "resonator.h"
#include "public.sdk/source/vst2.x/audioeffectx.h"
#include <math.h>

/* implements a biquad BPF from RBJ's audio EQ cookbook
   http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt
 */

Resonator::Resonator(float freq, float _Q){
	cf = freq;
	Q = _Q;
	a0 = a1 = a2 = b0 = b1 = b2 = 0;
}

Resonator::Resonator(){
	a0 = a1 = a2 = b0 = b1 = b2 = 0;
	y1 = y2 = x0 = x1 = x2 = 0; 
}

void Resonator::set_sr(int SR){
	sr = SR;
}

void Resonator::set_params(float freq, float _Q){
	cf = freq;
	Q = _Q;
	
	/* compute some constants */
	w0 = 2 * 3.14159 * cf / sr;
	cosW0 = cos(w0);
	alpha = sin(w0)/(2.f*Q); // case: Q - maybe this will need some experimentation
	
	/* compute coefficients */
	// using constant 0 dB peak gain form
	b0 = alpha;
	b1 = 0;
	b2 = -1 * alpha;
	a0 = 1 + alpha;
	a1 = -2*cos(w0);
	a2 = 1 - alpha;
}

void Resonator::set_q(float _q){
	Q = _q;
	set_params(cf, Q);
}

float Resonator::process(float input_sample){
	/* get output sample */
	float output_sample = 0;
	x0 = input_sample;
	output_sample = (b0/a0) * x0 + (b1 / a0)*x1 + (b2 / a0)*x2
								 - (a1 / a0)*y1 - (a2/a0)*y2;
	x1 = x0;
	x2 = x1;
	y1 = output_sample;
	y2 = y1;
	return output_sample;	
}