#include <ncurses.h>
#include <math.h>
#include <cstdlib>
#include <vector>
#include <array>

#define CELL_CHAR               "."
#define TARGET_HEIGHT           60
#define TARGET_WIDTH            201
#define TERR_HEIGHT_ORIGINAL    980
#define POINTS                  150000
#define STEP                    30

const long double ratio = TARGET_WIDTH/TARGET_HEIGHT;
const long double TERR_WIDTH_ORIGINAL = TERR_HEIGHT_ORIGINAL * ratio;

long double terr_height = TERR_HEIGHT_ORIGINAL;
long double terr_width = TERR_WIDTH_ORIGINAL;

int terr_scale = 145;
long double top_left[2] = {0, 0};

std::vector<std::array<long double, 2> > points;

// size of character is 14px 18px
// reference window is 1800px 980pxc
// reference turtle starts at 1400px 500px

long double xi = 1561;
long double yi = 500;

long double position[2] = {xi, yi};

void placePoint(long double x, long double y)
{
    std::array<long double, 2> point= {x, y};

    points.push_back(point);

    int window_x = floor(x * TARGET_HEIGHT/terr_height * 2);
    int window_y = floor(y * TARGET_HEIGHT/terr_height);

    move(window_y, window_x);
    printw(CELL_CHAR);
}

void rawPlace(long double x, long double y)
{
    int window_x = floor(x * TARGET_HEIGHT/terr_height * 2);
    int window_y = floor(y * TARGET_HEIGHT/terr_height);
    
    if (window_x >= 0 && window_x <= TARGET_WIDTH && window_y >= 0 && window_y <= TARGET_HEIGHT)
    {
        move(window_y, window_x);
        printw(CELL_CHAR);
    }
}

void Barnsley(int iterations)
{
    for (int i = 0; i < iterations; i++)
    {
        placePoint(terr_scale * -position[1] + xi, terr_scale * position[0] + yi);

        long double old_x = position[0];
        long double old_y = position[1];

        int r = (rand() % 100) + 1;
        if (r <= 1)
        {
            position[0] = 0;
            position[1] *= 0.16;
        }else if (r <= 8)
        {
            position[0] = -0.15 * old_x + 0.28 * old_y;
            position[1] = 0.26 * old_x + 0.24 * old_y + 0.44;
        }else if (r <= 15)
        {
            position[0] = 0.2 * old_x - 0.26 * old_y;
            position[1] = 0.23 * old_x + 0.22 * old_y + 1.6;
        }else
        {
            position[0] = 0.85 * old_x + 0.04 * old_y;
            position[1] = -0.04 * old_x + 0.85 * old_y + 1.6;
        }
    }
}

void move_window(float x, float y)
{
    long double width_extent = top_left[0] + terr_width;
    long double height_extent = top_left[1] + terr_height;

    x *= terr_width/TERR_WIDTH_ORIGINAL * 2;
    y *= terr_height/TERR_HEIGHT_ORIGINAL;

    if (width_extent + x > TERR_WIDTH_ORIGINAL)
    {
        top_left[0] += TERR_WIDTH_ORIGINAL - width_extent;
    }else if (top_left[0] + x < 0)
    {
        top_left[0] = 0;
    }else
    {
        top_left[0] += x;
    }

    if (height_extent + y > TERR_HEIGHT_ORIGINAL)
    {
        top_left[1] += TERR_HEIGHT_ORIGINAL - height_extent;
    }else if (top_left[1] + y < 0)
    {
        top_left[1] = 0;
    }else
    {
        top_left[1] += y;
    }
}

void resize_window(float factor)
{

    long double projected_terr_height = terr_height * factor;
    long double projected_terr_width = projected_terr_height * ratio;

    
    if (!(top_left[1] + projected_terr_height > TERR_HEIGHT_ORIGINAL || top_left[0] + projected_terr_width > TERR_WIDTH_ORIGINAL) )
    {
        long double x_shift = (terr_width - projected_terr_width) * (TERR_WIDTH_ORIGINAL/terr_width);
        long double y_shift = (terr_height - projected_terr_height) * (TERR_HEIGHT_ORIGINAL/terr_height);

        terr_height = projected_terr_height;
        terr_width = projected_terr_width;
    }
}

void draw()
{
    clear();

    for (int i = 0; i < POINTS; i ++)
    {
        std::array<long double, 2> point = points[i];
        rawPlace(point[0] - top_left[0], point[1] - top_left[1]);
    }

    refresh();
}

int main()
{
    initscr();
    noecho();
    keypad(stdscr, true);
    
    Barnsley(POINTS);
    refresh();

    move(0, 0);
    printw("                    ");
    move(0, 0);

    while (true)
    {
        int c = getch();
        if (c == KEY_LEFT)
        {
            resize_window(1.1);
            draw();
        }else if (c == KEY_RIGHT)
        {
            resize_window(0.9);
            draw();
        }else if (c == 119) // "W"
        {
            move_window(0, -STEP * 2);
            draw();
        }else if (c == 97) // "A"
        {
            move_window(-STEP, 0);
            draw();
        }else if (c == 115) // "S"
        {
            move_window(0, STEP * 2);
            draw();
        }else if (c == 100) // "D"
        {
            move_window(STEP, 0);
            draw();
        }else if (c == 113) // "Q"
        {
            break;
        }
    }

    endwin();
    return 0;
}
