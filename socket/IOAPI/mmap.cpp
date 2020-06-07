/*************************************************************************
 *File Name: mmap.cpp
 *Author: Leihang
 *mail: 973331055@qq.com
 *Created Time: 2020年06月06日 星期六 14时05分21秒
 *describe: 这个demo使用mmap将命令行第一个参数指定的文件内容，
 通过第二个参数offset指定起始位置和第三个参数length指定长度
 的内容输出到标准输出
 ************************************************************************/
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE);  } while (0)

int main(int argc, char *argv[])
{
    void *addr;
    int fd;
    struct stat sb;
    off_t offset, pa_offset;
    size_t length;
    ssize_t s;

    //可以不指定长度，默认到文件末尾
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "%s file offset [length]\n", argv[0]);
        exit(EXIT_FAILURE);

    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        handle_error("open");

    if (fstat(fd, &sb) == -1)           /* To obtain file size */
        handle_error("fstat");

    offset = atoi(argv[2]);
    pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
    /* offset for mmap() must be page aligned */
    //offset值在使用mmap时必须是页对齐的

    //对齐后大于文件长度
    if (offset >= sb.st_size) {
        fprintf(stderr, "offset is past end of file\n");
        exit(EXIT_FAILURE);

    }

    //如果指定文件长度，但是文件长度加偏移量超过文件大小，就默认到文件末尾
    if (argc == 4) {
        length = atoi(argv[3]);
        if (offset + length > sb.st_size)
            length = sb.st_size - offset;
        /* Can't display bytes past end of file */


    } else {    /* No length arg ==> display to end of file */
        length = sb.st_size - offset;
    }

    addr = mmap(NULL, length + offset - pa_offset, PROT_READ
                ,MAP_PRIVATE, fd, pa_offset);
    if (addr == MAP_FAILED)
        handle_error("mmap");
    s = write(STDOUT_FILENO, (char*)addr + offset - pa_offset, length);
    if (s != length) {
        if (s == -1)
            handle_error("write");

        fprintf(stderr, "partial write");
        exit(EXIT_FAILURE);

    }

    exit(EXIT_SUCCESS);

}

