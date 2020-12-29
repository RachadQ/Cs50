from cs50 import get_string

letters = 0
words = 0
sentences = 0

def main():
    global letters
    global words
    global sentences
    #prompt user for information
    text = get_string("Text:" )
    
    
    for i in range(len(text)):
        
        if i > len(text):
            break
        #check if spaces
        if not " " in text[i]:
            #check if not punction
            if not text[i] in ('!', "," ,"\'" ,";" ,"\"", ".", "-" ,"?"):
                letters += 1
            else:
                if text[i] == "." or text[i] == "?" or text[i] == "!":
                    sentences += 1
        #if is space
        word = text.split()
        words = len(word)
    ColemanLiau(AverageLtoW(),AverageStoW())
    





def ColemanLiau(l , s):
    
    letterCount = 0.0588 * l
    wordCount = 0.296 * s
    coleman = (letterCount - wordCount - 15.8)
    #check if under level 1
    index = round(coleman)
    if index < 1:
        print("Before Grade 1")
    elif index > 16:
        print("Grade 16+")
    else:
        print("Grade " + str(index))

def AverageLtoW():
    average = 0
    l = float(letters)
    w = float(words)
    #get average letter to word
    average = (l/w) * 100
    return average

def AverageStoW():
    average = 0
    
    s = float(sentences)
    w = float(words)
    # get average letter to word
    average = (s/w)*100
    return average


main()
