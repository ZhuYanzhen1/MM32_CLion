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
