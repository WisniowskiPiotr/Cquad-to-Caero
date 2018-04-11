// Cquad_to_Aero.cpp : Defines the entry point for the console application.

#include "stdafx.h"

std::string format_number(double val)
{
	std::string str;
	str= std::to_string(static_cast<long double>(val));

	size_t j=str.find(".");
	if (str.npos==j)
		str=str+".";
	

	return str;
}

std::string get_file_name(std::string file_path)
{
	size_t i;
	i=file_path.rfind(".");
	return file_path.substr(1,i);
}

std::string get_file_name2(std::string file_path)
{
	size_t i;
	i=file_path.rfind("\\");
	std::string tmp=file_path.substr(i+1,file_path.length()-i-1);
	return tmp;
}

std::vector<size_t> get_lines_to_skip(std::vector<Aero_class> CAERO_tab,std::vector<Aero_rod> A_ROD_tab)
{
	std::vector<size_t> lines_to_skip;
	for(size_t j=0; j<CAERO_tab.size(); j++)
	{
		lines_to_skip.push_back(CAERO_tab[j].line_nr);
		for(size_t i=0; i<3; i++)
			lines_to_skip.push_back(CAERO_tab[j].GRID[i].line_nr);
		for(size_t i=0; i<1; i++)
			lines_to_skip.push_back(CAERO_tab[j].ROD.line_nr);
	}
	for(size_t j=0; j<A_ROD_tab.size(); j++)
		lines_to_skip.push_back(A_ROD_tab[j].line_nr);
	// sorting and removing duplicates form lines to skip
	std::sort(lines_to_skip.begin(),lines_to_skip.end());
	std::vector<size_t>::iterator unique_lines_to_skip;
	unique_lines_to_skip= std::unique (lines_to_skip.begin(), lines_to_skip.end()); 
	lines_to_skip.resize( std::distance(lines_to_skip.begin(),unique_lines_to_skip)+1 );
	return lines_to_skip;
}

void fileExists (std::string fileName)
{
        std::fstream plik;
		std::string str_tmp;
		bool rename_=false;
		long long j=0;
		str_tmp=fileName;
		plik.open(fileName.c_str(), std::ios::in );
        while(plik.is_open()==true)
		{
			j++;
			rename_=true;
			plik.close();
			//plik.is_open()==false;
			std::string t=std::to_string(j);
			str_tmp=fileName+"."+t;
			plik.open(str_tmp.c_str(), std::ios::in );
		} 
		plik.close();
		if (rename_==false)
			return;
		rename_=rename(fileName.c_str(),str_tmp.c_str());

}

void write_new_bdf(std::string file_bdf_name,std::vector<Aero_class> caero_tab,std::string SID,std::vector<std::string> whole_bdf,std::vector<size_t> lines_to_skip,std::vector<std::string>  file_names_to_write,parameters params)
{

	std::ofstream file_bdf;
	
	fileExists(file_bdf_name);
	file_bdf.open(file_bdf_name.c_str());
	file_bdf<<std::setprecision(5);
	file_bdf.flush();
	
	size_t k=0;
    for(size_t j=0; j<whole_bdf.size(); j++)
    {
        if (whole_bdf[j].substr(0,7)!="ENDDATA" && j!=lines_to_skip[k]  )
            file_bdf<<whole_bdf[j]<<std::endl;
        else
            k++;
    }
	if (params.old_spline==false)
		{
		for (size_t j=0; j<caero_tab.size(); ++j)
			{
		
				if (caero_tab[j].PID==caero_tab[j].ROD.PID)
				{
					file_bdf<<"CAERO1,"<<caero_tab[j].ID<<","<<caero_tab[j].PID<<","<<caero_tab[j].GRID[0].SID<<","<<params.NSPAN<<","<<params.NCHORD<<",,,1,+"<<std::endl;
					file_bdf<<"+,"<<format_number(caero_tab[j].x1)<<","<<format_number(caero_tab[j].y1)<<","<<format_number(caero_tab[j].z1)<<","<<format_number(caero_tab[j].c1)<<",";
					file_bdf<<format_number(caero_tab[j].x2)<<","<<format_number(caero_tab[j].y2)<<","<<format_number(caero_tab[j].z2)<<","<<format_number(caero_tab[j].c2)<<","<<std::endl;

					// wtite set2 entry
					file_bdf<<"SET2,"<<caero_tab[j].ID<<","<<caero_tab[j].ID<<","<<params.SPLINEL<<","<<params.SPLINEU<<","<<params.SPLINEL<<","<<params.SPLINEU<<","<<params.ZMAX<<","<<params.ZMIN<<std::endl;

					// write AELIST entry
					file_bdf<<"AELIST,"<<caero_tab[j].ID<<","<<caero_tab[j].ID;
					if (params.NSPANI==1 && params.NCHORDI==1) 
						file_bdf<<std::endl;
					else
						file_bdf<<",THRU,"<<atoi(caero_tab[j].ID.c_str())+(params.NSPANI*params.NCHORDI)-1<<std::endl;
			
					//write spline6 entry
					file_bdf<<"SPLINE6,"<<caero_tab[j].ID<<","<<caero_tab[j].ID<<","<<caero_tab[j].ID<<",,"<<caero_tab[j].ID<<","<<params.DZ<<","<<params.FPS<<std::endl<<std::endl;
				}
			}
		}
	else
	{
		
	}

	for (size_t j=2; j<5; ++j)
		file_bdf<<"include "<<get_file_name2(file_names_to_write[j])<<std::endl;

	file_bdf<<"ENDDATA";

}

