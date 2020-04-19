
/*
    Support for process policy settings embedded into executable image,
    copy pasted from wdk.h header file.
*/

#define IMAGE_POLICY_METADATA_VERSION 1
#define IMAGE_POLICY_SECTION_NAME ".tPolicy"
#define IMAGE_POLICY_METADATA_NAME __ImagePolicyMetadata

typedef enum _IMAGE_POLICY_ENTRY_TYPE 
{
    ImagePolicyEntryTypeNone = 0,
    ImagePolicyEntryTypeBool,
    ImagePolicyEntryTypeInt8,
    ImagePolicyEntryTypeUInt8,
    ImagePolicyEntryTypeInt16,
    ImagePolicyEntryTypeUInt16,
    ImagePolicyEntryTypeInt32,
    ImagePolicyEntryTypeUInt32,
    ImagePolicyEntryTypeInt64,
    ImagePolicyEntryTypeUInt64,
    ImagePolicyEntryTypeAnsiString,
    ImagePolicyEntryTypeUnicodeString,
    ImagePolicyEntryTypeMaximum

} IMAGE_POLICY_ENTRY_TYPE;

typedef enum _IMAGE_POLICY_ID 
{
    ImagePolicyIdNone = 0,
    ImagePolicyIdEtw,
    ImagePolicyIdDebug,
    ImagePolicyIdCrashDump,
    ImagePolicyIdCrashDumpKey,
    ImagePolicyIdCrashDumpKeyGuid,
    ImagePolicyIdParentSd,
    ImagePolicyIdParentSdRev,
    ImagePolicyIdSvn,
    ImagePolicyIdDeviceId,
    ImagePolicyIdCapability,
    ImagePolicyIdScenarioId,
    ImagePolicyIdMaximum

} IMAGE_POLICY_ID;

typedef struct _IMAGE_POLICY_ENTRY 
{
    IMAGE_POLICY_ENTRY_TYPE Type;
    IMAGE_POLICY_ID PolicyId;
    union 
    {
        const VOID* None;
        BOOLEAN BoolValue;
        INT8 Int8Value;
        UINT8 UInt8Value;
        INT16 Int16Value;
        UINT16 UInt16Value;
        INT32 Int32Value;
        UINT32 UInt32Value;
        INT64 Int64Value;
        UINT64 UInt64Value;
        PCSTR AnsiStringValue;
        PCWSTR UnicodeStringValue;
    } u;

} IMAGE_POLICY_ENTRY,
*PIMAGE_POLICY_ENTRY;

#pragma warning(push)
#pragma warning(disable:4200) // zero-sized array in struct/union

typedef struct _IMAGE_POLICY_METADATA 
{
    UCHAR Version;
    UCHAR Reserved0[7];
    ULONGLONG ApplicationId;
    IMAGE_POLICY_ENTRY Policies[];

} IMAGE_POLICY_METADATA,
*PIMAGE_POLICY_METADATA;

#pragma warning(pop)

#define IMAGE_POLICY_START(_ApplicationId_)                                   \
__pragma(const_seg(push, IMAGE_POLICY_SECTION_NAME));                         \
                                                                              \
EXTERN_C __declspec(dllexport) const                                          \
IMAGE_POLICY_METADATA IMAGE_POLICY_METADATA_NAME =                            \
{                                                                             \
    IMAGE_POLICY_METADATA_VERSION,                                            \
    { 0 },                                                                    \
    _ApplicationId_,                                                          \
    {

#define IMAGE_POLICY_END()                                                    \
        { ImagePolicyEntryTypeNone, ImagePolicyIdNone, NULL }                 \
    }                                                                         \
};                                                                            \
__pragma(const_seg(pop))

#define IMAGE_POLICY_BOOL(_PolicyId_, _Value_) \
    { ImagePolicyEntryTypeBool, _PolicyId_, (const VOID *)_Value_ },

#define IMAGE_POLICY_INT8(_PolicyId_, _Value_) \
    { ImagePolicyEntryTypeInt8, _PolicyId_, (const VOID *)_Value_ },

#define IMAGE_POLICY_UINT8(_PolicyId_, _Value_) \
    { ImagePolicyEntryTypeUInt8, _PolicyId_, (const VOID *)_Value_ },

#define IMAGE_POLICY_INT16(_PolicyId_, _Value_) \
    { ImagePolicyEntryTypeInt16, _PolicyId_, (const VOID *)_Value_ },

#define IMAGE_POLICY_UINT16(_PolicyId_, _Value_) \
    { ImagePolicyEntryTypeUInt16, _PolicyId_, (const VOID *)_Value_ },

#define IMAGE_POLICY_INT32(_PolicyId_, _Value_) \
    { ImagePolicyEntryTypeInt32, _PolicyId_, (const VOID *)_Value_ },

#define IMAGE_POLICY_UINT32(_PolicyId_, _Value_) \
    { ImagePolicyEntryTypeUInt32, _PolicyId_, (const VOID *)_Value_ },

#define IMAGE_POLICY_INT64(_PolicyId_, _Value_) \
    { ImagePolicyEntryTypeInt64, _PolicyId_, (const VOID *)_Value_ },

#define IMAGE_POLICY_UINT64(_PolicyId_, _Value_) \
    { ImagePolicyEntryTypeUInt64, _PolicyId_, (const VOID *)_Value_ },

#define IMAGE_POLICY_ANSI_STRING(_PolicyId_, _Value_) \
    { ImagePolicyEntryTypeAnsiString, _PolicyId_, _Value_ },

#define IMAGE_POLICY_UNICODE_STRING(_PolicyId_, _Value_) \
    { ImagePolicyEntryTypeUnicodeString, _PolicyId_, _Value_ },
