#include "potatoClass.hpp"

potatoClass::potatoClass(int playerCount)
{

    potatoCount = vector<potatoCounter>(playerCount);
}

void potatoClass::incrementCount(int threadIndex)
{
    potatoCount[threadIndex].counter++;
    ;
}

int potatoClass::getCount(int vectorIndex)
{

    return potatoCount[vectorIndex].counter;
    ;
}

void potatoClass::heldPotato(int vectorIndex, bool stateToSet)
{

    potatoCount[vectorIndex].isHeld = stateToSet;
}

void potatoClass::setThreadState(int vectorIndex, bool stateToSet)
{

    potatoCount[vectorIndex].threadAlive = stateToSet;
}

bool potatoClass::currentlyHoldsPotato(int vectorIndex)
{

    return potatoCount[vectorIndex].isHeld;
}

bool potatoClass::geThreadState(int vectorIndex)
{

    return potatoCount[vectorIndex].threadAlive;
}