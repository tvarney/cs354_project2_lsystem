/*
 * plant.cpp
 * ---------
 * Contains function callbacks and bookkeeping used by glut.  Starter code
 * for Project 2.
 *
 * Group Members: Troy Varney
 */

#include <cstdio>
#include <unistd.h>

#include "common.hpp"
#include "Matrix.hpp"
#include "PlantView.hpp"
#include "View.hpp"

struct Window {
    int width, height;
    int x, y;
    int mode;
    const char *name;
} win = {
    800, 600,
    10, 10,
    GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH,
    "plant"
};

static const char _getopt_opts[] = "a:";
static const char _default_axiom[] = "BL";
int main (int argc, char** argv) {
    int c;
    const char *axiom = _default_axiom;
    while ((c = getopt (argc, argv, _getopt_opts)) != -1) {
        switch (c) {
        case 'a':
            axiom = optarg;
            break;
        case '?':
            if(optopt == 'a') {
                std::fprintf(stderr, "Error:: -a requires argument\n");
            }else {
                std::fprintf(stderr, "Error:: Unrecognized option: '%c'\n", c);
            }
            break;
        default:
            std::fprintf(stderr, "Error:: unrecognized option: '%c'\n", c);
            break;
        }
    }
    
    glutInit(&argc,argv);
    glutInitWindowSize(win.width, win.height);
    glutInitWindowPosition(win.x, win.y);
    glutInitDisplayMode(win.mode);
    glutCreateWindow(win.name);
    
    cs354::View::InitViewSystem();
    cs354::PlantView view(axiom);
    view.use();
    
    glutMainLoop();
    return 1;
}

/* end of plant.c */
