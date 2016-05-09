#include "stdafx.h"
#include "StateSave.h"
#include <iostream>

const char* savePath = ".\\gstats";

void StateSave::save()
{
	std::ofstream ofs(savePath, std::ios::binary);
	boost::archive::binary_oarchive oa(ofs);
	oa << *this;
}

void StateSave::load()
{
	std::ifstream ifs(savePath, std::ios::binary);

	if (ifs.good())
	{
		boost::archive::binary_iarchive ia(ifs);
		ia >> *this;
	}
}

