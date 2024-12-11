#include "FileUtils.h"
#include <stdlib.h>
#include <stdio.h>

#define LOGI(FORMAT, ...) printf(FORMAT,__VA_ARGS__)

/**
 * 获取文件大小。
 */
static long getFileSize(const char* path) {
    FILE* fp = fopen(path, "rb");
    fseek(fp, 0, SEEK_END);
    return ftell(fp);
}

/**
 * 拆分文件
 */
bool splitFile(const char* path, const char* path_pattern, const int file_num) {
    // 得到分割之后的子文件的路径列表
    char** patches = malloc(sizeof(char*) * file_num);

    int i = 0;
    for (; i < file_num; i++) {
        patches[i] = malloc(sizeof(char) * 100);
        // 元素赋值
        // 需要分割的文件：C://a.png
        // 子文件：C://a%d.png
        sprintf(patches[i], path_pattern, (i + 1));
        LOGI("split patch path%d:%s \n", i, patches[i]);
    }

    // 不断读取 path 文件，循环写入 file_num 个文件中
    //	整除
    //	    文件大小：90，分成 9 个文件，每个文件 10
    //	不整除
    //	    文件大小：110，分成 9 个文件，
    //	    前 (9-1) 个文件为 (110/(9-1))=13
    //	    最后一个文件 (110%(9-1))=6
    const int fileSize = getFileSize(path);

    FILE* fpr = fopen(path, "rb");
    // 整除
    if (fileSize % file_num == 0) {
        // 单个文件大小
        const int part = fileSize / file_num;
        i = 0;
        // 逐一写入不同的分割子文件中
        for (; i < file_num; i++) {
            FILE* fpw = fopen(patches[i], "wb");
            int j = 0;
            for (; j < part; j++) {
                //边读边写
                fputc(fgetc(fpr), fpw);
            }
            fclose(fpw);
        }
    } else {
        // 不整除
        const int part = fileSize / (file_num - 1);
        i = 0;
        // 逐一写入不同的分割子文件中
        for (; i < file_num - 1; i++) {
            FILE* fpw = fopen(patches[i], "wb");
            int j = 0;
            for (; j < part; j++) {
                //边读边写
                fputc(fgetc(fpr), fpw);
            }
            fclose(fpw);
        }
        // the last one
        FILE* fpw = fopen(patches[file_num - 1], "wb");
        i = 0;
        for (; i < fileSize % (file_num - 1); i++) {
            fputc(fgetc(fpr), fpw);
        }
        fclose(fpw);
    }

    // 关闭被分割的文件
    fclose(fpr);

    // 释放
    i = 0;
    for (; i < file_num; i++) {
        free(patches[i]);
    }
    free(patches);

    return true;
}

// 合并文件
bool mergeFile(const char* path_pattern, int file_num, const char* merge_path) {
    // 得到分割之后的子文件的路径列表
    char** patches = malloc(sizeof(char*) * file_num);

    int i = 0;
    for (; i < file_num; i++) {
        patches[i] = malloc(sizeof(char) * 100);
        // 元素赋值
        // 需要分割的文件：C://A.png
        // 子文件：C://A%d.png
        sprintf(patches[i], path_pattern, (i + 1));
        LOGI("merge patch path%d:%s \n", i, patches[i]);
    }

    FILE* fpw = fopen(merge_path, "wb");
    // 把所有的分割文件读取一遍，写入一个总的文件中
    i = 0;
    for (; i < file_num; i++) {
        // 每个子文件的大小
        const int fileSize = getFileSize(patches[i]);
        FILE* fpr = fopen(patches[i], "rb");
        int j = 0;
        for (; j < fileSize; j++) {
            fputc(fgetc(fpr), fpw);
        }
        fclose(fpr);
    }
    fclose(fpw);

    // 释放
    i = 0;
    for (; i < file_num; i++) {
        free(patches[i]);
    }
    free(patches);

    return true;
}
