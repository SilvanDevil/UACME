/*******************************************************************************
*
*  (C) COPYRIGHT AUTHORS, 2014 - 2020
*
*  TITLE:       SUP.H
*
*  VERSION:     3.27
*
*  DATE:        14 Sep 2020
*
*  Common header file for the program support routines.
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
* ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
* TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
* PARTICULAR PURPOSE.
*
*******************************************************************************/
#pragma once

typedef BOOL(WINAPI* pfnShellExecuteExW)(
    SHELLEXECUTEINFOW* pExecInfo);

typedef DWORD(WINAPI* pfnWaitForSingleObject)(
    HANDLE hHandle,
    DWORD dwMilliseconds);

typedef BOOL(WINAPI* pfnCloseHandle)(
    HANDLE hObject);

typedef HRESULT(WINAPI* pfnCoInitialize)(
    LPVOID pvReserved);

typedef HRESULT(WINAPI* pfnCoGetObject)(
    LPCWSTR pszName,
    BIND_OPTS* pBindOptions,
    REFIID riid,
    void** ppv);

typedef HRESULT(WINAPI* pfnSHCreateItemFromParsingName)(
    PCWSTR pszPath,
    IBindCtx* pbc,
    REFIID riid,
    void** ppv);

typedef void(WINAPI* pfnCoUninitialize)(
    VOID);

typedef NTSTATUS(NTAPI* pfnRtlExitUserThread)(
    _In_ NTSTATUS ExitStatus);

typedef struct tagLOAD_PARAMETERS {
    WCHAR                   szVerb[10];
    WCHAR                   szTargetApp[MAX_PATH + 1];
    pfnShellExecuteExW      ShellExecuteExW;
    pfnWaitForSingleObject  WaitForSingleObject;
    pfnCloseHandle          CloseHandle;
    pfnRtlExitUserThread    RtlExitUserThread;
} LOAD_PARAMETERS, * PLOAD_PARAMETERS;

typedef BOOL(CALLBACK *UCM_FIND_FILE_CALLBACK)(
    WIN32_FIND_DATA *fdata,
    LPWSTR lpDirectory);

typedef struct _SXS_SEARCH_CONTEXT {
    LPWSTR DllName;
    LPWSTR SxsKey;
    LPWSTR FullDllPath;
} SXS_SEARCH_CONTEXT, *PSXS_SEARCH_CONTEXT;

//ntifs.h
typedef struct _REPARSE_DATA_BUFFER {
    ULONG  ReparseTag;
    USHORT ReparseDataLength;
    USHORT Reserved;
    union {
        struct {
            USHORT SubstituteNameOffset;
            USHORT SubstituteNameLength;
            USHORT PrintNameOffset;
            USHORT PrintNameLength;
            ULONG Flags;
            WCHAR PathBuffer[1];
        } SymbolicLinkReparseBuffer;
        struct {
            USHORT SubstituteNameOffset;
            USHORT SubstituteNameLength;
            USHORT PrintNameOffset;
            USHORT PrintNameLength;
            WCHAR PathBuffer[1];
        } MountPointReparseBuffer;
        struct {
            UCHAR  DataBuffer[1];
        } GenericReparseBuffer;
    } DUMMYUNIONNAME;
} REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;

#define REPARSE_DATA_BUFFER_HEADER_LENGTH FIELD_OFFSET(REPARSE_DATA_BUFFER, GenericReparseBuffer.DataBuffer)

//
// Fusion
//

typedef struct _CLIMETAHDR {
    ULONG Signature;
    USHORT MajorVersion;
    USHORT MinorVersion;
    ULONG Reserved;
    ULONG VersionLength;
    CHAR Version[ANYSIZE_ARRAY];
} CLIMETAHDR, * PCLIMETAHDR;

typedef struct _CLISTREAMROOT {
    BYTE Flags;
    BYTE Align0;
    WORD Streams;
} CLISTREAMROOT, * PCLISTREAMROOT;

