#include "handle.h"
#include "handlep.h"

NTSTATUS AyameFreeHandleEntry(
	__in PAYAME_HANDLE_TABLE HandleTable,
	__in PAYAME_HANDLE_TABLE_ENTRY Entry
);

NTSTATUS AyameCreateHandleTable(
	__out PAYAME_HANDLE_TABLE* HandleTable,
	__in ULONG MaximumHandles,
	__in ULONG SizeOfEntry,
	__in ULONG Tag
) {
	PAYAME_HANDLE_TABLE handleTable;
	if (SizeOfEntry < sizeof(AYAME_HANDLE_TABLE_ENTRY))
		return STATUS_INVALID_PARAMETER_3;
	if (SizeOfEntry % 4 != 0)
		return STATUS_INVALID_PARAMETER_3;
	handleTable = ExAllocatePoolWithTag(PagedPool, sizeof(AYAME_HANDLE_TABLE), Tag);
	if (!handleTable)
		return STATUS_INSUFFICIENT_RESOURCES;
	handleTable->Table = ExAllocatePoolWithTag(
		PagedPool, MaximumHandles * SizeOfEntry, Tag
	);
	if (!handleTable->Table) {
		ExFreePoolWithTag(handleTable, Tag);
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	handleTable->Tag = Tag;
	handleTable->SizeOfEntry = SizeOfEntry;
	handleTable->NextHandle = (HANDLE)0;
	handleTable->FreeHandle = NULL;
	ExInitializeFastMutex(&handleTable->Mutex);
	handleTable->TableSize = MaximumHandles * SizeOfEntry;
	memset(handleTable->Table, 0, handleTable->TableSize);
	*HandleTable = handleTable;
	return STATUS_SUCCESS;
}

VOID AyameFreeHandleTable(
	__in PAYAME_HANDLE_TABLE HandleTable
) {
	ULONG i, tag;
	for (i = 0; i < HandleTable->TableSize / HandleTable->SizeOfEntry; ++i)
		AyameCloseHandle(HandleTable, AyameHandleFromIndex(i));
	tag = HandleTable->Tag;
	ExFreePoolWithTag(HandleTable->Table, tag);
	ExFreePoolWithTag(HandleTable, tag);
}

BOOLEAN AyameValidHandle(
	__in PAYAME_HANDLE_TABLE HandleTable,
	__in HANDLE Handle,
	__out_opt PAYAME_HANDLE_TABLE_ENTRY* Entry
) {
	PAYAME_HANDLE_TABLE_ENTRY entry;
	BOOLEAN valid;

	entry = AyameEntryFromHandle(HandleTable, Handle);
	valid =
		((ULONG_PTR)entry >= (ULONG_PTR)HandleTable->Table) &&
		((ULONG_PTR)entry + HandleTable->SizeOfEntry <=
			(ULONG_PTR)HandleTable->Table + HandleTable->TableSize);
	if (valid)
		*Entry = entry;
	return valid;
}