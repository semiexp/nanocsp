// nanocsp.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "core/NanoCSP.h"

using namespace NanoCSP;

int main(int argc, char** argv)
{
	NCSolver sol;

	NCInt x(sol, 1, 5);
	NCInt y(sol, 2, 6);
	NCInt z(sol, 0, 4);
	NCInt w(sol, 2, 5);

	sol.AddEqual(x, y, z);
	sol.AddEqual(z, y, w);

	bool res = sol.solve();

	if(res) {
		puts("There is a solution");
		printf("x: %d, y: %d, z: %d, w: %d\n", sol.GetIntValue(x), sol.GetIntValue(y), sol.GetIntValue(z), sol.GetIntValue(w));
	} else puts("No solution");

	return 0;
}

