
#include "PlantView.hpp"

#include <cstdlib>
#include <cstdio>
#include <string>
#include <time.h>
#include "Image.hpp"
#include "ImageIO.hpp"
#include "PlantData.hpp"
#include "Texture.hpp"

using namespace cs354;

static const char _constants[] = "[]xXyYzZsSB";
static Rule rule_L[] = {
    Rule(1, "s[xBE][XBE][zBE][ZBE]")
};
static Rule rule_E[] = {
    Rule(1, "sB[yxBL]"),
    Rule(2, "s[xBA][YBF]"),
    Rule(2, "s[XBE][yBF][xyBA]")
};
static Rule rule_A[] = {
    Rule(1, "s[BL]"),
    Rule(2, "s[xBE][XBF]"),
    Rule(1, "s[xBE][YBL]")
};
static Rule rule_F[] = {
    Rule(1, "s[ZxBL]"),
    Rule(3, "s[xBA]"),
    Rule(2, "s[zyBE]")
};

static Production productions[4] = {
    Production('L', rule_L, 1),
    Production('E', rule_E, 3),
    Production('A', rule_A, 3),
    Production('F', rule_F, 3)
};

static void lsystem_out(MatrixStack &stack, char ch) {
    switch(ch) {
    case 'B':
        _n_branch.drawColor(&_n_branch);
        stack.translate(0.0, 6.0, 0.0);
        break;
    case 'L':
        _n_leaf[0].drawColor(&(_n_leaf[0]));
        //drawLeaf(0);
        break;
    case 'E':
        _n_leaf[1].drawColor(&(_n_leaf[1]));
        //drawLeaf(1);
        break;
    case 'A':
        _n_leaf[2].drawColor(&(_n_leaf[2]));
        //drawLeaf(2);
        break;
    case 'F':
        _n_leaf[3].drawColor(&(_n_leaf[3]));
        //drawLeaf(3);
        break;
    case '[':
        stack.push();
        break;
    case ']':
        stack.pop();
        break;
    case 'x':
        stack.rotate(0.5, AXIS_X);
        break;
    case 'X':
        stack.rotate(-0.5, AXIS_X);
        break;
    case 'y':
        stack.rotate(0.5, AXIS_Y);
        break;
    case 'Y':
        stack.rotate(-0.5, AXIS_Y);
    case 'z':
        stack.rotate(0.5, AXIS_Z);
        break;
    case 'Z':
        stack.rotate(-0.5, AXIS_Z);
        break;
    case 's':
        stack.scale(0.8, 0.8, 0.8);
        break;
    case 'S':
        stack.scale(1.1, 1.1, 1.1);
        break;
    default:
        printf("Invalid letter in LSystem: \"%c\"\n", ch);
    }
}

PlantView::PlantView(const char *axiom) :
    BasicView(), depth(0), degrees(0), ortho(true),
    rand(new LCG_Random((int32_t)(time(NULL)))),
    system(axiom, productions, 4, _constants)
{
    system.print();
}
PlantView::~PlantView() { }

