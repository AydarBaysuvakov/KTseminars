class View;
class TView;
class GView;

enum Color {
    BLACK   = 30,
    RED     = 31,
    GREEN   = 32,
    YELLOW  = 33,
    BLUE    = 34,
    MAGENTA = 35,
    CYAN    = 36,
    WHITE   = 37,
    DEFAULT = 0
};

class View {
public:
    virtual void draw() = 0;
    static View * getView(char type, int size = 10);
};

class TView : View {
    int screen_size;

public:
    TView(int size) : screen_size(size) {}
    virtual void draw();

private:
    void clrscr();
    void gotoxy(int x, int y);
    void setbgrcolor(int color);
    void settxtcolor(int color);
    void resetcolor();
    void drawframe();
};