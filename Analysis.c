#include "Analysis.h"
void Find_ID_Position(FILE *fileptr,long* positive_XY,int Positive)
{
    //Find Packet number
    unsigned int *field_len = (unsigned int*)malloc(sizeof(unsigned int));
    if(field_len==NULL){printf("Memory Leak");exit(2);}

    unsigned char *code = (unsigned char*)malloc(sizeof(unsigned char));
    if(code==NULL){printf("Memory Leak");exit(2);}
    //Find file head
    fseek(fileptr,0,SEEK_END);
    long File_end = ftell(fileptr);
    //From Head
    rewind(fileptr);
    // Need 3 bytes -- 2 byte size field and 1 byte code
    for(int i = 0; i < Positive ; i++)
    {
        //find Code
        do
        {
            fread(code,1,1,fileptr);
            if(ftell(fileptr) >= File_end){printf("Not found");exit(2);}
        }
        while(*code!=187);
        *positive_XY = ftell(fileptr);
        //find field_len
        fseek(fileptr,-2,SEEK_CUR);
        fread(field_len,1,2,fileptr);
    }
    *(positive_XY+1) = ftell(fileptr) + *field_len;
}
void Find_PacketID(const char* path, Packet* buff,long Packet_number)
{
    FILE *fileptr;
    fileptr = fopen(path,"rb");
    long positive_XY[2]={0};
    unsigned int len = 1;
    unsigned int calc_len = 0;
    unsigned char* inBytes;
    unsigned int agc;
    unsigned int antenna_sel;
    unsigned int fake_rate_n_flags;
    //Check Packet is exist or not
    do
    {
        Find_ID_Position(fileptr,positive_XY,Packet_number++);
	    fseek(fileptr,positive_XY[0],SEEK_SET);
        inBytes = (unsigned char*)malloc(sizeof(unsigned char)*(positive_XY[1]-positive_XY[0]));
	    fread(inBytes,1,positive_XY[1]-positive_XY[0],fileptr);
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