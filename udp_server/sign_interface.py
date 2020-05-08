import ctypes
import random

def sign(priv_key, message):
    lib = ctypes.cdll.LoadLibrary("./verify/sign.so")
    print(lib.__dict__)
    #char* sign_message(const char* privateKey, const char* plainText);
    lib.sign_message.restype = ctypes.c_char_p
    lib.sign_message.argtypes = [ctypes.c_char_p, ctypes.c_char_p]

    signed = lib.sign_message(ctypes.create_string_buffer(priv_key.encode()), \
                                    ctypes.create_string_buffer(message.encode()))
    # print(signed)
    return signed.decode()
    

print(__file__)
if __name__ == "__main__":
    priv_key = "-----BEGIN RSA PRIVATE KEY-----\n" \
"MIICXgIBAAKBgQD6TlnopWu9pyx/G8x+fKI54yiR5wUdtjkhtXrYBpKtCHcaQFH3\n" \
"/vkVJ1Lzpk3QGwU7Wf4vBOGkK+jWNZZYAQ/n0EVaREO64kBKMYyFvXnJS1TIzRbM\n" \
"ZBMmYLGbc2j9Z3NILdI9tLrz+iWdrMnWmBaquO1s5wQMzTpynS9Q/nj1uwIDAQAB\n" \
"AoGAQM5YaOurYYcO9jn0UCtktSgjMONnZ7HuSNP74kKRZ0txytIsZtEVHGz6BSth\n" \
"qifUyvEZ5oKVCTR1LEDjlBVyUEd+9Io7MqDvckghKn6hA36o4NSFCjOXYnQXbfUA\n" \
"KmPuHu7P5BYWDlp6vHuQ40ujkzy4A3zWXIxJ/DEGGz41P8ECQQD+1HBFsR7hI+tk\n" \
"7Rc/KmRaHGgKMkxq+Z3BUueAC1ZeCn59ru5i6NwvqOyCNE7q4VD587llzsSFIrIM\n" \
"Bb0nlIGlAkEA+3SYQ52j6HUI3m8c56BZuOi5olYYhoia6olHQXxRpEHCXyrevTWa\n" \
"BAshj8S9cIOzTKAj6/9uJAa6AQO8Fl873wJBAOG8wIQMBeW80v5BxJEQ9XqZpVQ7\n" \
"AFTXMHccALT91hFSTID7oHMAAmU5x+EDg1EwLXJuA7L6+j1R4dUilFBmjn0CQQDH\n" \
"15kvm7hphaH/zcTmHR1RhLwL1Wr2eNjN7z5PQDNOXOjSY6rUKyZ1cjdBIHYCVtkH\n" \
"2/nQlJd/UE1BgytXiGZ3AkEAz/B5WHb/hLaBFdIq0xI/YpltDJpAfOapShk77QYO\n" \
"4FdktfvnapfL+Gytl4Db1PlZsqOWgEEp7o+TbCv0kQHqoQ==\n" \
"-----END RSA PRIVATE KEY-----\n"

    message = "1"
    print(sign(priv_key, message))