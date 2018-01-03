enum Positions {
    PG,  // best pass stat
    SG,  // best shoot stat
    SF,  // best speed stat
    PF,  // best block stat
    C    // best height stat
};

typedef struct stats {
    int _pass;
    int _shoot;
    int _speed;
    int _block;
    int _height;
} stats;

typedef struct Player {
    char * _name;
    enum Positions _pos;
    stats _stats;
    int _age;
    int _shirtNum;
    struct Player * _next;
} Player;

typedef struct Team {
    int _numOfPlayers;
    struct Player * _players[5];
} Team;

// Load all players here
Player * allPlayers;

// Initialize a team with 0 players
Team * initializeTeam();

// Import players into program line-by-line
char * inputFile(FILE * fd);

// Parse string into Player
Player * parsePlayer(char * str);

// Load data into allPlayers
void loadData(FILE * fd);

// Returns the pointer to player
Player * findBestPlayer(int pos);

// Returns number of players in team
int buildBestTeam(Team * t);

// Free the allocated blocks
void freePlayer(Player * p);
void freeTeam(Team * t);

// Print the best team
void print(Team * t);
