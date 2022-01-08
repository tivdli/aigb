#ifndef cobl_h
#define cobl_h
#include <Arduino.h>


class COBL
{
    public:
        COBL(String prefix, int num_slider, int num_int_input, int num_float_input, int divider=2);
        COBL(String prefix, int num_slider, int num_int_input, bool use_color);
        static char *object_list[];
    private:    
        String prefix;
        int num_slider;
        int num_int_input;

        // float inputs are defined as int / divider, to keep values exact (and current system is not exact enough to go below .5 mL anyways)
        int num_float_input;
        int divider=2;
        bool use_color;
        int color_r, color_g, color_b;
        int &input_list[]

        
};
#endif