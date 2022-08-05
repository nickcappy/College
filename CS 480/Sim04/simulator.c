#include "simulator.h"

// global constants for states
const int NEW_STATE = 0;
const int READY_STATE = 1;
const int RUNNING_STATE = 2;
const int EXIT_STATE = 3;
const int BLOCKED_STATE = 4;
const char WRITE_ONLY_FLAG[] = "w";
const int HIGHEST_CPU_LENGTH = 10;

const int FCFSN_CPU_SCHED_CODE = 4;
const int FCFSP_CPU_SCHED_CODE;
const int SJFN_CPU_SCHED_CODE = 0;
const int SRTFP_CPU_SCHED_CODE;
const int RRP_CPU_SCHED_CODE;
/*
Function name: runSim
Algorithm: master driver for simulator operations;
           conducts OS simulation with varying scheduling strategies
           and varying numbers of processes
Precondition: given head pointer to config data and meta data
Postcondition: simulation is provided, file output is provided as configured
Exceptions: none
Notes: none
*/

void runSim ( ConfigDataType *configPtr, OpCodeType *metaDataMstrPtr )
{
  // declaring values
  pthread_t mainThread;

  int logToValue = configPtr->logToCode;
  int numOfProcesses = 0;
  int averageCycleTime = (configPtr->procCycleRate + configPtr->ioCycleRate) /2;

  char* outputFileName = outputFileName;
  char timeStr[ STD_STR_LEN ];

  Boolean displayToMonitorFlag = False;
  Boolean displayToFileFlag = False;

  OpCodeType *iteratorNode = metaDataMstrPtr;
  PCB *headPCB = NULL;
  PCB *currentPCB = NULL;
  PCB *previousPCB = NULL;

  char cpuSchedCodeString[ HIGHEST_CPU_LENGTH ];
  configCodeToString( configPtr -> cpuSchedCode, cpuSchedCodeString );

  pthread_create( &mainThread, NULL, &runOpCode, &averageCycleTime );
  pthread_join( mainThread, NULL );

  // display title, with underline
    // function: printf
  printf( "Simulator Run\n" );
  printf( "=============\n\n" );

  // checks that the pointers put in, have values attached
  if( configPtr != NULL && metaDataMstrPtr != NULL )
  {
    // check to see if display is to monitor, file, or both
    if( logToValue == LOGTO_BOTH_CODE )
    {
      // copy to file flag is true
      displayToFileFlag = True;

      // displays to monitor flag is true
      displayToMonitorFlag = True;
    }

    // check to see if display is to monitor
    if( logToValue == LOGTO_MONITOR_CODE )
    {
      // displays to monitor flag is true
      displayToMonitorFlag = True;
    }

    // check to see if display is to file
    if( logToValue == LOGTO_FILE_CODE )
    {
      // copy to file flag is true
      displayToFileFlag = True;
    }

  // checks cpu sched code for FCFS-N
  if ( compareString( cpuSchedCodeString, "FCFS-N" ) == STR_EQ )
  {
    while ( compareString( iteratorNode->command, "sys" ) != STR_EQ ||
            compareString( iteratorNode->strArg1, "end" ) != STR_EQ )
      {
      // every time the line = app start create a new pcb
         // initially set the state to 0 or new
      if( compareString( iteratorNode->command, "app" ) == STR_EQ
    	 && compareString( iteratorNode->strArg1, "start" ) == STR_EQ )
         {
         // add the pcb to the linked list
         // pcb is a data structure with variables
         if( headPCB == NULL )
            {
            // if the headPCB is null, set the head pointer
            currentPCB = addPCBNode( iteratorNode, numOfProcesses, configPtr );

            // sets currentPCB back to the first PCB created
            headPCB = currentPCB;
            }

         else
            {
            // set the current pcb to previous and make current a new pcb and
            // set the previous nodes next node equal to the current node
            previousPCB = currentPCB;
            currentPCB = addPCBNode( iteratorNode, numOfProcesses, configPtr );

            // iterates through to next PCB node
            previousPCB -> nextNode = currentPCB;
            }

         // incremement the number of numOfProcesses
         numOfProcesses++;
         }

      // sets iteratorNode to the next node
      iteratorNode = iteratorNode -> nextNode;
    }
  }

  // checks cpu sched code for SJF-N
  else if ( compareString( cpuSchedCodeString, "SJF-N" ) == STR_EQ )
  {
    // calls helper function to get shortest job for SJF-N
    currentPCB = getNextJob( headPCB, currentPCB, configPtr );

    // loop through meta data file till it hits the end of the file
    while ( compareString( iteratorNode->command, "sys" ) != STR_EQ ||
            compareString( iteratorNode->strArg1, "end" ) != STR_EQ )
    {
      // every time the line = app start create a new pcb
         // initially set the state to 0 or new
      if( compareString( iteratorNode->command, "app" ) == STR_EQ
    	 && compareString( iteratorNode->strArg1, "start" ) == STR_EQ )
       {

         // add the pcb to the linked list
         // pcb is a data structure with variables
         if( headPCB == NULL )
         {
           // calls helper function to get shortest job for SJF-N
           currentPCB = getNextJob( headPCB, currentPCB, configPtr );

           // counts number of cycles iterated through
            currentPCB -> numOfCycles += 1;

            // sets currentPCB back to the first PCB created
            currentPCB = headPCB;
          }

         else
         {
            previousPCB = currentPCB;
            currentPCB = getNextJob( headPCB, currentPCB, configPtr );

            currentPCB -> numOfCycles += 1;
            previousPCB -> nextNode = currentPCB;
          }

           // incremement the number of numOfProcesses
           numOfProcesses++;
       }

      iteratorNode = iteratorNode -> nextNode;
    }
  }

  // checks cpu sched code for FCFS-P
  else if ( compareString( cpuSchedCodeString, "FCFS-P" ) == STR_EQ )
  {
    while ( compareString( iteratorNode->command, "sys" ) != STR_EQ ||
            compareString( iteratorNode->strArg1, "end" ) != STR_EQ )
      {
      // every time the line = app start create a new pcb
         // initially set the state to 0 or new
      if( compareString( iteratorNode->command, "app" ) == STR_EQ
    	 && compareString( iteratorNode->strArg1, "start" ) == STR_EQ )
         {
         // add the pcb to the linked list
         // pcb is a data structure with variables
         if( headPCB == NULL )
            {
            // if the headPCB is null, set the head pointer
            currentPCB = addPCBNode( iteratorNode, numOfProcesses, configPtr );

            // sets currentPCB back to the first PCB created
            headPCB = currentPCB;
            }

         else if ( headPCB != NULL || headPCB->processState != BLOCKED_STATE )
            {
            // set the current pcb to previous and make current a new pcb and
            // set the previous nodes next node equal to the current node
            previousPCB = currentPCB;
            currentPCB = addPCBNode( iteratorNode, numOfProcesses, configPtr );

            // iterates through to next PCB node
            previousPCB -> nextNode = currentPCB;
            }

         // incremement the number of numOfProcesses
         numOfProcesses++;
         }

      // sets iteratorNode to the next node
      iteratorNode = iteratorNode -> nextNode;
    }
  }

  // checks cpu sched code for SRTF-P
  else if ( compareString( cpuSchedCodeString, "SRTF-P" ) == STR_EQ )
  {
    // calls helper function to get shortest job for SJF-N
    currentPCB = getNextJob( headPCB, currentPCB, configPtr );

    // loop through meta data file till it hits the end of the file
    while ( compareString( iteratorNode->command, "sys" ) != STR_EQ ||
            compareString( iteratorNode->strArg1, "end" ) != STR_EQ )
    {
      // every time the line = app start create a new pcb
         // initially set the state to 0 or new
      if( compareString( iteratorNode->command, "app" ) == STR_EQ
    	 && compareString( iteratorNode->strArg1, "start" ) == STR_EQ )
       {

         // add the pcb to the linked list
         // pcb is a data structure with variables
         if( headPCB == NULL )
         {
           // calls helper function to get shortest job for SJF-N
           currentPCB = getNextJob( headPCB, currentPCB, configPtr );

           // counts number of cycles iterated through
            currentPCB -> numOfCycles += 1;

            // sets currentPCB back to the first PCB created
            currentPCB = headPCB;
          }

         else
         {
            previousPCB = currentPCB;
            currentPCB = getNextJob( headPCB, currentPCB, configPtr );

            currentPCB -> numOfCycles += 1;
            previousPCB -> nextNode = currentPCB;
          }

           // incremement the number of numOfProcesses
           numOfProcesses++;
       }

      iteratorNode = iteratorNode -> nextNode;
    }
  }

  // checks cpu sched code for RR-P
  else if ( compareString( cpuSchedCodeString, "RR-P" ) == STR_EQ )
  {
    while ( compareString( iteratorNode->command, "sys" ) != STR_EQ ||
            compareString( iteratorNode->strArg1, "end" ) != STR_EQ )
      {
      // every time the line = app start create a new pcb
         // initially set the state to 0 or new
      if( compareString( iteratorNode->command, "app" ) == STR_EQ
    	 && compareString( iteratorNode->strArg1, "start" ) == STR_EQ )
         {
         // add the pcb to the linked list
         // pcb is a data structure with variables
         if( headPCB == NULL )
            {
            // if the headPCB is null, set the head pointer
            currentPCB = addPCBNode( iteratorNode, numOfProcesses, configPtr );

            // sets currentPCB back to the first PCB created
            headPCB = currentPCB;
            }

         else
            {
            // set the current pcb to previous and make current a new pcb and
            // set the previous nodes next node equal to the current node
            previousPCB = currentPCB;
            currentPCB = addPCBNode( iteratorNode, numOfProcesses, configPtr );

            // iterates through to next PCB node
            previousPCB -> nextNode = currentPCB;
            }

         // incremement the number of numOfProcesses
         numOfProcesses++;
         }

      // sets iteratorNode to the next node
      iteratorNode = iteratorNode -> nextNode;
    }
  }

  // defaults to FCFS-N
  else
  {
    while ( compareString( iteratorNode->command, "sys" ) != STR_EQ ||
            compareString( iteratorNode->strArg1, "end" ) != STR_EQ )
      {
      // every time the line = app start create a new pcb
         // initially set the state to 0 or new
      if( compareString( iteratorNode->command, "app" ) == STR_EQ
    	 && compareString( iteratorNode->strArg1, "start" ) == STR_EQ )
         {

         // add the pcb to the linked list
         // pcb is a data structure with variables
         if( headPCB == NULL )
            {
            // if the headPCB is null, set the head pointer
            currentPCB = addPCBNode( iteratorNode, numOfProcesses, configPtr );

            currentPCB -> numOfCycles += 1;
            headPCB = currentPCB;
            }

         else
            {
            // set the current pcb to previous and make current a new pcb and
            // set the previous nodes next node equal to the current node
            previousPCB = currentPCB;
            currentPCB = addPCBNode( iteratorNode, numOfProcesses, configPtr );

            currentPCB -> numOfCycles += 1;
            previousPCB -> nextNode = currentPCB;
            }

         // incremement the number of numOfProcesses
         numOfProcesses++;
         }

      iteratorNode = iteratorNode -> nextNode;
    }
  }

  currentPCB = headPCB;

  if( displayToMonitorFlag == True )
   {
     // Display OS Start
     accessTimer( ZERO_TIMER, timeStr );
     printf( "%s, OS: Simulator Start\n", timeStr);

     while ( currentPCB != NULL )
     {
       accessTimer( LAP_TIMER, timeStr );

       currentPCB -> processState = READY_STATE;

       runTimer( currentPCB->metaDataPtr->intArg2 * configPtr->ioCycleRate );
       printf("%s, OS: Process %d, set to READY from NEW state\n", timeStr,
                                                              currentPCB->pid );

       currentPCB = currentPCB->nextNode;
     }

     currentPCB = headPCB;

     displaySimToMonitor( currentPCB, currentPCB -> metaDataPtr, headPCB,
                          displayToMonitorFlag, configPtr );
   }

   else if ( displayToFileFlag == True )
    {
      printf("\nOutputting simulator to file only\n");

      uploadConfigToFile( configPtr -> logToFileName, currentPCB,
                                                            displayToFileFlag );
    }
  }
}

