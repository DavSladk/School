import socket
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes

class Server():
    def __init__(self, port, verbose):
        self.port = port
        self.verbose = verbose
        self.localIP = "127.0.0.1"
        self.key = self.generateKey()

    def Run(self):
        self.printIfVerbose(f"I am server at {self.localIP}:{self.port}!")
        self.makeConnection()

    def printIfVerbose(self, string):
        if self.verbose:
            print(string)

    def makeConnection(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.bind((self.localIP, self.port))
            s.listen()
            conn, addr = s.accept()
            with conn:
                print(f"s: Client has joined")
                # send ppublic key to client
                conn.sendall(self.getPublicKeyBytes())
                # get public key from client
                tmpKey = conn.recv(1024)
                self.clientKey = serialization.load_pem_public_key(tmpKey)
                print(f"c: RSA_public_key_sender={self.getPublicKeyBytes().decode('utf-8')}", end='')
                print(f"c: RSA_private_key_sender={self.getPrivateKeyBytes().decode('utf-8')}", end='')
                print(f"c: RSA_public_key_receiver={self.getClientPublicKeyBytes().decode('utf-8')}", end='')

                while True:
                    self.recvData = conn.recv(2048)
                    if not self.recvData:
                        break
                    
                    self.cutRecvData()
                    self.decryptAESkey()
                    self.AESdecrypt()
                    self.printMessageData()
                    self.verify()

    def cutRecvData(self):
        self.recvAESkeyLen = int.from_bytes(self.recvData[0:2],'big')
        self.recvSignLen = int.from_bytes(self.recvData[2:4],'big')
        self.recvMsgAESpaddingLen = int.from_bytes(self.recvData[4:6],'big')
        self.iv = self.recvData[6:22]

        self.AESkeyEncrypted = self.recvData[-self.recvAESkeyLen:]
        self.ct = self.recvData[22:-self.recvAESkeyLen]

    def decryptAESkey(self):
        c = self.AESkeyEncrypted.decode('utf8')
        c = int(c)
        n = self.key.public_key().public_numbers().n
        d = self.key.private_numbers().d
        m = pow(c, d, n)
        tmp = m.to_bytes((m.bit_length() + 7) // 8,'big')
        i = tmp.find(b"\x00")
        self.AESkey = tmp[i+1:]

    def AESdecrypt(self):
        cipher = Cipher(algorithms.AES(self.AESkey), modes.CBC(self.iv))
        decryptor = cipher.decryptor()
        self.decryptedTextHash = decryptor.update(self.ct) + decryptor.finalize()
        self.decryptedTextHash = self.decryptedTextHash[:-self.recvMsgAESpaddingLen]
        self.originalMessage = self.decryptedTextHash[:-self.recvSignLen]
        self.sign = self.decryptedTextHash[-self.recvSignLen:]

        digest = hashes.Hash(hashes.MD5())
        digest.update(self.originalMessage)
        self.h = digest.finalize()

    def verify(self):
        s = int(self.sign.decode('utf8'))
        e = self.clientKey.public_numbers().e
        n = self.clientKey.public_numbers().n

        m = pow(s,e,n)
        tmp = m.to_bytes((m.bit_length() + 7) // 8,'big')
        i = tmp.find(b"\x00")
        tmp = tmp[i+1:]
        if tmp == self.h:
            print("s: The integrity of the message has not been compromised.")
        else:
            print("s: The integrity of the report has been compromised.")


    def printMessageData(self):
        print(f"s: ciphertext={self.recvData.hex()}")
        print(f"s: RSA_AES_key={self.AESkeyEncrypted.decode('utf8')}")
        print(f"s: AES_cipher={self.ct.hex()}")
        print(f"s: AES_key={self.AESkey.hex()}")
        print(f"s: text_hash={self.decryptedTextHash.hex()}")
        print(f"s: plaintext={self.originalMessage.decode('utf8')}")
        print(f"s: MD5={self.h.hex()}")

    def generateKey(self):
        key = rsa.generate_private_key(public_exponent=65537, key_size=2048)
        return key

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
    
    def getClientPublicKeyBytes(self):
        pem = self.clientKey.public_bytes(
                encoding=serialization.Encoding.PEM,
                format=serialization.PublicFormat.SubjectPublicKeyInfo
                )
        return pem

if __name__ == "__main__":
    pass