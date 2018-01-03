#include <stdio.h>
#include "best_team.h"

int main() {
    Team * t = initializeTeam();
    
    char filename[10];
    printf("Enter the filename to be imported:\n");
    scanf("%s", filename);
    printf("Loading team from %s . . .\n", filename);
    FILE * fd = fopen(filename, "r");
    loadData(fd);
    fclose(fd);

    if (allPlayers == NULL) {
        printf("Error loading file!\n");
        freeTeam(t);
        return -1;
    } else {
        printf("File successfully loaded to allPlayers!\n");
    }

    int players = buildBestTeam(t);

    if (players == t->_numOfPlayers) {
        printf("Best team is built! Printing . . .\n");
    }

    print(t);
    freeTeam(t);

    return 0;
}
