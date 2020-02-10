//
//  GmcWallet.c
//  gmc
//
//  Created by AnonymityMaster on 2020/2/9.
//  Copyright © 2020 ci6. All rights reserved.
//

#include "GmcWallet.h"

#include "secp256k1.h"
#include "secp256k1_recovery.h"

#include "libbase58.h"
#include "rmd160.h"
#include "sha1.h"
#include "../libs/sha/sha256.h"
#include "sha512.h"
#include "aes-cbc.h"
#include "LzmaLib.h"

#ifndef RMDsize
#define RMDsize 160
#endif

void rmd160(const unsigned char* message, const size_t length, unsigned char digest20[]){
    dword         MDbuf[RMDsize/32];   /* contains (A, B, C, D(, E))   */
    dword         X[16];               /* current 16-word chunk        */
    unsigned int  i;                   /* counter                      */
    dword         nbytes;              /* # of bytes not yet processed */
    
    /* initialize */
    MDinit(MDbuf);
    
    /* process message in 16-word chunks */
    for (nbytes=(dword)length; nbytes > 63; nbytes-=64) {
        for (i=0; i<16; i++) {
            X[i] = BYTES_TO_DWORD(message);
            message += 4;
        }
        compress(MDbuf, X);
    }                                    /* length mod 64 bytes left */
    
    /* finish: */
    MDfinish(MDbuf, (byte*)message, (dword)length, 0);
    
    for (i=0; i<RMDsize/8; i+=4) {
        digest20[i]   =  MDbuf[i>>2];         /* implicit cast to byte  */
        digest20[i+1] = (MDbuf[i>>2] >>  8);  /*  extracts the 8 least  */
        digest20[i+2] = (MDbuf[i>>2] >> 16);  /*  significant bits.     */
        digest20[i+3] = (MDbuf[i>>2] >> 24);
    }
    
    return;
}

static void rmd160hex(const byte* message, const dword length, byte digest40[]){
    byte digest20[20];
    rmd160(message, length, digest20);
    hex_encode(digest20, sizeof(digest20), digest40);
}

void sha1(const unsigned char* buffer, const size_t size, unsigned char digest20[]){
    Sha1Context   sha1Context;
    SHA1_HASH     sha1Hash;
    
    Sha1Initialise( &sha1Context );
    Sha1Update( &sha1Context, buffer, (uint32_t)size);
    Sha1Finalise( &sha1Context, &sha1Hash );
    
    memcpy((void*)digest20, sha1Hash.bytes, sizeof(sha1Hash.bytes));
}

void sha256(const unsigned char* buffer, const size_t size, unsigned char digest32[]){
    
    Sha256Context   sha256Context;
    SHA256_HASH     sha256Hash;
    
    Sha256Initialise( &sha256Context );
    Sha256Update( &sha256Context, buffer, (uint32_t)size);
    Sha256Finalise( &sha256Context, &sha256Hash );
    
    memcpy((void*)digest32, sha256Hash.bytes, sizeof(sha256Hash.bytes));
}

void sha512(const unsigned char* buffer, const size_t size, unsigned char digest64[]){
    Sha512Context   sha512Context;
    SHA512_HASH     sha512Hash;
    
    Sha512Initialise( &sha512Context );
    Sha512Update( &sha512Context, buffer, (uint32_t)size );
    Sha512Finalise( &sha512Context, &sha512Hash );
    
    //  TODO:这个拷贝可以省略吗
    memcpy((void*)digest64, sha512Hash.bytes, sizeof(sha512Hash.bytes));
}

static void base58_encode(const unsigned char* raw_data, const size_t raw_size, unsigned char output[], size_t* output_size){
    
    b58enc((char*)output, output_size, (const void*)raw_data, raw_size);
}

static unsigned char* base58_decode(const unsigned char* b58_string, const size_t b58_size, unsigned char output[], size_t* output_size){
    
    size_t origin_output_size = *output_size;
    
    if (!b58tobin(output, output_size, (const char*)b58_string, b58_size)){
        return 0;
    }

    return &output[origin_output_size - *output_size];
}

void hex_encode(const unsigned char* raw_data, const size_t raw_size, unsigned char hex_output[])
{
    size_t fill_pos = 0;
    for (size_t i = 0; i < raw_size; ++i) {
        sprintf((char*)&hex_output[fill_pos], "%02x", raw_data[i]);
        fill_pos += 2;
    }
}

void hex_decode(const unsigned char* hex_string, const size_t hex_size, unsigned char raw_output[])
{
    size_t i;
    char holdingBuffer[3] = {0};
    unsigned hexToNumber;
    for( i=0; i<hex_size/2; i++ )
    {
        holdingBuffer[0] = hex_string[i*2 + 0];
        holdingBuffer[1] = hex_string[i*2 + 1];
        sscanf(holdingBuffer, "%x", &hexToNumber );
        raw_output[i] = (unsigned char)hexToNumber;
    }
}
