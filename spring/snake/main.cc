#include "model.h"
#include "view.h"

int main(int argc, const char * argv[])
{
    if (argc != 2) return 0;
    View * v = View::getView(argv[1][0], 20);
    v->draw();
    delete v;
}