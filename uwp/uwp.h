#ifndef UWP_H
#define UWP_H

#include <cstdint>
#include <string>
#include <cstring>

using std::string;

#define MAX_BLOCK_NUM 1024

class UWP1 {
private:
    const uint32_t consts[16] = {71, 74, 34, 229, 151, 191, 33, 109, 37, 10, 67, 41, 37, 226, 200, 78};

    const uint8_t cov_kernel_8[16][3][3] = { \
        { { 215, 232, 36  }, { 193, 245, 71  }, { 168, 253, 105 } },
        { { 139, 255, 137 }, { 107, 253, 166 }, { 73,  246, 192 } },
        { { 38,  233, 214 }, { 2,   216, 231 }, { 33,  195, 244 } },
        { { 69,  169, 252 }, { 103, 141, 255 }, { 135, 109, 253 } },
        { { 164, 75,  246 }, { 190, 40,  234 }, { 212, 4,   217 } },
        { { 230, 31,  196 }, { 244, 67,  171 }, { 252, 101, 143 } },
        { { 255, 133, 111 }, { 254, 163, 78  }, { 247, 189, 42  } },
        { { 235, 211, 6   }, { 219, 229, 29  }, { 198, 243, 64  } },
        { { 173, 252, 99  }, { 144, 255, 131 }, { 113, 254, 161 } },
        { { 80,  247, 187 }, { 45,  236, 210 }, { 9,   220, 228 } },
        { { 27,  199, 242 }, { 62,  174, 251 }, { 97,  146, 255 } },
        { { 129, 115, 254 }, { 159, 82,  248 }, { 186, 47,  237 } },
        { { 209, 11,  221 }, { 227, 24,  200 }, { 242, 60,  176 } },
        { { 251, 95,  148 }, { 255, 127, 117 }, { 254, 157, 84  } },
        { { 248, 184, 49  }, { 238, 207, 13  }, { 222, 226, 22  } },
        {  {202, 241, 58  }, { 178, 250, 92  }, { 150, 255, 125 } }
    };

