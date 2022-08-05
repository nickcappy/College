#include "simulator.h"

// global constants for states
const int NEW_STATE = 0;
const int READY_STATE = 1;
const int RUNNING_STATE = 2;
const int EXIT_STATE = 3;
const char WRITE_ONLY_FLAG[] = "w";

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
    const int LOGTO_MONITOR_CODE = 5;
    const int LOGTO_FILE_CODE = 6;
    const int LOGTO_BOTH_CODE = 7;

    int logToValue = configPtr->logToCode, uniquePID;
    int averageCycleTime =
                    ( configPtr->procCycleRate + configPtr->quantumCycles ) / 2;

    Boolean displayToMonitorFlag = False;
    Boolean displayToFileFlag = False;
    OpCodeType *iterator;
    PCB *headNode;

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

      // make new pointer to iterate through file
      iterator = metaDataMstrPtr;

      // checks that it was succssfully copied
      if( iterator != NULL )
      {
        while ( !(compareString( iterator -> command, "sys" ) == STR_EQ
                && compareString( iterator -> strArg1, "end" ) == STR_EQ) )
        {
          if( compareString( iterator -> command, "app" ) != STR_EQ
              && compareString( iterator -> strArg1, "start" ) != STR_EQ )
              {
                // increment pid
                uniquePID++;

                // adding to PCB linked list
                headNode = PCBaddNode( headNode, uniquePID,
                                                  configPtr, metaDataMstrPtr );
              }
            // move pointer to next line in meta data file
            iterator = iterator->nextNode;
        }
      }

      // while loop iterates through the process through the pcb
        // in "os" anything added must have os before not the process #
        // since fcfs - n we just run the processes as we receive them
      while( headNode->nextNode != NULL )
      {
        // start timer here for whole run sim
        runTimer( averageCycleTime );

        // if both of the upload flags are true
           // calls uploadFile and prints to the screen
           // with the time stamp from the pcb
        if( displayToFileFlag == True && displayToMonitorFlag == True )
        {
          displaySim( headNode, metaDataMstrPtr );

          copyToFile( configPtr, headNode, metaDataMstrPtr );
        }

        // otherwise, just the monitor flag is True
           // calls to have the string printed to the
           // screen with the timestamp and process #
       if( displayToMonitorFlag == True )
       {
         displaySim( headNode, metaDataMstrPtr );
       }

        // otherwise, just the file flag is True
           // calls the uploadFile methods to upload it to a file
       if( displayToFileFlag == True )
       {
         copyToFile( configPtr, headNode, metaDataMstrPtr );
       }

        // set state from running to exit
        headNode->processState = EXIT_STATE;

        // iterate to next pcb
        headNode = headNode->nextNode;
      }
    }
  }


