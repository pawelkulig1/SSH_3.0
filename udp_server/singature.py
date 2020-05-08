from Crypto.PublicKey import RSA

f = open("rsa.public", "r")
key = RSA.importKey(f.read())
print(key)