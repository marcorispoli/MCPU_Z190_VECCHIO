#ifndef TYPEDEF_H
#define TYPEDEF_H

    typedef unsigned char byte;
    typedef unsigned short word;
    typedef unsigned int  dword;
    typedef unsigned long  udword;
    typedef unsigned char uint8_t;
    typedef short int16_t;
    typedef int int32_t;




    #ifndef _HTONS_IMPLEMENTATION
        extern unsigned short htons(unsigned short val);
        extern unsigned int htonl(unsigned int val);
    #endif

    #define MAX_NUM_PROCEDURE       4


    #define SH_NODE_ID              1
    #define APPLICATION_NODE_ID 	3
    #define GENERATOR_NODE_ID       6
    #define IP_ADDRESS  "127.0.0.1"
    #define HUB_PORT    10000


#endif
