#include <stdio.h>
#include <cs50.h>
#include <crypt.h>
#include <string.h>

int crack_password(string password);
int main(int argc, string argv[]) {
    
    // Make sure user provides 1 argument.
    if (argc != 2 || argc == 0) {
        printf("Must give only one command line argument\n");
        return 1;
    }
    
    string encrypted_password = argv[1];
    //printf("encrypted password: %s\n", encrypted_password);
    return crack_password(encrypted_password);
    
};

int crack_password(string enc_password) {
    // get enc_password length
    int enc_password_len = strlen(enc_password);
    printf("enc_password_len %d\n", enc_password_len);
    
    char salt[3] = {0};
    salt[0] = enc_password[0];
    salt[1] = enc_password[1];
    printf("%s\n", salt);
    printf("enc_password: %s\n", enc_password);
    
    char fake_password[5] = {0};
    
    // Lower case letters ascii range 97 - 122
    // Upper case letters ascii range 65 - 90
    
    char initial_value = 65;
    fake_password[0] = initial_value;
    fake_password[1] = initial_value;
    fake_password[2] = initial_value;
    fake_password[3] = initial_value;
    int fake_password_len = strlen(fake_password);
    
    // Try every combination of password
    int inner;
    int outer_one;
    int outer_two;
    int outer_three;
    
    int combination_counter = 0;
    int current_place = fake_password_len - 1;
    
    // Third outer loop, which changes the letters of fake_password[fake_password_len - 4] or fake_password[0] in this case
    for (outer_three = initial_value; outer_three < 123; outer_three++) {
        if (outer_three == 91) {
            outer_three = 97;
        }
        fake_password[current_place - 3] = outer_three;
    
        // Second outer loop, which changes the letters of fake_password[fake_password_len - 3]
        for (outer_two = initial_value; outer_two < 123; outer_two++) {
            if (outer_two == 91) {
                outer_two = 97;
            }
            fake_password[current_place - 2] = outer_two;
        
            //first outer loop, which changes the letters of fake_password[fake_password_len - 2]
            for (outer_one = initial_value; outer_one < 123; outer_one++) {
                // Change
                if (outer_one == 91) {
                    outer_one = 97;
                }
                fake_password[current_place - 1] = outer_one;
                
                
                //inner most loop, which changes the letters of the rightmost place of the fake_password, or fake_password[fake_password_len - 1]
                for (inner = initial_value; inner < 123; inner++) {
                    if (inner == 91) {
                        inner = 97;
                    }
                    fake_password[current_place] = inner;
                    //printf("%s\n", fake_password);
                    // check here for correct password
                    string guess = crypt(fake_password, salt);
                    
                    int guess_value = strncmp(guess, enc_password, enc_password_len);
                    //printf("guess value: %d\n", guess_value);
                    
                    if (guess_value == 0) {
                        printf("%s\n", fake_password);
                        return 0;
                    }
                    //return guess_password(fake_password, salt, enc_password);
                    combination_counter += 1;
                    
                }
                
            }
        }
    }
    printf("Combination counter: %d\n", combination_counter);
    printf("no match found\n");
    return 1;
    
    //printf("fake_password_len: %d\n", fake_password_len);
};


