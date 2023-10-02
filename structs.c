#include "raylib.h"
#include "raymath.h"

const float gravity = 4.0f;

#define MAX_TEXTURES_IN_MEMORY 128
#define MAX_ANIMATIONS_PER_ENTITY 6
#define MAX_ANIMATIONS_PER_OBJECT 1

Texture2D textures[MAX_TEXTURES_IN_MEMORY];

typedef enum  
{
    TEXTURE_PLAYER_IDLE,
    TEXTURE_PLAYER_JUMP,
    TEXTURE_PLAYER_RUNNING_RIGHT,
    TEXTURE_PLAYER_RUNNING_LEFT,
    TEXTURE_PLAYER_ATTACK,
    TEXTURE_PLAYER_ATTACK_BALL,
    TEXTURE_BOSS_GLOBE,
    TEXTURE_PLATFORM,
    TEXTURE_PORTAL_GLITCH,
    TEXTURE_BACKGROUND,
    TEXTURE_NUM
} textures_e;

typedef struct
{
    textures_e texture_id;
    int current_frame;
    int max_frame;
    float frame_duration;
    float timer;
} Animation;

typedef struct
{
    textures_e texture_id;
    Rectangle rect;
} StaticObject;

typedef struct
{
    Rectangle rect;
    Animation animations[MAX_ANIMATIONS_PER_OBJECT];
    int current_animation;
} InteractableObject;

typedef struct
{
    float velocity;
    bool jump;
    bool isJumping;
    Rectangle rect;
    Animation animations[MAX_ANIMATIONS_PER_ENTITY];
    int current_animation;
} Entity;
