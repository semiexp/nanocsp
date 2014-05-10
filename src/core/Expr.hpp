
#pragma once

#include "NanoCSP.h"
#include <iostream>

namespace NanoCSP
{
	enum
	{
		NC_INT_VARIABLE,	// representing an existing int variable; NCInt -> NCIntOpExpr
		NC_INT_ADD,			// (int + int) : int
		NC_INT_SUBTRACT,	// (int - int) : int
		NC_INT_EQUAL,		// (int == int) : bool
		NC_INT_NOT_EQUAL,	// (int != int) : bool
		NC_INT_LESS,		// (int < int) : bool
		NC_INT_LESS_EQUAL,	// (int <= int) : bool
		NC_INT_GTR,			// (int > int) : bool
		NC_INT_GTR_EQUAL,	// (int >= int) : bool
	};
	
	// expr representing an int value
	template <typename T1, typename T2, int Op>
	struct NCIntOpExpr
	{
		T1 v1; T2 v2;

		NCIntOpExpr(T1 v1, T2 v2) : v1 (v1), v2 (v2) {} 

		void apply(NCSolver *sol);
		NCInt trans(NCSolver *sol);
	};

	// expr representing a bool value
	template <typename T1, typename T2, int Op>
	struct NCBoolOpExpr
	{
		T1 v1; T2 v2;

		NCBoolOpExpr(T1 v1, T2 v2) : v1 (v1), v2 (v2) {} 

		void apply(NCSolver *sol);
		NCBool trans(NCSolver *sol);
	};

	template <>
	struct NCIntOpExpr <NCInt, void, NC_INT_VARIABLE>
	{
		NCInt v1;

		NCIntOpExpr(NCInt v1) : v1 (v1) {}

		void apply(NCSolver *sol);
		NCInt trans(NCSolver *sol) { return v1; }
	};

	static NCIntOpExpr <NCInt, void, NC_INT_VARIABLE> ExprOfInt (NCInt v) { return NCIntOpExpr <NCInt, void, NC_INT_VARIABLE> (v); }

	// x + y
	template <typename Ta, typename Tb, int Op1, typename Tc, typename Td, int Op2>
	struct NCIntOpExpr <NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, NC_INT_ADD>
	{
		NCIntOpExpr<Ta, Tb, Op1> v1; NCIntOpExpr<Tc, Td, Op2> v2;

		NCIntOpExpr(NCIntOpExpr<Ta, Tb, Op1> v1, NCIntOpExpr<Tc, Td, Op2> v2) : v1 (v1), v2 (v2) {}

		void apply(); // This expr is not applicable
		NCInt trans(NCSolver *sol)
		{
			NCInt tr1 = v1.trans(sol);
			NCInt tr2 = v2.trans(sol);

			NCInt ret(*sol, tr1.nMin + tr2.nMin, tr1.nMax + tr2.nMax);

			sol->AddEqual(tr1, tr2, ret);

			return ret;
		}
	};

	// x - y
	template <typename Ta, typename Tb, int Op1, typename Tc, typename Td, int Op2>
	struct NCIntOpExpr <NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, NC_INT_SUBTRACT>
	{
		NCIntOpExpr<Ta, Tb, Op1> v1; NCIntOpExpr<Tc, Td, Op2> v2;

		NCIntOpExpr(NCIntOpExpr<Ta, Tb, Op1> v1, NCIntOpExpr<Tc, Td, Op2> v2) : v1 (v1), v2 (v2) {}

		void apply(); // This expr is not applicable
		NCInt trans(NCSolver *sol)
		{
			NCInt tr1 = v1.trans(sol);
			NCInt tr2 = v2.trans(sol);

			NCInt ret(*sol, tr1.nMin - tr2.nMax, tr1.nMax - tr2.nMin);

			sol->AddEqual(ret, tr2, tr1);

			return ret;
		}
	};

	// (x : IntExpr + y : IntExpr) == z : IntExpr of IntVariable
	template <typename Ta, typename Tb, int Op1, typename Tc, typename Td, int Op2>
	struct NCBoolOpExpr <NCIntOpExpr<NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, NC_INT_ADD>, NCIntOpExpr<NCInt, void, NC_INT_VARIABLE>, NC_INT_EQUAL>
	{
		NCIntOpExpr<NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, NC_INT_ADD> v1;
		NCIntOpExpr<NCInt, void, NC_INT_VARIABLE> v2;

		NCBoolOpExpr(NCIntOpExpr<NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, NC_INT_ADD> v1, NCIntOpExpr<NCInt, void, NC_INT_VARIABLE> v2) : v1 (v1), v2 (v2) {}

