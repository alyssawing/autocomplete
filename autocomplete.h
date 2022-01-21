#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

struct term{
    char term[200]; // assume terms are not longer than 200
    double weight;
};

int compare_structs(const void *head1, const void *head2);

int compare_weights(const void *head1, const void *head2);

void qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void*)); //necessary if built in?

void read_in_terms(struct term **terms, int *pnterms, char *filename);  //part 1

int lowest_match(struct term *terms, int nterms, char *substr); //part 2a

int highest_match(struct term *terms, int nterms, char *substr);    //part 2b

void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr);   //part 3

#endif