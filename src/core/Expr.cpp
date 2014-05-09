
#include "NanoCSP.h"
#include <iostream>

namespace NanoCSP
{
	NCExpr operator+(NCExpr& left, NCExpr& right)
	{
		return NCExpr(NCExpr::NC_ADD, &left, &right);
	};

	NCExpr operator-(NCExpr& left, NCExpr& right)
	{
		return NCExpr(NCExpr::NC_SUBSTRACT, &left, &right);
	};

	NCExpr operator==(NCExpr& left, NCExpr& right)
	{
		return NCExpr(NCExpr::NC_EQUAL, &left, &right);
	};

	NCExpr operator!=(NCExpr& left, NCExpr& right)
	{
		return NCExpr(NCExpr::NC_NOT_EQUAL, &left, &right);
	};

	NCExpr operator<(NCExpr& left, NCExpr& right)
	{
		return NCExpr(NCExpr::NC_LESS, &left, &right);
	};

	NCExpr operator<=(NCExpr& left, NCExpr& right)
	{
		return NCExpr(NCExpr::NC_LESSEQUAL, &left, &right);
	};

	NCExpr operator>(NCExpr& left, NCExpr& right)
	{
		return NCExpr(NCExpr::NC_GREATER, &left, &right);
	};

	NCExpr operator>=(NCExpr& left, NCExpr& right)
	{
		return NCExpr(NCExpr::NC_GREATEREQUAL, &left, &right);
	};

	NCExpr operator+(NCBool& left, NCExpr& right)
	{
		NCExpr lf(left);

		return NCExpr(NCExpr::NC_ADD, &lf, &right);
	};

	void NCExpr::print(std::ostream &ofs)
	{
		if(opKind == NC_BOOLVAR) {
			ofs << "bool[" << boolTarget->vId << "]";
		} else if(opKind == NC_INTVAR) {
			ofs << "int[" << intTarget->vTop << ",(" << intTarget->nMin << "," << intTarget->nMax << ")]";
		} else {
			ofs << "(";
			opLeft->print(ofs);

			switch(opKind)
			{
			case NC_ADD:
				ofs << " + ";
				break;
			case NC_SUBSTRACT:
				ofs << " - ";
				break;
			case NC_EQUAL:
				ofs << " == ";
				break;
			case NC_NOT_EQUAL:
				ofs << " != ";
				break;
			case NC_LESS:
				ofs << " < ";
				break;
			case NC_LESSEQUAL:
				ofs << " <= ";
				break;
			case NC_GREATER:
				ofs << " > ";
				break;
			case NC_GREATEREQUAL:
				ofs << " >= ";
				break;
			};

			opRight->print(ofs);
			ofs << ")";
		}
	}
};