void displaySimToMonitor( PCB *currentPCB, OpCodeType *currentCmd, PCB *headRef,
                          Boolean displayFlag, ConfigDataType *configPtr )
{
  // initializing
  pthread_t mainThread;

  char timeStr[ STD_STR_LEN ];
  int calculatedThread;

  PCB *temp = headRef;

  temp -> nextNode = headRef -> nextNode;

  while( currentPCB -> processState == READY_STATE )
    {
      // print which process which selected
      accessTimer(LAP_TIMER, timeStr);
      printf("\n%s, OS: Process %d selected with %d ms remaining\n",
                            timeStr, currentPCB->pid, currentPCB->msRemaining );

      currentPCB -> processState = RUNNING_STATE;

      // print that the process was set from the ready to running state
      accessTimer(LAP_TIMER, timeStr);
      printf("%s, OS: Process %d set from READY to RUNNING\n\n",
                                                    timeStr, currentPCB->pid );

      // loop through the metadata till app end from pointer in pcb
      while( compareString(currentCmd->strArg1, "end") != STR_EQ )
        {
          calculatedThread = currentCmd -> intArg2 * configPtr -> procCycleRate;

          pthread_create( &mainThread, NULL, &runOpCode, &calculatedThread );
          pthread_join( mainThread, NULL );

          // check for cpu process
          if( compareString(currentCmd->command, "cpu") == STR_EQ )
          {
             accessTimer(LAP_TIMER, timeStr);
             printf("%s, Process %d, cpu process operation start\n",
                      timeStr, currentPCB->pid );

             runTimer( currentPCB->metaDataPtr->intArg2 *
                                                    configPtr->ioCycleRate );
             accessTimer(LAP_TIMER, timeStr);
             printf("%s, Process %d, cpu process operation end\n",
                      timeStr, currentPCB->pid );
        }

        // check for io process
        else if( compareString( currentCmd->command, "dev" ) == STR_EQ )
        {
        // checks each of the string arguments
        if ( compareString( currentCmd->inOutArg, "in" ) == STR_EQ )
          {
          // check for hard drive, sound signal, video signal, keyboard
          if( compareString( currentCmd->strArg1, "hard drive" ) == STR_EQ )
             {
               accessTimer(LAP_TIMER, timeStr);
               printf("%s, Process %d, hard drive input operation start\n",
                                                      timeStr, currentPCB->pid);

               runTimer( currentPCB->metaDataPtr->intArg2 *
                                                    configPtr->ioCycleRate );
               accessTimer(LAP_TIMER, timeStr);
               printf("%s, Process %d, hard drive input operation end\n",
                                                      timeStr, currentPCB->pid);
             }

          if( compareString( currentCmd->strArg1, "sound signal" ) == STR_EQ )
             {
             accessTimer(LAP_TIMER, timeStr);
             printf("%s, Process %d, sound signal input operation start\n",
                                                     timeStr, currentPCB->pid);

             runTimer( currentPCB->metaDataPtr->intArg2 * configPtr->ioCycleRate );
             accessTimer(LAP_TIMER, timeStr);
             printf("%s, Process %d, sound signal input operation end\n",
                                                     timeStr, currentPCB->pid);
             }

          if( compareString(currentCmd->strArg1, "video signal") == STR_EQ )
             {
             accessTimer(LAP_TIMER, timeStr);
             printf("%s, Process %d, video signal input operation start\n",
                                                     timeStr, currentPCB->pid);

             runTimer( currentPCB->metaDataPtr->intArg2 * configPtr->ioCycleRate );
             accessTimer(LAP_TIMER, timeStr);
             printf("%s, Process %d, video signal input operation end\n",
                                                     timeStr, currentPCB->pid);
             }

          if( compareString(currentCmd->strArg1, "keyboard") == STR_EQ )
             {
             accessTimer(LAP_TIMER, timeStr);
             printf("%s, Process %d, keyboard input operation start\n",
                                                     timeStr, currentPCB->pid);

             runTimer( currentPCB->metaDataPtr->intArg2 * configPtr->ioCycleRate );
             accessTimer(LAP_TIMER, timeStr);
             printf("%s, Process %d, keyboard input operation end\n",
                                                     timeStr, currentPCB->pid);
             }
          }

        // check for out string argument
        if ( compareString(currentCmd->inOutArg, "out") == STR_EQ )
          {
          // check for monitor, usb, video signal, sound signal, serial
          if( compareString(currentCmd->strArg1, "monitor") == STR_EQ )
             {
             accessTimer(LAP_TIMER, timeStr);
             printf("%s, Process %d, monitor output operation start\n",
                                                     timeStr, currentPCB->pid);

             runTimer( currentPCB->metaDataPtr->intArg2 * configPtr->ioCycleRate );
             accessTimer(LAP_TIMER, timeStr);
             printf("%s, Process %d, monitor output operation end\n",
                                                     timeStr, currentPCB->pid);
             }

          if( compareString(currentCmd->strArg1, "usb") == STR_EQ )
             {
             accessTimer(LAP_TIMER, timeStr);
             printf("%s, Process %d, usb output operation start\n",
                                                     timeStr, currentPCB->pid);

             runTimer( currentPCB->metaDataPtr->intArg2 * configPtr->ioCycleRate );
             accessTimer(LAP_TIMER, timeStr);
             printf("%s, Process %d, usb output operation end\n",
                                                     timeStr, currentPCB->pid);
             }

          if( compareString(currentCmd->strArg1, "video signal") == STR_EQ )
             {
             accessTimer(LAP_TIMER, timeStr);
             printf("%s, Process %d, video signal output operation start\n",
                                                     timeStr, currentPCB->pid);

             runTimer( currentPCB->metaDataPtr->intArg2 * configPtr->ioCycleRate );
             accessTimer(LAP_TIMER, timeStr);
             printf("%s, Process %d, video signal output operation end\n",
                                                     timeStr, currentPCB->pid);
             }

          if( compareString(currentCmd->strArg1, "sound signal") == STR_EQ )
             {
             accessTimer(LAP_TIMER, timeStr);
             printf("%s, Process %d, sound signal output operation start\n",
                                                     timeStr, currentPCB->pid);

             runTimer( currentPCB->metaDataPtr->intArg2 * configPtr->ioCycleRate );
             accessTimer(LAP_TIMER, timeStr);
             printf("%s, Process %d, sound signal output operation end\n",
                                                     timeStr, currentPCB->pid);
             }

          if( compareString(currentCmd->strArg1, "serial") == STR_EQ )
             {
             accessTimer(LAP_TIMER, timeStr);
             printf("%s, Process %d, serial output operation start\n",
                                                     timeStr, currentPCB->pid);

             runTimer(currentPCB->metaDataPtr->intArg2*configPtr->ioCycleRate);
             accessTimer(LAP_TIMER, timeStr);
             printf("%s, Process %d, serial output operation end\n",
                                                     timeStr, currentPCB->pid);
             }
          }
        }

        else if ( compareString( currentCmd->command, "mem" ) == STR_EQ )
        {
           accessTimer(LAP_TIMER, timeStr);
           printf("%s, Process %d, memory allocation request\n",
                                                   timeStr, currentPCB->pid);

           runTimer( currentPCB->metaDataPtr->intArg2*configPtr->ioCycleRate );
           accessTimer(LAP_TIMER, timeStr);

           // place holder until I can fix my memory allocation
           // right now, it should always pass
           if ( currentPCB != NULL )
           {
             // place holder for mem allocate function

             printf("%s, Process %d, memory allocation request successful\n",
                                                   timeStr, currentPCB->pid);
           }

           else
           {
             printf("%s, Process %d, failed mem allocate request\n",
                                                   timeStr, currentPCB->pid);

             // end currnet process and move to next
             currentPCB -> processState = EXIT_STATE;

             printf("%s, OS: Segmentation fault, Process %d ended\n",
                                                 timeStr, currentPCB -> pid);

            currentPCB = currentPCB -> nextNode;
           }
        }

        // new line for after the opcodes
        currentCmd = currentCmd -> nextNode;
      }

      accessTimer(LAP_TIMER, timeStr);
      printf("\n%s, OS: Process %d ended\n", timeStr, currentPCB -> pid );

      currentPCB -> processState = EXIT_STATE;

      accessTimer(LAP_TIMER, timeStr);
      printf("%s, OS: Process %d set to EXIT", timeStr, currentPCB -> pid );

      currentCmd = currentCmd -> nextNode;

      if( currentPCB -> nextNode != NULL )
      {
        currentPCB = currentPCB -> nextNode;
      }

      if( compareString( currentCmd->command, "sys" ) == STR_EQ &&
                        compareString( currentCmd->strArg1, "end" ) == STR_EQ )
      {
        accessTimer(LAP_TIMER, timeStr);
        printf("\n%s, OS: System Stop", timeStr );

        accessTimer(LAP_TIMER, timeStr);
        printf("\n%s, OS: Simulation End\n", timeStr );
      }
  }
}

