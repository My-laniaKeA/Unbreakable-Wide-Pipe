#include "../uwp/uwp.h"
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

using std::string;
using std::ifstream;
using std::ofstream;

#define STR_SIZE 1024 * 1024 * 200  // string size = 200 MB

uint8_t c_str[STR_SIZE];
void output(uint8_t* buf);

void read_input(int cnt){
	ifstream fIn("./sts-2.1.2/data/data copy.e");
	char* buffer;
	int char_idx = 0;
	int offset = 0;
	int length;
	if (fIn.is_open()){
		// get length of file:
		fIn.seekg (0, fIn.end);
		length = fIn.tellg();
		fIn.seekg (0, fIn.beg);
		// allocate memory:
		buffer = new char [length];
		// read data as a block:
		fIn.read (buffer,length);
	}
	for (int i = 0; i < length && cnt >= 0; i ++){
		if(buffer[i] == '1'){
			c_str[char_idx] |= (0x1 << offset);	
			cnt--;	
		}else if( buffer[i] == '0'){
			c_str[char_idx] &= ~(0x1 << offset);
			cnt--;
		}
		else{
			continue;
		}

		offset++;
		if(offset == 8){
			offset = 0;
			// printf("%x!\n",c_str[char_idx]);
			char_idx++;
		}

		if(char_idx >= 10){
			c_str[10] = int('\n');
			string str = (char*)(c_str);
			printf("input: \n");
			for(int i = 0; i < char_idx; i ++){
				printf("%.2x ",c_str[i]);
			}
			printf("\n");
			UWP1 uwp;
			uwp.hash(str, true);
			output(uwp.getOutput());
			char_idx = 0;
		}
	}
	fIn.close();
	// for(int i = 0; i < char_idx; i ++){
	// 	printf("%x ",c_str[i]);
	// }
}

void output(uint8_t* BUF){
	printf("output: \n");
	for(int i = 0; i < 10; i ++){
		printf("%.2x ", BUF[i]);
	}
	printf("\n");
	ofstream fOut;
	string s= "";
	fOut.open("output.txt", std::ios::app);
	for(int i = 0; i < 10; i ++){
		for(int offset = 7; offset >= 0; offset--){
			if ( (BUF[i] >> offset) & 0x1 ){
				s += "1";
			}else{
				s += "0";
			}
		}
	}
	s += "\n";
	fOut << s;
	fOut.close();
}

int main() {
	int cnt = 200;
	read_input(cnt);
	char c_str_1[10] = {(char)0xce, (char)0xd6, (char)0xc3, (char)0x82, (char)0x37,
	(char)0x30, (char)0xe7, (char)0xe9, (char)0x1e, (char)0x66};
	UWP1 uwp;
	string str = c_str_1;
	uwp.hash(str, true);
	output(uwp.getOutput());
	return 0;
}