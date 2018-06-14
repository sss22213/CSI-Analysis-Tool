#include "server.h"
int Find_text(const char* org,const char* obj)
{
    int cmp_flag = 0;
    for(int i = 0 ; i < strlen(obj) ; i++)
    {
        if(obj[i] == org[i])++cmp_flag;
    }
    if(cmp_flag >= strlen(obj))return 0;
    else return 1;
}
char* Packet_parse(const char* string)
{
    int head_loc = 0;
    int tail_loc = 0;
    const char* ptr_str = string;
    while((*string++)!='$')head_loc++;
    while((*string++)!='$')tail_loc++;
    tail_loc += head_loc;
    char *new_string = (char *)malloc(sizeof(char)*tail_loc - head_loc + 1);
    int count = 0;
    for(int i = head_loc+1 ; i <= tail_loc ; i++)
    {
        new_string[count] = ptr_str[i];
        count++;
    }
    new_string[count]='\0';
    return new_string;
} 
int main()
{
    WSADATA ws;
    SOCKET socket_server;
    unsigned char server_reply[3000]={0};
    WSAStartup(MAKEWORD(2,2),&ws);
    socket_server = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8888);
    server.sin_family = AF_INET;

    bind(socket_server,(struct sockaddr *)&server,sizeof(server));
    listen(socket_server,3);
    int file_flag = 0;
    Packet *obj;
    int Packet_Number = 0;
    while(1)
    {
        int c = sizeof(struct sockaddr_in);
        SOCKET client_socket = accept(socket_server , (struct sockaddr *)&server, &c);
        if (client_socket == INVALID_SOCKET)
        {
            printf("accept failed with error code : %d" , WSAGetLastError());
            exit(5);
        }
        if(recv(client_socket , server_reply , 3000 , 0) <= 0)continue;
        if(!Find_text(server_reply,"Filename"))
        {
            obj = New_Packet(Packet_parse(server_reply));
            printf("Setting Path at %s\n",Packet_parse(server_reply));
        }
        else if(!Find_text(server_reply,"Packet_Number"))
        {
            Packet_Number = atoi(Packet_parse(server_reply));
            printf("Setting Packet_Number at %d\n",Packet_Number);
        }
        else if(!Find_text(server_reply,"Packet_count_packet"))
        {
            int* ptr = Packet_count_packet(obj);
            int Num = Packet_count(obj);
            send(client_socket,(const char*)ptr,sizeof((const char*)ptr),0);
        }
        else
        {
            if(Find_PacketID(obj,Packet_Number))
            {
                send(client_socket,"1",sizeof("1"),0);
            }
            else if(!Find_text(server_reply,"Bfee_count"))
            {
                char str1[5];
                sprintf(str1,"%d",(obj->bfee_count));
                send(client_socket,str1,sizeof(str1),0);
            }
            else if(!Find_text(server_reply,"Perm"))
            {
                char str1[12];
                sprintf(str1,"%d,%d,%d", obj->perm[0],obj->perm[1],obj->perm[2]);
                send(client_socket,str1,sizeof(str1),0);
            }
            else if(!Find_text(server_reply,"Nrx"))
            {
                char str1[3];
                sprintf(str1,"%d", obj->Nrx);
                send(client_socket,str1,sizeof(str1),0);
            }
            else if(!Find_text(server_reply,"Ntx"))
            {
                char str1[3];
                sprintf(str1,"%d", obj->Ntx);
                send(client_socket,str1,sizeof(str1),0);
            }
            else if(!Find_text(server_reply,"Ntx"))
            {
                char str1[3];
                sprintf(str1,"%d", obj->noise);
                send(client_socket,str1,sizeof(str1),0);
            }
            else if(!Find_text(server_reply,"RSSI"))
            {
                char str1[12];
                sprintf(str1,"%u,%u,%u", obj->rssi_a,obj->rssi_b,obj->rssi_c);
                send(client_socket,str1,sizeof(str1),0);
            }
            else if(!Find_text(server_reply,"CSI"))
            {
                char str1[3000];
                int index = 0;
                for(int i = 0 ; i < 360 ; i+=2)index += sprintf(&str1[index],"$%d,%d|",obj->csi[i],obj->csi[i+1]);
                send(client_socket,str1,sizeof(str1),0);
            }
            
        }
        //Packet_Number = (int)Packet_parse(server_reply);




    }

    return 0;
}