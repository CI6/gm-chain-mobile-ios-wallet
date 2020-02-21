//
//  TestGmcWallet.c
//  gmc
//
//  Created by AnonymityMaster on 2020/2/20.
//  Copyright © 2020 ci6. All rights reserved.
//

#include "TestGmcWallet.h"

#include <assert.h>

#include "libbase58.h"
#include "rmd160.h"
#include "sha1.h"
#include "../libs/sha/sha256.h"
#include "sha512.h"
#include "aes-cbc.h"
#include "LzmaLib.h"

#include "../libs/secp256k1/secp256k1.h"
#include "../libs/secp256k1/secp256k1_recovery.h"
#include "../wallet/GmcWallet.h"

const unsigned char TEST_DATA[10] = "hello gmc";
const unsigned char* TEST_DATA2 = "{\"a\":\"123\",\"b\":456,\"c\":\"你好\"}";

void testBase58(){
    char output[14 + 1] = {0, };
    size_t output_size = sizeof(output);
    b58enc(output, &output_size, TEST_DATA, 9);
    
    printf("%s\n",output);
}

void testRmd160(){
    unsigned char output[20] = {0, };
    size_t output_size = (size_t)20;
    rmd160(TEST_DATA, output_size, output);
    
    unsigned char hex_output[20 * 2 + 1] = {0, };
    hex_encode(output,20, hex_output);
    hex_output[40] = '\0';
    
    printf("%s\n",output);
}

void testAes(){
    
    uint8_t const* _key = "39lz9ywchlyfd3kanxcsufck18jruidi";
    uint8_t const _iv[AES_CBC_IV_SIZE] = {'3','9','l','z','9','y','w','c','h','l','y','f','d','3','k','a'};
    unsigned char output[32] = {0, };
    uint32_t key_size = strlen(_key);
    int error = AesCbcEncryptWithKey(_key, key_size, _iv, TEST_DATA2, output, (uint32_t)32);
    
    unsigned char output2[33] = {0, };
    AesCbcDecryptWithKey(_key, key_size, _iv, output, output2, (uint32_t)32);
    
    unsigned char* ptrOutput2 = output2;

    printf("%s\n",output);
    printf("%s\n",ptrOutput2);
}

void testLzma(){
    
    // 压缩
    unsigned char* dstBuffer = 0;
    size_t data_size = strlen(TEST_DATA2);
    const size_t compressHeaderSize = LZMA_PROPS_SIZE + sizeof(uint64_t);
    size_t destLen = data_size + 256;
    dstBuffer = malloc(compressHeaderSize + destLen);
    
    assert(dstBuffer);
    size_t nPropSize = LZMA_PROPS_SIZE;
    int result = LzmaCompress(&dstBuffer[compressHeaderSize], &destLen,
                              TEST_DATA2, data_size,
                              &dstBuffer[0], &nPropSize, 1, 1<<24, 3, 0, 2, 32, 2);
    assert(result == SZ_OK);
    printf("%s\n",dstBuffer);
    
    
    // 解压
    uint64_t uncompressed_size = 0;
    memcpy(&uncompressed_size, &dstBuffer[LZMA_PROPS_SIZE], sizeof(uint64_t));
    unsigned char* uncompressed_buffer = malloc(uncompressed_size + 1);
    assert(uncompressed_buffer);

    uncompressed_buffer[uncompressed_size] = 0;
    //  REMARK: 解压函数不太完善，这个 destLen 设置 buffer 的大小可能导致返回 SZ_ERROR_INPUT_EOF 错误，必须设置为 原始数据长度才可以。不能设置为 uncompressed_size + 1 !!
    size_t uncompressed_buffer_size = uncompressed_size;
    size_t compressed_size = strlen(dstBuffer) - LZMA_PROPS_SIZE - sizeof(uint64_t);
    
    result = LzmaUncompress(uncompressed_buffer, &uncompressed_buffer_size,
                              &dstBuffer[LZMA_PROPS_SIZE + sizeof(uint64_t)], &compressed_size,
                              dstBuffer, LZMA_PROPS_SIZE);
    
//    assert(result == SZ_OK);

    
    
    
}

void testHex(){
    const size_t size_wallet_json = 13;
    unsigned char hex_output[size_wallet_json * 2 + 1] = {0, };
    hex_encode(TEST_DATA, size_wallet_json, hex_output);
    printf("%s\n",hex_output);
    
    unsigned char decode_output[size_wallet_json + 1] = {0, };
    hex_decode(hex_output, sizeof(hex_output), decode_output);
    printf("%s\n",decode_output);
    decode_output[size_wallet_json] = '\0';
    
    int rst = strcmp(TEST_DATA, decode_output);
    
    assert(rst == 0);
}

void testSha1(){
    unsigned char output[20 + 1] = {0, };
    sha1(TEST_DATA, 9, output);
    
    printf("%s\n",output);
    
    unsigned char hex_output[20 * 2 + 1] = {0, };
    hex_encode(output,20, hex_output);
    hex_output[40] = '\0';
    
    printf("%s\n",hex_output);
    
    int rst = strcmp(hex_output, "41517bcafe6e34f3e3c3f4d1d38024004f5eda99");
    assert(rst == 0);
    
}

void testSha256(){
    unsigned char output[32 + 1] = {0, };
    sha256(TEST_DATA, 9, output);
    
    printf("%s\n",output);
    
    unsigned char hex_output[32 * 2 + 1] = {0, };
    hex_encode(output,32, hex_output);
    hex_output[64] = '\0';
    
    printf("%s\n",hex_output);
    
    int rst = strcmp(hex_output, "940fbf7bd7c812d81da70baf9ffe26181ada52e4f306c1aedeed268784b2ae24");
    assert(rst == 0);
}

void testSha512(){
    unsigned char output[64 + 1] = {0, };
    sha512(TEST_DATA, 9, output);
    
    printf("%s\n",output);
    
    unsigned char hex_output[64 * 2 + 1] = {0, };
    hex_encode(output,64, hex_output);
    hex_output[128] = '\0';
    
    printf("%s\n",hex_output);
    
    int rst = strcmp(hex_output, "402a487a88346d39b6a743226ac0501c4fb4665553b48e5005e973289c7ae207fa2de34b0822a374cbcc7fd4009a3303bbe238799c6f5c68f8b15ba67f32d349");
    assert(rst == 0);
}

void testSecp256k1(){
    // step 1. 生成 Context
    secp256k1_context* ctx_spec256k1 = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY | SECP256K1_CONTEXT_SIGN);
    
    // step 2. 生成32位私钥.
    unsigned char private_key[32+1] = {0, };
    sha256(TEST_DATA, strlen(TEST_DATA), private_key);
    
    // step3. 生成公钥
    secp256k1_pubkey public_key = {0, };
    secp256k1_ec_pubkey_create(ctx_spec256k1, &public_key, private_key);
    
    unsigned char hex_output_private_key[32 * 2 + 1] = {0, };
    unsigned char hex_output_public_key[64 * 2 + 1] = {0, };
    hex_encode(private_key,32, hex_output_private_key);
    hex_encode(public_key.data,64, hex_output_public_key);
    
    printf("%s\n",hex_output_private_key);
    printf("%s\n",hex_output_public_key);
}

void testMain(){
    testHex();
    testSha1();
    testSha256();
    testSha512();
    testBase58();
    testRmd160();
    testLzma();
    testAes();
    testSecp256k1();
}

