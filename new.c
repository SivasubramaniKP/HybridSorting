#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
typedef struct String {
    char s[100];
    int l;
} String;

typedef struct StringArray {
    String * strings;
    int l;
} StringArray;

void merge(StringArray *array, int left, int mid, int right);
void MergeSort(StringArray *array, int left, int right);
void shellSort(StringArray *array);
void insertionSort(StringArray *array);
void printStringArray(StringArray *array);
int getInt(char *s);
int len(char *s);
StringArray *getStringArray();
void HybridSort(StringArray *array);
int countInversions(StringArray *array);
float getDisorderRatio(StringArray *array);
StringArray *readFromFile(FILE *fp, int l);

StringArray * buildStringArray ( String * givenStrings, int l ) {
    StringArray * array = ( StringArray * )malloc( sizeof(StringArray) );
    array->l = l;
    array->strings = ( String * )malloc( sizeof(String) * l );
    for ( int i = 0; i < l; i++ ) {
        array->strings[i] = givenStrings[i];
    } 
    return array;
}

void merge ( StringArray * array, int left, int mid, int right ) {
    int l1 = mid - left + 1;
    int l2 = right - mid;
    int isMergeExecuted = 0;
    int k = left;
    String * leftArray = ( String * )malloc( sizeof(String) * l1 );
    String * rightArray = ( String * )malloc( sizeof(String) * l2 );
    for ( int i = 0 ; i < l1; i++ ){
        leftArray[i] = array->strings[ left + i ];
    }
    for ( int j = 0; j < l2; j++ ) {
        rightArray[j] = array->strings[ mid + 1 + j ];
    }
    StringArray * leftStringArray = buildStringArray(leftArray, l1);
    StringArray * rightStringArray = buildStringArray(rightArray, l2);
    float ratioLeft = getDisorderRatio(leftStringArray);
    float ratioRight = getDisorderRatio(rightStringArray);
    if ( l1 <= 10 && ratioLeft < 0.1 ) {
        printf("Switching to Insertion Sort for left SubArray with l1 = %dwith disorder ratio %f\n", l1, ratioLeft);
        insertionSort(leftStringArray);
    }
    else if ( l1 < 50 && l1 > 10 && ratioLeft <= 0.3 ) {
        printf("Switching to Shell Sort for left SubArray with l2 = %d with disorder ratio %f\n", l1, ratioLeft);
        shellSort(leftStringArray);
    }
    if ( l2 <= 10 && ratioRight < 0.1 ) {
        printf("Switching to Insertion Sort for right SubArray with l1 = %d with disorder ratio %f\n", l2, ratioRight);
        insertionSort(rightStringArray);
    }
    else if ( l2 < 50 && l2 >= 10 && ratioRight <= 0.3 ) {
        printf("Switching to Shell Sort for right SubArray with l1 = %d with disorder ratio %f\n", l2, ratioRight);
        shellSort(rightStringArray);
    }
    else {
        printf("Proceeded by Merging with l1 =  %d and l2 = %d and disorder ratio = %f %f\n", l1,l2, ratioLeft, ratioRight);
        isMergeExecuted = 1;
        int i = 0, j = 0, k = left;
        while ( i < l1 && j < l2 ) {
            if ( leftArray[i].l <= rightArray[j].l ) array->strings[k++] = leftArray[i++];
            else array->strings[k++] = rightArray[j++];
        }
        while ( i < l1 ) array->strings[k++] = leftArray[i++];
        while ( j < l2 ) array->strings[k++] = rightArray[j++];
        free(leftArray);free(rightArray);

    }
    if ( !isMergeExecuted ) {
        for (int i = 0; i < l1; i++) {
            array->strings[k++] = leftArray[i];
        }
        for (int j = 0; j < l2; j++) {
            array->strings[k++] = rightArray[j];
        }
    }

}

void MergeSort ( StringArray * array, int left, int right ) {
    if ( left < right ) {
        int mid = ( left + right )/2;
        MergeSort(array, left, mid );
        MergeSort(array, mid + 1, right);
        merge(array, left, mid, right);

    }
} 
void shellSort ( StringArray * array ) {
    for ( int interval = (array->l)/2; interval > 0 ; interval/=2 ) {
        for ( int i = interval; i < array->l; i++ ) {
            int j; String temp = array->strings[i];
            for (j = i; ( j - interval ) >= 0 && array->strings[j-interval].l > temp.l; j-=interval ) {
                array->strings[ j ] = array->strings[j - interval]; 
            }
            array->strings[j] = temp;
        } 
        printStringArray(array);
        if ( getDisorderRatio(array) <= 0.1 && interval <= 2 && array->l <= 10 ) {
            printf("Switching to Insertion Sort\n");
            insertionSort(array);
            return;
        }
    }
}

