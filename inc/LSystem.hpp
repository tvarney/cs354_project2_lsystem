
#ifndef CS354_PROJECT2_LSYSTEM_HPP
#define CS354_PROJECT2_LSYSTEM_HPP

#include <stdint.h>
#include <string>
#include <vector>
#include "MatrixStack.hpp"
#include "Random.hpp"

namespace cs354 {
    class Rule {
    public:
        Rule();
        Rule(uint16_t weight, std::string &str);
        Rule(uint16_t weight, const char *string);
        ~Rule();
        
        Rule & operator=(const Rule &rhs);
        
        uint16_t getWeight() const;
        const std::string & getValue() const;
        
        void print() const;
    private:
        uint16_t weight;
        std::string value;
    };
    
    class Production {
    public:
        Production(const Production &prod);
        Production(char ch, Rule *rules, size_t nrules);
        ~Production();
        
        Production & operator+=(const Production &rhs);
        const Rule & operator[](size_t index) const;
        
        const std::string & apply(Random &rand) const;
        char token() const;
        uint32_t getTotalWeight() const;
        size_t getNumRules() const;
        void print() const;
    protected:
        char ch;
        size_t size;
        uint32_t total;
        std::vector<Rule> rules;
    };
    
    typedef void (*LSystem_CallbackFn_t)(MatrixStack &, char);
    
    class LSystem {
    public:
        LSystem(const char *axiom, Production *productions, size_t nprod);
        LSystem(std::string &axiom, Production *productions, size_t nprod);
        LSystem(const char *axiom, Production *productions, size_t nprod,
                const char *constants);
        ~LSystem();
        
        //const std::string & apply(Random &rand, uint32_t depth);
        void apply(Random &rand, uint32_t depth, MatrixStack &stack,
                   LSystem_CallbackFn_t out);
        void print() const;
    private:
        void sanitize_axiom();
        void init_rules(Production *prod, size_t nprod, const char *c);
        void apply(const std::string &value);
        //void apply(const std::string &value, LSystem_CallbackFn_t output);
        
        Production *rules[256];
        bool constants[256];
        std::string axiom, buffer;
        /* No point in putting these on the stack EVERY time we recurse */
        uint32_t depth;
        Random *randp;
        MatrixStack *stack;
        LSystem_CallbackFn_t output;
    };
}

#endif
