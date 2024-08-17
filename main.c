#include "raylib.h"
#include<stdio.h>
#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 1000

// Enum to represent the current page
typedef enum {
    START_PAGE,
    HOMEPAGE,
    RACETRACK,
    ABOUTPAGE,
    CTRL,
    CREDITSPAGE,
    EXIT,
    RESULT
} Page;
// struct for attributes of cars
typedef struct {
    float posx;
    float posy;
    float fspeed;
    float bspeed;
    float rotate;
    int state;
    int lapcount;
} controls;
//Global variables
unsigned int timer; //for ready steady go!
int laps = 1;
// Functions declaration
void Race(Texture2D* alltexture, controls*, controls*, Sound* audios, unsigned int*); // Racetrack and game logic
void handling(controls*, controls*, Sound*); // Function containing code for movements of cars
void collision(Sound*, controls*, controls*, Rectangle*); // Function to check collisions
bool IsButtonClicked(Rectangle button, Vector2 mousePoint); // Function to check if a button is clicked
void counter(unsigned int*);

int main(void) {
    // Initialization
    controls car1 = { 375,150,135,65,90,0,0 };
    controls car2 = { 375,182,135,65,90,0,0 };
    unsigned int count[4];
    //audio 
    Music music;
    Sound audios[4];
    Texture2D alltexture[10]; //textures
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Top-Down View Racing Game");
    InitAudioDevice();
    //Loading all textures including car sprites and map
    //Capt.
    alltexture[0] = LoadTexture("bg.png");
    alltexture[1] = LoadTexture("CNTRL.png");
    alltexture[2] = LoadTexture("About.png");
    alltexture[3] = LoadTexture("Credits.png");
    alltexture[4] = LoadTexture("Riyal.png");
    alltexture[5] = LoadTexture("Aud.png");
    alltexture[6] = LoadTexture("Sticker.png");
    alltexture[7] = LoadTexture("Vip.png");
    alltexture[8] = LoadTexture("over.png");
    alltexture[9] = LoadTexture("Boost.png");
    // loading music
    music = LoadMusicStream("highoctane.mp3");
    PlayMusicStream(music);
    audios[0] = LoadSound("collision.mp3");
    audios[1] = LoadSound("engine.mp3");
    audios[2] = LoadSound("acc1.mp3");
    audios[3] = LoadSound("acc2.mp3");
    // setting currentPage to Starting page
    Page currentPage = START_PAGE;
    Rectangle backButtonRect = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT - 60, 150, 40 };
    // Main game loop
    SetTargetFPS(40);
    while (!WindowShouldClose()) {
        // Update
        UpdateMusicStream(music);
        Vector2 mousePoint = GetMousePosition();
        if (IsKeyPressed(KEY_P))
            PlayMusicStream(music);
        if (IsKeyPressed(KEY_M))
            StopMusicStream(music);
        BeginDrawing();
        // Draw background
        DrawTexture(alltexture[0], 0, 0, WHITE);
        // Draw buttons based on the current page
        if (currentPage == START_PAGE) {
            DrawTexture(alltexture[6], 450, 180, WHITE); //sticker
            // Draw start page buttons
            Rectangle startButtonRect = { SCREEN_WIDTH / 2 - 220, SCREEN_HEIGHT / 2 + 220, 480, 60 };
            DrawRectangleRec(startButtonRect, BEIGE);
            DrawText("Press Enter to Start", startButtonRect.x + 10, startButtonRect.y + 10, 40, DARKBROWN);
            if (IsKeyPressed(KEY_ENTER)) {
                currentPage = HOMEPAGE;
            }
            // Check if the start button is clicked to switch to the homepage
            if (IsButtonClicked(startButtonRect, mousePoint)) {
                currentPage = HOMEPAGE;
                PlaySound(audios[0]);
            }
        }
        else if (currentPage == HOMEPAGE)
        {
            // Draw homepage buttons
            Rectangle startRaceButtonRect = { 100, 180, 300, 45 };
            Rectangle lapButtonRect = { 100, 260, 260, 45 };
            Rectangle nextButtonRect = { 405, 260, 35, 45 };
            Rectangle lowButtonRect = { 365, 260, 35, 45 };
            Rectangle controlsButtonRect = { 100, 340, 300, 45 };
            Rectangle aboutButtonRect = { 100, 420, 300, 45 };
            Rectangle creditsButtonRect = { 100, 500, 300, 45 };
            Rectangle exitButtonRect = { 100, 580, 300, 45 };
            //reset cars' attributes
            car1.state = 0;
            car2.state = 0;
            car2.posx = 375;
            car2.posy = 182;
            car1.posx = 375;
            car1.posy = 150;
            car1.rotate = 90;
            car2.rotate = 90;
            car1.lapcount = 0;
            car2.lapcount = 0;
            for (int i = 0; i < 4; i++)//resetting time of race
                count[i] = 0;
            timer = 0;
            //Sticker
            DrawTexture(alltexture[6], 700, 150, WHITE);
            // Draw buttons
            DrawRectangleRec(startRaceButtonRect, BEIGE);
            DrawText("Start Race", startRaceButtonRect.x + 10, startRaceButtonRect.y + 10, 30, DARKBROWN);
            //for selecting laps
            DrawRectangleRec(lapButtonRect, BEIGE);
            DrawText("Select Laps : ", lapButtonRect.x + 10, lapButtonRect.y + 10, 30, DARKBROWN);
            DrawText(TextFormat("%d", laps), lapButtonRect.x + 230, lapButtonRect.y + 10, 30, DARKBROWN);
            DrawRectangleRec(nextButtonRect, BEIGE);
            DrawTriangle((Vector2){ 405,270 }, (Vector2) { 405,295 }, (Vector2) { 435,283 }, DARKBROWN);
            DrawRectangleRec(lowButtonRect, BEIGE);
            DrawTriangle((Vector2) { 400,270 }, (Vector2) { 370,283 }, (Vector2) { 400,295 }, DARKBROWN);

            DrawRectangleRec(controlsButtonRect, BEIGE);
            DrawText("Controls", controlsButtonRect.x + 10, controlsButtonRect.y + 10, 30, DARKBROWN);

            DrawRectangleRec(aboutButtonRect, BEIGE);
            DrawText("About", aboutButtonRect.x + 10, aboutButtonRect.y + 10, 30, DARKBROWN);

            DrawRectangleRec(creditsButtonRect, BEIGE);
            DrawText("Credits", creditsButtonRect.x + 10, creditsButtonRect.y + 10, 30, DARKBROWN);
            DrawRectangleRec(exitButtonRect, BEIGE);
            DrawText("Exit", exitButtonRect.x + 10, exitButtonRect.y + 10, 30, DARKBROWN);
            // Check which button is clicked
            if (IsButtonClicked(startRaceButtonRect, mousePoint))
            {
                // Handle Start Race button click
                currentPage = RACETRACK;
                PlaySound(audios[0]);
            }
            //selecting no. of laps
            else if (IsButtonClicked(nextButtonRect, mousePoint))
            {
                PlaySound(audios[0]);
                if (laps >= 1 && laps < 3)
                    laps++;

            }
            else if (IsButtonClicked(lowButtonRect, mousePoint))
            {
                PlaySound(audios[0]);
                if (laps > 1 && laps <= 3)
                    laps--;

            }

            else if (IsButtonClicked(controlsButtonRect, mousePoint))
            {
                // Handle Controls button click
                currentPage = CTRL;
                PlaySound(audios[0]);
            }
            else if (IsButtonClicked(aboutButtonRect, mousePoint))
            {
                PlaySound(audios[0]);
                currentPage = ABOUTPAGE;
                // Handle About button click
            }
            else if (IsButtonClicked(creditsButtonRect, mousePoint))
            {
                PlaySound(audios[0]);
                currentPage = CREDITSPAGE;
                // Handle Credits button click
            }
            else if (IsButtonClicked(exitButtonRect, mousePoint))
            {   // Handle Exit button click
                PlaySound(audios[0]);
                currentPage = EXIT;
            }

        }
        else if (currentPage == CTRL)
        {
            DrawTexture(alltexture[1], 0, 0, WHITE);
            DrawRectangleRec(backButtonRect, BEIGE);
            DrawText("Go Back", backButtonRect.x + 10, backButtonRect.y + 10, 30, DARKBROWN);
            if (IsButtonClicked(backButtonRect, mousePoint))
            {
                PlaySound(audios[0]);
                currentPage = HOMEPAGE;
            }
        }
        else if (currentPage == ABOUTPAGE)
        {
            DrawTexture(alltexture[2], 0, 0, WHITE);
            DrawRectangleRec(backButtonRect, BEIGE);
            DrawText("Go Back", backButtonRect.x + 10, backButtonRect.y + 10, 30, DARKBROWN);
            if (IsButtonClicked(backButtonRect, mousePoint))
            {
                PlaySound(audios[0]);
                currentPage = HOMEPAGE;
            }
        }
        else if (currentPage == CREDITSPAGE)
        {
            DrawTexture(alltexture[3], 0, 0, WHITE);
            DrawRectangleRec(backButtonRect, BEIGE);
            DrawText("Go Back", backButtonRect.x + 10, backButtonRect.y + 10, 30, DARKBROWN);
            if (IsButtonClicked(backButtonRect, mousePoint))
            {
                PlaySound(audios[0]);
                currentPage = HOMEPAGE;
            }
        }
        else if (currentPage == RACETRACK)
        {
            StopMusicStream(music);                             // stoping music for race
            Race(alltexture, &car1, &car2, audios,count);     // function to manage Race
            //Following lines are used to end an on-going race
            Rectangle exitbutton = { SCREEN_WIDTH - 215,SCREEN_HEIGHT - 75,188,40 };
            DrawRectangleRec(exitbutton, BEIGE);
            DrawText("Click to End Race", exitbutton.x + 4, exitbutton.y + 8, 20, DARKBROWN);
            if (IsButtonClicked(exitbutton, mousePoint))
            {
                PlaySound(audios[0]);
                currentPage = HOMEPAGE;
            }
            // checking if any player has won
            if (car1.state == 1 || car2.state == 1)
                currentPage = RESULT;
        }
        else if (currentPage == RESULT)
        {
            laps = 1; //reset laps
            PlayMusicStream(music);
            StopSound(audios[2]);
            StopSound(audios[3]);
            DrawTexture(alltexture[8], 0, 0, WHITE);
            //Sticker
            DrawTexture(alltexture[6], 730, 270, WHITE);
            //results
            Rectangle winnerRect = { 100, 300, 550, 50 };
            Rectangle resultRect = { 100, 400, 400, 50 };
            if (car1.state)
            {
                DrawRectangleRec(winnerRect, BEIGE);
                DrawText("Player 1 won the race! Congrats", winnerRect.x + 10, winnerRect.y + 10, 30, DARKBROWN);
            }
            else if (car2.state)
            {
                DrawRectangleRec(winnerRect, BEIGE);
                DrawText("Player 2 won the race! Congrats", winnerRect.x + 10, winnerRect.y + 10, 30, DARKBROWN);
            }

            DrawRectangleRec(resultRect, BEIGE);
            DrawText("Thanks for playing!", resultRect.x + 10, resultRect.y + 10, 30, DARKBROWN);
            //back button
            DrawRectangleRec(backButtonRect, BEIGE);
            DrawText("Go Back", backButtonRect.x + 10, backButtonRect.y + 10, 30, DARKBROWN);
            if (IsButtonClicked(backButtonRect, mousePoint))
            {
                PlaySound(audios[0]);
                currentPage = HOMEPAGE;
            }

        }
        else if (currentPage == EXIT)
            break;

        EndDrawing();
    }
    StopMusicStream(music);
    // Cleanup
    for (int i = 0; i < 10; i++)
        UnloadTexture(alltexture[i]);
    for (int i = 0;i<4;i++)
        UnloadSound(audios[i]);
    UnloadMusicStream(music);
    // Close window and OpenGL context and audio devices
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

