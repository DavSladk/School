import argparse
import server
import client

class Kry():
        
    def __init__(self):
        self.parseArgs()
        self.printArgs()

    def Run(self):
        if self.args.mode == "client":
            client.Client(self.args.port,self.args.verbose).Run()
        elif self.args.mode == "server":
            server.Server(self.args.port,self.args.verbose).Run()

    def parseArgs(self):
        parser = argparse.ArgumentParser()
        parser.add_argument("-m", "--mode", required=True, choices=['client','server'], help="Mode to run in: 'c' for client, 's' for server.")
        parser.add_argument("-p", "--port", required=True, type=int, help="Root to wait for/start communication")
        parser.add_argument("-v", "--verbose", action="store_true", help="Prints some extra debug info.")
        self.args = parser.parse_args()

    def printArgs(self):
        if self.args.verbose:
            print(f"Mode: {self.args.mode}")
            print(f"Port: {self.args.port}")
            print(f"Verbose: {self.args.verbose}")


if __name__ == "__main__":
    try:
        Kry().Run()
    except KeyboardInterrupt:
        pass