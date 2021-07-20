#include <iostream>
#include <random>
#include <stdlib.h>
#include <chrono>


using namespace std;

enum Cell {
   CROSS = 'x',
    ZERO = 'o',
    EMPTY = '_'
};

struct Coord {
    size_t y;
    size_t x;
};

enum Progress{
    IN_PROGRESS,
    WON_HUMAN,
    WON_AI,
    DRAW
};

struct Field {
    Cell** ppField = nullptr;
    const size_t SIZE = 3;
    size_t turn = 0; //hod
    Progress progress = IN_PROGRESS;
    Cell ai;
    Cell human;
};

void clear()
{
    system("cls");
}

int32_t getRandomNum (int32_t min, int32_t max)
{
    const static auto seed = chrono::system_clock::now().time_since_epoch().count();
    static mt19937_64 generator (seed);
    uniform_int_distribution<int32_t> dis(min,max);
    return dis(generator);
}

void initField(Field & f)
{
    f.ppField = new (nothrow) Cell * [f.SIZE];
    for (size_t i=0; i < f.SIZE; i++)
    {
        f.ppField[i] = new (nothrow) Cell[f.SIZE];
    }

    for (size_t y=0; y < f.SIZE; y++)
    {
        for (size_t x = 0; x < f.SIZE; x++)
        {
        f.ppField[y][x] = EMPTY;
        }
    }

    if (getRandomNum (0,1000) > 500)
    {
        f.human = CROSS;
        f.ai = ZERO;
        f.turn = 0;
    }
    else
    {
        f.human = ZERO;
        f.ai = CROSS;
        f.turn = 1;
    }
}


void deinitField (Field & f)
{
    for (size_t i=0; i < f.SIZE; i++)
    {
        f.ppField[i] = new (nothrow) Cell[f.SIZE];
    }
    delete [] f.ppField;
    f.ppField = nullptr;
}

void printField(const Field & f)
{
//    cout << "     ";
//    for (size_t x = 0; x < f.SIZE; x++)
//    {
//        cout << x+1 << "   ";
//        cout << endl;
//    }
    for (size_t y=0; y < f.SIZE; y++)
    {
        cout << " " << y+1 << "  | ";
        for (size_t x = 0; x < f.SIZE; x++)
        {
        cout << (char)f.ppField[y][x] << " | ";
        }
        cout << endl;
    }
    cout << endl << "Human: " << (char)f.human << endl;
    cout << "AI: " << (char)f.ai << endl << endl;
}

Coord getHumanCoord(Field & f)
{
    Coord c{0,0};
    do{
        cout << "X coord: ";
        cin >> c.x;
        cout << "Y coord: ";
        cin >> c.y;
    }while (c.x == 0 || c.y == 0 || c.x > 3 || c.y > 3 || f.ppField[c.y-1][c.x-1] != EMPTY);
    c.x--;
    c.y--;

    return c;
}

Coord getAICoord(Field & f)
{


    if (f.ppField[1][1] == EMPTY)
    {
        return {1,1};
    }

    //ugli
    if (f.ppField[0][0] == EMPTY)
        return {0,0};
    if (f.ppField[2][2] == EMPTY)
        return {2,2};
    if (f.ppField[0][2] == EMPTY)
        return {0,2};
    if (f.ppField[2][0] == EMPTY)
        return {2,0};

    //ne ugli
    if (f.ppField[0][1] == EMPTY)
        return {0,1};
    if (f.ppField[2][1] == EMPTY)
        return {2,1};
    if (f.ppField[1][0] == EMPTY)
        return {1,0};
    if (f.ppField[1][2] == EMPTY)
        return {1,2};
}

Progress getWon(Field & f) //kto vigral? ili igra v progressee
{
    //stroki
    for (size_t y=0; y<f.SIZE;y++)
    {
        if (f.ppField[y][0] == f.ppField[y][1] && f.ppField[y][0] == f.ppField[y][2])
        {
            if (f.ppField[y][0] == f.ai)
                return WON_AI;
            if (f.ppField[y][0] == f.human)
                return WON_HUMAN;
        }
    }
    //stolbci
    for (size_t x=0; x<f.SIZE;x++)
    {
        if (f.ppField[0][x] == f.ppField[1][x] && f.ppField[0][x] == f.ppField[2][x])
        {
            if (f.ppField[0][x] == f.ai)
                return WON_AI;
            if (f.ppField[0][x] == f.human)
                return WON_HUMAN;
        }
    }
    //diagonali
    if(f.ppField[0][0] == f.ppField[1][1] && f.ppField[0][0] == f.ppField[2][2])
    {
        if (f.ppField[0][0] == f.ai)
            return WON_AI;
        if (f.ppField[0][0] == f.human)
            return WON_HUMAN;
    }
    if(f.ppField[0][2] == f.ppField[1][1] && f.ppField[2][0] == f.ppField[1][1])
    {
        if (f.ppField[1][1] == f.ai)
            return WON_AI;
        if (f.ppField[1][1] == f.human)
            return WON_HUMAN;
    }

    //nichya
    bool draw = true;
    for (size_t y=0; y<f.SIZE; y++)
    {
        for (size_t x=0; x<f.SIZE; x++)
        {
            if (f.ppField[y][x] == EMPTY)
            {
                draw = false;
                break;
            }
        }
        if (!draw)
        {
            break;
        }
    }
    if (draw)
    {
        return DRAW;
    }

    return IN_PROGRESS;
}

int main()
{
    Field f;
    clear();
    initField(f);
    printField(f);
    do{
        if (f.turn % 2 == 0)
        {
            //chetnoe - hod cheloveka
            Coord c = getHumanCoord(f);
            f.ppField[c.y][c.x] = f.human;
        }
        else
        {
            Coord c = getAICoord(f);
            f.ppField[c.y][c.x] = f.ai;
        }
        clear();
        printField(f);
        f.turn++;
        f.progress = getWon(f);
    } while (f.progress == IN_PROGRESS);
    if (f.progress == WON_HUMAN)
    {
        cout << "You won!";
    }
    if (f.progress == WON_AI)
    {
        cout << "AI won!";
    }
    if (f.progress == DRAW)
    {
        cout << "DRAW!";
    }
    deinitField(f);
    return 0;
}
