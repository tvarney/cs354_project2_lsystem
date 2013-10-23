
#include "View.hpp"

#include <stddef.h>

using namespace cs354;

View * View::current;

View::View() { }
View::~View() { }

void View::use() {
    if(current) {
        current->end();
    }
    current = this;
    current->init();
}

void View::timer(TimerFn_t timerfn, int value, int ms) {
    glutTimerFunc(ms, timerfn, value);
}

void View::InitViewSystem() {
    current = NULL;
        
    glutDisplayFunc(View::DisplayCallback);
    glutOverlayDisplayFunc(View::OverlayCallback);
    glutReshapeFunc(View::ReshapeCallback);
    glutKeyboardFunc(View::KeyboardCallback);
    glutSpecialFunc(View::SpecialCallback);
    glutMouseFunc(View::MouseCallback);
    glutMotionFunc(View::MotionCallback);
    glutPassiveMotionFunc(View::PassiveMotionCallback);
    glutEntryFunc(View::EntryCallback);
    glutIdleFunc(View::IdleCallback);
}

void View::EndViewSystem() {
    current = NULL;
    
    glutDisplayFunc(NULL);
    glutOverlayDisplayFunc(NULL);
    glutReshapeFunc(NULL);
    glutKeyboardFunc(NULL);
    glutSpecialFunc(NULL);
    glutMouseFunc(NULL);
    glutMotionFunc(NULL);
    glutPassiveMotionFunc(NULL);
    glutEntryFunc(NULL);
    glutIdleFunc(NULL);
}

void View::DisplayCallback() {
    if(current) {
        current->display();
    }
}
void View::OverlayCallback() {
    if(current) {
        current->overlay();
    }
}
void View::ReshapeCallback(int width, int height) {
    if(current) {
        current->reshape(width, height);
    }
}
void View::KeyboardCallback(unsigned char ch, int x, int y) {
    if(current) {
        current->keyPressed(ch);
    }
}
void View::SpecialCallback(int ch, int x, int y) {
    if(current) {
        current->keyPressed(ADJUST_GLUT_KEYCODE(ch));
    }
}
void View::MouseCallback(int button, int state, int x, int y) {
    if(current) {
        MouseButton mButton = MOUSE_BUTTON_LEFT;
        ButtonState mState = (state == GLUT_UP ? BUTTON_UP : BUTTON_DOWN);
        switch(button) {
        case GLUT_LEFT_BUTTON:
            button = MOUSE_BUTTON_LEFT;
            break;
        case GLUT_RIGHT_BUTTON:
            button = MOUSE_BUTTON_RIGHT;
            break;
        case GLUT_MIDDLE_BUTTON:
            button = MOUSE_BUTTON_MIDDLE;
            break;
        default:
            return;
        }
        
        current->mousePressed(mButton, mState);
    }
}
void View::MotionCallback(int x, int y) {
    if(current) {
        current->motion(x, y);
    }
}
void View::PassiveMotionCallback(int x, int y) {
    if(current) {
        current->passiveMotion(x, y);
    }
}
void View::EntryCallback(int state) {
    if(current) {
        current->entry(state);
    }
}
void View::IdleCallback() {
    if(current) {
        current->idle();
    }
}

BasicView::BasicView() {
    win.dim.width = glutGet(GLUT_WINDOW_WIDTH);
    win.dim.height = glutGet(GLUT_WINDOW_HEIGHT);
    win.aspect_ratio = double(win.dim.width) / double(win.dim.height);
}
BasicView::~BasicView() { }

/* Stubbed functions */
void BasicView::init() { }
void BasicView::end() { }
void BasicView::overlay() { }
void BasicView::reshape(int width, int height) {
    win.aspect_ratio = (double(width) / double(height));
    win.dim.width = width;
    win.dim.height = height;
}
void BasicView::keyPressed(int ch) { }
void BasicView::idle() { }

/* Implemented functions  */
void BasicView::mousePressed(MouseButton button, ButtonState state) {
    uint32_t bmask = (uint32_t)button;
    if(state == BUTTON_UP) {
        bmask = ~bmask;
    }
    mouse.state |= bmask;
}

void BasicView::passiveMotion(int x, int y) {
    mouse.pos.x = x;
    mouse.pos.y = y;
}

void BasicView::motion(int x, int y) {
    mouse.pos.x = x;
    mouse.pos.y = y;
}

void BasicView::entry(int state) {
    mouse.isInWindow = (state = GLUT_ENTERED);
}
