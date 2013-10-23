
#include "PlantData.hpp"

#include <stddef.h>
#include "common.hpp"

using namespace cs354;

#define GREEN_1 0.1, 0.9, 0.1
#define GREEN_2 0.05, 0.45, 0.1
#define GREEN_3 0.1, 0.7, 0.1
#define GREEN_4 0.05, 0.35, 0.01
#define GREEN_5 0.2, 0.8, 0.15
#define GREEN_6 0.1, 0.4, 0.075
#define GREEN_7 0.7, 0.96, 0.35
#define GREEN_8 0.25, 0.48, 0.175

#define POINT_CV(v, c, index, offset)                                   \
    do {                                                                \
        glColor3f((c)[(index)*3+(offset)], (c)[(index)*3+(offset)+1],   \
                  (c)[(index)*3+(offset)+2]);                           \
        glVertex3f((v)[(index)*3+(offset)], (v)[(index)*3+(offset)+1],  \
                   (v)[(index)*3+(offset)+2]);                          \
    }while(0)

#define POINT_TV(v, t, index, offset)                                    \
    do {                                                                \
        glTexCoord2f((t)[(index)*2+(offset)], (t)[(index)*2+(offset)+1]); \
        glVertex3f((v)[(index)*3+(offset)], (v)[(index)*3+(offset)+1],  \
                   (v)[(index)*3+(offset)+2]);                          \
    }while(0)
#define TRIANGLE_CV(v, c, index)                        \
    do {                                                \
        POINT_CV(v, c, index, 0);                       \
        POINT_CV(v, c, index, 1);                       \
        POINT_CV(v, c, index, 2);                       \
    }while(0)
#define TRIANGLE_TV(v, t, index)                        \
    do {                                                \
        POINT_TV(v, t, index, 0);                       \
        POINT_TV(v, t, index, 1);                       \
        POINT_TV(v, t, index, 2);                       \
    }while(0)

#include <cstdio>
static void draw_color_triangles(BasicModel *model) {
    size_t size = model->size;
    float *vertices = model->vertex;
    float *colors = model->color;
    int *elements = model->elements;
    int e;
    glBegin(GL_TRIANGLES); {
        for(size_t i = 0; i < size; i += 1) {
            e = elements[i];
            glColor3f(colors[e*3], colors[e*3+1], colors[e*3+2]);
            glVertex3f(vertices[e*3], vertices[e*3+1], vertices[e*3+2]);
        }
    }glEnd();
}
static void draw_texture_triangles(BasicModel *model) {
}


static float _leaf_vertex[] = {
    0.0, 0.0, -0.25,
    1.0, 0.7, -0.25,
    1.3, 1.8, -0.25,
    1.0, 2.8, -0.25,
    0.0, 4.0, -0.25,
    -1.0, 2.8, -0.25,
    -1.3, 1.8, -0.25,
    -1.0, 0.7, -0.25,
    0.0, 0.0, 0.25,
    1.0, 0.7, 0.25,
    1.3, 1.8, 0.25,
    1.0, 2.8, 0.25,
    0.0, 4.0, 0.25,
    -1.0, 2.8, 0.25,
    -1.3, 1.8, 0.25,
    -1.0, 0.7, 0.25
};
static int _leaf_elements[] = {
    0, 1, 2,
    0, 2, 3,
    0, 3, 4,
    0, 4, 5,
    0, 5, 6,
    0, 6, 7,
    
    8, 9, 10,
    8, 10, 11,
    8, 11, 12,
    8, 12, 13,
    8, 13, 14,
    8, 14, 15,
    
    0, 1, 9,
    0, 9, 8,
    1, 2, 10,
    1, 10, 9,
    2, 3, 11,
    2, 11, 10,
    3, 4, 12,
    3, 12, 11,
    4, 5, 13,
    4, 13, 12,
    5, 6, 14,
    5, 14, 13,
    6, 7, 15,
    6, 15, 14,
    7, 0, 8,
    7, 8, 15
};