		void apply(NCSolver *sol)
		{
			NCInt tr1 = v1.v1.trans(sol);
			NCInt tr2 = v1.v2.trans(sol);
			NCInt tr3 = v2.trans(sol);

			sol->AddEqual(tr1, tr2, tr3);
		}

		NCBool trans(NCSolver *sol)
		{
			NCInt tr1 = v1.trans(sol);
			NCInt tr2 = v2.trans(sol);
			NCBool ret(*sol);

			// (ret <=> (tr1 == tr2))
			std::vector<int> prem; prem.push_back(-ret.vId);
			sol->IntEqual(tr1, tr2, prem);

			prem[0] = -prem[0];
			sol->IntNotEqual(tr1, tr2, prem);
		}
	};

	// (x : IntExpr - y : IntExpr) == z : IntExpr of IntVariable
	template <typename Ta, typename Tb, int Op1, typename Tc, typename Td, int Op2>
	struct NCBoolOpExpr <NCIntOpExpr<NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, NC_INT_SUBTRACT>, NCIntOpExpr<NCInt, void, NC_INT_VARIABLE>, NC_INT_EQUAL>
	{
		NCIntOpExpr<NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, NC_INT_SUBTRACT> v1;
		NCIntOpExpr<NCInt, void, NC_INT_VARIABLE> v2;

		NCBoolOpExpr(NCIntOpExpr<NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, NC_INT_SUBTRACT> v1, NCIntOpExpr<NCInt, void, NC_INT_VARIABLE> v2) : v1 (v1), v2 (v2) {}

		void apply(NCSolver *sol)
		{
			NCInt tr1 = v1.v1.trans(sol);
			NCInt tr2 = v1.v2.trans(sol);
			NCInt tr3 = v2.trans(sol);

			sol->AddEqual(tr3, tr2, tr1);
		}

		NCBool trans(NCSolver *sol)
		{
			NCInt tr1 = v1.trans(sol);
			NCInt tr2 = v2.trans(sol);
			NCBool ret(*sol);

			// (ret <=> (tr1 == tr2))
			std::vector<int> prem; prem.push_back(-ret.vId);
			sol->IntEqual(tr1, tr2, prem);

			prem[0] = -prem[0];
			sol->IntNotEqual(tr1, tr2, prem);
		}
	};

	// z : IntExpr == (x : IntExpr + y : IntExpr)
	template <typename Ta, typename Tb, int Op1, typename Tc, typename Td, int Op2, typename Te, typename Tf, int Op3>
	struct NCBoolOpExpr <NCIntOpExpr<Te, Tf, Op3>, NCIntOpExpr<NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, NC_INT_ADD>, NC_INT_EQUAL>
	{
		NCIntOpExpr<Te, Tf, Op3> v1;
		NCIntOpExpr<NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, NC_INT_ADD> v2;

		NCBoolOpExpr(NCIntOpExpr<Te, Tf, Op3> v1, NCIntOpExpr<NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, NC_INT_ADD> v2) : v1 (v1), v2 (v2) {}

		void apply(NCSolver *sol)
		{
			NCInt tr1 = v2.v1.trans(sol);
			NCInt tr2 = v2.v2.trans(sol);
			NCInt tr3 = v1.trans(sol);

			sol->AddEqual(tr1, tr2, tr3);
		}

		NCBool trans(NCSolver *sol)
		{
			NCInt tr1 = v1.trans(sol);
			NCInt tr2 = v2.trans(sol);
			NCBool ret(*sol);

			// (ret <=> (tr1 == tr2))
			std::vector<int> prem; prem.push_back(-ret.vId);
			sol->IntEqual(tr1, tr2, prem);

			prem[0] = -prem[0];
			sol->IntNotEqual(tr1, tr2, prem);
		}
	};

	// z : IntExpr == (x : IntExpr - y : IntExpr)
	template <typename Ta, typename Tb, int Op1, typename Tc, typename Td, int Op2, typename Te, typename Tf, int Op3>
	struct NCBoolOpExpr <NCIntOpExpr<Te, Tf, Op3>, NCIntOpExpr<NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, NC_INT_SUBTRACT>, NC_INT_EQUAL>
	{
		NCIntOpExpr<Te, Tf, Op3> v1;
		NCIntOpExpr<NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, NC_INT_SUBTRACT> v2;

		NCBoolOpExpr(NCIntOpExpr<Te, Tf, Op3> v1, NCIntOpExpr<NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, NC_INT_SUBTRACT> v2) : v1 (v1), v2 (v2) {}

		void apply(NCSolver *sol)
		{
			NCInt tr1 = v2.v1.trans(sol);
			NCInt tr2 = v2.v2.trans(sol);
			NCInt tr3 = v1.trans(sol);

			sol->AddEqual(tr3, tr2, tr1);
		}

