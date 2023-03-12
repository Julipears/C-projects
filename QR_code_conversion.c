#include "a2.h"
// message string will be converted to encrypted SC codes by performing bit-wise XOR encryption on the ASCII value of each character in the message, convert the encrypted value to a 7 bit binary string of 1s and 0s and copying th eencrypted strings generated for each character into a character array following a specific SC code structure
/* use XOR encryption to encrypt and decrypt characters by performing a bit-wise XOR operation on the binary representation of the ASCII value fo KEY, a chacter defined in a2.h

result of encryption is a new binary number. 

To recover the original character, perform an XOR operation on the encrypted binary number using the same key value used for encrypting */

//helper function from ascii to Binary

int min(int a, int b, int c);
int same(char a, char b);
char* asciiToBinary(int value);
int binaryToAscii(char* value);

char* asciiToBinary(int value) {
    //takes value as an ascii int, returns binary as
    //string of binary number
    char* b_res = (char*)calloc(8, sizeof(char));
    int rem, a_res = value;
    
    for (int i=0; i<7; i++) {
        rem = a_res%2;
        a_res = (a_res-rem)/2;
        b_res[6-i] = rem + '0';
    }

    b_res[7] = '\0';
  
    return b_res;
}

int binaryToAscii(char* value) {
    //takes a binary string and returns ascii number as int
    int a_res = 0;

    for (int i=0; i<7; i++) {
        a_res += (value[6-i]-'0')*pow(2, i);
    }
    return a_res;
}

int bitwise_xor(int value){
  /* returns the result of performing a bit-wise XOR operation on <value> with the ASCII value of the KEY character defined in a2.h

ex: bitwise_xor(67) should return the integer 16 if KEY is 'S' (ASCII value 83) and bitwise_xor(83) should return the integer 83 is KEY is '\0' (ASCII value 0)

<value> is a positive ASCII value (integer)
return the integer result of performing a bit-wise XOR operation on <value> with the ASCII value of KEY deined in a2.h
*/
    char str_br[7] = {0};
    char* asc_v = asciiToBinary(value);
    char* key_v = asciiToBinary(KEY);
    
    for (int i=6; i>=0; i--) {
        str_br[i] = asc_v[i]^key_v[i];
    }
    
    int res = 0;
  
    for (int i=0; i<7; i++) {
         res += str_br[6-i] * pow(2, i);
      }
  
    free(asc_v);
    free(key_v);
    return res;
}

char *xor_encrypt(char c){ // return the binary representation of the character c
  /*generates a 7-digit string of "1s" and "0s" representing the binary result of performing a bit-wise XOR operation on the ASCII value of <c> witht he ASCII value of KEY defined in a2.h

ex: xor_encrypt('C') should return a pointer to "0010000" if KEY is 'S' and xor_encrypt('S') should return a point to "1010011" if KEY is '\0'

<c> is a character
return a point to a 7 digit string of 1s and 0s, reperesetning the result of performing a bit-wise XOR operation on the ASCII value of <c> witht he ASCII value of KEY, defined in a2.h */
    int value = (int)c;
    char* binary_res = asciiToBinary(bitwise_xor(value));
    
    return binary_res;
}

char xor_decrypt(char *s){ // return the character representation of a binary number
    /* use XOR encryption to encrypt and decrypt characters 

deciphers which character is representd by <s>, assume <s> represents an XOR encrypted character that must be decrypted with an XOR operation using KEY (defined in a2.h

ex: xor_decrypt("0010000") should return the character 'C' is KEY is 'S' and xor_decrypt("1010011") returns the character 'S' if KEY is '\0'

<s> is a point to a 7 digit string of 1s and 0s
return the character obtained after evaluating the ASCII value resulting from performing a bit-wise XOR operation on the value represented by <s> with the ASCII value of KEY*/
    char* ascii_res = xor_encrypt(binaryToAscii(s));
    int res = binaryToAscii(ascii_res);

    free(ascii_res);
    return (char)res;
}

char *gen_code(char *msg){
/* encrypt the character of <msg> and stores the result in an SC code string

return a pointer to a 256 digit string of 1s and 0s representing an SC code containing the encrypted message*/
    char* sc_code = (char*)calloc(257, sizeof(char));
    int len = strlen(msg);
    char* enc_letter;
    
    for (int i=0; i<256; i++) {
        if (i<=4 || (i>=11&i<17) || i==20 || i==27 || i==31 || i==32 || i==34 || i==36 || i==43 || i==45 || i==47 \
        || i==48 || i==52 || i==59 || (i>=63&&i<=68) || (i>=75&&i<=79) || (i>=176&&i<=180) || i==192 || i==196 \
        || i==208 || i==210 || i==212 || i==224 || i==228 || (i>=240&&i<=244) || i==255) {
            sc_code[i] = '1';
        } else {
            sc_code[i] = '0';
        }
    }
    
    int count = 5;
    
    for (int i=0; i<len; i++) {
        enc_letter = xor_encrypt(msg[i]);
        
        for (int j=0; j<7; j++) {
            if (sc_code[count] != '1') {
                sc_code[count] = enc_letter[j];
                count++;
            } else {
                j--;
                count += 5;
            }
        }
        
        free(enc_letter);
    }
    
    enc_letter = xor_encrypt('\0');
        
    for (int j=0; j<7; j++) {
        if (sc_code[count] != 1) {
            sc_code[count] = enc_letter[j];
            count++;
        } else {
            j--;
            count += 5;
        }
    }

    sc_code[256] = '\0';
    free(enc_letter);
    
    return sc_code;
}

