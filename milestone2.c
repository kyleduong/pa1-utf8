#include <stdio.h>
#include <string.h>
#include <stdint.h>
/*
// milestone 1 starts here -------------------------------------------------------
int32_t is_ascii(char str[]){

    for (int i = 0; i > strlen(str); i++){
        if (str[i] >= 0 && str[i] <= 127){
            return 1;
        }
        return 0;
    }
    */
// keep commented
    /*
    for(int i = 0; i < strlen(str); i++){
        if(str[i] > 127 || str[i] < 0){
            return 0;
        }
    }

    return 1;
}

int32_t capitalize_ascii(char str[]){
    int32_t numUpdated = 0;

    for(int j = 0; j < strlen(str); j++){
        if(str[j] <= 122 && str[j] >= 97){
            str[j] -= 32;
            numUpdated++;
        }
    }
    return numUpdated;

}
*/



/*
int32_t capitalize_ascii(char str[]){
    int32_t total = 0;
    for (int i = 0; i < strlen(str); i++){
        if (is_validascii(str)){
            str[i] -= 32;
        }
        total += 1
    }
    return total;


}

int32_t width_from_start_byte(char start_byte){
    if( (start_byte & 0b11111000) == 0b11110000){
        return 4;
    }else if(start_byte & 0b11110000 == 0b11100000){
        return 3;
    }else if( (start_byte & 0b11100000) == 0b11000000){
        return 2;
    }else if( (start_byte & 0b10000000) == 0b00000000 ){
        return 1;
    }else{
        return -1;
    }
}

int32_t utf8_strlen(char str[]){
    int pos = 0;
    while (pos < strlen(str)){
        pos += width_from_start_byte(str[pos])
    }
    return pos;
}

int32_t codepoint_index_to_byte_index(char str[], int32_t cpi){
    int pos = 0;
    for(int i = 0; i < cpi; i++){

        pos += width_from_start_byte(str[pos]);
    }
    return pos;
}

void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char result[]){
    // find the position in which the str starts
    int32_t startPos = codepoint_index_to_byte_index(str, cpi_start);

    // find the position in which the str ends
    int32_t endPos = codepoint_index_to_byte_index(str, cpi_end);

    char curString[100];
    int32_t pos = 0;
    for( int i = startPos; i < endPos; i++){
        result[pos] = str[i];
        pos++;
    }

    result[pos] = '\0'; // Null terminate the result

}

int32_t codepoint_at(char str[], int32_t cpi){
    
    // track the total amount 
    int32_t total = 0;

    // starting position
    int32_t pos = codepoint_index_to_byte_index(str, cpi);

    // how big the byte is
    int32_t byteSize = width_from_start_byte(str[pos])  - 1;

    

}

int main(){

    // Test first function
    char s[] = "Héy"; // same as { 'H', 0xC3, 0xA9, 'y', 0 },   é is start byte + 1 cont. byte
    printf("Width: %d bytes\n", width_from_start_byte(s[1])); // start byte 0xC3 indicates 2-byte sequence
    printf("Width: %d bytes\n", width_from_start_byte(s[2])); // start byte 0xA9 is a continuation byte, not a start byte
    
    // Test second function
    char str[] = "Joséph";
    printf("Length of string %s is %d\n", str, utf8_strlen(str));  // 6 codepoints, (even though 7 bytes)

    // Test third function
    char str1[] = "Joséph";
    int32_t idx1 = 4;
    printf("Codepoint index %d is byte index %d\n", idx1, codepoint_index_to_byte_index(str1, idx1));

    // Test fourth function
    char result[17];
    utf8_substring("🦀🦮🦮🦀🦀🦮🦮", 3, 7, result);
    printf("String: %s\nSubstring: %s", "🦀🦮🦮🦀🦀🦮🦮" , result); // these emoji are 4 bytes long

    return 0;
}
*/
