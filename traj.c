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
	cl->toleft = 1;

	cr->p.x = cp.p.x + cp.r*sin(cp.ori);
	cr->p.y = cp.p.y - cp.r*cos(cp.ori);
	cr->r = cp.r;
	cr->toleft = -1;
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
		c.toleft *= -1;
		d = c1;
		d.toleft *= -1;
	} else {
		c = c1;
		d = c2;
	}

	h.x = (c.r*d.p.x - c.toleft*d.toleft*d.r*c.p.x) /
		(c.r - c.toleft*d.toleft*d.r);
	h.y = (c.r*d.p.y - c.toleft*d.toleft*d.r*c.p.y) /
		(c.r - c.toleft*d.toleft*d.r);

	d1 = (h.x - c.p.x)*(h.x - c.p.x) + (h.y - c.p.y)*(h.y - c.p.y);
	d2 = (h.x - d.p.x)*(h.x - d.p.x) + (h.y - d.p.y)*(h.y - d.p.y);

	p.x = c.p.x + (c.r*c.r*(h.x-c.p.x) +
		c.toleft*c.r*(h.y-c.p.y)*sqrt(d1-c.r*c.r)) / d1;
	p.y = c.p.y + (c.r*c.r*(h.y-c.p.y) -
		c.toleft*c.r*(h.x-c.p.x)*sqrt(d1-c.r*c.r)) / d1;

	q.x = d.p.x + (d.r*d.r*(h.x-d.p.x) +
		c.toleft*d.r*(h.y-d.p.y)*sqrt(d2-d.r*d.r)) / d2;
	q.y = d.p.y + (d.r*d.r*(h.y-d.p.y) -
		c.toleft*d.r*(h.x-d.p.x)*sqrt(d2-d.r*d.r)) / d2;

	if (invert == 0) {
		rett->p1 = p;
		rett->p2 = q;
	} else {
		rett->p1 = q;
		rett->p2 = p;
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

	len[0] = (tll.p1.x-tll.p2.x)*(tll.p1.x-tll.p2.x) +
		(tll.p1.y-tll.p2.y)*(tll.p1.y-tll.p2.y);
	len[1] = (tlr.p1.x-tlr.p2.x)*(tlr.p1.x-tlr.p2.x) +
		(tlr.p1.y-tlr.p2.y)*(tlr.p1.y-tlr.p2.y);
	len[2] = (trl.p1.x-trl.p2.x)*(trl.p1.x-trl.p2.x) +
		(trl.p1.y-trl.p2.y)*(trl.p1.y-trl.p2.y);
	len[3] = (trr.p1.x-trr.p2.x)*(trr.p1.x-trr.p2.x) +
		(trr.p1.y-trr.p2.y)*(trr.p1.y-trr.p2.y);

	imin = 0;
	for (i = 1; i < 4; i++)
		if (len[i] < len[imin])
			imin = i;

	switch (imin) {
	case 0:
		rett->p1.x = tll.p1.x;
		rett->p1.y = tll.p1.y;
		rett->toleft1 = 1;
		rett->p2.x = tll.p2.x;
		rett->p2.y = tll.p2.y;
		rett->toleft2 = 1;
		break;
	case 1:
		rett->p1.x = tlr.p1.x;
		rett->p1.y = tlr.p1.y;
		rett->toleft1 = 1;
		rett->p2.x = tlr.p2.x;
		rett->p2.y = tlr.p2.y;
		rett->toleft2 = -1;
		break;
	case 2:
		rett->p1.x = trl.p1.x;
		rett->p1.y = trl.p1.y;
		rett->toleft1 = -1;
		rett->p2.x = trl.p2.x;
		rett->p2.y = trl.p2.y;
		rett->toleft2 = 1;
		break;
	case 3:
		rett->p1.x = trr.p1.x;
		rett->p1.y = trr.p1.y;
		rett->toleft1 = -1;
		rett->p2.x = trr.p2.x;
		rett->p2.y = trr.p2.y;
		rett->toleft2 = -1;
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
