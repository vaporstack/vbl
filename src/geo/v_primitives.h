//
//  v_primitives.h
//  vblit
//
//  Created by vs on 2/2/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef v_primitives_h
#define v_primitives_h


#include <r4/src/geo/r_line.h>

RLine* v_primitives_generate_arc(double size, int sides, double frac, double offset);
RLine* v_primitives_generate_circle(double size, int sides, double offset);
RLine* v_primitives_generate_hex(double size, double offset);
RLine* v_primitives_generate_hex_facet(double size, double offset);
RLine* v_primitives_generate_triangle(double size, double offset);
RLine* v_primitives_generate_rect(double width, double height);
RLine* v_primitives_generate_square(double r);

RPoint3 v_primitives_random_point_on_sphere(double r);
RPoint3 v_primitives_random_point_in_sphere(double r);
RPoint3 v_primitives_random_point_in_box(double x, double y, double z);
RPoint3 v_primitives_random_point_on_box(double x, double y, double z);
void v_primitives_random_point_on_box_oldschool(double x, double y, double z, double* rx, double *ry, double* rz);

#endif /* v_primitives_h */
