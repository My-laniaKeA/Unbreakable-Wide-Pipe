#include "../uwp/uwp.h"
#include <ctime>
#include <string>
#include <cstring>

using std::string;

#define STR_SIZE 1024 * 1024 * 200  // string size = 200 MB

char c_str[STR_SIZE];

int main() {
    memset(c_str, 0x41, sizeof(c_str));     // string = "AAA...AAA"
    string str = c_str;
    UWP1 uwp;
    clock_t start, end;                     // ms
    float interval;
    start = clock();
    uwp.hash(str);
    end = clock();
    interval = float(start - end) / 1000;
    printf("[*] Test Frequency: %.2f MB/s", (STR_SIZE * 1.0 / 1024 / 1024 / interval));
}