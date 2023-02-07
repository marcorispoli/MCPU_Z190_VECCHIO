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


    #include <QObject>




#endif
