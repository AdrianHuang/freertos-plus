#ifndef __ROMFS_H__
#define __ROMFS_H__

#include <stdint.h>

#define ROMFS_MOUNT_POINT "romfs"

/* The column size of the console. */
#define CONSOLE_COL_SIZE 	80	
#define CONSOLE_FILES_SPACE	2

#define CONSOLE_EXCEED_COL_SIZE(len) \
	(len + CONSOLE_FILES_SPACE > CONSOLE_COL_SIZE)

void register_romfs(const char * mountpoint, const uint8_t * romfs);
const uint8_t * romfs_get_file_by_hash(const uint8_t * romfs, uint32_t h, uint32_t * len);

#endif