void write_HV_DMI(std::string file_DMI_name,std::vector<Aero_class> caero_tab)
{
	
	std::ofstream file_DMI_HV;
	
	fileExists(file_DMI_name);
	file_DMI_HV.open(file_DMI_name.c_str());
	

    file_DMI_HV<<"ALTAIR ASCII FILE"<<std::endl;
    file_DMI_HV<<"$TITLE = DMI"<<std::endl;
    file_DMI_HV<<"$SUBCASE = 1       Subcase 1"<<std::endl;
    file_DMI_HV<<"$BINDING = ELEMENT"<<std::endl;
    file_DMI_HV<<"$COLUMN_INFO = ENTITY_ID GRID_ID"<<std::endl;
    file_DMI_HV<<"$RESULT_TYPE = W2GJ,WKK1,WKK2,FA2J"<<std::endl;

	
    for (size_t j=0; j<caero_tab.size(); ++j)
            file_DMI_HV<<caero_tab[j].ID<<"  \t0  \t"<<caero_tab[j].w2gj<<"  \t"<<caero_tab[j].ROD.DMI_value.wkk1<<"  \t"<<caero_tab[j].ROD.DMI_value.wkk2<<"  \t"<<caero_tab[j].ROD.DMI_value.fa2j<<std::endl;
            

}

void write_single_DMI(std::string file_DMI_name,std::string which, std::vector<Aero_class> caero_tab,parameters params,size_t subcases=1)
{
	
	std::ofstream file_DMI;
	
	fileExists(file_DMI_name);
	file_DMI.open(file_DMI_name.c_str());

	file_DMI<<std::fixed<<std::setprecision(4);

    
    file_DMI<<"DMI,"<<which<<",0,2,1,0,,"<<caero_tab.size()*params.NCHORDI*params.NSPANI<<","<<subcases<<" \n";


	for (size_t sub=1; sub<subcases+1; ++sub)
	{
		size_t poz_dmi=5;
		file_DMI<<"DMI,"<<which<<","<<sub<<",1";

		
		for (size_t j=0; j<caero_tab.size(); ++j)
			for (size_t ndiv=0; ndiv<params.NCHORDI*params.NSPANI; ++ndiv)
			{
            
				if (which=="w2gj")
					file_DMI<<","<<caero_tab[j].w2gj;
				if (which=="fa2j")
					file_DMI<<","<<caero_tab[j].ROD.DMI_value.fa2j;

			poz_dmi++;
				if (poz_dmi==10)
				{
					file_DMI<<",+\n+";
					poz_dmi=2;
				}
			}
		file_DMI<<"\n";
	}


}

