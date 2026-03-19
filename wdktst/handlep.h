#ifndef _HANDLEP_H
#define _HANDLEP_H

#define _HANDLE_PRIVATE
#include "Ayame.h"

#define AYAME_HANDLE_INCREMENT 4

typedef struct _AYAME_HANDLE_TABLE {
	ULONG Tag;
	ULONG SizeOfEntry;
	HANDLE NextHandle;
	struct _AYAME_HANDLE_TABLE_ENTRY* FreeHandle;
	FAST_MUTEX Mutex;
	ULONG TableSize;
	PVOID Table;
} AYAME_HANDLE_TABLE, *PAYAME_HANDLE_TABLE;

#define AyameHandleFromIndex(Index) ((HANDLE)((ULONG_PTR)(Index) * AYAME_HANDLE_INCREMENT))
#define AyameEntryFromHandle(HandleTable, Handle) KphEntryFromIndex((HandleTable), KphIndexFromHandle(Handle))
#define AyameEntryFromIndex(HandleTable, Index) \
    ((PKPH_HANDLE_TABLE_ENTRY)((ULONG_PTR)(HandleTable)->Table + (Index) * (HandleTable)->SizeOfEntry))

FORCEINLINE BOOLEAN AyameLockAllocatedHandleEntry(
	__inout PAYAME_HANDLE_TABLE_ENTRY
);

#endif // !HANDLEP_H
