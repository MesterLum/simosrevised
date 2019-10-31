#include <stdio.h>
#include <string.h>
#include <math.h>

#define LENGTH_ARRAY(array) (sizeof(array) / sizeof(array[0]))
#define BLANK_NAME "###BLANK###"

int count_distinct(int arr[], int n) {
    int res = 1;
    
    for (int i = 1; i < n; i++) {
        int j = 0;
        for (j = 0; j < i; j++)
            if (arr[i] == arr[j])
                break;
        
        //Value -1 means that is a White card so, it will ignore them
        if (i == j && arr[i] != -1 && arr[j] != -1)
            res++;
    }
    return res;
}

//e'[r]
int* white_cards(int* positions, const int n_criteries, const int n_ranks) {
    int* white_cards = malloc((n_ranks-1) * sizeof(int));
    for(int i = 0; i<n_ranks; i++) white_cards[i] = 0; // initializing
    int last_position = 0;

    for(int i = 0; i<n_criteries; i++) {
        if (positions[i] != -1)
            last_position = positions[i];
        
        if (positions[i] == -1)
            white_cards[last_position]++;
        
    }
    
    return white_cards;
}

float calcule_u(int* white_cards, const float Z, const int n_ranks) {
    int accum_white_cards = 0; // e
    for (int i = 0; i<n_ranks; i++) accum_white_cards+=white_cards[i]+1;
    
    return (Z-1)/accum_white_cards;
}

float* non_normalized_weights(int* white_cards, const float u_value, const int n_ranks) {
    float* weights = malloc((n_ranks) * sizeof(float));
    for (int i = 0; i<n_ranks; i++) {
        if (i == 0) {
            weights[i] = 1;
            continue;
        }
        weights[i] = (float)weights[i - 1] + u_value * (white_cards[i-1] + 1);
    }
    return weights;
}

int* normalized_weights(float* v_weights, const int accum_positions_number, const int n_positions) {
    int* weights = malloc((n_positions+1) * sizeof(float));
    for (int i = 0; i<n_positions; i++) {
        weights[i] = round((float)v_weights[i] / accum_positions_number * 100);
    }
    return weights;
}

int get_total(int* weights, int* accum_positions, const int n_positions) {
    int accum = 0;
    for (int i = 0; i<n_positions; i++) {
        accum+=accum_positions[i] * weights[i];
    }
    return accum;
}

float simos(char** criteries, int* positions, const int n_criteries, const float Z) {
    int n_ranks = count_distinct(positions, n_criteries);
    int* n_white_cards = white_cards(positions, n_criteries, n_ranks-1);
    float u_value = calcule_u(n_white_cards, Z, n_ranks-1);
    
    float* n_weights = non_normalized_weights(n_white_cards, u_value, n_ranks);
    for(int i = 0; i<n_ranks; i++) printf("%.2f\n", n_weights[i]);
    
    return 0.1f;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    
    char* criterios[] = {
        "C", // 0
        "G", // 0
        "L", // 0
        "D", // 1
        BLANK_NAME, // -1,
        "B", // 2
        "F", // 2
        "I", // 2
        "J",  // 2,
        "E", // 3
        "A", // 4
        "H", // 4
        "K" // 5
        
    };
    
    int positions[] = { 0, 0, 0, 1, -1, 2, 2, 2, 2, 3, 4, 4, 5};
    
    simos(&criterios, &positions, LENGTH_ARRAY(criterios), 6.5);
    
    return 0;
}
