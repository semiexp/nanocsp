
#include "NanoCSP.h"

namespace NanoCSP
{
	NCBool::NCBool(NCSolver& sol)
	{
		vId = sol.Allocate(1);
	}

	NCInt::NCInt(NCSolver& sol, int nMin, int nMax) : nMin(nMin), nMax(nMax)
	{
		vTop = sol.AssignInt(nMax - nMin);
	}
};
