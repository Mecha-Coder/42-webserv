#!/usr/bin/env python3

import base64

def xor_crypt_bytes(data, key):
    key_bytes = key.encode('utf-8')
    return bytes(data[i] ^ key_bytes[i % len(key_bytes)] for i in range(len(data)))

def encrypt(message, key, rounds=4):
    data = message.encode('utf-8')
    for _ in range(rounds):
        data = xor_crypt_bytes(data, key)
        data = base64.b64encode(data)
    return data.decode('ascii')

def decrypt(encrypted, key, rounds=4):
    data = encrypted.encode('ascii')
    data = base64.b64decode(data)
    for i in range(rounds):
        data = xor_crypt_bytes(data, key)
        if i < rounds - 1:
            data = base64.b64decode(data)
    return data.decode('utf-8')

def main():
    message = "Jason"
    key = "!P5t;2<h?A3K+%S:ePE#H_G^+qkmy|UEOcc9lMy\"iB[4C!uH<BjnAD+?h}8K@9"
    rounds = 4  # Increase rounds for longer output

    encrypted = encrypt(message, key, rounds)
    decrypted = decrypt(encrypted, key, rounds)

    print("Encrypted (Base64):", encrypted)
    print("Decrypted:", decrypted)

if __name__ == "__main__":
    main()