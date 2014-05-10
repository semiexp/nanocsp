
#pragma once

#include "NanoCSP.h"
#include <iostream>

namespace NanoCSP
{
	enum
	{
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
	};

	// expr representing a bool value
	template <typename T1, typename T2, int Op>
	struct NCBoolOpExpr
	{
		T1 v1; T2 v2;

		NCBoolOpExpr(T1 v1, T2 v2) : v1 (v1), v2 (v2) {} 

		void apply(NCSolver *sol);
	};

	// (x + y) == z
	template <>
	struct NCBoolOpExpr <NCIntOpExpr<NCInt, NCInt, NC_INT_ADD>, NCInt, NC_INT_EQUAL>
	{
		NCIntOpExpr<NCInt, NCInt, NC_INT_ADD> v1; NCInt v2;

		NCBoolOpExpr(NCIntOpExpr<NCInt, NCInt, NC_INT_ADD> v1, NCInt v2) : v1 (v1), v2 (v2) {} 

		void apply(NCSolver *sol)
		{
			sol->AddEqual(v1.v1, v1.v2, v2);
		}
	};

	// x == (y + z)
	template <>
	struct NCBoolOpExpr <NCInt, NCIntOpExpr<NCInt, NCInt, NC_INT_ADD>, NC_INT_EQUAL>
	{
		NCInt v1; NCIntOpExpr<NCInt, NCInt, NC_INT_ADD> v2;

		NCBoolOpExpr(NCInt v1, NCIntOpExpr<NCInt, NCInt, NC_INT_ADD> v2) : v1 (v1), v2 (v2) {} 

		void apply(NCSolver *sol)
		{
			sol->AddEqual(v2.v1, v2.v2, v1);
		}
	};

#define defIntIntOperator(ID, OP) \
	template <typename Ta, typename Tb, int Op1, typename Tc, typename Td, int Op2> \
	static NCIntOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, ID> operator OP (const NCIntOpExpr<Ta, Tb, Op1> v1, const NCIntOpExpr<Tc, Td, Op2> v2) { \
		return NCIntOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, ID> (v1, v2); \
	} \
	\
	template <typename Tc, typename Td, int Op2> \
	static NCIntOpExpr< NCInt, NCIntOpExpr<Tc, Td, Op2>, ID> operator OP (const NCInt v1, const NCIntOpExpr<Tc, Td, Op2> v2) { \
		return NCIntOpExpr< NCInt, NCIntOpExpr<Tc, Td, Op2>, ID> (v1, v2); \
	} \
	\
	template <typename Ta, typename Tb, int Op1> \
	static NCIntOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCInt, ID> operator OP (const NCIntOpExpr<Ta, Tb, Op1> v1, const NCInt v2) { \
		return NCIntOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCInt, ID> (v1, v2); \
	} \
	\
	static NCIntOpExpr< NCInt, NCInt, ID> operator OP (const NCInt v1, const NCInt v2) { \
		return NCIntOpExpr< NCInt, NCInt, ID> (v1, v2); \
	} \

	defIntIntOperator (NC_INT_ADD, +);
	defIntIntOperator (NC_INT_SUBTRACT, -);

#define defIntBoolOperator(ID, OP) \
	template <typename Ta, typename Tb, int Op1, typename Tc, typename Td, int Op2> \
	static NCBoolOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, ID> operator OP (const NCIntOpExpr<Ta, Tb, Op1> v1, const NCIntOpExpr<Tc, Td, Op2> v2) { \
		return NCBoolOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCIntOpExpr<Tc, Td, Op2>, ID> (v1, v2); \
	} \
	template <typename Tc, typename Td, int Op2> \
	static NCBoolOpExpr< NCInt, NCIntOpExpr<Tc, Td, Op2>, ID> operator OP (const NCInt v1, const NCIntOpExpr<Tc, Td, Op2> v2) { \
		return NCBoolOpExpr< NCInt, NCIntOpExpr<Tc, Td, Op2>, ID> (v1, v2); \
	} \
	\
	template <typename Ta, typename Tb, int Op1> \
	static NCBoolOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCInt, ID> operator OP (const NCIntOpExpr<Ta, Tb, Op1> v1, const NCInt v2) { \
		return NCBoolOpExpr< NCIntOpExpr<Ta, Tb, Op1>, NCInt, ID> (v1, v2); \
	} \
	\
	static NCBoolOpExpr< NCInt, NCInt, ID> operator OP (const NCInt v1, const NCInt v2) { \
		return NCBoolOpExpr< NCInt, NCInt, ID> (v1, v2); \
	} \

	defIntBoolOperator (NC_INT_EQUAL, ==);
	defIntBoolOperator (NC_INT_NOT_EQUAL, !=);
	defIntBoolOperator (NC_INT_LESS, <);
	defIntBoolOperator (NC_INT_LESS_EQUAL, <=);
	defIntBoolOperator (NC_INT_GTR, >);
	defIntBoolOperator (NC_INT_GTR_EQUAL, >=);
};

