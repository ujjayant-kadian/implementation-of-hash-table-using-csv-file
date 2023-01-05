#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING_SIZE 200 //max length of a string
#define ARRAY_SIZE 99991  //best be prime


int number_of_collisions = 0;
int number_of_terms = 0;

//Structure for storing each fields data except surname
typedef struct People People;
struct People{
    int person_id;
    char deposition_id[MAX_STRING_SIZE];
    char forename[MAX_STRING_SIZE];
    int age;
    char person_type[MAX_STRING_SIZE];
    char gender[MAX_STRING_SIZE];
    char nationality[MAX_STRING_SIZE];
    char religion[MAX_STRING_SIZE];
    char occupation[MAX_STRING_SIZE];
};

//Linked List for a particular surname (instead of count)
typedef struct PeopleList PeopleList;
struct PeopleList{
    People data;
    PeopleList *next;
};

typedef struct Element Element;
struct Element{
    char surname[MAX_STRING_SIZE];
    PeopleList *list;
};

//To append linked list and copy the data
void add_node_people(PeopleList **list, People *data){
    PeopleList *newNode = (PeopleList *)malloc(sizeof(PeopleList));
    newNode->data.person_id = data->person_id;
    strcpy(newNode->data.deposition_id, data->deposition_id);
    strcpy(newNode->data.forename, data->forename);
    newNode->data.age = data->age;
    strcpy(newNode->data.person_type, data->person_type);
    strcpy(newNode->data.gender, data->gender);
    strcpy(newNode->data.nationality, data->nationality);
    strcpy(newNode->data.religion, data->religion);
    strcpy(newNode->data.occupation, data->occupation);
    newNode->next = *list;
    *list = newNode;
}

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

// Creating a new entry in the table and initialising it - using the head inside Element
Element* createNewElement(char* surname, People *data){
    Element *new = (Element *)malloc(sizeof(Element));
    strcpy(new->surname, surname);
    new->list = NULL;
    add_node_people(&new->list, data);
    return new;
}

//Search- same as before
// returns the element with name name or NULL if the element is not present
Element* search (char* surname){
    int index = hash_function(surname);
    int iteration = 0;
    for (int i = 0; i < ARRAY_SIZE; i++){
        //Linear Probing
        iteration = (i + index) % ARRAY_SIZE;
        if (hashTable[iteration] != NULL && strcmp(hashTable[iteration]->surname, surname) == 0)
        {
            return hashTable[iteration];
        }
   }
   return NULL;
}
//Insert same as before, just passing the struct People as arguement
void insert(char* surname, People *data){
    int index = hash_function(surname);
    int i = 0;
    int iteration = 0;
    for (i = 0; i < ARRAY_SIZE; i++) {
        // Linear Probing
        iteration = (i + index) % ARRAY_SIZE;
        if(hashTable[iteration] == NULL) {
            hashTable[iteration] = createNewElement(surname, data);
            number_of_terms++;
            break;
        }
        else {
            number_of_collisions++;
        }
    }
}

 //searches the name in the array, if it is there increment its count, if not, add it
void addOrIncrement(char* name, People *data){
    Element *search_result = search(name);
    //Adding
    if(search_result == NULL){
        insert(name, data);
    } else { //Incrementing - appending the linked list
        add_node_people(&search_result->list, data);
    }
}

// void print_list (PeopleList* list){
//     PeopleList* node = list;
//     while(node!=NULL){
//         printf("Node: %s\n", node->data.forename);
//         node = node ->next;

//     }
// }

//To print linked list
void print_people(PeopleList* list, char *name){
    PeopleList* node = list;
    printf("%15s%15s%15s%15s%15s%15s%15s%15s%15s%15s\n", "Person ID", "Depositon ID", "Surname", "Forename", "Age", "Person Type", "Gender", "Nationality", "Religion", "Occupation");
    while(node!=NULL){
        printf("%15d%15s%15s%15s%15d%15s%15s%15s%15s%15s\n", node->data.person_id, node->data.deposition_id, name, node->data.forename, node->data.age, node->data.person_type, node->data.gender, node->data.nationality, node->data.religion, node->data.occupation);
        node = node -> next;
    }
}

// prints the list of people with a particular surname
void printNumberOfOccurences(char* name){
	Element *search_result = search(name);
    if(search_result == NULL){
        printf("%s not in table\n", name);
    }
    else 
    {
        print_people(search_result->list, name);
    }
	// printf("%s - %i \n", name, count);
}

