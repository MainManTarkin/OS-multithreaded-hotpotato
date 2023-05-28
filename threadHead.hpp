#pragma once
#include <iostream>
#include <time.h>
#include <random>
#include "potatoClass.hpp"
#include "semp.hpp"

using namespace std;



struct threadArgHolder{

    int threadCountID;
    potatoClass *potatoC;
    semp *semophoreHolder;
    pthread_mutex_t *counterMutex;

};

void *threadEntry(void * arg);