void displaySim( PCB *enitreLinkedNodes, OpCodeType *metaDataMstrPtr )
{
  // initialze variables
  PCB *tempNode;
  int tempPid;
  double timer;
  char timeInStr[ 12 ];

  // set to a temporary pointer
  tempNode = enitreLinkedNodes;

  // call timer
  runTimer( 15 );

  // calculate current time remaining in the current process
     // "time left in process"
  timer = accessTimer( LAP_TIMER, timeInStr );

  // Display OS Start
  printf("%f, OS: Simulator Start\n", timer);

  // set all processes to ready from new
  while ( enitreLinkedNodes->nextNode != NULL )
  {
    // calculate current time remaining in the current process
       // "time left in process"
    timer = accessTimer( LAP_TIMER, timeInStr );

    // set process states to ready
    tempNode->processState = READY_STATE;

    printf("%f OS: Process %d set to READY state from NEW state\n",
                                                        timer, tempNode->pid );

    tempNode = tempNode->nextNode;

    timer = accessTimer( LAP_TIMER, timeInStr );


    tempPid = enitreLinkedNodes->pid;
    // display how much ms remaining in first process
    printf("%f OS: Process %d has %d msec remaining\n",
                                        timer, tempPid, tempNode->msRemaining );

    // set from ready to running
    enitreLinkedNodes->processState = RUNNING_STATE;

    // loop through entire app start
    while( compareString( metaDataMstrPtr->command, "sys" ) != STR_EQ
            && compareString( metaDataMstrPtr->strArg1, "end" ) != STR_EQ)
    {
      // check if mem allocate
      if( compareString( metaDataMstrPtr->command, "mem" ) == STR_EQ )
      {
        printf("%f, Process: %d, attempting memory allocate request\n",
                                                timer, enitreLinkedNodes->pid);
      }

      // check if cpu
      if( compareString( metaDataMstrPtr->command, "cpu" ) == STR_EQ )
      {
        printf("%f, Process: %d, cpu process operation start\n",
                                                timer, enitreLinkedNodes->pid);

        printf("%f, Process: %d, cpu process operation end\n",
                                                timer, enitreLinkedNodes->pid);
      }

      // check if dev
      if( compareString( metaDataMstrPtr->command, "dev" ) == STR_EQ )
      {
        // check if dev in
        if( compareString( metaDataMstrPtr->inOutArg, "in" ) == STR_EQ )
        {
          if( compareString( metaDataMstrPtr->strArg1, "sound signal" ) ==
                                                                        STR_EQ )
          {
            printf("%f, Process: %d, sound signal input operation start\n",
                                                timer, enitreLinkedNodes->pid);

            printf("%f, Process: %d, sound signal input operation end\n",
                                                timer, enitreLinkedNodes->pid);
          }

          if (compareString( metaDataMstrPtr->strArg1, "hard drive" ) ==
                                                                        STR_EQ )
          {
            printf("%f, Process: %d, hard drive input operation start\n",
                                                timer, enitreLinkedNodes->pid);

            printf("%f, Process: %d, hard drive input operation end\n",
                                                timer, enitreLinkedNodes->pid);
          }

          if ( compareString( metaDataMstrPtr->strArg1, "video signal" ) ==
                                                                        STR_EQ )
          {
            printf("%f, Process: %d, video signal input operation start\n",
                                                timer, enitreLinkedNodes->pid);

            printf("%f, Process: %d, video signal input operation end\n",
                                                timer, enitreLinkedNodes->pid);
          }
        }

        // check if dev out
        if( compareString( metaDataMstrPtr->inOutArg, "out" ) == STR_EQ )
        {
          if( compareString( metaDataMstrPtr->strArg1, "monitor" ) == STR_EQ )
          {
            printf("%f, Process: %d, monitor output operation start\n",
                                                timer, enitreLinkedNodes->pid);

            printf("%f, Process: %d, monitor output operation end\n",
                                                timer, enitreLinkedNodes->pid);
          }

          if (compareString( metaDataMstrPtr->strArg1, "usb" ) == STR_EQ )
          {
            printf("%f, Process: %d, usb output operation start\n",
                                                timer, enitreLinkedNodes->pid);

            printf("%f, Process: %d, usb output operation end\n",
                                                timer, enitreLinkedNodes->pid);
          }

          if ( compareString( metaDataMstrPtr->strArg1, "serial" ) == STR_EQ )
          {
            printf("%f, Process: %d, serial output operation start\n",
                                                timer, enitreLinkedNodes->pid);

            printf("%f, Process: %d, serial output operation end\n",
                                                timer, enitreLinkedNodes->pid);
          }
        }
      }

      // display current process has ended
      printf("%f OS: Process %d ended\n", timer, enitreLinkedNodes->pid);

      // set current proccess to end state
      enitreLinkedNodes->processState = EXIT_STATE;

      // set current process to exit
      printf("%f OS: Process %d set to EXIT\n", timer, enitreLinkedNodes->pid);

      // go to next process
      enitreLinkedNodes = enitreLinkedNodes->nextNode;
    }

    printf( "%f, OS: Simulation end\n", timer );
  }
}

PCB *PCBaddNode( PCB *copyData, int iteration, ConfigDataType *configPtr,
                                                  OpCodeType *metaDataMstrPtr )
{
  // checks pointer is not null
  if( copyData == NULL )
  {
    // initialize variables
    OpCodeType *workingNode = metaDataMstrPtr;
    int totalCycles, totalTime, totalIOTime = configPtr->ioCycleRate;

    copyData->pid = ++iteration;
    copyData->processState = NEW_STATE;

    // checks to see that you arent at the end of the app (app end)
    while( compareString( workingNode->strArg1, "end" ) != STR_EQ )
    {
      // checks if cpu command is given
      if ( compareString( workingNode->command, "cpu" ) == STR_EQ )
      {
        // adds total cycles given in file
        totalCycles = totalCycles + workingNode->intArg2;

        // adds total time needed to complete operation
        totalTime =
                totalTime + ( workingNode->intArg2 * configPtr->procCycleRate );
      }

      // checks if dev command is given
      else if( compareString( workingNode->command, "dev" ) == STR_EQ )
      {
        // adds total cycles given in file
        totalCycles = totalCycles + workingNode->intArg2;

        // adds total time needed to complete operation
        totalTime =
                  totalTime + ( workingNode->intArg2 * configPtr->ioCycleRate );
      }
      // iterates to next node
      workingNode = workingNode->nextNode;
    }

    // sets unassigned values to the PCB
    copyData->numOfCycles = totalCycles;
    copyData->msRemaining = totalTime + totalIOTime;

  }

  // linking next node references
  copyData->nextNode = PCBaddNode( copyData->nextNode,
                                        iteration, configPtr, metaDataMstrPtr );

  return copyData;
}
