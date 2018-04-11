#include "StdAfx.h"



void Aero_rod::calculate_dmi(DMI_interp DMI_int)
{
	if (ID<=DMI_int.DMI_points[1].rod_ID && ID>=DMI_int.DMI_points[0].rod_ID)
	{
		double DID=(ID-DMI_int.DMI_points[0].rod_ID)/((double)DMI_int.DMI_points[1].rod_ID-(double)DMI_int.DMI_points[0].rod_ID);
		double Dwkk1=DMI_int.DMI_points[1].wkk1-DMI_int.DMI_points[0].wkk1;
		double Dwkk2=DMI_int.DMI_points[1].wkk2-DMI_int.DMI_points[0].wkk2;
		double Dfa2j=DMI_int.DMI_points[1].fa2j-DMI_int.DMI_points[0].fa2j;

		DMI_value.wkk1=DMI_int.DMI_points[0].wkk1+Dwkk1*DID;
		DMI_value.wkk2=DMI_int.DMI_points[0].wkk2+Dwkk2*DID;
		DMI_value.fa2j=DMI_int.DMI_points[0].fa2j+Dfa2j*DID;
	}
}

void Aero_rod::fill_up_grids(std::vector<Aero_grid> GRID_tabl)
{
	// wyszukiwanie gridów po ID z AERO ELEM
	int k=0;
	for (size_t j=0; j<GRID_tabl.size(); ++j)
	{
		if (k==2)
			break;
		for (size_t jt=0; jt<2; ++jt)
			if (GRID_tabl[j].ID==G[jt])
			{
				GRID[jt]=GRID_tabl[j];
				k++;
				continue;
			}
	}
	if (GRID[0].ID=="" || GRID[1].ID=="" )
		std::cout<<" No grid found for CROD ID: "<<ID<<std::endl;
	
}

void Aero_rod::swap_grids()
{
	Aero_grid GRIDt[2];
	std::string id[2];
	GRIDt[0]=GRID[0];
	id[0]=G[0];
	GRIDt[1]=GRID[1];
	id[1]=G[1];
	
	GRID[0]=GRIDt[1];
	G[0]=id[1];
	GRID[1]=GRIDt[0];
	G[1]=id[0];
}

Aero_rod::Aero_rod(std::string IDt,std::string PIDt, std::string G1, std::string G2, size_t j)
{
	ID=atoi(IDt.c_str());
	PID=PIDt;
	G[0]=G1;
	G[1]=G2;
	line_nr=j;
}

Aero_rod::Aero_rod(void)
{
}

Aero_rod::~Aero_rod(void)
{
}
