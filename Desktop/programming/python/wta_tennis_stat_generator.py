import csv
import pprint
import datetime
import glob
import sys
import operator
import itertools
import collections
from operator import itemgetter
from collections import OrderedDict
import json
import numpy as np
import pandas as pd
import math
import urllib
import datetime
#from pandas.core.categorical import Categorical

# from urllib2 import urlopen, HTTPError
# from spyderlib.widgets.externalshell import namespacebrowser
def readATPMatches():
	#tourney_id,tourney_name,surface,draw_size,tourney_level,tourney_date,match_num,winner_id,winner_seed,winner_entry,winner_name,winner_hand,winner_ht,winner_ioc,winner_age,winner_rank,winner_rank_points,loser_id,loser_seed,loser_entry,loser_name,loser_hand,loser_ht,loser_ioc,loser_age,loser_rank,loser_rank_points,score,best_of,round,minutes,w_ace,w_df,w_svpt,w_1stIn,w_1stWon,w_2ndWon,w_SvGms,w_bpSaved,w_bpFaced,l_ace,l_df,l_svpt,l_1stIn,l_1stWon,l_2ndWon,l_SvGms,l_bpSaved,l_bpFaced
    """Reads ATP matches but does not parse time into datetime object"""
    i = 1968
    while i < 1969:
    	url = "https://raw.githubusercontent.com/JeffSackmann/tennis_atp/master/atp_matches_" + str(i) + ".csv"
    	webpage = urllib.urlopen(url)
    	datareader = csv.reader(webpage)

    	data = []
    	tourney_id = ""
    	tourney_name = ""
    	for row in datareader:
    		data.append(row)
    		tourney_id = data[20]
    	i+=1
    #print("tourney_id: " + str(tourney_id))
   	print (data)

def matchesPerCountryAndRound(matches):
    """find single matches based on country and round"""
    matches = matches[(matches['round']=='F') & (matches['winner_ioc'] == 'AUT') & (matches['loser_ioc'] == 'AUT')]
    matches = matches.sort(['tourney_date'], ascending=False)
    #print matches.to_string(columns=['tourney_name','tourney_date','winner_name', 'loser_name'])
    print(matches[['tourney_name','tourney_date','winner_name', 'loser_name']].to_csv(sys.stdout,index=False))
def main():
	#parse("https://raw.githubusercontent.com/JeffSackmann/tennis_atp/master/atp_matches_2017.csv")
	readATPMatches()

if __name__ == "__main__":
	main()
