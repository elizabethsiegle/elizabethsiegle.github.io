import json
from random import *

def random_digits():
    range_start = 1
    range_end = 47 #max
    return randint(range_start, range_end)
    
def getQuote():
    with open('data.json') as datafile:
        data = json.load(datafile)
    quote = data[random_digits()]
    print quote
    return quote