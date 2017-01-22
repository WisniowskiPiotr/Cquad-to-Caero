#pragma once

class Aero_grid
{
public:
	std::string ID;
	double x;
    double y;
    double z;
	std::string SID;
	size_t line_nr;
	
	Aero_grid(std::string ID, double x, double y, double z,std::string SID,size_t j);
	Aero_grid();
	~Aero_grid(void);
};
