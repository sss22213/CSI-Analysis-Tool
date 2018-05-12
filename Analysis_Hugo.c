#include "Analysis.h"
int Packet_count(const char* path)
{
    //Read all file into memory
    FILE *fileptr;
    fileptr = fopen(path,"rb");
    fseek(fileptr,0,SEEK_END);
    long End_file = ftell(fileptr);
    unsigned char *Space = (unsigned char*)malloc(sizeof(unsigned char)*End_file);
    if(Space==NULL){printf("Memory Leak");exit(2);}
    //From Head
    rewind(fileptr);
    fread(Space,End_file,1,fileptr);
    fclose(fileptr);
    //declare
    unsigned char field_len[2];
    long Head = 0;
    long count = 0;
    long point_location = 0;
    do{
        do{
            point_location ++;
        }while(*(Space + point_location)!=187);
        field_len[0] = *(Space + point_location - 2);
        field_len[1] = *(Space + point_location - 1);
        //Point Move 1
        point_location++;
        int Nrx = *(Space + point_location + 8);
	    int Ntx = *(Space + point_location + 9);
        int len = *(Space + point_location + 16) + (*(Space + point_location + 17) << 8);
	    int calc_len = (30 * (Nrx * Ntx * 8 * 2 + 3) + 7) / 8;
        if(calc_len==len)++count;
    }while(point_location < End_file - 100);
    fclose(fileptr);
    return count;
}
int Find_ID_Position(unsigned char *Space,long *positive_l,int Positive,long tail)
{
    long point_location = 0;
    for(int i = 0; i < Positive ; i++)
    {
        do{
            point_location ++;
            if(point_location >= tail)return 1;
        }while(*(Space + point_location)!=187);
    }
    *positive_l = point_location + 1;
    return 0;
}
int Find_PacketID(const char* path, Packet* buff,long Packet_number)
{
    FILE *fileptr;
    fileptr = fopen(path,"rb");
    long positive_XY = 0;
    unsigned int len = 1;
    unsigned int calc_len = 0;
    unsigned int agc;
    unsigned int antenna_sel;
    unsigned int fake_rate_n_flags;
    fseek(fileptr,0,SEEK_END);
    long End_file = ftell(fileptr);
    unsigned char *inBytes = (unsigned char*)malloc(sizeof(unsigned char)*End_file);
    if(inBytes==NULL){printf("Memory Leak");exit(2);}
    //From Head
    rewind(fileptr);
    fread(inBytes,End_file,1,fileptr);
    //Check Packet is exist or not
    do
    {
        if (Find_ID_Position(inBytes,&positive_XY,Packet_number++,End_file))return 1;
        inBytes = inBytes + positive_XY;
        //Analysis Packet
        buff->timestamp_low = inBytes[0] + (inBytes[1] << 8) +
		    (inBytes[2] << 16) + (inBytes[3] << 24);
	    buff->bfee_count = inBytes[4] + (inBytes[5] << 8);
	    buff->Nrx = inBytes[8];
	    buff->Ntx = inBytes[9];
	    buff->rssi_a = inBytes[10];
	    buff->rssi_b = inBytes[11];
	    buff->rssi_c = inBytes[12];
	    buff->noise = inBytes[13];
	    agc = inBytes[14];
	    antenna_sel = inBytes[15];
	    len = inBytes[16] + (inBytes[17] << 8);
	    fake_rate_n_flags = inBytes[18] + (inBytes[19] << 8);
	    calc_len = (30 * (buff->Nrx * buff->Ntx * 8 * 2 + 3) + 7) / 8;
    }while(len != calc_len);
	unsigned int i, j;
	unsigned int index = 0, remainder;
	unsigned char *payload = &inBytes[20];
	char tmp;
	int size[] = {buff->Nrx, buff->Ntx, 30};
   
	// Compute CSI from all this crap :) 
    int CSI_index = 0;
	for (i = 0; i < 30; ++i)
	{
		index += 3;
		remainder = index % 8;
		for (j = 0; j < buff->Nrx * buff->Ntx; ++j)
		{
			tmp = (payload[index / 8] >> remainder) |
				(payload[index/8+1] << (8-remainder));
			//printf("Real: %d\n", tmp);
            buff->csi[CSI_index] = (tmp);
			tmp = (payload[index / 8+1] >> remainder) |
				(payload[index/8+2] << (8-remainder));
			//printf("Fake: %d\n", tmp);
            buff->csi[CSI_index+1] = (tmp);
            CSI_index+=2;
			index += 16;
		}
	}
	/* Compute the permutation array */
	buff->perm[0] = ((antenna_sel) & 0x3) + 1;
	buff->perm[1] = ((antenna_sel >> 2) & 0x3) + 1;
	buff->perm[2] = ((antenna_sel >> 4) & 0x3) + 1;
    fclose(fileptr);
    return 0;
}
Packet *New_Packet(void)
{
    Packet *Packet_obj = (Packet*)malloc(sizeof(Packet));
    Packet_obj->timestamp_low = 0;
    Packet_obj->bfee_count = 0;
    Packet_obj->Nrx = 0;
    Packet_obj->Ntx = 0;
    Packet_obj->rssi_a = 0;
    Packet_obj->rssi_b = 0;
    Packet_obj->rssi_c = 0;
    Packet_obj->noise = 0;
    Packet_obj->perm = (unsigned int*)malloc(sizeof(unsigned int)*3);
    if(Packet_obj->perm==NULL){perror("Memory Leak");exit(2);}
    Packet_obj->csi= (int*)malloc(sizeof(int)*720);
    if(Packet_obj->csi==NULL){perror("Memory Leak");exit(2);}
    return Packet_obj; 
}
void Delete_Packet(Packet* obj)
{
    free(obj->csi);
    obj->csi = NULL;
    free(obj->perm);
    obj->perm = NULL;
    free(obj);
    obj = NULL;
}