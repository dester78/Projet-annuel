// #include <winsock2.h>
// #include <ws2bth.h>
// #include <bluetoothapis.h>

// #include <stdio.h>

// #include <bluetoothHandler.h>



// #define MAX_NAME 248

// typedef ULONGLONG bt_addr, *pbt_addr, BT_ADDR, *PBT_ADDR;



// union {

//     // Storage for returned struct

//     CHAR buf[5000];

//     // properly align buffer to BT_ADDR requirements

//     SOCKADDR_BTH _Unused_;

// } butuh;

// typedef ULONGLONG bt_addr, *pbt_addr, BT_ADDR, *PBT_ADDR;


// BOOL FindingBtDevices(){

//     WSAQUERYSET wsaq;

//     HANDLE hLookup;

//     LPWSAQUERYSET pwsaResults;

//     DWORD dwSize;

//     BOOL bHaveName;

//     BT_ADDR btAddr;

//     DWORD dwNameSpace;


//     pwsaResults = (LPWSAQUERYSET) butuh.buf;

//     dwSize  = sizeof(butuh.buf);



//     ZeroMemory(&wsaq, sizeof(wsaq));

//     wsaq.dwSize = sizeof(wsaq);

//     // namespace MUST be NS_BTH for bluetooth queries

//     wsaq.dwNameSpace = NS_BTH;

//     wsaq.lpcsaBuffer = NULL;



//     printf("\n");

//     // initialize searching procedure

//     if (WSALookupServiceBegin(&wsaq, LUP_CONTAINERS, &hLookup) == SOCKET_ERROR)

//     {
//         printf("WSALookupServiceBegin() failed %d\r\n", WSAGetLastError());

//         return FALSE;

//     }

//     else

//         printf("WSALookupServiceBegin() pretty damn OK!\n");



//     ZeroMemory(pwsaResults, sizeof(WSAQUERYSET));

//     pwsaResults->dwSize = sizeof(WSAQUERYSET);

//     pwsaResults->dwNameSpace = NS_BTH;

//     pwsaResults->lpBlob = NULL;



//     // iterate through all found devices, returning name and address

//     while (WSALookupServiceNext(hLookup, LUP_RETURN_NAME | LUP_RETURN_ADDR, &dwSize, pwsaResults) == 0)

//     {

//         btAddr = ((SOCKADDR_BTH *)pwsaResults->lpcsaBuffer->RemoteAddr.lpSockaddr)->btAddr;

//         // Verify the returned name

//         bHaveName = (pwsaResults->lpszServiceInstanceName) && *(pwsaResults->lpszServiceInstanceName);


//         dwNameSpace = pwsaResults->dwNameSpace;

//         // print name and address

//         wprintf (L"Name\tNAP Address\tSAP Address\tName Space\n");

//         printf ("%s\t0X%04X\t\t0X%08X\t0X%0X\n", pwsaResults->lpszServiceInstanceName, (unsigned int)GET_NAP(btAddr), (unsigned int)GET_SAP(btAddr),

//                  (unsigned int) dwNameSpace);

//     }



//     if(WSALookupServiceEnd(hLookup) == 0)

//         printf("WSALookupServiceEnd(hLookup) is OK!\n");

//     else

//         printf("WSALookupServiceEnd(hLookup) failed with error code %ld\n",WSAGetLastError());

//     return TRUE;

// }



