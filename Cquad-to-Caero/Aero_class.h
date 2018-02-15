#pragma once
#include "StdAfx.h"



class Aero_class
{
public:
	std::string ID;
    std::string PID;
    std::string  G1;
    std::string  G2;
    std::string  G3;
    std::string  G4;
	Aero_grid GRID[4];
	Aero_rod ROD;
    double x1;
    double y1;
    double z1;
    double c1;
    double x2;
    double y2;
    double z2;
    double c2;
    double w2gj;
	size_t line_nr;

	void fill_up_grids(std::vector<Aero_grid> GRID_tabl);
	bool sort_grids();
	void calculate();
	void fill_up_rods(std::vector<Aero_rod> GRID_tabl);
	Aero_class(std::string ID,std::string PID,std::string  G1,std::string  G2,std::string  G3,std::string  G4,size_t j);
	~Aero_class(void);
};
