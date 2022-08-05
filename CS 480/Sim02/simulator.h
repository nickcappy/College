#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "configops.h"
#include "metadataops.h"
#include "simtimer.h"
#include "DataTypes.h"
#include "fileUpload.h"

// declaring const values
extern const int NEW_STATE;
extern const int READY_STATE;
extern const int RUNNING_STATE;
extern const int EXIT_STATE;
extern const char WRITE_ONLY_FLAG[];

// declarations of functions
void runSim ( ConfigDataType *configPtr, OpCodeType *metaDataMstrPtr );
void displaySim( PCB *enitreLinkedNodes, OpCodeType *metaDataMstrPtr );
PCB *PCBaddNode( PCB *copyData, int iteration, ConfigDataType *configPtr,
                                                  OpCodeType *metaDataMstrPtr );

#endif
