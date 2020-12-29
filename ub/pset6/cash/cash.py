from cs50 import get_float
#run main
def main():
    #loop to get input
    while True:
        change = get_float("Change owed: ");
        if change > 0:
            break
    #convert input
    cents = int(change * 100)
    greed(cents)

def greed(amount):
    count = 0
    #loop until get 0
    while amount > 0:
        #25 cent
        if amount >= 25:
            amount -= 25
            count += 1
        elif amount >= 10 and amount < 25:
            amount -= 10;
            count += 1
        elif amount >= 5 and amount < 25 and amount < 10:
            amount -= 5;
            count += 1;
        elif amount >= 1 and amount < 25 and amount < 10 and amount < 5:
            amount -= 1
            count += 1
    #print amount    
    print(count)
    
main()