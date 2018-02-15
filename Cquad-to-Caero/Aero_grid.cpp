#include "StdAfx.h"



Aero_grid::Aero_grid(std::string IDt, double xt, double yt, double zt,std::string SIDt,size_t j)
{
	ID=IDt;
	x=xt;
	y=yt;
	z=zt;
	SID=SIDt;
	line_nr=j;
}

Aero_grid::Aero_grid()
{
}

Aero_grid::~Aero_grid(void)
{
}