void *runOpCode( void *spinTime )
{
    runTimer( * ( int * ) spinTime );

    pthread_exit( NULL );
}


PCB *addPCBNode( OpCodeType *appStartPtr, int pidValue,
                                                    ConfigDataType *configPtr )
{
   // initialize variables
   PCB *returnPCB;
   OpCodeType *iterator;
   int totalCPUCycles = 0;
   int totalIOCycles = 0;
   int totalProcessTime;
   int totalIOTime;

   returnPCB = ( PCB * ) malloc( sizeof( PCB ) );

   // initializing a ptr to the appStart
   iterator = appStartPtr;

   // set the initial values of the pcb
   returnPCB->pid = pidValue;
   returnPCB->processState = NEW_STATE;
   returnPCB->metaDataPtr = appStartPtr;
   returnPCB->nextNode = NULL;

   // while loop through the op codes inside the process
   while( compareString(iterator->strArg1, "end") != STR_EQ )
      {
      // check that it is a cpu process
      if (  compareString(iterator->command, "cpu") == STR_EQ )
         {

         // grab the number of cycles and add it to the variable
         totalCPUCycles += iterator->intArg2;

         }
      // otherwise it is i/o
      else if( compareString(iterator->command, "dev") == STR_EQ )
         {

         // update the number of cycles each opcode has
         totalIOCycles += iterator->intArg2;

         }

      // incrementing the iterator variable
      iterator = iterator->nextNode;
      }

   totalProcessTime = totalCPUCycles * configPtr->procCycleRate;
   totalIOTime = totalIOCycles * configPtr->ioCycleRate;

   // update the number of cycles in the process and the total time remaning
   returnPCB->numOfCycles = totalIOCycles + totalCPUCycles;
   returnPCB->msRemaining = totalProcessTime + totalIOTime;

   // return the pcb that was created
   return returnPCB;
}

