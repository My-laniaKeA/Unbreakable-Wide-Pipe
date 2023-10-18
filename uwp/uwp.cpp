#include "uwp.h"
#include <ctime>
#include <string>
#include<iostream>
#include <cstring>
using std::string;

UWP1::UWP1() {
    this->BUF[0] = 0x20190102;
    this->BUF[1] = 0x29202001;
    this->BUF[2] = 0x23539876;
    this->BUF[3] = 0x54321234;
}

void UWP1::hash(const string& message, bool show_res = true) {

    size_t len = message.length();
    uint8_t* ptr = (uint8_t*)message.c_str();
    int BlockNum = int((len + 8) / 64) + 1; // get the block number after padding

    // put the last block aside
    for (int i = 0; i < BlockNum - 1; i++) {
        memset(BLOCK, 0, 64);
        if (64 * (i + 1) <= len) {
            memcpy(BLOCK, ptr + 64 * i, 64);
        }
        else {
            int rest_len = len - 64 * i;
            memcpy(BLOCK, ptr + 64 * i, rest_len);
            BLOCK[rest_len] = 0x80;     // padding: first bit = 1, others = 0
        }
        transform(BLOCK);
    }

    // the last block
    memset(BLOCK, 0, 64);
    if (len % 64 == 0) BLOCK[0] = 0x80;     // in case that the padding is in the last block
    *(uint32_t*)(BLOCK + 56) = len;        // little endian
    transform(BLOCK);

    // take the first 10 bytes of BUF (total 16 bytes) as output
    memcpy(output, BUF, 10);

    // print to the terminal
    printf("Hash = 0x");
    for (int i = 0; i < sizeof(output); i++) {
        printf("%.2x", output[i]);
    }
    printf("\n");
}

uint8_t res[4][4] = { 0 };
const uint8_t cov_kernel[16][3][3] = { \
    { {215, 232, 36}, { 193, 245, 71 }, { 168, 253, 105 } },
    { {139, 255, 137}, {107, 253, 166}, {73, 246, 192} },
    { {38, 233, 214}, {2, 216, 231}, {33, 195, 244} },
    { {69, 169, 252}, {103, 141, 255}, {135, 109, 253} },
    { {164, 75, 246 }, {190, 40, 234}, {212, 4, 217} },
    { {230, 31, 196}, {244, 67, 171}, {252, 101, 143} },
    { {255, 133, 111}, {254, 163, 78}, {247, 189, 42} },
    { {235, 211, 6 }, {219, 229, 29 }, {198, 243, 64} },
    { {173, 252, 99}, {144, 255, 131}, {113, 254, 161} },
    { {80, 247, 187}, {45, 236, 210}, {9, 220, 228} },
    { {27, 199, 242}, {62, 174, 251}, {97, 146, 255} },
    { {129, 115, 254}, {159, 82, 248}, {186, 47, 237} },
    { {209, 11, 221}, {227, 24, 200}, {242, 60, 176} },
    { {251, 95, 148 }, {255, 127, 117}, {254, 157, 84} },
    { {248, 184, 49}, {238, 207, 13}, {222, 226, 22} },
    { {202, 241, 58}, {178, 250, 92}, {150, 255, 125} } };
const uint32_t consts[16] = {71, 74, 34, 229, 151, 191, 33, 109, 37, 10, 67, 41, 37, 226, 200, 78};

