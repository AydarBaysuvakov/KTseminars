#include "general.h"

View * View::getView(char type, Model &model) {
    switch (type) {
        case 't':
            return (View*) new TView(model);
        default:
            std::cout << "Неверный тип view: " << type << std::endl;
            return nullptr;
    }
}


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
            draw(snake.get_tail().get_x(), snake.get_tail().get_y(), EMPTY     , snake.get_color());
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