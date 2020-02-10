//
//  GmcWallet.h
//  gmc
//
//  Created by AnonymityMaster on 2020/2/9.
//  Copyright © 2020 ci6. All rights reserved.
//

#ifndef GmcWallet_h
#define GmcWallet_h

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>

extern void rmd160(const unsigned char* message, const size_t length, unsigned char digest20[]);
extern void sha1(const unsigned char* buffer, const size_t size, unsigned char digest20[]);
extern void sha256(const unsigned char* buffer, const size_t size, unsigned char digest32[]);
extern void sha512(const unsigned char* buffer, const size_t size, unsigned char digest64[]);
extern void hex_encode(const unsigned char* raw_data, const size_t raw_size, unsigned char hex_output[]);
extern void hex_decode(const unsigned char* hex_string, const size_t hex_size, unsigned char raw_output[]);


#ifdef __cplusplus
}
#endif

#endif
