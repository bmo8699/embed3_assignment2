// ----------------------------------- framebf.h -------------------------------------
void framebf_init(int w, int h);
void drawPixelARGB32(int x, int y, unsigned int attr);
void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill);
void set_screen(char screen, int w, int h);

