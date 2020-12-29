import sys
import csv
import itertools
import re

def main():
    #check if enough arguments was give
    if len(sys.argv) is not 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit(1)

    #open crv file with exemptions to a list
    with open(sys.argv[1])as csvfile:
        reader = list(csv.reader(csvfile))
        reader[0].remove("name")
        i = reader[0]


    #read sequencces file
    dnaSequences = open(sys.argv[2], "r")
    #read dna Sequence
    str = dnaSequences.readline()
    valueList = []
    for index in range(len(i)):
        maxCounter = 0
        counter = 0
        position = 0
        previousPosition = 0
        
        #loop through through and find dnasequence
        while position < len(str):
            position = str.find(i[index], position)
            if position == -1:
               counter = 0
               break
            #at beginning of the sequence
            elif (position != -1) and previousPosition == 0:
                counter +=1
                maxCounter = counter
                previousPosition = position
            elif (position != -1) and ((position - len(i[index])) == previousPosition):
                counter +=1
                previousPosition = position
                if maxCounter < counter:
                    maxCounter = counter
            elif (position != -1) and ((position - len(i[index])) != previousPosition):
                counter = 1
                previousPosition = position
                if maxCounter < counter:
                    maxCounter = counter
        poisition +=1
        #record highest sequence
        valueList.append(maxCounter)

    #update the list to be a list of strings to enable comparison.
    valueList = list(map(str,valueList))
    #make new list to preseve reader
    
    cleaned = list(reader)
    cleaned.pop(0)
    
    for person in cleaned:
        if person[1:] == valueList:
            print(f"{person[0]}")
            break
        elif person == cleaned[-1]:
            print("no match")

main()