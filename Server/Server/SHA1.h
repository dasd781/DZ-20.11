#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#define SIZE 10
#define LOGINLENGTH 10

using namespace std;

const int hash_size = 5;

typedef unsigned int uint;

#define one_block_size_bytes 64 
#define one_block_size_uints 16 
#define block_expend_size_uints 80 

#define SHA1HASHLENGTHBYTES 20
#define SHA1HASHLENGTHUINTS 5

typedef uint* Block;
typedef uint ExpendBlock[block_expend_size_uints];

const uint H[5] = {
    0x67452301,
    0xEFCDAB89,
    0x98BADCFE,
    0x10325476,
    0xC3D2E1F0
};

uint cycle_shift_left(uint val, int bit_count);
uint bring_to_human_view(uint val);
std::vector<uint> sha1_str(const std::string& msg);
void sha1_add_salt(std::vector<uint>& input, const uint salt);
const uint sha1_pick_salt(const std::vector<uint>& input);
bool two_hashes_compare(std::vector<uint> hash1, std::vector<uint> hash2, int sha1_size = 5);