#include "raylib.h"
#include "raymath.h"
#include "structs.c"

// Internal varaibles
bool win = false;
const int screenWidth = 1280;
const int screenHeight = 720;
bool isRunning = false;
int fromPlayerPosX = 0;

// Load all the textures from sprite folder
void LoadAllTextures()
{
    // Players sprites
    textures[TEXTURE_PLAYER_IDLE] = LoadTexture("sprites/playerCube.png");
    textures[TEXTURE_PLAYER_JUMP] = LoadTexture("sprites/playerCubeJump.png");
    textures[TEXTURE_PLAYER_RUNNING_RIGHT] = LoadTexture("sprites/playerCubeRunningRight.png");
    textures[TEXTURE_PLAYER_RUNNING_LEFT] = LoadTexture("sprites/playerCubeRunningLeft.png");
    textures[TEXTURE_PLAYER_ATTACK] = LoadTexture("sprites/playerCubeAttack.png");
    textures[TEXTURE_PLAYER_ATTACK_BALL] = LoadTexture("sprites/playerCubeAttackBall.png");

    // Boss & Enemy sprites
    textures[TEXTURE_BOSS_GLOBE] = LoadTexture("sprites/bossGlobe.png");

    // Object sprites
    textures[TEXTURE_PLATFORM] = LoadTexture("sprites/platformMain.png");
    textures[TEXTURE_PORTAL_GLITCH] = LoadTexture("sprites/portalGlitch.png");
    textures[TEXTURE_BACKGROUND] = LoadTexture("sprites/background.png");
}

