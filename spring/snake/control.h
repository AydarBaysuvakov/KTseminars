#pragma once

class Control {
public:
    virtual void handle_input(char c) = 0;
    static Control * getControl(char type, Model &model, int snake_index);
};

class Control_wasd : Control {
    Model& model;
    int snake_index;

public:
    Control_wasd(Model &model, int snake_index) : model(model), snake_index(snake_index) {};
    virtual void handle_input(char c);
};

class Control_arrow : Control {
    Model& model;
    int snake_index;

public:
    Control_arrow(Model &model, int snake_index) : model(model), snake_index(snake_index) {};
    virtual void handle_input(char c);
};

class Control_smart : Control {
    Model& model;
    int snake_index;

public:
    Control_smart(Model &model, int snake_index) : model(model), snake_index(snake_index) {};
    virtual void handle_input(char c);
};

class Control_dummy : Control {
    Model& model;
    int snake_index;
    Rabbit closest;

public:
    Control_dummy(Model &model, int snake_index) : model(model), snake_index(snake_index) {};
    virtual void handle_input(char c);

private:
    Rabbit get_closest();
};