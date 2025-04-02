#pragma once

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <vector>

enum Direction { UP, RIGHT, DOWN, LEFT };

const int PERIOD = 150000;
enum Color {
    BLACK   = 30,
    RED     = 31,
    GREEN   = 32,
    YELLOW  = 33,
    BLUE    = 34,
    MAGENTA = 35,
    CYAN    = 36,
    WHITE   = 37,
    DEFAULT = 0
};

const char TEXT_MODE = 't';
const int  WIDTH  = 30;
const int  HEIGTH = 15;

namespace windowsize {
    extern int width;
    extern int height;
}

const char SNAKE_HEAD = 'O';
const char SNAKE_BODY = 'o';
const char RABBIT = '*';
const char EMPTY = ' ';

#include "model.h"
#include "control.h"
#include "view.h"