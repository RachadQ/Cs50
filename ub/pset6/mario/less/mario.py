from cs50 import get_int

def main():
    height = GetHeight()
    row = 0
    
    while row < height:
        column = 0
        decrease = height - 1
        while decrease > row:
            print(" ",end="")
            decrease -= 1
        while column < row:
            print("#", end="")
            column += 1
        print("#")
        row += 1
    
   
        

def GetHeight():
    while True:
         height = get_int("Height: ")
         if height > 0 and height <= 8:
            break
    return height

main()


