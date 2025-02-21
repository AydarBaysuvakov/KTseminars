#include <iostream>
#include "view.h"

View* View::getView(char type, int size) {
    switch (type) {
        case 't':
            return (View*) new TView(size);
        default:
            std::cout << "Неверный тип: " << type << std::endl;
            return nullptr;
    }
}

void TView::draw() {
    drawframe();
    setbgrcolor(GREEN);
    gotoxy(screen_size / 2 - 4, screen_size / 2);
    std::cout << "Hello world\n";
    resetcolor();
    gotoxy(0, -1);
}

void TView::clrscr() {
    std::cout << "\e[H\e[J";
}

void TView::gotoxy(int x, int y) {
    if (x < 0 || x >= screen_size || y < -1 || y >= screen_size) { return; }
    std::cout << "\e[" << screen_size - y << ";" << 2 * (x + 1) << "H";
}

void TView::settxtcolor(int color) {
    std::cout << "\033[" << color << "m";
}

void TView::setbgrcolor(int color) {
    std::cout << "\033[" << color + 10 << "m";
}

void TView::resetcolor() {
    std::cout << "\033[0m";
}

void TView::drawframe() {
    clrscr();
    settxtcolor(RED);
    for (int i = 0; i < screen_size + 2; ++i)
        std::cout << "**";
    std::cout << std::endl;

    for (int i = 0; i < screen_size; ++i)
        std::cout << "**\e[" << i << ";" << 2 * screen_size + 4 << "H**" << std::endl;

    for (int i = 0; i < screen_size + 2; ++i)
        std::cout << "**";
    std::cout << std::endl;
    resetcolor();
}