// //
// //        WSADATA wsd;
// //
// //        SOCKET s, s2;
// //
// //        SOCKADDR_BTH sab, sab2;
// //
// //        // NULL_GUID
// //
// //        GUID nguiD = {00000000-0000-0000-0000-000000000000};
// //
// //        int ilen, iResult;
// //
// //
// //        // This should be const void * type for non-char data
// //
// //        char *sendbuf = "Test data from receiver...";
// //
// //        int recvbuflen = DEFAULT_BUFLEN;
// //
// //        // Change the type accordingly for non-char data
// //
// //        char recvbuf[DEFAULT_BUFLEN] = "";
// //
// //
// //
// //        if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
// //
// //        {
// //
// //            printf("Unable to load Winsock! Error code is %d\n", WSAGetLastError());
// //
// //            return 1;
// //
// //        }
// //
// //        else
// //
// //            printf("WSAStartup() is OK, Winsock lib loaded!\n");
// //
// //
// //        WSAQUERYSET wsaq;
// //
// //        ZeroMemory(&wsaq, sizeof(wsaq));
// //
// //        wsaq.dwSize = sizeof(wsaq);
// //
// //        wsaq.dwNameSpace = NS_BTH;
// //
// //        wsaq.lpcsaBuffer = NULL;
// //        s = socket (AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
// //
// //        if (s == INVALID_SOCKET)
// //
// //        {
// //
// //            printf ("Socket creation failed, error %d\n", WSAGetLastError());
// //
// //            return 1;
// //
// //        }
// //
// //        else
// //
// //            printf ("socket() looks fine!\n");
// //
// //
// //
// //        memset (&sab, 0, sizeof(sab));
// //
// //        sab.addressFamily  = AF_BTH;
// //
// //        // We hardcoded it
// //
// //        sab.port = 4;
// //
// //
// //
// //        if (0 != bind(s, (SOCKADDR *) &sab, sizeof(sab)))
// //
// //        {
// //
// //            printf ("bind() failed with error code %d\n", WSAGetLastError());
// //
// //            closesocket (s);
// //
// //        }
// //
// //        else
// //
// //            printf ("bind() looks fine!\n");
// //
// //
// //
// //        if(listen (s, 5) == 0){
// //            printf("listen() is OK! Listening for connection...\n");
// //
// //        }
// //
// //
// //
// //        else
// //            printf("listen() failed with error code %d\n", WSAGetLastError());
// //
// //
// //
// //        while(1)
// //
// //        {
// //
// //            // Get information on the port assigned
// //
// //            ilen = sizeof(sab2);
// //
// //            s2 = accept (s, (SOCKADDR *)&sab2, &ilen);
// //            printf("TOTO");
// //
// //            if (s2 == INVALID_SOCKET)
// //
// //            {
// //                printf ("accept() failed with error code %d\n", WSAGetLastError ());
// //                break;
// //
// //            }
// //
// //            else{
// //                printf ("accept(), is OK buddy!\n");
// //
// //            }
// //
// //
// //                        printf("toto");
// //
// //
// //            // Print the info
// //
// //            printf ("Connection came from %04x%08x to channel %d\n", GET_NAP(sab2.btAddr), GET_SAP(sab2.btAddr), sab2.port);
// //
// //        }
// //
// //
// //
// //        // Receive until the peer closes the connection
// //
// //        do {
// //
// //            iResult = recv(s2, recvbuf, recvbuflen, 0);
// //
// //            if (iResult > 0)
// //
// //                printf(" %d Bytes received from sender\n", iResult);
// //
// //            else if ( iResult == 0 )
// //
// //                printf("Connection closed by peer!\n");
// //
// //            else
// //
// //                printf("recv() failed with error code %d\n", WSAGetLastError());
// //
// //
// //
// //        } while(iResult > 0);
// //
// //
// //
// //        // Echo back the data
// //
// //        iResult = send(s2, recvbuf, recvbuflen, 0 );
// //
// //        if (iResult == SOCKET_ERROR)
// //
// //        {
// //
// //            printf("send() failed with error code %d\n", WSAGetLastError());
// //
// //            closesocket(s2);
// //
// //            WSACleanup();
// //
// //            return 1;
// //
// //        }
// //
// //        else
// //
// //        {
// //
// //            printf("send() is OK!\n");
// //
// //            printf("Bytes Sent: %d\n", iResult);
// //
// //        }
// //
// //
// //
// //        if(closesocket(s) == 0)
// //
// //            printf("closesocket() pretty fine!\n");
// //
// //        if(WSACleanup () == 0)
// //
// //            printf("WSACleanup() is OK!\n");
// //
// //
// //
// //
// //



// #define DEFAULT_BUFLEN 512



// typedef ULONGLONG bt_addr, *pbt_addr, BT_ADDR, *PBT_ADDR;

// int str2ba(BTH_ADDR *btaddr,const char *straddr)
// {
//     int i;
//     unsigned int aaddr[6];
//     BTH_ADDR tmpaddr = 0;

//     if (sscanf(straddr, "%02x:%02x:%02x:%02x:%02x:%02x",
//                &aaddr[0], &aaddr[1], &aaddr[2],
//                &aaddr[3], &aaddr[4], &aaddr[5]) != 6)
//         return 1;
//     *btaddr = 0;
//     for (i = 0; i < 6; i++) {
//         tmpaddr = (BTH_ADDR) (aaddr[i] & 0xff);
//         *btaddr = ((*btaddr) << 8) + tmpaddr;
//     }
//     return 0;
// }



// //int main(int argc, char **argv)
// //
// //{
// //
// //    WSADATA wsd;
// //
// //    BOOL retVal;
// //
// //
// //
// //    if(WSAStartup(MAKEWORD(2,2), &wsd) != 0)
// //
// //        printf("WSAStartup() failed with error code %ld\n", WSAGetLastError());
// //
// //    else
// //
// //        printf("WSAStartup() is OK!\n");
// //
// //
// //
// //    retVal = FindingBtDevices();
// //
// //
// //
// //    if(FindingBtDevices() == FALSE)
// //
// //        printf("\nNo bulutooth device in range found!\n");
// //
// //    else
// //
// //        printf("\nWell, found some bluetooth devices around!\n");
// //
// //
// //
// //    if(WSACleanup() == 0)
// //
// //        printf("WSACleanup() is OK!\n");
// //
// //    else
// //
// //        printf("WSACleanup() failed with error code %ld\n", WSAGetLastError());
// //
// //    return 0;
// //
// //}

