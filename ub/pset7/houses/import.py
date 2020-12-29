# TODO
import sys
import csv
from cs50 import SQL
import sqlite3
def main():
     #check if enough arguments was give
    if len(sys.argv) is not 2:
        print("Usage: python import.py characters.csv")
        exit(1)

     # Connect with the .db file and make a cursor
    db = "students.db"
    con = sqlite3.connect(db)

    cursor = con.cursor()
   
     #open crv file with exemptions to a list
    with open(sys.argv[1])as csvfile:
        reader = list(csv.reader(csvfile))
        line_count = 0
        counter = 1
        for row in reader:
            if line_count == 0:
                #print rows
                print(f'Column names are {", ".join(row)}')
                line_count += 1
            else:
                #split the name
                splitName = row[0].split()
                print(splitName)
                
                #IF LENG IS 3 iinsert into database
                if(len(splitName) == 3):
                    cursor.execute("INSERT INTO students VALUES(?,?,?,?,?,?)",(None,splitName[0],splitName[1],splitName[2],row[1],row[2]))
                    line_count += 1
                
                elif (len(splitName) == 2):
                    cursor.execute("INSERT INTO students VALUES(?,?,?,?,?,?)",(None,splitName[0],None,splitName[1],row[1],row[2]))
                    line_count += 1
            
                counter+= 1
                con.commit()
       
    
            
        
         #give acces to database
  





main()