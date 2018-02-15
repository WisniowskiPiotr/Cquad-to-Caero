# Cquad-to-Caero
Program translates MSC Nastran Cquad elements with 1D Crod on leading edge to CAERO1 elements with generation of d2wgj, wkk and f2gj matricles. 

Copyright (C) 2018 Wiśniowski Piotr

Please read [Static stability calculation of aircraft](./Documents/StaticStabilityCalculationOfAircraft.pdf) for explained usage of this software.
Additionaly read [Dynamic stability calculation of aircraft](./Documents/DynamicStabilityAnalysisOfAirplane.pdf) for futher reading regarding usagde of results of this analysis.

Main method is in Cquad_to_Aero.cpp. Typical parameter values are set by default.

Default values for parameters:
- Values uset by SET2 Nastran data for defining the prism containing the set for splining structural grid points to aerodynamic grid points. 
    SPLINEL=-0.1
    SPLINEU=1.1
    ZMIN=0
    ZMAX=0

- Division of each CAERO1 to a number of aerodynamic grid points. Used for mesh size dependency validation.
    NCHORD=1
    NSPAN=1

- SPLINE6 Nastran entry parameters
    DZ=0 -Linear attachment flexibility.
    FPS=FPS6 -Method for the spline fit. Either FPS3 or FPS6.

Typical usage:
Cquad_to_Aero.exe Whole_aero.bdf NSPAN=1 SPLINEL=-0.1

Whole_aero.bdf is an example of input file
