#ifndef INTERRUPTMANAGER_H
#define INTERRUPTMANAGER_H

#include "DataTypes.h"

// declare global variables


Interrupt *addInterruptToQueue( PCB *passedInNode );
void interruptQueue( PCB* nextToAdd );
Boolean queueIsEmpty( Interrupt queue );

#endif
