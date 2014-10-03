#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <stdint.h>
#include <hash-djb2.h>

#define MAX_FS 16
#define OPENFAIL (-1)

struct file_operations {
	int (*open) (void *, const char *, int,	int);
	int (*show_files) (void *);
};

/* Need to be called before using any other fs functions */
__attribute__((constructor)) void fs_init();

int register_fs(const char * mountpoint, struct file_operations *fops,
		void * opaque);

int fs_open(const char * path, int flags, int mode);
int fs_show_files(void);

#endif
