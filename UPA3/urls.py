#!/usr/bin/env python3
import sys
import requests
from bs4 import BeautifulSoup

class HousePlantsUrlGetter:
    allHousePlantsUrl = "https://houseplantshop.com/collections/all-plants"
    allAccessoriesUrl = "https://houseplantshop.com/collections/garden-shop"
    baseUrl = "https://houseplantshop.com"
    
    urlCount = 0
    GET_ALL = -1
    maxUrlCount = GET_ALL

    def __init__(self, n = GET_ALL):
        self.maxUrlCount = int(n)
    
    def needMore(self):
        return self.maxUrlCount == self.GET_ALL or self.urlCount < self.maxUrlCount
    
    def getNextPage(self, page):
        return requests.get(page).content

    def getAllUrlsFrom(self, fromMe):
        url = fromMe
        while self.needMore():
            page = self.getNextPage(url)
            soup = BeautifulSoup(page, 'html.parser')
            links = soup.find_all('a', class_='productitem--image-link')

            for link in links:
                if self.needMore():
                    print(self.baseUrl + link.get('href'))
                    self.urlCount = self.urlCount + 1
                else:
                    return
            
            nextPage = soup.find('li', class_='pagination--next')
            if nextPage:
                url = self.baseUrl + nextPage.find('a').get('href')
            else:
                return
    
    def getUrls(self):
        self.getAllUrlsFrom(self.allHousePlantsUrl)
        self.getAllUrlsFrom(self.allAccessoriesUrl)

    def printHelp(self):
        print("Description:")
        print("    Gets links to products at https://houseplantshop.com")
        print("")
        print("Usage:")
        print("    python3 urls.py [-h|--help] [-n <int>]")
        print("")
        print("Options:")
        print("    -n <int>")
        print("        How much links are to be fetched. If not used, gets all links.")
        print("    -h|--help")
        print("        Prints out this help.")


if __name__ == "__main__":
    arguments = sys.argv[1:]

    if len(arguments) == 0:
        HousePlantsUrlGetter().getUrls()

    elif arguments[0] == "-n" and len(arguments) == 2 and arguments[1].isnumeric():
        HousePlantsUrlGetter(arguments[1]).getUrls()

    elif arguments[0] == ("-h", "--help"):
        HousePlantsUrlGetter().printHelp()
    else:
        HousePlantsUrlGetter().printHelp()