// Definition of function containing race track and logic
void Race(Texture2D* alltexture, controls* car1, controls* car2, Sound* audios, unsigned int *count)
{
    DrawTexture(alltexture[4], 0, 0, WHITE);   //map
    Rectangle col_recs[22]; //collision recs
    col_recs[16] = (Rectangle){ 45,30,400,26 };
    col_recs[17] = (Rectangle){ 45,60,400,26 };
    col_recs[18] = (Rectangle){ 0,SCREEN_HEIGHT / 2,SCREEN_WIDTH,60 }; //starting headline
    col_recs[19] = (Rectangle){ 45,SCREEN_HEIGHT - 60,165,26 }; //for lap
    float deltaTime = GetFrameTime();
    //info at top left
    DrawRectangleRec(col_recs[16], BEIGE);
    DrawText(TextFormat("Player 1's car : Red Audi (Lap : %d)", car1->lapcount + 1), col_recs[16].x + 5, col_recs[16].y + 4, 20, DARKBROWN);
    //DrawText(TextFormat("Player 1's car : Red Audi (Lap : %d)", car1->lapcount + 1), 345, 49, 20, DARKBROWN);
    DrawRectangleRec(col_recs[17], BEIGE);
    DrawText(TextFormat("Player 2's car : Black Viper (Lap : %d)", car2->lapcount + 1), col_recs[17].x+5, col_recs[17].y+4, 20, DARKBROWN);
    //DrawText(TextFormat("Player 2's car : Black Viper (Lap : %d)", car2->lapcount + 1), 345, 79, 20, DARKBROWN);
    DrawRectangleRec(col_recs[19], BEIGE);
    DrawText(TextFormat("Total Laps : %d", laps), 50, col_recs[19].y + 3, 20, DARKBROWN);
    // time passed while race
    Rectangle top_right = {1120,24,260,33};
    DrawRectangleRec(top_right, BEIGE);
    DrawText(TextFormat("Time passed:%2uH:%2uM:%2u S", count[3], count[2], count[1]), top_right.x + 7, top_right.y + 5, 20, DARKBROWN);
    //car1
    Rectangle aud = { 0,0,alltexture[5].width,alltexture[5].height };
    Vector2 car1cent = { alltexture[5].width / 2,alltexture[5].height / 2 };
    DrawTexturePro(alltexture[5], aud, (Rectangle){ car1->posx,car1->posy ,aud.width,aud.height }, car1cent, car1->rotate, WHITE);
    //car2
    Rectangle vip = { 0,0,alltexture[7].width,alltexture[7].height };
    Vector2 car2cent = { alltexture[7].width / 2,alltexture[7].height / 2 };
    DrawTexturePro(alltexture[7], vip, (Rectangle) { car2->posx,car2->posy ,vip.width,vip.height }, car2cent, car2->rotate, WHITE);
    // showing timer rec!
    if (timer++ >= 0 && timer <= 70) {
        PlaySound(audios[1]);
        DrawRectangleRec(col_recs[18], BEIGE);
        DrawText("Ready!", col_recs[18].x + 650, col_recs[18].y + 15, 30, DARKBROWN);
    }
    else if (timer > 70 && timer <= 150)
    {

        DrawRectangleRec(col_recs[18], BEIGE);
        DrawText("Steady! ", col_recs[18].x + 650, col_recs[18].y + 15, 30, DARKBROWN);
    }
    else if (timer > 150 && timer <= 225)
    {

        DrawRectangleRec(col_recs[18], BEIGE);
        DrawText("Go!  ", col_recs[18].x + 650, col_recs[18].y + 15, 30, DARKBROWN);
    }
    else {
        //collision recs initialization
        col_recs[0] = (Rectangle){ 0,0,1400,127 }; //upper
        col_recs[1] = (Rectangle){ 0,845,1400,150 }; //lower
        col_recs[2] = (Rectangle){ 0,0,127,1000 };  //sideleft
        col_recs[3] = (Rectangle){ 1288,0,110,1000 }; //sideright
        col_recs[4] = (Rectangle){ 219,215,10,535 };  //midleft
        col_recs[5] = (Rectangle){ 553,218,11,294 };  //mid m right
        col_recs[6] = (Rectangle){ 219,215,346,8 }; // midupleft
        col_recs[7] = (Rectangle){ 219,742,975,9 };  // middown
        col_recs[8] = (Rectangle){ 660,104,10,298 };  //upleft
        col_recs[9] = (Rectangle){ 660,393,437,10 };   //updown
        col_recs[10] = (Rectangle){ 568,499,619,8 };  //midupright
        col_recs[11] = (Rectangle){ 1183,214,8,400 };  //uleft
        col_recs[12] = (Rectangle){ 1195,214,10,536 };  //uright
        col_recs[13] = (Rectangle){ 1088,104,10,297 };  //upright
        col_recs[21] = (Rectangle){ 1186,215,10,5 };  //u-turn
        // winning and stopping rec
        col_recs[14] = (Rectangle){ 290,90,1,140 }; //end rec
        col_recs[15] = (Rectangle){ 314,90,2,140 };  //start block
        //for displaying booster icon
        col_recs[20] = (Rectangle){ 272,100,20,110 };  //boost block

        //booster
        if (laps > 1)
            if ((!CheckCollisionCircleRec((Vector2) { car1->posx, car1->posy }, 15, col_recs[20])) && (!CheckCollisionCircleRec((Vector2) { car2->posx, car2->posy }, 15, col_recs[20])))
                DrawTextureEx(alltexture[9], (Vector2){ 275, 142 }, 30, 0.8, WHITE);
        counter(count);
        handling(car1, car2, audios);
        collision(audios,car1,car2,col_recs);
        //check winner
        if (CheckCollisionCircleRec((Vector2) { car1->posx, car1->posy }, 15, col_recs[14]))
        {
            car1->lapcount += 1;
            car1->posx += (deltaTime * car1->fspeed) + 30;
            car1->fspeed += 15;
            car1->bspeed += 10;
            if (car1->lapcount >= laps)
            {
                car1->state = 1;
            }
        }
        if (CheckCollisionCircleRec((Vector2) { car2->posx, car2->posy }, 15, col_recs[14]))
        {
            car2->lapcount += 1;
            car2->posx += (deltaTime * car2->fspeed) + 30;
            car2->fspeed += 15;
            car2->bspeed += 10;
            if (car2->lapcount >= laps)
            {
                car2->state = 1;
            }
        }
    }
}

