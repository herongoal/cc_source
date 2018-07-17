#include "sha_256.h"


#include <string>
#include <map>
#include <stdint.h>
#include <string.h>
#include <iostream>


using namespace std;

const signed char       char_reserved = -1;
const signed char       digit = 0;
const signed char       alpha = 0;
const signed char       captl = 0;
const signed char       chr_plus = 0;
const signed char       slash = 0;
const signed char       chr_equal = 127;


const signed char       encode_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const signed char	decode_table[] = {
	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,
	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,
	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,
	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,

	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,
	char_reserved,     char_reserved,     char_reserved,     chr_plus | 62,     char_reserved,     char_reserved,     char_reserved,     slash | 63,
	digit | 52,	   digit | 53,	      digit | 54,        digit | 55,        digit | 56,        digit | 57,        digit | 58,        digit | 59,
	digit | 60,        digit | 61,        char_reserved,     char_reserved,     char_reserved,     chr_equal,         char_reserved,     char_reserved,

	char_reserved,     alpha | 0,         alpha | 1, 	 alpha | 2, 	    alpha | 3,         alpha | 4,         alpha | 5, 	     alpha | 6,
	alpha | 7,         alpha | 8,         alpha | 9, 	 alpha | 10, 	    alpha | 11,        alpha | 12,        alpha | 13, 	     alpha | 14,
	alpha | 15,        alpha | 16,        alpha | 17, 	 alpha | 18, 	    alpha | 19,        alpha | 20,        alpha | 21, 	     alpha | 22,
	alpha | 23,        alpha | 24,        alpha | 25,        char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,

	char_reserved,     captl | 26,        captl | 27,        captl | 28,        captl | 29,        captl | 30,        captl | 31,        captl | 32,
        captl | 33,        captl | 34,        captl | 35,        captl | 36,        captl | 37,        captl | 38,        captl | 39,        captl | 40,
        captl | 41,        captl | 42,        captl | 43,        captl | 44,        captl | 45,        captl | 46,        captl | 47,        captl | 48,
        captl | 49,        captl | 50,        captl | 51,        char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,

	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,
	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,
	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,
	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,

	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,
	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,
	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,
	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,

	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,
	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,
	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,
	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,

	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,
	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,
	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,
	char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,     char_reserved,
};

int     encode_data(const void *data, size_t data_len, void *buff, size_t buff_len, bool padding)
{
	const uint32_t  u32_max = ~ 0x0u;
	const size_t    code_bit_num = 6;

	unsigned	bit_num = 0;
	unsigned	bit_buf = 0;

	size_t  remainder = data_len % 3;
	size_t  encode_length = data_len / 3 * 4  + (remainder ? (padding ? 4 : (remainder + 1)) : 0);
	
	if(buff_len < encode_length)
	{
		return	-1;
	}

	const unsigned char *indata = (const unsigned char *)data;
	size_t	pos = 0;
	for(size_t n = 0; bit_num >= code_bit_num || n < data_len; )
	{
		if(bit_num < code_bit_num)
		{
			bit_buf = (bit_buf << 8) | indata[n++];
			bit_num += 2;
		}
		else
		{
			bit_num -= 6;
		}
		((char*)buff)[pos++] = encode_table[bit_buf >> bit_num];
		bit_buf &= ~(u32_max << bit_num);
	}

	if(bit_num > 0)
	{
		bit_buf <<= 6u - bit_num;
		bit_num = 6u;
		((char*)buff)[pos++] = encode_table[bit_buf];
		bit_buf = 0u;
		bit_num = 0u;
		while(pos < encode_length)((char*)buff)[pos++] = '=';
	}

	return	encode_length;
}

int     decode_data(const void *data, size_t data_len, void *buff, size_t buff_len)
{
	const uint32_t  u32_max = ~ 0x0u;

	unsigned int    bit_num = 0u;
	unsigned int    bit_arr = 0u;

	const unsigned char *indata = (const unsigned char *)data;
	size_t pos = 0u;
	for(size_t n = 0; n < data_len; ++n)
	{
		signed char val = decode_table[indata[n]];
		if(val == chr_equal)
		{
			bit_arr = 0u;
			bit_num = 0u;
		}
		else    if(val >= 0)
		{       
			bit_arr = (bit_arr << 6) | val;
			bit_num += 6; 
			if(bit_num >= 8)
			{       
				if(pos == buff_len)
				{
					return	-1;
				}

				bit_num -= 8;
				((char *)buff)[pos++] = bit_arr >> bit_num;
				bit_arr &= ~(u32_max << bit_num);
			}
		}
		else
		{       
			cout << "pos=" << n << " " << indata[n] << endl;
			return	-1;
		}
	}

	return pos;
}
