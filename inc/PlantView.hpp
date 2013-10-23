
#ifndef CS354_PROJECT2_PLANT_VIEW_HPP
#define CS354_PROJECT2_PLANT_VIEW_HPP

#include <stdint.h>
#include "LSystem.hpp"
#include "Matrix.hpp"
#include "MatrixStack.hpp"
#include "Random.hpp"
#include "Texture.hpp"
#include "View.hpp"

namespace cs354 {
    class PlantView : public BasicView {
    public:
        PlantView(const char *axiom);
        virtual ~PlantView();
       
        virtual void display();
        
        virtual void init();
        virtual void end();
        
        virtual void keyPressed(int ch);
        virtual void motion(int x, int y);
        virtual void reshape(int width, int height);
        
        void setDepth(uint32_t depth);
        void incDepth();
        void decDepth();
    protected:
        uint32_t depth;
        int32_t degrees;
        bool ortho;
        Matrix base_modelview;
        MatrixStack stack;
        Random *rand;
        LSystem system;
        
        struct {
            Texture *bg;
            Texture *branch;
            Texture *leaf[4];
        }tex;
        
        void drawPlant(uint32_t depth);
        void drawLeaf(int index);
        void drawBranch();
    };
};

#endif
