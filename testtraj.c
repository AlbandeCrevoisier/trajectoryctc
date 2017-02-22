#include <stdlib.h>
#include <stdio.h>
#include "traj.h"

#define PI 3.141592653589793238

int
main(void)
{
	char cmd;
	float a;
	Choreop cp1, cp2;
	Circle c1, c2;
	Tan t;

	for (;;) {
		scanf("%c", &cmd);
		switch (cmd) {
		case '?':
			printf("c: return both circles: Choreop\n");
			printf("t: return the tangent: Circle, Circle\n");
			printf("s: return the shortest tangent: ");
			printf("Choreop, Choreop\n");
			printf("a: return the angle: Choreop, Point\n");
			printf("q: quit\n");
			break;
		case 'c':
			printf("cp: x y ori r: ");
			scanf("%d %d %f %d",
				&cp1.p.x, &cp1.p.y, &cp1.ori, &cp1.r);
			getcircles(cp1, &c1, &c2);
			printf("cl: (%d,%d), %d, %d\n",
				c1.p.x, c1.p.y, c1.r, c1.acw);
			printf("cr: (%d,%d), %d, %d\n",
				c2.p.x, c2.p.y, c2.r, c2.acw);
			break;
		case 't':
			printf("c1: x y r acw: ");
			scanf("%d %d %d %d",
				&c1.p.x, &c1.p.y, &c1.r, &c1.acw);
			printf("c2: x y r acw: ");
			scanf("%d %d %d %d",
				&c2.p.x, &c2.p.y, &c2.r, &c2.acw);
			gettan(&t, c1, c2);
			printf("t1:(%d,%d), t2(%d,%d)\n",
				t.dep.x, t.dep.y, t.dest.x, t.dest.y);
			break;
		case 's':
			printf("cp1: x y ori r: ");
			scanf("%d %d %f %d",
				&cp1.p.x, &cp1.p.y, &cp1.ori, &cp1.r);
			printf("cp2: x y ori r: ");
			scanf("%d %d %f %d",
				&cp2.p.x, &cp2.p.y, &cp2.ori, &cp2.r);
			getshortesttan(&t, cp1, cp2);
			printf("t1:(%d,%d), %d ", t.dep.x, t.dep.y, t.depacw);
			printf("t2:(%d,%d), %d\n", t.dest.x, t.dest.y, t.destacw);
			break;
		case 'a':
			printf("cp: x y r: ");
			scanf("%d %d %d", &cp1.p.x, &cp1.p.y, &cp1.r);
			printf("p: x y: ");
			scanf("%d %d", &cp1.p.x, &cp1.p.y);
			a = getangle(cp1, cp2.p);
			printf("angle: %f\n", a);
			break;
		case 'q':
			return 0;
			break;
		}
	}
	return 0;
}