typedef struct _CLIMETASTREAM {
    DWORD Offset;
    DWORD Size;
    CHAR Name[ANYSIZE_ARRAY];
} CLIMETASTREAM, * PCLIMETASTREAM;

typedef HRESULT(WINAPI* pfnCreateAssemblyEnum)(
    _Out_ IAssemblyEnum** pEnum,
    _In_opt_  IUnknown* pUnkReserved,
    _In_opt_  IAssemblyName* pName,
    _In_  DWORD dwFlags,
    _Reserved_  LPVOID pvReserved);

typedef HRESULT(WINAPI* pfnCreateAssemblyCache)(
    _Out_ IAssemblyCache** ppAsmCache,
    _In_  DWORD            dwReserved);

typedef struct _FUSION_SCAN_PARAM {
    _In_ GUID *ReferenceMVID;
    _Out_ LPWSTR lpFileName;
} FUSION_SCAN_PARAM, * PFUSION_SCAN_PARAM;

typedef BOOL(CALLBACK* pfnFusionScanFilesCallback)(
    LPWSTR CurrentDirectory,
    WIN32_FIND_DATA* FindData,
    PVOID UserContext);

#define DEFAULT_ALLOCATION_TYPE MEM_COMMIT | MEM_RESERVE
#define DEFAULT_PROTECT_TYPE PAGE_READWRITE

VOID supSetLastErrorFromNtStatus(
    _In_ NTSTATUS LastNtStatus);

BOOL supQueryProcessTokenIL(
    _In_ HANDLE hProcess,
    _Out_ PULONG IntegrityLevel);

HANDLE supGetProcessWithILAsCaller(
    _In_ ACCESS_MASK UseDesiredAccess);

BOOLEAN supIsProcess32bit(
    _In_ HANDLE hProcess);

BOOL supGetElevationType(
    _Out_ TOKEN_ELEVATION_TYPE *lpType);

BOOL supWriteBufferToFile(
    _In_ LPWSTR lpFileName,
    _In_ PVOID Buffer,
    _In_ DWORD BufferSize);

BOOL supDecodeAndWriteBufferToFile(
    _In_ LPWSTR lpFileName,
    _In_ CONST PVOID Buffer,
    _In_ DWORD BufferSize,
    _In_ ULONG Key);

PBYTE supReadFileToBuffer(
    _In_ LPWSTR lpFileName,
    _Inout_opt_ LPDWORD lpBufferSize);

PBYTE supReadFileToBuffer_Cur(
    _In_ LPWSTR lpFileName,
    _Inout_opt_ LPDWORD lpBufferSize);

BOOL supRunProcess2(
    _In_ LPWSTR lpszProcessName,
    _In_opt_ LPWSTR lpszParameters,
    _In_opt_ LPWSTR lpVerb,
    _In_ INT nShow,
    _In_ BOOL fWait);

BOOL supRunProcess(
    _In_ LPWSTR lpszProcessName,
    _In_opt_ LPWSTR lpszParameters);

_Success_(return != NULL)
HANDLE NTAPI supRunProcessEx(
    _In_ LPWSTR lpszParameters,
    _In_opt_ LPWSTR lpCurrentDirectory,
    _In_opt_ LPWSTR lpApplicationName,
    _Out_opt_ HANDLE *PrimaryThread);

_Success_(return != NULL)
HANDLE NTAPI supRunProcessIndirect(
    _In_ LPWSTR lpszParameters,
    _In_opt_ LPWSTR lpCurrentDirectory,
    _Inout_opt_ LPWSTR lpApplicationName,
    _In_ ULONG CreationFlags,
    _In_ WORD ShowWindowFlags,
    _Out_opt_ HANDLE *PrimaryThread);

void supCopyMemory(
    _Inout_ void *dest,
    _In_ size_t cbdest,
    _In_ const void *src,
    _In_ size_t cbsrc);

DWORD supQueryEntryPointRVA(
    _In_ LPWSTR lpImageFile);

