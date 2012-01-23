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
	Resonator(float freq, float Q);
	Resonator();
	void set_params(float freq, float decay);
	void set_sr(int SR);
	void set_q(float Q);
	float process(float input_sample);
	float Q, cf;
private:
	float sr;
	float a0, a1, a2, b0, b1, b2;
	float w0;
	float cosW0, sinW0;
	float alpha;
	float x0, x1, x2, y1, y2;
	float c;
	float output_sample;
	float decay;
};