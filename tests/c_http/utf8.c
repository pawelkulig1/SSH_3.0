#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include <stdlib.h>
// #include <inttypes.h>
 
// typedef struct {
// 	char mask;    /* char data will be bitwise AND with this */
// 	char lead;    /* start bytes of current char in utf-8 encoded character */
// 	uint32_t beg; /* beginning of codepoint range */
// 	uint32_t end; /* end of codepoint range */
// 	int bits_stored; /* the number of bits from the codepoint that fits in char */
// }utf_t;
 
// utf_t * utf[] = {
// 	/*             mask        lead        beg      end       bits */
// 	[0] = &(utf_t){0b00111111, 0b10000000, 0,       0,        6    },
// 	[1] = &(utf_t){0b01111111, 0b00000000, 0000,    0177,     7    },
// 	[2] = &(utf_t){0b00011111, 0b11000000, 0200,    03777,    5    },
// 	[3] = &(utf_t){0b00001111, 0b11100000, 04000,   0177777,  4    },
// 	[4] = &(utf_t){0b00000111, 0b11110000, 0200000, 04177777, 3    },
// 	      &(utf_t){0},
// };
 
// /* All lengths are in bytes */
// int codepoint_len(const uint32_t cp); /* len of associated utf-8 char */
// int utf8_len(const char ch);          /* len of utf-8 encoded char */
 
// char *to_utf8(const uint32_t cp);
// uint32_t to_cp(const char chr[4]);
 
// int codepoint_len(const uint32_t cp)
// {
// 	int len = 0;
// 	for(utf_t **u = utf; *u; ++u) {
// 		if((cp >= (*u)->beg) && (cp <= (*u)->end)) {
// 			break;
// 		}
// 		++len;
// 	}
// 	if(len > 4) /* Out of bounds */
// 		exit(1);
 
// 	return len;
// }
 
// int utf8_len(const char ch)
// {
// 	int len = 0;
// 	for(utf_t **u = utf; *u; ++u) {
// 		if((ch & ~(*u)->mask) == (*u)->lead) {
// 			break;
// 		}
// 		++len;
// 	}
// 	if(len > 4) { /* Malformed leading byte */
// 		exit(1);
// 	}
// 	return len;
// }
 
// char *to_utf8(const uint32_t cp)
// {
// 	static char ret[5];
// 	const int bytes = codepoint_len(cp);
 
// 	int shift = utf[0]->bits_stored * (bytes - 1);
// 	ret[0] = (cp >> shift & utf[bytes]->mask) | utf[bytes]->lead;
// 	shift -= utf[0]->bits_stored;
// 	for(int i = 1; i < bytes; ++i) {
// 		ret[i] = (cp >> shift & utf[0]->mask) | utf[0]->lead;
// 		shift -= utf[0]->bits_stored;
// 	}
// 	ret[bytes] = '\0';
// 	return ret;
// }
 
// uint32_t to_cp(const char chr[4])
// {
// 	int bytes = utf8_len(*chr);
// 	int shift = utf[0]->bits_stored * (bytes - 1);
// 	uint32_t codep = (*chr++ & utf[bytes]->mask) << shift;
 
// 	for(int i = 1; i < bytes; ++i, ++chr) {
// 		shift -= utf[0]->bits_stored;
// 		codep |= ((char)*chr & utf[0]->mask) << shift;
// 	}
 
// 	return codep;
// }
 
// int main(void)
// {
// 	const uint32_t *in, input[] = {0x0041, 0x00f6, 0x0416, 0x20ac, 0x1d11e, 0x0};
 
// 	printf("Character  Unicode  UTF-8 encoding (hex)\n");
// 	printf("----------------------------------------\n");
 
// 	char *utf8;
// 	uint32_t codepoint;
// 	for(in = input; *in; ++in) {
// 		utf8 = to_utf8(*in);
// 		codepoint = to_cp(utf8);
// 		printf("%s          U+%-7.4x", utf8, codepoint);
 
// 		for(int i = 0; utf8[i] && i < 4; ++i) {
// 			printf("%hhx ", utf8[i]);
// 		}
// 		printf("\n");
// 	}
// 	return 0;
// }
 
int main() {
    unsigned char *in_data = "qJay53TbY3LeXiciPwJbQujpK4OQbn8LokRgznAg2mISnPL/lBY7fJSPxreUE7yz\n"\
                        "cY0DGQ5haPzUqW4ngHqRZMLSqf0Ur2/ChrIo03ELZOO+5eoWjt7L5ajxPNLm0V8E\n"\
                        "VQShaLktnnRavmmj+yz9LBQrY3/+uwdY51IZjfSHefVoZgfeezRgzWbvYfeVa6e5\n"\
                        "bnLC8xypEnQX/6qmfmloSVSZzTYxIHpqCzdtbFjHTS+hThiabp9QLGRFzjCZTyHd\n"\
                        "JyttX/mQh5L7mVLYJUaoVIflk2AaWBKbpjwBl+dU/i6OO0/73ZM02GKUJ1CZufzk\n"\
                        "FPldptmrGDGW58sRWqYYmDyCBobNh2KqK8eAGg7D9u6nO7RaylE1BP6QwcITSE+3\n"\
                        "9QaoPVdai/mtLCjcHRXyYmF8rktrVB5K/2hGjbnXm5KsOtNj9jQRK/EsbNXNrMks\n"\
                        "8MF5+wa4sgtzI3chXHd3ISH/o8CQ/sRxIEmkpUwmJkSQjOKRInOt6rPsrPbnAKzf";

    unsigned char *in, *out;
    in = in_data;
    unsigned char* out_data = malloc(strlen(in_data) * 2);
    out = out_data;

    while (*in)
        if (*in<128) *out++=*in++;
        else *out++=0xc2+(*in>0xbf), *out++=(*in++&0x3f)+0x80;

    //printf("%d", strlen(in) * 2 * sizeof(unsigned char));
    printf("test123\n\n");
    for(int i=0;i < strlen(in_data) * 2 * sizeof(unsigned char);i++)
    {
        printf("%c", out_data[i]);
    }
    printf("\n");
    
}
