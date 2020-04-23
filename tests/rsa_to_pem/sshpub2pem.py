#!/usr/bin/python

# usage: sshpub2pem.py id_rsa.pub > id_rsa.pub.pem

import sys, base64, struct
from pyasn1.type import univ
from pyasn1.codec.der import encoder as der_encoder

def convert_RSA_to_PEM(self, key_data):
    keydata = base64.b64decode(
        open(sys.argv[1]).readlines()[0].split()[1])

    parts = []
    while keydata:
        dlen = struct.unpack('>I', keydata[:4])[0]
        data, keydata = keydata[4:dlen+4], keydata[4+dlen:]
        parts.append(data)

    e_val = int(parts[1].hex(), 16)
    n_val = int(parts[2].hex(), 16)


    pkcs1_seq = univ.Sequence()
    pkcs1_seq.setComponentByPosition(0, univ.Integer(n_val))
    pkcs1_seq.setComponentByPosition(1, univ.Integer(e_val))
    pkcs1_val = der_encoder.encode(pkcs1_seq)

    head_seq = univ.Sequence()
    head_seq.setComponentByPosition(0, univ.ObjectIdentifier('1.2.840.113549.1.1.1'))
    head_seq.setComponentByPosition(1, univ.Null(''))

    out_seq = univ.Sequence()
    out_seq.setComponentByPosition(0, head_seq)
    out_seq.setComponentByPosition(1, univ.BitString("'%s'H" % pkcs1_val.hex()))
    out = '-----BEGIN PUBLIC KEY-----'
    out += base64.encodebytes(der_encoder.encode(out_seq)).strip().decode()
    out += '-----END PUBLIC KEY-----'
    return out
