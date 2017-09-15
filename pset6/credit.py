#http://docs.cs50.net/problems/credit/credit.html
def check_credit(card_number):

    card_length = len(card_number)
    sum = 0

    for i in range(card_length - 2, -1, -2):
        product = int(card_number[i])*2
        for i in str(product):
            sum = sum + int(i)
    for i in range(card_length - 1, -1, -2):
        sum = sum + int(card_number[i])

    if sum % 10 == 0:
        first = int(card_number[0])
        second = int(card_number[1])
        if first == 4:
            print("VISA")
        elif first == 3 and (second == 4 or second == 7):
            print("AMEX")
        elif first == 5 and (second in range(1, 66)):
            print("MASTERCARD")
    else:
        print("INVALID")

if __name__ == "__main__":
    # Ask user for credit card number
    while True:
        try:
            number_string = input("Number: ")
            number = int(number_string)
            break
        # Keep asking as long as number given is not integer
        except ValueError:
            pass
    check_credit(number_string)