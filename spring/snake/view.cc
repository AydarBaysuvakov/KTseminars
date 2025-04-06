#include "general.h"

// View

View * View::getView(char type, Model &model) {
    switch (type) {
        case 't':
            return (View*) new TView(model);
        case 'g':
            return (View*) new GView(model);
        default:
            std::cout << "Неверный тип view: " << type << std::endl;
            return nullptr;
    }
}

//TView

void TView::run() {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char c;

    drawframe();

    while (!model.isGameOver()) {
        for (auto snake: model.get_snakes()) {
            draw(snake.get_head().get_x(), snake.get_head().get_y(), SNAKE_BODY, snake.get_color());
            draw(snake.get_tail().get_x(), snake.get_tail().get_y(), EMPTY     , DEFAULT);
        }
        
        if (key_pressed()) c = getchar();
        for (auto control: controllers)
            control->handle_input(c);
        model.update();

        for (auto rabbit: model.get_rabbits())
            draw(rabbit.get_x(), rabbit.get_y(), RABBIT, GREEN);

        for (auto snake: model.get_snakes()) {
            draw(snake.get_head().get_x(), snake.get_head().get_y(), SNAKE_HEAD,  snake.get_color());
        }
        draw(100, 1, EMPTY, DEFAULT);

        usleep(PERIOD);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\e[" << windowsize::height / 2 + 2 << ";" << windowsize::width / 2 - 4 << "H" << "\e[31m\e[43mGame Over!\e[0m" << std::endl;
}

void TView::draw(int x, int y, char c, int color) {
    settxtcolor(color);
    gotoxy(x, y);
    std::cout << c << std::flush;
    resetcolor();
}

void TView::add_control(Control * control) {
    controllers.push_back(control);
}

void TView::alignscr() {
    struct winsize ws = {0, 0};
    ioctl(1, TIOCGWINSZ, &ws);
    windowsize::width  = (windowsize::width  < ws.ws_col - 2) ? windowsize::width  : ws.ws_col - 2;
    windowsize::height = (windowsize::height < ws.ws_row - 2) ? windowsize::height : ws.ws_row - 2;
}

void TView::clrscr() {
    std::cout << "\e[H\e[J" << std::flush;
}

void TView::gotoxy(int x, int y) {
    std::cout << "\e[" << windowsize::height - y + 1 << ";" << x + 2 << "H" << std::flush;
}

void TView::settxtcolor(int color) {
    std::cout << "\e[" << color << ";1m" << std::flush;
}

void TView::setbgrcolor(int color) {
    std::cout << "\e[" << color + 10 << "m" << std::flush;
}

void TView::resetcolor() {
    std::cout << "\e[0m" << std::flush;
}

void TView::drawframe() {
    alignscr();
    clrscr();
    settxtcolor(BLUE);
    for (int i = 0; i < windowsize::width + 2; ++i)
        std::cout << "*";
    std::cout << std::endl;

    for (int i = 0; i < windowsize::height; ++i) {
        std::cout << "*";
        for (int j = 0; j < windowsize::width; ++j)
            std::cout << " ";
        std::cout << "*\n";
    }

    for (int i = 0; i < windowsize::width + 2; ++i)
        std::cout << "*";
    resetcolor();
}

bool TView::key_pressed() {
    struct timeval tv;
    fd_set rdfs;

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    FD_ZERO(&rdfs);
    FD_SET(STDIN_FILENO, &rdfs);

    select(STDIN_FILENO + 1, &rdfs, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &rdfs);
}


//GView

void GView::run() {
    window.create(sf::VideoMode(windowsize::width * BLOCK_SIZE, windowsize::height * BLOCK_SIZE), "Snake!");
    sf::Keyboard::Key input;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyReleased) {
                input = event.key.code;
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }

        if (!model.isGameOver()) {
            window.clear();
            for (auto control: controllers)
                control->handle_input(convert_input(input));
            model.update();

            for (auto rabbit: model.get_rabbits())
                draw(rabbit.get_x(), rabbit.get_y(), 'c', WHITE);

            for (auto snake: model.get_snakes()) {
                int color = snake.get_color();
                for (auto body: snake.get_body())
                    draw(body.get_x(), body.get_y(), 'c', color);
            }

            usleep(PERIOD);
            window.display();
        }
    }
}

char GView::convert_input(sf::Keyboard::Key input) {
    if (input == sf::Keyboard::Q) return 'q';
    if (input == sf::Keyboard::W) return 'w';
    if (input == sf::Keyboard::A) return 'a';
    if (input == sf::Keyboard::S) return 's';
    if (input == sf::Keyboard::D) return 'd';
    if (input == sf::Keyboard::Num8) return '8';
    if (input == sf::Keyboard::Num4) return '4';
    if (input == sf::Keyboard::Num5) return '5';
    if (input == sf::Keyboard::Num6) return '6';
    if (input == sf::Keyboard::Up)      return 'A';
    if (input == sf::Keyboard::Down)    return 'C';
    if (input == sf::Keyboard::Right)   return 'B';
    if (input == sf::Keyboard::Left)    return 'D';
    return 0;
}

sf::Color GView::convert_color(int color) {
    switch (color) {
        case BLACK:     return sf::Color::Black;
        case RED:       return sf::Color::Red;
        case GREEN:     return sf::Color::Green;
        case YELLOW:    return sf::Color::Yellow;
        case BLUE:      return sf::Color::Blue;
        case MAGENTA:   return sf::Color::Magenta;
        case CYAN:      return sf::Color::Cyan;
        case WHITE:     return sf::Color::White;
    } 
    return sf::Color::Black; 
}

void GView::draw(int x, int y, char c, int color) {
    y = windowsize::height - y - 1;
    switch (c) {
        case 'c': {
            sf::CircleShape circle(BLOCK_SIZE / 2);
            circle.setFillColor(convert_color(color));
            circle.setPosition(x * BLOCK_SIZE, y * BLOCK_SIZE);
            window.draw(circle);
            return;
        }
        case 'r': {
            sf::RectangleShape rectangle({BLOCK_SIZE, BLOCK_SIZE});
            rectangle.setFillColor(convert_color(color));
            rectangle.setPosition(x * BLOCK_SIZE, y * BLOCK_SIZE);
            window.draw(rectangle);
            return;
        }
        default: return;
    }
}

void GView::add_control(Control * control) {
    controllers.push_back(control);
}