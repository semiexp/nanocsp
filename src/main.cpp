// nanocsp.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "core/NanoCSP.h"

using namespace NanoCSP;

int main(int argc, char** argv)
{
	NCBool hoge = {0};
	NCInt piyo = {1, 0, 10};

	(NCExpr(hoge) + NCExpr(piyo)).print(std::cout);

	return 0;
}

