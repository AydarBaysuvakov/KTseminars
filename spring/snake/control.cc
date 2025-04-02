#include "general.h"

Control * Control::getControl(char type, Model &model, int snake_index) {
    switch (type) {
        case 'w':
            return (Control*) new Control_wasd(model, snake_index);
        case 'a':
            return (Control*) new Control_arrow(model, snake_index);
        default:
            std::cout << "Неверный тип controller: " << type << std::endl;
            return nullptr;
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

void Control_arrow::handle_input(char c) {
    switch (c) {
        case '8': model.change_direction(snake_index, UP);    return;
        case '5': model.change_direction(snake_index, DOWN);  return;
        case '4': model.change_direction(snake_index, LEFT);  return;
        case '6': model.change_direction(snake_index, RIGHT); return;
        case 'q': model.GameOver();                           return;
    }
}
