#include <graphics.h>
#include <dos.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <string.h>
#define MAX_METEORS 3

struct Meteor {
    int x, y;
    int speed;
};

// Background music
void playBackgroundMusic() {
    int notes[] = {262, 294, 330, 349, 392, 440, 494};
    for (int i = 0; i < 7; i++) {
        sound(notes[i]);
        delay(50);
    }
    nosound();
}

// Draw background stars
void drawStars() {
    for (int i = 0; i < 100; i++) {
        putpixel(rand() % getmaxx(), rand() % getmaxy(), WHITE);
    }
}

// Draw Moon
void drawMoon() {
    setcolor(LIGHTGRAY);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    fillellipse(80, 80, 30, 30);
}

// Draw Earth
void drawEarth() {
    setcolor(BLUE);
    setfillstyle(SOLID_FILL, BLUE);
    fillellipse(500, 400, 50, 50);
    setcolor(GREEN);
    setfillstyle(SOLID_FILL, GREEN);
    fillellipse(510, 390, 10, 10); // land
}

// Draw Meteor
void drawMeteor(int x, int y) {
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    fillellipse(x, y, 10, 10);
    setcolor(YELLOW);
    line(x - 15, y - 15, x, y);
}

// Explosion + Boom Sound + Flood
void explode(int x, int y) {
    // Boom Sound
    sound(1000);
    delay(100);
    sound(1200);
    delay(100);
    sound(800);
    delay(100);
    nosound();

    // Explosion cloud with floodfill
    for (int r = 10; r < 70; r += 10) {
        int color = rand() % 14 + 1;
        setcolor(color);
        circle(x, y, r);
        setfillstyle(SOLID_FILL, color);
        floodfill(x + 1, y + 1, color);
        delay(50);
    }

    // Sparks
    for (int i = 0; i < 200; i++) {
        int px = x + rand() % 60 - 30;
        int py = y + rand() % 60 - 30;
        putpixel(px, py, rand() % 15 + 1);
    }
}

// Shockwave
void shockwave(int x, int y) {
    for (int r = 10; r < 120; r += 15) {
        setcolor(LIGHTCYAN);
        circle(x, y, r);
        delay(40);
    }
}

// Show explosion message
void showMessage(char msg[], int x, int y) {
    settextstyle(3, 0, 2); // Bold font
    setcolor(WHITE);
    outtextxy(x, y, msg);
    delay(1000);
}

void main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\Turboc3\\BGI");

    Meteor meteors[MAX_METEORS];
    int exploded[MAX_METEORS] = {0};

    for (int i = 0; i < MAX_METEORS; i++) {
        meteors[i].x = rand() % 600 + 20;
        meteors[i].y = rand() % 100;
        meteors[i].speed = rand() % 4 + 2;
    }

    while (!kbhit()) {
        cleardevice();
        drawStars();
        drawMoon();
        drawEarth();
        playBackgroundMusic();

        for (int i = 0; i < MAX_METEORS; i++) {
            drawMeteor(meteors[i].x, meteors[i].y);
            meteors[i].y += meteors[i].speed;

	    if (meteors[i].y >= 370 && !exploded[i]) {
                explode(meteors[i].x, meteors[i].y);
                shockwave(meteors[i].x, meteors[i].y);
		showMessage("BOOM! Meteor hit Earth!", 140, 440);
                exploded[i] = 1;
            }

            if (meteors[i].y > getmaxy()) {
                meteors[i].x = rand() % 600 + 20;
                meteors[i].y = rand() % 100;
                meteors[i].speed = rand() % 4 + 2;
                exploded[i] = 0;
            }
        }

        delay(80);
    }

    settextstyle(3, 0, 2);
    outtextxy(180, 450, "Press any key to exit...");
    getch();
    closegraph();
}
