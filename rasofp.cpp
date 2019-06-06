//
//  rasofp.cpp
//  RASOFP
//
//  Created by Harris Fan on 21/8/2018.
//  Copyright (c) 2018 Harris. All rights reserved.
//

#include "rasofp.h"

const int FPanelDataSize = 5;

void fileExist(fstream &file, string filename)
{
  if (!file) {
    cerr << "\n" << filename << " can't be opened!";
    cerr << "\nPlease check " << filename << " again!" << endl;
    exit(1);
  }
  return;
}

void intInput(string statement, int &input, int min, int max)
{
  do {
    cout << statement;
    cin >> input;
  } while ( input < min || input > max );
}

struct FPanelResultStruct
{
    double time;
    int elemNo;
    double data[FPanelDataSize];
};

bool sortByTimeElemNo(const FPanelResultStruct &lhs, const FPanelResultStruct &rhs) 
{ 
  if (lhs.time == rhs.time)
    return lhs.elemNo < rhs.elemNo;
  else
    return lhs.time < rhs.time; 
}

void readFPEProcess(vector<int> &FPEProcess)
{
  string FPEProcessFliename = "FPEProcess.dat";
  int tempInt;
  fstream FPEProcessFlie(FPEProcessFliename.c_str(), ios::in);
  fileExist(FPEProcessFlie, FPEProcessFliename.c_str());
  cout << "\nReading " << setiosflags(ios::left) << setw(57) << setfill('.') << FPEProcessFliename;
  while (FPEProcessFlie.good()) {
    FPEProcessFlie >> tempInt;
    FPEProcess.push_back(tempInt);
  }
  FPEProcess.pop_back();
  FPEProcessFlie.close();
  cout << "Done!";
}

void readLocalFPElemMap(string LocalFPElemMapFliename, map<int, int> &FPElemMap)
{
  int tempInt, index, localElementNumber, gobalElementNumber;
  double tempDouble;
  fstream LocalFPElemMapFlie(LocalFPElemMapFliename.c_str(), ios::in);
  fileExist(LocalFPElemMapFlie, LocalFPElemMapFliename.c_str());
  cout << "\nReading " << setiosflags(ios::left) << setw(57) << setfill('.') << LocalFPElemMapFliename;
  while (LocalFPElemMapFlie.good()) {
    LocalFPElemMapFlie >> index >> tempInt >> tempInt >> localElementNumber >> tempInt >> tempInt >> tempInt 
      >> gobalElementNumber >> tempDouble >> tempDouble >> tempDouble >> tempDouble >> tempDouble 
      >> tempDouble >> tempDouble >> tempDouble >> tempDouble >> tempDouble >> tempDouble;
    if (index <= 0)
      break;
    else 
      FPElemMap[localElementNumber] = gobalElementNumber;
  }
  LocalFPElemMapFlie.close();    
  cout << "Done!";
}

void readFPanelResult(string FPanelResultFliename, vector<FPanelResultStruct> &GobalFPanelResult, map<int, int> FPElemMap, int MPIflag)
{
  FPanelResultStruct tempFPanelResult;
  fstream FPanelResultFlie(FPanelResultFliename.c_str(), ios::in);
  fileExist(FPanelResultFlie, FPanelResultFliename.c_str());
  cout << "\nReading " << setiosflags(ios::left) << setw(57) << setfill('.') << FPanelResultFliename;
  while (FPanelResultFlie.good()) {
    FPanelResultFlie >> tempFPanelResult.time >> tempFPanelResult.elemNo 
      >> tempFPanelResult.data[0] >> tempFPanelResult.data[1] >> tempFPanelResult.data[2] 
      >> tempFPanelResult.data[3] >> tempFPanelResult.data[4];
    if (MPIflag == 1)
      tempFPanelResult.elemNo = FPElemMap[tempFPanelResult.elemNo];
    GobalFPanelResult.push_back(tempFPanelResult);
  }
  GobalFPanelResult.pop_back();
  FPanelResultFlie.close();
  cout << "Done!";
}

