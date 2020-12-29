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
    
    db = "students.db"
    con = sqlite3.connect(db)
    cursor = con.cursor()
    sql = "SELECT first, middle, Last, birth from students Where house =?  ORDER BY Last"
    val = (sys.argv[1],)
   
    cursor.execute(sql,val)
    myresult = cursor.fetchall()
    
   
    for result in myresult:
        if result[1] == None:
            print(result[0] + " " + result[2] + ", born " + str(result[3]))
        else:
            print(result[0] + " " + result[1] + " " + result[2] + ", born " + str(result[3]))
          
    
        
        
   
    
    

main()