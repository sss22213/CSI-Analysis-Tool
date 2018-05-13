#include "Analysis.h"
#include <stdlib.h>
#include <string.h>
int main(int argc,char *argv[])
{   
    Packet *obj = New_Packet(argv[1]);
    int Packet_Number = atoi(argv[2]);
    if(!strcmp(argv[3],"Num"))printf("%d",Packet_count(obj));
    else if(!strcmp(argv[3],"Packet_effection"))printf("%d",Packet_effection(obj,Packet_Number));
    else if(!strcmp(argv[3],"Packet_count_packet"))
    {
        int* ptr = Packet_count_packet(obj);
        int Num = Packet_count(obj);
        for(int i=0; i<Num; i++)printf("%d$",ptr[i]);
    }
    else
    {
        //Skip to not found 
        if(Find_PacketID(obj,Packet_Number))printf("1");
        else if(!strcmp(argv[3],"Bfee_count"))printf("%d",obj->bfee_count);
        else if(!strcmp(argv[3],"Perm"))printf("%d,%d,%d", obj->perm[0], obj->perm[1], obj->perm[2]);
        else if(!strcmp(argv[3],"Nrx"))printf("%d",obj->Nrx);
        else if(!strcmp(argv[3],"Ntx"))printf("%d",obj->Ntx);
        else if(!strcmp(argv[3],"Noise"))printf("%d",obj->noise);
        else if(!strcmp(argv[3],"RSSI"))printf("%u,%u,%u",obj->rssi_a,obj->rssi_b,obj->rssi_c);
        else if(!strcmp(argv[3],"CSI"))for(int i = 0 ; i < 360 ; i+=2)printf("$%d,%d|",obj->csi[i],obj->csi[i+1]);
    }
    Delete_Packet(obj);
    return 0;
}