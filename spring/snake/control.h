#pragma once

class Control {
public:
    virtual void handle_input(char c) = 0;
    virtual void set_index(int index) = 0;
    static Control * getControl(char type, Model &model);
};

class Control_wasd : Control {
    Model& model;
    int snake_index;

public:
    Control_wasd(Model &model) : model(model){};
    virtual void handle_input(char c);
    virtual void set_index(int index) { snake_index = index; }
};

class Control_arrow : Control {
    Model& model;
    int snake_index;

public:
    Control_arrow(Model &model) : model(model) {};
    virtual void handle_input(char c);
    virtual void set_index(int index) { snake_index = index; }
};

class Control_smart : Control {
    Model& model;
    int snake_index;
    Rabbit closest;

public:
    Control_smart(Model &model) : model(model), closest({-1, -1}) {};
    virtual void handle_input(char c);
    virtual void set_index(int index) { snake_index = index; }

private:
    void get_closest();
    bool get_path();
    bool is_wall(int* walls);
};

class Control_dummy : Control {
    Model& model;
    int snake_index;
    Rabbit closest;

public:
    Control_dummy(Model &model) : model(model), closest({-1, -1}) {};
    virtual void handle_input(char c);
    virtual void set_index(int index) { snake_index = index; }

private:
    void get_closest();
    bool get_path();
    bool is_wall(int* walls);
};