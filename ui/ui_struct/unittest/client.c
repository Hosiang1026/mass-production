#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8888

int main(int argc, char **argv)
{
    int iRet;
    int iAddrLen;
    int iSendLen;
    int iSocketClient;
    struct sockaddr_in tSocketServerAddr;

    if (argc != 3)
    {
        printf("Usage:\n");
        printf("%s <server_ip> <str>\n", argv[0]);
        return -1;
    }
    
    iSocketClient = socket(AF_INET, SOCK_DGRAM, 0);

    tSocketServerAddr.sin_family = AF_INET;
    tSocketServerAddr.sin_port = htons(SERVER_PORT);
    tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
    if(0 == inet_aton(argv[1], &tSocketServerAddr.sin_addr))
    {
        printf("invalid server_ip\n");
        return -1;
    }
    
    memset(tSocketServerAddr.sin_zero, 0, 8);

#if 0
    iRet = connect(iSocketClient, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
    if (-1 == iRet)
    {
        printf("connect error!\n");
    }
 #endif
    iAddrLen = sizeof(struct sockaddr);
    iSendLen = sendto(iSocketClient, argv[2], strlen(argv[2]), 0,
    (const struct sockaddr *)&tSocketServerAddr, iAddrLen);
    close(iSocketClient);
    
    return 0;
}