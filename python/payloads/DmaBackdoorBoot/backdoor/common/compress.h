
#define P_COMPRESSION_FORMAT_LZNT1    0x0002
#define P_COMPRESSION_ENGINE_STANDARD 0x0000
#define P_COMPRESSION_ENGINE_MAXIMUM  0x0100

typedef NTSTATUS (NTAPI * func_RtlGetCompressionWorkSpaceSize)(
    USHORT CompressionFormatAndEngine,
    PULONG CompressBufferWorkSpaceSize,
    PULONG CompressFragmentWorkSpaceSize
);

typedef NTSTATUS (NTAPI * func_RtlCompressBuffer)(
    USHORT CompressionFormatAndEngine,
    PUCHAR UncompressedBuffer,
    ULONG  UncompressedBufferSize,
    PUCHAR CompressedBuffer,
    ULONG  CompressedBufferSize,
    ULONG  UncompressedChunkSize,
    PULONG FinalCompressedSize,
    PVOID  WorkSpace
);

typedef NTSTATUS (NTAPI * func_RtlDecompressBuffer)(
    USHORT CompressionFormat,
    PUCHAR UncompressedBuffer,
    ULONG  UncompressedBufferSize,
    PUCHAR CompressedBuffer,
    ULONG  CompressedBufferSize,
    PULONG FinalUncompressedSize
);
