#ifndef TRAJ_H
#define TRAJ_H

#include "trigo.h"
#include <math.h>

typedef struct {
	int x;
	int y;
} Point;

/* Tangent */
typedef struct {
	Point p1;
	int toleft1;
	Point p2;
	int toleft2;
} Tan;

/* Point of the choreography, with orientation and radius */
typedef struct {
	Point p;
	float ori;
	int r;
} Choreop;

typedef struct {
	Point p;
	int r;
	int toleft;
} Circle;

void	getcircles(Choreop cp, Circle *cl, Circle *cr);
Tan	gettan(Circle c1, Circle c2);
Tan	getshortesttan(Choreop cp1, Choreop cp2);
float	getangle(Choreop cp, Point p);

#endif /* TRAJ_H */