static float _leaf_color0[] = {
    GREEN_2, GREEN_1, GREEN_1, GREEN_1, GREEN_2, GREEN_1, GREEN_1, GREEN_1,
    GREEN_2, GREEN_1, GREEN_1, GREEN_1, GREEN_2, GREEN_1, GREEN_1, GREEN_1
};
static float _leaf_color1[] = {
    GREEN_4, GREEN_3, GREEN_3, GREEN_3, GREEN_4, GREEN_3, GREEN_3, GREEN_3,
    GREEN_4, GREEN_3, GREEN_3, GREEN_3, GREEN_4, GREEN_3, GREEN_3, GREEN_3
};
static float _leaf_color2[] = {
    GREEN_6, GREEN_5, GREEN_5, GREEN_5, GREEN_6, GREEN_5, GREEN_5, GREEN_5,
    GREEN_6, GREEN_5, GREEN_5, GREEN_5, GREEN_6, GREEN_5, GREEN_5, GREEN_5
};
static float _leaf_color3[] = {
    GREEN_8, GREEN_7, GREEN_7, GREEN_7, GREEN_8, GREEN_7, GREEN_7, GREEN_7,
    GREEN_8, GREEN_7, GREEN_7, GREEN_7, GREEN_8, GREEN_7, GREEN_7, GREEN_7
};

#define SIZE_OF(array) (sizeof(array) / sizeof((array)[0]))
static size_t _leaf_size = SIZE_OF(_leaf_elements);

struct BasicModel cs354::_n_leaf[4] = {
    {_leaf_size, _leaf_vertex, _leaf_color0, NULL, _leaf_elements,
     &draw_color_triangles, &draw_texture_triangles},
    {_leaf_size, _leaf_vertex, _leaf_color1, NULL, _leaf_elements,
     &draw_color_triangles, &draw_texture_triangles},
    {_leaf_size, _leaf_vertex, _leaf_color2, NULL, _leaf_elements,
     &draw_color_triangles, &draw_texture_triangles},
    {_leaf_size, _leaf_vertex, _leaf_color3, NULL, _leaf_elements,
     &draw_color_triangles, &draw_texture_triangles}
};

#define BROWN1 0.35, 0.175, 0.045
#define BROWN2 0.40, 0.175, 0.1

static float _branch_vertex[] = {
    1.0, 0.0,  0.0,
    0.7, 0.0,  0.7,
    0.0, 0.0,  1.0,
    -0.7, 0.0,  0.7,
    -1.0, 0.0,  0.0,
    -0.7, 0.0, -0.7,
    0.0, 0.0, -1.0,
    0.7, 0.0, -0.7,
    0.8,  6.0,  0.0,
    0.56, 6.0,  0.56,
    0.0,  6.0,  0.8,
    -0.56, 6.0,  0.56,
    -0.8,  6.0,  0.0,
    -0.56, 6.0, -0.56,
    0.0,  6.0, -0.8,
    0.56, 6.0, -0.56,
};
static int _branch_elements[] = {
    0, 8, 1,  8, 1, 9,
    1, 9, 2,  9, 2, 10,
    2, 10, 3, 10, 3, 11,
    3, 11, 4, 11, 4, 12,
    4, 12, 5, 12, 5, 13,
    5, 13, 6, 13, 6, 14,
    6, 14, 7, 14, 7, 15,
    7, 15, 0
};
static float _branch_color[] = {
    BROWN1, BROWN2, BROWN1, BROWN2,
    BROWN2, BROWN1, BROWN1, BROWN2,
    BROWN1, BROWN2, BROWN1, BROWN2,
    BROWN2, BROWN1, BROWN1, BROWN2
};
static size_t _branch_size = SIZE_OF(_branch_elements);
                                                            
struct BasicModel cs354::_n_branch = {
    _branch_size, _branch_vertex, _branch_color, NULL, _branch_elements,
    &draw_color_triangles, &draw_texture_triangles
};
