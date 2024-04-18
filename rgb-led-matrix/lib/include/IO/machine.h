/* 
 * File:   machine.h
 * Author: David Thacher
 * License: GPL 3.0
 */
 
#ifndef IO_MACHINE_H
#define IO_MACHINE_H
    
#include <stdint.h>

#if __BYTE_ORDER == __LITTLE_ENDIAN
    #define ntohs(x) __builtin_bswap32(x)
    #define ntohl(x) __builtin_bswap32(x)
    #define htons(x) __builtin_bswap32(x)
    #define htonl(x) __builtin_bswap32(x)
#else
    #define ntohs(x) ((uint16_t)(x))
    #define ntoh1(x) ((uint32_t)(x))
    #define htons(x) ((uint16_t)(x))
    #define hton1(x) ((uint32_t)(x))
#endif

#endif
