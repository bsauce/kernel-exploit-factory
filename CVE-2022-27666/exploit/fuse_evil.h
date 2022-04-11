#define FUSE_USE_VERSION 34
#define _GNU_SOURCE
            
#include <sched.h>
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/syscall.h>

#define MNT_PATH "evil"

extern const char *evil_path;
extern int fuse_pipes[2];
extern char *evil_str;
extern char *evil_buffer;
extern int pause_flag;

int evil_read_pause(const char *path, char *buf, size_t size, off_t offset,
              struct fuse_file_info *fi);

int evil_read_sleep(const char *path, char *buf, size_t size, off_t offset,
              struct fuse_file_info *fi);

int evil_getattr(const char *path, struct stat *stbuf,
             struct fuse_file_info *fi);

int evil_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
             off_t offset, struct fuse_file_info *fi,
             enum fuse_readdir_flags flags);