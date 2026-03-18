#ifndef HANDLEP_H
#define HANDLEP_H

#include "Ayame.h"

typedef struct _AYAME_HANDLE_TABLE {
	ULONG Tag;
	ULONG SizeOfEntry;
	HANDLE NextHandle;
	struct _AYAME_HANDLE_TABLE_ENTRY* FreeHandle;
	FAST_MUTEX Mutex;
	ULONG TableSize;
	PVOID Table;
} AYAME_HANDLE_TABLE, *PAYAME_HANDLE_TABLE;

#endif // !HANDLEP_H
