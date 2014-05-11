
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
		// NC_INT_GTR,			// (int > int) : bool (not necessary)
		// NC_INT_GTR_EQUAL,	// (int >= int) : bool (not necessary)
	};
	
	// expr representing an int value
	template <typename T1, typename T2, int Op>
	struct NCIntOpExpr
	{
		T1 v1; T2 v2;

		NCIntOpExpr(T1 v1, T2 v2) : v1 (v1), v2 (v2) {} 

		void apply(NCSolver *sol);
		NCInt trans(NCSolver *sol)
		{
			NCInt tr1, tr2, ret;
			switch(Op)
			{
			case NC_INT_ADD:
				tr1 = v1.trans(sol);
				tr2 = v2.trans(sol);

				ret = NCInt(*sol, tr1.nMin + tr2.nMin, tr1.nMax + tr2.nMax);
				sol->AddEqual(tr1, tr2, ret);

				return ret;

			case NC_INT_SUBTRACT:
				tr1 = v1.trans(sol);
				tr2 = v2.trans(sol);

				ret = NCInt(*sol, tr1.nMin - tr2.nMax, tr1.nMax - tr2.nMin);
				sol->AddEqual(ret, tr2, tr1);

				return ret;

			case NC_INT_VARIABLE:
				break;
			}

			return NCInt();
		};
	};

	// expr representing a bool value
	template <typename T1, typename T2, int Op>
	struct NCBoolOpExpr
	{
		T1 v1; T2 v2;

		NCBoolOpExpr(T1 v1, T2 v2) : v1 (v1), v2 (v2) {} 

		void apply(NCSolver *sol)
		{
			union {
				struct { NCInt tri1, tri2; };
				struct { NCBool trb1, trb2; };
			};

			switch(Op)
			{
			case NC_INT_EQUAL:
				tri1 = v1.trans(sol);
				tri2 = v2.trans(sol);

				sol->IntEqual(tri1, tri2);
				break;

			case NC_INT_NOT_EQUAL:
				tri1 = v1.trans(sol);
				tri2 = v2.trans(sol);

				sol->IntNotEqual(tri1, tri2);
				break;

			case NC_INT_LESS:
				tri1 = v1.trans(sol);
				tri2 = v2.trans(sol);

				sol->LessThan(tri1, tri2);
				break;

			case NC_INT_LESS_EQUAL:
				tri1 = v1.trans(sol);
				tri2 = v2.trans(sol);

				sol->LessEqualThan(tri1, tri2);
				break;
			}
		}

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

	typedef NCIntOpExpr <NCInt, void, NC_INT_VARIABLE> NCIntVarExpr;
	static NCIntVarExpr ExprOfInt (NCInt v) { return NCIntVarExpr(v); }

	// x + y
	/*
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
	*/

	// (x : IntExpr + y : IntExpr) == z : IntExpr of IntVariable
	template <typename Ta, typename Tb, int Op1, typename Tc, typename Td, int Op2>
	struct NCBoolOpExpr <NCIntOpExpr<NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, NC_INT_ADD>, NCIntVarExpr, NC_INT_EQUAL>
	{
		NCIntOpExpr<NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, NC_INT_ADD> v1;
		NCIntVarExpr v2;

		NCBoolOpExpr(NCIntOpExpr<NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, NC_INT_ADD> v1, NCIntVarExpr v2) : v1 (v1), v2 (v2) {}

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
	struct NCBoolOpExpr <NCIntOpExpr<NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, NC_INT_SUBTRACT>, NCIntVarExpr, NC_INT_EQUAL>
	{
		NCIntOpExpr<NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, NC_INT_SUBTRACT> v1;
		NCIntVarExpr v2;

		NCBoolOpExpr(NCIntOpExpr<NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, NC_INT_SUBTRACT> v1, NCIntVarExpr v2) : v1 (v1), v2 (v2) {}

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
	static NCIntOpExpr< NCIntVarExpr, NCIntOpExpr<Tc, Td, Op2>, ID> operator OP (const NCInt v1, const NCIntOpExpr<Tc, Td, Op2> v2) { \
		return NCIntOpExpr< NCIntVarExpr, NCIntOpExpr<Tc, Td, Op2>, ID> (ExprOfInt(v1), v2); \
	} \
	\
	template <typename Ta, typename Tb, int Op1> \
	static NCIntOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCIntVarExpr, ID> operator OP (const NCIntOpExpr<Ta, Tb, Op1> v1, const NCInt v2) { \
		return NCIntOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCIntVarExpr, ID> (v1, ExprOfInt(v2)); \
	} \
	\
	static NCIntOpExpr< NCIntVarExpr, NCIntVarExpr, ID> operator OP (const NCInt v1, const NCInt v2) { \
		return NCIntOpExpr< NCIntVarExpr, NCIntVarExpr, ID> (ExprOfInt(v1), ExprOfInt(v2)); \
	} \

	defIntIntOperator (NC_INT_ADD, +);
	defIntIntOperator (NC_INT_SUBTRACT, -);

#define defIntBoolOperator(ID, OP) \
	template <typename Ta, typename Tb, int Op1, typename Tc, typename Td, int Op2> \
	static NCBoolOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, ID> operator OP (const NCIntOpExpr<Ta, Tb, Op1> v1, const NCIntOpExpr<Tc, Td, Op2> v2) { \
		return NCBoolOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, ID> (v1, v2); \
	} \
	template <typename Tc, typename Td, int Op2> \
	static NCBoolOpExpr< NCIntVarExpr, NCIntOpExpr<Tc, Td, Op2>, ID> operator OP (const NCInt v1, const NCIntOpExpr<Tc, Td, Op2> v2) { \
		return NCBoolOpExpr< NCIntVarExpr, NCIntOpExpr<Tc, Td, Op2>, ID> (ExprOfInt(v1), v2); \
	} \
	\
	template <typename Ta, typename Tb, int Op1> \
	static NCBoolOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCIntVarExpr, ID> operator OP (const NCIntOpExpr<Ta, Tb, Op1> v1, const NCInt v2) { \
		return NCBoolOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCIntVarExpr, ID> (v1, ExprOfInt(v2)); \
	} \
	\
	static NCBoolOpExpr< NCIntVarExpr, NCIntVarExpr, ID> operator OP (const NCInt v1, const NCInt v2) { \
		return NCBoolOpExpr< NCIntVarExpr, NCIntVarExpr, ID> (ExprOfInt(v1), ExprOfInt(v2)); \
	} \

	defIntBoolOperator (NC_INT_EQUAL, ==);
	defIntBoolOperator (NC_INT_NOT_EQUAL, !=);
	defIntBoolOperator (NC_INT_LESS, <);
	defIntBoolOperator (NC_INT_LESS_EQUAL, <=);

#define defIntBoolOperatorFlip(ID, OP) \
	template <typename Ta, typename Tb, int Op1, typename Tc, typename Td, int Op2> \
	static NCBoolOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, ID> operator OP (const NCIntOpExpr<Ta, Tb, Op1> v1, const NCIntOpExpr<Tc, Td, Op2> v2) { \
		return NCBoolOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, ID> (v2, v1); \
	} \
	template <typename Tc, typename Td, int Op2> \
	static NCBoolOpExpr< NCIntOpExpr<Tc, Td, Op2>, NCIntVarExpr, ID> operator OP (const NCInt v1, const NCIntOpExpr<Tc, Td, Op2> v2) { \
		return NCBoolOpExpr< NCIntOpExpr<Tc, Td, Op2>, NCIntVarExpr, ID> (v2, ExprOfInt(v1)); \
	} \
	\
	template <typename Ta, typename Tb, int Op1> \
	static NCBoolOpExpr< NCIntVarExpr, NCIntOpExpr<Ta, Tb, Op1>, ID> operator OP (const NCIntOpExpr<Ta, Tb, Op1> v1, const NCInt v2) { \
		return NCBoolOpExpr< NCIntVarExpr, NCIntOpExpr<Ta, Tb, Op1>, ID> (ExprOfInt(v2), v1); \
	} \
	\
	static NCBoolOpExpr< NCIntVarExpr, NCIntVarExpr, ID> operator OP (const NCInt v1, const NCInt v2) { \
		return NCBoolOpExpr< NCIntVarExpr, NCIntVarExpr, ID> (ExprOfInt(v2), ExprOfInt(v1)); \
	} \

	defIntBoolOperatorFlip (NC_INT_LESS, >);
	defIntBoolOperatorFlip (NC_INT_LESS_EQUAL, >=);
};

