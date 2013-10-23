
#include "MatrixStack.hpp"

#include <cstdio>

using namespace cs354;

MatrixStack::MatrixStack() :
    stack(1, Matrix::Identity())
{
    current = &(stack.back());
}
MatrixStack::~MatrixStack() { }

void MatrixStack::set(const Matrix &mat) {
    *current = mat;
    current->use();
}
Matrix & MatrixStack::push() {
    return push(*current);
}
Matrix & MatrixStack::push(const Matrix &mat) {
    stack.push_back(mat);
    current = &(stack.back());
    return *current;
}

void MatrixStack::pop() {
    if(stack.size() > 1) {
        stack.pop_back();
        current = &(stack.back());
    }else {
        //std::printf("Setting matrix to identity\n");
        *current = Matrix::Identity();
    }
    current->use();
}

Matrix & MatrixStack::peek() const {
    return *current;
}

void MatrixStack::clear() {
    stack.clear();
    stack.push_back(Matrix::Identity());
    current = &(stack.back());
}

void MatrixStack::translate(float x, float y, float z) {
    current->translate(x, y, z);
    current->use();
}
void MatrixStack::scale(float x, float y, float z) {
    current->scale(x, y, z);
    current->use();
}
void MatrixStack::rotate(float rad, Axis axis) {
    current->rotate(rad, axis);
    current->use();
}
void MatrixStack::rotate(float rad, float x, float y, float z) {
    current->rotate(rad, x, y, z);
    current->use();
}

void MatrixStack::update() {
    current->use();
}
