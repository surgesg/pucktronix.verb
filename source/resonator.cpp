/*
 *  resonator.cpp
 *  vst 2.4 examples
 *
 *  Created by Greg Surges on 1/11/12.
 *  Copyright 2012 Pucktronix. All rights reserved.
 *
 */

#include "resonator.h"

Resonator::Resonator(int del_time, float decay_coef){
	delay_line = new float[del_time];
	delay_time = del_time;
	decay_time = decay_coef;
	write_ptr = 0;
	read_ptr = 1;
}

Resonator::Resonator(){
	
}

void Resonator::set_params(int del_time, float decay_coef){
	
	delay_line = new float[del_time];
	for(int i = 0; i < del_time; i++){ // zero out for noise
		delay_line[i] = 0;	
	}
	delay_time = del_time;
	decay_time = decay_coef;
	write_ptr = 0;
	read_ptr = 0;
}

float Resonator::process(float input_sample){
	/* get output sample */
	float output_sample;
	output_sample = delay_line[read_ptr];
	delay_line[read_ptr] = input_sample + output_sample * decay_time;
	read_ptr++;
	if(read_ptr >= delay_time){
		read_ptr = 0;
	}
	return output_sample;	
}