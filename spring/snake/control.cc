#include "general.h"

Control * Control::getControl(char type, Model &model) {
    switch (type) {
        case 'w':
            return (Control*) new Control_wasd(model);
        case 'a':
            return (Control*) new Control_arrow(model);
        case 's':
            return (Control*) new Control_smart(model);
        case 'd':
            return (Control*) new Control_dummy(model);
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

//Smart

void Control_smart::handle_input(char c) {
    get_closest();
    get_path();
}

void Control_smart::get_closest() {
    Segment head = model.get_snakes()[snake_index].get_head();
    int dist = INFINITY;
    for (auto rabbit: model.get_rabbits()) {
        if (abs(rabbit.get_x() - head.get_x()) + abs(rabbit.get_y() - head.get_y()) < dist) {
            closest = rabbit;
            dist = abs(rabbit.get_x() - head.get_x()) + abs(rabbit.get_y() - head.get_y());
        }
    }
}

bool Control_smart::get_path() {
    Snake snake = model.get_snakes()[snake_index];
    Segment head = snake.get_head();
    int wall[4] = {0, 0, 0, 0};

    if (is_wall((int*) &wall)) return (head == closest);

    if (head.get_x() == closest.get_x()) {
        if (head.get_y() < closest.get_y()) {
            if (snake.get_direction() == DOWN) {
                if (!wall[RIGHT]) model.change_direction(snake_index, RIGHT);
                else if (!wall[LEFT]) model.change_direction(snake_index, LEFT); 
            }
            else if (!wall[UP]) model.change_direction(snake_index, UP);
        } else {
            if (snake.get_direction() == UP) {
                if (!wall[RIGHT]) model.change_direction(snake_index, RIGHT);
                else if (!wall[LEFT]) model.change_direction(snake_index, LEFT); 
            }
            else if (!wall[DOWN]) model.change_direction(snake_index, DOWN);
        }
    } else if (head.get_x() < closest.get_x()) {
        if (snake.get_direction() == LEFT) {
            if (!wall[UP]) model.change_direction(snake_index, UP);
            else if (!wall[DOWN]) model.change_direction(snake_index, DOWN); 
        }
        else if (!wall[RIGHT]) model.change_direction(snake_index, RIGHT);
    } else if (head.get_x() > closest.get_x()) {
        if (snake.get_direction() == RIGHT) {
            if (!wall[UP]) model.change_direction(snake_index, UP);
            else if (!wall[DOWN]) model.change_direction(snake_index, DOWN); 
        }
        else if (!wall[LEFT]) model.change_direction(snake_index, LEFT);
    }
    return (head == closest);
}

bool Control_smart::is_wall(int* wall) {
    std::vector <Snake> snakes = model.get_snakes();
    Snake snake = snakes[snake_index];
    Segment head = snake.get_head();

    if (head.get_y() == windowsize::height - 1) wall[UP] = 1;
    if (head.get_y() == 0) wall[DOWN] = 1;
    if (head.get_x() == 0) wall[LEFT] = 1;
    if (head.get_x() == windowsize::width - 1) wall[RIGHT] = 1;

    for (auto s : snakes) {
        for (size_t i = 0; i < s.get_body().size(); ++i) {
            if (head.get_y() + 1 == s.get_body()[i].get_y() && head.get_x() == s.get_body()[i].get_x()) wall[UP] = 1;
            if (head.get_y() - 1 == s.get_body()[i].get_y() && head.get_x() == s.get_body()[i].get_x()) wall[DOWN] = 1;
            if (head.get_x() - 1 == s.get_body()[i].get_x() && head.get_y() == s.get_body()[i].get_y()) wall[LEFT] = 1;
            if (head.get_x() + 1 == s.get_body()[i].get_x() && head.get_y() == s.get_body()[i].get_y()) wall[RIGHT] = 1;
        }
    }

    if (snake.get_direction() == UP     && wall[UP]   || snake.get_direction() == DOWN   && wall[DOWN]) {
        if (!wall[LEFT] && !wall[RIGHT]) {
            if (rand() % 2) model.change_direction(snake_index, RIGHT);
            else model.change_direction(snake_index, LEFT);
        }
        else if (wall[LEFT])    model.change_direction(snake_index, RIGHT);
        else if (wall[RIGHT])   model.change_direction(snake_index, LEFT);
        return true;
    }
    if (snake.get_direction() == LEFT   && wall[LEFT] || snake.get_direction() == RIGHT  && wall[RIGHT]) {
        if (!wall[UP] && !wall[DOWN]) {
            if (rand() % 2) model.change_direction(snake_index, UP);
            else model.change_direction(snake_index, DOWN);
        }
        else if (wall[UP])      model.change_direction(snake_index, DOWN);
        else if (wall[DOWN])    model.change_direction(snake_index, UP);
        return true;
    }
    return false;
}

// Dummy

void Control_dummy::handle_input(char c) {
    if (closest == Rabbit(-1, -1)) get_closest();
    if (get_path()) get_closest();
}

void Control_dummy::get_closest() {
    Segment head = model.get_snakes()[snake_index].get_head();
    int dist = INFINITY;
    for (auto rabbit: model.get_rabbits()) {
        if (abs(rabbit.get_x() - head.get_x()) + abs(rabbit.get_y() - head.get_y()) < dist) {
            closest = rabbit;
            dist = abs(rabbit.get_x() - head.get_x()) + abs(rabbit.get_y() - head.get_y());
        }
    }
}

bool Control_dummy::get_path() {
    Snake snake = model.get_snakes()[snake_index];
    Segment head = snake.get_head();
    int wall[4] = {0, 0, 0, 0};

    if (is_wall((int*) &wall)) return (head == closest);

    if (head.get_x() == closest.get_x()) {
        if (head.get_y() < closest.get_y()) {
            if (snake.get_direction() == DOWN) {
                if (!wall[RIGHT]) model.change_direction(snake_index, RIGHT);
                else if (!wall[LEFT]) model.change_direction(snake_index, LEFT); 
            }
            else if (!wall[UP]) model.change_direction(snake_index, UP);
        } else {
            if (snake.get_direction() == UP) {
                if (!wall[RIGHT]) model.change_direction(snake_index, RIGHT);
                else if (!wall[LEFT]) model.change_direction(snake_index, LEFT); 
            }
            else if (!wall[DOWN]) model.change_direction(snake_index, DOWN);
        }
    } else if (head.get_x() < closest.get_x()) {
        if (snake.get_direction() == LEFT) {
            if (!wall[UP]) model.change_direction(snake_index, UP);
            else if (!wall[DOWN]) model.change_direction(snake_index, DOWN); 
        }
        else if (!wall[RIGHT]) model.change_direction(snake_index, RIGHT);
    } else if (head.get_x() > closest.get_x()) {
        if (snake.get_direction() == RIGHT) {
            if (!wall[UP]) model.change_direction(snake_index, UP);
            else if (!wall[DOWN]) model.change_direction(snake_index, DOWN); 
        }
        else if (!wall[LEFT]) model.change_direction(snake_index, LEFT);
    }
    return (head == closest);
}

bool Control_dummy::is_wall(int* wall) {
    std::vector <Snake> snakes = model.get_snakes();
    Snake snake = snakes[snake_index];
    Segment head = snake.get_head();

    if (head.get_y() == windowsize::height - 1) wall[UP] = 1;
    if (head.get_y() == 0) wall[DOWN] = 1;
    if (head.get_x() == 0) wall[LEFT] = 1;
    if (head.get_x() == windowsize::width - 1) wall[RIGHT] = 1;

    for (auto s : snakes) {
        for (size_t i = 0; i < s.get_body().size(); ++i) {
            if (head.get_y() + 1 == s.get_body()[i].get_y() && head.get_x() == s.get_body()[i].get_x()) wall[UP] = 1;
            if (head.get_y() - 1 == s.get_body()[i].get_y() && head.get_x() == s.get_body()[i].get_x()) wall[DOWN] = 1;
            if (head.get_x() - 1 == s.get_body()[i].get_x() && head.get_y() == s.get_body()[i].get_y()) wall[LEFT] = 1;
            if (head.get_x() + 1 == s.get_body()[i].get_x() && head.get_y() == s.get_body()[i].get_y()) wall[RIGHT] = 1;
        }
    }

    if (snake.get_direction() == UP     && wall[UP]) {
        if (wall[LEFT]) model.change_direction(snake_index, RIGHT);
        else model.change_direction(snake_index, LEFT);
        return true;
    }
    if (snake.get_direction() == DOWN   && wall[DOWN]) {
        if (wall[LEFT]) model.change_direction(snake_index, RIGHT);
        else model.change_direction(snake_index, LEFT);
        return true;
    }
    if (snake.get_direction() == LEFT   && wall[LEFT]) {
        if (wall[DOWN]) model.change_direction(snake_index, UP);
        else model.change_direction(snake_index, DOWN);
        return true;
    }
    if (snake.get_direction() == RIGHT  && wall[RIGHT]) {
        if (wall[DOWN]) model.change_direction(snake_index, UP);
        else model.change_direction(snake_index, DOWN);
        return true;
    }
    return false;
}