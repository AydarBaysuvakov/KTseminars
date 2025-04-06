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
    View * view = View::getView(type, model);
    Control * control1 = Control::getControl('d', model);
    Control * control2 = Control::getControl('s', model);
    Control * control3 = Control::getControl('s', model);
    view->add_control(control1);
    view->add_control(control2);
    view->add_control(control3);
    view->run();
    delete view;
    delete control1;
    delete control2;
    delete control3;
}