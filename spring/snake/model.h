#pragma once

class Segment {
    int x;
    int y;

public:
    Segment(int x = 0, int y = 0) : x(x), y(y) {}
    ~Segment() {}

    bool operator==(const Segment& other) const {
        return x == other.x && y == other.y;
    }

    int  get_x() { return x; }
    void set_x(int X) { x = X; }
    int  get_y() { return y; }
    void set_y(int Y) { y = Y; }
};

typedef Segment Rabbit;

class Snake {
    Direction direction;
    Color color;
    std::vector <Segment> body;

public:
    Snake(Color color);
    int move(std::vector <Rabbit> &rabbits, std::vector <Snake> &snakes);

    int  get_direction() { return direction; }
    int  get_color()   { return color; };
    void set_direction(Direction newDirection);

    std::vector <Segment> &get_body() { return body; }
    Segment get_head() { if (body.empty()) return Segment(-1, -1) ; return body.front(); }
    Segment get_tail() { if (body.empty()) return Segment(-1, -1) ; return body.back(); }
};


class Model {
    int game_over = 0;
    int dead_snakes = 0;

    std::vector <Snake>  snakes;
    std::vector <Rabbit> rabbits;

public:
    Model();
    void update(); 

    std::vector <Snake>  get_snakes()  const { return snakes; }
    std::vector <Rabbit> get_rabbits() const { return rabbits; }

    int isGameOver() const { return game_over; }
    void GameOver() {game_over = 1;}
    void change_direction(int index, Direction dir) { if (index < snakes.size()) snakes[index].set_direction(dir); }
    void add_rabbit() { rabbits.push_back({rand() % windowsize::width, rand() % windowsize::height}); }
    void add_snake();
};