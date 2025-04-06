#pragma once

bool key_pressed();

class View {
public:
    virtual void draw(int x, int y, char c, int color) = 0;
    virtual void run() = 0;
    virtual void add_control(Control * control) = 0;
    static View * getView(char type, Model &model);
};

class TView : View {
    Model &model;
    std::vector <Control *> controllers;
    termios oldt, newt;

public:
    TView(Model &model) : model(model) {};
    virtual void draw(int x, int y, char c, int color);
    virtual void run();
    virtual void add_control(Control * control);

private:
    void alignscr();
    void clrscr();
    void gotoxy(int x, int y);
    void setbgrcolor(int color);
    void settxtcolor(int color);
    void resetcolor();
    void drawframe();
    bool key_pressed();
};

class GView : View {
    Model &model;
    std::vector <Control *> controllers;
    sf::RenderWindow window;

public:
    GView(Model &model) : model(model) {};
    virtual void draw(int x, int y, char c, int color);
    virtual void run();
    virtual void add_control(Control * control);

private:
    sf::Color convert_color(int color);
    char convert_input(sf::Keyboard::Key input);
};