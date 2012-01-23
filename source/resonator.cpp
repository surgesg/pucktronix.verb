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

/* implements a biquad resonant LPF from RBJ's audio EQ cookbook
   http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt
 
 decay time sketchpad
 Q = number of oscillations required for energy to fall to 1 / e^(2 * pi)

 decay_time = Q * (1 / fc)
 Q = decay_time / (1 / fc)
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

void Resonator::set_params(float freq, float _decay){
	cf = freq;
	decay = _decay;
	Q = (float)decay / (1.0 / cf); // works?
	
	/* compute some constants */
	w0 = (2.f * 3.14159 * cf) / sr;
	alpha = sinW0/(2.f*Q); // case: Q - maybe this will need some experimentation
	cosW0 = cos(w0);
	sinW0 = sin(w0);
	
	/* compute coefficients */
	// using constant 0 dB peak gain form
	b0 = (1 - cosW0) / 2.0;
	b1 = 1.0 - cosW0;
	b2 = (1 - cosW0) / 2.0;
	a0 = 1 + alpha;
	a1 = -2.0 * cosW0;
	a2 = 1.0 - alpha;
}

void Resonator::set_q(float _q){
	Q = _q;
	set_params(cf, Q);
}

float Resonator::process(float input_sample){
	/* get output sample */
	output_sample = 0;
	x0 = input_sample;
	output_sample = (b0 / a0) * x0 + (b1 / a0)*x1 + (b2 / a0)*x2
								 - (a1/a0)*y1 - (a2/a0)*y2;
	x2 = x1;
	x1 = x0;
	y2 = y1;
	y1 = output_sample;
	return output_sample;	
}