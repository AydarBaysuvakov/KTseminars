#include "general.h"

Control * Control::getControl(char type, Model &model, int snake_index) {
    switch (type) {
        case 'w':
            return (Control*) new Control_wasd(model, snake_index);
        case 'a':
            return (Control*) new Control_arrow(model, snake_index);
        case 's':
            return (Control*) new Control_smart(model, snake_index);
        case 'd':
            return (Control*) new Control_dummy(model, snake_index);
        default:
            std::cout << "Неверный тип controller: " << type << std::endl;
            return nullptr;
    }
}

void Control_arrow::handle_input(char c) {
    switch (c) {
        case 'A': model.change_direction(snake_index, UP);    return;
        case 'C': model.change_direction(snake_index, DOWN);  return;
        case 'D': model.change_direction(snake_index, LEFT);  return;
        case 'B': model.change_direction(snake_index, RIGHT); return;
        case 'q': model.GameOver();                           return;
    }
}

void Control_wasd::handle_input(char c) {
    switch (c) {
        case 'w': model.change_direction(snake_index, UP);    return;
        case 's': model.change_direction(snake_index, DOWN);  return;
        case 'a': model.change_direction(snake_index, LEFT);  return;
        case 'd': model.change_direction(snake_index, RIGHT); return;
        case 'q': model.GameOver();                           return;
    }
}

void Control_smart::handle_input(char c) {
    //
}

void Control_dummy::handle_input(char c) {
    //
}

Rabbit Control_dummy::get_closest() {
    Rabbit curent(0, 0);
    int dist = INFINITY;
    for (auto rabbit: model.get_rabbits()) {
        if (abs(rabbit.get_x() - curent.get_x()) + abs(rabbit.get_y() - curent.get_y()) < dist) {
            curent = rabbit;
            dist = abs(rabbit.get_x() - curent.get_x()) + abs(rabbit.get_y() - curent.get_y());
        }
    }
    return curent;
}