#include "StdAfx.h"

DMI_::DMI_(long rod_IDt,double wkk1t,double wkk2t,double fa2jt)
{
	rod_ID=rod_IDt;
	wkk1=wkk1t;
	wkk2=wkk2t;
	fa2j=fa2jt;
}

DMI_::DMI_(void)
{
	wkk1=1;
	wkk2=1;
	fa2j=0;
}


DMI_::~DMI_(void)
{
}
