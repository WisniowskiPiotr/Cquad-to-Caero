# Cquad-to-Caero
Program translates MSC Nastran *CQUAD4* elements with 1D *CROD* elements on leading edge to *CAERO1* elements with generation of *d2wgj*, *wkk* and *f2gj* matrices. 

Copyright (C) 2018 Wiśniowski Piotr

### Theoretical details
Please read [Static stability calculation of aircraft](./Documents/StaticStabilityCalculationOfAircraft.pdf) for explained usage of this software.
Additionally read [Dynamic stability calculation of aircraft](./Documents/DynamicStabilityAnalysisOfAirplane.pdf) for further reading regarding usage of results of this analysis.

### Usage Example

#### Input model
[Example of input file](./Example/Whole_aero.bdf) can be found in *Example* folder. 
This is basically any valid MSC Nastran aerodynamic input file (with structural part) with additional with some additional modeling tricks:
- All aerodynamic surfaces are modeled by using standard *CQUAD4* elements on center surface.
  - All these elements must have same normal vector.
  - All these elements must have associated *PAERO1* properties.
  - All these elements must have all grids which lie approximately behind front leading edge grids with *CROD* in the direction of flow.
  - Triangular surfaces must be modeled as *CQUAD4* elements with single side edge length equal to 0.
- Leading edge of each aerodynamic surface must be meshed with *CROD* elements.
  - All these elements must have associated *PAERO1* properties same as *CQUAD4* elements from this surface.
  - All *CROD* elements numbering must be continuous across leading edge. This is in order to allow coefficient interpolation.
- For each leading edge (defined as first *CROD* element and last *CROD* element) additional aerodynamic profile coefficients can be supported:
```
| $DMI_INT | EID1     | WKK1     | WKK2     | F2GJ     | EID2     | WKK1     | WKK2     | F2GJ     |
| $DMI_INT | 10001460 |  1.43116 | -0.51663 | -1.50991 | 10001542 |  1.43116 | -0.51663 | -1.50991 |
	
	EID1, EID2		- First and Last elements between which interpolation will occur.
	WKK1, WKK2, F2GJ 	- Aerodynamic coefficients at EID1 and EID2.
	
Remark:
	Free and long field formats are not supported.
```

#### Result files
Several files are generated:
- *.aero_bdf* - file generated from input file with inserted *CAERO1*, *SET2*, *AELIST*, *SPLINE6* elements and included *.w2gj*, *.fa2j*, *.wkk* files. This file is basically ready to be run under MSC Nastran.
- *.w2gj*, *.fa2j*, *.wkk* - files containing *DMI* cards with *w2gj*, *fa2j*, *wkk* matrices.
- *.HVASCI* - File used to visualize generated matrices in Altair HyperView or with some modifications in Partan.

### Execute Parameters
Typical parameter values are set by default. Default values for parameters:

Values used by *SET2* card for defining the prism containing the set for splining structural grid points to aerodynamic grid points:
```
SPLINEL = -0.1
SPLINEU = 1.1
ZMIN = 0
ZMAX = 0
```
Division of each *CAERO1* to a number of aerodynamic grid points. Used for mesh size dependency validation:
```
NCHORD = 1
NSPAN = 1
```
*SPLINE6* Nastran entry parameters:
```
DZ = 0 - Linear attachment flexibility.
FPS = FPS6 - Method for the spline fit. Either FPS3 or FPS6.
```
Typical usage:
```
Cquad_to_Aero.exe Whole_aero.bdf NSPAN=1 NCHORD=1
```

### Contact

If You have any questions or problems contact me at [contact.wisniowskipiotr@gmail.com](mailto:contact.wisniowskipiotr@gmail.com).
