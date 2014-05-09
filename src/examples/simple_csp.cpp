
#include "../core/NanoCSP.h"
#include "examples.h"

using namespace NanoCSP;


void simple_csp()
{
	NCSolver sol;

	NCInt x(sol, 1, 5);
	NCInt y(sol, 2, 6);
	NCInt z(sol, 0, 3);
	NCInt w(sol, 2, 3);

	sol.AddEqual(x, y, z);
	sol.IntNotEqual(z, w);

	bool res = sol.solve();

	if(res) {
		puts("There is a solution");
		printf("x: %d, y: %d, z: %d, w: %d\n", sol.GetIntValue(x), sol.GetIntValue(y), sol.GetIntValue(z), sol.GetIntValue(w));
	} else puts("No solution");
}