int main()
{
    InitWindow(screenWidth, screenHeight, "Main window");
    SetTargetFPS(60);  // Sets the framerate to 60
    LoadAllTextures(); // Call the "LoadAllTextures" function

    // Player initialise
    Entity player = (Entity){
        .animations = {
            (Animation){
                .texture_id = TEXTURE_PLAYER_IDLE,
                .current_frame = 0,
                .max_frame = 10,
                .frame_duration = 0.1f,
                .timer = 0,
            },
            (Animation){
                .texture_id = TEXTURE_PLAYER_RUNNING_RIGHT,
                .current_frame = 0,
                .max_frame = 8,
                .frame_duration = 0.1f,
                .timer = 0,
            },
            (Animation){
                .texture_id = TEXTURE_PLAYER_RUNNING_LEFT,
                .current_frame = 0,
                .max_frame = 8,
                .frame_duration = 0.1f,
                .timer = 0,
            },
            (Animation){
                .texture_id = TEXTURE_PLAYER_JUMP,
                .current_frame = 0,
                .max_frame = 11,
                .frame_duration = 0.1f,
                .timer = 0,
            },
            (Animation){
                .texture_id = TEXTURE_PLAYER_ATTACK,
                .current_frame = 0,
                .max_frame = 5,
                .frame_duration = 0.1f,
                .timer = 0,
            }},
        .rect = (Rectangle){screenWidth / 2, screenHeight / 2, 150, 150},
        .jump = false,
        .isJumping = false,
        .velocity = 0,
        .current_animation = 0};

    // Boss "Globe" initialise
    Entity bossGlobe = (Entity){
        .animations = {
            (Animation){
                .texture_id = TEXTURE_BOSS_GLOBE,
                .current_frame = 0,
                .max_frame = 1,
                .frame_duration = 0.0f,
                .timer = 0,
            }},
        .rect = (Rectangle){500, screenHeight - 570, 400, 400},
        .current_animation = 0};

    // Platform initialise
    StaticObject platform = (StaticObject){
        .texture_id = TEXTURE_PLATFORM,
        .rect = (Rectangle){
            0,
            screenHeight - 170,
            1280,
            170}};

    // Background
    StaticObject background = (StaticObject){
        .texture_id = TEXTURE_BACKGROUND};

    // Attack ball
    InteractableObject attackBall = (InteractableObject){
        .animations = {
            (Animation){
                .texture_id = TEXTURE_PLAYER_ATTACK_BALL,
                .current_frame = 0,
                .max_frame = 8,
                .frame_duration = 0.1f,
                .timer = 0,
            }},
        .rect = (Rectangle){1000 + fromPlayerPosX, screenHeight - 200 - 170, 64, 64},
        .current_animation = 0};

    // Portal initialise
    InteractableObject portal = (InteractableObject){
        .animations = {
            (Animation){
                .texture_id = TEXTURE_PORTAL_GLITCH,
                .current_frame = 0,
                .max_frame = 15,
                .frame_duration = 0.1f,
                .timer = 0,
            }},
        .rect = (Rectangle){1300 + fromPlayerPosX, screenHeight - 200 - 170, 200, 200},
        .current_animation = 0};

    // Sets the current frame time to 0
    float currentFrameTime = 0.0f;

    while (!WindowShouldClose())
    {
        //----------------------------------------------------
        // Update
        //----------------------------------------------------

        // Update necessary animation (player, portal, attack):
        {
            // Player
            Animation *player_animation = &player.animations[player.current_animation];
            player_animation->timer += GetFrameTime();
            if (player_animation->timer >= player_animation->frame_duration)
            {
                player_animation->current_frame++;
                if (player_animation->current_frame > player_animation->max_frame)
                {
                    player_animation->current_frame = 0;
                }
                player_animation->timer = 0.0f;
            }

            // Portal
            Animation *portal_animation = &portal.animations[portal.current_animation];
            portal_animation->timer += GetFrameTime();
            if (portal_animation->timer >= portal_animation->frame_duration)
            {
                portal_animation->current_frame++;
                if (portal_animation->current_frame > portal_animation->max_frame)
                {
                    portal_animation->current_frame = 0;
                }
                portal_animation->timer = 0.0f;
            }

            // Attack ball
            Animation *attackBall_animation = &attackBall.animations[attackBall.current_animation];
            attackBall_animation->timer += GetFrameTime();
            if (attackBall_animation->timer >= attackBall_animation->frame_duration)
            {
                attackBall_animation->current_frame++;
                if (attackBall_animation->current_frame > attackBall_animation->max_frame)
                {
                    attackBall_animation->current_frame = 0;
                }
                attackBall_animation->timer = 0.0f;
            }
        }
        bool attackActive = false;

        // Player move input
        if (IsKeyDown(KEY_RIGHT))
        {
            player.rect.x += 10.0f;
            player.current_animation = 1; // Set animation to running right
            isRunning = true;
        }
        else if (IsKeyDown(KEY_LEFT))
        {
            player.rect.x -= 10.0f;
            player.current_animation = 2; // Set animation to running left
            isRunning = true;
        }
        else
        {
            isRunning = false;
        }

        // Player jump input
        if (IsKeyPressed(KEY_UP))
        {
            if (!player.jump)
            {
                player.jump = true;
                player.velocity = 55.0f;
            }
        }
        if (player.jump)
        {
            player.current_animation = 3; // Set animation to jumping
            player.velocity -= gravity;
            player.rect.y -= player.velocity;
        }

        // Player attack input
        if (IsKeyDown(KEY_SPACE))
        {
            attackActive = true;
            player.current_animation = 4; // Set animation to attack
        }

        // Gravity inflicted to player
        player.rect.y += gravity;

        // Collision detection with player and platform
        if (CheckCollisionRecs(player.rect, platform.rect))
        {
            player.rect.y = platform.rect.y - 150;
            player.jump = false;
            player.velocity = 0;
            if (isRunning == false)
            {
                player.current_animation = 0; // Set animation to idle
            }
        }

        // Hidden collision
        if (player.rect.x > 1000)
        {
            player.rect.x -= 10.0f;
            fromPlayerPosX -= 4;
        }
        else if (player.rect.x < 100)
        {
            player.rect.x += 10.0f;
            fromPlayerPosX += 4;
        }

        //----------------------------------------------------
        // Draw
        //----------------------------------------------------
        BeginDrawing();
        DrawTexture(textures[background.texture_id], background.rect.x, background.rect.y, WHITE);

        // Draw platfrom
        DrawTexture(textures[platform.texture_id], platform.rect.x, platform.rect.y, WHITE);

        // Draw portal
        portal.rect.x = 1300 + fromPlayerPosX;
        DrawTexturePro(textures[portal.animations[portal.current_animation].texture_id], (Rectangle){200 * portal.animations[portal.current_animation].current_frame, 0, 200, 200}, portal.rect, Vector2Zero(), 0, WHITE);

        // Draw boss globe
        DrawTexturePro(textures[bossGlobe.animations[bossGlobe.current_animation].texture_id], (Rectangle){400 * bossGlobe.animations[bossGlobe.current_animation].current_frame, 0, 400, 400}, bossGlobe.rect, Vector2Zero(), 0, WHITE);

        // Draw player
        DrawTexturePro(textures[player.animations[player.current_animation].texture_id], (Rectangle){150 * player.animations[player.current_animation].current_frame, 0, 150, 150}, player.rect, Vector2Zero(), 0, WHITE);

        // Draw players attackBall
        if (attackActive)
        {
            DrawTexturePro(textures[attackBall.animations[attackBall.current_animation].texture_id], (Rectangle){32 * attackBall.animations[attackBall.current_animation].current_frame, 0, 32, 32}, attackBall.rect, Vector2Zero(), 0, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}