// //int main(int argc, char **argv)
// //
// //{
// //
// //    WSADATA wsd;
// //
// //    SOCKET s;
// //
// //    SOCKADDR_BTH sab;
// //
// //    // Hard coded directly, got it from the receiver/server
// //
// //    BT_ADDR aSddr = 0X00140305F461;//0X001DD9EA24D8;
// //
// //    int iResult;
// //
// //    // This should be const void * type for non-char data
// //
// //    char *sendbuf = "Test data from client...";
// //
// //    int recvbuflen = DEFAULT_BUFLEN;
// //
// //    // Change the type accordingly for non-char data
// //
// //    char recvbuf[DEFAULT_BUFLEN] = "";
// //
// //
// //
// //    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
// //
// //    {
// //
// //        printf("Unable to load Winsock! Error code is %d\n", WSAGetLastError());
// //
// //        return 1;
// //
// //    }
// //
// //    else
// //
// //        printf("WSAStartup() is OK, Winsock lib loaded!\n");
// //
// //
// //
// //    s = socket (AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
// //
// //    if (s == INVALID_SOCKET)
// //
// //    {
// //
// //        printf ("Socket creation failed, error %d\n", WSAGetLastError());
// //
// //        WSACleanup();
// //
// //        return 1;
// //
// //    }
// //
// //    else
// //
// //        printf ("socket() looks fine!\n");
// //
// //
// //
// //    memset (&sab, 0, sizeof(sab));
// //
// //    sab.addressFamily  = AF_BTH;
// //
// //    // Set the btAddr member to a BT_ADDR variable that
// //
// //    // contains the address of the target device. App
// //
// //    // can accept the device address as a string but must convert
// //
// //    // the address and store it in a variable of type BT_ADDR.
// //
// //    sab.btAddr = aSddr;
// //
// //
// //
// //
// //
// //
// //
// //
// //
// //    // If the service identifier is available, then set the
// //
// //    // serviceClassId member of SOCKADDR_BTH to the GUID of
// //
// //    // the RFCOMM-based service. In this case, the client
// //
// //    // performs an SDP query and then uses the resulting server channel.
// //
// //    // sab.serviceClassId = nguiD;
// //
// //
// //
// //    // Or If you want to use a hard-coded channel number, set the
// //
// //    // port member of SOCKADDR_BTH to the server channel number (1-31)
// //
// //    sab.port = 1;
// //
// //
// //
// //    // Connect to the Bluetooth socket, created previously
// //
// //    if (connect (s, (SOCKADDR *)&sab, sizeof(sab)) == SOCKET_ERROR)
// //
// //    {
// //
// //        printf("connect() failed with error code %d\n", WSAGetLastError ());
// //
// //        closesocket(s);
// //
// //        WSACleanup();
// //
// //        return 1;
// //
// //    }
// //
// //    else
// //
// //        printf("connect() should be fine!\n");
// //
// //
// //
// //    // Send some data
// //
// //    iResult = send(s, sendbuf, (int)strlen(sendbuf), 0 );
// //
// //    if (iResult == SOCKET_ERROR) {
// //
// //        printf("send() failed with error code %d\n", WSAGetLastError());
// //
// //        closesocket(s);
// //
// //        WSACleanup();
// //
// //        return 1;
// //
// //    }
// //
// //    else
// //
// //    {
// //
// //        printf("send() is OK!\n");
// //        printf("Bytes Sent: %d\n", iResult);
// //
// //    }
// //
// //
// //
// //    // shutdown the stream connection since no more data will be sent
// //
// //    iResult = shutdown(s, SD_SEND);
// //
// //    if (iResult == SOCKET_ERROR)
// //
// //    {
// //
// //        printf("shutdown() failed with error code %d\n", WSAGetLastError());
// //
// //        closesocket(s);
// //
// //        WSACleanup();
// //
// //        return 1;
// //
// //    }
// //
// //    else
// //
// //        printf("shutdown() is working!\n");
// //
// //
// //
// //    // receive
// //
// //    // Receive until the peer closes the connection
// //
// //    do {
// //
// //        iResult = recv(s, recvbuf, recvbuflen, 0);
// //
// //
// //
// //        if (iResult > 0)
// //
// //            printf(" %d Bytes received from sender\n", iResult);
// //
// //        else if (iResult == 0)
// //
// //            printf("Connection was closed by peer!\n");
// //
// //        else
// //
// //            printf("recv() failed with error code %d\n", WSAGetLastError());
// //
// //
// //
// //    } while(iResult > 0);
// //
// //
// //
// //    // Do all the cleanup
// //
// //    if(closesocket(s) == 0)
// //
// //        printf("closesocket() pretty fine!\n");
// //
// //    if(WSACleanup () == 0)
// //
// //        printf("WSACleanup() is OK!\n");
// //
// //    return 0;
// //
// //}