// function from the assignment 0
// Reads strings of alpha numeric characters from input file. Truncates strings which are too long to string_max-1
// void next_token ( char *buf, FILE *f, int string_max ) {
// 	// start by skipping any characters we're not interested in
//     int inQuotes = 0;
// 	buf[0] = fgetc(f);
// 	while ( !isalnum(buf[0]) && !feof(f)) {
//         if(buf[0] == '"') inQuotes = 1;
//         buf[0] = fgetc(f);
//         if( buf[0] == ' ' || buf[0] == ':' || buf[0] == '{' || buf[0] == '}' || buf[0] == '*' || buf[0] == '[' || buf[0] == ']' || buf[0] == '&' || buf[0] == '(' || buf[0]== ')' || buf[0] == '-' || buf[0] == '=' || buf[0] == '\'' || buf[0] == '.') {
//             break;
//     }
//     }
// 	// read string of alphanumeric characters
// 	int i=1;
//     int comma_inQuotes = 0;
// 	for (;;) {
//         int condition = 0;
// 		buf[i] = fgetc(f); 
//         if(buf[i] == '"') inQuotes = 0;               // get next character from file
//         if( buf[i] == ' ' || buf[i] == ':' || buf[i] == '{' || buf[i] == '}' || buf[i] == '*' || buf[i] == '[' || buf[i] == ']' || buf[i] == '&' || buf[i] == '(' || buf[i]== ')' || buf[i] == '-' || buf[i] == '=' || buf[i] == '\'' || buf[i] == '.') {
//             condition = 1;
//         }
//         if (buf[i] == ',' && inQuotes) comma_inQuotes = 1;
// 		if(!isalnum(buf[i]) && !condition && !comma_inQuotes) { break; } // only load letters and numbers
// 		if( feof(f) ) { break; }          // file ended?
// 		if( i < (string_max-1) ) { ++i; } // truncate strings that are too long
// 	}
// 	buf[i] = '\0'; // NULL terminate the string
// }

void next_token ( char *buf, FILE *f, int string_max ) {
	// start by skipping any characters we're not interested in
    int bufferpos = 0;
    int inQuotes = 0;
    char c;
    for(int i = 0; i < string_max; i++){
        c = fgetc(f);
        if (c == '"'){
            inQuotes = !inQuotes;
            continue;
        }
        if (c == ',' && !inQuotes){
            bufferpos++;
            break;
        }
        else if(c == '\n'){
            bufferpos++;
            break;
        }
        buf[bufferpos] = c;
        bufferpos++;
    }
    buf[bufferpos] = '\0';
}


//  Reads the contents of a file and adds them to the hash table - returns 1 if file was successfully read and 0 if not.

int load_file ( char *fname ) {
	FILE *f;
	char buf[MAX_STRING_SIZE];
    int field_count = 0;
    People *data = (People *)malloc(sizeof(People));
    char surname[MAX_STRING_SIZE];
	// boiler plate code to ensure we can open the file
	f = fopen(fname, "r");
	if (!f) { 
		printf("Unable to open %s\n", fname);
		return 0; 
	}
	printf("File %s loaded\n", fname);
	// read until the end of the file
	while ( !feof(f) ) {
        memset(buf, 0, sizeof(buf));
        next_token(buf, f, MAX_STRING_SIZE);
        // printf("%s\n", buf);
        if(field_count == 0) {
            data->person_id = atoi(buf);
            // printf("%d\n", data->person_id);
        }
        else if(field_count == 1) {
            strcpy(data->deposition_id, buf);
            // printf("%s\n", data->deposition_id);
        }
        else if(field_count == 2) {
            strcpy(surname, buf);
            // printf("%s\n", surname);
        }
        else if(field_count == 3) {
            strcpy(data->forename,buf);
            // printf("%s\n", data->forename);
        }
        else if(field_count == 4) {
            data->age = atoi(buf);
            // printf("%d\n", data->age);
        }
        else if(field_count == 5) {
            strcpy(data->person_type,buf);
            // printf("%s\n", data->person_type);
        }
        else if(field_count == 6) {
            strcpy(data->gender,buf);
            // printf("%s\n", data->gender);
        }
        else if(field_count == 7) {
            strcpy(data->nationality,buf);
            // printf("%s\n", data->nationality);
        }
        else if(field_count == 8) {
            strcpy(data->religion,buf);
            // printf("%s\n", data->religion);
        }
        else if(field_count == 9) {
            strcpy(data->occupation,buf);
            // printf("%s\n", data->occupation);
        }
        field_count++;
        if (field_count == 10){
            field_count = 0;
            // printf("%d\n", data->person_id);
            // printf("%s\n", data->deposition_id);
            // printf("%s\n", surname);
            // printf("%s\n", data->forename);
            // printf("%d\n", data->age);
            // printf("%s\n", data->person_type);
            // printf("%s\n", data->gender);
            // printf("%s\n", data->nationality);
            // printf("%s\n", data->religion);
            // printf("%s\n", data->occupation);
            // add_node_people(&new, data);
            if(strcmp(surname, "Surname") == 0) continue;
            addOrIncrement(surname, data);
        }
    }
	// always remember to close your file stream
    // print_list(new);
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
    // for(int i = 0; i < ARRAY_SIZE; i++ ){
    //     if(hashTable[i] != NULL)
    //     printf("%s\n", hashTable[i]->surname);
    // }

    // People data1, data2, data3;
    // data1.person_id = 1;
    // strcpy(data1.deposition_id,"1241f");
    // strcpy(data1.forename,"1241f");
    // strcpy(data1.deposition_id,"1241f");


    
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
