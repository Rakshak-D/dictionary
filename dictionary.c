#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//a structure to represent a dictionary 
typedef struct 
{
    char word[100];
    char meaning[250];
} Dictionary;

// to search for a word in the file
void searchword(const char *word)
 {
    FILE *file = fopen("dictionary.csv", "r");
    if (!file) 
    {
        printf("Error: Could not open file dictionary.csv.\n");
        return;
    }

    Dictionary d;
    int found = 0;

    printf("Searching for word: %s\n", word); 

    while (fscanf(file, "%99[^,],%[^\n]\n", d.word, d.meaning) == 2)
     {
        printf("Reading word: %s\n", d.word); 
        if (strcmp(d.word, word) == 0) 
        {
            printf("Word found!\n"); 
            printf("Word: %s, Meaning: %s\n", d.word, d.meaning);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("The word '%s' was not found in the dictionary.\n", word);
    }

    fclose(file);
}
//  to insert a new word into the file
void insertword(const char *word, const char *meaning)
{
    FILE *file = fopen("dictionary.csv", "a");
    if (!file)
    {
        printf("Error: Could not open file dictionary.csv.\n"); 
        return;
    }

    fprintf(file, "%s,%s\n", word, meaning);
    printf("Word '%s' has been added to the dictionary.\n", word);
    fclose(file);
}

// to delete a word from the file
void deleteword(const char *word)
{
     FILE *file = fopen("dictionary.csv", "r");
    if (!file)
{
    printf("Error: Could not open file dictionary.csv.\n"); 
     return;
}
 FILE *temp = fopen("temp.csv", "w");
     if (!temp)
{
    printf("Error: Could not create temporary file.\n");
    fclose(file);
     return;
 }

 Dictionary d;
 int found = 0;

 while (fscanf(file, "%99[^,],%[^\n]\n", d.word, d.meaning) == 2) 
 {
    if (strcmp(d.word, word) == 0) 
     {
         found = 1; 
    }
 else {
         fprintf(temp, "%s,%s\n", d.word, d.meaning);
        }
    }

    fclose(file);
    fclose(temp);

    if (found) 
    {
    remove("dictionary.csv");
     rename("temp.csv", "dictionary.csv");
     printf("Word '%s' has been deleted from the dictionary.\n", word);
     } 
else
 {
     printf("The word '%s' was not found in the dictionary.\n", word);
     remove("temp.csv");
 }
}

int main() 
{
     int ch;
    char word[100], meaning[250];

    printf("Simple Dictionary\n");

    while (1) 
{
     printf("\nMenu:\n");
     printf("1. Search Word\n");
     printf("2. Insert Word\n");
     printf("3. Delete Word\n");
     printf("4. Exit\n");
     printf("Enter your choice: ");
     scanf("%d", &ch);

switch (ch) 
{
    case 1:
    printf("Enter the word to search: ");
    scanf("%s", word);
    searchword(word);
    break;  
    case 2:
     printf("Enter the word to insert: ");
     scanf("%s", word);
     printf("Enter the meaning of '%s': ", word);
     scanf(" %s", meaning); 
    insertword(word, meaning);
    break;
    case 3:
    printf("Enter the word to delete: ");
    scanf("%s", word);
    deleteword(word);
    break;
    case 4:
    printf("exiting the program\n");
    exit(0);
    default:
            printf("Invalid choice. Please try again.\n");
        }
     }

     return 0;
}            