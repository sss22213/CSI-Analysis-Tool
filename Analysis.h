#ifndef _Analysis_H_
#define _Analysis_H_
#include <stdio.h>
#include <stdlib.h>
typedef struct _Packet Packet;
typedef struct _Packet
{
    unsigned long timestamp_low;
    unsigned short bfee_count;
    unsigned int Nrx;
    unsigned int Ntx;
    unsigned int rssi_a;
    unsigned int rssi_b;
    unsigned int rssi_c;
    char noise;
    unsigned int *perm;
    int *csi;
}Packet;
Packet *New_Packet(void);
void Find_PacketID(const char*,Packet*,long);
void Delete_Packet(Packet*);

#endif