
#ifndef CS354_GENERIC_RANDOM_HPP
#define CS354_GENERIC_RANDOM_HPP

#include <stdint.h>

namespace cs354 {
    class Random {
    public:
        virtual ~Random();
        
        virtual void reseed(int32_t value) = 0;
        
        virtual int32_t nextInt() = 0;
        virtual int32_t nextInt(int32_t min, int32_t max) = 0;
        virtual double nextRational() = 0;
        
        virtual Random * copy() const = 0;
        virtual bool operator==(const Random &rand) const = 0;
    protected:
        Random();
    };
    
    class LCG_Random : public Random {
    public:
        LCG_Random(int32_t seed);
        virtual ~LCG_Random();
        
        virtual void reseed(int32_t value);
        
        virtual int32_t nextInt();
        virtual int32_t nextInt(int32_t min, int32_t max);
        virtual double nextRational();
        
        virtual Random * copy() const;
        virtual bool operator==(const Random &rand) const;
    protected:
        LCG_Random(int32_t s0, int32_t s1);
        int32_t seed[2];
    };
}

#endif