void handling(controls* car1, controls* car2, Sound* audios)
{
    float deltaTime = GetFrameTime();
    if (car1->rotate >= 360 || car1->rotate <= -360)
        car1->rotate = 0.0;
    if (car2->rotate >= 360 || car2->rotate <= -360)
        car2->rotate = 0.0;
    if (IsKeyDown(KEY_D)) {
        car1->rotate += 1.0f * 120 * deltaTime;
    }
    if (IsKeyDown(KEY_A)) {
        car1->rotate -= 1.0f * 120 * deltaTime;
    }
    // move forward and backward(acceleration and deceleration)
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_S))
        PlaySound(audios[2]);
    if (IsKeyUp(KEY_W) && IsKeyUp(KEY_S))
        StopSound(audios[2]);


    if (IsKeyDown(KEY_W))
        if ((car1->rotate >= 10 && car1->rotate <= 80) || (car1->rotate <= -280 && car1->rotate >= -350))
        {
            car1->posx += deltaTime * car1->bspeed;
            car1->posy -= deltaTime * car1->bspeed;
        }
        else if ((car1->rotate >= 100 && car1->rotate <= 170) || (car1->rotate <= -190 && car1->rotate >= -260))
        {
            car1->posx += deltaTime * car1->bspeed;
            car1->posy += deltaTime * car1->bspeed;

        }
        else if ((car1->rotate >= 190 && car1->rotate <= 260) || (car1->rotate <= -90 && car1->rotate >= -170)) {
            car1->posx -= deltaTime * car1->bspeed;
            car1->posy += deltaTime * car1->bspeed;
        }
        else if ((car1->rotate >= 280 && car1->rotate <= 350) || (car1->rotate <= -10 && car1->rotate >= -80)) {
            car1->posx -= deltaTime * car1->bspeed;
            car1->posy -= deltaTime * car1->bspeed;
        }
        else if ((car1->rotate >= 0 && car1->rotate < 10) || (car1->rotate > 350 && car1->rotate <= 360) || (car1->rotate >= -10 && car1->rotate < 0) || (car1->rotate < -350 && car1->rotate >= -360))
        {
            car1->posy -= deltaTime * car1->fspeed;
        }
        else if ((car1->rotate >= 80 && car1->rotate < 100) || (car1->rotate <= -260 && car1->rotate > -280))
        {
            car1->posx += deltaTime * car1->fspeed;
        }
        else if ((car1->rotate >= 170 && car1->rotate < 190) || (car1->rotate <= -170 && car1->rotate > -190))
        {
            car1->posy += deltaTime * car1->fspeed;
        }
        else if ((car1->rotate >= 260 && car1->rotate < 280) || (car1->rotate <= -80 && car1->rotate > -100))
        {
            car1->posx -= deltaTime * car1->fspeed;
        }
    if (IsKeyDown(KEY_S))
        if ((car1->rotate >= 10 && car1->rotate <= 80) || (car1->rotate <= -280 && car1->rotate >= -350))
        {
            car1->posx -= deltaTime * car1->bspeed;
            car1->posy += deltaTime * car1->bspeed;
        }
        else if ((car1->rotate >= 100 && car1->rotate <= 170) || (car1->rotate <= -190 && car1->rotate >= -260))
        {
            car1->posx -= deltaTime * car1->bspeed;
            car1->posy -= deltaTime * car1->bspeed;

        }
        else if ((car1->rotate >= 190 && car1->rotate <= 260) || (car1->rotate <= -90 && car1->rotate >= -170)) {
            car1->posx += deltaTime * car1->bspeed;
            car1->posy -= deltaTime * car1->bspeed;
        }
        else if ((car1->rotate >= 280 && car1->rotate <= 350) || (car1->rotate <= -10 && car1->rotate >= -80)) {
            car1->posx += deltaTime * car1->bspeed;
            car1->posy += deltaTime * car1->bspeed;
        }
        else if ((car1->rotate >= 0 && car1->rotate < 10) || (car1->rotate > 350 && car1->rotate <= 360) || (car1->rotate >= -10 && car1->rotate < 0) || (car1->rotate < -350 && car1->rotate >= -360))
        {
            car1->posy += deltaTime * car1->fspeed;
        }
        else if ((car1->rotate >= 80 && car1->rotate < 100) || (car1->rotate <= -260 && car1->rotate > -280))
        {
            car1->posx -= deltaTime * car1->fspeed;
        }
        else if ((car1->rotate >= 170 && car1->rotate < 190) || (car1->rotate <= -170 && car1->rotate > -190))
        {
            car1->posy -= deltaTime * car1->fspeed;
        }
        else if ((car1->rotate >= 260 && car1->rotate < 280) || (car1->rotate <= -80 && car1->rotate > -100))
        {
            car1->posx += deltaTime * car1->fspeed;
        }

    //car2 controls
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN))
        PlaySound(audios[3]);
    if (IsKeyUp(KEY_UP) && IsKeyUp(KEY_DOWN))
        StopSound(audios[3]);
    if (IsKeyDown(KEY_RIGHT)) {
        car2->rotate += 1.0f * 140 * deltaTime;
    }
    if (IsKeyDown(KEY_LEFT)) {
        car2->rotate -= 1.0f * 140 * deltaTime;
    }
    // move forward and backward(acceleration and deceleration)
    //test
    if (IsKeyDown(KEY_UP))
        if ((car2->rotate >= 10 && car2->rotate <= 80) || (car2->rotate <= -280 && car2->rotate >= -350))
        {
            car2->posx += deltaTime * car2->bspeed;
            car2->posy -= deltaTime * car2->bspeed;
        }
        else if ((car2->rotate >= 100 && car2->rotate <= 170) || (car2->rotate <= -190 && car2->rotate >= -260))
        {
            car2->posx += deltaTime * car2->bspeed;
            car2->posy += deltaTime * car2->bspeed;

        }
        else if ((car2->rotate >= 190 && car2->rotate <= 260) || (car2->rotate <= -90 && car2->rotate >= -170)) {
            car2->posx -= deltaTime * car2->bspeed;
            car2->posy += deltaTime * car2->bspeed;
        }
        else if ((car2->rotate >= 280 && car2->rotate <= 350) || (car2->rotate <= -10 && car2->rotate >= -80)) {
            car2->posx -= deltaTime * car2->bspeed;
            car2->posy -= deltaTime * car2->bspeed;
        }
        else if ((car2->rotate >= 0 && car2->rotate < 10) || (car2->rotate > 350 && car2->rotate <= 360) || (car2->rotate >= -10 && car2->rotate < 0) || (car2->rotate < -350 && car2->rotate >= -360))
        {
            car2->posy -= deltaTime * car2->fspeed;
        }
        else if ((car2->rotate >= 80 && car2->rotate < 100) || (car2->rotate <= -260 && car2->rotate > -280))
        {
            car2->posx += deltaTime * car2->fspeed;
        }
        else if ((car2->rotate >= 170 && car2->rotate < 190) || (car2->rotate <= -170 && car2->rotate > -190))
        {
            car2->posy += deltaTime * car2->fspeed;
        }
        else if ((car2->rotate >= 260 && car2->rotate < 280) || (car2->rotate <= -80 && car2->rotate > -100))
        {
            car2->posx -= deltaTime * car2->fspeed;
        }
    if (IsKeyDown(KEY_DOWN))
        if ((car2->rotate >= 10 && car2->rotate <= 80) || (car2->rotate <= -280 && car2->rotate >= -350))
        {
            car2->posx -= deltaTime * car2->bspeed;
            car2->posy += deltaTime * car2->bspeed;
        }
        else if ((car2->rotate >= 100 && car2->rotate <= 170) || (car2->rotate <= -190 && car2->rotate >= -260))
        {
            car2->posx -= deltaTime * car2->bspeed;
            car2->posy -= deltaTime * car2->bspeed;

        }
        else if ((car2->rotate >= 190 && car2->rotate <= 260) || (car2->rotate <= -90 && car2->rotate >= -170)) {
            car2->posx += deltaTime * car2->bspeed;
            car2->posy -= deltaTime * car2->bspeed;
        }
        else if ((car2->rotate >= 280 && car2->rotate <= 350) || (car2->rotate <= -10 && car2->rotate >= -80)) {
            car2->posx += deltaTime * car2->bspeed;
            car2->posy += deltaTime * car2->bspeed;
        }
        else if ((car2->rotate >= 0 && car2->rotate < 10) || (car2->rotate > 350 && car2->rotate <= 360) || (car2->rotate >= -10 && car2->rotate < 0) || (car2->rotate < -350 && car2->rotate >= -360))
        {
            car2->posy += deltaTime * car2->fspeed;
        }
        else if ((car2->rotate >= 80 && car2->rotate < 100) || (car2->rotate <= -260 && car2->rotate > -280))
        {
            car2->posx -= deltaTime * car2->fspeed;
        }
        else if ((car2->rotate >= 170 && car2->rotate < 190) || (car2->rotate <= -170 && car2->rotate > -190))
        {
            car2->posy -= deltaTime * car2->fspeed;
        }
        else if ((car2->rotate >= 260 && car2->rotate < 280) || (car2->rotate <= -80 && car2->rotate > -100))
        {
            car2->posx += deltaTime * car2->fspeed;
        }
}
void collision(Sound *audios, controls* car1, controls* car2, Rectangle* col_recs) {
    float deltaTime = GetFrameTime();
    //collision of cars
    if (CheckCollisionCircles((Vector2) { car1->posx, car1->posy }, 15, (Vector2) { car2->posx, car2->posy }, 15)) {
        PlaySound(audios[0]);
        car1->posy -= deltaTime * car1->fspeed;
        car2->posy += deltaTime * car2->fspeed;
        car1->posx += deltaTime * car1->fspeed;
        car2->posx -= deltaTime * car2->fspeed;
    }
    ///curves for car1
    if (CheckCollisionCircles((Vector2) { 1173, 486 }, 16, (Vector2) { car1->posx, car1->posy }, 15))
    {
        PlaySound(audios[0]);
        car1->posx -= deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircles((Vector2) { 614, 126 }, 15, (Vector2) { car1->posx, car1->posy }, 15))
    {
        PlaySound(audios[0]);
        car1->posx -= deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircles((Vector2) { 579, 482 }, 20, (Vector2) { car1->posx, car1->posy }, 15))
    {
        PlaySound(audios[0]);
        car1->posy -= deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircles((Vector2) { 1113, 137 }, 18, (Vector2) { car1->posx, car1->posy }, 15))
    {
        PlaySound(audios[0]);
        car1->posy += deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircles((Vector2) { 1271, 137 }, 16, (Vector2) { car1->posx, car1->posy }, 15))
    {
        PlaySound(audios[0]);
        car1->posy += deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircles((Vector2) { 1257, 836 }, 15, (Vector2) { car1->posx, car1->posy }, 15))
    {
        PlaySound(audios[0]);
        car1->posy -= deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircles((Vector2) { 1278, 819 }, 16, (Vector2) { car1->posx, car1->posy }, 15))
    {
        PlaySound(audios[0]);
        car1->posy -= deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircles((Vector2) { 134, 833 }, 18, (Vector2) { car1->posx, car1->posy }, 15))
    {
        PlaySound(audios[0]);
        car1->posx += deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircles((Vector2) { 138, 136 }, 18, (Vector2) { car1->posx, car1->posy }, 15))
    {
        PlaySound(audios[0]);
        car1->posy += deltaTime * car1->fspeed;
    }
    //checking collisions of car1
    if (CheckCollisionCircleRec((Vector2) { car1->posx, car1->posy }, 15, col_recs[0]))
    {
        PlaySound(audios[0]);
        car1->posy += deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car1->posx, car1->posy }, 15, col_recs[1]))
    {
        PlaySound(audios[0]);
        car1->posy -= deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car1->posx, car1->posy }, 15, col_recs[3]))
    {
        PlaySound(audios[0]);
        car1->posx -= deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car1->posx, car1->posy }, 15, col_recs[2]))
    {
        PlaySound(audios[0]);
        car1->posx += deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car1->posx, car1->posy }, 15, col_recs[1]))
    {
        PlaySound(audios[0]);
        car1->posy -= deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car1->posx, car1->posy }, 15, col_recs[6]))
    {
        PlaySound(audios[0]);
        car1->posy -= deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car1->posx, car1->posy }, 15, col_recs[4]))
    {
        PlaySound(audios[0]);
        car1->posx -= deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car1->posx, car1->posy }, 15, col_recs[7]))
    {
        PlaySound(audios[0]);
        car1->posy += deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car1->posx, car1->posy }, 15, col_recs[5]))
    {
        PlaySound(audios[0]);
        car1->posx += deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car1->posx, car1->posy }, 15, col_recs[8]))
    {
        PlaySound(audios[0]);
        car1->posx -= deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car1->posx, car1->posy }, 15, col_recs[13]))
    {
        PlaySound(audios[0]);
        car1->posx += deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car1->posx, car1->posy }, 15, col_recs[12]))
    {
        PlaySound(audios[0]);
        car1->posx += deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car1->posx, car1->posy }, 15, col_recs[10]))
    {
        PlaySound(audios[0]);
        car1->posy -= deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car1->posx, car1->posy }, 15, col_recs[11]))
    {
        PlaySound(audios[0]);
        car1->posx -= deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car1->posx, car1->posy }, 15, col_recs[9]))
    {
        PlaySound(audios[0]);
        car1->posy += deltaTime * car1->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car1->posx, car1->posy }, 15, col_recs[15]))
    {
        PlaySound(audios[0]);
        car1->posx += deltaTime * car1->fspeed + 2.0;
    }
    if (CheckCollisionCircleRec((Vector2) { car1->posx, car1->posy }, 15, col_recs[21]))
    {
        PlaySound(audios[0]);
        car1->posy -= deltaTime * car1->fspeed;
    }

    //checking collisions of car2
    if (CheckCollisionCircleRec((Vector2) { car2->posx, car2->posy }, 15, col_recs[0]))
    {
        PlaySound(audios[0]);
        car2->posy += deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car2->posx, car2->posy }, 15, col_recs[1]))
    {
        PlaySound(audios[0]);
        car2->posy -= deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car2->posx, car2->posy }, 15, col_recs[3]))
    {
        PlaySound(audios[0]);
        car2->posx -= deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car2->posx, car2->posy }, 15, col_recs[2]))
    {
        PlaySound(audios[0]);
        car2->posx += deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car2->posx, car2->posy }, 15, col_recs[1]))
    {
        PlaySound(audios[0]);
        car2->posy -= deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car2->posx, car2->posy }, 15, col_recs[6]))
    {
        PlaySound(audios[0]);
        car2->posy -= deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car2->posx, car2->posy }, 15, col_recs[4]))
    {
        PlaySound(audios[0]);
        car2->posx -= deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car2->posx, car2->posy }, 15, col_recs[7]))
    {
        PlaySound(audios[0]);
        car2->posy += deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car2->posx, car2->posy }, 15, col_recs[5]))
    {
        PlaySound(audios[0]);
        car2->posx += deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car2->posx, car2->posy }, 15, col_recs[8]))
    {
        PlaySound(audios[0]);
        car2->posx -= deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car2->posx, car2->posy }, 15, col_recs[13]))
    {
        PlaySound(audios[0]);
        car2->posx += deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car2->posx, car2->posy }, 15, col_recs[12]))
    {
        PlaySound(audios[0]);
        car2->posx += deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car2->posx, car2->posy }, 15, col_recs[10]))
    {
        PlaySound(audios[0]);
        car2->posy -= deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car2->posx, car2->posy }, 15, col_recs[11]))
    {
        PlaySound(audios[0]);
        car2->posx -= deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car2->posx, car2->posy }, 15, col_recs[9]))
    {
        PlaySound(audios[0]);
        car2->posy += deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircleRec((Vector2) { car2->posx, car2->posy }, 15, col_recs[15]))
    {
        PlaySound(audios[0]);
        car2->posx += deltaTime * car2->fspeed + 2.0;
    }
    if (CheckCollisionCircleRec((Vector2) { car2->posx, car2->posy }, 15, col_recs[21]))
    {
        PlaySound(audios[0]);
        car2->posy -= deltaTime * car2->fspeed;
    }

    ///curves for car2
    if (CheckCollisionCircles((Vector2) { 614, 126 }, 15, (Vector2) { car2->posx, car2->posy }, 15))
    {
        PlaySound(audios[0]);
        car2->posx -= deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircles((Vector2) { 579, 482 }, 20, (Vector2) { car2->posx, car2->posy }, 15))
    {
        PlaySound(audios[0]);
        car2->posy -= deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircles((Vector2) { 1113, 137 }, 18, (Vector2) { car2->posx, car2->posy }, 15))
    {
        PlaySound(audios[0]);
        car2->posy += deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircles((Vector2) { 1271, 137 }, 16, (Vector2) { car2->posx, car2->posy }, 15))
    {
        PlaySound(audios[0]);
        car2->posy += deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircles((Vector2) { 1257, 836 }, 15, (Vector2) { car2->posx, car2->posy }, 15))
    {
        PlaySound(audios[0]);
        car2->posy -= deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircles((Vector2) { 1278, 819 }, 16, (Vector2) { car2->posx, car2->posy }, 15))
    {
        PlaySound(audios[0]);
        car2->posy -= deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircles((Vector2) { 134, 833 }, 18, (Vector2) { car2->posx, car2->posy }, 15))
    {
        PlaySound(audios[0]);
        car2->posx += deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircles((Vector2) { 138, 136 }, 18, (Vector2) { car2->posx, car2->posy }, 15))
    {
        PlaySound(audios[0]);
        car2->posy += deltaTime * car2->fspeed;
    }
    if (CheckCollisionCircles((Vector2) { 1173, 486 }, 16, (Vector2) { car2->posx, car2->posy }, 15))
    {
        PlaySound(audios[0]);
        car2->posx -= deltaTime * car2->fspeed;
    }
}
bool IsButtonClicked(Rectangle button, Vector2 mousePoint) {

    return CheckCollisionPointRec(mousePoint, button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}
void counter(unsigned int* count)
{
    if (count[0]++ == 40)
    {
        count[1]++;//seconds
        count[0] = 0;
    }
    if (count[1] == 60)
    {
        count[2]++;//minutes
        count[1] = 0;
    }
    if (count[2] == 60)
    {
        count[3]++;//hours
        count[2] = 0;
    }
    if (count[3] == 12)
    {
       for (int i = 0; i < 4; i++)//resetting time of race
                count[i] = 0;
    }
}