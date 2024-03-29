#include <stdio.h>
#include <string.h>
#include <math.h>

#define LENGTH_ARRAY(array) (sizeof(array) / sizeof(array[0]))
#define BLANK_NAME -1
#define W 1

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

float roundXDecimals(const float number, const int decimals) {
    //printf("%.9f\n", number);
    int multiple = 1; // default
    for (int i = 0; i<decimals; i++) multiple*=10;
    float nF = (number - (int)number) * multiple;
    return (int)number + (round(nF) / multiple);
}

float calcule_u(int* white_cards, const float Z, const int n_ranks) {
    int accum_white_cards = 0; // e
    for (int i = 0; i<n_ranks; i++) accum_white_cards+=white_cards[i]+1;
    
    return floor(((Z-1)/accum_white_cards) * 1000000) / 1000000;
}

float* non_normalized_weights(int* white_cards, const float u_value, const int n_ranks) {
    float* weights = malloc((n_ranks) * sizeof(float));
    for (int i = 0; i<n_ranks; i++) {
        if (i == 0) {
            weights[i] = 1.00f;
            continue;
        }
        weights[i] = (float)weights[i - 1] + u_value * (white_cards[i-1] + 1);
        
    }
    
    for (int i = 0; i<n_ranks; i++) weights[i] =roundXDecimals(weights[i], 2);
    return weights;
}

float* normalized_weights_prime(float* v_weights, const int n_ranks, const float K) {
    float* weights = malloc((n_ranks) * sizeof(float));
    for (int i = 0; i<n_ranks; i++) {
        //printf("%.9f\n", v_weights[i]);
        weights[i] = 100 / K * v_weights[i];
        //printf("K=%.9lf v_weights=%.9lf\n", K, v_weights[i]);
    }
    return weights;
}

float get_total(float* weights, int* accum_ranks, const int n_ranks) {
    float accum = 0;
    for (int i = 0; i<n_ranks; i++) {
        //printf("w=%.9f * a=%d total=%f\n", weights[i], accum_ranks[i],accum_ranks[i] * weights[i]);
        accum+=accum_ranks[i] * weights[i];
    }
    return accum;
}

float truncate_to_one_decimal(float num) {
    return floor(num * 10) / 10;
}

void print_table_simos(int* criteries, int* positions, const int n_criteries, const float Z, float* non_normalized_weights, float* normalized_weights, float* r_1, float* r_2) {
    for (int i = 0; i<n_criteries; i++) {
        if (criteries[i] == BLANK_NAME) continue;
        printf("%d\t%d\t%.9f\t%.9f\t%.1f\t%.9f\t%.9f\n",positions[i]+1 ,criteries[i], non_normalized_weights[positions[i]], normalized_weights[positions[i]], truncate_to_one_decimal(normalized_weights[positions[i]]), r_1[positions[i]], r_2[positions[i]]);
    }
}

float* ratio_1(float* normalized_weights, const int n_ranks) {
    float* ratio = malloc((n_ranks) * sizeof(float));
    for (int i = 0; i<n_ranks; i++) {
        ratio[i] = (pow(10, -W) - (normalized_weights[i] - truncate_to_one_decimal(normalized_weights[i]))) / normalized_weights[i];
        //printf("pow=%f resta=%f divisor=%f normalized_weight=%f ratio=%f\n", pow(10, -1), (normalized_weights[i] - truncate_to_one_decimal(normalized_weights[i])),(pow(10, -W) - (normalized_weights[i] - truncate_to_one_decimal(normalized_weights[i]))), normalized_weights[i], ratio[i]);
    }
    return ratio;
}

float* ratio_2(float* normalized_weights, const int n_ranks) {
    float* ratio = malloc((n_ranks) * sizeof(float));
    for (int i = 0; i<n_ranks; i++) {
        ratio[i] = (normalized_weights[i] - truncate_to_one_decimal(normalized_weights[i])) / normalized_weights[i];
    }
    return ratio;
}

void swap(float *xp, float *yp) {
    float temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bubbleSort(float* arr, int n) {
    int i, j;
    for (i = 0; i < n-1; i++)
        
        // Last i elements are already in place
        for (j = 0; j < n-i-1; j++)
            if (arr[j] > arr[j+1])
                swap(&arr[j], &arr[j+1]);
}


float* get_weights(int* criteries, int* positions, const int n_criteries, const float Z) {
    int n_ranks = count_distinct(positions, n_criteries);
    int* n_white_cards = white_cards(positions, n_criteries, n_ranks-1);
    
    float u_value = calcule_u(n_white_cards, Z, n_ranks-1);
    
    float* n_weights = non_normalized_weights(n_white_cards, u_value, n_ranks);
    
    int accum_ranks[n_ranks];
    //initializing with 0
    for(int i = 0; i<n_ranks; i++) accum_ranks[i] = 0;
    
    for(int i = 0; i<n_criteries; i++) {
        if (criteries[i] == BLANK_NAME) continue;
        accum_ranks[positions[i]]++;
    }
    float total = get_total(n_weights, accum_ranks, n_ranks);
    
    float* normalize_weights = normalized_weights_prime(n_weights, n_ranks, total);
    float* r_1 = ratio_1(normalize_weights, n_ranks);
    float* r_2 = ratio_2(normalize_weights, n_ranks);

    print_table_simos(criteries, positions, n_criteries, Z, n_weights, normalize_weights, r_1, r_2);
    //for (int i = 0; i<n_ranks; i++) printf("%f\n", r_1[i]);
    //printf("\n");
    //bubbleSort(r_1, n_ranks);
    //for (int i = 0; i<n_ranks; i++) printf("%f\n", r_1[i]);
    
    // tmp return
    float* returned_weights = malloc((n_criteries) * sizeof(float));
    int i_fake = 0;
    for (int i = 0; i<n_criteries; i++) {
        if (positions[i] == BLANK_NAME)
            continue;
        
        //printf("f=%f i=%d\n",  normalize_weights[positions[i]], i);
        returned_weights[i_fake] = normalize_weights[positions[i]];
        i_fake++;
    }
    
    return returned_weights;
}

float* simos(int data[][2], const int n_criteries, const float Z) {
    int criteries[n_criteries];
    int positions[n_criteries];
    for (int i = 0; i < n_criteries; i++) {
        criteries[i] = data[i][0];
        positions[i] = data[i][1];
    }
    
    return get_weights(criteries, positions, n_criteries, Z);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    
    /*int criterios[] = {
        1, // 0
        2, // 0
        3, // 0
        4, // 1
        BLANK_NAME, // -1,
        5, // 2
        6, // 2
        7, // 2
        8,  // 2,
        9, // 3
        10, // 4
        11, // 4
        12 // 5
        
    };
    
    int positions[] = { 0, 0, 0, 1, -1, 2, 2, 2, 2,  3, 4, 4, 5};*/
    int data[][2] = {
        {1, 0},
        {2, 0},
        {3, 0},
        {4, 1},
        {BLANK_NAME, BLANK_NAME},
        {5, 2},
        {6, 2},
        {7, 2},
        {8, 2},
        {9, 3},
        {10, 4},
        {11, 4},
        {12, 5}
    };
    
    float* d = simos(data, 13, 6.5);
    
    for (int i = 0; i < 12; i++) printf("i=%d %f\n",i+1, d[i]);
    
    return 0;
}
