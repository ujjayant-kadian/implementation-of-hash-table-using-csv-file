#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING_SIZE 20 //max length of a string
#define ARRAY_SIZE 59  //best be prime


int number_of_collisions = 0;
int number_of_terms = 0;

typedef struct Element Element;
struct Element{
    char name[MAX_STRING_SIZE];
    int count;
};

Element* hashTable[ARRAY_SIZE];

void initialising_hash_table(){
    for(int i = 0; i < ARRAY_SIZE; i++){
        hashTable[i] = NULL;
    }
}

int hash_function(char* s){
    int hash = 0;
    while(*s){
        hash = hash + *s;
        s++;
    }
    return (hash % ARRAY_SIZE);
}

unsigned long hash2(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        // hash = ((hash << 5) + hash) + c; 
        hash = hash * 33 + c;

    return (hash % ARRAY_SIZE);
}

int hash3(char* s) {

    int hash = 0;
    while(*s){
      hash = 1+ (hash + *s) % (ARRAY_SIZE-1);
      s++;
    }
    return hash;
}

Element* createNewElement(char* name){
    Element *new = (Element *)malloc(sizeof(Element));
    strcpy(new->name, name);
    new->count = 1;
    return new;
}


// returns the element with name name or NULL if the element is not present
Element* search (char* name){
    int index = hash_function(name);
    int iteration = 0;
    for (int i = 0; i < ARRAY_SIZE; i++){
        //Double Hashing
        iteration = (i * hash3(name) + index) % ARRAY_SIZE;
        if (hashTable[iteration] != NULL && strcmp(hashTable[iteration]->name, name) == 0)
        {
            return hashTable[iteration];
        }
   }
   return NULL;
}

void insert(char* name){
    int index = hash_function(name);
    int i = 0;
    int iteration = 0;
    for (i = 0; i < ARRAY_SIZE; i++) {
        //Double Hashing
        iteration = (i * hash3(name) + index) % ARRAY_SIZE;
        if(hashTable[iteration] == NULL) {
            hashTable[iteration] = createNewElement(name);
            number_of_terms++;
            break;
        }
        else {
            number_of_collisions++;
        }
    }
}

 //searches the name in the array, if it is there increment its count, if not, add it
void addOrIncrement(char* name){
    Element *search_result = search(name);
    //Adding
    if(search_result == NULL){
        insert(name);
    } else { //Incrementing
        search_result->count++;
        // number_of_collisions++;
    }
}


// prints the number of occurences, or 0 if not in the file
void printNumberOfOccurences(char* name){
    int count=0;
	Element *search_result = search(name);
    if(search_result != NULL){
        count = search_result->count;
    }
	printf("%s - %i \n", name, count);
}

// function from the assignment 0
// Reads strings of alpha numeric characters from input file. Truncates strings which are too long to string_max-1
void next_token ( char *buf, FILE *f, int string_max ) {
	// start by skipping any characters we're not interested in
	buf[0] = fgetc(f);
	while ( !isalnum(buf[0]) && !feof(f) ) { buf[0] = fgetc(f); }
	// read string of alphanumeric characters
	int i=1;
	for (;;) {
		buf[i] = fgetc(f);                // get next character from file
		if( !isalnum(buf[i]) && buf[i] != ' ') { break; } // only load letters and numbers
		if( feof(f) ) { break; }          // file ended?
		if( i < (string_max-1) ) { ++i; } // truncate strings that are too long
	}
	buf[i] = '\0'; // NULL terminate the string
}


//  Reads the contents of a file and adds them to the hash table - returns 1 if file was successfully read and 0 if not.
int load_file ( char *fname ) {
	FILE *f;
	char buf[MAX_STRING_SIZE];

	// boiler plate code to ensure we can open the file
	f = fopen(fname, "r");
	if (!f) { 
		printf("Unable to open %s\n", fname);
		return 0; 
	}
	printf("File %s loaded\n", fname);
	// read until the end of the file
	while ( !feof(f) ) {
		next_token(buf, f, MAX_STRING_SIZE);
		addOrIncrement(buf);                           //here you call your function from above!
	}

	// always remember to close your file stream
	fclose(f);

	return 1;
}


int main(int argc, char** argv)
{
    //first test insert and search
    //then move on to addOrIncrement
    //only then worry about loading the file
    //and later again about an interactive interface
    initialising_hash_table();
    // insert("Dun");
    // Element* search_result = search("Dun");
    // printf("Element found: %s\n", search_result->name);
    // addOrIncrement("Dun");
    // addOrIncrement("Dun");
    // Element* search_result_other = search("Dun");
    // printf("%d\n", search_result_other->count);


    load_file(argv[1]);

    printf(" Capacity: %d\n", ARRAY_SIZE);
    printf(" Num Terms: %d\n", number_of_terms);
    printf(" Collisions: %d\n", number_of_collisions);
    float load;
    float capacity = (float)ARRAY_SIZE;
    load = number_of_terms/capacity;
    printf(" Load: %f\n", load);

    char answer[20];
    printf("Enter term to get frequency or type \"quit\" to escape\n");
    for(;;){
        printf(">>> ");
        scanf(" %[^\n]s", answer);
        if(strcmp(answer, "quit")){
            printNumberOfOccurences(answer);
        } else{
            break;
        }
    }
    return 0;
}
