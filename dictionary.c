#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structure to store dictionary entries
struct Dictionary {
    char word[100];
    char meaning[250];
} dictionary[1000];

// Global counter for dictionary entries
int countwords = 0;

// Function prototypes
void load_dictionary();
int validate_word(const char *word);
void search_word();
void insert_word();
void delete_word();

int main() {
    load_dictionary();
    printf("Simple Dictionary\n");

    while (1) {
        int choice;
        printf("\nMenu:\n");
        printf("1. Search Word\n");
        printf("2. Insert Word\n");
        printf("3. Delete Word\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        // Validate integer input
        while (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Error: Please enter a valid number.\nEnter your choice: ");
        }
        getchar(); // Clear newline

        switch (choice) {
            case 1:
                search_word();
                break;
            case 2:
                insert_word();
                break;
            case 3:
                delete_word();
                break;
            case 4:
                printf("Exiting the program.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    return 0;
}

// Load dictionary from CSV file
void load_dictionary() {
    FILE *fp = fopen("dictionary.csv", "r");
    if (!fp) {
        printf("Error: Could not open dictionary.csv. Creating a new file.\n");
        fp = fopen("dictionary.csv", "w");
        if (!fp) {
            printf("Error: Could not create dictionary.csv.\n");
            exit(1);
        }
        fclose(fp);
        return;
    }

    char line[350];
    countwords = 0;
    while (fgets(line, sizeof(line), fp) && countwords < 1000) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = '\0';
        if (sscanf(line, "%[^,],%[^\n]", dictionary[countwords].word, dictionary[countwords].meaning) == 2) {
            countwords++;
        } else {
            printf("Warning: Skipping malformed line: %s\n", line);
        }
    }

    fclose(fp);
    if (countwords == 1000) {
        printf("Warning: Dictionary is full (1000 entries).\n");
    }
}

// Validate word input (alphabetic, no spaces, reasonable length)
int validate_word(const char *word) {
    if (strlen(word) < 1 || strlen(word) >= 100) {
        printf("Error: Word must be 1-99 characters long.\n");
        return 0;
    }
    for (int i = 0; word[i]; i++) {
        if (!isalpha(word[i])) {
            printf("Error: Word must contain only alphabetic characters.\n");
            return 0;
        }
    }
    return 1;
}

// Search for a word in the dictionary
void search_word() {
    if (countwords == 0) {
        printf("No words in the dictionary.\n");
        return;
    }

    char word[100];
    printf("Enter the word to search: ");
    if (scanf(" %99[^\n]", word) != 1) {
        while (getchar() != '\n');
        printf("Error: Invalid input.\n");
        return;
    }
    getchar(); // Clear newline

    if (!validate_word(word)) {
        return;
    }

    for (int i = 0; i < countwords; i++) {
        if (strcasecmp(word, dictionary[i].word) == 0) {
            printf("Word found!\n");
            printf("Word: %s\nMeaning: %s\n", dictionary[i].word, dictionary[i].meaning);
            return;
        }
    }
    printf("Word '%s' not found.\n", word);
}

// Insert a new word and meaning
void insert_word() {
    if (countwords >= 1000) {
        printf("Error: Dictionary is full.\n");
        return;
    }

    char word[100], meaning[250];
    printf("Enter the word to insert: ");
    if (scanf(" %99[^\n]", word) != 1) {
        while (getchar() != '\n');
        printf("Error: Invalid input.\n");
        return;
    }
    getchar(); // Clear newline

    if (!validate_word(word)) {
        return;
    }

    // Check for duplicate word
    for (int i = 0; i < countwords; i++) {
        if (strcasecmp(word, dictionary[i].word) == 0) {
            printf("Error: Word '%s' already exists.\n", word);
            return;
        }
    }

    printf("Enter the meaning: ");
    if (scanf(" %249[^\n]", meaning) != 1) {
        while (getchar() != '\n');
        printf("Error: Invalid input.\n");
        return;
    }
    getchar(); // Clear newline

    // Sanitize meaning to prevent CSV issues
    for (int i = 0; meaning[i]; i++) {
        if (meaning[i] == ',') {
            meaning[i] = ';'; // Replace commas to avoid CSV parsing issues
        }
    }

    // Append to file
    FILE *fp = fopen("dictionary.csv", "a");
    if (!fp) {
        printf("Error: Could not open dictionary.csv for writing.\n");
        return;
    }
    fprintf(fp, "%s,%s\n", word, meaning);
    fclose(fp);

    // Add to in-memory dictionary
    strcpy(dictionary[countwords].word, word);
    strcpy(dictionary[countwords].meaning, meaning);
    countwords++;
    printf("Word '%s' added to the dictionary.\n", word);
}

// Delete a word from the dictionary
void delete_word() {
    if (countwords == 0) {
        printf("No words to delete.\n");
        return;
    }

    char word[100];
    printf("Enter the word to delete: ");
    if (scanf(" %99[^\n]", word) != 1) {
        while (getchar() != '\n');
        printf("Error: Invalid input.\n");
        return;
    }
    getchar(); // Clear newline

    if (!validate_word(word)) {
        return;
    }

    FILE *temp_fp = fopen("temp.csv", "w");
    if (!temp_fp) {
        printf("Error: Could not create temporary file.\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < countwords; i++) {
        if (strcasecmp(dictionary[i].word, word) == 0) {
            found = 1;
        } else {
            fprintf(temp_fp, "%s,%s\n", dictionary[i].word, dictionary[i].meaning);
        }
    }

    fclose(temp_fp);

    if (found) {
        remove("dictionary.csv");
        rename("temp.csv", "dictionary.csv");
        printf("Word '%s' deleted from the dictionary.\n", word);
        load_dictionary(); // Reload to update in-memory dictionary
    } else {
        remove("temp.csv");
        printf("Word '%s' not found in the dictionary.\n", word);
    }
}
