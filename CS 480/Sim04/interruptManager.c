#include "interruptManager.h"

// global variables
Interrupt *headOfQueue;


void interruptQueue( PCB* nextToAdd )
{
  Interrupt *iteratorOfQueue;
  iteratorOfQueue = addInterruptToQueue( nextToAdd );


}


Interrupt *addInterruptToQueue( PCB *passedInNode )
{
  Interrupt *nextInQueue;
  nextInQueue = headOfQueue;

  // checks to see if current queue is empty
  if ( queueIsEmpty( headOfQueue ) )
  {
    // adds next item to be added to the queue, to the queue
    nextInQueue -> Storedpid = passedInNode -> pid;
    nextInQueue -> StoredProcessState = passedInNode -> processState;
  }

  else
  {
    // iterates to end of the queue to add next value to the end
    while( headOfQueue != NULL )
    {
      nextInQueue = nextInQueue -> nextInQueue;
    }

    // adds next item to be added to the queue, to the queue
    nextInQueue -> Storedpid = passedInNode -> pid;
    nextInQueue -> StoredProcessState = passedInNode -> processState;
  }

  return nextInQueue;
}

Boolean queueIsEmpty( Interrupt queue )
{
  return queue == NULL;
}
