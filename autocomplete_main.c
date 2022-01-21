#include <stdio.h>
#include <stdlib.h>

#include "autocomplete.h" //this line may not be necessary
#include "autocomplete copy.c"

int main(void)
{
    struct term *terms;
    int nterms;
    read_in_terms(&terms, &nterms, "cities.txt");
    //read_in_terms(&terms, &nterms, "wiktionary.txt");
    //char s[5] = "abcd";
    //printf("%i\n", strlen(s));
    // printf("first index: %i\n", lowest_match(terms, nterms, "'s"));
    // printf("highest match: %i\n", highest_match(terms, nterms, "Zhum"));
    //printf("lowest match for Tor: %d\n", lowest_match(terms, nterms, "Tor"));
    // printf("highest match for Tor: %d\n", highest_match(terms, nterms, "Tor"));
    // printf("lowest match for Zhiz: %d\n", lowest_match(terms, nterms, "Zhiz"));
    // printf("highest match for Zhiz: %d\n", highest_match(terms, nterms, "Zhiz")); 
    //printf("%d\n", 3/2);
    struct term *answer;
    int n_answer;
    autocomplete(&answer, &n_answer, terms, nterms, "Toronto, Ontario");
    //autocomplete(&answer, &n_answer, terms, nterms, "the");   //problem: "they" is sorted first but is lighter
    //autocomplete(&answer, &n_answer, terms, nterms, "rece");
    
    //free allocated blocks here -- not required for the project, but good practice
    //free(terms); 
    //free(answer);
    return 0;
}