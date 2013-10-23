
#ifndef CS354_GENERIC_MATRIX_STACK_HPP
#define CS354_GENERIC_MATRIX_STACK_HPP

#include <vector>
#include "Matrix.hpp"

namespace cs354 {
    class MatrixStack {
    public:
        MatrixStack();
        ~MatrixStack();
        
        void set(const Matrix &mat);
        Matrix & push();
        Matrix & push(const Matrix &mat);
        void pop();
        Matrix & peek() const;
        void clear();
        
        void translate(float x, float y, float z);
        void scale(float x, float y, float z);
        void rotate(float rad, Axis axis);
        void rotate(float rad, float x, float y, float z);
        
        void update();
    private:
        Matrix *current;
        std::vector<Matrix> stack;
    };
}

#endif