void readFPanelInfo(map<int, int> &FPSurfaceMap, map<int, int> &firstFlemFlag, int MPIflag)
{
  string FPanelInfoFliename = "FPanelInfo.dat", tempString;
  int tempInt, index, elemNo, surfaceNo(0);
  double tempDouble;
  //vector<int> elemNoGroup;
  if (MPIflag == 1)
    FPanelInfoFliename = "FPanelInfo2.dat";
  fstream FPanelInfoFlie(FPanelInfoFliename.c_str(), ios::in);
  fileExist(FPanelInfoFlie, FPanelInfoFliename.c_str());
  cout << "\nReading " << setiosflags(ios::left) << setw(57) << setfill('.') << FPanelInfoFliename;
  if (MPIflag == 0)
    getline(FPanelInfoFlie, tempString);
  while (FPanelInfoFlie.good()) {
    if (MPIflag == 0)
      FPanelInfoFlie >> index >> tempInt >> tempInt >> elemNo >> tempInt >> tempInt >> tempInt 
        >> tempDouble >> tempDouble >> tempDouble >> tempDouble >> tempDouble >> tempDouble >> tempDouble >> tempDouble;
    if (MPIflag == 1)
      FPanelInfoFlie >> index >> tempInt >> tempInt >> elemNo >> tempInt >> tempInt >> tempInt 
        >> tempDouble >> tempDouble >> tempDouble >> tempDouble >> tempDouble >> tempDouble;
    if (index == 1) {
      surfaceNo++;
      firstFlemFlag[elemNo] = 1;
    } else
      firstFlemFlag[elemNo] = 0;
    FPSurfaceMap[elemNo] = surfaceNo;
//    cout << "Reading firstFlemFlag[ " << elemNo << "] = " << firstFlemFlag[elemNo] << endl;
  }
  FPanelInfoFlie.close();
  cout << "Done!";
}

struct FPanelIntermediateStruct
{
    int elemNo;
    double data[FPanelDataSize];
};

bool sortByElemNo(const FPanelIntermediateStruct &lhs, const FPanelIntermediateStruct &rhs) 
{ 
  return lhs.elemNo < rhs.elemNo;
}

void readFPanelIntermediate(string FPanelResultFliename, vector<FPanelIntermediateStruct> &GobalFPanelIntermediate, map<int, int> FPElemMap)
{
  FPanelIntermediateStruct tempFPanelIntermediate;
  fstream FPanelResultFlie(FPanelResultFliename.c_str(), ios::in);
  fileExist(FPanelResultFlie, FPanelResultFliename.c_str());
  cout << "\nReading " << setiosflags(ios::left) << setw(57) << setfill('.') << FPanelResultFliename;
  while (FPanelResultFlie.good()) {
    FPanelResultFlie >> tempFPanelIntermediate.elemNo >> tempFPanelIntermediate.data[0] >> tempFPanelIntermediate.data[1] 
      >> tempFPanelIntermediate.data[2] >> tempFPanelIntermediate.data[3] >> tempFPanelIntermediate.data[4];
    tempFPanelIntermediate.elemNo = FPElemMap[tempFPanelIntermediate.elemNo];
    GobalFPanelIntermediate.push_back(tempFPanelIntermediate);
  }
  GobalFPanelIntermediate.pop_back();
  FPanelResultFlie.close();
  cout << "Done!";
}

