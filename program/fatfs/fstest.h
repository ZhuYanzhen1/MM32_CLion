//
// Created by LaoZhu on 2022/4/23.
//

#ifndef MAIN_C_FATFS_FSTEST_H_
#define MAIN_C_FATFS_FSTEST_H_

unsigned char fs_get_free(char *drv);
unsigned char fs_scan_files(char *path);
unsigned char fs_access_file_test(void);

#endif //MAIN_C_FATFS_FSTEST_H_
