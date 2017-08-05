#pragma once

typedef enum {idle, waitForAck, waitForFifoFree} TSendState;

#define MOTOR_CNT 10
typedef struct{
    QQueue<QString> contrStringQueue;
    bool bCmdOk;
    TSendState sendState;

} TMotorStr;

typedef struct{
    int div;
    int steps;
    int dir;
    int pos;
} DivPosDataStr;

typedef enum{
    MT_IDLE,
    MT_GoDOWN
} TMotorState ;
