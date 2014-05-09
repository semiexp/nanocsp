
#include "NanoCSP.h"
#include "../minisat/minisat_wrap.h"

namespace NanoCSP
{
	int NCSolver::AssignInt(int sz)
	{
		int top = vTop;
		vTop += sz;

		for(int i = 0; i < sz - 1; i++) {
			cond.push_back(-(top + i));
			cond.push_back(top + i + 1);
			cond.push_back(0);
		}

		return top;
	}

	bool NCSolver::solve()
	{
		/*
		for(int i=0;i<cond.size();i++) {
			printf("%d ", cond[i]);
			if(cond[i] == 0) puts("");
		}
		*/

		sol = SAT(cond);

		return sol.size() != 0;
	}

	int NCSolver::GetIntValue(NCInt& iv)
	{
		for(int i = 0; i < (iv.nMax - iv.nMin); i++) {
			if(sol[iv.vTop + i - 1]) return i + iv.nMin;
		}
		return iv.nMax;
	}

	void NCSolver::IntEqual(NCInt& opLeft, NCInt& opRight, std::vector<int>& prem = std::vector<int>())
	{
	}

	void NCSolver::AddLessEqual(NCInt& addLeft, NCInt& addRight, NCInt& opRight, std::vector<int>& prem)
	{
		// (addLeft + addRight) <= opRight

		for(int n = opRight.nMin; n <= opRight.nMax; n++) {
			for(int m = addLeft.nMin - 1; m < addLeft.nMax; m++) {
				int condZ = opRight.LessEqualThan(n);
				int condX = addLeft.LessEqualThan(m);

				if(addRight.AlwaysHoldsLessEqualThan(n - m - 1)) continue;
				int condY = addRight.LessEqualThan(n - m - 1);

				for(int i = 0; i < prem.size(); i++) cond.push_back(prem[i]);

				if(condZ != 0) cond.push_back(-condZ);
				if(condX != 0) cond.push_back(condX);
				if(condY != 0) cond.push_back(condY);

				cond.push_back(0);
			}
		}
	}

	void NCSolver::AddGtrEqual(NCInt& addLeft, NCInt& addRight, NCInt& opRight, std::vector<int>& prem)
	{
		// (addLeft + addRight) >= opRight

		for(int n = opRight.nMin; n <= opRight.nMax; n++) {
			for(int m = addLeft.nMin + 1; m <= addLeft.nMax + 1; m++) {
				int condZ = opRight.GtrEqualThan(n);
				int condX = addLeft.GtrEqualThan(m);

				if(addRight.AlwaysHoldsGtrEqualThan(n - m + 1)) continue;
				int condY = addRight.GtrEqualThan(n - m + 1);

				for(int i = 0; i < prem.size(); i++) cond.push_back(prem[i]);

				if(condZ != 0) cond.push_back(-condZ);
				if(condX != 0) cond.push_back(condX);
				if(condY != 0) cond.push_back(condY);

				cond.push_back(0);
			}
		}
	}

	void NCSolver::AddEqual(NCInt& addLeft, NCInt& addRight, NCInt& opRight, std::vector<int>& prem)
	{
		AddLessEqual(addLeft, addRight, opRight, prem);
		AddGtrEqual(addLeft, addRight, opRight, prem);
	}
};