void write_double_DMI(std::string file_DMI_name,std::string which, std::vector<Aero_class> caero_tab,parameters params)
{
	
	std::ofstream file_DMI;
	
	fileExists(file_DMI_name);
	file_DMI.open(file_DMI_name.c_str());

	file_DMI<<std::fixed<<std::setprecision(4);

    size_t poz_dmi=0;
    file_DMI<<"DMI,"<<which<<",0,3,1,0,,"<<caero_tab.size()*2*params.NCHORDI*params.NSPANI<<",1 \n";

    for (size_t j=0; j<caero_tab.size(); ++j)
		for (size_t ndiv=0; ndiv<params.NCHORDI*params.NSPANI; ++ndiv)
        {
            if(poz_dmi==0)
            {
                file_DMI<<"DMI,"<<which<<",1,1,";
                poz_dmi=5;
            }

            if (poz_dmi==10)
            {
                file_DMI<<"+\n+,";
                poz_dmi=2;
            }
            file_DMI<<caero_tab[j].ROD.DMI_value.wkk1<<",";
			poz_dmi++;
		

			if (poz_dmi==10)
            {
                file_DMI<<"+\n+,";
                poz_dmi=2;
            }
			file_DMI<<caero_tab[j].ROD.DMI_value.wkk2<<",";
            poz_dmi++;
        }

}

double read_double(std::string str)
{
	bool flag=false;
	std::string s[2];
	std::string plus="+";
	std::string minus="-";
	std::string space=" ";
	s[0]="";
	s[1]="";
	for(size_t j=0; j<str.size(); j++)
	{
		if ((str[j]==plus[0]  || str[j]==minus[0]) && s[0].size()!=0)
			flag=true;
		if (flag==false && str[j]!=space[0])
			s[0]=s[0]+str[j];
		if (flag==true && str[j]!=space[0])
			s[1]=s[1]+str[j];
	}

	double x[2];
	x[0]=atof(s[0].c_str());
	x[1]=atof(s[1].c_str());

	double y=x[0]*pow(10.0, x[1]);
	return y;
}

bool sort_caero(Aero_class a,Aero_class b)
{
	 if(a.ID<b.ID)  
        return true;
	 else
		 return false;
}


