
#include "../core/NanoCSP.h"
#include "../core/Expr.hpp"

#include "examples.h"

using namespace NanoCSP;

void expr_csp()
{
	NCSolver sol;
	NCInt x(sol, 0, 5);
	NCInt y(sol, 0, 5);
	NCInt z(sol, 0, 5);
	NCInt w(sol, 0, 5);

	NCBool p(sol);

	sol.satisfy(x == 3);
	sol.satisfy(x == 3 >>= y == 1);
	sol.satisfy(p >>= x == 2);

	bool res = sol.solve();

	if(res) {
		puts("There is a solution");
		printf("x: %d, y: %d, z: %d, w: %d, p: %d\n", sol.GetIntValue(x), sol.GetIntValue(y), sol.GetIntValue(z), sol.GetIntValue(w), (int) sol.GetBoolValue(p));
	} else puts("No solution");
}