void insertionSort( StringArray * array ) {
    for ( int i = 1; i < array->l; i++ ) {
        int j; String temp = array->strings[i];
        for ( j = i; ( j - 1 ) >= 0 && array->strings[ j - 1 ].l > temp.l; j-=1 ) {
            array->strings[j] = array->strings[j-1];
        } 
        array->strings[j] = temp;
        printStringArray(array);
    }
}
void printStringArray ( StringArray * L ) {
    for ( int i = 0; i < L->l; i++ ) {
        printf("%s ", L->strings[i].s);
    }
    printf("\n");
}
int getInt ( char * s ) {
    printf("%s \n", s);
    int x;
    scanf("%d", &x);
    return x;
}
int len ( char * s ) {
    int c = 0, i = 0;
    while ( s[i] != '\0' ) {
        c++; i++;
    }
    return c;
}
void printStrings ( String * strings, int l ) {
    for ( int i = 0 ; i < l; i++ ) {
        printf("%s ", strings[i].s);
    }
    printf("\n");
}
StringArray * getStringArray () {
    int length = getInt("Enter the length");
    StringArray * L = ( StringArray * )malloc( sizeof(StringArray) );
    L->l = length;
    L->strings = ( String * )malloc( sizeof(String) * length );
    for ( int i = 0; i < length; i++ ) {
        printf("Enter the String\n");
        scanf("%s", L->strings[i].s);
        L->strings[i].l = len(L->strings[i].s);
    }
    return L;
    
}

int countInversions ( StringArray * array ) {
    int  count = 0;
    for ( int i = 0 ; i < array->l - 1; i++ ){
        for ( int j = i + 1; j < array->l; j++ ) {
            if ( array->strings[i].l > array->strings[j].l ) count++;
        }
    }
    return count;
}

float getDisorderRatio ( StringArray * array ) {
    if ( array->l == 1 ) return 0;
    int totalNumberOfPairs = (array->l * ( array->l - 1 ))/2;
    return (float)countInversions(array)/totalNumberOfPairs;
}
void sortStrinArrayMergeSort(StringArray *array) {
    MergeSort(array, 0, array->l - 1);
}
void HybridSort(StringArray *array) {
    float disorderRatio = getDisorderRatio(array);
    
    // Choose MergeSort for large arrays with high disorder
    if (array->l > 50) {
        printf("Choosing MergeSort\n");
        sortStrinArrayMergeSort(array);
    } 
    // Choose Shell Sort for medium-sized arrays with moderate disorder
    else if (array->l > 10 ) {
        printf("Choosing Shell Sort\n");
        shellSort(array);
    } 
    // Choose Insertion Sort for small arrays with low disorder
    else if (array->l <= 10) {
        printf("Choosing Insertion Sort\n");
        insertionSort(array);
    }
}




StringArray * readFromFile ( FILE * fp, int l ) {
    StringArray * array = ( StringArray * )malloc( sizeof(StringArray) );
    array->l = l;
    array->strings = ( String * )malloc( sizeof(String) * l );
    int c = 0;
    while ( fscanf(fp, "%99s", array->strings[c].s) == 1 && c < l ) {
        array->strings[c].l = len(array->strings[c].s);
        c++;
    }
    array->l = c;
    return array;
}
StringArray * CopyStringArrays ( StringArray * array1) {
    StringArray * array2 = ( StringArray * )malloc( sizeof(StringArray) );
    array2->l = array1->l;
    array2->strings = ( String * )malloc( sizeof(String) * array1->l );
    for ( int i = 0 ; i < array1->l; i++ ){
        strcpy(array2->strings[i].s, array1->strings[i].s);
        array2->strings[i].l = array1->strings[i].l;
    }
    return array2; 
}
// int main () {
//     StringArray * array = getStringArray();
//     printf("No of inversions = %d\n", countInversions(array));
//     printf("Disorder Ratio = %f\n", getDisorderRatio(array));
// }

int main () {
    FILE * fp = fopen("words.txt", "r");
    int l = getInt("Enter number of words to read from file");
    StringArray * array = readFromFile( fp, l );
    if ( array == NULL || array->strings == NULL ) {
        perror("Failed to read words from the file\n");
        return EXIT_FAILURE;
    }
    
    HybridSort(array);
    printf("\n\n\n Final Sorted Array\n");
    printStringArray(array);
    getch();
    fclose(fp);
    free(array);
    
}
