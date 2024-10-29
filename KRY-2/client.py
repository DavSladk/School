import socket
import os
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes

class Client():
    def __init__(self, port, verbose):
        self.port = port
        self.verbose = verbose
        self.localIP = "127.0.0.1"
        self.key = self.generateKey()

    def Run(self):
        self.printIfVerbose(f"I am client at {self.localIP} connecting to {self.port}!")
        self.makeConnection()

    def printIfVerbose(self, string):
        if self.verbose:
            print(string)

    def makeConnection(self):
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((self.localIP, self.port))
                # send public key to server
                s.sendall(self.getPublicKeyBytes())
                # get public key from server
                tmpKey = s.recv(1024)
                self.serverKey = serialization.load_pem_public_key(tmpKey)
                print(f"c: RSA_public_key_sender={self.getPublicKeyBytes().decode('utf-8')}", end='')
                print(f"c: RSA_private_key_sender={self.getPrivateKeyBytes().decode('utf-8')}", end='')
                print(f"c: RSA_public_key_receiver={self.getServerPublicKeyBytes().decode('utf-8')}", end='')
                while True:
                    try:
                        self.message = input("c: Enter input: ").encode('utf8')
                    except KeyboardInterrupt:
                        break
                    self.makeSign()
                    self.AESencrypt()
                    self.encryptAESkey()
                    self.compactDataToSend()
                    self.printMessageData()                    
                    try:
                        s.sendall(self.toSend)
                    except:
                        break

                    print(f"c: The message was successfully delivered")
        except ConnectionRefusedError:
            print(f"c: Error XYZ")
    
    def generateKey(self):
        key = rsa.generate_private_key(public_exponent=65537, key_size=2048)
        return key
    
    def makeSign(self):
        digest = hashes.Hash(hashes.MD5())
        digest.update(self.message)
        self.h = digest.finalize()

        padding = b"\x00\x01\xff\xff\xff\xff\xff\xff\xff\xff"
        rando = int.from_bytes(os.getrandom(1),'big') % 32
        for i in range(rando):
            padding = padding + b"\xff"
        padding = padding + b"\x00"
        self.hPadded = padding + self.h

        m = int.from_bytes(self.hPadded,'big')
        d = self.key.private_numbers().d
        n = self.key.public_key().public_numbers().n
        s = pow(m,d,n)
        self.sign = str(s).encode('utf8')

    def AESencrypt(self):
        toEncrypt = self.message + self.sign
        self.extraAESpadding = 16 - len(toEncrypt) % 16
        toEncrypt = toEncrypt + os.getrandom(self.extraAESpadding)

        self.AESkey = os.getrandom(32)
        self.iv = os.getrandom(16)
        cipher = Cipher(algorithms.AES(self.AESkey), modes.CBC(self.iv))
        encryptor = cipher.encryptor()
        self.ct = encryptor.update(toEncrypt) + encryptor.finalize()

    def encryptAESkey(self):
        padding = b"\x00\x02"
        rando = int.from_bytes(os.getrandom(1),'big') % 32
        for i in range(8, rando + 8):
            tmp = os.getrandom(1)
            if tmp == b"\x00":
                tmp = b"\x01"
            padding = padding + tmp
        padding = padding + b"\x00"

        self.AESkeyPadded = padding + self.AESkey

        m = int.from_bytes(self.AESkeyPadded, 'big')
        print(f"m={m}")
        e = self.serverKey.public_numbers().e
        n = self.serverKey.public_numbers().n

        c = pow(m,e,n)
        print(f"c={c}")

        self.AESkeyEncrypted = str(c).encode('utf8')

    def compactDataToSend(self):
        data = len(self.AESkeyEncrypted).to_bytes(2,'big') + len(self.sign).to_bytes(2,'big') + self.extraAESpadding.to_bytes(2,'big') + self.iv
        data = data + self.ct + self.AESkeyEncrypted
        self.toSend = data



    def printMessageData(self):
        print(f"c: AES_key={self.AESkey.hex()}")
        print(f"c: AES_key_padding={self.AESkeyPadded.hex()}")
        print(f"c: MD5={self.h.hex()}")
        print(f"c: MD5_padding={self.hPadded.hex()}")
        print(f"c: RSA_MD5_hash={self.sign.decode('utf8')}")
        print(f"c: AES_cipher={self.ct.hex()}")
        print(f"c: RSA_AES_key={self.AESkeyEncrypted.decode('utf8')}")
        print(f"c: ciphertext={self.toSend.hex()}")

    def getPublicKeyBytes(self):
        pem = self.key.public_key().public_bytes(
                encoding=serialization.Encoding.PEM,
                format=serialization.PublicFormat.SubjectPublicKeyInfo
                )
        return pem
    
    def getPrivateKeyBytes(self):
        pem = self.key.private_bytes(
                encoding=serialization.Encoding.PEM,
                format=serialization.PrivateFormat.TraditionalOpenSSL,
                encryption_algorithm=serialization.NoEncryption()
                )
        return pem
    
    def getServerPublicKeyBytes(self):
        pem = self.serverKey.public_bytes(
                encoding=serialization.Encoding.PEM,
                format=serialization.PublicFormat.SubjectPublicKeyInfo
                )
        return pem

if __name__ == "__main__":
    pass