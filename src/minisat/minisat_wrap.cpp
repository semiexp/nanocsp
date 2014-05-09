#include "core/Solver.h"

#include "minisat_wrap.h"
#include <vector>

using namespace Minisat;

namespace NanoCSP
{
	std::vector<bool> SAT(std::vector<int> cond)
	{
		Solver S;

		vec<Lit> lits;
		for(int i=0;i<cond.size();i++){
			if(cond[i] == 0) {
				S.addClause_(lits);

				lits.clear();
			} else {
				int plit = cond[i];
				int var = abs(plit) - 1;
				while(var >= S.nVars()) S.newVar();
				lits.push(plit > 0 ? mkLit(var) : ~mkLit(var));
			}
		}

		if (!S.simplify()){
			return std::vector<bool>();
		}

		vec<Lit> dummy;
		lbool ret = S.solveLimited(dummy);

		if (ret == l_True){
			std::vector<bool> ret;
			for(int i=0;i<S.nVars();i++) ret.push_back(S.model[i] == l_True);

			return ret;
		}

		return std::vector<bool>();
	}
};
