
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
	};

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

		void IntEqual(NCInt& opLeft, NCInt& opRight, std::vector<int>& prem);
		void LessThan(NCInt& opLeft, NCInt& opRight, std::vector<int>& prem);
		void GtrThan(NCInt& opLeft, NCInt& opRight, std::vector<int>& prem) { LessThan(opRight, opLeft, prem); }
		void AddLessEqual(NCInt& addLeft, NCInt& addRight, NCInt& opRight, std::vector<int>& prem = std::vector<int>());
		void AddGtrEqual(NCInt& addLeft, NCInt& addRight, NCInt& opRight, std::vector<int>& prem = std::vector<int>());
		void AddEqual(NCInt& addLeft, NCInt& addRight, NCInt& opRight, std::vector<int>& prem = std::vector<int>());

		bool solve();
		int GetIntValue(NCInt& iv);
	};

	/* (* TODO: codes below are buggy; they shouldn't be used *) */

	NCExpr operator+(NCExpr& left, NCExpr& right);
	NCExpr operator-(NCExpr& left, NCExpr& right);
	NCExpr operator==(NCExpr& left, NCExpr& right);
	NCExpr operator!=(NCExpr& left, NCExpr& right);
	NCExpr operator<(NCExpr& left, NCExpr& right);
	NCExpr operator<=(NCExpr& left, NCExpr& right);
	NCExpr operator>(NCExpr& left, NCExpr& right);
	NCExpr operator>=(NCExpr& left, NCExpr& right);
	NCExpr operator+(NCBool& left, NCExpr& right);

	class NCExpr
	{
		friend NCExpr operator+(NCExpr& left, NCExpr& right);
		friend NCExpr operator-(NCExpr& left, NCExpr& right);
		friend NCExpr operator==(NCExpr& left, NCExpr& right);
		friend NCExpr operator!=(NCExpr& left, NCExpr& right);
		friend NCExpr operator<(NCExpr& left, NCExpr& right);
		friend NCExpr operator<=(NCExpr& left, NCExpr& right);
		friend NCExpr operator>(NCExpr& left, NCExpr& right);
		friend NCExpr operator>=(NCExpr& left, NCExpr& right);
		friend NCExpr operator+(NCBool& left, NCExpr& right);

		enum {
			NC_BOOLVAR,
			NC_INTVAR,
			NC_ADD,
			NC_SUBSTRACT,
			NC_EQUAL,
			NC_NOT_EQUAL,
			NC_LESS,
			NC_LESSEQUAL,
			NC_GREATER,
			NC_GREATEREQUAL
		};
		union {
			struct{ NCExpr *opLeft, *opRight; };
			NCBool *boolTarget;
			NCInt *intTarget;
		};
		int opKind;

		NCExpr (int opKind, NCExpr *opLeft, NCExpr *opRight) : opKind(opKind), opLeft(opLeft), opRight(opRight) {}
		NCExpr (int opKind) : opKind(opKind), opLeft(NULL), opRight(NULL) {}
	public:
		NCExpr (NCBool& v)
		{
			boolTarget = &v;
			opKind = NC_BOOLVAR;
		}

		NCExpr (NCInt& v)
		{
			intTarget = &v;
			opKind = NC_INTVAR;
		}

		void print(std::ostream &ofs);
	};
};

