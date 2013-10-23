
#include "LSystem.hpp"

#include <cstdio>
#include <stdint.h>

using namespace cs354;

Rule::Rule() :
    weight(0), value("")
{ }
Rule::Rule(uint16_t weight, std::string &str) :
    weight(weight), value(str)
{ }
Rule::Rule(uint16_t weight, const char *string) :
    weight(weight), value(string)
{ }
Rule::~Rule() { }

Rule & Rule::operator=(const Rule &rhs) {
    weight = rhs.weight;
    value = rhs.value;
    return *this;
}

uint16_t Rule::getWeight() const {
    return weight;
}
const std::string & Rule::getValue() const {
    return value;
}

void Rule::print() const {
    std::printf("(%u, %s)", (uint32_t)weight, value.c_str());
}

Production::Production(const Production &prod) :
    ch(prod.ch), size(prod.size), total(prod.total), rules(prod.rules)
{ }
Production::Production(char ch, Rule *rules, size_t nrules) :
    ch(ch), size(nrules), total(0), rules(nrules, Rule())
{
    for(size_t i = 0; i < size; ++i) {
        this->rules[i] = rules[i];
        total += rules[i].getWeight();
    }
}
Production::~Production() { }

Production & Production::operator+=(const Production &rhs) {
    if(ch != rhs.ch) {
        return *this;
    }
    size += rhs.size;
    total += rhs.total;
    for(size_t i = 0; i < rhs.size; ++i) {
        rules.push_back(rhs.rules[i]);
    }
    
    return *this;
}
const Rule & Production::operator[](size_t index) const {
    return rules[index];
}

const std::string & Production::apply(Random &rand) const {
    uint32_t choice = rand.nextInt();
    choice %= total;
    size_t i;
    for(i = 0; i < size; ++i) {
        if(rules[i].getWeight() > choice) {
            return rules[i].getValue();
        }
        choice -= rules[i].getWeight();
    }
    return rules[i - 1].getValue();
}

char Production::token() const {
    return ch;
}
uint32_t Production::getTotalWeight() const {
    return total;
}
size_t Production::getNumRules() const {
    return size;
}
void Production::print() const {
    for(size_t i = 0; i < size; ++i) {
        std::printf("'%c' -> ", ch);
        rules[i].print();
        std::putchar('\n');
    }
}

LSystem::LSystem(const char *axiom, Production *productions, size_t nprod) :
    axiom(axiom), buffer(""), depth(0), randp(NULL)
{
    init_rules(productions, nprod, "");
    sanitize_axiom();
}
LSystem::LSystem(std::string &axiom, Production *productions, size_t nprod) :
    axiom(axiom), buffer(""), depth(0), randp(NULL)
{
    init_rules(productions, nprod, "");
    sanitize_axiom();
}

LSystem::LSystem(const char *axiom, Production *productions, size_t nprod,
                 const char *constants) :
    axiom(axiom), buffer(""), depth(0), randp(NULL)
{
    init_rules(productions, nprod, constants);
    sanitize_axiom();
}

LSystem::~LSystem() {
    for(size_t i = 0; i < 256; ++i) {
        if(rules[i] != NULL) {
            delete rules[i];
            rules[i] = NULL;
        }
    }
}

/*
const std::string & LSystem::apply(Random &rand, uint32_t depth) {
    if(depth == 0) {
        return axiom;
    }
    if(this->depth == depth && randp != NULL && rand == *randp) {
        return buffer;
    }
    
    if(randp != NULL) {
        delete randp;
        randp = NULL;
    }
    buffer = "";
    
    Random *r = rand.copy();
    this->randp = &rand;
    this->depth = depth;
    apply(axiom);
    randp = r;
    return buffer;
}
*/
void LSystem::apply(Random &rand, uint32_t depth, MatrixStack &stack,
                    LSystem_CallbackFn_t out)
{
    this->randp = &rand;
    this->depth = depth;
    this->output = out;
    this->stack = &stack;
    apply(axiom);
}

