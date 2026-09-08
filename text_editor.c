#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINES 100
#define MAX_LENGTH 200

/* 
   The document is stored as an array of strings.

   lines[0] = first line
   lines[1] = second line
   lines[2] = third line
   etc.
*/
char lines[MAX_LINES][MAX_LENGTH];

/* Number of lines currently in the document */
int lineCount = 0;


/* Function declarations */
void insertLine();
void deleteLine();
void displayDocument();
void saveFile();
void loadFile();
void showHelp();


int main()
{
    char command[300];

    printf("====================================\n");
    printf("        SIMPLE LINE EDITOR\n");
    printf("====================================\n");

    printf("Type 'help' to see the commands.\n");

    while (1)
    {
        printf("\n> ");

        /*
           Read the complete command typed by the user.
           
           Example:
           insert 1 Hello World
        */
        fgets(command, sizeof(command), stdin);

        /* Remove the '\n' added by fgets */
        command[strcspn(command, "\n")] = '\0';


        /* ---------------- HELP ---------------- */

        if (strcmp(command, "help") == 0)
        {
            showHelp();
        }


        /* ---------------- DISPLAY ---------------- */

        else if (strcmp(command, "display") == 0)
        {
            displayDocument();
        }


        /* ---------------- INSERT ---------------- */

        else if (strncmp(command, "insert ", 7) == 0)
        {
            insertLine(command);
        }


        /* ---------------- DELETE ---------------- */

        else if (strncmp(command, "delete ", 7) == 0)
        {
            deleteLine(command);
        }


        /* ---------------- SAVE ---------------- */

        else if (strncmp(command, "save ", 5) == 0)
        {
            saveFile(command);
        }


        /* ---------------- LOAD ---------------- */

        else if (strncmp(command, "load ", 5) == 0)
        {
            loadFile(command);
        }


        /* ---------------- EXIT ---------------- */

        else if (strcmp(command, "exit") == 0)
        {
            printf("Exiting editor...\n");
            break;
        }


        /* ---------------- INVALID COMMAND ---------------- */

        else if (strlen(command) == 0)
        {
            /* Do nothing if the user only presses ENTER */
        }
        else
        {
            printf("Invalid command.\n");
            printf("Type 'help' to see available commands.\n");
        }
    }

    return 0;
}


/* ==========================================================
   INSERT LINE
   ========================================================== */

void insertLine(char command[])
{
    int position;
    char text[MAX_LENGTH];


    /*
       Read the line number and the text.

       Example:
       insert 2 Hello World

       position = 2
       text = "Hello World"
    */
    if (sscanf(command, "insert %d %[^\n]", &position, text) != 2)
    {
        printf("Invalid insert command.\n");
        printf("Use: insert <line number> <text>\n");
        return;
    }


    /*
       We can insert between 1 and lineCount + 1.

       Example:

       If there are 3 lines:

       1
       2
       3

       We can insert at:

       1 -> beginning
       2 -> middle
       3 -> middle
       4 -> end
    */
    if (position < 1 || position > lineCount + 1)
    {
        printf("Invalid line number.\n");
        return;
    }


    /* Check whether the document is full */
    if (lineCount >= MAX_LINES)
    {
        printf("Document is full.\n");
        return;
    }


    /*
       Convert the user's line number to an array index.

       User line 1 = array index 0
       User line 2 = array index 1

       So position - 1 is used.
    */
    position--;


    /*
       Shift existing lines DOWN.

       Example:

       Before:

       1. Hello
       2. World
       3. Bye

       insert 2 Beautiful

       We move:

       Bye   -> line 4
       World -> line 3

       Then put Beautiful at line 2.
    */
    for (int i = lineCount; i > position; i--)
    {
        strcpy(lines[i], lines[i - 1]);
    }


    /* Put the new text into the empty position */
    strcpy(lines[position], text);


    /* Increase the number of lines */
    lineCount++;


    printf("Line inserted successfully.\n");
}


/* ==========================================================
   DELETE LINE
   ========================================================== */