LPWSTR supQueryEnvironmentVariableOffset(
    _In_ PUNICODE_STRING Value);

DWORD supCalculateCheckSumForMappedFile(
    _In_ PVOID BaseAddress,
    _In_ ULONG FileLength);

BOOLEAN supVerifyMappedImageMatchesChecksum(
    _In_ PVOID BaseAddress,
    _In_ ULONG FileLength);

BOOLEAN supSetCheckSumForMappedFile(
    _In_ PVOID BaseAddress,
    _In_ ULONG CheckSum);

VOID ucmShowMessageById(
    _In_ BOOL OutputToDebugger,
    _In_ ULONG MessageId);

VOID ucmShowMessage(
    _In_ BOOL OutputToDebugger,
    _In_ LPWSTR lpszMsg);

INT ucmShowQuestionById(
    _In_ ULONG MessageId);

INT ucmShowQuestion(
    _In_ LPWSTR lpszMsg);

PBYTE supLdrQueryResourceData(
    _In_ ULONG_PTR ResourceId,
    _In_ PVOID DllHandle,
    _In_ PULONG DataSize);

VOID supMasqueradeProcess(
    _In_ BOOL Restore);

DWORD supExpandEnvironmentStrings(
    _In_ LPCWSTR lpSrc,
    _In_ LPWSTR lpDst,
    _In_ DWORD nSize);

BOOL sxsFindLoaderEntry(
    _In_ PSXS_SEARCH_CONTEXT Context);

VOID supDebugPrint(
    _In_ LPWSTR ApiName,
    _In_ DWORD status);

PVOID supVirtualAlloc(
    _Inout_ PSIZE_T Size,
    _In_ ULONG AllocationType,
    _In_ ULONG Protect,
    _Out_opt_ NTSTATUS *Status);

BOOL supVirtualFree(
    _In_ PVOID Memory,
    _Out_opt_ NTSTATUS *Status);

BOOL supSecureVirtualFree(
    _In_ PVOID Memory,
    _In_ SIZE_T MemorySize,
    _Out_opt_ NTSTATUS *Status);

PVOID FORCEINLINE supHeapAlloc(
    _In_ SIZE_T Size);

BOOL FORCEINLINE supHeapFree(
    _In_ PVOID Memory);

BOOL supRegDeleteKeyRecursive(
    _In_ HKEY hKeyRoot,
    _In_ LPWSTR lpSubKey);

BOOL supSetEnvVariable(
    _In_ BOOL fRemove,
    _In_opt_ LPWSTR lpKeyName,
    _In_ LPWSTR lpVariableName,
    _In_opt_ LPWSTR lpVariableData);

BOOL supSetMountPoint(
    _In_ HANDLE hDirectory,
    _In_ LPWSTR lpTarget,
    _In_ LPWSTR lpPrintName);

BOOL supDeleteMountPoint(
    _In_ HANDLE hDirectory);

HANDLE supOpenDirectoryForReparse(
    _In_ LPWSTR lpDirectory);

BOOL supWinstationToName(
    _In_opt_ HWINSTA hWinsta,
    _In_ LPWSTR lpBuffer,
    _In_ DWORD cbBuffer,
    _Out_ PDWORD BytesNeeded);

BOOL supDesktopToName(
    _In_opt_ HDESK hDesktop,
    _In_ LPWSTR lpBuffer,
    _In_ DWORD cbBuffer,
    _Out_ PDWORD BytesNeeded);

BOOL supReplaceDllEntryPoint(
    _In_ PVOID DllImage,
    _In_ ULONG SizeOfDllImage,
    _In_ LPCSTR lpEntryPointName,
    _In_ BOOL fConvertToExe);

NTSTATUS supRegReadValue(
    _In_ HANDLE hKey,
    _In_ LPWSTR ValueName,
    _In_ DWORD ValueType,
    _Out_ PVOID *Buffer,
    _Out_ ULONG *BufferSize,
    _In_opt_ HANDLE hHeap);

BOOL supQuerySystemRoot(
    _Inout_ PVOID Context);

