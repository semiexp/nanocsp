
#include "../core/NanoCSP.h"
#include "examples.h"

#include <vector>

using namespace NanoCSP;

/*
http://www.theguardian.com/media/2010/aug/22/worlds-hardest-sudoku
This problem is described as the "worlds hardest sudoku"
 */
char problem[9][10] = {
	"..53.....",
	"8......2.",
	".7..1.5..",
	"4....53..",
	".1..7...6",
	"..32...8.",
	".6.5....9",
	"..4....3.",
	".....97..",
};

void sudoku_test()
{
	NCSolver sol;
	NCInt vals[81];

	for(int i = 0; i < 81; i++) {
		if(problem[i / 9][i % 9] == '.') vals[i] = NCInt(sol, 1, 9);
		else {
			int hint = (problem[i / 9][i % 9] - '0');
			vals[i] = NCInt(sol, hint, hint);
		}
	}

	for(int i = 0; i < 9; i++) {
		std::vector<NCInt> cond;

		//row
		for(int j = 0; j < 9; j++) {
			cond.push_back(vals[i + 9 * j]);
		}

		sol.IntAllDifferent(cond);
		cond.clear();

		//col
		for(int j = 0; j < 9; j++) {
			cond.push_back(vals[i * 9 + j]);
		}

		sol.IntAllDifferent(cond);
		cond.clear();

		//block
		for(int j = 0; j < 9; j++) {
			int h = (i / 3) * 3 + (j / 3);
			int w = (i % 3) * 3 + (j % 3);
			cond.push_back(vals[h * 9 + w]);
		}

		sol.IntAllDifferent(cond);
		cond.clear();
	}

	bool res = sol.solve();

	if(res) {
		puts("There is a solution");

		for(int i = 0; i < 9; i++) {
			for(int j = 0; j < 9; j++) {
				printf("%d ", sol.GetIntValue(vals[i * 9 + j]));
			}

			puts("");
		}
	}else puts("No solution");
}
