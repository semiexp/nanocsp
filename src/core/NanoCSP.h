
#pragma once

#include <iostream>
#include <vector>

namespace NanoCSP
{
	class NCExpr;
	class NCSolver;

	struct NCBool
	{
		int vId;

		NCBool() : vId(-1) {}
		NCBool(int vId) : vId(vId) {}
		NCBool(NCSolver& sol);
	};

	struct NCInt
	{
		int vTop, nMin, nMax;

		NCInt() : vTop(-1) {}
		NCInt(int vTop, int nMin, int nMax) : vTop(vTop), nMin(nMin), nMax(nMax) {}
		NCInt(NCSolver& sol, int nMin, int nMax);

		bool AlwaysHoldsLessEqualThan(int x) { return nMax <= x; }
		int LessEqualThan(int x) { return (x < nMin || x >= nMax) ? 0 : (vTop + x - nMin); }
		bool AlwaysHoldsLessThan(int x) { return nMax < x; }
		int LessThan(int x) { return LessEqualThan(x - 1); }

		bool AlwaysHoldsGtrEqualThan(int x) { return x <= nMin; }
		int GtrEqualThan(int x) { return -LessThan(x); }
		bool AlwaysHoldsGtrThan(int x) { return x < nMin; }
		int GtrThan(int x) { return -LessEqualThan(x); }

		inline NCInt operator+(const int d) { return NCInt(vTop, nMin + d, nMax + d); }
		inline NCInt operator-(const int d) { return NCInt(vTop, nMin - d, nMax - d); }
	};

	template <typename T1, typename T2, int Op>
	struct NCIntOpExpr;

	template <typename T1, typename T2, int Op>
	struct NCBoolOpExpr;

	class NCSolver
	{
		friend struct NCBool;
		friend struct NCInt;

		int vTop;
		std::vector<int> cond;
		std::vector<bool> sol;

		int AssignInt(int sz);
		int Allocate(int sz) { int ret = vTop; vTop += sz; return ret; }

	public:
		NCSolver() { vTop = 1; }

		void IntEqual(NCInt& opLeft, NCInt& opRight, std::vector<int>& prem = std::vector<int>());
		void IntNotEqual(NCInt& opLeft, NCInt& opRight, std::vector<int>& prem = std::vector<int>());
		void IntAllDifferent(std::vector<NCInt>& vals, std::vector<int>& prem = std::vector<int>());
		void LessEqualThan(NCInt& opLeft, NCInt& opRight, std::vector<int>& prem = std::vector<int>());
		void LessThan(NCInt& opLeft, NCInt& opRight, std::vector<int>& prem = std::vector<int>()){ LessEqualThan(opLeft, opRight - 1, prem); }
		void GtrEqualThan(NCInt& opLeft, NCInt& opRight, std::vector<int>& prem = std::vector<int>()) { LessEqualThan(opRight, opLeft, prem); }
		void GtrThan(NCInt& opLeft, NCInt& opRight, std::vector<int>& prem = std::vector<int>()){ LessThan(opRight, opLeft, prem); }
		void AddLessEqual(NCInt& addLeft, NCInt& addRight, NCInt& opRight, std::vector<int>& prem = std::vector<int>());
		void AddGtrEqual(NCInt& addLeft, NCInt& addRight, NCInt& opRight, std::vector<int>& prem = std::vector<int>());
		void AddEqual(NCInt& addLeft, NCInt& addRight, NCInt& opRight, std::vector<int>& prem = std::vector<int>());

		bool solve();
		int GetIntValue(NCInt& iv);

		template <typename T>
		void satisfy(T expr) { expr.apply(this); }
	};

};

