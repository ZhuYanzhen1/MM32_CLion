//
// Created by Lao·Zhu on 2021/12/25.
//

#include "sort.h"

static int merge_memory1[512], merge_memory2[512];

int heaplify(int *a, int len, int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2, tmp;
    if (left > len - 1)
        return 0;
    else if (left == len - 1) {
        if (a[index] < a[left]) {
            tmp = a[index];
            a[index] = a[left];
            a[left] = tmp;
        }
        return 0;
    } else {
        if (a[index] < a[left] || a[index] < a[right]) {
            if (a[left] < a[right]) {
                tmp = a[index];
                a[index] = a[right];
                a[right] = tmp;
                heaplify(a, len, right);
            } else {
                tmp = a[index];
                a[index] = a[left];
                a[left] = tmp;
                heaplify(a, len, left);
            }
        }
    }
    return 0;
}

int build_heap(int *a, int len) {
    for (int i = len - 1; i >= 0; i--) {
        if (2 * i + 1 > len - 1)
            continue;
        heaplify(a, len, i);
    }
    return 0;
}

void sort_heapsort(int *a, int len) {
    build_heap(a, len);
    int tmp, newlen = len;
    while (newlen > 1) {
        //swap head with last
        tmp = a[0];
        a[0] = a[newlen - 1];
        a[newlen - 1] = tmp;
        newlen--;
        //heapify new heap
        heaplify(a, newlen, 0);
    }
}

void u_memcpy(void *destin, void *source, unsigned int n) {
    unsigned char *pointer1 = (unsigned char *) destin;
    unsigned char *pointer2 = (unsigned char *) source;
    for (int counter = 0; counter < n; ++counter)
        pointer1[counter] = pointer2[counter];
}

int merge_two(int *a, int s1, int e1, int s2, int e2, int *m1, int *m2) {
    int len1 = e1 - s1 + 1;
    int len2 = e2 - s2 + 1;
    int p1 = 0, p2 = 0, p = s1;
    u_memcpy(m1, a + s1, sizeof(int) * len1);
    u_memcpy(m2, a + s2, sizeof(int) * len2);
    while (p1 < len1 && p2 < len2) {
        if (m1[p1] < m2[p2])
            a[p++] = m1[p1++];
        else
            a[p++] = m2[p2++];
    }
    while (p1 < len1)
        a[p++] = m1[p1++];
    while (p2 < len2)
        a[p++] = m2[p2++];
    return 0;
}

void sort_mergesort(int *a, int len) {
    int step = 1;
    int *m1 = merge_memory1;
    int *m2 = merge_memory2;
    for (unsigned int counter = 0; counter < sizeof(merge_memory1) / 4; counter++) {
        merge_memory1[counter] = 0x00000000L;
        merge_memory2[counter] = 0x00000000L;
    }
    while (step < len) {
        int i, s1, e1, s2, e2;
        for (i = 0; (i + step - 1) < (len - 1); i += 2 * step) {
            s1 = i;
            e1 = i + step - 1;
            s2 = e1 + 1;
            (i + 2 * step - 1) < (len - 1) ? (e2 = i + 2 * step - 1) : (e2 = len - 1);
            merge_two(a, s1, e1, s2, e2, m1, m2);
        }
        step = step << 1;
    }
}

int quicksort_r(int *a, int start, int end) {
    if (start >= end)
        return 0;
    int pivot = a[end], swp;
    int pointer = start;
    for (int i = start; i < end; i++) {
        if (a[i] < pivot) {
            if (pointer != i) {
                swp = a[i];
                a[i] = a[pointer];
                a[pointer] = swp;
            }
            pointer++;
        }
    }
    //swap back pivot to proper position
    swp = a[end];
    a[end] = a[pointer];
    a[pointer] = swp;
    quicksort_r(a, start, pointer - 1);
    quicksort_r(a, pointer + 1, end);
    return 0;
}

void sort_quicksort(int *a, int len) {
    quicksort_r(a, 0, len - 1);
}