PCB *getNextJob( PCB *headRef, PCB *currentPCB, ConfigDataType *configPtr )
{
  PCB *iteratorNode = NULL;
  PCB *returnPCB;

  iteratorNode = headRef;

  // checks if cpu code is SJF-N
  if ( configPtr -> cpuSchedCode == SJFN_CPU_SCHED_CODE )
  {
    if ( currentPCB -> processState == EXIT_STATE )
    {
      while ( iteratorNode != NULL )
      {
        if( iteratorNode -> processState != EXIT_STATE )
        {
          currentPCB = iteratorNode;
        }

        iteratorNode = iteratorNode -> nextNode;
      }
    }

    returnPCB = currentPCB;

    iteratorNode = headRef;

    while( iteratorNode != NULL )
    {
      if ( iteratorNode -> msRemaining < currentPCB -> msRemaining &&
                                    iteratorNode -> processState != EXIT_STATE )
      {
        returnPCB = iteratorNode;

        currentPCB = iteratorNode;
      }

      iteratorNode = iteratorNode -> nextNode;
    }
  }

  // checks if cpu code is FCFS-N
  if ( configPtr->cpuSchedCode == FCFSN_CPU_SCHED_CODE )
  {
    returnPCB = currentPCB;
  }

  return returnPCB;
}
