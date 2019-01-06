//
//  vbl_lissajous.c
//  vbl
//
//  Created by vs on 9/22/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_lissajous.h"

#include <drw/drw.h>

#define TMP_SCALE .25

RLine3* vbl_lissajous_create3d(double sigma, double omega, double step, int cycles)
{
	
	return NULL;
}

RLine* vbl_lissajous_create(double sigma, double omega, double step, int cycles)
{
	RLine* line = r_line_create();
	for ( double i = 0; i < cycles * M_PI * 2 + step; i+= step )
	{
		double x = sin(omega*i + sigma) * TMP_SCALE;
		double y = sin(i) * TMP_SCALE;
		r_line_add_point2f(line, x, y);
		
	}
	
	return line;
}

void vbl_lissajous_test(void)
{
	
	int cycles = 10;
	double  step = .01;
	static double omega = .5;
	
	static double sigma = .2;
	sigma -= .001;
	omega += .001;
	if (omega > M_PI )
		omega = 0;
	if ( sigma <= 0)
		sigma = M_PI;
	
	RLine* line = vbl_lissajous_create(sigma, omega, step, cycles);
	drw_rline(line);
	r_line_destroy(line);
	
	/*
	ctx.beginPath();
	for(var i=0; i<(cycles * Math.PI*2) + step; i+=step) {
		var x = Math.sin(omega*i + sigma);
		var y = Math.sin(i);
		var scale=200;
		ctx.lineTo(c_w/2 + (x*scale), c_h/2 + (y*scale));
	}
	ctx.stroke();
	*/
}
