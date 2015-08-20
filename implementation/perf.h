#ifndef __PERF_H__
#define __PERF_H__

#ifndef REPEAT      
    #define REPEAT 1000000
#endif 
#ifndef WARMUP 
    #define WARMUP REPEAT/4 
#endif 
 
    uint64_t start_clk,end_clk; 
    double total_clk; 

extern inline uint64_t get_Clks(void);

#define MEASURE(x)  for (int i=0; i< WARMUP; i++)   \
                        {x;}                        \
                    start_clk=get_Clks();           \
                    for (int i = 0; i < REPEAT; i++)\
                    {                               \
                        {x;}                        \
                    }                               \
                    end_clk=get_Clks();             \
                    total_clk=(double)(end_clk-start_clk)/REPEAT; 


#endif/*__PERF_H__*/
