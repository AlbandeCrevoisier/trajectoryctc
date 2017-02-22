/* Algorithm: get the trajectory.
 * The robot first follows an arc of a circle of given radius, the a straight
 * line, and finally another arc of a circle to achieve the demanded
 * orientation.
 */

#include "traj.h"

/* get circles of radius r to the left and right of the oriented point */
void
getcircles(Choreop cp, Circle *cl, Circle *cr)
{
	cl->p.x = cp.p.x - cp.r*sin(cp.ori);
	cl->p.y = cp.p.y + cp.r*cos(cp.ori);
	cl->r = cp.r;
	cl->acw = 1;

	cr->p.x = cp.p.x + cp.r*sin(cp.ori);
	cr->p.y = cp.p.y - cp.r*cos(cp.ori);
	cr->r = cp.r;
	cr->acw = -1;
}

/* get the right tangent points with the orientation*/
void
gettan(Tan *rett, Circle c1, Circle c2)
{
	int d1; /* distance h - c1 */
	int d2; /* distance h - c2 */
	Point h; /* homothetic center */
	Point p, q;
	Circle c, d;
	int invert = 0;

	/* homothetic center does not exist for circles of the same radius
	 * force c1.r > c2.r
	 */
	if (c1.r == c2.r) {
		c1.r++;
		c = c1;
		d = c2;
	} else if (c1.r < c2.r) {
		invert = 1;
		c = c2;
		c.acw *= -1;
		d = c1;
		d.acw *= -1;
	} else {
		c = c1;
		d = c2;
	}

	h.x = (c.r*d.p.x - c.acw*d.acw*d.r*c.p.x) /
		(c.r - c.acw*d.acw*d.r);
	h.y = (c.r*d.p.y - c.acw*d.acw*d.r*c.p.y) /
		(c.r - c.acw*d.acw*d.r);

	d1 = (h.x - c.p.x)*(h.x - c.p.x) + (h.y - c.p.y)*(h.y - c.p.y);
	d2 = (h.x - d.p.x)*(h.x - d.p.x) + (h.y - d.p.y)*(h.y - d.p.y);

	p.x = c.p.x + (c.r*c.r*(h.x-c.p.x) +
		c.acw*c.r*(h.y-c.p.y)*sqrt(d1-c.r*c.r)) / d1;
	p.y = c.p.y + (c.r*c.r*(h.y-c.p.y) -
		c.acw*c.r*(h.x-c.p.x)*sqrt(d1-c.r*c.r)) / d1;

	q.x = d.p.x + (d.r*d.r*(h.x-d.p.x) +
		c.acw*d.r*(h.y-d.p.y)*sqrt(d2-d.r*d.r)) / d2;
	q.y = d.p.y + (d.r*d.r*(h.y-d.p.y) -
		c.acw*d.r*(h.x-d.p.x)*sqrt(d2-d.r*d.r)) / d2;

	if (invert == 0) {
		rett->dep = p;
		rett->dest = q;
	} else {
		rett->dep = q;
		rett->dest = p;
	}
}

/* get the shortest tangent */
void
getshortesttan(Tan *rett, Choreop cp1, Choreop cp2)
{
	int i, imin;
	int len[4];
	Tan tll, tlr, trl, trr;
	Circle c1l, c1r, c2l, c2r;

	getcircles(cp1, &c1l, &c1r);
	getcircles(cp2, &c2l, &c2r);

	gettan(&tll, c1l, c2l);
	gettan(&tlr, c1l, c2r);
	gettan(&trl, c1r, c2l);
	gettan(&trr, c1r, c2r);

	len[0] = (tll.dep.x-tll.dest.x)*(tll.dep.x-tll.dest.x) +
		(tll.dep.y-tll.dest.y)*(tll.dep.y-tll.dest.y);
	len[1] = (tlr.dep.x-tlr.dest.x)*(tlr.dep.x-tlr.dest.x) +
		(tlr.dep.y-tlr.dest.y)*(tlr.dep.y-tlr.dest.y);
	len[2] = (trl.dep.x-trl.dest.x)*(trl.dep.x-trl.dest.x) +
		(trl.dep.y-trl.dest.y)*(trl.dep.y-trl.dest.y);
	len[3] = (trr.dep.x-trr.dest.x)*(trr.dep.x-trr.dest.x) +
		(trr.dep.y-trr.dest.y)*(trr.dep.y-trr.dest.y);

	imin = 0;
	for (i = 1; i < 4; i++)
		if (len[i] < len[imin])
			imin = i;

	switch (imin) {
	case 0:
		rett->dep.x = tll.dep.x;
		rett->dep.y = tll.dep.y;
		rett->depacw = 1;
		rett->dest.x = tll.dest.x;
		rett->dest.y = tll.dest.y;
		rett->destacw = 1;
		break;
	case 1:
		rett->dep.x = tlr.dep.x;
		rett->dep.y = tlr.dep.y;
		rett->depacw = 1;
		rett->dest.x = tlr.dest.x;
		rett->dest.y = tlr.dest.y;
		rett->destacw = -1;
		break;
	case 2:
		rett->dep.x = trl.dep.x;
		rett->dep.y = trl.dep.y;
		rett->depacw = -1;
		rett->dest.x = trl.dest.x;
		rett->dest.y = trl.dest.y;
		rett->destacw = 1;
		break;
	case 3:
		rett->dep.x = trr.dep.x;
		rett->dep.y = trr.dep.y;
		rett->depacw = -1;
		rett->dest.x = trr.dest.x;
		rett->dest.y = trr.dest.y;
		rett->destacw = -1;
		break;
	}
}

/* Return the angle of the arc of the circle between cp and p.
 * The radius is given by cp.
 * Note that the angle is not oriented
 */
 float
 getangle(Choreop cp, Point p)
 {
 	int d;

	d = sqrt((cp.p.x-p.x)*(cp.p.x-p.x) + (cp.p.y-p.y)*(cp.p.y-p.y));

	return 2*asin(d / (2.0 * cp.r));
}
