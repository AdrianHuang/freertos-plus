#include "osdebug.h"
#include "filesystem.h"
#include "fio.h"

#include <stdint.h>
#include <string.h>
#include <hash-djb2.h>

#define MAX_FS 16

struct fs_t {
	uint32_t hash;
	void * opaque;
	struct file_operations *fops;
};

static struct fs_t fss[MAX_FS];

__attribute__((constructor)) void fs_init()
{
	memset(fss, 0, sizeof(fss));
}

static struct fs_t *fs_get_fss(const char *str)
{
	int i;
	uint32_t hash;

	hash = hash_djb2((const uint8_t *) str, -1);

	for (i=0;i<MAX_FS && fss[i].fops;i++) {
		if (fss[i].hash == hash)
			return &fss[i];
	}

	return NULL;
}

int fs_show_files(void)
{
	struct fs_t *fs_ptr = fs_get_fss("romfs");
		
	if (!fs_ptr || !fs_ptr->fops || !fs_ptr->fops->show_files)
		return -1;

	fs_ptr->fops->show_files(fs_ptr->opaque);

	return 0;
}

int register_fs(const char * mountpoint, struct file_operations *fops,
		void * opaque)
{
	int i;
	DBGOUT("register_fs(\"%s\", %p, %p)\r\n", mountpoint, fops, opaque);

	for (i = 0; i < MAX_FS; i++) {
		if (fss[i].fops)
			continue;

		fss[i].hash = hash_djb2((const uint8_t *) mountpoint, -1);
		fss[i].fops = fops;
		fss[i].opaque = opaque;

		return 0;
        }

	return -1;
}

int fs_open(const char * path, int flags, int mode)
{
	const char * slash;
	uint32_t hash;
	int i;

	//DBGOUT("fs_open(\"%s\", %i, %i)\r\n", path, flags, mode);

	while (path[0] == '/')
		path++;

	slash = strchr(path, '/');

	if (!slash)
		return -2;

	hash = hash_djb2((const uint8_t *) path, slash - path);
	path = slash + 1;

	for (i = 0; i < MAX_FS; i++) {
		if (fss[i].hash != hash)
			continue;

		return fss[i].fops->open(fss[i].opaque, path, flags, mode);
	}

	return -2;
}
