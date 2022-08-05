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
extern const int BLOCKED_STATE;
extern const char WRITE_ONLY_FLAG[];

// declaring cpu sched codes
extern const int FCFSN_CPU_SCHED_CODE;
extern const int FCFSP_CPU_SCHED_CODE;
extern const int SJFN_CPU_SCHED_CODE;
extern const int SRTFP_CPU_SCHED_CODE;
extern const int RRP_CPU_SCHED_CODE;

// declarations of functions
void runSim ( ConfigDataType *configPtr, OpCodeType *metaDataMstrPtr );
void displaySimToMonitor( PCB *currentPCB, OpCodeType *currentCmd, PCB *headRef,
                          Boolean displayFlag, ConfigDataType *configPtr );
void *runOpCode( void *spinTime );
PCB *addPCBNode( OpCodeType *appStartPtr, int pidValue,
                                                    ConfigDataType *configPtr);
PCB *getNextJob( PCB *headRef, PCB *currentPCB, ConfigDataType *configPtr );

#endif