void deleteLine(char command[])
{
    int position;


    /*
       Get the line number.

       Example:

       delete 2

       position = 2
    */
    if (sscanf(command, "delete %d", &position) != 1)
    {
        printf("Invalid delete command.\n");
        printf("Use: delete <line number>\n");
        return;
    }


    /* Cannot delete if there are no lines */
    if (lineCount == 0)
    {
        printf("Document is empty.\n");
        return;
    }


    /* Check whether the line exists */
    if (position < 1 || position > lineCount)
    {
        printf("Invalid line number.\n");
        return;
    }


    /*
       Convert line number to array index.

       User:
       line 1

       Array:
       index 0
    */
    position--;


    /*
       Shift all lines AFTER the deleted line UP.

       Example:

       Before:

       1. Hello
       2. Beautiful
       3. World

       delete 2

       World moves from line 3 to line 2.
    */
    for (int i = position; i < lineCount - 1; i++)
    {
        strcpy(lines[i], lines[i + 1]);
    }


    /* One line has been removed */
    lineCount--;


    printf("Line deleted successfully.\n");
}


/* ==========================================================
   DISPLAY DOCUMENT
   ========================================================== */

void displayDocument()
{
    /* Check if there are no lines */
    if (lineCount == 0)
    {
        printf("Document is empty.\n");
        return;
    }


    printf("\n---------- DOCUMENT ----------\n");


    /*
       Print every line.

       Array index starts at 0,
       but the user sees line numbers starting at 1.

       Therefore we print i + 1.
    */
    for (int i = 0; i < lineCount; i++)
    {
        printf("%d. %s\n", i + 1, lines[i]);
    }


    printf("------------------------------\n");
}


/* ==========================================================
   SAVE FILE
   ========================================================== */

void saveFile(char command[])
{
    char filename[100];
    FILE *file;


    /*
       Get the filename.

       Example:

       save document.txt

       filename = document.txt
    */
    if (sscanf(command, "save %99s", filename) != 1)
    {
        printf("Use: save <filename>\n");
        return;
    }


    /*
       Open the file in write mode.

       "w" means:
       - create the file if it doesn't exist
       - overwrite it if it already exists
    */
    file = fopen(filename, "w");


    if (file == NULL)
    {
        printf("Could not open file.\n");
        return;
    }


    /*
       Write every line into the file.
    */
    for (int i = 0; i < lineCount; i++)
    {
        fprintf(file, "%s\n", lines[i]);
    }


    /* Close the file */
    fclose(file);


    printf("File saved successfully.\n");
}


/* ==========================================================
   LOAD FILE
   ========================================================== */

void loadFile(char command[])
{
    char filename[100];
    FILE *file;


    /*
       Get the filename.

       Example:

       load document.txt
    */
    if (sscanf(command, "load %99s", filename) != 1)
    {
        printf("Use: load <filename>\n");
        return;
    }


    /*
       Open the file in read mode.
    */
    file = fopen(filename, "r");


    if (file == NULL)
    {
        printf("Could not open file.\n");
        return;
    }


    /*
       Start with an empty document.
    */
    lineCount = 0;


    /*
       Read one line at a time.
    */
    while (lineCount < MAX_LINES &&
           fgets(lines[lineCount], MAX_LENGTH, file) != NULL)
    {
        /*
           Remove the newline character.
        */
        lines[lineCount][strcspn(lines[lineCount], "\n")] = '\0';


        /* Move to the next line */
        lineCount++;
    }


    fclose(file);


    printf("File loaded successfully.\n");
}


/* ==========================================================
   HELP
   ========================================================== */

void showHelp()
{
    printf("\n========== HELP ==========\n");

    printf("\ninsert <line number> <text>\n");
    printf("Insert a new line.\n");
    printf("Example: insert 1 Hello World\n");

    printf("\ndelete <line number>\n");
    printf("Delete a line.\n");
    printf("Example: delete 1\n");

    printf("\ndisplay\n");
    printf("Display the complete document.\n");
    printf("Example: display\n");

    printf("\nsave <filename>\n");
    printf("Save the document to a text file.\n");
    printf("Example: save document.txt\n");

    printf("\nload <filename>\n");
    printf("Load a text file into the editor.\n");
    printf("Example: load document.txt\n");

    printf("\nhelp\n");
    printf("Display the available commands.\n");

    printf("\nexit\n");
    printf("Exit the editor.\n");

    printf("\n==========================\n");
}
