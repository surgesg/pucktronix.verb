/*
 *  resonator.h
 *  vst 2.4 examples
 *
 *  Created by Greg Surges on 1/11/12.
 *  Copyright 2012 Pucktronix. All rights reserved.
 *
 */

class Resonator {
public:
	Resonator(int del_time, float decay_coef);
	Resonator();
	void set_params(int del_time, float decay_coef);
	float process(float input_sample);
	
private:
	float * delay_line;
	int delay_time;
	float decay_time;
	int read_ptr, write_ptr;
};