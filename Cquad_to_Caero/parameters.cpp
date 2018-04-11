#include "StdAfx.h"

void parameters::read_parameters(int argc, char* argv[]) 
{
	file_bdf="";
	std::string tmp_str;
	ZMAX="0";
	ZMIN="0";
	NSPAN="1";
	NSPANI=1;
	SPLINEL="-0.1";
	SPLINEU="1.1";
	NCHORD="1";
	NCHORDI=1;
	old_spline=false;
	DZ="";
	for(int j=1; j<argc; j++)
	{
		tmp_str=argv[j];
		if(tmp_str.substr(0,6)=="NSPAN=")
		{
			NSPAN=tmp_str.substr(6,tmp_str.size()-6);
			NSPANI=atoi(NSPAN.c_str());
			continue;
		}
		if(tmp_str.substr(0,8)=="SPLINEL=")
		{
			SPLINEL=tmp_str.substr(8,tmp_str.size()-8);
			continue;
		}
		if(tmp_str.substr(0,8)=="SPLINEU=")
		{
			SPLINEU=tmp_str.substr(8,tmp_str.size()-8);
			continue;
		}
		if(tmp_str.substr(0,10)=="OLDSPL=YES")
		{
			old_spline=true;
			continue;
		}
		if(tmp_str.substr(0,7)=="NCHORD=")
		{
			NCHORD=tmp_str.substr(7,tmp_str.size()-7);
			NCHORDI=atoi(NCHORD.c_str());
			continue;
		}
		if(tmp_str.substr(0,3)=="DZ=")
		{
			ZMAX=tmp_str.substr(3,tmp_str.size()-3);
			continue;
		}
		if(tmp_str.substr(0,5)=="ZMAX=")
		{
			ZMAX=tmp_str.substr(5,tmp_str.size()-5);
			continue;
		}
		if(tmp_str.substr(0,5)=="ZMIN=")
		{
			ZMIN=tmp_str.substr(5,tmp_str.size()-5);
			continue;
		}
		if(tmp_str.substr(0,4)=="FPS=")
		{
			FPS=tmp_str.substr(4,tmp_str.size()-4);
			continue;
		}
		if(file_bdf=="")
			file_bdf=tmp_str;
	}
    while( file_bdf=="" )
        {
        std::cout<<"\n Insert file path: ";
        std::cin>>file_bdf;
        }
}

std::string parameters::get_file_name() const
{
	size_t tmp_poz;
	tmp_poz=file_bdf.find(".");
	return file_bdf.substr(0,tmp_poz);
};
