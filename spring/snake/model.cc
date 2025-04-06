#include "general.h"

Model::Model() {
    Snake snake1(RED), snake2(BLUE);
    snakes.push_back(snake1); 
    //snakes.push_back(snake2); 
    rabbits.push_back({rand() % windowsize::width, rand() % windowsize::height});
    rabbits.push_back({rand() % windowsize::width, rand() % windowsize::height});
    rabbits.push_back({rand() % windowsize::width, rand() % windowsize::height});
}

void Model::update() {
    if (game_over) return;

    for (int i = 0; i < snakes.size(); i++) {
        game_over |= snakes[i].move(rabbits);
    }
}

Snake::Snake(Color color) : color(color) {
    int x = (rand() % (windowsize::width  / 2)) + (windowsize::width  / 4);
    int y = (rand() % (windowsize::height / 2)) + (windowsize::height / 4);
    body.push_back({x, y});

    int dir = rand() % 4;
    switch (dir) {
        case UP:
            direction = UP;
            body.push_back({x, y - 1});
            body.push_back({x, y - 2});
            break;
        case DOWN:
            direction = DOWN;
            body.push_back({x, y + 1});
            body.push_back({x, y + 2});
            break;
        case RIGHT:
            direction = RIGHT;
            body.push_back({x - 1, y});
            body.push_back({x - 2, y});
            break;
        case LEFT:
            direction = LEFT;
            body.push_back({x + 1, y});
            body.push_back({x + 2, y});
            break;
        
        default:
            break;
    }
}

void Snake::set_direction(Direction newDirection) { 
    if (direction == UP     && newDirection == DOWN)    return;
    if (direction == DOWN   && newDirection == UP)      return;
    if (direction == RIGHT  && newDirection == LEFT)    return;
    if (direction == LEFT   && newDirection == RIGHT)   return;
    direction = newDirection; 
}

int Snake::move(std::vector <Rabbit> &rabbits) {
    Segment head = get_head();
    Segment newHead(0, 0);

    switch (direction) {
    case UP:
        newHead.set_x(head.get_x());
        newHead.set_y(head.get_y() + 1);
        break;

    case RIGHT:
        newHead.set_x(head.get_x() + 1);
        newHead.set_y(head.get_y());
        break;

    case DOWN:
        newHead.set_x(head.get_x());
        newHead.set_y(head.get_y() - 1);
        break;

    case LEFT:
        newHead.set_x(head.get_x() - 1);
        newHead.set_y(head.get_y());
        break;
    }

    if (newHead.get_x() == -1 || newHead.get_x() == windowsize::width ||
        newHead.get_y() == -1 || newHead.get_y() == windowsize::height) {
        return 1;
    }

    for (size_t i = 1; i < body.size(); ++i) {
        if (newHead == body[i]) {
            return 1;
        }
    }

    int dead_rabbits;
    for (int i = 0; i < rabbits.size(); i++) {
        dead_rabbits = 0;
        if (newHead == rabbits[i]) {
            body.insert(body.begin(), newHead);
            rabbits[i].set_x(rand() % windowsize::width);
            rabbits[i].set_y(rand() % windowsize::height);
            dead_rabbits++;
        }
    }
    if (!dead_rabbits) {
        body.insert(body.begin(), newHead);
        body.pop_back();
    }

    return 0;
}