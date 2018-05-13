#ifndef _Analysis_H_
#define _Analysis_H_
#include <stdio.h>
#include <stdlib.h>
#define Hugo
typedef struct _Packet Packet;
typedef struct _Packet
{
    unsigned char *inBytes;
    long file_tail;
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
Packet *New_Packet(const char*);
int Find_PacketID(Packet*,long);
void Delete_Packet(Packet*);
int Packet_count(Packet*);
int *Packet_count_packet(Packet*);
#ifdef Hugo
    int Packet_effection(Packet*,unsigned int);
#endif
#endif