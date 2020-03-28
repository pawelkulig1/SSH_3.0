#include <string>
#include <cstring>
#include <iostream>
#include <openssl/err.h>
#include <openssl/bn.h>

extern int b64_pton(const char *, uint8_t *, size_t);

int calculate_data_length(std::string data)
{
	int counter = 0;
	const int size = data.length();
	if (data[size - 1] == '=') counter++;
	if (data[size - 2] == '=') counter++;
	return (size * 2 * (6.0 / 8.0)) - (8 * counter);
}

extern "C" {

const char *convert_from_key_to_num(const char *data_c)
{
    std::string data(data_c);
    const int data_length = calculate_data_length(data.c_str());
    //std::cout<<data_length << std::endl;
    uint8_t out[data_length];

    b64_pton(data_c, out, data_length);
    //for (int i=0;i<data.length();i++)
    //{
    //    std::cout << std::oct << static_cast<int>(out[i]) << " ";
    //} 
    //std::cout << std::dec <<  std::endl;
    BIGNUM *b1 = BN_new();
    BN_bin2bn(reinterpret_cast<const unsigned char *>(out), data.length(), b1);
    int size = BN_num_bytes(b1);
    char *c;
    c = BN_bn2hex(b1);
    BN_clear(b1);
    //std::cout << size << " " << size * 2 << std::endl;

    //for(int i=40;i<data_length;i++)
    //{
    //    std::cout<<c[i];
    //}

    //std::cout<<std::endl;
    //std::cout<<s<<std::endl;
    char *ret = new char[data_length + 1];
    std::memcpy(ret, c + 40, data_length - 40);
    ret[data_length] = '\0';
    delete[] c;
    return ret;
}

void delete_char_arr(const char *ptr)
{
    delete[] ptr;
    ptr = nullptr;
}

}
