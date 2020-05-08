import ctypes

def verify_signature(public_key, message, signature):
    lib = ctypes.cdll.LoadLibrary("./verify/verify.so")
    #print(lib.__dict__)
    #int verify_signature(const char* publicKey, const char* plainText, char* signatureBase64);
    lib.verify_signature.restype = ctypes.c_int
    lib.verify_signature.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p]
    
    # buf1 = ctypes.create_string_buffer(len(public_key))
    # buf2 = ctypes.create_string_buffer(len(message))
    # buf3 = ctypes.create_string_buffer(len(signature))

    verified = lib.verify_signature(ctypes.create_string_buffer(public_key.encode()), \
                                    ctypes.create_string_buffer(message.encode()), \
                                    ctypes.create_string_buffer(signature.encode()))
    # print(verified)
    return verified
    

print(__file__)
if __name__ == "__main__":
    pub_key = "-----BEGIN PUBLIC KEY-----\n"\
                "MIIBojANBgkqhkiG9w0BAQEFAAOCAY8AMIIBigKCAYEArzDGSrlZWqmTFN+RKdZg3S/gHkdLKXny\n" \
                "6TrZheEHUAxDlr8kNcbCzGXgV1WPCMFbBM9DSzdq6jqljXY6vEbFJ1gMN524Il6n0U0mZdKQBKFc\n" \
                "IVFwJxXRztQyNUjQO3hRRTDf+ZDlWDqy8KjXivVk9x4t3h5TVV52fOlQ1tWsv4HdgwRzumrHlpIk\n" \
                "8pacvTyj71+NE17csy4IdQrdoIzYDtJU++8OHwkzO69mvFPkTag1ZjQXMebj9bDn3M/zeQfVFyad\n" \
                "I2ZuN4/VwHJTQIPThv4f81HlsF+yn71+RSfOPgnQ8CraUG5h5FSZg096NwqmIGJPFBuCQDYYSJu8\n" \
                "WhgZOlGTFzG9iqvm0JYKa0qQdUVr5CucDGMwXcCxCxhFIacwaQcxfAQCX6MBiMsolAqRb2re2erY\n" \
                "FoavVo93y2UgIZA/5MXTSpZounRiqN6LWtEgYHaLlA7dPmbRGoa+/iIRtS+E7LJHHEnLlgPJ+ym5\n" \
                "vx4acmd6N98ps1BuGRR4JFS7AgMBAAE=\n"\
                "-----END PUBLIC KEY-----\n"
    
    signature = "MxuYEr5Rus8h8VjiYSYaJbNIC4tS66V/3V9O+YHesIn1jx+Y7bmvof9QwP0ZBKSp\n"\
              "HEW7cfQI0mPz74fbHLBlBiJwVv5jEfr/gMAUn4WGUlmv/nAaVcXlK6xDKCdqNCwx\n"\
              "A+JoMNU4CsAV97EcqdSmgmb3UrWGZSLcQr+hcMbtrRG13G0K5RqdE+GvBqYBVebJ\n"\
              "hDjjcqem2kzlQkdG6mHRhhndH/V4bFKsuMxSKbY9zChiTvRPPbplhJYKmZRqYDZl\n"\
              "rAtklmlB4zU6K/MfCLosNYrr+HnGWgTo7/4nuGDsfeL/fWe9FBYlepDhVIQiyoBN\n"\
              "1Ty/qDGqHrQJKvT7tdVbqjIhfy79E8ceLcZw7iHeUmURPPo2UrgyNL4B4dpEA1uO\n"\
              "evV9hHAquZzpqjVfAk5yWwqgnoYFbreWli3LiGKO0cRFdUokioOI/O3383B8dBbj\n"\
              "bJhg7at3ZjbMfGptIqY4lBlSg+TTd1Vshp48fByDD3DSG7nAfQ8hWyZxtXJtDEKM"

    message = "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQCvMMZKuVlaqZMU35Ep1mDdL+AeR0spefLpOtmF4QdQDEOWvyQ1xsLMZeBXVY8IwVsEz0NLN2rqOqWNdjq8RsUnWAw3nbgiXqfRTSZl0pAEoVwhUXAnFdHO1DI1SNA7eFFFMN/5kOVYOrLwqNeK9WT3Hi3eHlNVXnZ86VDW1ay/gd2DBHO6aseWkiTylpy9PKPvX40TXtyzLgh1Ct2gjNgO0lT77w4fCTM7r2a8U+RNqDVmNBcx5uP1sOfcz/N5B9UXJp0jZm43j9XAclNAg9OG/h/zUeWwX7KfvX5FJ84+CdDwKtpQbmHkVJmDT3o3CqYgYk8UG4JANhhIm7xaGBk6UZMXMb2Kq+bQlgprSpB1RWvkK5wMYzBdwLELGEUhpzBpBzF8BAJfowGIyyiUCpFvat7Z6tgWhq9W"

    print(verify_signature(pub_key, message, signature))