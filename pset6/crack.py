#http://docs.cs50.net/problems/crack/crack.html
import sys
from crypt import crypt
from string import ascii_letters as alphab
from itertools import product

def crack_password(enc_pass):
    # we can assume that the salt is made up of the first two
    # characters of the encrypted password
    salt = enc_pass[0:2]

    combination_counter = 0

    for letters in product(alphab, repeat=4):
        password = "".join(letters)
        hash = crypt(password, salt)
        if hash == enc_pass:
            print(password)
            sys.exit()

if __name__ == "__main__":
    if (len(sys.argv) != 2):
        sys.exit("Usage: python crack.c hash")
    enc_pass = sys.argv[1]
    crack_password(enc_pass)