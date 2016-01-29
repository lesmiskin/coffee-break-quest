void pollInput(void);
void processSystemCommands(void);

typedef enum {
    CMD_QUIT = 0,
    CMD_PLAYER_UP = 1,
    CMD_PLAYER_DOWN = 2,
    CMD_PLAYER_LEFT = 3,
    CMD_PLAYER_RIGHT = 4,
    CMD_PLAYER_FIRE = 5,
} Command;
