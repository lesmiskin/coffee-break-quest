extern void pollInput(void);
extern void processSystemCommands(void);

typedef enum {
    CMD_QUIT = 0,
    CMD_PLAYER_DRINK = 1,
    CMD_PLAYER_BATHROOM = 2
} Command;
