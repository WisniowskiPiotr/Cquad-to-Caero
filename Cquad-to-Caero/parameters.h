#pragma once
//#include "StdAfx.h"

class parameters
{
public:
	std::string file_bdf;
	std::string ZMAX;
	std::string ZMIN;
	std::string FPS;
	std::string DZ;
	std::string NSPAN;
	std::string SPLINEL;
	std::string SPLINEU;
	std::string NCHORD;
	bool old_spline;
	int NSPANI;
	int NCHORDI;


	void read_parameters(int argc, char* argv[] ) ;
	std::string get_file_name() const;
};
