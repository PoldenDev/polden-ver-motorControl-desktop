#pragma once

typedef enum {idle, waitForAck, waitForFifoFree} TSendState;

#define MOTOR_CNT 10
#define FPGA_FREQ_24 25000000
#define FPGA_FREQ_25 24000000
typedef struct{
    QQueue<QString> contrStringQueue;
    bool bCmdOk;
    TSendState sendState;

} TMotorStr;

typedef enum{
    MT_IDLE,
    MT_GoDOWN,
    MT_GoUP,
    MT_INIT_GoDOWN,
    MT_INIT_GoUp,
} TMotorState ;


typedef struct{
    quint32 div;
    quint32 steps;
    int dir;
    qint32 finishPos; //pos after cmd
    quint32 msecsFor;
    quint32 skipStartTime;
} DivPosDataStr;
