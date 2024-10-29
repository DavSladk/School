#!/usr/bin/env python3
import sys
import requests
from bs4 import BeautifulSoup

class HousePlantsDataGetter:

    def __init__(self, fd = sys.stdin):
        self.fd = fd

    def getDataFrom(self, fromMe):
        link = fromMe.strip()
        page = requests.get(link).content
        soup = BeautifulSoup(page, 'html.parser')
        name = soup.find('h1', class_='product-title').get_text().strip()
        price = soup.find('div', class_='price--main').findChildren()[-1].get_text().strip()[1:]

        print(link + "\t" + name + "\t" + price, flush = True)

    def getData(self):
        for line in self.fd.readlines():
            self.getDataFrom(line)

    def printHelp(self):
        print("Description:")
        print("    Gets data from link for products at https://houseplantshop.com")
        print("")
        print("Usage:")
        print("    python3 data.py [-h|--help]")
        print("")
        print("Options:")
        print("    -h|--help")
        print("        Prints out this help.")

if __name__ == "__main__":
    arguments = sys.argv[1:]

    if len(arguments) == 0:
        HousePlantsDataGetter().getData()

    elif arguments[0] == ("-h", "--help"):
        HousePlantsDataGetter().printHelp()
    else:
        HousePlantsDataGetter().printHelp()
