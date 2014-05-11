
#include "../core/NanoCSP.h"
#include "../core/Expr.hpp"

#include "examples.h"
#include <vector>

using namespace NanoCSP;

void send_more_money()
{
	NCSolver sol;

	NCInt S(sol, 1, 9), E(sol, 0, 9), N(sol, 0, 9), D(sol, 0, 9), M(sol, 1, 9), O(sol, 0, 9), R(sol, 0, 9), Y(sol, 0, 9);

	std::vector<NCInt> vec;
	vec.push_back(S);
	vec.push_back(E);
	vec.push_back(N);
	vec.push_back(D);
	vec.push_back(M);
	vec.push_back(O);
	vec.push_back(R);
	vec.push_back(Y);

	sol.IntAllDifferent(vec);

	NCInt car1(sol, 0, 1), car2(sol, 0, 1), car3(sol, 0, 1), car4(sol, 0, 1);

	//D + E == 10 * car1 + Y
	sol.satisfy(car1 == 0 >>= D + E == Y);
	sol.satisfy(car1 == 1 >>= D + E == 10 + Y);

	//car1 + N + R == 10 * car2 + E
	sol.satisfy(car2 == 0 >>= car1 + N + R == E);
	sol.satisfy(car2 == 1 >>= car1 + N + R == 10 + E);

	//car2 + E + O == 10 * car3 + N
	sol.satisfy(car3 == 0 >>= car2 + E + O == N);
	sol.satisfy(car3 == 1 >>= car2 + E + O == 10 + N);

	//car3 + S + M == 10 * car4 + O
	sol.satisfy(car4 == 0 >>= car3 + S + M == O);
	sol.satisfy(car4 == 1 >>= car3 + S + M == 10 + O);

	//car4 == M
	sol.satisfy(car4 == M);

	if(sol.solve()) {
		printf(" %d%d%d%d\n", sol.GetIntValue(S), sol.GetIntValue(E), sol.GetIntValue(N), sol.GetIntValue(D));
		printf("+%d%d%d%d\n", sol.GetIntValue(M), sol.GetIntValue(O), sol.GetIntValue(R), sol.GetIntValue(E));
		printf("-----\n");
		printf("%d%d%d%d%d\n", sol.GetIntValue(M), sol.GetIntValue(O), sol.GetIntValue(N), sol.GetIntValue(E), sol.GetIntValue(Y));
	}else puts("No solution");
}