int main()
{
  string LocalFPElemMapFliename, LocalFPElemMapFlienamePrefix = "Local", LocalFPElemMapFlienameSuffix = "_FPElemMap.dat";
  string FPanelResultFliename, FPanelResultFlienamePrefix = "FPanel-000000", LM = "LM", dotDat = ".dat", dotBin = ".bin";
  string outputFliename, outputFlienamePrefix = "FlexibleSurface", tempOutputFlienamePrefix;
  string dataName[FPanelDataSize] = {"-Displacement", "-Velocity", "-Acceleration", "-FluidPressure", "-BackPressure"};
  char rankLabel[3], surfaceLabel[3];
  vector<int> FPEProcess;
  double surfaceNumber(-1);
  bool firstLine;
  map<int, int> FPElemMap, FPSurfaceMap, firstFlemFlag;
  //map<int, int>::iterator FPIterator;
  vector<FPanelResultStruct> GobalFPanelResult;
  vector<FPanelIntermediateStruct> GobalFPanelIntermediate;
  ofstream FPanelOutputFlie;
  // Options
  int MPIflag(-1), nonMPIFormatOutput(-1), processResultOrIntermediate(-1), intermediateType(-1), outputDataType(-1);
  string menuInstruction;

  // Menu ~~~~~~~
  cout << "\nRASOFP - The program of rearranging the solution of flexible panel surface for CE/SE program";
  cout << "\nVersion 0.0" << endl;
  menuInstruction = "\nAre the results produced with MPI parallel computing? (0: No  1: Yes): ";
  intInput(menuInstruction, MPIflag, 0, 1);
  if (MPIflag == 1){
    menuInstruction = "\nPlease enter the file type you want to process (0: Result  1: Intermediate): ";
    intInput(menuInstruction, processResultOrIntermediate, 0, 1);
    if (processResultOrIntermediate == 0){
      menuInstruction = "\nDo you want the non-MPI version format outputs? (0: No  1: Yes): ";
      intInput(menuInstruction, nonMPIFormatOutput, 0, 1);
    }
    if (processResultOrIntermediate == 1){
      cout << "\nPlease enter the prefix of filename: ";
      cin >> FPanelResultFlienamePrefix;
    }
  }
  menuInstruction = "\nPlease enter the data type of the output files (0: ASCII  1: Binary): ";
  intInput(menuInstruction, outputDataType, 0, 1);
  // End of Menu ^^^^^^^

  if (MPIflag == 1){  // MPI
    readFPEProcess(FPEProcess);
    // Read Results ~~~~~~~
    for (int k = 0; k < FPEProcess.size(); k++) {
      sprintf(rankLabel, "%3.3d", FPEProcess[k]);
      LocalFPElemMapFliename = LocalFPElemMapFlienamePrefix;
      LocalFPElemMapFliename.append(rankLabel);
      LocalFPElemMapFliename.append(LocalFPElemMapFlienameSuffix);
      readLocalFPElemMap(LocalFPElemMapFliename, FPElemMap); 
      FPanelResultFliename = FPanelResultFlienamePrefix;
      if(processResultOrIntermediate == 0) 
        FPanelResultFliename.append("-");
      FPanelResultFliename.append(LM);
      FPanelResultFliename.append(rankLabel);
      FPanelResultFliename.append(dotDat);
      switch (processResultOrIntermediate) {
        case 0:  // Read FPanel-######-LM###.dat
          readFPanelResult(FPanelResultFliename, GobalFPanelResult, FPElemMap, MPIflag);
          break;
        case 1:  // Read FPanel_Inter...
          readFPanelIntermediate(FPanelResultFliename, GobalFPanelIntermediate, FPElemMap);
          break;
        default:
          break;
      }
      FPElemMap.clear();
    }
    // Finish Reading Results ^^^^^^^
    // Sorting Gobal Result ~~~~~~~
    cout << "\nProcessing " << setiosflags(ios::left) << setw(54) << setfill('.') << "Gobal Result";
    switch (processResultOrIntermediate) {
      case 0:  // Result 
        sort(GobalFPanelResult.begin(), GobalFPanelResult.end(), sortByTimeElemNo);
        break;
      case 1:  // Intermediate
        sort(GobalFPanelIntermediate.begin(), GobalFPanelIntermediate.end(), sortByElemNo);
        break;
      default:
        break;
    }
    cout << "Done!";
    // Finish Sorting Gobal Result ^^^^^^^
    if (nonMPIFormatOutput == 1){  // Write FPanel-######.dat
      FPanelResultFliename = FPanelResultFlienamePrefix;
      switch (outputDataType) {
        case 0:  // ASCII
          FPanelResultFliename.append(dotDat);
          cout << "\nWriting " << setiosflags(ios::left) << setw(57) << setfill('.') << FPanelResultFliename;
          FPanelOutputFlie.open(FPanelResultFliename.c_str(), std::ofstream::out);
          for (int k = 0; k < GobalFPanelResult.size(); k++) {
            FPanelOutputFlie << right << setiosflags (ios::fixed | ios::showpoint)
              << scientific << std::setprecision( 8) 
              << setw(15) << GobalFPanelResult[k].time << "\t"
              << setw(10) << GobalFPanelResult[k].elemNo << "\t"
              << scientific << std::setprecision(16)
              << setw(23) << GobalFPanelResult[k].data[0] << "\t"
              << setw(23) << GobalFPanelResult[k].data[1] << "\t"
              << setw(23) << GobalFPanelResult[k].data[2] << "\t"
              << setw(23) << GobalFPanelResult[k].data[3] << "\t"
	      << setw(23) << GobalFPanelResult[k].data[4] << "\t"
              << endl;
          }
          FPanelOutputFlie.close();
          cout << "Done!";
          break;       
        case 1: // Binary
          FPanelResultFliename.append(dotBin);
          cout << "\nWriting " << setiosflags(ios::left) << setw(57) << setfill('.') << FPanelResultFliename;
          FPanelOutputFlie.open(FPanelResultFliename.c_str(), std::ofstream::out | ios::binary);
          for (int k = 0; k < GobalFPanelResult.size(); k++) {
            FPanelOutputFlie.write(reinterpret_cast<const char *>(&GobalFPanelResult[k].time), sizeof(double));
            FPanelOutputFlie.write(reinterpret_cast<const char *>(&GobalFPanelResult[k].elemNo), sizeof(int));
            for (int j = 0; j < FPanelDataSize; j++) 
              FPanelOutputFlie.write(reinterpret_cast<const char *>(&GobalFPanelResult[k].data[j]), sizeof(double));
          }
          FPanelOutputFlie.close();
          cout << "Done!";
          break;
        default:
          break;
      }
    }
  } else {  // non-MPI
    FPanelResultFliename = FPanelResultFlienamePrefix;
    FPanelResultFliename.append(dotDat);
    readFPanelResult(FPanelResultFliename, GobalFPanelResult, FPElemMap, MPIflag);
  }

  // Final Outputs ~~~~~~~
  switch (processResultOrIntermediate) {
    case 1:  // Intermediate 
      outputFliename = "FPanel_Intermediate";
      switch (outputDataType) {
        case 0:  // ASCII
          outputFliename.append(dotDat);
          cout << "\nWriting " << setiosflags(ios::left) << setw(57) << setfill('.') << outputFliename;
          FPanelOutputFlie.open(outputFliename.c_str(), ios::app | ios::out);
          for (int k = 0; k < GobalFPanelIntermediate.size(); k++) {
            FPanelOutputFlie << setiosflags(ios::right) << setw(10) << GobalFPanelIntermediate[k].elemNo
              << setiosflags (ios::fixed | ios::showpoint);
            for (int j = 0; j < FPanelDataSize; j++) {
              FPanelOutputFlie << setw(23) << setprecision(16) << GobalFPanelIntermediate[k].data[j];
            }
            FPanelOutputFlie << endl;
          }
          break;
        case 1: // Binary
          outputFliename.append(dotBin);
          cout << "\nWriting " << setiosflags(ios::left) << setw(57) << setfill('.') << outputFliename;
          FPanelOutputFlie.open(outputFliename.c_str(), ios::app | ios::out | ios::binary);
          for (int k = 0; k < GobalFPanelIntermediate.size(); k++) {
            FPanelOutputFlie.write(reinterpret_cast<const char *>(&GobalFPanelIntermediate[k].elemNo), sizeof(int));
            for (int j = 0; j < FPanelDataSize; j++) 
              FPanelOutputFlie.write(reinterpret_cast<const char *>(&GobalFPanelIntermediate[k].data[j]), sizeof(double));
          }
          break;
        default:
          break;
      }
      FPanelOutputFlie.close();
      cout << "Done!";
      break;
    default:  // Result 
      readFPanelInfo(FPSurfaceMap, firstFlemFlag, MPIflag);
      cout << "\nWriting " << setiosflags(ios::left) << setw(57) << setfill('.') << "Final Outputs";
      for (int k = 0; k < GobalFPanelResult.size(); k++) {
        tempOutputFlienamePrefix = outputFlienamePrefix;
        sprintf(surfaceLabel, "%3.3d", FPSurfaceMap[GobalFPanelResult[k].elemNo]);
        tempOutputFlienamePrefix.append(surfaceLabel);
        for (int j = 0; j < FPanelDataSize; j++) {
          outputFliename = tempOutputFlienamePrefix;
          outputFliename.append(dataName[j]);
          switch (outputDataType) {
            case 0:  // ASCII
              outputFliename.append(dotDat);
              FPanelOutputFlie.open(outputFliename.c_str(), ios::app | ios::out);
              FPanelOutputFlie << right << setiosflags (ios::fixed | ios::showpoint) << scientific << std::setprecision(16) << setw(24);
              if (firstFlemFlag[GobalFPanelResult[k].elemNo] == 1) {
                if (FPanelOutputFlie.tellp() > 0) 
                  FPanelOutputFlie << endl;
                FPanelOutputFlie << GobalFPanelResult[k].time << "    ";
              }
              FPanelOutputFlie << GobalFPanelResult[k].data[j] << "  ";
              break;
            case 1: // Binary
              outputFliename.append(dotBin);
              FPanelOutputFlie.open(outputFliename.c_str(), ios::app | ios::out | ios::binary);
              if (surfaceNumber != FPSurfaceMap[GobalFPanelResult[k].elemNo]) 
                FPanelOutputFlie.write(reinterpret_cast<const char *>(&GobalFPanelResult[k].time), sizeof(double));
              FPanelOutputFlie.write(reinterpret_cast<const char *>(&GobalFPanelResult[k].data[j]), sizeof(double));
              break;
            default:
              break;
          }
          FPanelOutputFlie.close();
        }
        surfaceNumber = FPSurfaceMap[GobalFPanelResult[k].elemNo];
      }
      cout << "Done!";
      break;
  }

  cout << endl;

  return 0;
}
