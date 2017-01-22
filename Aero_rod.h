#pragma once
#include "StdAfx.h"

class Aero_rod
{
public:
	long ID;
	std::string PID;
	std::string G[2];
	Aero_grid GRID[2];
	DMI_ DMI_value;
	size_t line_nr;

	void fill_up_grids(std::vector<Aero_grid> GRID_tabl);
	void Aero_rod::calculate_dmi(DMI_interp DMI_int);
	void Aero_rod::swap_grids();
	Aero_rod(std::string IDt,std::string PID,std::string G1, std::string G2, size_t j);
	Aero_rod(void);
	~Aero_rod(void);
};

