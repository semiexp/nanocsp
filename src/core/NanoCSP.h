
#include <iostream>

namespace NanoCSP
{
	class NCSolver
	{
	};

	class NCExpr;

	struct NCBool
	{
		int vId;
	};

	struct NCInt
	{
		int vTop, nMin, nMax;
	};

	NCExpr operator+(NCExpr& left, NCExpr& right);
	NCExpr operator-(NCExpr& left, NCExpr& right);
	NCExpr operator==(NCExpr& left, NCExpr& right);
	NCExpr operator!=(NCExpr& left, NCExpr& right);
	NCExpr operator<(NCExpr& left, NCExpr& right);
	NCExpr operator<=(NCExpr& left, NCExpr& right);
	NCExpr operator>(NCExpr& left, NCExpr& right);
	NCExpr operator>=(NCExpr& left, NCExpr& right);

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

