#include "Analysis.h"
#include <stdlib.h>
#include <string.h>
int main(int argc,char *argv[])
{
    Packet *obj = New_Packet();
    int Packet_Number = atoi(argv[2]);
    Find_PacketID(argv[1],obj,Packet_Number);
    if(!strcmp(argv[3],"Bfee_count"))printf("%d",obj->bfee_count);
    else if(!strcmp(argv[3],"Perm"))printf("%d,%d,%d", obj->perm[0], obj->perm[1], obj->perm[2]);
    else if(!strcmp(argv[3],"Nrx"))printf("%d",obj->Nrx);
    else if(!strcmp(argv[3],"Ntx"))printf("%d",obj->Ntx);
    else if(!strcmp(argv[3],"Noise"))printf("%d",obj->noise);
    else if(!strcmp(argv[3],"RSSI"))printf("%u,%u,%u",obj->rssi_a,obj->rssi_b,obj->rssi_c);
    else if(!strcmp(argv[3],"CSI"))for(int i = 0 ; i < 360 ; i+=2)printf("$%d,%d|",obj->csi[i],obj->csi[i+1]);
    Delete_Packet(obj);
    return 0;
}