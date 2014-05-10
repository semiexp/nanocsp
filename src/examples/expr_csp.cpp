
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

	sol.satisfy(x + (y - 2) == z);
	sol.satisfy(x == y + z);

	bool res = sol.solve();

	if(res) {
		puts("There is a solution");
		printf("x: %d, y: %d, z: %d\n", sol.GetIntValue(x), sol.GetIntValue(y), sol.GetIntValue(z));
	} else puts("No solution");
}