int main(int argc, char* argv[])
{
	// read parameters
	parameters params;
	params.read_parameters(argc, argv);
	
	// read whole bdf to vector string
	std::vector<std::string> whole_bdf;
	std::ifstream file;
    file.open(params.file_bdf.c_str() );
    if ( !file.is_open() )
        {
        std::cout<<"\n Could not open file. Exiting...\n";
		return 1;
        }
	file.seekg(0);
	std::string line;

	std::string AERO_SID="        ";
	std::vector<std::string> PAERO_tab;
	std::vector<Aero_grid> GRID_tab;
	std::vector<Aero_class> CQUAD_tab;
	std::vector<Aero_rod> CROD_tab;
	std::vector<DMI_interp> DMI_tab;
	size_t subcases=0;
	bool begin_bulk=false;

	size_t jt=0;
    while( std::getline( file, line ) )
	{
		
		// sheach for AERO SID to string
        whole_bdf.push_back(line);
		if (line.substr(0,8)=="AERO    ")
			AERO_SID=line.substr(8,8);
	
		// search for PAERO to vector
		if (line.substr(0,8)=="PAERO1  ")
			PAERO_tab.push_back(line.substr(8,8));

		// Search for GRIDS to vector
		if (line.substr(0,8)=="GRID    ")
		{
			double x,y,z;
			x=read_double(line.substr(24,8));
			y=read_double(line.substr(32,8));
			z=read_double(line.substr(40,8));
			GRID_tab.push_back(Aero_grid::Aero_grid(line.substr(8,8),x,y,z,line.substr(48,8),jt));
		}
		
		// search for CQUAD4 to vector
		if (line.substr(0,8)=="CQUAD4  ")
			CQUAD_tab.push_back(Aero_class::Aero_class(line.substr(8,8),line.substr(16,8),line.substr(24,8),line.substr(32,8),line.substr(40,8),line.substr(48,8),jt));

		// search for rods to vector
		if (line.substr(0,8)=="CROD    ")
			CROD_tab.push_back(Aero_rod::Aero_rod(line.substr(8,8),line.substr(16,8),line.substr(24,8),line.substr(32,8),jt));

		// search for wkk interp to vector
		if (line.substr(0,8)=="$DMI_INT")
		{
			double wkk1,wkk2,fa2j;
			long id;

			id=atoi(line.substr(8,8).c_str());
			wkk1=atof(line.substr(16,8).c_str());
			wkk2=atof(line.substr(24,8).c_str());
			fa2j=atof(line.substr(32,8).c_str());
			DMI_ d1(id,wkk1,wkk2,fa2j);

			id=atoi(line.substr(40,8).c_str());
			wkk1=atof(line.substr(48,8).c_str());
			wkk2=atof(line.substr(56,8).c_str());
			fa2j=atof(line.substr(64,8).c_str());
			DMI_ d2(id,wkk1,wkk2,fa2j);
		
			DMI_tab.push_back(DMI_interp::DMI_interp(d1,d2));
		}

		// serch for begin bulk
		if(line.substr(0,10)=="BEGIN BULK")
			begin_bulk=true;

		// search for subcases
		if (line.substr(0,7)=="SUBCASE" && begin_bulk==false)
			subcases=subcases+1;


		jt++;
	}
	file.close();
	if (AERO_SID=="        ")
		AERO_SID="       0";
	// moznaby przyspieszyc
	// check if CQUAD got PAERO
	std::vector<Aero_class> CAERO_tab;
	for(size_t j=0; j<CQUAD_tab.size(); j++)
		for(size_t i=0; i<PAERO_tab.size(); i++)
			if (PAERO_tab[i]==CQUAD_tab[j].PID)
				CAERO_tab.push_back(CQUAD_tab[j]);
	CQUAD_tab.~vector();

	// check if CROD got PAERO
	std::vector<Aero_rod> A_ROD_tab;
	for(size_t j=0; j<CROD_tab.size(); j++)
		for(size_t i=0; i<PAERO_tab.size(); i++)
			if (PAERO_tab[i]==CROD_tab[j].PID)
				A_ROD_tab.push_back(CROD_tab[j]);
	CROD_tab.~vector();

	std::cout<<" Summary: \n\t"<<PAERO_tab.size()<<"\t PAERO1"<<std::endl;
	std::cout<<"\t"<<CAERO_tab.size()<<"\t CAERO1"<<std::endl;
	std::cout<<"\t"<<A_ROD_tab.size()<<"\t AERO CROD"<<std::endl;
	std::cout<<"\t"<<DMI_tab.size()<<"\t $DMI INT"<<std::endl;
	std::cout<<" Calculating...."<<std::endl;
	//clrscr();


	// fill data for ROD
	Concurrency::parallel_for (size_t(0), A_ROD_tab.size(), [&](size_t j)
	{
		A_ROD_tab[j].fill_up_grids(GRID_tab);	// get data from grids
		for(size_t i=0; i<DMI_tab.size(); i++)
			A_ROD_tab[j].calculate_dmi(DMI_tab[i]);
	});
	std::cout<<"\tAERO CRODs calculated."<<std::endl;

	// calculate data for CQUAD
	Concurrency::parallel_for (size_t(0), CAERO_tab.size(), [&](size_t j)
	{
		CAERO_tab[j].fill_up_grids(GRID_tab);	// get data from grids
		CAERO_tab[j].sort_grids(); 			// sort grids. 1st and 4th has to be in front without changing normal
		CAERO_tab[j].fill_up_rods(A_ROD_tab);	// get data from grids
		CAERO_tab[j].calculate();			
	});
	std::cout<<"\tCAERO1 calculated."<<std::endl;

    std::cout << " Writing...\n" ;
	std::sort(CAERO_tab.begin(),CAERO_tab.end(),sort_caero);
	std::vector<size_t> lines_to_skip=get_lines_to_skip(CAERO_tab,A_ROD_tab); // finding all lines to skip

	// creating new bdf
	std::vector<std::string> file_names_to_write;
	file_names_to_write.push_back(params.get_file_name()+".aero_bdf");
	file_names_to_write.push_back(params.get_file_name()+".HVASCI");
	file_names_to_write.push_back(params.get_file_name()+".w2gj");
	file_names_to_write.push_back(params.get_file_name()+".fa2j");
	file_names_to_write.push_back(params.get_file_name()+".wkk");
	
	// write
	write_new_bdf(file_names_to_write[0],CAERO_tab, AERO_SID,whole_bdf,lines_to_skip,file_names_to_write,params);
	write_HV_DMI(file_names_to_write[1],CAERO_tab);
	write_single_DMI(file_names_to_write[2],"w2gj",CAERO_tab,params);
	write_single_DMI(file_names_to_write[3],"fa2j",CAERO_tab,params);
	write_double_DMI(file_names_to_write[4],"wkk",CAERO_tab,params);

	std::cout<<"All files are writen. Job finished."<<std::endl;

	return 0;
}