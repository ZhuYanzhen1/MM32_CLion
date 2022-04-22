//
// Created by LaoZhu on 2022/4/23.
//

#include "fstest.h"
#include "ff.h"
#include "printf.h"
#include "delay.h"

unsigned char fs_get_free(char *drv) {
    FATFS *fs1;
    unsigned int fre_clust = 0, fre_sect, tot_sect, total, free;
    FRESULT result = f_getfree((const TCHAR *) drv, (DWORD *) &fre_clust, &fs1);
    if (result == 0) {
        tot_sect = (fs1->n_fatent - 2) * fs1->csize;
        fre_sect = fre_clust * fs1->csize;
        total = tot_sect >> 1;     // Trans into KB
        free = fre_sect >> 1;      // Trans into KB
        printf("Total: %dKB, Free: %dKB\r\n", total, free);
        _fflush(stdout);
        delayms(100);
    }
    return result;
}

static FILINFO fileinfo;
static DIR dir;
unsigned char fs_scan_files(char *path) {
    FRESULT res = f_opendir(&dir, (const TCHAR *) path);
    if (res == FR_OK) {
        while (1) {
            res = f_readdir(&dir, &fileinfo);
            if (res != FR_OK || fileinfo.fname[0] == 0)
                break;
            printf("%s/%s\r\n", path, fileinfo.fname);
            _fflush(stdout);
            delayms(50);
        }
    }
    return res;
}

unsigned char fs_access_file_test(void) {
    static FIL newfile, readfile;
    unsigned int bw, br;
    unsigned char test_buffer[] = "hello fatfs!";
    unsigned char buffer_read[32] = {0};
    FRESULT fr = f_open(&newfile, "0:/test_file.txt", FA_WRITE | FA_CREATE_ALWAYS);
    if (fr) while (1);
    fr = f_write(&newfile, test_buffer, 12, &bw);
    if (fr) while (1);
    f_close(&newfile);

    fr = f_open(&readfile, "0:/test_file.txt", FA_READ);
    if (fr) while (1);
    fr = f_read(&readfile, buffer_read, sizeof(buffer_read), &br);
    if (fr) while (1);
    f_close(&readfile);

    for (int counter = 0; counter < sizeof(test_buffer); ++counter) {
        if (buffer_read[counter] != test_buffer[counter])
            return 1;
    }
    return 0;
}