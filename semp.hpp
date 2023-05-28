#pragma once
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

struct semp{

    int sempValue;
    pthread_cond_t semCondVar;
    pthread_mutex_t semMutex;

};


    void SemInit(semp & s, int32_t initial_value);
    void SemPost(semp & s);
    void SemWait(semp & s);


