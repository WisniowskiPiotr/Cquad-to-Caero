#include "StdAfx.h"

Aero_grid calculate_normal(Aero_grid G1t, Aero_grid G2t, Aero_grid G3t, bool normalize=true)
{
	Aero_grid V1("",G2t.x-G1t.x,G2t.y-G1t.y,G2t.z-G1t.z,"",0);
	Aero_grid V2("",G3t.x-G2t.x,G3t.y-G2t.y,G3t.z-G2t.z,"",0);
	Aero_grid V_norm;
	
	V_norm.x=V1.y*V2.z-V2.y*V1.z;
	V_norm.y=V1.z*V2.x-V2.z*V1.x;
	V_norm.z=V1.x*V2.y-V2.x*V1.y;

	if (normalize==true)
	{
		double d=sqrt(pow(V_norm.x,2)+pow(V_norm.y,2)+pow(V_norm.z,2));
		if (d==0)
			d=1;
		V_norm.x=V_norm.x/d;
		V_norm.y=V_norm.y/d;
		V_norm.z=V_norm.z/d;
	}

	return V_norm;
}

void Aero_class::fill_up_grids(std::vector<Aero_grid> GRID_tabl)
{
	// wyszukiwanie gridów po ID z AERO ELEM
	int k=0;
	for (size_t j=0; j<GRID_tabl.size(); ++j)
	{
		
		if (k==4)
			break;
		if (GRID_tabl[j].ID==G1)
		{
			GRID[0]=GRID_tabl[j];
			k++;
			continue;
		}
		if (GRID_tabl[j].ID==G2)
		{
			GRID[1]=GRID_tabl[j];
			k++;
			continue;
		}
		if (GRID_tabl[j].ID==G3)
		{
			GRID[2]=GRID_tabl[j];
			k++;
			continue;
		}
		if (GRID_tabl[j].ID==G4)
		{
			GRID[3]=GRID_tabl[j];
			k++;
			continue;
		}
	}
	
}

double Vector_length2(Aero_grid G1, Aero_grid G2, bool all_comp =true)
{
	double d;

	if (all_comp==true)
	{
		d=sqrt(pow(G2.x-G1.x,2)+pow(G2.y-G1.y,2)+pow(G2.z-G1.z,2));
	}
	else
	{
		d=sqrt(pow(G2.y-G1.y,2)+pow(G2.z-G1.z,2));
	}
	return d;
}

double Vector_length(Aero_grid G1)
{
	double d=sqrt(pow(G1.x,2)+pow(G1.y,2)+pow(G1.z,2));
	return d;
}

bool Aero_class::sort_grids()
{
	
	Aero_grid GRID_tmp[4];
	GRID_tmp[0]=GRID[0];
	GRID_tmp[1]=GRID[1];
	GRID_tmp[2]=GRID[2];
	GRID_tmp[3]=GRID[3];

	double d[2];
	d[0]=Vector_length2(GRID_tmp[0],GRID_tmp[1],false);
	d[1]=Vector_length2(GRID_tmp[1],GRID_tmp[2],false);

	if (d[1]<d[0])
	{
		GRID[0]=GRID_tmp[1];
		GRID[1]=GRID_tmp[2];
		GRID[2]=GRID_tmp[3];
		GRID[3]=GRID_tmp[0];

		GRID_tmp[0]=GRID[0];
		GRID_tmp[1]=GRID[1];
		GRID_tmp[2]=GRID[2];
		GRID_tmp[3]=GRID[3];
	}
	
	if (GRID[1].x<GRID[0].x)
	{
		GRID[0]=GRID_tmp[2];
		GRID[1]=GRID_tmp[3];
		GRID[2]=GRID_tmp[0];
		GRID[3]=GRID_tmp[1];
	}
	return true;

}

void Aero_class::fill_up_rods(std::vector<Aero_rod> ROD_tabl)
{
	// wyszukiwanie rodów po po³o¿eniu gridów 
	int nr_roda=-1;
	double d1=-1;
	double d2=-1;
	double d1t,d2t;
	for (size_t j=0; j<ROD_tabl.size(); ++j) 
	{
		if (ROD_tabl[j].PID==PID)
		{
			Aero_rod tmp_rod[2];
			tmp_rod[0]=ROD_tabl[j];
			tmp_rod[1]=ROD_tabl[j];
			tmp_rod[1].swap_grids();
			
			for (size_t k=0; k<2; ++k) 
			{
				d1t=sqrt(pow(tmp_rod[k].GRID[0].y-GRID[0].y,2)+pow(tmp_rod[k].GRID[0].z-GRID[0].z,2));
				d2t=sqrt(pow(tmp_rod[k].GRID[1].y-GRID[3].y,2)+pow(tmp_rod[k].GRID[1].z-GRID[3].z,2));
				if ((d1<0 && d2<0) || (d1>d1t || d2> d2t) )
				{
					d1=d1t;
					d2=d2t;
					ROD=tmp_rod[k];
					nr_roda=j;
				}
			}
		}

	}
	if (nr_roda==-1)
		std::cout<<"There is no CROD for CQUAD: "<<ID<<std::endl;
}

void Aero_class::calculate()
{
	x1=GRID[0].x;
	y1=ROD.GRID[0].y;
	z1=ROD.GRID[0].z;
	c1=GRID[1].x-GRID[0].x;
	x2=GRID[3].x;
	y2=ROD.GRID[1].y;
	z2=ROD.GRID[1].z;
	c2=GRID[2].x-GRID[3].x;

	Aero_grid G1t("",x1,y1,z1,"",0);
	Aero_grid G2t("",x1+c1,y1,z1,"",0);
	Aero_grid G3t("",x2+c2,y2,z2,"",0);
	Aero_grid vec_normal;
	vec_normal=calculate_normal(G1t,G2t,G3t);

	G1t.x=GRID[1].x-GRID[0].x;
	G2t.x=GRID[2].x-GRID[3].x;
	G1t.y=GRID[1].y-GRID[0].y;
	G2t.y=GRID[2].y-GRID[3].y;
	G1t.z=GRID[1].z-GRID[0].z;
	G2t.z=GRID[2].z-GRID[3].z;

	G3t.x=(G1t.x+G2t.x)/2;
	G3t.y=(G1t.y+G2t.y)/2;
	G3t.z=(G1t.z+G2t.z)/2;

	w2gj=atan((vec_normal.x*G3t.x+vec_normal.y*G3t.y+vec_normal.z*G3t.z)/((c1+c2)/2));

}

Aero_class::Aero_class(std::string IDt,std::string PIDt,std::string  G1t,std::string  G2t,std::string  G3t,std::string  G4t,size_t jt)
{
	ID=IDt;
	PID=PIDt;
	G1=G1t;
	G2=G2t;
	G3=G3t;
	G4=G4t;
	for (size_t j=0; j<3; ++j)
		GRID[j]=Aero_grid("",0,0,0,"",0);

	ROD=Aero_rod();
	x1=0;
    y1=0;
    z1=0;
    c1=0;
    x2=0;
    y2=0;
    z2=0;
    c2=0;
    w2gj=0;
	line_nr=jt;
}

Aero_class::~Aero_class(void)
{
}