		NCBool trans(NCSolver *sol)
		{
			NCInt tr1 = v1.trans(sol);
			NCInt tr2 = v2.trans(sol);
			NCBool ret(*sol);

			// (ret <=> (tr1 == tr2))
			std::vector<int> prem; prem.push_back(-ret.vId);
			sol->IntEqual(tr1, tr2, prem);

			prem[0] = -prem[0];
			sol->IntNotEqual(tr1, tr2, prem);
		}
	};

#define defIntIntOperator(ID, OP) \
	template <typename Ta, typename Tb, int Op1, typename Tc, typename Td, int Op2> \
	static NCIntOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, ID> operator OP (const NCIntOpExpr<Ta, Tb, Op1> v1, const NCIntOpExpr<Tc, Td, Op2> v2) { \
		return NCIntOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, ID> (v1, v2); \
	} \
	\
	template <typename Tc, typename Td, int Op2> \
	static NCIntOpExpr< NCIntOpExpr<NCInt, void, NC_INT_VARIABLE>, NCIntOpExpr<Tc, Td, Op2>, ID> operator OP (const NCInt v1, const NCIntOpExpr<Tc, Td, Op2> v2) { \
		return NCIntOpExpr< NCIntOpExpr<NCInt, void, NC_INT_VARIABLE>, NCIntOpExpr<Tc, Td, Op2>, ID> (ExprOfInt(v1), v2); \
	} \
	\
	template <typename Ta, typename Tb, int Op1> \
	static NCIntOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<NCInt, void, NC_INT_VARIABLE>, ID> operator OP (const NCIntOpExpr<Ta, Tb, Op1> v1, const NCInt v2) { \
		return NCIntOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<NCInt, void, NC_INT_VARIABLE>, ID> (v1, ExprOfInt(v2)); \
	} \
	\
	static NCIntOpExpr< NCIntOpExpr<NCInt, void, NC_INT_VARIABLE>, NCIntOpExpr<NCInt, void, NC_INT_VARIABLE>, ID> operator OP (const NCInt v1, const NCInt v2) { \
		return NCIntOpExpr< NCIntOpExpr<NCInt, void, NC_INT_VARIABLE>, NCIntOpExpr<NCInt, void, NC_INT_VARIABLE>, ID> (ExprOfInt(v1), ExprOfInt(v2)); \
	} \

	defIntIntOperator (NC_INT_ADD, +);
	defIntIntOperator (NC_INT_SUBTRACT, -);

#define defIntBoolOperator(ID, OP) \
	template <typename Ta, typename Tb, int Op1, typename Tc, typename Td, int Op2> \
	static NCBoolOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, ID> operator OP (const NCIntOpExpr<Ta, Tb, Op1> v1, const NCIntOpExpr<Tc, Td, Op2> v2) { \
		return NCBoolOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, ID> (v1, v2); \
	} \
	template <typename Tc, typename Td, int Op2> \
	static NCBoolOpExpr< NCIntOpExpr<NCInt, void, NC_INT_VARIABLE>, NCIntOpExpr<Tc, Td, Op2>, ID> operator OP (const NCInt v1, const NCIntOpExpr<Tc, Td, Op2> v2) { \
		return NCBoolOpExpr< NCIntOpExpr<NCInt, void, NC_INT_VARIABLE>, NCIntOpExpr<Tc, Td, Op2>, ID> (ExprOfInt(v1), v2); \
	} \
	\
	template <typename Ta, typename Tb, int Op1> \
	static NCBoolOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<NCInt, void, NC_INT_VARIABLE>, ID> operator OP (const NCIntOpExpr<Ta, Tb, Op1> v1, const NCInt v2) { \
		return NCBoolOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<NCInt, void, NC_INT_VARIABLE>, ID> (v1, ExprOfInt(v2)); \
	} \
	\
	static NCBoolOpExpr< NCIntOpExpr<NCInt, void, NC_INT_VARIABLE>, NCIntOpExpr<NCInt, void, NC_INT_VARIABLE>, ID> operator OP (const NCInt v1, const NCInt v2) { \
		return NCBoolOpExpr< NCIntOpExpr<NCInt, void, NC_INT_VARIABLE>, NCIntOpExpr<NCInt, void, NC_INT_VARIABLE>, ID> (ExprOfInt(v1), ExprOfInt(v2)); \
	} \

	defIntBoolOperator (NC_INT_EQUAL, ==);
	defIntBoolOperator (NC_INT_NOT_EQUAL, !=);
	defIntBoolOperator (NC_INT_LESS, <);
	defIntBoolOperator (NC_INT_LESS_EQUAL, <=);
	defIntBoolOperator (NC_INT_GTR, >);
	defIntBoolOperator (NC_INT_GTR_EQUAL, >=);
};