void PlantView::display() {
    glEnable(GL_TEXTURE_2D);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    if(tex.bg != NULL) {
        glBindTexture(GL_TEXTURE_2D, tex.bg->getHandle());
        glMatrixMode(GL_PROJECTION);
        Matrix::Identity().use();
        glOrtho(-1, 1, -1, 1, -1, 1);
        glDisable(GL_DEPTH_TEST);
        
        glMatrixMode(GL_MODELVIEW);
        Matrix::Identity().use();
        double left = (1.0 / 360.0) * (degrees % 360);
        double right = (80.0 / 360.0) + left;
        glBegin(GL_TRIANGLES); {
            glColor3f(1.0, 1.0, 1.0);
            
            glTexCoord2f(left, 1); glVertex3f(-1, 1, -1);
            glTexCoord2f(right, 0); glVertex3f(1, -1, -1);
            glTexCoord2f(left, 0); glVertex3f(-1, -1, -1);
            
            glTexCoord2f(left, 1); glVertex3f(-1, 1, -1);
            glTexCoord2f(right, 1); glVertex3f(1, 1, -1);
            glTexCoord2f(right, 0); glVertex3f(1, -1, -1);
        }glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    glEnable(GL_DEPTH_TEST);
    /* Update projection matrix */
    
    glMatrixMode(GL_MODELVIEW);
    stack.clear();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(ortho) {
        glOrtho(-40, 40, -40, 40, -40, 40);
        glMatrixMode(GL_MODELVIEW);
        stack.clear();
        /* Ortho rotation */
        stack.set(Matrix::Identity());
        stack.translate(0, 0, -15);
    }else {
        gluPerspective(80, win.aspect_ratio, 1.0, 100);
        glMatrixMode(GL_MODELVIEW);
        stack.clear();
        stack.set(Matrix::LookAt(Vector3f(0,0,50)));
    }
    stack.translate(0, -20, 0);
    stack.rotate(degrees * 0.017453292519943295, AXIS_Y);
    stack.scale(1.5,1.5,1.5);
    drawPlant(depth);
    
    glFlush();
    glutSwapBuffers();
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
}

static inline Texture * _loadTexture(const char *fname) {
    Image *img = NULL;
    Texture *ntex = NULL;
    try {
        img = ImageIO::Load(fname);
        if(!img) {
            std::fprintf(stderr, "Could not load %s\n",fname);
            return NULL;
        }
    }catch(...) {
        std::fprintf(stderr, "Could not load %s\n", fname);
        return NULL;
    }

    try {
        ntex = new Texture(*img);
        delete img;
    }catch(...) {
        std::fprintf(stderr, "Could not create branch texture.\n");
    }
    return ntex;
}

void PlantView::init() {
    glDisable(GL_CULL_FACE);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    tex.bg = _loadTexture("./data/bg.png");
    tex.branch = _loadTexture("./data/branch.png");
    tex.leaf[0] = _loadTexture("./data/leaf0.png");
    tex.leaf[1] = _loadTexture("./data/leaf1.png");
    tex.leaf[2] = _loadTexture("./data/leaf2.png");
    tex.leaf[3] = _loadTexture("./data/leaf3.png");
}

void PlantView::end() {
    if(tex.branch) {
        delete tex.branch;
    }
    if(tex.leaf[0]) {
        delete tex.leaf[0];
    }
    if(tex.leaf[1]) {
        delete tex.leaf[1];
    }
    if(tex.leaf[2]) {
        delete tex.leaf[2];
    }
    if(tex.leaf[3]) {
        delete tex.leaf[3];
    }
    tex.branch = tex.leaf[0] = tex.leaf[1] = tex.leaf[2] = tex.leaf[3] = NULL;
}

void PlantView::keyPressed(int ch) {
    switch(ch) {
    case 'a':
        if(depth < 1000) {
            depth += 1;
            std::printf("L-System Depth: %d\n", depth);
            glutPostRedisplay();
        }
        break;
    case 's':
        if(depth > 1) {
            depth -= 1;
            std::printf("L-System Depth: %d\n", depth);
            glutPostRedisplay();
        }
        break;
    case 'p':
    case 'P':
        /* Invert Ortho state */
        ortho = !ortho;
        std::printf("Projection: %s\n", (ortho ? "Orthogonal":"Perspective"));
        glutPostRedisplay();
        break;
    case 'r':
    case 'R':
        rand->reseed((int32_t)time(NULL));
        glutPostRedisplay();
        break;
    case 'q':
        std::exit(0);
        break;
    case '/':
        degrees = (degrees + 5);
        if(degrees > 360) {
            degrees = degrees - 360;
        }
        
        stack.peek() = base_modelview;
        stack.rotate(degrees * 0.0174532925, AXIS_Y);
        /*
        stack.peek() *= _rotate_ccw;
        stack.update();
        */
        glutPostRedisplay();
        break;
    case '?':
        degrees -= 5;
        if(degrees < 0) {
            degrees = 360 + degrees;
        }
        stack.peek() = base_modelview;
        stack.rotate(degrees * -0.0174532925, AXIS_Y);
        /*
        stack.peek() *= _rotate_cw;
        stack.update();
        */
        glutPostRedisplay();
        break;
    }
}

void PlantView::motion(int x, int y) {
    int diff = x - mouse.pos.x;
    degrees += diff;
    while(degrees > 360) {
        degrees -= 360;
    }
    while(degrees < 0) {
        degrees += 360;
    }
    BasicView::motion(x, y);
    glutPostRedisplay();
}

void PlantView::reshape(int width, int height) {
    BasicView::reshape(width, height);
    glViewport(0, 0, width, height);
}

void PlantView::setDepth(uint32_t depth) {
    this->depth = depth;
}
void PlantView::incDepth() {
    depth++;
}
void PlantView::decDepth() {
    depth--;
}

void PlantView::drawPlant(uint32_t depth) {
    Random *nrand = rand->copy();
    system.apply(*nrand, depth, stack, lsystem_out);
    
    /*
    Random *nrand = rand->copy();
    const std::string &plan = system.apply(*nrand, depth);
    
    for(size_t i = 0; i < plan.size(); ++i) {
        switch(plan[i]) {
        case 'B':
            drawBranch();
            stack.translate(0.0, 6.0, 0.0);
            break;
        case 'L':
            drawLeaf(0);
            break;
        case 'E':
            drawLeaf(1);
            break;
        case 'A':
            drawLeaf(2);
            break;
        case 'F':
            drawLeaf(3);
            break;
        case '[':
            stack.push();
            break;
        case ']':
            stack.pop();
            break;
        case 'x':
            stack.rotate(0.5, AXIS_X);
            break;
        case 'X':
            stack.rotate(-0.5, AXIS_X);
            break;
        case 'y':
            stack.rotate(0.5, AXIS_Y);
            break;
        case 'Y':
            stack.rotate(-0.5, AXIS_Y);
        case 'z':
            stack.rotate(0.5, AXIS_Z);
            break;
        case 'Z':
            stack.rotate(-0.5, AXIS_Z);
            break;
        case 's':
            stack.scale(0.8, 0.8, 0.8);
            break;
        case 'S':
            stack.scale(1.1, 1.1, 1.1);
            break;
        default:
            printf("Invalid letter in LSystem: \"%c\"\n", plan[i]);
        }
    }
    */
}

void PlantView::drawLeaf(int index) {
    _n_leaf[index].drawColor(&(_n_leaf[index]));
}

void PlantView::drawBranch() {
    _n_branch.drawColor(&_n_branch);
}
