#include <stdio.h>
#include <string.h>
#include <math.h>

#define LENGTH_ARRAY(array) (sizeof(array) / sizeof(array[0]))
#define BLANK_NAME "###BLANK###"

int count_distinct(int arr[], int n)
{
    int res = 1;
    
    for (int i = 1; i < n; i++) {
        int j = 0;
        for (j = 0; j < i; j++)
            if (arr[i] == arr[j])
                break;
        
        if (i == j && arr[i] != -1 && arr[j] != -1)
            res++;
    }
    return res;
}

float* non_normalized_weights(int* accum_positions, int* accum_positions_criteries, const int n_positions) {
    float* weights = malloc((n_positions+1) * sizeof(float));
    for (int i = 0; i<n_positions; i++) {
        weights[i] = (float)accum_positions_criteries[i] / accum_positions[i];
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

float simos(char** criteries, int* positions, const int n_criteries) {
    int n_positions = count_distinct(positions, n_criteries);
    int accum_positions[n_positions];
    int accum_positions_criteries[n_positions];
    int accum_positions_number = 0;
    // Initialize array
    for (int i=0; i<n_positions; i++) {
        accum_positions[i] = 0;
        accum_positions_criteries[i] = 0;
    }
    
    for (int i = 0; i<n_criteries; i++) {
        if (strcmp(criteries[i], BLANK_NAME) == 0) continue;

        accum_positions[positions[i]]++;
        accum_positions_criteries[positions[i]]+=i+1;
        accum_positions_number+=i+1;
        
    }
    
    //Non normalized weights
    float* weights = non_normalized_weights(accum_positions, accum_positions_criteries, n_positions);
    //Normalize weights
    int* _weights = normalized_weights(weights, accum_positions_number, n_positions);
    //for(int i = 0; i<n_positions; i++) printf("value=%d\n", _weights[i]);
    int total = get_total(_weights, accum_positions, n_positions);
    return 0.1f;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    
    char* criterios[] = {
        "C", // 0
        "G", // 0
        "L", // 0
        "D", // 1
        "###BLANK###", // -1,
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
    
    printf("%.2f\n", simos(&criterios, &positions, LENGTH_ARRAY(criterios)));
    
    return 0;
}
