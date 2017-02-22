#ifndef TRAJ_H
#define TRAJ_H

#include <math.h>

typedef struct {
	int x;
	int y;
} Point;

/* Tangent
 * acw: anticlockwise ? 1 : -1
 */
typedef struct {
	Point dep;
	int depacw;
	Point dest;
	int destacw;
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
	int acw;
} Circle;

void	getcircles(Choreop cp, Circle *cl, Circle *cr);
void	gettan(Tan *rett, Circle c1, Circle c2);
void	getshortesttan(Tan *rett, Choreop cp1, Choreop cp2);
float	getangle(Choreop cp, Point p);

#endif /* TRAJ_H */
