import ctypes
import copy

def convert_key_to_hex(data):
    if isinstance(data, str):
        data = data.encode()
    if not isinstance(data, bytes):
        raise RuntimeError("Function argument must be of type bytes or string") 
    lib = ctypes.cdll.LoadLibrary("./convert_key/convert_key.so")
    
    #const char *convert_from_key_to_num(const char *data_c);
    lib.convert_from_key_to_num.restype = ctypes.c_void_p #get it as void* to disable conversion
    lib.convert_from_key_to_num.argtypes = [ctypes.c_char_p]
    
    #void delete_char_arr(const char *c);
    lib.delete_char_arr.argtypes = [ctypes.c_char_p]
    lib.delete_char_arr.restype = None
    
    ptr = lib.convert_from_key_to_num(data)
    out = ctypes.cast(ptr, ctypes.c_char_p)
    out_copy = copy.deepcopy(out.value)
    lib.delete_char_arr(out)
    return out_copy

if __name__ == "__main__":
    data = "AAAAB3NzaC1yc2EAAAADAQABAAABgQDK8pyBlvgiJ0olL/D0JjfzQblkQz5t3Pqbpn8OeXJfFEYVedvR9Oy9143v8E5RjjJNye6jNPNqGn0v/zm/9f0OLURGPlqMO4pWCiQ6E7holEgM30ZH99B+lhJELgBs3z+KakWrtzXp0A9zIhtFV/c2TaZgzBtPOX8TeVxBNfPdT4+IrbmJQDSVx4NDoveHCOAFq5PkNsW9WzM2NE/Hh9hsV4XC+Q9VhBIHOawNPzGGTcVymOuJpG6yaNAh71eWeyMjhrll2NgidsO3UInnQWd+etT8gBKcuJL3wCv5luv4pSO/kEsD4D8YwDVhG8RpOYd3rkjtB/0FITLyHr0k3migvaSGxTtl+DMo+eYbFlSiA98V6OZTIXYWc38d+r7PTy+3U07cvZTCr3eY4+BcBpxJnIs4HqpQUtIF7vrOdRqI59cB73yGsYzx43uCU+4Bj0YytR51W1646iGg7uClji9AZSTImBEKXuXmVKp8cObrcy4ZLeCafC2AIt1w7uYCPqk="
    print(convert_key_to_hex(data))
    data = "AAAAB3NzaC1yc2EAAAADAQABAAABgQC1nT6EMi1VNlMD3UqOVRuHjShJIancAYOYcKAv4rLnNUg2nUC5gX2f9ipeVSxUIi2tpKG/oysDb1y4hdv8/Kp7ftFnaGjM5oGzbdTbaUGG/SVNAVtnm+qM/TeQsjg7oGoeBtQ6QaSCUH3kKiL0FE6biEHElrR3i1Yo9/x8Mhjt4mwWWw/hurHoeOpU1YpZeKYwrDKIgLAWyefd4is3vOkQDWac7DfN18XQnyiCJkMe2MOLn0Ykk2wnFs9RXgM9QqB23EDjYROSxkvE4iqj4aL+miG54zCziM9LTMYdn0bUR7aMGoOp8WYEjUc1o6sbG79eEx1DDpxS8mYM/jnBh+CuPg0/IUKXgknii221wQunTlD33ueBX7UIfUsXvM9FSv9DrulnX6epPwy9EV6HhPdn2maLE1qLyg6BVTNAqckAHm5a5v9kq5D36+bTVYVaxbjueki70QrxGbWh/k1AlYkrsoDtUSnwPT85uEtHkVJiYiHg7Ol8iGSO7ejA8PDkJg0="
    print(convert_key_to_hex(data))