// // to visualize the qr code
//     void visualize(char* sc_code){
//         for (int i = 0; i<256;i++)
//     {
//         if(code[i] == '1')
//         {
//             printf(" ■ ");
            
//         }
//         else
//         {
//             printf("   ");
//         }
        
//         if((i+1)%16 == 0 )
//         {
//             printf("\n");
//         }
//     }
// }

char *read_code(char *code){
/* deciphers the message encoded in an SC code and generates a string containing the deciphered message, when the SC code in Appendix A is passed to scane_code() and a single  string input, a pointer to the string "Program in C!" should be returned*/
  // the string ends when the null character is found in the SC code
    char* sc_code = (char*)calloc(256, sizeof(char));
    for (int i=0; i<256; i++) {
        sc_code[i] = code[i];
    }
    char* dec_code = (char*)calloc(1, sizeof(char));
    char dec_letter;
    char* dec_binary = (char*)calloc(7, sizeof(char));
    
    for (int i=0; i<256; i++) {
        if (i<=4 || (i>=11&i<=20) || (i>=27&&i<=36) || (i>=43&&i<=52) || (i>=59&&i<=68) || (i>=75&&i<=79) || (i>=176&&i<=180) || (i>=192&&i<=196) \
        || (i>=208&&i<=212) || (i>=224&&i<=228) || (i>=240&&i<=244) || i==255) {
            sc_code[i] = '2';
        }
    }
  
    int count = 0;
    int letter_count = 0;

    // continue here
  
    for (int i=5; i<256; i++) {
        if (sc_code[i] == '2') {
            i+=4;
        } else {
            if (count == 7) {
                dec_letter = xor_decrypt(dec_binary);
                dec_code[letter_count] = dec_letter;
                free(dec_binary);
                dec_binary = (char*)calloc(7, sizeof(char));
                letter_count++;
                dec_code = realloc(dec_code, (letter_count+1)*sizeof(char));
                count = 0;
            }
            dec_binary[count] = sc_code[i];
            count++;
        }
    }
    dec_code[letter_count] = '\0';
  
    free(dec_binary);
    free(sc_code);
    
    return dec_code;
}

char *compress(char *code){
    char* hex_code = (char*)calloc(65, sizeof(char));
    char* b_code = (char*)calloc(4, sizeof(char));
    int hex_num = 0;
    int count = 0, h_count = 0;
  
    for (int i=0; i<256; i++) {
        b_code[count] = code[i];
        if (count == 3) {
            hex_num += (b_code[0]-'0')*pow(2, 3) + (b_code[1]-'0')*pow(2, 2) + (b_code[2]-'0')*pow(2, 1) + (b_code[3]-'0');
            if (hex_num > 9) {
                if (hex_num == 10) {
                    hex_code[h_count] = 'A';
                } else if (hex_num == 11) {
                    hex_code[h_count] = 'B';
                } else if (hex_num == 12) {
                    hex_code[h_count] = 'C';
                } else if (hex_num == 13) {
                    hex_code[h_count] = 'D';
                } else if (hex_num == 14) {
                    hex_code[h_count] = 'E';
                } else if (hex_num == 15) {
                    hex_code[h_count] = 'F';
                }
            } else {
                hex_code[h_count] = hex_num + '0';
            }
            hex_num = 0;
            free(b_code);
            b_code = (char*)calloc(4, sizeof(char));
            count = 0;
            h_count++;
        } else {
            count++;
        } 
    }
    free(b_code);
    hex_code[64] = '\0';
    
    return hex_code;
}

char *decompress(char *code){
    char* b_code = (char*)calloc(257, sizeof(char));
    int len = strlen(code), b_count = 0, h_num = 0;
    
    for (int i=0; i<len; i++) {
        if (code[i] == 'A') {
            h_num = 10;
        } else if (code[i] == 'B') {
            h_num = 11;
        } else if (code[i] == 'C') {
            h_num = 12;
        } else if (code[i] == 'D') {
            h_num = 13;
        } else if (code[i] == 'E') {
            h_num = 14;
        } else if (code[i] == 'F') {
            h_num = 15;
        } else {
            h_num = code[i]-'0';
        }
        for (int j=0; j<4; j++) {
            b_code[b_count+3-j] = h_num%2+'0';
            h_num = (h_num-h_num%2)/2;
        }
        h_num = 0;
        b_count += 4;
    }
  
    return b_code;
}

int min(int a, int b, int c) {
    if (a>=b && c>=b) {
        return b;
    } else if (b>=c && a>=c) {
        return c;
    } else {
        return a;
    }
}

int same(char a, char b) {
    if (a == b) {
        return 0;
    } else {
        return 1;
    }
}

int calc_ld(char *sandy, char *cima){
    int len_s = strlen(sandy), len_c = strlen(cima);
    int matrix[len_s+1][len_c+1];
    matrix[0][0] = 0;
    for (int row = 1; row <= len_s; row++) {
        matrix[row][0] = matrix[row-1][0]+1;
    }
    for (int col = 1; col <= len_c; col++) {
        matrix[0][col] = matrix[0][col-1]+1;
    }
    for (int row = 1; row <= len_s; row++) {
        for (int col = 1; col <= len_c; col++) {
            matrix[row][col] = min(matrix[row-1][col-1] + same(sandy[row-1], cima[col-1]), matrix[row-1][col]+1, matrix[row][col-1]+1);
        }
    }
  
    return matrix[len_s][len_c];
}

/**
Code to print the QR code thing:
for (int i=0; i<256; i++) {
    if (i%16 != 0) {
        printf("%c ", sc_code[i]);
    } else {
        printf("\n");
        printf("%c ", sc_code[i]);
    }
}
*/