    const uint64_t conv_kernel_64[16][3][3] = { \
        { { 0xd7d7d7d7d7d7d7d7, 0xe8e8e8e8e8e8e8e8, 0x2424242424242424 }, { 0xc1c1c1c1c1c1c1c1, 0xf5f5f5f5f5f5f5f5, 0x4747474747474747 }, { 0xa8a8a8a8a8a8a8a8, 0xfdfdfdfdfdfdfdfd, 0x6969696969696969 } },
        { { 0x8b8b8b8b8b8b8b8b, 0xffffffffffffffff, 0x8989898989898989 }, { 0x6b6b6b6b6b6b6b6b, 0xfdfdfdfdfdfdfdfd, 0xa6a6a6a6a6a6a6a6 }, { 0x4949494949494949, 0xf6f6f6f6f6f6f6f6, 0xc0c0c0c0c0c0c0c0 } },
        { { 0x2626262626262626, 0xe9e9e9e9e9e9e9e9, 0xd6d6d6d6d6d6d6d6 }, { 0x0202020202020202, 0xd8d8d8d8d8d8d8d8, 0xe7e7e7e7e7e7e7e7 }, { 0x2121212121212121, 0xc3c3c3c3c3c3c3c3, 0xf4f4f4f4f4f4f4f4 } },
        { { 0x4545454545454545, 0xa9a9a9a9a9a9a9a9, 0xfcfcfcfcfcfcfcfc }, { 0x6767676767676767, 0x8d8d8d8d8d8d8d8d, 0xffffffffffffffff }, { 0x8787878787878787, 0x6d6d6d6d6d6d6d6d, 0xfdfdfdfdfdfdfdfd } },
        { { 0xa4a4a4a4a4a4a4a4, 0x4b4b4b4b4b4b4b4b, 0xf6f6f6f6f6f6f6f6 }, { 0xbebebebebebebebe, 0x2828282828282828, 0xeaeaeaeaeaeaeaea }, { 0xd4d4d4d4d4d4d4d4, 0x0404040404040404, 0xd9d9d9d9d9d9d9d9 } },
        { { 0xe6e6e6e6e6e6e6e6, 0x1f1f1f1f1f1f1f1f, 0xc4c4c4c4c4c4c4c4 }, { 0xf4f4f4f4f4f4f4f4, 0x4343434343434343, 0xabababababababab }, { 0xfcfcfcfcfcfcfcfc, 0x6565656565656565, 0x8f8f8f8f8f8f8f8f } },
        { { 0xffffffffffffffff, 0x8585858585858585, 0x6f6f6f6f6f6f6f6f }, { 0xfefefefefefefefe, 0xa3a3a3a3a3a3a3a3, 0x4e4e4e4e4e4e4e4e }, { 0xf7f7f7f7f7f7f7f7, 0xbdbdbdbdbdbdbdbd, 0x2a2a2a2a2a2a2a2a } },
        { { 0xebebebebebebebeb, 0xd3d3d3d3d3d3d3d3, 0x0606060606060606 }, { 0xdbdbdbdbdbdbdbdb, 0xe5e5e5e5e5e5e5e5, 0x1d1d1d1d1d1d1d1d }, { 0xc6c6c6c6c6c6c6c6, 0xf3f3f3f3f3f3f3f3, 0x4040404040404040 } },
        { { 0xadadadadadadadad, 0xfcfcfcfcfcfcfcfc, 0x6363636363636363 }, { 0x9090909090909090, 0xffffffffffffffff, 0x8383838383838383 }, { 0x7171717171717171, 0xfefefefefefefefe, 0xa1a1a1a1a1a1a1a1 } },
        { { 0x5050505050505050, 0xf7f7f7f7f7f7f7f7, 0xbbbbbbbbbbbbbbbb }, { 0x2d2d2d2d2d2d2d2d, 0xecececececececec, 0xd2d2d2d2d2d2d2d2 }, { 0x0909090909090909, 0xdcdcdcdcdcdcdcdc, 0xe4e4e4e4e4e4e4e4 } },
        { { 0x1b1b1b1b1b1b1b1b, 0xc7c7c7c7c7c7c7c7, 0xf2f2f2f2f2f2f2f2 }, { 0x3e3e3e3e3e3e3e3e, 0xaeaeaeaeaeaeaeae, 0xfbfbfbfbfbfbfbfb }, { 0x6161616161616161, 0x9292929292929292, 0xffffffffffffffff } },
        { { 0x8181818181818181, 0x7373737373737373, 0xfefefefefefefefe }, { 0x9f9f9f9f9f9f9f9f, 0x5252525252525252, 0xf8f8f8f8f8f8f8f8 }, { 0xbabababababababa, 0x2f2f2f2f2f2f2f2f, 0xedededededededed } },
        { { 0xd1d1d1d1d1d1d1d1, 0x0b0b0b0b0b0b0b0b, 0xdddddddddddddddd }, { 0xe3e3e3e3e3e3e3e3, 0x1818181818181818, 0xc8c8c8c8c8c8c8c8 }, { 0xf2f2f2f2f2f2f2f2, 0x3c3c3c3c3c3c3c3c, 0xb0b0b0b0b0b0b0b0 } },
        { { 0xfbfbfbfbfbfbfbfb, 0x5f5f5f5f5f5f5f5f, 0x9494949494949494 }, { 0xffffffffffffffff, 0x7f7f7f7f7f7f7f7f, 0x7575757575757575 }, { 0xfefefefefefefefe, 0x9d9d9d9d9d9d9d9d, 0x5454545454545454 } },
        { { 0xf8f8f8f8f8f8f8f8, 0xb8b8b8b8b8b8b8b8, 0x3131313131313131 }, { 0xeeeeeeeeeeeeeeee, 0xcfcfcfcfcfcfcfcf, 0x0d0d0d0d0d0d0d0d }, { 0xdededededededede, 0xe2e2e2e2e2e2e2e2, 0x1616161616161616 } },
        { { 0xcacacacacacacaca, 0xf1f1f1f1f1f1f1f1, 0x3a3a3a3a3a3a3a3a }, { 0xb2b2b2b2b2b2b2b2, 0xfafafafafafafafa, 0x5c5c5c5c5c5c5c5c }, { 0x9696969696969696, 0xffffffffffffffff, 0x7d7d7d7d7d7d7d7d } }
    };

    uint8_t res[4][4] = { 0 };

    // 4 buffers (A, B, C, D) with length = 32b, referring to MD5.
    // NOTE that there are no initial keys (IV1, IV2) any more.
    uint32_t BUF[4];
    // message block, length = 512b
    uint8_t BLOCK[64] = {0};
    // output, length = 80b
    uint8_t output[10] = {0};
    void init();    // TODO: initialize the buffers based on your own algorithm.
    void transform(uint8_t block[64]);    // TODO: compression function, one message block as input.
public:
    UWP1(); 
    void hash(const string& message, bool print_result);
	uint8_t* getOutput(){
		return &(output[0]);
	}
};

#endif
