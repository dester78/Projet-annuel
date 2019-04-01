#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>


// int main(int argc, char **argv){
 
// struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
//     char buf[1024] = { 0 };
//     int s, client, bytes_read;
//     socklen_t opt = sizeof(rem_addr);

//     // allocate socket
//     s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

//     // bind socket to port 1 of the first available 
//     // local bluetooth adapter
//     loc_addr.rc_family = AF_BLUETOOTH;
//     loc_addr.rc_bdaddr = *BDADDR_ANY;
//     loc_addr.rc_channel = (uint8_t) 1;
//     bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

//     // put socket into listening mode
//     listen(s, 1);

//     // accept one connection
//     client = accept(s, (struct sockaddr *)&rem_addr, &opt);

//     ba2str( &rem_addr.rc_bdaddr, buf );
//     fprintf(stderr, "accepted connection from %s\n", buf);
//     memset(buf, 0, sizeof(buf));

//     // read data from the client
//     bytes_read = read(client, buf, sizeof(buf));
//     if( bytes_read > 0 ) {
//         printf("received [%s]\n", buf);
//     }

//     // close connection
//     close(client);
//     close(s);
//     return 0;
    
// }
int main(int argc, char **argv)
{
    inquiry_info *ii = NULL;
    int max_rsp;//Nombre maximum d'appareil bluetooth renvoyés par la recherche bluetooth
    int num_rsp;
    int dev_id, sock,s, flags, status;
    struct sockaddr_rc addrB = { 0 };
    int len;//Facteur * 1.28 secondes pour effectuer des découvertes bluetooth
    int i;
    char addr[19] = { 0 };
    char name[248] = { 0 };

    //Récupère l'adresse du premier adapteur bluetooth présent sur l'ordinateur 
    dev_id = hci_get_route(NULL);
    
    
    
    //Ouvre une socket bluetooth à partir d'un adpateur 
    sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }

    len  = 8;
    max_rsp = 255;
    //
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));
    
    // Lance un scan des appareil bluetooth environnants
    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if( num_rsp < 0 ) perror("hci_inquiry");

    for (i = 0; i < num_rsp; i++) {
        bdaddr_t    bdAddr=(ii+i)->bdaddr;
        ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), name, 0) < 0){
            strcpy(name, "[unknown]");
        }
        
        if(strcmp(name,"DSD TECH HC-05")==0){

            printf("%s  %s\n", addr, name);
            uint16_t     handle;

            if(hci_create_connection(sock,&(ii+i)->bdaddr,ACL_PTYPE_MASK, &(ii+i)->clock_offset, 0, &handle, 0)<0){
                printf("Can't create connection with : %s",name);
            }


            // allocate socket
            addrB.rc_family = AF_BLUETOOTH;
            addrB.rc_channel = (uint8_t) 1;
            str2ba( addr, &addrB.rc_bdaddr );

            s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
            status = connect(s, (struct sockaddr *)&addrB, sizeof(addrB));

            // send a message
            if( status == 0 ) {
                printf("OK MESSAGE");
                status = write(s, "hello!", 6);
                
            }
            else if(status<0){
                printf("Can't connect socket to : %s",name);
                perror("");
            }
            char buf[255]={0};
            char sendString[255]={0};
            do{
                
                printf("SIZE : %d",read(s, buf,13));

                if(strlen(sendString)+strlen(buf)<=13){
                    strcat(sendString,buf);
                }
                
                memset(buf,0,strlen(buf));

                if(strlen(sendString)==13){
                    printf("STRING : %s\n",sendString);
                    memset(sendString,0,strlen(sendString));
                    write(s,1,1);
                }
                
            }while(!strpbrk(sendString,"stop"));
            

            close(s);
        }
        
    }
}


//     free( ii );
//     close( sock );
//     return 0;
// }

// struct sockaddr_rc addr = { 0 };
//     int s, status;
//     char dest[18] = "01:23:45:67:89:AB";

//     // allocate a socket
//     s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

//     // set the connection parameters (who to connect to)
//     addr.rc_family = AF_BLUETOOTH;
//     addr.rc_channel = (uint8_t) 1;
//     str2ba( dest, &addr.rc_bdaddr );

//     // connect to server
//     status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

//     // send a message
//     if( status == 0 ) {
//         status = write(s, "hello!", 6);
//     }

//     if( status < 0 ) perror("uh oh");

//     close(s);
//     return 0;