#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#define BUF_SIZE 80
#define NAME_SIZE 32

struct student {
    double score; 
    char name[NAME_SIZE];
};

struct student *table;
int counter_compare, counter_swap;

/* カウンタを0にリセット */
void Clear() {
    counter_compare = 0;
    counter_swap = 0;
}

/* 現在のカウンタ値を表示 */
void Counter() {
    printf("Compare %d Swap %d\n", counter_compare, counter_swap);
}

/* 比較関数：呼び出されるたびに比較回数をインクリメント */
int my_compare(const struct student *a, const struct student *b) {
    counter_compare++;
    return (a->score > b->score) ? 1 : (a->score < b->score) ? -1 : 0;
}

/* 入れ替え関数：呼び出されるたびに交換回数をインクリメント */
void my_swap(struct student *a, struct student *b) {
    struct student c;
    counter_swap++;
    memcpy(&c, a, sizeof(struct student));
    memcpy(a, b, sizeof(struct student));
    memcpy(b, &c, sizeof(struct student));
}

// --- ソートロジックは変更なし ---

void Bubble(int size) {
    struct timeval stime, etime;
    double sec;
    gettimeofday(&stime, NULL);
    int i, j;
    for (i = 0; i < size - 1; i++)
        for (j = size - 1; j > i; j--)
            if (my_compare(&table[j - 1], &table[j]) == 1)
                my_swap(&table[j - 1], &table[j]);
    gettimeofday(&etime, NULL);
    sec = (etime.tv_sec - stime.tv_sec) + 
            (etime.tv_usec - stime.tv_usec) / 1000000.0;
    printf("Elapsed Time %.6f [sec] \n", sec);
}

void Select(int size) {
    struct timeval stime, etime;
    double sec;
    gettimeofday(&stime, NULL);
    int i, j;
    for (i = 0; i < size - 1; i++) {
        int min = i;
        for (j = i + 1; j < size; j++)
            if (my_compare(&table[min], &table[j]) == 1)
                min = j;
        my_swap(&table[i], &table[min]);
    }
    gettimeofday(&etime, NULL);
    sec = (etime.tv_sec - stime.tv_sec) + 
            (etime.tv_usec - stime.tv_usec) / 1000000.0;
    printf("Elapsed Time %.6f [sec] \n", sec);
}

void Insert(int size) {
    struct timeval stime, etime;
    double sec;
    gettimeofday(&stime, NULL);
    int i, j;
    for (i = 1; i < size; i++) {
        j = i;
        while (j >= 1 && my_compare(&table[j - 1], &table[j]) == 1) {
            my_swap(&table[j], &table[j - 1]);
            j--;
        }
    }
    gettimeofday(&etime, NULL);
    sec = (etime.tv_sec - stime.tv_sec) + 
            (etime.tv_usec - stime.tv_usec) / 1000000.0;
    printf("Elapsed Time %.6f [sec] \n", sec);
}

void Shell(int size) {
    struct timeval stime, etime;
    double sec;
    gettimeofday(&stime, NULL);
    int i, j, gap;
    gap = size / 2;
    while (gap > 0) {
        for (i = gap; i < size; i++) {
            j = i;
            while (j >= gap && my_compare(&table[j - gap], &table[j]) == 1) {
                my_swap(&table[j], &table[j - gap]);
                j -= gap;
            }
        }
        gap = gap / 2;
    }
    gettimeofday(&etime, NULL);
    sec = (etime.tv_sec - stime.tv_sec) + 
            (etime.tv_usec - stime.tv_usec) / 1000000.0;
    printf("Elapsed Time %.6f [sec] \n", sec);
}

void Disp(int size) {
    int i;
    for (i = 0; i < size; i++)
        printf("%.1f\t%s\n", table[i].score, table[i].name); 
}

int main(int argc, char *argv[]) {
    FILE *fp;
    int n;
    char buf[BUF_SIZE];

    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]); 
        exit(1);
    }

    if ((fp = fopen(argv[1], "r")) == NULL) {
        printf("Cannot open file (%s) \n", argv[1]); 
        exit(1);
    }

    n = 0;
    while (fgets(buf, BUF_SIZE, fp) != NULL) n++;
    
    if ((table = malloc(sizeof(struct student) * n)) == NULL) {
        printf("Cannot allocate memory \n"); 
        exit(1);
    }

    fseek(fp, 0L, SEEK_SET);
    n = 0;
    while (fgets(buf, BUF_SIZE, fp) != NULL) {
        sscanf(buf, "%lf\t%s", &table[n].score, table[n].name); 
        n++;
    }
    fclose(fp);

//  Disp(n);
    Clear();

    // メイン処理では現在シェルソート（Shell）が呼ばれています
    Shell(n);

    Disp(n);
    Counter();

    free(table);
    return 0;
}