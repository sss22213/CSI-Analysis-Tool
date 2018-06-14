#include "Analysis.h"
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
int* Packet_count_packet(Packet* obj)
{ 
    long Total_Num = Packet_count(obj);
    int* Box = (unsigned int*)malloc(sizeof(unsigned int)*Total_Num);
    if(Box==NULL){printf("Memory Leak");exit(2);}
    long Packet_number = 0;
    long index = 0;
    while(Total_Num > 0)
    {
        if(!Packet_effection(obj,Packet_number++))
        {
            Box[index] = Packet_number;
            Total_Num--;
            index++;
        }
    }
    return Box;
}
int Packet_effection(Packet* obj,unsigned int Packet_number)
{
    unsigned char* inBytes = obj->inBytes;
    long positive_XY = 0;   
    unsigned int len = 1;
    unsigned int calc_len = 0;
    unsigned int agc;
    unsigned int antenna_sel;
    unsigned int fake_rate_n_flags;
    if (Find_ID_Position(inBytes,&positive_XY,Packet_number,obj->file_tail))return 1;
    inBytes = inBytes + positive_XY;
    //Analysis Packet
    unsigned char Nrx = inBytes[8];
	unsigned char Ntx = inBytes[9];
	agc = inBytes[14];
	antenna_sel = inBytes[15];
	len = inBytes[16] + (inBytes[17] << 8);
	fake_rate_n_flags = inBytes[18] + (inBytes[19] << 8);
	calc_len = (30 * (Nrx * Ntx * 8 * 2 + 3) + 7) / 8;
    if(len != calc_len)return 1;
    else return 0;
}
int Packet_count(Packet* obj)
{
    unsigned char* Space = obj->inBytes;
    //declare
    unsigned char field_len[2];
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
    }while(point_location < obj->file_tail - (field_len[1]+256*field_len[0]));
    return count;
}
int Find_PacketID(Packet* buff,long Packet_number)
{
    unsigned char* inBytes = buff->inBytes;
    long positive_XY = 0;
    unsigned int len = 1;
    unsigned int calc_len = 0;
    unsigned int agc;
    unsigned int antenna_sel;
    unsigned int fake_rate_n_flags;
    //Check Packet is exist or not
    do
    {
        if(Packet_effection(buff,Packet_number))return 1;
        Find_ID_Position(inBytes,&positive_XY,Packet_number++,buff->file_tail);
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

    return 0;
}
Packet *New_Packet(const char* path)
{
    Packet *Packet_obj = (Packet*)malloc(sizeof(Packet));
    //Read all of file
    FILE *fileptr;
    fileptr = fopen(path,"rb");
    fseek(fileptr,0,SEEK_END);
    Packet_obj->file_tail = ftell(fileptr);
    Packet_obj->inBytes = (unsigned char*)malloc(sizeof(unsigned char)*(Packet_obj->file_tail));
    if(Packet_obj->inBytes==NULL){printf("Memory Leak");exit(2);}
    rewind(fileptr);
    fread(Packet_obj->inBytes,Packet_obj->file_tail,1,fileptr);
    fclose(fileptr);
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