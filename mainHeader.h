


// geneating arrays
void generate_array(int *arr, int n);

// printing
void printArray(int A[], int size);

// time
double get_time();

// MergeSort
void merge(int arr[], int l, int m, int r);
void mergeSort(int arr[], int l, int r);
void mergeSortParallel(int arr[], int l, int r);
double measure_merge_sort(int repeats, int arr_size, char* impl);