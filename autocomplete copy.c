#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "autocomplete.h"

int compare_structs(const void *head1, const void *head2){
    return strcmp(((struct term *)head1)->term, ((struct term *)head2)->term);    //strcmp: value is negative if order is correct, positive if chars should be switched
}

int compare_weights(const void *head1, const void *head2){
    float i = ((struct term *)head2)->weight - ((struct term *)head1)->weight;  //value is postive if head2 > head1 (which means they'll be switched). to sort highest to lowest
    if (i > 0){
        return 1;
    }
    else if (i < 0){
        return -1;
    }
    else if (i == 0){
        return 0;
    }
}

void read_in_terms(struct term **terms, int *pnterms, char *filename){
    /* Take in a pointer to a pointer to struct term, a pointer to an int (pnterms), and the name
    of a file formatted like cities.txt. Allocate memory for all the terms in the file 
    and store a pointer to the block in *terms. Store the number of terms in *pnterms. 
    Read in all the terms from filename, and place them in the block pointed to by *terms.
    The terms should be sorted in lexicographic order. */

    char line[200];
    FILE *fp = fopen(filename, "r");
    fgets(line, sizeof(line), fp);   //gets the 1st line which is the # of lines/terms in the file
    *pnterms = atoi(line);
    //printf("number of terms: %i\n", *pnterms);    //93827
    *terms = (struct term *)malloc((sizeof(struct term)) * (*pnterms));    //or **terms since stores a pointer to a block in *terms?
    //*terms = (struct term *)malloc((sizeof(struct term)) * (2));
    for(int i = 0; i < *pnterms; i++){ 
        fgets(line, sizeof(line), fp); //read in at most sizeof(line) characters (including '\0') into line.
        //printf("%c\n", line[15]); //checking that the city name starts at that index: S
        int j = 0;
        while(isdigit(*(line + j)) == 0){   //returns 0 if it is NOT a number
            j++;    //counting to find the index where the weight number starts
        }
        //printf("%d\n", j);  
        int k = j;
        while(isdigit(*(line + k)) != 0){   //to find the index where the name starts
            k++;
        }
        char city_weight[k-j];  //declaring weight (size based on where all city names start at 15)
        strncpy(city_weight, line + j, k-j);    //creates a string starting from index j until where names start
        //printf("string version of weight: %s\n", city_weight);
        //printf("integer version of weight: %i\n", atoi(city_weight));   //convert string weight into integer
        char city[201-k-1];
        strncpy(city, line + k + 1, 201- k - 1);  
        //printf("extracted city: %s\n", city);
        int size = strlen(city);
        city[size - 1] = '\0';  //to get rid of newline character at end of city
        strcpy((*terms +i)->term, city);  //*terms is the address of the first term; +i to keep going
        (*terms + i)->weight = atof(city_weight);   //*terms = head
        //printf("city using struct: %s\n", (*terms + i)->term); 
        //printf("weight using struct: %f\n", (*terms + i)->weight);        
    }   
    qsort(*terms, *pnterms, sizeof(struct term), compare_structs);
    //qsort(*terms, 3, sizeof(struct term), compare_structs);
    // printf("first city after sorting:%s\n", (*terms + 0)->term); 
    // printf("first city's weight after sorting:%f\n", (*terms)->weight); 
    // printf("last city after sorting: %s\n", (*terms + *pnterms - 1)->term);
    // printf("last city's weight after sorting: %f\n", (*terms + *pnterms - 1)->weight);
}

int lowest_match(struct term *terms, int nterms, char *substr){
    /* return the index in terms of the first term in lexicographic ordering that matches the string substr */
    int low = 0;    
    int high = nterms -1;   
    int length = strlen(substr);    //to know up to how many chars to compare 
    while(low < high - 1){
        int mid = (low + high) / 2;
        if(strncmp((terms + mid)->term, substr, length) < 0){    
            low = mid;  //<0 means subtr comes after mid
        }
        else if(strncmp((terms + mid)->term, substr, length) > 0){   //means substr comes before mid
            high = mid;
        }
        else{
            high = mid;
        }
    }
    if(strncmp((terms + low)->term, substr, length) == 0){
        return low; 
    }      
    else if(strncmp((terms + high)->term, substr, length) == 0){
        return high;
    }
    return -1;  
} 

int highest_match(struct term *terms, int nterms, char *substr){
    /* return the index in terms of the last term in lexicographic order that matches the string substr */
    int low = 0;    
    int high = nterms -1;   
    int length = strlen(substr);    //to know up to how many chars to compare
    while(low < high - 1){
        int mid = (low + high) / 2;
        if(strncmp((terms + mid)->term, substr, length) < 0){    
            low = mid;  //<0 means subtr comes after mid
        }
        else if(strncmp((terms + mid)->term, substr, length) > 0){   //means substr comes before mid
            high = mid;
        }
        else{
            low = mid;
        }
    }
    if(strncmp((terms + high)->term, substr, length) == 0){
        return high; 
    }               
    else if(strncmp((terms + low), substr, length) == 0){
        return low;
    }
    return -1; 
}

void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr){
    /* takes terms (assume it is sorted lexicographically), the number of terms nterms, and the query string substr, 
    and place the answers in answer, with *n_answer being the number of answers. The answer should be sorted by decreasing weight */

    //find the number of answers:
    int high = highest_match(terms, nterms, substr);
    int low = lowest_match(terms, nterms, substr);
    if (high == -1){
        *n_answer = 0;
        return;
    }

    *n_answer = high - low + 1;
    
    //allocate memory for answer struct
    *answer = (struct term *)malloc((sizeof(struct term)) * (*n_answer)); 
    //printf("number of answers: %i\n", *n_answer);

    // put in matches into struct:  loop to run as long as the highest match index hasn't been reached
    int i = 0;  //to keep track of the index in the answer struct
    while(low <= high){
        strcpy((*answer + i)->term, (terms + low)->term);  //terms is the address of the first term; +i to keep going
        (*answer + i)->weight = (terms + low)->weight;   //terms = head
        // printf("city: %s\n", (*answer + i)->term);
        // printf("city weight: %f\n", (*answer + i)->weight); 
        low++;
        i++;
    }
    //sort by weight - use qsort but this time with a compare function that does integers
    qsort(*answer, *n_answer, sizeof(struct term), compare_weights);
    printf("first city after sorting: %s\n", (*answer)->term); 
    printf("first city's weight after sorting: %f\n", (*answer)->weight);  
    printf("last city after sorting: %s\n", (*answer + *n_answer - 1)->term);
    printf("last city's weight after sorting: %f\n", (*answer + *n_answer - 1)->weight);
}