PVOID supGetSystemInfo(
    _In_ SYSTEM_INFORMATION_CLASS InfoClass);

BOOL supIsCorImageFile(
    PVOID ImageBase);

NTSTATUS supRegSetValueIndirectHKCU(
    _In_ LPWSTR TargetKey,
    _In_opt_ LPWSTR ValueName,
    _In_ LPWSTR lpData,
    _In_ ULONG cbData);

NTSTATUS supRemoveRegLinkHKCU(
    VOID);

BOOL supIsConsentApprovedInterface(
    _In_ LPWSTR InterfaceName,
    _Out_ PBOOL IsApproved);

BOOL supDeleteKeyValueAndFlushKey(
    _In_ HKEY hRootKey,
    _In_ LPWSTR lpKeyName,
    _In_ LPWSTR lpValueName);

PVOID supEncodePointer(
    _In_ PVOID Pointer);

PVOID supDecodePointer(
    _In_ PVOID Pointer);

NTSTATUS supCreateDirectory(
    _Out_opt_ PHANDLE phDirectory,
    _In_ OBJECT_ATTRIBUTES *ObjectAttributes,
    _In_ ULONG DirectoryShareFlags,
    _In_ ULONG DirectoryAttributes);

BOOL supCreateSharedParametersBlock(
    _In_ PVOID ucmContext);

VOID supDestroySharedParametersBlock(
    _In_ PVOID ucmContext);

PVOID supCreateUacmeContext(
    _In_ ULONG Method,
    _In_reads_or_z_opt_(OptionalParameterLength) LPWSTR OptionalParameter,
    _In_ ULONG OptionalParameterLength,
    _In_ PVOID DecompressRoutine,
    _In_ BOOL OutputToDebugger);

VOID supDestroyUacmeContext(
    _In_ PVOID Context);

NTSTATUS supEnableDisableWow64Redirection(
    _In_ BOOL bDisable);

BOOLEAN supIndirectRegAdd(
    _In_ WCHAR* pszRootKey,
    _In_ WCHAR* pszKey,
    _In_opt_ WCHAR* pszValue,
    _In_opt_ WCHAR* pszDataType,
    _In_ WCHAR* pszData);

BOOLEAN supIsNetfx48PlusInstalled(
    VOID);

NTSTATUS supGetProcessDebugObject(
    _In_ HANDLE ProcessHandle,
    _Out_ PHANDLE DebugObjectHandle);

BOOLEAN supInitFusion(
    _In_ DWORD dwVersion);

HRESULT supFusionGetAssemblyName(
    _In_ IAssemblyName* pInterface,
    _Inout_ LPWSTR* lpAssemblyName);

HRESULT supFusionGetAssemblyPath(
    _In_ IAssemblyCache* pInterface,
    _In_ LPWSTR lpAssemblyName,
    _Inout_ LPWSTR* lpAssemblyPath);

BOOLEAN supFusionGetAssemblyPathByName(
    _In_ LPWSTR lpAssemblyName,
    _Inout_ LPWSTR* lpAssemblyPath);

BOOL supIsProcessRunning(
    _In_ LPWSTR ProcessName);

BOOL supFusionGetImageMVID(
    _In_ LPWSTR lpImageName,
    _Out_ GUID* ModuleVersionId);

BOOL supFusionScanDirectory(
    _In_ LPWSTR lpDirectory,
    _In_ LPWSTR lpExtension,
    _In_ pfnFusionScanFilesCallback pfnCallback,
    _In_opt_ PVOID pvUserContext);

BOOL supFusionFindFileByMVIDCallback(
    LPWSTR CurrentDirectory,
    WIN32_FIND_DATA* FindData,
    PVOID UserContext);

#ifdef _DEBUG
#define supDbgMsg(Message)  OutputDebugString(Message)
#else
#define supDbgMsg(Message)  
#endif

#define PathFileExists(lpszPath) (GetFileAttributes(lpszPath) != (DWORD)-1)
