#include "general.h"

namespace windowsize {
    int width = WIDTH;
    int height = HEIGTH;
}

//TODO: bots(smart, dump), sfml

int main(int argc, const char * argv[])
{
    srand(time(0));
    char type = TEXT_MODE;
    if (argc > 1) type = argv[1][0];
    if (argc > 2) windowsize::height = (windowsize::width = atoi(argv[2]));
    if (argc > 3) windowsize::height = atoi(argv[3]);

    Model model;
    Control * control1 = Control::getControl('w', model, 0);
    Control * control2 = Control::getControl('a', model, 1);
    View * view = View::getView(type, model);
    view->add_control(control1);
    view->add_control(control2);
    view->run();
    delete view;
    delete control1;
    delete control2;
}