#define CHAR_OUT_BUFFER_LEN 64
static char * fmtchar(char buffer[CHAR_OUT_BUFFER_LEN], int ch) {
    if(std::isprint(ch)) {
        sprintf(buffer, "%c", (char)ch);
    }else {
        sprintf(buffer, "0x%x", (int)ch);
    }
    return buffer;
}
void LSystem::print() const {
    char buffer[CHAR_OUT_BUFFER_LEN];
    std::printf("LSystem:\n  Axiom := \"%s\"\n  Constants :=", axiom.c_str());
    for(size_t i = 0; i < 256; ++i) {
        if(constants[i] == true) {
            std::printf(" %s", fmtchar(buffer, (int)i));
        }
    }
    std::fputs("\n  Variables :=", stdout);
    for(size_t i = 0; i < 256; ++i) {
        if(rules[i] != NULL) {
            std::printf(" %s", fmtchar(buffer, (int)i));
        }
    }
    std::fputs("\n  Productions:\n", stdout);
    uint32_t total;
    size_t j, nrules;
    for(size_t i = 0; i < 256; ++i) {
        if(rules[i] != NULL) {
            total = (*rules[i]).getTotalWeight();
            nrules = (*rules[i]).getNumRules();
            for(j = 0; j < nrules; ++j) {
                std::printf("    %s (%3.3f) -> %s\n", fmtchar(buffer, (int)i),
                            double((*rules[i])[j].getWeight()) /
                            double(total) * 100.0,
                            (*rules[i])[j].getValue().c_str());
            }
        }
    }
}


/*
void LSystem::apply(const std::string &value) {
    if(value.c_str() == NULL) {
        return;
    }
    
    if(depth == 0) {
        buffer.append(value);
        return;
    }
    
    const char *data = value.c_str();
    size_t length = value.size();
    uint8_t ch;
    for(size_t i = 0; i < length; ++i) {
        ch  = data[i];
        if(rules[ch] != NULL) {
            depth -= 1;
            apply(rules[ch]->apply(*randp));
            depth += 1;
        }else if(constants[ch] == true) {
            buffer.append(1, data[i]);
        }else {
            std::fprintf(stderr,
                         "LSystem: Illegal character in rule '%c'\n",ch);
        }
    }
}
*/

void LSystem::apply(const std::string &value) {
    if(value.c_str() == NULL || value.empty()) {
        return;
    }
    
    size_t i;
    if(depth == 0) {
        for(i = 0; i < value.size(); ++i) {
            output(*stack, value[i]);
        }
        return;
    }
    const char *data = value.c_str();
    size_t length = value.size();
    uint8_t ch;
    for(size_t i = 0; i < length; ++i) {
        ch  = data[i];
        if(rules[ch] != NULL) {
            depth -= 1;
            apply(rules[ch]->apply(*randp));
            depth += 1;
        }else if(constants[ch] == true) {
            output(*stack, ch);
        }else {
            std::fprintf(stderr,
                         "LSystem: Illegal character in rule '%c'\n",ch);
        }
    }
}

void LSystem::sanitize_axiom() {
    std::string new_axiom = "";
    size_t length = axiom.size();
    if(length == 0 || axiom.c_str() == NULL) {
        axiom = "BL";
        return;
    }
    
    char buffer[CHAR_OUT_BUFFER_LEN];
    for(size_t i = 0; i < length; ++i) {
        if(rules[(uint8_t)axiom[i]] != NULL ||
           constants[(uint8_t)axiom[i]] != false)
        {
            new_axiom.append(1, axiom[i]);
        }else {
            std::fprintf(stderr, "Invalid LSystem character: %s\n",
                         fmtchar(buffer, (int)i));
        }
    }
    
    if(new_axiom.length() == 0) {
        axiom = "BL";
    }else {
        axiom = new_axiom;
    }
}
void LSystem::init_rules(Production *prod, size_t nprod, const char *c) {
    uint8_t ch;
    size_t i;
    for(i = 0; i < 256; ++i) {
        rules[i] = NULL;
        constants[i] = false;
    }
    for(i = 0; i < nprod; ++i) {
        ch = prod[i].token();
        if(rules[ch] != NULL) {
            std::fprintf(stderr,
                         "LSystem Collision -> Variable x Variable '%c'\n",
                         ch);
        }else {
            rules[ch] = new Production(prod[i]);
        }
    }
    if(c != NULL) {
        for(size_t i = 0; c[i] != '\0'; ++i) {
            ch = c[i];
            if(rules[ch] != NULL) {
                std::fprintf(stderr,
                             "LSystem Collision -> Variable x Constant '%c'\n",
                             ch);
            }else if(constants[ch]) {
                std::fprintf(stderr,
                             "LSystem Collision -> Constant x Constant '%c'\n",
                             ch);
            }else {
                constants[ch] = true;
            }
        }
    }
}
