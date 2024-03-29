/******************************************************************************
 *                                  IMS 2022
 *                               2 Teplárenství 
 * 
 *      Authors: Erik Hrubý         (xhruby30)
 *               Alžbeta Hricovová  (xhrico00)
 *      
 * 
 *                        Last change: 04. 12. 2022
 *****************************************************************************/
#include <stdio.h>
#include <math.h>
#include <getopt.h>

#define ITERATIONS 100 // 100 iterations per year
#define STEP 0.01
#define START_TIME  0

typedef enum {
    OZE,
    OZE_PRICE,
    NATURAL_GAS,
    NATURAL_GAS_PRICE,
    COAL,
    COAL_PRICE
} type;

double differentialFormula(double time, type type, int iteration) {
    double result;
    switch(type) {
        case OZE:                                    
            result = 2677.7325 * exp(0.0376 * time);
            break;
        case OZE_PRICE:
            if (iteration < 800 || iteration > 1100) {                                
                result = 5.3162 * exp(0.0121 * time);
            } else {                                
                result = 37.0511 * exp(0.0785 * time);
            }                       
            break;
        case NATURAL_GAS:                            
            result = 6995 * cos(1.3999 * time - 1);                       
            break;
        case NATURAL_GAS_PRICE:
        if (iteration < 800 || iteration > 1100)                        
            result = -6.3888 * exp(-0.0114 * time);
        else {                        
            result = 65.0971 * exp(0.1539 * time);
        }
            break;
        case COAL:                                
            result = -2066.7603 * exp(-0.0512 * time);
            break;
        case COAL_PRICE:
            if (iteration < 800 || iteration > 1100) {                 
                result = 12.7762 * exp(0.0288 * time);
            } else {                
                result = 48.6960 * exp(0.0946 * time);
            }           
            break;
    }
    return result;       
}

void rungeKutta(double time, double *values, type type) {       
    for (int i = 1; i <= 3; ++i) {
        double k1 = STEP * differentialFormula(time, type, i);
        double k2 = STEP * differentialFormula(time + STEP / 2, type, i);
        double k3 = STEP * differentialFormula(time + STEP / 2, type, i);
        double k4 = STEP * differentialFormula(time + STEP, type, i);        

        values[i] = values[i - 1] + (k1 + 2*k2 + 2*k3 + k4) / 6;
        time += STEP;
    }    
}

void adamsBashforth(double time, double *values, type type, int iterations) {
    time += STEP * 4;
    for (int i = 4; i < iterations; i++) {
        values[i] = values[i - 1] + (0.01 / 24) * 
        (55 * differentialFormula(time - 0.01, type, i) 
        -59 * differentialFormula(time - 0.02, type, i) 
        +37 * differentialFormula(time - 0.03, type, i) 
        -9  * differentialFormula(time - 0.04, type, i));               
        time = time + 0.01;
    }
}

void printResult(int year, double *values, type type, int iterations) {
    char * header;
    char * row;
    switch(type) {
        case OZE:
            header = "   Konečná spotřeba-OZE               ";
            row = "         TJ              ";            
            break;
        case OZE_PRICE:
            header = "   Priemerna cena-OZE                  ";
            row = "         Kč/GJ           ";
            break;
        case NATURAL_GAS:
            header = "   Konečná spotřeba-Zemny plyn        ";
            row = "         TJ              ";                       
            break;
        case NATURAL_GAS_PRICE:
            header = "   Priemerna cena-Zemny plyn           ";
            row = "         Kč/GJ           ";
            break;
        case COAL:
            header = "   Konečná spotřeba-Uhlie             ";
            row = "         TJ              ";
            break;
        case COAL_PRICE:
            header = "   Priemerna cena-Uhlie                 ";
            row = "         Kč/GJ           ";
            break;
    }

    printf("++++++++++++++++++++++++++++++++++++++++\n+%s+\n++++++++++++++++++++++++++++++++++++++++\n", header);
    printf("+   Za rok   +%s+\n", row);
    printf("++++++++++++++++++++++++++++++++++++++++\n");    
    
    printf("+   %d     +       %lf \n",  year, values[(int)START_TIME]);
    year++;    
    printf("----------------------------------------\n");
    for (int i = (int)START_TIME + 100; i < iterations; ++i) {
         if ((i - 4) % 100 == 0) {
            printf("+   %d     +       %lf \n",  year, values[i]);
            printf("----------------------------------------\n");
            year++;
        }       
    }

}

int main(int argc, char **argv) {
    double startValue;
    int iterations;
    int year;    
    type type;

    int option;
    while ((option = getopt(argc, argv, "abcABC")) != -1) {
        switch(option) {
            case 'a':                
                type = OZE;
                startValue = 72890;
                iterations = ITERATIONS * 15 + 5;
                year = 2015;
                break;
            case 'A':
                type = OZE_PRICE;
                startValue = 434.09;
                iterations = ITERATIONS * 18 + 5;
                year = 2012;
                break;             
            case 'b':                
                type = NATURAL_GAS;
                startValue = 47627;
                iterations = ITERATIONS * 15 + 5;
                year = 2015;
                break;
            case 'B':
                type = NATURAL_GAS_PRICE;
                startValue = 551.13;
                iterations = ITERATIONS * 18 + 5;
                year = 2012;
                break;
            case 'c':
                type = COAL;
                startValue = 37823;
                iterations = ITERATIONS * 15 + 5;
                year = 2015;
                break;
            case 'C':
                type = COAL_PRICE;
                startValue = 448.88;
                iterations = ITERATIONS * 18 + 5;
                year = 2012;
                break;                                   
        }
    }

    double values[iterations];        
    values[(int)START_TIME] = startValue;

    rungeKutta(START_TIME, values, type);    
    adamsBashforth(START_TIME, values, type, iterations);
    printResult(year, values, type, iterations);      
}


