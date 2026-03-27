


// geneating arrays
void generate_uniform(int* arr, int n);
void generate_gaussian(int* arr, int n);
void generate_nearly_sorted(int* arr, int n);
void generate_reversed(int* arr, int n);

// utils
bool is_sorted(int* arr, int n);
double get_time();

// MergeSort
void merge(int arr[], int l, int m, int r, int temp[]);
void mergeSort(int arr[], int l, int r, int temp[]);
void mergeSortParallel(int arr[], int l, int r, int temp[]);
double measure_merge_sort(int repeats, int threads, int arr_size, char* impl, char* array_distribution, int seed);