void UWP1::transform(uint8_t block[64]) {
    uint8_t* buf = (uint8_t*)BUF;
    for (int step_num = 0; step_num < 16; step_num++) {
        uint8_t* msg = block + 4 * step_num;
        uint8_t tab[6][6] = { \
            { *msg, *buf, *(msg+1), *(msg+2), *(buf+1), *(msg+3) },
            { *(buf+4), *(msg+3), *(buf+8), *(buf+5), *(msg), *(buf+9) },
            { *(msg+1), *(buf+12), *(msg+2), *(msg+3), *(buf+13), *(msg) },
            { *(msg+3), *(buf+2), *msg, *(msg+1), *(buf+3), *(msg+2) },
            { *(buf+6), *(msg+1), *(buf+10), *(buf+7), *(msg+2), *(buf+11) },
            { *(msg+2), *(buf+14), *(msg+3), *(msg+0), *(buf+15), *(msg+1) },
        };
        // 16 * 9 Loop. Unroll to speed up.
        // for (int row = 1; row <= 4; row++){
        //     for (int col = 1; col <= 4; col++) {
        //         res[row - 1][col - 1] = 0;
        //         for (int row_offset = -1; row_offset <= 1; row_offset++) {
        //             for (int col_offset = -1; col_offset <= 1; col_offset++) {
        //                 uint16_t sum = res[row - 1][col - 1] + (tab[row + row_offset][col + col_offset] ^ cov_kernel[step_num][row_offset + 1][col_offset + 1]);
        //                 res[row - 1][col - 1] = (uint8_t)(sum & 0xff);
        //             }
        //         }
        //     }
        // }

        res[0][0] = (tab[0][0] ^ cov_kernel[step_num][0][0] + tab[0][1] ^ cov_kernel[step_num][0][1] + tab[0][2] ^ cov_kernel[step_num][0][2] + tab[1][0] ^ cov_kernel[step_num][1][0] + tab[1][1] ^ cov_kernel[step_num][1][1] + tab[1][2] ^ cov_kernel[step_num][1][2] + tab[2][0] ^ cov_kernel[step_num][2][0] + tab[2][1] ^ cov_kernel[step_num][2][1] + tab[2][2] ^ cov_kernel[step_num][2][2])&0xff;
        res[0][1] = (tab[0][1] ^ cov_kernel[step_num][0][0] + tab[0][2] ^ cov_kernel[step_num][0][1] + tab[0][3] ^ cov_kernel[step_num][0][2] + tab[1][1] ^ cov_kernel[step_num][1][0] + tab[1][2] ^ cov_kernel[step_num][1][1] + tab[1][3] ^ cov_kernel[step_num][1][2] + tab[2][1] ^ cov_kernel[step_num][2][0] + tab[2][2] ^ cov_kernel[step_num][2][1] + tab[2][3] ^ cov_kernel[step_num][2][2])&0xff;
        res[0][2] = (tab[0][2] ^ cov_kernel[step_num][0][0] + tab[0][3] ^ cov_kernel[step_num][0][1] + tab[0][4] ^ cov_kernel[step_num][0][2] + tab[1][2] ^ cov_kernel[step_num][1][0] + tab[1][3] ^ cov_kernel[step_num][1][1] + tab[1][4] ^ cov_kernel[step_num][1][2] + tab[2][2] ^ cov_kernel[step_num][2][0] + tab[2][3] ^ cov_kernel[step_num][2][1] + tab[2][4] ^ cov_kernel[step_num][2][2])&0xff;
        res[0][3] = (tab[0][3] ^ cov_kernel[step_num][0][0] + tab[0][4] ^ cov_kernel[step_num][0][1] + tab[0][5] ^ cov_kernel[step_num][0][2] + tab[1][3] ^ cov_kernel[step_num][1][0] + tab[1][4] ^ cov_kernel[step_num][1][1] + tab[1][5] ^ cov_kernel[step_num][1][2] + tab[2][3] ^ cov_kernel[step_num][2][0] + tab[2][4] ^ cov_kernel[step_num][2][1] + tab[2][5] ^ cov_kernel[step_num][2][2])&0xff;
        res[1][0] = (tab[1][0] ^ cov_kernel[step_num][0][0] + tab[1][1] ^ cov_kernel[step_num][0][1] + tab[1][2] ^ cov_kernel[step_num][0][2] + tab[2][0] ^ cov_kernel[step_num][1][0] + tab[2][1] ^ cov_kernel[step_num][1][1] + tab[2][2] ^ cov_kernel[step_num][1][2] + tab[3][0] ^ cov_kernel[step_num][2][0] + tab[3][1] ^ cov_kernel[step_num][2][1] + tab[3][2] ^ cov_kernel[step_num][2][2])&0xff;
        res[1][1] = (tab[1][1] ^ cov_kernel[step_num][0][0] + tab[1][2] ^ cov_kernel[step_num][0][1] + tab[1][3] ^ cov_kernel[step_num][0][2] + tab[2][1] ^ cov_kernel[step_num][1][0] + tab[2][2] ^ cov_kernel[step_num][1][1] + tab[2][3] ^ cov_kernel[step_num][1][2] + tab[3][1] ^ cov_kernel[step_num][2][0] + tab[3][2] ^ cov_kernel[step_num][2][1] + tab[3][3] ^ cov_kernel[step_num][2][2])&0xff;
        res[1][2] = (tab[1][2] ^ cov_kernel[step_num][0][0] + tab[1][3] ^ cov_kernel[step_num][0][1] + tab[1][4] ^ cov_kernel[step_num][0][2] + tab[2][2] ^ cov_kernel[step_num][1][0] + tab[2][3] ^ cov_kernel[step_num][1][1] + tab[2][4] ^ cov_kernel[step_num][1][2] + tab[3][2] ^ cov_kernel[step_num][2][0] + tab[3][3] ^ cov_kernel[step_num][2][1] + tab[3][4] ^ cov_kernel[step_num][2][2])&0xff;
        res[1][3] = (tab[1][3] ^ cov_kernel[step_num][0][0] + tab[1][4] ^ cov_kernel[step_num][0][1] + tab[1][5] ^ cov_kernel[step_num][0][2] + tab[2][3] ^ cov_kernel[step_num][1][0] + tab[2][4] ^ cov_kernel[step_num][1][1] + tab[2][5] ^ cov_kernel[step_num][1][2] + tab[3][3] ^ cov_kernel[step_num][2][0] + tab[3][4] ^ cov_kernel[step_num][2][1] + tab[3][5] ^ cov_kernel[step_num][2][2])&0xff;
        res[2][0] = (tab[2][0] ^ cov_kernel[step_num][0][0] + tab[2][1] ^ cov_kernel[step_num][0][1] + tab[2][2] ^ cov_kernel[step_num][0][2] + tab[3][0] ^ cov_kernel[step_num][1][0] + tab[3][1] ^ cov_kernel[step_num][1][1] + tab[3][2] ^ cov_kernel[step_num][1][2] + tab[4][0] ^ cov_kernel[step_num][2][0] + tab[4][1] ^ cov_kernel[step_num][2][1] + tab[4][2] ^ cov_kernel[step_num][2][2])&0xff;
        res[2][1] = (tab[2][1] ^ cov_kernel[step_num][0][0] + tab[2][2] ^ cov_kernel[step_num][0][1] + tab[2][3] ^ cov_kernel[step_num][0][2] + tab[3][1] ^ cov_kernel[step_num][1][0] + tab[3][2] ^ cov_kernel[step_num][1][1] + tab[3][3] ^ cov_kernel[step_num][1][2] + tab[4][1] ^ cov_kernel[step_num][2][0] + tab[4][2] ^ cov_kernel[step_num][2][1] + tab[4][3] ^ cov_kernel[step_num][2][2])&0xff;
        res[2][2] = (tab[2][2] ^ cov_kernel[step_num][0][0] + tab[2][3] ^ cov_kernel[step_num][0][1] + tab[2][4] ^ cov_kernel[step_num][0][2] + tab[3][2] ^ cov_kernel[step_num][1][0] + tab[3][3] ^ cov_kernel[step_num][1][1] + tab[3][4] ^ cov_kernel[step_num][1][2] + tab[4][2] ^ cov_kernel[step_num][2][0] + tab[4][3] ^ cov_kernel[step_num][2][1] + tab[4][4] ^ cov_kernel[step_num][2][2])&0xff;
        res[2][3] = (tab[2][3] ^ cov_kernel[step_num][0][0] + tab[2][4] ^ cov_kernel[step_num][0][1] + tab[2][5] ^ cov_kernel[step_num][0][2] + tab[3][3] ^ cov_kernel[step_num][1][0] + tab[3][4] ^ cov_kernel[step_num][1][1] + tab[3][5] ^ cov_kernel[step_num][1][2] + tab[4][3] ^ cov_kernel[step_num][2][0] + tab[4][4] ^ cov_kernel[step_num][2][1] + tab[4][5] ^ cov_kernel[step_num][2][2])&0xff;
        res[3][0] = (tab[3][0] ^ cov_kernel[step_num][0][0] + tab[3][1] ^ cov_kernel[step_num][0][1] + tab[3][2] ^ cov_kernel[step_num][0][2] + tab[4][0] ^ cov_kernel[step_num][1][0] + tab[4][1] ^ cov_kernel[step_num][1][1] + tab[4][2] ^ cov_kernel[step_num][1][2] + tab[5][0] ^ cov_kernel[step_num][2][0] + tab[5][1] ^ cov_kernel[step_num][2][1] + tab[5][2] ^ cov_kernel[step_num][2][2])&0xff;
        res[3][1] = (tab[3][1] ^ cov_kernel[step_num][0][0] + tab[3][2] ^ cov_kernel[step_num][0][1] + tab[3][3] ^ cov_kernel[step_num][0][2] + tab[4][1] ^ cov_kernel[step_num][1][0] + tab[4][2] ^ cov_kernel[step_num][1][1] + tab[4][3] ^ cov_kernel[step_num][1][2] + tab[5][1] ^ cov_kernel[step_num][2][0] + tab[5][2] ^ cov_kernel[step_num][2][1] + tab[5][3] ^ cov_kernel[step_num][2][2])&0xff;
        res[3][2] = (tab[3][2] ^ cov_kernel[step_num][0][0] + tab[3][3] ^ cov_kernel[step_num][0][1] + tab[3][4] ^ cov_kernel[step_num][0][2] + tab[4][2] ^ cov_kernel[step_num][1][0] + tab[4][3] ^ cov_kernel[step_num][1][1] + tab[4][4] ^ cov_kernel[step_num][1][2] + tab[5][2] ^ cov_kernel[step_num][2][0] + tab[5][3] ^ cov_kernel[step_num][2][1] + tab[5][4] ^ cov_kernel[step_num][2][2])&0xff;
        res[3][3] = (tab[3][3] ^ cov_kernel[step_num][0][0] + tab[3][4] ^ cov_kernel[step_num][0][1] + tab[3][5] ^ cov_kernel[step_num][0][2] + tab[4][3] ^ cov_kernel[step_num][1][0] + tab[4][4] ^ cov_kernel[step_num][1][1] + tab[4][5] ^ cov_kernel[step_num][1][2] + tab[5][3] ^ cov_kernel[step_num][2][0] + tab[5][4] ^ cov_kernel[step_num][2][1] + tab[5][5] ^ cov_kernel[step_num][2][2])&0xff;

        BUF[0] = (uint32_t)((res[1][3] << 24) + (res[1][0] << 16) + (res[1][1] << 8) + res[1][2] + consts[step_num]) & 0xffffffff;
        BUF[1] = (res[2][2] << 24) + (res[2][3] << 16) + (res[2][0] << 8) + res[2][1];
        BUF[2] = (res[3][1] << 24) + (res[3][2] << 16) + (res[3][3] << 8) + res[3][0];
        BUF[3] = (res[0][0] << 24) + (res[0][1] << 16) + (res[0][2] << 8) + res[0][3];
    }
}

