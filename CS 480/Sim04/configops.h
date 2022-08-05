// protect from multiple compiling
#ifndef CONFIGOPS_H
#define CONFIGOPS_H

// header files
#include "StringUtils.h"
#include "DataTypes.h"

// global constants - may be used in other files
  // Note: starts at 3 so it does not compete eith StringManipCode values
typedef enum { CFG_FILE_ACCESS_ERR = 3,
               CFG_CORRUPT_DESCRIPTOR_ERR,
               CFG_DATA_OUT_OF_RANGE_ERR,
               CFG_CORRUPT_PROMPT_ERR,
               CFG_VERSION_CODE,
               CFG_MD_FILE_NAME_CODE,
               CFG_CPU_SCHED_CODE,
               CFG_QUANT_CYCLES_CODE,
               CFG_MEM_AVAILABLE_CODE,
               CFG_PROC_CYCLES_CODE,
               CFG_IO_CYCLES_CODE,
               CFG_LOG_TO_CODE,
               CFG_LOG_FILE_NAME_CODE } ConfigCodeMessages;

typedef enum { CPU_SCHED_SJF_N_CODE,
               CPU_SCHED_SRTF_P_CODE,
               CPU_SCHED_FCFS_P_CODE,
               CPU_SCHED_RR_P_CODE,
               CPU_SCHED_FCFS_N_CODE,
               LOGTO_MONITOR_CODE,
               LOGTO_FILE_CODE,
               LOGTO_BOTH_CODE } ConfigDataCodes;

// function prototypes
ConfigDataType *clearConfigData( ConfigDataType *configData );
void configCodeToString( int code, char *outString);
void displayConfigData( ConfigDataType *configData );
Boolean getConfigData( char*fileName, ConfigDataType **configData, char *endStateMsg);
ConfigDataCodes getCpuSchedCode( char *codeStr );
int getDataLineCode( char *dataBuffer);
ConfigDataCodes getLogToCode( char *logToStr );
Boolean valueInRange( int lineCode, int intVal, double doubleVal, char *stringVal);

#endif
