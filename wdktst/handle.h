#ifndef _HANDLE_H
#define _HANDLE_H

#include "Ayame.h"

struct _AYAME_HANDLE_TABLE;
typedef struct _AYAME_HANDLE_TABLE* PAYAME_HANDLE_TABLE;

typedef struct _AYAME_HANDLE_TABLE_ENTRY {
	union {
		HANDLE Handle;
		ULONG_PTR Value;
		struct _AYAME_HANDLE_TABLE_ENTRY* NextFree;
	};
	PVOID Object;
} AYAME_HANDLE_TABLE_ENTRY, *PAYAME_HANDLE_TABLE_ENTRY;

NTSTATUS AyameCreateHandleTable(
	__out PAYAME_HANDLE_TABLE* HandleTable,
	__in ULONG MaximumHandles,
	__in ULONG SizeOfEntry,
	__in ULONG Tag
);

VOID AyameFreeHandleTable(
	__in PAYAME_HANDLE_TABLE HandleTable
);

NTSTATUS AyameCloseHandle(
	__in PAYAME_HANDLE_TABLE HandleTable,
	__in HANDLE Handle
);

BOOLEAN AyameValidHandle(
	__in PAYAME_HANDLE_TABLE HandleTable,
	__in HANDLE Handle,
	__out_opt PAYAME_HANDLE_TABLE_ENTRY* Entry
);

#endif