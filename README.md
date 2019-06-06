# RASOFP

RASOFP is a post-processing program for CESE. It can ReArrange the Solution Of each variable of each Flexible Panel surface into separate files and merge the local solutions of flexible panel to a global solution.

Compatible with CESE 4.3.6.0


Required files are listed as the following.

For processing result flies of MPI version:
1. FPEProcess.dat
2. Local###_FPElemMap.dat
3. FPanel-000000-LM###.dat (Result flies)
4. FPanelInfo2.dat

For processing intermediate flies of MPI version:
1. FPEProcess.dat
2. {user input prefix}LM###.dat (Intermediate flies)

For processing result flies of non-MPI version:
1. FPanel-000000.dat (Result flie)
2. FPanelInfo.dat


Required file formats are listed as the following.

1. FPEProcess.dat
  * No header
  * Only one column: Ranks that flexible panel elements exist

2. Local###_FPElemMap.dat
  * No header
  * Total 19 columns
  * Column 1: Index
  * Column 4: Local Element Number
  * Column 8: Gobal Element Number

3. FPanel-000000-LM###.dat or FPanel-000000.dat (Result flies)
  * No header
  * Total 8 columns
  * Column 1: Time
  * Column 2: Element Number
  * Column 3: Displacement
  * Column 4: Velocity
  * Column 5: Acceleration
  * Column 6: Fluid Pressure
  * Column 7: Back Pressure

4. FPanelInfo2.dat
  * No header
  * Total 13 columns
  * Column 1: Index
  * Column 4: Local Element Number

5. FPanelInfo.dat
  * With header
  * Total 15 columns
  * Column 1: Index
  * Column 4: Local Element Number

6. {user input prefix}LM###.dat (Intermediate flies)
  * No header
  * Total 6 columns
  * Column 1: Element Number
  * Column 2: Displacement
  * Column 3: Velocity
  * Column 4: Acceleration
  * Column 5: Fluid Pressure
  * Column 6: Back Pressure


Output file formats are listed as the following.

1. FlexibleSurface###-{variable name}.dat or .bin (Result flie), where ### is the surface number and it follows the order of groups of elements in "FPanelInfo.dat".
  * No header
  * Column 1: Time
  * Data in same row are in the same time step, the order of data follows the incremental order of the corresponding element numbers
  * Data in same column are corresponding to the same element, where the element numbers can be referred to "FPanelInfo.dat"

2. FPanel_Intermediate.dat or .bin (Intermediate flies)
  * No header
  * Total 6 columns
  * Column 1: Element Number
  * Column 2: Displacement
  * Column 3: Velocity
  * Column 4: Acceleration
  * Column 5: Fluid Pressure
  * Column 6: Back Pressure


