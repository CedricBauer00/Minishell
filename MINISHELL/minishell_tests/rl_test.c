#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int main() {
    char *input;

    while (1) {
        input = readline(">> ");

        if (input == NULL) {
            // Bei Ctrl+D:
            // Ersetze die Zeile mit "exit" und lasse den Prompt weg
            rl_replace_line("exit", 0);      // Ersetze Eingabe durch "exit"
            rl_point = strlen("exit");       // Setze den Cursor ans Ende
            rl_on_new_line();                // Markiere als neue Eingabezeile
            rl_redisplay();                  // Zeichne den Prompt neu ohne >> 
            printf("\n");                    // Zeilenumbruch nach exit
            break;                           // Beende das Programm
        }

        if (*input) {
            add_history(input); // Falls eine Eingabe vorhanden ist, speichere sie in der History
        }

        free(input); // Gebe den Speicher der Eingabe frei
    }

    return 0;
}