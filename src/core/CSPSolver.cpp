
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
		for(int i=0;i<cond.size();i++) {
			printf("%d ", cond[i]);
			if(cond[i] == 0) puts("");
		}

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
	
	bool NCSolver::GetBoolValue(NCBool& bv)
	{
		if(bv.vId < 0) return !sol[bv.vId - 1];
		return sol[bv.vId - 1];
	}

	void NCSolver::IntEqual(NCInt& opLeft, NCInt& opRight, std::vector<int>& prem)
	{
		LessEqualThan(opLeft, opRight, prem);
		GtrEqualThan(opLeft, opRight, prem);
	}

	void NCSolver::IntNotEqual(NCInt& opLeft, NCInt& opRight, std::vector<int>& prem)
	{
		NCBool c1(*this);
		NCBool c2(*this);

		cond.push_back(c1.vId);
		cond.push_back(c2.vId);
		cond.push_back(0);

		prem.push_back(-c1.vId);
		GtrThan(opLeft, opRight, prem);

		prem.pop_back();
		prem.push_back(-c2.vId);
		LessThan(opLeft, opRight, prem);

		prem.pop_back();
	}

	void NCSolver::IntAllDifferent(std::vector<NCInt>& vals, std::vector<int>& prem)
	{
		for(int i = 0; i < vals.size(); i++)
			for(int j = i + 1; j < vals.size(); j++)
				IntNotEqual(vals[i], vals[j], prem);
	}

	void NCSolver::LessEqualThan(NCInt& opLeft, NCInt& opRight, std::vector<int>& prem)
	{
		// opLeft <= opRight

		for(int n = opRight.nMin; n <= opRight.nMax; n++) {
			int condY = opRight.LessEqualThan(n);

			if(opLeft.AlwaysHoldsLessEqualThan(n)) continue;
			int condX = opLeft.LessEqualThan(n);

			for(int i = 0; i < prem.size(); i++) cond.push_back(prem[i]);
			if(condY != 0) cond.push_back(-condY);
			if(condX != 0) cond.push_back(condX);

			cond.push_back(0);
		}
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
