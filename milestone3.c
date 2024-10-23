#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// power function to avoid using -lm in the complier
int32_t int_pow(int base, int exp) {
    int32_t result = 1;
    while (exp > 0) {
        result *= base;
        exp--;
    }
    return result;
}


// milestone 1 starts here -------------------------------------------------------
int32_t is_ascii(char str[]){
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


// milestone 2 starts here --------------------------------------------------
int32_t width_from_start_byte(char start_byte){
    if ( (start_byte & 0b11111000) == 0b11110000){ // and with 1111000... then it means 4 byte
        return 4;
    } 
    else if ( (start_byte & 0b11110000) == 0b11100000){ // and with 11100... then it means 3 bytes
        return 3;

    } // 11000000
    else if( (start_byte & 0b11100000) == 0b11000000 ){ // and with 1100... then it means 2 bytes
        return 2;
    }
    else if ( (start_byte & 0b10000000) == 0b00000000 ){ // and with 000000... then it means 1 bytes
        return 1;
    }
    else { // doesn't exist, its a continuation byte, not a start byte
        return -1;
    }

}

int32_t utf8_strlen(char str[]){
    int sub = 0;
    for(int i = 0; i < strlen(str); i++){
        sub += width_from_start_byte(str[i]);
    }
    return sub;

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

    // add opening quote
    result[pos++] = '"';

    for( int i = startPos; i < endPos; i++){
        result[pos] = str[i];
        pos++;
    }

    // add closing quote
    result[pos++] = '"';

    result[pos] = '\0'; // Null terminate the result

}

// milestone3 functions start here ---------------------------------------------------------
int32_t codepoint_at(char str[], int32_t cpi){
    /*int32_t pos = codepoint_index_to_byte_index(str, cpi);
    return str[5];*/
        
    // track the total amount 
    int32_t total = 0;

    // starting position
    int32_t pos = codepoint_index_to_byte_index(str, cpi);

    // how big the byte is
    int32_t byteSize = width_from_start_byte(str[pos])  - 1;

    // the size byte the UTF-8 character is must be calculated first.
    if (byteSize == 3){
        total += ( (unsigned char)str[pos] & (0b00000111)) * (int_pow(64,  byteSize));
    }else if(byteSize == 2){
        total += ( (unsigned char)str[pos] & (0b00001111)) * (int_pow(64,  byteSize));
    }else if(byteSize == 1){
        total += ( (unsigned char)str[pos] & (0b00011111)) * (int_pow(64,  byteSize)); //(int_pow(64,  byteSize))
    }else{
        total += ( (unsigned char)str[pos] );
    }

    // increment to next byte
    byteSize--;
    pos++;

    // start from the beginning of the cpi position, and keep on checking the bytes after (if exists)
    // Ex: if 4 byte sequence, starts at 3 and multiplies the number by 64^3
    for( int i = byteSize; i >= 0; i--){   
        total += ( (unsigned char)str[pos] & (0b00111111)) * (int_pow(64,  i));
        pos++;
    }
    return total;

}

// 128000-128063
// 129408-129454
char is_animal_emoji_at(char str[], int32_t cpi){
    int32_t pos = codepoint_index_to_byte_index(str, cpi);
    int32_t num = codepoint_at(str, cpi);
    int32_t byteSize = width_from_start_byte(str[pos]);
    char ans[100];

    if ( (num >= 128000 && num <= 128063) || (num >= 129408 && 129454) ){
        for (int i = 0; i < byteSize; i++){
            ans[i] = str[pos + i];
        }
        //printf("%s", ans);
        return 1;
    }
    return 0;
}


// helper function for the resubmission function
void codepoint_to_utf8(int32_t codepoint, char result[]) {
    if (codepoint < 0x80) {
        // 1-byte sequence: 0xxxxxxx
        result[0] = (char)codepoint;
        result[1] = '\0'; // Null-terminate
    } else if (codepoint < 0x800) {
        // 2-byte sequence: 110xxxxx 10xxxxxx
        result[0] = (char)((codepoint >> 6) | 0xC0);
        result[1] = (char)((codepoint & 0x3F) | 0x80);
        result[2] = '\0'; // Null-terminate
    } else if (codepoint < 0x10000) {
        // 3-byte sequence: 1110xxxx 10xxxxxx 10xxxxxx
        result[0] = (char)((codepoint >> 12) | 0xE0);
        result[1] = (char)((((codepoint >> 6) & 0x3F) | 0x80));
        result[2] = (char)((codepoint & 0x3F) | 0x80);
        result[3] = '\0'; // Null-terminate
    } else if (codepoint < 0x110000) {
        // 4-byte sequence: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        result[0] = (char)((codepoint >> 18) | 0xF0);
        result[1] = (char)((((codepoint >> 12) & 0x3F) | 0x80));
        result[2] = (char)((((codepoint >> 6) & 0x3F) | 0x80));
        result[3] = (char)((codepoint & 0x3F) | 0x80);
        result[4] = '\0'; // Null-terminate
    } else {
        // Invalid codepoint
        result[0] = '\0'; // Null-terminate for error handling
    }
}

// resubmission function
void next_utf8_char(char str[], int32_t cpi, char result[]){
    // take string, find position of character wnated to index up once, figure out how long the sequence is, then go to 
    // last byte and increment
    char temp[256];
    strcpy(temp, str);
    int pos = codepoint_index_to_byte_index(str, cpi);

    // Get the current codepoint
    int32_t current_codepoint = codepoint_at(str, cpi);

    // Increment the codepoint by 1
    int32_t next_codepoint = current_codepoint + 1;
    
    // holds the new character that will be created by codepoint_to_utf8 function
    char holder[256];
    // Convert the next codepoint to UTF-8
    codepoint_to_utf8(next_codepoint, holder);

    //find the length of the holder of updated character
    // width_from_start_byte(holder[0])

    for(int i = 0; i < width_from_start_byte(holder[0]); i++){
        temp[i+codepoint_index_to_byte_index(str, cpi)] = holder[i];
    }

    strcpy(result, temp);

}

// My 🐩’s name is Erdés.
// My é🐩’s name is Erds.
// My 🐩’s name is Erdős.


int main(){
    // milestone1
    // test is_ascii
    char tester[100];
    char inputStr[100];
    printf("Enter a UTF-8 encoded string: \n");
    fgets(tester, sizeof(tester), stdin);
    
    // make a copy so that when we uppercase it it doesn't affect future uses
    char temp[100];
    strcpy(temp, tester);
    // get rid of the "" at the beginning and end for analyzing.
    /*
    for (int i = 0; i < strlen(inputStr)-2; i++){
        tester[i] = inputStr[i+1];
    }
    */
    
    // Remove newline character if it exists
    size_t len = strlen(tester);
    if (len > 0 && tester[len - 1] == '\n') {
        tester[len - 1] = '\0';
    }

    // Test if the ASCII is valid
    if (is_ascii(tester)){
        printf("Valid ASCII: true\n");
    }else{
        printf("Valid ASCII: false\n");
    }


    // test capitalize_ascii
    capitalize_ascii(temp);
    printf("Uppercased ASCII: %s\n", temp);
    
    // Just used strlen(), there was no particular function on PA1 to return the length in bytes
    printf("Length in bytes: %ld\n", strlen(tester));

    // Test Number of code points
    printf("Number of code points: %d\n", utf8_strlen(tester));

    // Test bytes per code point
    printf("Bytes per code point: ");
    int32_t pos = 0;
    for(int i = 0; i < utf8_strlen(tester); i++){
        pos = codepoint_index_to_byte_index(tester, i);
        printf("%d ", width_from_start_byte(tester[pos]));
    }
    printf("\n");

    // Test Substring of the first 6 code points
    char results[100];
    if (utf8_strlen(tester) > 6){
        utf8_substring(tester, 0, 6, results);
    }
    else{
        utf8_substring(tester, 0, utf8_strlen(tester), results);
    }
    
    printf("Substring of the first 6 code points: %s\n", results);

    // Test Code points as decimal numbers
    printf("Code points as decimal numbers: ");
    for(int i = 0; i < utf8_strlen(tester); i++){
        printf("%d ", codepoint_at(tester, i));
    }
    printf("\n");

    // Test Animal emojis
    printf("Animal emojis: ");
    int32_t byteSize;
    char emoji[100];
    for(int i = 0; i < utf8_strlen(tester); i++){
        pos = codepoint_index_to_byte_index(tester, i);
        byteSize = width_from_start_byte(tester[pos]);
        if(is_animal_emoji_at(tester, i)){
            for(int j = 0; j < byteSize; j++){
                emoji[j] = tester[pos+j];
            }
            printf("%s ", emoji);

        }
    
    }

    printf("\n");

    // Next codepoint
    int cpi = 3;
    char results1[256];
    strcpy(results1, tester);
    next_utf8_char(tester, cpi, results1);
    printf("Next Character of Codepoint at Index %d: %s\n", cpi ,results1);

    /*
    // milestone2
    // Test first function
    char s[] = "Héy"; // same as { 'H', 0xC3, 0xA9, 'y', 0 },   é is start byte + 1 cont. byte
    printf("Width: %d bytes\n", width_from_start_byte(s[1])); // start byte 0xC3 indicates 2-byte sequence
    printf("Width: %d bytes\n", width_from_start_byte(s[2])); // start byte 0xA9 is a continuation byte, not a start byte

    // Test second function
    char str3[] = "Joséph";
    printf("Length of string %s is %d\n", str3, utf8_strlen(str3));  // 6 codepoints, (even though 7 bytes)

    // Test third function
    char str4[] = "Joséph";
    int32_t idx1 = 4;
    printf("Codepoint index %d is byte index %d\n", idx1, codepoint_index_to_byte_index(str4, idx1));

    // Test fourth function
    char result[17];
    utf8_substring("🦀🦮🦮🦀🦀🦮🦮", 3, 7, result);
    printf("String: %s\nSubstring: %s\n", "🦀🦮🦮🦀🦀🦮🦮" , result); // these emoji are 4 bytes long

    // milestone3 
    char str5[] = "Joséph";
    char str6[] = "🐩";
    int32_t idx = 0;
    printf("Codepoint at %d in %s is %d\n", idx, str6, codepoint_at(str6, idx)); // 'p' is the 4th codepoint
    printf("Animal emojis: %c\n", is_animal_emoji_at(str6, idx));
    */
    return 0;
}
