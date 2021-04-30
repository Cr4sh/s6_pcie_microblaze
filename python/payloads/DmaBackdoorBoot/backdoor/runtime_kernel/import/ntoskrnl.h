#define I_MODULE_NT 0 
 
#define H_AlpcGetHeaderSize 0x53848aad
#define H_AlpcGetMessageAttribute 0x371b07e6
#define H_AlpcInitializeMessageAttribute 0x7445f611
#define H_BgkDisplayCharacter 0xf34aa4af
#define H_BgkGetConsoleState 0x436865a0
#define H_BgkGetCursorState 0xf2639aa4
#define H_BgkSetCursor 0x8d9f7c18
#define H_CcAddDirtyPagesToExternalCache 0x33c84993
#define H_CcCanIWrite 0xbd99d0d9
#define H_CcCoherencyFlushAndPurgeCache 0xd5755927
#define H_CcCopyRead 0x79decf42
#define H_CcCopyReadEx 0xb3d0bc8f
#define H_CcCopyWrite 0xbd85a959
#define H_CcCopyWriteEx 0x6a564d99
#define H_CcCopyWriteWontFlush 0xe48016e0
#define H_CcDeductDirtyPagesFromExternalCache 0x674d3a66
#define H_CcDeferWrite 0x1b9f9512
#define H_CcFastCopyRead 0x70e61718
#define H_CcFastCopyWrite 0x21e9845d
#define H_CcFastMdlReadWait 0x2920b793
#define H_CcFlushCache 0x50db43c2
#define H_CcFlushCacheToLsn 0xcb2927f9
#define H_CcGetDirtyPages 0x79a09d13
#define H_CcGetFileObjectFromBcb 0x4afd1b8c
#define H_CcGetFileObjectFromSectionPtrs 0x67be6755
#define H_CcGetFileObjectFromSectionPtrsRef 0xeab8452a
#define H_CcGetFlushedValidData 0x4396619f
#define H_CcGetLsnForFileObject 0xc21dbe97
#define H_CcInitializeCacheMap 0x43c7b638
#define H_CcIsThereDirtyData 0xdb249eba
#define H_CcIsThereDirtyDataEx 0x27ae9431
#define H_CcIsThereDirtyLoggedPages 0x2241d198
#define H_CcMapData 0x4b5f4d6e
#define H_CcMdlRead 0x899e47c2
#define H_CcMdlReadComplete 0xcf975b4d
#define H_CcMdlWriteAbort 0xf254b07c
#define H_CcMdlWriteComplete 0x2c8304e3
#define H_CcPinMappedData 0xb47483e6
#define H_CcPinRead 0xa99e33aa
#define H_CcPrepareMdlWrite 0x8ddc363a
#define H_CcPreparePinWrite 0x8de6022a
#define H_CcPurgeCacheSection 0x19d9ab6a
#define H_CcRegisterExternalCache 0xa154ab1c
#define H_CcRemapBcb 0xfdb526ad
#define H_CcRepinBcb 0x7e752645
#define H_CcScheduleReadAhead 0x104f8b41
#define H_CcScheduleReadAheadEx 0xe2d066eb
#define H_CcSetAdditionalCacheAttributes 0xc93f2b1a
#define H_CcSetAdditionalCacheAttributesEx 0xcac690b7
#define H_CcSetBcbOwnerPointer 0x0ff46d08
#define H_CcSetDirtyPageThreshold 0x09b2ddd6
#define H_CcSetDirtyPinnedData 0x878ebe4c
#define H_CcSetFileSizes 0x65d778a6
#define H_CcSetFileSizesEx 0xde29bb8d
#define H_CcSetLogHandleForFile 0x8a6df363
#define H_CcSetLogHandleForFileEx 0x7cd8c063
#define H_CcSetLoggedDataThreshold 0x74ff91f8
#define H_CcSetParallelFlushFile 0xb880c13f
#define H_CcSetReadAheadGranularity 0x6e50b6db
#define H_CcSetReadAheadGranularityEx 0x2db6f96c
#define H_CcTestControl 0x1949cda0
#define H_CcUninitializeCacheMap 0x11d984fb
#define H_CcUnmapFileOffsetFromSystemCache 0x6c132a09
#define H_CcUnpinData 0x3d45a9de
#define H_CcUnpinDataForThread 0xd6246ead
#define H_CcUnpinRepinnedBcb 0x0b3d759d
#define H_CcUnregisterExternalCache 0xf93ee8da
#define H_CcWaitForCurrentLazyWriterActivity 0x4afd4d96
#define H_CcZeroData 0x1b2dedd6
#define H_CcZeroDataOnDisk 0x5f2d23f9
#define H_CmCallbackGetKeyObjectID 0x1cc3e7d2
#define H_CmCallbackGetKeyObjectIDEx 0xf9f4a5c8
#define H_CmCallbackReleaseKeyObjectIDEx 0x0e57000b
#define H_CmGetBoundTransaction 0x035b95d7
#define H_CmGetCallbackVersion 0xa0855505
#define H_CmKeyObjectType 0xc4f75012
#define H_CmRegisterCallback 0x21ac2e28
#define H_CmRegisterCallbackEx 0x0b8a2a93
#define H_CmSetCallbackObjectContext 0xf06b3bab
#define H_CmUnRegisterCallback 0x6aaf2653
#define H_DbgBreakPoint 0x7b7da9c8
#define H_DbgBreakPointWithStatus 0x7abeea59
#define H_DbgCommandString 0x93ba9705
#define H_DbgLoadImageSymbols 0x04be1b63
#define H_DbgPrint 0x0ed3fc49
#define H_DbgPrintEx 0xff12614c
#define H_DbgPrintReturnControlC 0x8dcd256b
#define H_DbgPrompt 0x693ee6f3
#define H_DbgQueryDebugFilterState 0x37698be3
#define H_DbgSetDebugFilterState 0x35ffb927
#define H_DbgSetDebugPrintCallback 0x2c895519
#define H_DbgkLkmdRegisterCallback 0x7d9b973b
#define H_DbgkLkmdUnregisterCallback 0x84ebb15e
#define H_DbgkWerCaptureLiveKernelDump 0x7eb426b6
#define H_EmClientQueryRuleState 0x55e4705d
#define H_EmClientRuleDeregisterNotification 0xe72babe9
#define H_EmClientRuleEvaluate 0xbd952731
#define H_EmClientRuleRegisterNotification 0x4e30dce8
#define H_EmProviderDeregister 0xdda6ce0e
#define H_EmProviderDeregisterEntry 0xb0eb490b
#define H_EmProviderRegister 0x31583d35
#define H_EmProviderRegisterEntry 0xd71cd0d4
#define H_EmpProviderRegister 0x65453d24
#define H_EtwActivityIdControl 0x53f94ac0
#define H_EtwEnableTrace 0x00d333bd
#define H_EtwEventEnabled 0x55ca9937
#define H_EtwProviderEnabled 0xcdb410a3
#define H_EtwRegister 0xe9d8b4b6
#define H_EtwRegisterClassicProvider 0x59efe9a5
#define H_EtwSendTraceBuffer 0x31065e93
#define H_EtwUnregister 0x74c63826
#define H_EtwWrite 0x7ed1a9d8
#define H_EtwWriteEndScenario 0xbda0256c
#define H_EtwWriteEx 0x6a763d4c
#define H_EtwWriteStartScenario 0x5e532ef6
#define H_EtwWriteString 0x08fd1403
#define H_EtwWriteTransfer 0x36aea855
#define H_ExAcquireCacheAwarePushLockExclusive 0xa000171a
#define H_ExAcquireCacheAwarePushLockExclusiveEx 0x05c68af8
#define H_ExAcquireCacheAwarePushLockSharedEx 0x1d699631
#define H_ExAcquireFastMutex 0xb5db4261
#define H_ExAcquireFastMutexUnsafe 0x8371f71c
#define H_ExAcquirePushLockExclusiveEx 0xd0a1da76
#define H_ExAcquirePushLockSharedEx 0x27ede09a
#define H_ExAcquireResourceExclusiveLite 0x0400d64d
#define H_ExAcquireResourceSharedLite 0xb1c2f003
#define H_ExAcquireRundownProtection 0x57c4116c
#define H_ExAcquireRundownProtectionCacheAware 0x0e3b817d
#define H_ExAcquireRundownProtectionCacheAwareEx 0xe05f6176
#define H_ExAcquireRundownProtectionEx 0x045b3709
#define H_ExAcquireSharedStarveExclusive 0xb72787bc
#define H_ExAcquireSharedWaitForExclusive 0x0dd421b9
#define H_ExAcquireSpinLockExclusive 0xd23848ef
#define H_ExAcquireSpinLockExclusiveAtDpcLevel 0xb6264a24
#define H_ExAcquireSpinLockShared 0x11dbde37
#define H_ExAcquireSpinLockSharedAtDpcLevel 0x4ec3fc14
#define H_ExAllocateCacheAwarePushLock 0xa0fbebe7
#define H_ExAllocateCacheAwareRundownProtection 0x7e09521a
#define H_ExAllocatePool 0x1a544b7e
#define H_ExAllocatePoolWithQuota 0x44420c9b
#define H_ExAllocatePoolWithQuotaTag 0x937db8a6
#define H_ExAllocatePoolWithTag 0xdbe85b00
#define H_ExAllocatePoolWithTagPriority 0xfe20596c
#define H_ExAllocateTimer 0x6ae53bff
#define H_ExBlockOnAddressPushLock 0x4bad6f78
#define H_ExBlockPushLock 0xa617bc93
#define H_ExCancelTimer 0xa7a3f567
#define H_ExCompositionObjectType 0x617dce80
#define H_ExConvertExclusiveToSharedLite 0x5127cb5e
#define H_ExCreateCallback 0xb6f3c515
#define H_ExDeleteLookasideListEx 0x2c794b6e
#define H_ExDeleteNPagedLookasideList 0x9682e2c8
#define H_ExDeletePagedLookasideList 0x63954ca0
#define H_ExDeleteResourceLite 0x324e392b
#define H_ExDeleteTimer 0xa5afb0ed
#define H_ExDesktopObjectType 0x8b42d883
#define H_ExDisableResourceBoostLite 0x6175b215
#define H_ExEnterCriticalRegionAndAcquireFastMutexUnsafe 0xc53a5df8
#define H_ExEnterCriticalRegionAndAcquireResourceExclusive 0x3313d312
#define H_ExEnterCriticalRegionAndAcquireResourceShared 0x4d04313e
#define H_ExEnterCriticalRegionAndAcquireSharedWaitForExclusive 0x581cad2e
#define H_ExEnterPriorityRegionAndAcquireResourceExclusive 0x9156994b
#define H_ExEnterPriorityRegionAndAcquireResourceShared 0x6756fc2c
#define H_ExEnumHandleTable 0xd5e9466f
#define H_ExEventObjectType 0x1e53c34a
#define H_ExExtendZone 0xb3bade31
#define H_ExFetchLicenseData 0x3f965f5c
#define H_ExFlushLookasideListEx 0x3195950f
#define H_ExFreeCacheAwarePushLock 0x43f2359d
#define H_ExFreeCacheAwareRundownProtection 0x0f8dbd27
#define H_ExFreePool 0xa2963ce0
#define H_ExFreePoolWithTag 0x34d52a84
#define H_ExGetCurrentProcessorCounts 0xea15f9c0
#define H_ExGetCurrentProcessorCpuUsage 0xce94f563
#define H_ExGetExclusiveWaiterCount 0xb5bbb390
#define H_ExGetFirmwareEnvironmentVariable 0x2457bb7b
#define H_ExGetLicenseTamperState 0x95e8e3eb
#define H_ExGetPreviousMode 0xe067a067
#define H_ExGetSharedWaiterCount 0x3a342c6e
#define H_ExInitializeLookasideListEx 0x8b69d232
#define H_ExInitializeNPagedLookasideList 0x5cf3eb5d
#define H_ExInitializePagedLookasideList 0x4801aeb3
#define H_ExInitializePushLock 0x86c96765
#define H_ExInitializeResourceLite 0xb684dc13
#define H_ExInitializeRundownProtection 0x6d10145b
#define H_ExInitializeRundownProtectionCacheAware 0xbb3accb3
#define H_ExInitializeZone 0xf46fe665
#define H_ExInterlockedAddLargeInteger 0xd16ca7e2
#define H_ExInterlockedAddUlong 0xa8948a69
#define H_ExInterlockedExtendZone 0x88389d52
#define H_ExInterlockedInsertHeadList 0x405d3857
#define H_ExInterlockedInsertTailList 0xc0652817
#define H_ExInterlockedPopEntryList 0x63157f79
#define H_ExInterlockedPushEntryList 0x752e67c8
#define H_ExInterlockedRemoveHeadList 0x89eefad5
#define H_ExIsProcessorFeaturePresent 0xb1f7638c
#define H_ExIsResourceAcquiredExclusiveLite 0xb0375442
#define H_ExIsResourceAcquiredSharedLite 0x0dd28da2
#define H_ExLocalTimeToSystemTime 0xaa0f5a7c
#define H_ExNotifyBootDeviceRemoval 0x1dc79101
#define H_ExNotifyCallback 0xc321e71c
#define H_ExQueryDepthSList 0xf2600594
#define H_ExQueryFastCacheAppOrigin 0xd60bb0de
#define H_ExQueryFastCacheDevLicense 0x3095ad54
#define H_ExQueryPoolBlockSize 0x1d5bfaf8
#define H_ExQueryTimerResolution 0x057c0d4e
#define H_ExQueryWnfStateData 0xe0baedf2
#define H_ExQueueWorkItem 0x998c3516
#define H_ExRaiseAccessViolation 0xa0a9352e
#define H_ExRaiseDatatypeMisalignment 0x46ba898f
#define H_ExRaiseException 0x5d764fd1
#define H_ExRaiseHardError 0x0082cbc6
#define H_ExRaiseStatus 0xa185b8dc
#define H_ExReInitializeRundownProtection 0xcc07d528
#define H_ExReInitializeRundownProtectionCacheAware 0xfeca905b
#define H_ExRealTimeIsUniversal 0x77b86364
#define H_ExRegisterBootDevice 0x872cd87b
#define H_ExRegisterCallback 0x22b92e28
#define H_ExRegisterExtension 0x98e7f637
#define H_ExReinitializeResourceLite 0xc3a6243d
#define H_ExReleaseCacheAwarePushLockExclusive 0x9a405e8c
#define H_ExReleaseCacheAwarePushLockExclusiveEx 0x17a30468
#define H_ExReleaseCacheAwarePushLockSharedEx 0x311d16a2
#define H_ExReleaseFastMutex 0x5cda6439
#define H_ExReleaseFastMutexUnsafe 0x87e894b8
#define H_ExReleaseFastMutexUnsafeAndLeaveCriticalRegion 0x72e45b1b
#define H_ExReleasePushLockEx 0xdbd1f254
#define H_ExReleasePushLockExclusiveEx 0x90e84c4c
#define H_ExReleasePushLockSharedEx 0x6b5c3298
#define H_ExReleaseResourceAndLeaveCriticalRegion 0xb42db8f4
#define H_ExReleaseResourceAndLeavePriorityRegion 0xa607ea39
#define H_ExReleaseResourceForThreadLite 0x6cbe92bb
#define H_ExReleaseResourceLite 0x9ac37c91
#define H_ExReleaseRundownProtection 0x0f2d104a
#define H_ExReleaseRundownProtectionCacheAware 0x347bc8eb
#define H_ExReleaseRundownProtectionCacheAwareEx 0xf23aefe6
#define H_ExReleaseRundownProtectionEx 0x4412a133
#define H_ExReleaseSpinLockExclusive 0x8ad149c9
#define H_ExReleaseSpinLockExclusiveFromDpcLevel 0xbda40053
#define H_ExReleaseSpinLockShared 0x59d2ecf0
#define H_ExReleaseSpinLockSharedFromDpcLevel 0xee393069
#define H_ExRundownCompleted 0xdbd7dd42
#define H_ExRundownCompletedCacheAware 0x0ac88ade
#define H_ExSemaphoreObjectType 0x1c5c2aca
#define H_ExSetFirmwareEnvironmentVariable 0x24571b7b
#define H_ExSetLicenseTamperState 0x95e9a3eb
#define H_ExSetResourceOwnerPointer 0xd0a16818
#define H_ExSetResourceOwnerPointerEx 0x5a0616d0
#define H_ExSetTimer 0xb59ce575
#define H_ExSetTimerResolution 0x5c4495cd
#define H_ExSizeOfRundownProtectionCacheAware 0x56a8be1d
#define H_ExSubscribeWnfStateChange 0xe5f95f1e
#define H_ExSystemExceptionFilter 0xdfded0d4
#define H_ExSystemTimeToLocalTime 0xa539a8b2
#define H_ExTimedWaitForUnblockPushLock 0xb0f2ac7a
#define H_ExTryAcquirePushLockExclusiveEx 0x31f61f30
#define H_ExTryAcquirePushLockSharedEx 0x99c7d790
#define H_ExTryConvertPushLockSharedToExclusiveEx 0x18ec362c
#define H_ExTryConvertSharedSpinLockExclusive 0xea166349
#define H_ExTryQueueWorkItem 0xb3749dca
#define H_ExTryToAcquireFastMutex 0xea890c3e
#define H_ExTryToAcquireResourceExclusiveLite 0xe1f52369
#define H_ExUnblockOnAddressPushLockEx 0xe5d6265b
#define H_ExUnblockPushLockEx 0x9334e7ec
#define H_ExUnregisterCallback 0x2aaf6696
#define H_ExUnregisterExtension 0x93c3a933
#define H_ExUnsubscribeWnfStateChange 0x74854f32
#define H_ExUuidCreate 0xd9635650
#define H_ExVerifySuite 0xc836f41a
#define H_ExWaitForRundownProtectionRelease 0xc5f71bc3
#define H_ExWaitForRundownProtectionReleaseCacheAware 0x82f92a99
#define H_ExWaitForUnblockPushLock 0xd5c64565
#define H_ExWindowStationObjectType 0xaa4b5430
#define H_ExfAcquirePushLockExclusive 0xf365dc87
#define H_ExfAcquirePushLockShared 0xfd789f3d
#define H_ExfReleasePushLock 0x600e5347
#define H_ExfReleasePushLockExclusive 0xab8cdda1
#define H_ExfReleasePushLockShared 0xb571adfa
#define H_ExfTryAcquirePushLockShared 0xc4fcf91f
#define H_ExfTryToWakePushLock 0xf9943358
#define H_ExfUnblockPushLock 0x36ef70d3
#define H_ExpInterlockedFlushSList 0x9abb4b7b
#define H_ExpInterlockedPopEntrySList 0x6f47b744
#define H_ExpInterlockedPushEntrySList 0xccd167c7
#define H_FirstEntrySList 0x985dce00
#define H_FsRtlAcknowledgeEcp 0x3c8774c2
#define H_FsRtlAcquireEofLock 0x2cbc3242
#define H_FsRtlAcquireFileExclusive 0x04b59224
#define H_FsRtlAcquireHeaderMutex 0x4b26da13
#define H_FsRtlAddBaseMcbEntry 0x777a3fcf
#define H_FsRtlAddBaseMcbEntryEx 0x8ff3ff26
#define H_FsRtlAddLargeMcbEntry 0x26e26739
#define H_FsRtlAddMcbEntry 0xae0babd1
#define H_FsRtlAddToTunnelCache 0x4dc07bdc
#define H_FsRtlAllocateExtraCreateParameter 0xfb8b9636
#define H_FsRtlAllocateExtraCreateParameterFromLookasideList 0xcf8b0553
#define H_FsRtlAllocateExtraCreateParameterList 0x6622c097
#define H_FsRtlAllocateFileLock 0xe082c330
#define H_FsRtlAllocatePool 0x9352ac37
#define H_FsRtlAllocatePoolWithQuota 0x80c17f3f
#define H_FsRtlAllocatePoolWithQuotaTag 0xe7e528c8
#define H_FsRtlAllocatePoolWithTag 0x157b490d
#define H_FsRtlAllocateResource 0x478db3c6
#define H_FsRtlAreNamesEqual 0x788b1bc4
#define H_FsRtlAreThereCurrentOrInProgressFileLocks 0xdac6fc2a
#define H_FsRtlAreThereWaitingFileLocks 0x8294f71f
#define H_FsRtlAreVolumeStartupApplicationsComplete 0x7f021307
#define H_FsRtlBalanceReads 0xebeb4263
#define H_FsRtlCancellableWaitForMultipleObjects 0xb9ba6fe7
#define H_FsRtlCancellableWaitForSingleObject 0x7304f345
#define H_FsRtlChangeBackingFileObject 0x3b7094ee
#define H_FsRtlCheckLockForOplockRequest 0x52516923
#define H_FsRtlCheckLockForReadAccess 0x53bc968a
#define H_FsRtlCheckLockForWriteAccess 0x56ae5501
#define H_FsRtlCheckOplock 0xff1142ae
#define H_FsRtlCheckOplockEx 0x50ab9d3c
#define H_FsRtlCheckUpperOplock 0xe905a73b
#define H_FsRtlCopyRead 0xf2b0bbd9
#define H_FsRtlCopyWrite 0x0abfe49c
#define H_FsRtlCreateSectionForDataScan 0x707d175c
#define H_FsRtlCurrentBatchOplock 0xedda0875
#define H_FsRtlCurrentOplock 0x2f0b3afe
#define H_FsRtlCurrentOplockH 0x859d7f5f
#define H_FsRtlDeleteExtraCreateParameterLookasideList 0xf9de581b
#define H_FsRtlDeleteKeyFromTunnelCache 0xc221135e
#define H_FsRtlDeleteTunnelCache 0xc9da27d5
#define H_FsRtlDeregisterUncProvider 0xc966402c
#define H_FsRtlDismountComplete 0x5dabdfae
#define H_FsRtlDissectDbcs 0x2c323a16
#define H_FsRtlDissectName 0x2d72fd00
#define H_FsRtlDoesDbcsContainWildCards 0x66c5ece0
#define H_FsRtlDoesNameContainWildCards 0xa1d3eda0
#define H_FsRtlFastCheckLockForRead 0xd5e11130
#define H_FsRtlFastCheckLockForWrite 0xa26a900f
#define H_FsRtlFastUnlockAll 0x2fb25c3b
#define H_FsRtlFastUnlockAllByKey 0x52a01324
#define H_FsRtlFastUnlockSingle 0xd73c00be
#define H_FsRtlFindExtraCreateParameter 0x739c279c
#define H_FsRtlFindInTunnelCache 0xc2095fdd
#define H_FsRtlFreeExtraCreateParameter 0x1fc4379c
#define H_FsRtlFreeExtraCreateParameterList 0xc8663a8d
#define H_FsRtlFreeFileLock 0x1a986d74
#define H_FsRtlGetEcpListFromIrp 0x1415805d
#define H_FsRtlGetFileNameInformation 0x61f249b4
#define H_FsRtlGetFileSize 0x39bac63f
#define H_FsRtlGetIoAtEof 0x6656caa2
#define H_FsRtlGetNextBaseMcbEntry 0x436d3e9f
#define H_FsRtlGetNextExtraCreateParameter 0x2bab2caf
#define H_FsRtlGetNextFileLock 0xeb2b5e32
#define H_FsRtlGetNextLargeMcbEntry 0x2d62cf23
#define H_FsRtlGetNextMcbEntry 0xef7bbed2
#define H_FsRtlGetSectorSizeInformation 0x2e99bfe1
#define H_FsRtlGetSupportedFeatures 0x1d3a1d13
#define H_FsRtlGetVirtualDiskNestingLevel 0x174c0f54
#define H_FsRtlHeatInit 0x21c45741
#define H_FsRtlHeatLogIo 0xb2083e7f
#define H_FsRtlHeatLogTierMove 0x09781e82
#define H_FsRtlHeatUninit 0x15d6fc65
#define H_FsRtlIncrementCcFastMdlReadWait 0xc23118c1
#define H_FsRtlIncrementCcFastReadNoWait 0x0cf33f7b
#define H_FsRtlIncrementCcFastReadNotPossible 0x79c35582
#define H_FsRtlIncrementCcFastReadResourceMiss 0x9b7c0c4c
#define H_FsRtlIncrementCcFastReadWait 0x2d1facd9
#define H_FsRtlInitExtraCreateParameterLookasideList 0xafcd0c1b
#define H_FsRtlInitializeBaseMcb 0x549aa623
#define H_FsRtlInitializeBaseMcbEx 0xa988f7de
#define H_FsRtlInitializeEofLock 0x01b23a5b
#define H_FsRtlInitializeExtraCreateParameter 0xd3114999
#define H_FsRtlInitializeExtraCreateParameterList 0x94ab6d6d
#define H_FsRtlInitializeFileLock 0x4d7831b9
#define H_FsRtlInitializeLargeMcb 0x68449140
#define H_FsRtlInitializeMcb 0x829ebd04
#define H_FsRtlInitializeOplock 0x914ba22d
#define H_FsRtlInitializeTunnelCache 0x3467fbf7
#define H_FsRtlInsertExtraCreateParameter 0xadf83cb6
#define H_FsRtlInsertPerFileContext 0x764f48b0
#define H_FsRtlInsertPerFileObjectContext 0x5562c3db
#define H_FsRtlInsertPerStreamContext 0x40ff72bf
#define H_FsRtlInsertReservedPerFileContext 0xf31ef43f
#define H_FsRtlInsertReservedPerStreamContext 0x2fdc93eb
#define H_FsRtlIsDbcsInExpression 0xe6cc2072
#define H_FsRtlIsEcpAcknowledged 0x9930c83d
#define H_FsRtlIsEcpFromUserMode 0xef663cb7
#define H_FsRtlIsFatDbcsLegal 0x7e964ea9
#define H_FsRtlIsHpfsDbcsLegal 0x7d86b732
#define H_FsRtlIsNameInExpression 0x97ac347e
#define H_FsRtlIsNtstatusExpected 0x0fd45f11
#define H_FsRtlIsPagingFile 0xbf9a0ab0
#define H_FsRtlIsSystemPagingFile 0xb937e306
#define H_FsRtlIsTotalDeviceFailure 0xbbd3b983
#define H_FsRtlIssueDeviceIoControl 0xba7dc4a8
#define H_FsRtlKernelFsControlFile 0x7e385d64
#define H_FsRtlLegalAnsiCharacterArray 0x2c10f491
#define H_FsRtlLogCcFlushError 0xa5bbd1ba
#define H_FsRtlLookupBaseMcbEntry 0x2abc1f21
#define H_FsRtlLookupLargeMcbEntry 0xc5f21017
#define H_FsRtlLookupLastBaseMcbEntry 0x7e215880
#define H_FsRtlLookupLastBaseMcbEntryAndIndex 0x1d24a884
#define H_FsRtlLookupLastLargeMcbEntry 0x8b51c0bd
#define H_FsRtlLookupLastLargeMcbEntryAndIndex 0x20d1d81c
#define H_FsRtlLookupLastMcbEntry 0x3bbddf21
#define H_FsRtlLookupMcbEntry 0x7269a534
#define H_FsRtlLookupPerFileContext 0x38414600
#define H_FsRtlLookupPerFileObjectContext 0x6d5802e3
#define H_FsRtlLookupPerStreamContextInternal 0x620b582b
#define H_FsRtlLookupReservedPerFileContext 0x4350fa31
#define H_FsRtlLookupReservedPerStreamContext 0xac5f3ff8
#define H_FsRtlMdlRead 0xbe889b2b
#define H_FsRtlMdlReadComplete 0x26a04d91
#define H_FsRtlMdlReadCompleteDev 0xb235e6ff
#define H_FsRtlMdlReadDev 0x6566e3e5
#define H_FsRtlMdlReadEx 0x26cacd5a
#define H_FsRtlMdlWriteComplete 0xb7086a97
#define H_FsRtlMdlWriteCompleteDev 0x52e7d3fb
#define H_FsRtlMupGetProviderIdFromName 0xf855caac
#define H_FsRtlMupGetProviderInfoFromFileObject 0x4486d120
#define H_FsRtlNormalizeNtstatus 0x73c49687
#define H_FsRtlNotifyChangeDirectory 0x5bf486f2
#define H_FsRtlNotifyCleanup 0xfd56ed71
#define H_FsRtlNotifyCleanupAll 0xae2fdcb1
#define H_FsRtlNotifyFilterChangeDirectory 0x0385000f
#define H_FsRtlNotifyFilterReportChange 0x6481157d
#define H_FsRtlNotifyFullChangeDirectory 0x88c3247e
#define H_FsRtlNotifyFullReportChange 0x55a29927
#define H_FsRtlNotifyInitializeSync 0x4485af5b
#define H_FsRtlNotifyReportChange 0xe8b6ffbe
#define H_FsRtlNotifyUninitializeSync 0x25fc6a49
#define H_FsRtlNotifyVolumeEvent 0xc8241061
#define H_FsRtlNotifyVolumeEventEx 0x041850f1
#define H_FsRtlNumberOfRunsInBaseMcb 0x6262b975
#define H_FsRtlNumberOfRunsInLargeMcb 0x144b3a5b
#define H_FsRtlNumberOfRunsInMcb 0xed1f4867
#define H_FsRtlOplockBreakH 0xe5f0c0a2
#define H_FsRtlOplockBreakToNone 0xd65df294
#define H_FsRtlOplockBreakToNoneEx 0x7ca5176f
#define H_FsRtlOplockFsctrl 0xf5358c26
#define H_FsRtlOplockFsctrlEx 0x63099fb5
#define H_FsRtlOplockIsFastIoPossible 0xb6aa3862
#define H_FsRtlOplockIsSharedRequest 0x02e9703b
#define H_FsRtlOplockKeysEqual 0xfa4fbe66
#define H_FsRtlPostPagingFileStackOverflow 0x034bbe85
#define H_FsRtlPostStackOverflow 0x561d913b
#define H_FsRtlPrepareMdlWrite 0x64eb20e6
#define H_FsRtlPrepareMdlWriteDev 0x1cddaf92
#define H_FsRtlPrepareMdlWriteEx 0xc839bbc2
#define H_FsRtlPrepareToReuseEcp 0xe16d60ff
#define H_FsRtlPrivateLock 0xa73b6fba
#define H_FsRtlProcessFileLock 0xd59efe6b
#define H_FsRtlQueryCachedVdl 0xa9d86835
#define H_FsRtlQueryKernelEaFile 0xd5ee6cf8
#define H_FsRtlQueryMaximumVirtualDiskNestingLevel 0x3ffd5825
#define H_FsRtlRegisterFileSystemFilterCallbacks 0x00c60643
#define H_FsRtlRegisterFltMgrCalls 0x928e6687
#define H_FsRtlRegisterMupCalls 0x4a565726
#define H_FsRtlRegisterUncProvider 0xd4b6322c
#define H_FsRtlRegisterUncProviderEx 0x8c8b17d5
#define H_FsRtlReleaseEofLock 0xb5df9646
#define H_FsRtlReleaseFile 0x235ce887
#define H_FsRtlReleaseFileNameInformation 0xcf48018a
#define H_FsRtlReleaseHeaderMutex 0x02b0e053
#define H_FsRtlRemoveBaseMcbEntry 0x3ce61b31
#define H_FsRtlRemoveDotsFromPath 0x2d35dfc9
#define H_FsRtlRemoveExtraCreateParameter 0x85d4a78a
#define H_FsRtlRemoveLargeMcbEntry 0xe8f0181c
#define H_FsRtlRemoveMcbEntry 0x17c9e435
#define H_FsRtlRemovePerFileContext 0xb9454396
#define H_FsRtlRemovePerFileObjectContext 0x7d4e58e7
#define H_FsRtlRemovePerStreamContext 0xc236c17d
#define H_FsRtlRemoveReservedPerFileContext 0xd5d1fe34
#define H_FsRtlRemoveReservedPerStreamContext 0xed5e5a58
#define H_FsRtlResetBaseMcb 0x7fd6c4df
#define H_FsRtlResetLargeMcb 0xce75ef55
#define H_FsRtlSendModernAppTermination 0xf2e60162
#define H_FsRtlSetEcpListIntoIrp 0xa441bcbc
#define H_FsRtlSetKernelEaFile 0xb02c6ca1
#define H_FsRtlSplitBaseMcb 0xf3d6e190
#define H_FsRtlSplitLargeMcb 0xce674893
#define H_FsRtlSyncVolumes 0xb73130bf
#define H_FsRtlTeardownPerFileContexts 0xbe21e8b9
#define H_FsRtlTeardownPerStreamContexts 0x138539b2
#define H_FsRtlTruncateBaseMcb 0xb6517392
#define H_FsRtlTruncateLargeMcb 0x0dae49b1
#define H_FsRtlTruncateMcb 0xae23e61a
#define H_FsRtlTryToAcquireHeaderMutex 0x1fc51ac3
#define H_FsRtlUninitializeBaseMcb 0xd91ad166
#define H_FsRtlUninitializeFileLock 0x8d43937f
#define H_FsRtlUninitializeLargeMcb 0xa87f3386
#define H_FsRtlUninitializeMcb 0x5a99c95c
#define H_FsRtlUninitializeOplock 0x1a50a2c3
#define H_FsRtlUpdateDiskCounters 0x922bbbb9
#define H_FsRtlUpperOplockFsctrl 0x48f1b0f3
#define H_FsRtlValidateReparsePointBuffer 0x764bd73b
#define H_HalDispatchTable 0x499c945f
#define H_HalExamineMBR 0x10fced3e
#define H_HalPrivateDispatchTable 0x96640c16
#define H_HeadlessDispatch 0xf1f99e99
#define H_HvlGetLpIndexFromApicId 0x5051c517
#define H_HvlPerformEndOfInterrupt 0x2166d133
#define H_HvlQueryActiveHypervisorProcessorCount 0x084ba288
#define H_HvlQueryActiveProcessors 0x48263293
#define H_HvlQueryConnection 0x5b6f4be6
#define H_HvlQueryHypervisorProcessorNodeNumber 0x4a9ff772
#define H_HvlQueryNumaDistance 0x8c7a2986
#define H_HvlQueryProcessorTopology 0x1f33c166
#define H_HvlQueryProcessorTopologyCount 0xc4637c40
#define H_HvlQueryProcessorTopologyHighestId 0x19375b11
#define H_HvlRegisterInterruptCallback 0x6c411a7d
#define H_HvlRegisterWheaErrorNotification 0xc701051d
#define H_HvlUnregisterInterruptCallback 0xe4827d19
#define H_HvlUnregisterWheaErrorNotification 0x8f8d336b
#define H_InbvAcquireDisplayOwnership 0x1295e253
#define H_InbvCheckDisplayOwnership 0x84c7eac0
#define H_InbvDisplayString 0x969d708b
#define H_InbvEnableBootDriver 0xfdb97b34
#define H_InbvEnableDisplayString 0x5ec34663
#define H_InbvInstallDisplayStringFilter 0x41815dd4
#define H_InbvIsBootDriverInstalled 0xfe831a85
#define H_InbvNotifyDisplayOwnershipChange 0x4202dea5
#define H_InbvNotifyDisplayOwnershipLost 0xb4e0c111
#define H_InbvResetDisplay 0xcc98e974
#define H_InbvSetScrollRegion 0x6b0b50dc
#define H_InbvSetTextColor 0x7e5a3e97
#define H_InbvSolidColorFill 0xd1169abb
#define H_InitSafeBootMode 0x6374a6d2
#define H_InitializeSListHead 0xe324007c
#define H_InterlockedPushListSList 0xdc0283fa
#define H_IoAcquireCancelSpinLock 0x46b413bc
#define H_IoAcquireRemoveLockEx 0xf4385bbf
#define H_IoAcquireVpbSpinLock 0x24f36df1
#define H_IoAdapterObjectType 0x58c69a48
#define H_IoAdjustStackSizeForRedirection 0xd1658e6b
#define H_IoAllocateAdapterChannel 0x399da50e
#define H_IoAllocateController 0xecb4a881
#define H_IoAllocateDriverObjectExtension 0x8f7e2881
#define H_IoAllocateErrorLogEntry 0x938504bb
#define H_IoAllocateIrp 0x24f00609
#define H_IoAllocateMdl 0x24f10d15
#define H_IoAllocateMiniCompletionPacket 0xd1532e32
#define H_IoAllocateSfioStreamIdentifier 0xe6302ea5
#define H_IoAllocateWorkItem 0x9e8176b2
#define H_IoApplyPriorityInfoThread 0x99ffc7d8
#define H_IoAssignResources 0x93e7db7c
#define H_IoAttachDevice 0x71e5eefb
#define H_IoAttachDeviceByPointer 0x67dc86f1
#define H_IoAttachDeviceToDeviceStack 0x16892083
#define H_IoAttachDeviceToDeviceStackSafe 0x3b10e16d
#define H_IoBoostThreadIo 0xd4bd7da7
#define H_IoBuildAsynchronousFsdRequest 0xc8d3b9f8
#define H_IoBuildDeviceIoControlRequest 0x7ce4cfc2
#define H_IoBuildPartialMdl 0xd2a2e701
#define H_IoBuildSynchronousFsdRequest 0x22a22166
#define H_IoCallDriver 0xcced3b34
#define H_IoCancelFileOpen 0xab6cdec4
#define H_IoCancelIrp 0x9e51d019
#define H_IoCheckDesiredAccess 0x731695bd
#define H_IoCheckEaBufferValidity 0x2ffc9233
#define H_IoCheckFunctionAccess 0x969a4159
#define H_IoCheckQuerySetFileInformation 0x62f2149d
#define H_IoCheckQuerySetVolumeInformation 0x1013845a
#define H_IoCheckQuotaBufferValidity 0xa08e1fd8
#define H_IoCheckShareAccess 0x574e1caa
#define H_IoCheckShareAccessEx 0x872ab72b
#define H_IoClearActivityIdThread 0x06da862f
#define H_IoClearDependency 0x4f4fc522
#define H_IoClearIrpExtraCreateParameter 0xfcf7a062
#define H_IoCompleteRequest 0x94e5f6e6
#define H_IoCompletionObjectType 0xfa683531
#define H_IoConnectInterrupt 0x3583941e
#define H_IoConnectInterruptEx 0xe507af98
#define H_IoConvertFileHandleToKernelHandle 0x039f77fe
#define H_IoCopyDeviceObjectHint 0xe764e8e0
#define H_IoCreateArcName 0xb12d2924
#define H_IoCreateController 0x18b7ffce
#define H_IoCreateDevice 0xf1cb9b73
#define H_IoCreateDisk 0xaa58c5ac
#define H_IoCreateDriver 0x822c5865
#define H_IoCreateFile 0xaa18ca22
#define H_IoCreateFileEx 0x3288887e
#define H_IoCreateFileSpecifyDeviceObjectHint 0x759dd8a5
#define H_IoCreateNotificationEvent 0xd93f3ac1
#define H_IoCreateStreamFileObject 0x16a65711
#define H_IoCreateStreamFileObjectEx 0x95c46751
#define H_IoCreateStreamFileObjectEx2 0xe233a8f8
#define H_IoCreateStreamFileObjectLite 0x18f01f14
#define H_IoCreateSymbolicLink 0x9ced78ef
#define H_IoCreateSynchronizationEvent 0x1fc38fc7
#define H_IoCreateSystemThread 0x3d3a7a87
#define H_IoCreateUnprotectedSymbolicLink 0x004afb2e
#define H_IoCsqInitialize 0x8f77d368
#define H_IoCsqInitializeEx 0xf4da0125
#define H_IoCsqInsertIrp 0xa9b7caa5
#define H_IoCsqInsertIrpEx 0xf2a94895
#define H_IoCsqRemoveIrp 0xf081b2f5
#define H_IoCsqRemoveNextIrp 0xd19a7ee7
#define H_IoDecrementKeepAliveCount 0xeab9ba2c
#define H_IoDeleteAllDependencyRelations 0x8427b1bb
#define H_IoDeleteController 0x50f7f192
#define H_IoDeleteDevice 0x75cb7eb7
#define H_IoDeleteDriver 0x062cbda1
#define H_IoDeleteSymbolicLink 0x9f7a6aff
#define H_IoDetachDevice 0x71e54abb
#define H_IoDeviceHandlerObjectSize 0x6f84b181
#define H_IoDeviceHandlerObjectType 0x6f60b481
#define H_IoDeviceObjectType 0x3a381f8a
#define H_IoDisconnectInterrupt 0x259b49ef
#define H_IoDisconnectInterruptEx 0xd27beb9e
#define H_IoDriverObjectType 0xc3c8da16
#define H_IoDuplicateDependency 0x0db0fbbb
#define H_IoEnqueueIrp 0xb25895bd
#define H_IoEnumerateDeviceObjectList 0xa2af859e
#define H_IoEnumerateRegisteredFiltersList 0x4c3d66d4
#define H_IoFastQueryNetworkAttributes 0x310855dc
#define H_IoFileObjectType 0x9ed6e5a3
#define H_IoForwardAndCatchIrp 0xb5d68245
#define H_IoForwardIrpSynchronously 0xf17d8652
#define H_IoFreeController 0x256acbbb
#define H_IoFreeErrorLogEntry 0xf4dc3f77
#define H_IoFreeIrp 0x156d62e6
#define H_IoFreeMdl 0x156c69fa
#define H_IoFreeMiniCompletionPacket 0xa6cbe080
#define H_IoFreeSfioStreamIdentifier 0x91a8e017
#define H_IoFreeWorkItem 0x126a51cb
#define H_IoGetActivityIdIrp 0xeab2581f
#define H_IoGetActivityIdThread 0xa3a42646
#define H_IoGetAffinityInterrupt 0xd162a305
#define H_IoGetAttachedDevice 0xce98b072
#define H_IoGetAttachedDeviceReference 0x193c72f3
#define H_IoGetBaseFileSystemDeviceObject 0x298f1920
#define H_IoGetBootDiskInformation 0x3ea979e7
#define H_IoGetBootDiskInformationLite 0x7a70edfb
#define H_IoGetConfigurationInformation 0x82ce3007
#define H_IoGetContainerInformation 0xe6402cf4
#define H_IoGetCurrentProcess 0x7a09ad17
#define H_IoGetDeviceAttachmentBaseRef 0x066dc390
#define H_IoGetDeviceInterfaceAlias 0x51f1a076
#define H_IoGetDeviceInterfacePropertyData 0xc95b35a6
#define H_IoGetDeviceInterfaces 0xc6bd0867
#define H_IoGetDeviceNumaNode 0xaddf34ba
#define H_IoGetDeviceObjectPointer 0xde00a6b3
#define H_IoGetDeviceProperty 0xb88460b7
#define H_IoGetDevicePropertyData 0x73103c6a
#define H_IoGetDeviceToVerify 0xea6adc7e
#define H_IoGetDiskDeviceObject 0x28d66aa7
#define H_IoGetDmaAdapter 0x0fa289d1
#define H_IoGetDriverObjectExtension 0x51c258d0
#define H_IoGetFileObjectGenericMapping 0xba71f72b
#define H_IoGetGenericIrpExtension 0xa9620db1
#define H_IoGetInitialStack 0xe54b1225
#define H_IoGetInitiatorProcess 0x2e210c55
#define H_IoGetIoPriorityHint 0x4fc207a7
#define H_IoGetIrpExtraCreateParameter 0xbc24ea9f
#define H_IoGetLowerDeviceObject 0x13b459d7
#define H_IoGetOplockKeyContext 0x9f660bdd
#define H_IoGetOplockKeyContextEx 0x82f74521
#define H_IoGetPagingIoPriority 0xdd9654a5
#define H_IoGetRelatedDeviceObject 0x6604e325
#define H_IoGetRequestorProcess 0xee30bef9
#define H_IoGetRequestorProcessId 0x2fbe5f68
#define H_IoGetRequestorSessionId 0xef176fe8
#define H_IoGetSfioStreamIdentifier 0x458d5645
#define H_IoGetStackLimits 0xc223d351
#define H_IoGetSymlinkSupportInformation 0xa0804fe5
#define H_IoGetTopLevelIrp 0x9355e84f
#define H_IoGetTransactionParameterBlock 0xca0ac796
#define H_IoIncrementKeepAliveCount 0xdee1ba2c
#define H_IoInitializeIrp 0xb9ec1162
#define H_IoInitializeMiniCompletionPacket 0x649da039
#define H_IoInitializeRemoveLockEx 0x4f6219e1
#define H_IoInitializeTimer 0xd73fdc88
#define H_IoInitializeWorkItem 0x7661cdee
#define H_IoInvalidateDeviceRelations 0x4910b379
#define H_IoInvalidateDeviceState 0x50970a35
#define H_IoIs32bitProcess 0xdaa6c56a
#define H_IoIsActivityTracingEnabled 0xe0fb2589
#define H_IoIsFileObjectIgnoringSharing 0x3dce13a6
#define H_IoIsFileOriginRemote 0x38c238db
#define H_IoIsInitiator32bitProcess 0xb7434102
#define H_IoIsOperationSynchronous 0xd15c7725
#define H_IoIsSystemThread 0x35a82b85
#define H_IoIsValidIrpStatus 0x972af1d0
#define H_IoIsValidNameGraftingBuffer 0x6430ca17
#define H_IoIsWdmVersionAvailable 0xcff9393f
#define H_IoLoadCrashDumpDriver 0xf91d5e0b
#define H_IoMakeAssociatedIrp 0xe492419c
#define H_IoOpenDeviceInterfaceRegistryKey 0xf3ba2fad
#define H_IoOpenDeviceRegistryKey 0xd6e5bc2f
#define H_IoPageRead 0xb5f8f7fe
#define H_IoPropagateActivityIdToThread 0xcf8acc64
#define H_IoPropagateIrpExtension 0xd2ce59a5
#define H_IoQueryDeviceDescription 0xb12a9a67
#define H_IoQueryFileDosDeviceName 0x1231a285
#define H_IoQueryFileInformation 0x463507d1
#define H_IoQueryFullDriverPath 0x15774277
#define H_IoQueryVolumeInformation 0xd4c08d6b
#define H_IoQueueThreadIrp 0x471f9c92
#define H_IoQueueWorkItem 0x218c3526
#define H_IoQueueWorkItemEx 0x0d49aa9b
#define H_IoQueueWorkItemToNode 0xc90f6845
#define H_IoRaiseHardError 0x0082d39a
#define H_IoRaiseInformationalHardError 0xc3f166d2
#define H_IoReadDiskSignature 0x9ed11173
#define H_IoReadOperationCount 0x5f47be19
#define H_IoReadPartitionTable 0xd7909e8d
#define H_IoReadPartitionTableEx 0x27a3571c
#define H_IoReadTransferCount 0x1cac7302
#define H_IoRegisterBootDriverCallback 0x24eb3d97
#define H_IoRegisterBootDriverReinitialization 0xab467c50
#define H_IoRegisterContainerNotification 0xcd586b8f
#define H_IoRegisterDeviceInterface 0x7b234bc8
#define H_IoRegisterDriverReinitialization 0xa0bc0731
#define H_IoRegisterFileSystem 0x818dde4d
#define H_IoRegisterFsRegistrationChange 0x199111e7
#define H_IoRegisterFsRegistrationChangeMountAware 0x9557bb21
#define H_IoRegisterIoTracking 0xa4fac651
#define H_IoRegisterLastChanceShutdownNotification 0xa2d6d170
#define H_IoRegisterPlugPlayNotification 0xc87727bf
#define H_IoRegisterPriorityCallback 0x5ec97ce4
#define H_IoRegisterShutdownNotification 0x79144fd2
#define H_IoReleaseCancelSpinLock 0x0ebd217b
#define H_IoReleaseRemoveLockAndWaitEx 0xee26ce0c
#define H_IoReleaseRemoveLockEx 0x3f257b9b
#define H_IoReleaseVpbSpinLock 0x6d6557b1
#define H_IoRemoveShareAccess 0x552290ac
#define H_IoReplaceFileObjectName 0x8c8c93b6
#define H_IoReplacePartitionUnit 0xd47c16e6
#define H_IoReportDetectedDevice 0xf66e6539
#define H_IoReportHalResourceUsage 0x89538f30
#define H_IoReportInterruptActive 0x2cb8d048
#define H_IoReportInterruptInactive 0x944104e3
#define H_IoReportResourceForDetection 0x9998bb43
#define H_IoReportResourceUsage 0xbc450200
#define H_IoReportRootDevice 0x93145b7d
#define H_IoReportTargetDeviceChange 0x777748e7
#define H_IoReportTargetDeviceChangeAsynchronous 0x9d6312ed
#define H_IoRequestDeviceEject 0x6dd18506
#define H_IoRequestDeviceEjectEx 0x6141b98c
#define H_IoReserveDependency 0x71a63430
#define H_IoResolveDependency 0x20463430
#define H_IoRetrievePriorityInfo 0xb8b821b2
#define H_IoReuseIrp 0xd317eefb
#define H_IoSetActivityIdIrp 0xeab2701f
#define H_IoSetActivityIdThread 0xa3a42643
#define H_IoSetCompletionRoutineEx 0xa9383119
#define H_IoSetDependency 0x54669a8a
#define H_IoSetDeviceInterfacePropertyData 0xc95b95a6
#define H_IoSetDeviceInterfaceState 0x22f3aae1
#define H_IoSetDevicePropertyData 0x73117c6a
#define H_IoSetDeviceToVerify 0xea7edc7e
#define H_IoSetFileObjectIgnoreSharing 0x99c0214a
#define H_IoSetFileOrigin 0x39dcde95
#define H_IoSetGenericIrpExtension 0xa9c20db1
#define H_IoSetHardErrorOrVerifyDevice 0x29a95801
#define H_IoSetInformation 0xe61bbd49
#define H_IoSetIoCompletion 0x9e415026
#define H_IoSetIoCompletionEx 0x54098568
#define H_IoSetIoPriorityHint 0x4fd607a7
#define H_IoSetIoPriorityHintIntoFileObject 0x353662f6
#define H_IoSetIoPriorityHintIntoThread 0x953127b5
#define H_IoSetIrpExtraCreateParameter 0xbc2eea9f
#define H_IoSetMasterIrpStatus 0xe3038119
#define H_IoSetPartitionInformation 0x00e3bee3
#define H_IoSetPartitionInformationEx 0xefb8e2c0
#define H_IoSetShareAccess 0xc3c2e8b7
#define H_IoSetShareAccessEx 0xba2dd208
#define H_IoSetStartIoAttributes 0x3500d1a3
#define H_IoSetSystemPartition 0x6882c54a
#define H_IoSetThreadHardErrorMode 0xc23e9029
#define H_IoSetTopLevelIrp 0x3355e84f
#define H_IoSizeofGenericIrpExtension 0xee057cb1
#define H_IoSizeofWorkItem 0xc10d47b2
#define H_IoStartNextPacket 0xdfdca21a
#define H_IoStartNextPacketByKey 0xd1d7e22b
#define H_IoStartPacket 0x7902d488
#define H_IoStartTimer 0xb9f182be
#define H_IoStatisticsLock 0x553ef87e
#define H_IoStopTimer 0x9ed2df00
#define H_IoSynchronousCallDriver 0x2c8f66b1
#define H_IoSynchronousInvalidateDeviceRelations 0x8dabb8b9
#define H_IoSynchronousPageWrite 0xa5593384
#define H_IoTestDependency 0x8f04c813
#define H_IoThreadToProcess 0xefc35d44
#define H_IoTransferActivityId 0x0969b572
#define H_IoTranslateBusAddress 0x64c5a78e
#define H_IoTryQueueWorkItem 0xb5639dca
#define H_IoUninitializeWorkItem 0x242f3e6e
#define H_IoUnregisterBootDriverCallback 0xa4b97364
#define H_IoUnregisterContainerNotification 0x132861c6
#define H_IoUnregisterFileSystem 0xd3c32dcd
#define H_IoUnregisterFsRegistrationChange 0x8a2df1f3
#define H_IoUnregisterIoTracking 0xf6b435d1
#define H_IoUnregisterPlugPlayNotification 0x5bcbc7ab
#define H_IoUnregisterPlugPlayNotificationEx 0xf1eaf40a
#define H_IoUnregisterPriorityCallback 0x65077dad
#define H_IoUnregisterShutdownNotification 0xeaa8afc6
#define H_IoUpdateShareAccess 0x2505ba88
#define H_IoValidateDeviceIoControlAccess 0x6f472e72
#define H_IoVerifyPartitionTable 0x45d2631b
#define H_IoVerifyVolume 0xdaaa4928
#define H_IoVolumeDeviceToDosName 0xdffafb81
#define H_IoVolumeDeviceToGuid 0x140dabd5
#define H_IoVolumeDeviceToGuidPath 0x5b58a0d5
#define H_IoWMIAllocateInstanceIds 0x88c7415e
#define H_IoWMIDeviceObjectToInstanceName 0xaadc70fc
#define H_IoWMIDeviceObjectToProviderId 0x249b60b8
#define H_IoWMIExecuteMethod 0xc3c317cd
#define H_IoWMIHandleToInstanceName 0x22eaff13
#define H_IoWMIOpenBlock 0xac8095f9
#define H_IoWMIQueryAllData 0x56b89f2b
#define H_IoWMIQueryAllDataMultiple 0x66f1599d
#define H_IoWMIQuerySingleInstance 0x736cefcd
#define H_IoWMIQuerySingleInstanceMultiple 0x80d48ded
#define H_IoWMIRegistrationControl 0xaa368fce
#define H_IoWMISetNotificationCallback 0xb26c251c
#define H_IoWMISetSingleInstance 0xd1a8e2ad
#define H_IoWMISetSingleItem 0xd933bf94
#define H_IoWMISuggestInstanceName 0x7609e072
#define H_IoWMIWriteEvent 0xd1731714
#define H_IoWithinStackLimits 0xcd37d32e
#define H_IoWriteErrorLogEntry 0x68e8001a
#define H_IoWriteOperationCount 0x4f4f00d3
#define H_IoWritePartitionTable 0xc7982047
#define H_IoWritePartitionTableEx 0x0811d31e
#define H_IoWriteTransferCount 0x888c627f
#define H_IofCallDriver 0xc879f974
#define H_IofCompleteRequest 0xb043e4e6
#define H_KdAcquireDebuggerLock 0xbbf9a082
#define H_KdChangeOption 0xbe1d9ba3
#define H_KdDebuggerEnabled 0xd7db5572
#define H_KdDebuggerNotPresent 0x2d8944eb
#define H_KdDeregisterPowerHandler 0xc79bb31a
#define H_KdDisableDebugger 0x76079440
#define H_KdEnableDebugger 0x179ccac1
#define H_KdEnteredDebugger 0xcf771040
#define H_KdLogDbgPrint 0xed6f6f7e
#define H_KdPollBreakIn 0xb45f7b4f
#define H_KdPowerTransition 0x936eb999
#define H_KdRefreshDebuggerNotPresent 0xa4ed4123
#define H_KdRegisterPowerHandler 0x57e3a78d
#define H_KdReleaseDebuggerLock 0x70e480a6
#define H_KdSystemDebugControl 0x9c72cf49
#define H_KeAcquireGuardedMutex 0xcbd90fbb
#define H_KeAcquireGuardedMutexUnsafe 0x8a469ee4
#define H_KeAcquireInStackQueuedSpinLock 0x8bd77e26
#define H_KeAcquireInStackQueuedSpinLockAtDpcLevel 0xcdebf872
#define H_KeAcquireInStackQueuedSpinLockForDpc 0xa3a9a07a
#define H_KeAcquireInStackQueuedSpinLockRaiseToSynch 0x2e920796
#define H_KeAcquireInterruptSpinLock 0x8f710ab1
#define H_KeAcquireQueuedSpinLock 0xd7a5763c
#define H_KeAcquireQueuedSpinLockRaiseToSynch 0xaf37c0b6
#define H_KeAcquireSpinLockAtDpcLevel 0xe2648734
#define H_KeAcquireSpinLockForDpc 0x5b5e5418
#define H_KeAcquireSpinLockRaiseToDpc 0xf6720183
#define H_KeAcquireSpinLockRaiseToSynch 0xf1438c75
#define H_KeAddGroupAffinityEx 0x8e49b9c8
#define H_KeAddProcessorAffinityEx 0x480fb2ba
#define H_KeAddProcessorGroupAffinity 0x8ba1b13b
#define H_KeAddSystemServiceTable 0x12e4bb7a
#define H_KeAlertThread 0xef001234
#define H_KeAllocateCalloutStack 0xc02e4184
#define H_KeAllocateCalloutStackEx 0x906112f3
#define H_KeAndAffinityEx 0x544e1d81
#define H_KeAndGroupAffinityEx 0x8e43b9c8
#define H_KeAreAllApcsDisabled 0xe9e86b35
#define H_KeAreApcsDisabled 0x79c1624c
#define H_KeAttachProcess 0x33b6a67a
#define H_KeBugCheck 0xd89ca6f2
#define H_KeBugCheckEx 0x29bc94df
#define H_KeCancelTimer 0xa7405567
#define H_KeCapturePersistentThreadState 0x6803a9a8
#define H_KeCheckProcessorAffinityEx 0x7d0a264a
#define H_KeCheckProcessorGroupAffinity 0x15a71189
#define H_KeClearEvent 0xb21b8981
#define H_KeClockInterruptNotify 0xc98a92ea
#define H_KeClockTimerPowerChange 0xcb69b9f1
#define H_KeComplementAffinityEx 0xe2a09942
#define H_KeCopyAffinityEx 0x66ad4c78
#define H_KeCountSetBitsAffinityEx 0x42806c90
#define H_KeCountSetBitsGroupAffinity 0x4ee0e0c0
#define H_KeDelayExecutionThread 0x55e51cb6
#define H_KeDeregisterBugCheckCallback 0xc74c91ba
#define H_KeDeregisterBugCheckReasonCallback 0xf91f1656
#define H_KeDeregisterNmiCallback 0x1e4a3a49
#define H_KeDeregisterProcessorChangeCallback 0xfa1557d1
#define H_KeDetachProcess 0x33e4867a
#define H_KeDispatchSecondaryInterrupt 0x024475cc
#define H_KeEnterCriticalRegion 0xd7648f37
#define H_KeEnterGuardedRegion 0xcea1bdef
#define H_KeEnterKernelDebugger 0x995e492c
#define H_KeEnumerateNextProcessor 0xc6a9db29
#define H_KeExpandKernelStackAndCallout 0x38739e96
#define H_KeExpandKernelStackAndCalloutEx 0xe7a5ace4
#define H_KeFindConfigurationEntry 0x50a97a68
#define H_KeFindConfigurationNextEntry 0x43c19eec
#define H_KeFindFirstSetLeftAffinityEx 0x0f0924f4
#define H_KeFindFirstSetLeftGroupAffinity 0x426951e9
#define H_KeFindFirstSetRightAffinityEx 0xb530f5ed
#define H_KeFindFirstSetRightGroupAffinity 0x615e16d3
#define H_KeFirstGroupAffinityEx 0x9580c86b
#define H_KeFlushEntireTb 0x5abf4d92
#define H_KeFlushQueuedDpcs 0xda4257ee
#define H_KeForceEnableNx 0xe47f21e8
#define H_KeFreeCalloutStack 0x59fa53f3
#define H_KeGenericCallDpc 0x9a0b7982
#define H_KeGetClockOwner 0x3e5881a0
#define H_KeGetClockTimerResolution 0x5d6da0c3
#define H_KeGetCurrentIrql 0x858728f7
#define H_KeGetCurrentNodeNumber 0xdc314a90
#define H_KeGetCurrentProcessorNumberEx 0x1c4a305e
#define H_KeGetCurrentThread 0x6a5f916c
#define H_KeGetNextClockTickDuration 0x466bea87
#define H_KeGetProcessorIndexFromNumber 0xda489320
#define H_KeGetProcessorNumberFromIndex 0xf683fd77
#define H_KeGetRecommendedSharedDataAlignment 0xf75ddd29
#define H_KeGetXSaveFeatureFlags 0x572d9ed1
#define H_KeHwPolicyLocateResource 0x9dcbe36b
#define H_KeInitializeAffinityEx 0xa8661652
#define H_KeInitializeApc 0xe9ee1079
#define H_KeInitializeCrashDumpHeader 0x3e52147f
#define H_KeInitializeDeviceQueue 0xf2831a92
#define H_KeInitializeDpc 0xe9ef5079
#define H_KeInitializeEnumerationContext 0xf056905a
#define H_KeInitializeEnumerationContextFromGroup 0x40950db1
#define H_KeInitializeEvent 0xc4dfcd0f
#define H_KeInitializeGuardedMutex 0x58836ca1
#define H_KeInitializeInterrupt 0x9e20c0fc
#define H_KeInitializeMutant 0x5dc54ad6
#define H_KeInitializeMutex 0x44bb8883
#define H_KeInitializeQueue 0x84bfc09f
#define H_KeInitializeSecondaryInterruptServices 0xe6db9c01
#define H_KeInitializeSemaphore 0xa77025cd
#define H_KeInitializeSpinLock 0xa6cf7272
#define H_KeInitializeThreadedDpc 0x34b35419
#define H_KeInitializeTimer 0xd73dc888
#define H_KeInitializeTimerEx 0x72221737
#define H_KeInsertByKeyDeviceQueue 0x590faff2
#define H_KeInsertDeviceQueue 0x358db927
#define H_KeInsertHeadQueue 0x21058a5d
#define H_KeInsertQueue 0x69ce0337
#define H_KeInsertQueueApc 0x66fd41a3
#define H_KeInsertQueueDpc 0x66fc01a3
#define H_KeInterlockedClearProcessorAffinityEx 0x60a415e6
#define H_KeInterlockedSetProcessorAffinityEx 0x0aaf9602
#define H_KeInvalidateAllCaches 0xe49dc2de
#define H_KeInvalidateRangeAllCaches 0x824c4880
#define H_KeIpiGenericCall 0x99be1b46
#define H_KeIsAttachedProcess 0xcc35e584
#define H_KeIsEmptyAffinityEx 0xe594bceb
#define H_KeIsEqualAffinityEx 0x05c619ab
#define H_KeIsExecutingDpc 0x4022c289
#define H_KeIsSingleGroupAffinityEx 0x7adcc441
#define H_KeIsSubsetAffinityEx 0x0b9cd600
#define H_KeIsWaitListEmpty 0x8c580480
#define H_KeLastBranchMSR 0x533ad371
#define H_KeLeaveCriticalRegion 0x92f1a969
#define H_KeLeaveGuardedRegion 0x722a97a3
#define H_KeLoadMTRR 0xac2c9679
#define H_KeLoaderBlock 0xc8eed548
#define H_KeLowerIrql 0xe5e37531
#define H_KeNumberProcessors 0x076d5723
#define H_KeOrAffinityEx 0x260a392c
#define H_KeProcessorGroupAffinity 0x9d537065
#define H_KeProfileInterruptWithSource 0x15597bff
#define H_KePulseEvent 0x2b09c130
#define H_KeQueryActiveGroupCount 0x3fe8e4a4
#define H_KeQueryActiveProcessorAffinity 0xf9b89993
#define H_KeQueryActiveProcessorCount 0x363b9457
#define H_KeQueryActiveProcessorCountEx 0xe515ef76
#define H_KeQueryActiveProcessors 0xbc6e3203
#define H_KeQueryDpcWatchdogInformation 0xfc106478
#define H_KeQueryEffectivePriorityThread 0xb0b2803c
#define H_KeQueryGroupAffinity 0xc9d27807
#define H_KeQueryGroupAffinityEx 0x9e01d08c
#define H_KeQueryHardwareCounterConfiguration 0x6deceb80
#define H_KeQueryHighestNodeNumber 0x5e68d616
#define H_KeQueryInterruptTimePrecise 0x21bf025c
#define H_KeQueryLogicalProcessorRelationship 0xf64fafe3
#define H_KeQueryMaximumGroupCount 0xabbe845b
#define H_KeQueryMaximumProcessorCount 0xcf7ef258
#define H_KeQueryMaximumProcessorCountEx 0xbc961127
#define H_KeQueryNodeActiveAffinity 0xd6a6a737
#define H_KeQueryNodeMaximumProcessorCount 0xc5dc4619
#define H_KeQueryPrcbAddress 0x2756e93b
#define H_KeQueryPriorityThread 0x1a7cf4e9
#define H_KeQueryRuntimeThread 0x42599f4a
#define H_KeQuerySystemTimePrecise 0x3db020a3
#define H_KeQueryTimeIncrement 0x31dc0122
#define H_KeQueryTotalCycleTimeThread 0x56bbed57
#define H_KeQueryUnbiasedInterruptTime 0x9aa633dd
#define H_KeRaiseIrqlToDpcLevel 0x5cf711f8
#define H_KeRaiseUserException 0x0f545b0e
#define H_KeReadStateEvent 0x3c460ae7
#define H_KeReadStateMutant 0x1126beaa
#define H_KeReadStateMutex 0xbc224f6b
#define H_KeReadStateQueue 0x7c260777
#define H_KeReadStateSemaphore 0x28f9b9b3
#define H_KeReadStateTimer 0x2fa40f60
#define H_KeRegisterBugCheckCallback 0xbe0b11f3
#define H_KeRegisterBugCheckReasonCallback 0xe71e33b3
#define H_KeRegisterNmiCallback 0x3162ca40
#define H_KeRegisterProcessorChangeCallback 0xfa87a55e
#define H_KeReleaseGuardedMutex 0x00c42f9f
#define H_KeReleaseGuardedMutexUnsafe 0xfec60dc8
#define H_KeReleaseInStackQueuedSpinLock 0xee59ee34
#define H_KeReleaseInStackQueuedSpinLockForDpc 0x99e9e9ec
#define H_KeReleaseInStackQueuedSpinLockFromDpcLevel 0xc27248d9
#define H_KeReleaseInterruptSpinLock 0xd7980b97
#define H_KeReleaseMutant 0x92535b06
#define H_KeReleaseMutex 0xe524a4a0
#define H_KeReleaseQueuedSpinLock 0x9fac44fb
#define H_KeReleaseSemaphore 0x9d69d70f
#define H_KeReleaseSpinLock 0x22bb4197
#define H_KeReleaseSpinLockForDpc 0x135766df
#define H_KeReleaseSpinLockFromDpcLevel 0x384e86f3
#define H_KeRemoveByKeyDeviceQueue 0xdbc61c30
#define H_KeRemoveByKeyDeviceQueueIfBusy 0x702db8d8
#define H_KeRemoveDeviceQueue 0x65d48f5f
#define H_KeRemoveEntryDeviceQueue 0x22b6126d
#define H_KeRemoveGroupAffinityEx 0xb6769e0b
#define H_KeRemoveProcessorAffinityEx 0x7b8c40c6
#define H_KeRemoveProcessorGroupAffinity 0xc427c145
#define H_KeRemoveQueue 0xf7da157a
#define H_KeRemoveQueueDpc 0xaf4fc321
#define H_KeRemoveQueueDpcEx 0xf0c8492b
#define H_KeRemoveQueueEx 0x855e9f0e
#define H_KeRemoveSystemServiceTable 0xea83bc9e
#define H_KeResetEvent 0x3b3799b9
#define H_KeRestoreExtendedProcessorState 0x16cc3ac0
#define H_KeRestoreFloatingPointState 0x49eeeb3d
#define H_KeRevertToUserAffinityThread 0xedd1a880
#define H_KeRevertToUserAffinityThreadEx 0x6a20198c
#define H_KeRevertToUserGroupAffinityThread 0x30dc825e
#define H_KeRundownQueue 0x243cf8e5
#define H_KeSaveExtendedProcessorState 0xd27099ed
#define H_KeSaveFloatingPointState 0x0224d9e1
#define H_KeSaveStateForHibernate 0x5eadff08
#define H_KeSetActualBasePriorityThread 0x7aa9e629
#define H_KeSetAffinityThread 0x1b0f87f5
#define H_KeSetBasePriorityThread 0x6ed17691
#define H_KeSetCoalescableTimer 0x50c24625
#define H_KeSetDmaIoCoherency 0x3b6beb61
#define H_KeSetEvent 0xbb7ee0f5
#define H_KeSetEventBoostPriority 0xa30dbdc9
#define H_KeSetHardwareCounterConfiguration 0x4d26a1ca
#define H_KeSetIdealProcessorThread 0x96b4188a
#define H_KeSetImportanceDpc 0x4f20a4b5
#define H_KeSetKernelStackSwapEnable 0x9d038ef6
#define H_KeSetPriorityThread 0x924e667b
#define H_KeSetProfileIrql 0xa8af1c25
#define H_KeSetSystemAffinityThread 0xdf078636
#define H_KeSetSystemAffinityThreadEx 0xe18d9539
#define H_KeSetSystemGroupAffinityThread 0xa66df7ef
#define H_KeSetTargetProcessorDpc 0x379c3269
#define H_KeSetTargetProcessorDpcEx 0x0c9a6f1f
#define H_KeSetTimer 0xa89ce572
#define H_KeSetTimerEx 0x395c88df
#define H_KeSignalCallDpcDone 0x508ca79e
#define H_KeSignalCallDpcSynchronize 0xb2eb7a21
#define H_KeStackAttachProcess 0x2a426da3
#define H_KeStallWhileFrozen 0x6f0f16fe
#define H_KeStartDynamicProcessor 0xaedd45e3
#define H_KeSubtractAffinityEx 0x87ba7404
#define H_KeSweepLocalCaches 0xf4023405
#define H_KeSynchronizeExecution 0x90bb55f6
#define H_KeTestAlertThread 0xc4c9f967
#define H_KeTestSpinLock 0x4366abde
#define H_KeTryToAcquireGuardedMutex 0x00351b52
#define H_KeTryToAcquireQueuedSpinLock 0xd29f04c7
#define H_KeTryToAcquireQueuedSpinLockRaiseToSynch 0x80879311
#define H_KeTryToAcquireSpinLockAtDpcLevel 0x5237201b
#define H_KeUnstackDetachProcess 0xf84e1e61
#define H_KeUserModeCallback 0xa313e6b0
#define H_KeWaitForMultipleObjects 0xdb6ca780
#define H_KeWaitForMutexObject 0x848d1a5b
#define H_KeWaitForSingleObject 0xc547c853
#define H_KeWriteProtectPAT 0xda9ea1cd
#define H_KfRaiseIrql 0x0bff0d81
#define H_KiBugCheckData 0x25d1b00e
#define H_KiCheckForKernelApcDelivery 0xf7accea6
#define H_KitLogFeatureUsage 0xf102888b
#define H_KseQueryDeviceData 0xb6d8485a
#define H_KseQueryDeviceDataList 0xa2f7fd71
#define H_KseQueryDeviceFlags 0x4d816e28
#define H_KseRegisterShim 0x5dc7bf9e
#define H_KseRegisterShimEx 0xefe7b589
#define H_KseSetDeviceFlags 0x055e8a7b
#define H_KseUnregisterShim 0x6d385927
#define H_LdrAccessResource 0x036996c8
#define H_LdrEnumResources 0x18ce7969
#define H_LdrFindResourceDirectory_U 0x333206e1
#define H_LdrFindResourceEx_U 0xf36d3006
#define H_LdrFindResource_U 0x7f4fc099
#define H_LdrResFindResource 0x93451947
#define H_LdrResFindResourceDirectory 0xb3eabef4
#define H_LdrResSearchResource 0xb117d89d
#define H_LpcPortObjectType 0x2ef953d3
#define H_LpcReplyWaitReplyPort 0xeccd2fc2
#define H_LpcRequestPort 0xdcfc1a5b
#define H_LpcRequestWaitReplyPort 0x26dff1fe
#define H_LpcRequestWaitReplyPortEx 0xfc7fab4f
#define H_LpcSendWaitReceivePort 0x47c70f5b
#define H_LsaCallAuthenticationPackage 0x87680d7b
#define H_LsaDeregisterLogonProcess 0xc94254fb
#define H_LsaFreeReturnBuffer 0x3e87dc47
#define H_LsaLogonUser 0x266af17c
#define H_LsaLookupAuthenticationPackage 0x4c4d465a
#define H_LsaRegisterLogonProcess 0x7cfa5c1d
#define H_Mm64BitPhysicalAddress 0xcb193b2a
#define H_MmAddPhysicalMemory 0x4e36be62
#define H_MmAddVerifierThunks 0x27efeca7
#define H_MmAdjustWorkingSetSize 0x2d505755
#define H_MmAdvanceMdl 0xd8574e7a
#define H_MmAllocateContiguousMemory 0x0c3898a9
#define H_MmAllocateContiguousMemorySpecifyCache 0x5571d012
#define H_MmAllocateContiguousMemorySpecifyCacheNode 0x2c8cef64
#define H_MmAllocateContiguousNodeMemory 0xd76a590b
#define H_MmAllocateMappingAddress 0x89674677
#define H_MmAllocateMdlForIoSpace 0x7dae13f2
#define H_MmAllocateNodePagesForMdlEx 0x90dc86e9
#define H_MmAllocateNonCachedMemory 0x3c0a587e
#define H_MmAllocatePagesForMdl 0x60bc0a55
#define H_MmAllocatePagesForMdlEx 0x02957ad7
#define H_MmAreMdlPagesCached 0xe33d8d4f
#define H_MmBadPointer 0xfc1dcaed
#define H_MmBuildMdlForNonPagedPool 0x48bbba6c
#define H_MmCanFileBeTruncated 0xc7f58180
#define H_MmCommitSessionMappedView 0x408b11d1
#define H_MmCopyMemory 0xb253a476
#define H_MmCopyVirtualMemory 0x93d6a87d
#define H_MmCreateMdl 0x8f56f615
#define H_MmCreateMirror 0x116d5d2c
#define H_MmCreateSection 0xa66f6087
#define H_MmDisableModifiedWriteOfSection 0x96babc6e
#define H_MmDoesFileHaveUserWritableReferences 0x5074267c
#define H_MmFlushImageSection 0x311eda74
#define H_MmForceSectionClosed 0xea701c34
#define H_MmFreeContiguousMemory 0x7596518e
#define H_MmFreeContiguousMemorySpecifyCache 0xc7064afe
#define H_MmFreeMappingAddress 0xadfaa0cc
#define H_MmFreeNonCachedMemory 0x72f905ec
#define H_MmFreePagesFromMdl 0x10e3fc42
#define H_MmGetCacheAttribute 0x356f1800
#define H_MmGetMaximumFileSectionSize 0xf0b26d23
#define H_MmGetPhysicalAddress 0x8bfbe45c
#define H_MmGetPhysicalMemoryRanges 0xdf29795d
#define H_MmGetSystemRoutineAddress 0x50b70ff2
#define H_MmGetVirtualForPhysical 0x7d815bfb
#define H_MmGrowKernelStack 0x5af6cf63
#define H_MmHighestUserAddress 0xe0eb2db4
#define H_MmIsAddressValid 0x51c859fd
#define H_MmIsDriverSuspectForVerifier 0x99970c2c
#define H_MmIsDriverVerifying 0xd9a68250
#define H_MmIsDriverVerifyingByAddress 0x60793ffd
#define H_MmIsIoSpaceActive 0xc38dc2b4
#define H_MmIsNonPagedSystemAddressValid 0x663a5825
#define H_MmIsRecursiveIoFault 0xe15ba1d0
#define H_MmIsThisAnNtAsSystem 0xc13567cd
#define H_MmIsVerifierEnabled 0xa05afb6e
#define H_MmLockPagableDataSection 0x8292ba96
#define H_MmLockPagableImageSection 0xbc91b094
#define H_MmLockPagableSectionByHandle 0xb9eef22d
#define H_MmMapIoSpace 0xdff0edfe
#define H_MmMapLockedPages 0x81f0ed57
#define H_MmMapLockedPagesSpecifyCache 0x0a990c95
#define H_MmMapLockedPagesWithReservedMapping 0x8e06877d
#define H_MmMapMemoryDumpMdl 0x8ed14132
#define H_MmMapUserAddressesToPage 0xe0161fb3
#define H_MmMapVideoDisplay 0x548322f3
#define H_MmMapViewInSessionSpace 0x9f24a00a
#define H_MmMapViewInSessionSpaceEx 0x28028531
#define H_MmMapViewInSystemSpace 0x0b54385b
#define H_MmMapViewInSystemSpaceEx 0x0e16e02d
#define H_MmMapViewOfSection 0x52d79a3c
#define H_MmMarkPhysicalMemoryAsBad 0xeec46f52
#define H_MmMarkPhysicalMemoryAsGood 0x62942c93
#define H_MmMdlPageContentsState 0x43464864
#define H_MmMdlPagesAreZero 0x9839bdbd
#define H_MmPageEntireDriver 0x5c6d3ee8
#define H_MmPrefetchPages 0x04de25f2
#define H_MmPrefetchVirtualAddresses 0x721bf5a8
#define H_MmProbeAndLockPages 0x206ea3b0
#define H_MmProbeAndLockProcessPages 0x4d747977
#define H_MmProbeAndLockSelectedPages 0xacc089b5
#define H_MmProtectMdlSystemAddress 0x7cdf6afe
#define H_MmQuerySystemSize 0x8d8e2b65
#define H_MmRemovePhysicalMemory 0x4c76402c
#define H_MmResetDriverPaging 0xb3abea89
#define H_MmRotatePhysicalView 0xf63c07f9
#define H_MmSectionObjectType 0x7805df41
#define H_MmSecureVirtualMemory 0xf7f241d2
#define H_MmSetAddressRangeModified 0x9930385c
#define H_MmSetBankedSection 0x5992a4e2
#define H_MmSizeOfMdl 0x2f008a36
#define H_MmSystemRangeStart 0xe6e4fad7
#define H_MmTrimAllSystemPagableMemory 0x23f9003e
#define H_MmUnlockPagableImageSection 0x2fe72892
#define H_MmUnlockPages 0x643bc8d9
#define H_MmUnmapIoSpace 0xb123ed2c
#define H_MmUnmapLockedPages 0xa71ddd5a
#define H_MmUnmapReservedMapping 0xf060c723
#define H_MmUnmapVideoDisplay 0x221b2460
#define H_MmUnmapViewInSessionSpace 0xff3eedd0
#define H_MmUnmapViewInSystemSpace 0xbf940cc0
#define H_MmUnmapViewOfSection 0x1ed4d387
#define H_MmUnsecureVirtualMemory 0xc09bc1bb
#define H_MmUserProbeAddress 0xa555c9ad
#define H_NlsAnsiCodePage 0xd4a7f641
#define H_NlsLeadByteInfo 0xda87cc82
#define H_NlsMbCodePageTag 0x04a2bead
#define H_NlsMbOemCodePageTag 0x50505887
#define H_NlsOemCodePage 0xc151bf33
#define H_NlsOemLeadByteInfo 0xb40b2a86
#define H_NtAddAtom 0x06d430cb
#define H_NtAdjustPrivilegesToken 0x3ffc4add
#define H_NtAllocateLocallyUniqueId 0xeebcacbf
#define H_NtAllocateUuids 0x19643b52
#define H_NtAllocateVirtualMemory 0x0952a9e4
#define H_NtBuildGUID 0x0a1ed018
#define H_NtBuildLab 0xb816e78b
#define H_NtBuildNumber 0xb08fb03f
#define H_NtClose 0x3d9ac241
#define H_NtCommitComplete 0xe7e3e3ac
#define H_NtCommitEnlistment 0xeac96a76
#define H_NtCommitTransaction 0x8b9af27d
#define H_NtConnectPort 0xa028fe9c
#define H_NtCreateEnlistment 0xaa0ace42
#define H_NtCreateEvent 0x3ff434a1
#define H_NtCreateFile 0xaa182ce2
#define H_NtCreateResourceManager 0xeb9fcd96
#define H_NtCreateSection 0x6e6f608b
#define H_NtCreateTransaction 0xea48e85d
#define H_NtCreateTransactionManager 0x3c83116d
#define H_NtDeleteAtom 0x3ded7d69
#define H_NtDeleteFile 0x3d0a3ce1
#define H_NtDeviceIoControlFile 0x385c70f2
#define H_NtDuplicateObject 0x63f394e8
#define H_NtDuplicateToken 0x8967a725
#define H_NtEnumerateTransactionObject 0x6b63d428
#define H_NtFindAtom 0x3cb090bc
#define H_NtFreeVirtualMemory 0xbf539229
#define H_NtFreezeTransactions 0x56f42e96
#define H_NtFsControlFile 0x825dccd7
#define H_NtGetEnvironmentVariableEx 0x0150ee34
#define H_NtGetNotificationResourceManager 0x010b137e
#define H_NtGlobalFlag 0x240d66ca
#define H_NtLockFile 0xcc43c95c
#define H_NtMakePermanentObject 0x17463e2d
#define H_NtMapViewOfSection 0x534e9a3c
#define H_NtNotifyChangeDirectoryFile 0x63eb0930
#define H_NtOpenEnlistment 0xdc6cf69b
#define H_NtOpenFile 0x9c45b56c
#define H_NtOpenProcess 0x9d4aa99d
#define H_NtOpenProcessToken 0xa7afbe07
#define H_NtOpenProcessTokenEx 0xef81cb13
#define H_NtOpenResourceManager 0x58ae0b5d
#define H_NtOpenThread 0x7e9b1765
#define H_NtOpenThreadToken 0xb92249c0
#define H_NtOpenThreadTokenEx 0x92700cb0
#define H_NtOpenTransaction 0xd95484e6
#define H_NtOpenTransactionManager 0xe5f57755
#define H_NtPrePrepareComplete 0xd6321fed
#define H_NtPrePrepareEnlistment 0x95d92602
#define H_NtPrepareComplete 0xfe0e3430
#define H_NtPrepareEnlistment 0x9f2e6c0d
#define H_NtPropagationComplete 0xe46b506e
#define H_NtPropagationFailed 0x48a89504
#define H_NtQueryDirectoryFile 0x9f8b3756
#define H_NtQueryEaFile 0xf9d10431
#define H_NtQueryEnvironmentVariableInfoEx 0xf4d8946e
#define H_NtQueryInformationAtom 0x0f3cd53f
#define H_NtQueryInformationEnlistment 0xa4eb98d5
#define H_NtQueryInformationFile 0x0fdb94b7
#define H_NtQueryInformationProcess 0xa638da59
#define H_NtQueryInformationResourceManager 0x9c95792e
#define H_NtQueryInformationThread 0xf6edf382
#define H_NtQueryInformationToken 0xcd0b9be8
#define H_NtQueryInformationTransaction 0x9ae3a3da
#define H_NtQueryInformationTransactionManager 0xab8df03b
#define H_NtQueryQuotaInformationFile 0x8c87255c
#define H_NtQuerySecurityAttributesToken 0x59f77700
#define H_NtQuerySecurityObject 0xa00de181
#define H_NtQuerySystemInformation 0xb044a119
#define H_NtQuerySystemInformationEx 0x28464ee9
#define H_NtQueryVolumeInformationFile 0x53567eb3
#define H_NtReadFile 0x3c7fe14c
#define H_NtReadOnlyEnlistment 0xf4e884c6
#define H_NtRecoverEnlistment 0x665ea7d5
#define H_NtRecoverResourceManager 0x893c8128
#define H_NtRecoverTransactionManager 0xab4f4504
#define H_NtRequestPort 0xbf1efa59
#define H_NtRequestWaitReplyPort 0x172e81ff
#define H_NtRollbackComplete 0xa7a638c4
#define H_NtRollbackEnlistment 0x9c137a67
#define H_NtRollbackTransaction 0xe692fac6
#define H_NtSetCachedSigningLevel 0x166088bc
#define H_NtSetEaFile 0x4b11b7f1
#define H_NtSetEvent 0x2a7ee0f7
#define H_NtSetInformationEnlistment 0x258c19b0
#define H_NtSetInformationFile 0x56bbcd57
#define H_NtSetInformationProcess 0x9a33f652
#define H_NtSetInformationResourceManager 0x97a97202
#define H_NtSetInformationThread 0xe095e5da
#define H_NtSetInformationToken 0x7d276bc4
#define H_NtSetInformationTransaction 0x2923111a
#define H_NtSetInformationVirtualMemory 0x0225aa34
#define H_NtSetQuotaInformationFile 0x4785ea5e
#define H_NtSetSecurityObject 0x60bf2132
#define H_NtSetVolumeInformationFile 0xd231ffd6
#define H_NtShutdownSystem 0x6f1ca892
#define H_NtThawTransactions 0x8bd2457f
#define H_NtTraceControl 0x7a1d2420
#define H_NtTraceEvent 0x2c132572
#define H_NtUnlockFile 0x560a1451
#define H_NtVdmControl 0x8a4e65b9
#define H_NtWaitForSingleObject 0xc5479a73
#define H_NtWriteFile 0x0f3fc279
#define H_ObAssignSecurity 0xbfc3089f
#define H_ObCheckCreateObjectAccess 0x11538b70
#define H_ObCheckObjectAccess 0xb64482cc
#define H_ObCloseHandle 0x76c2f0d5
#define H_ObCreateObject 0xb2989b3a
#define H_ObCreateObjectType 0xa1b7f1d6
#define H_ObDeleteCapturedInsertInfo 0xfe50c01f
#define H_ObDereferenceObject 0x1cb690e0
#define H_ObDereferenceObjectDeferDelete 0x694ea2e7
#define H_ObDereferenceObjectDeferDeleteWithTag 0x08dabd35
#define H_ObDereferenceSecurityDescriptor 0x7222ff2f
#define H_ObDuplicateObject 0x63f2b8e8
#define H_ObFindHandleForObject 0x518015ca
#define H_ObGetFilterVersion 0xc810986c
#define H_ObGetObjectSecurity 0x25977fd6
#define H_ObGetObjectType 0xbc375362
#define H_ObInsertObject 0xb6959831
#define H_ObIsDosDeviceLocallyMapped 0x22ffe317
#define H_ObIsKernelHandle 0x3c2b62d0
#define H_ObLogSecurityDescriptor 0xbb952c29
#define H_ObMakeTemporaryObject 0x128dbb73
#define H_ObOpenObjectByName 0xf514c384
#define H_ObOpenObjectByPointer 0x31e3e81a
#define H_ObOpenObjectByPointerWithTag 0x9d200c6f
#define H_ObQueryNameInfo 0x35b88c47
#define H_ObQueryNameString 0x81907ad8
#define H_ObQueryObjectAuditingByHandle 0x9f3205c3
#define H_ObReferenceObjectByHandle 0x6eb16dbd
#define H_ObReferenceObjectByHandleWithTag 0x966eb2ca
#define H_ObReferenceObjectByName 0x6fa1bb44
#define H_ObReferenceObjectByPointer 0x29f0beb5
#define H_ObReferenceObjectByPointerWithTag 0x307e3c49
#define H_ObReferenceObjectSafe 0x34a3aa82
#define H_ObReferenceObjectSafeWithTag 0x181006ef
#define H_ObReferenceSecurityDescriptor 0x9b1def9f
#define H_ObRegisterCallbacks 0xd1971460
#define H_ObReleaseObjectSecurity 0xd9247f3b
#define H_ObSetHandleAttributes 0x5ee90d0e
#define H_ObSetSecurityDescriptorInfo 0xde82feb9
#define H_ObSetSecurityObjectByPointer 0xcaf1e115
#define H_ObUnRegisterCallbacks 0x5793e826
#define H_ObWaitForMultipleObjects 0xc76ca788
#define H_ObWaitForSingleObject 0xc54788b3
#define H_ObfDereferenceObject 0x5eb6990b
#define H_ObfDereferenceObjectWithTag 0x7f02d2c5
#define H_ObfReferenceObject 0xca109673
#define H_ObfReferenceObjectWithTag 0x61f3fb89
#define H_POGOBuffer 0x13568ccd
#define H_PcwAddInstance 0xae96fa0a
#define H_PcwCloseInstance 0x1595300c
#define H_PcwCreateInstance 0x36d475de
#define H_PcwRegister 0x69d8b1fd
#define H_PcwUnregister 0x7594d826
#define H_PfFileInfoNotify 0x26caf5f1
#define H_PfxFindPrefix 0x4068f85c
#define H_PfxInitialize 0xf645700f
#define H_PfxInsertPrefix 0xfed55c85
#define H_PfxRemovePrefix 0xf4de7a4a
#define H_PoCallDriver 0xccee1b34
#define H_PoCancelDeviceNotify 0x5874abe0
#define H_PoClearPowerRequest 0xc3b5f0a3
#define H_PoCreatePowerRequest 0xbcf4e8bc
#define H_PoDeletePowerRequest 0xbf63faac
#define H_PoDisableSleepStates 0xdd53a906
#define H_PoEndDeviceBusy 0x41ed0686
#define H_PoFxActivateComponent 0xcd2e37e2
#define H_PoFxCompleteDevicePowerNotRequired 0xc1d0dac2
#define H_PoFxCompleteIdleCondition 0x1271f10d
#define H_PoFxCompleteIdleState 0xcddba63c
#define H_PoFxIdleComponent 0xfaca84db
#define H_PoFxNotifySurprisePowerOn 0x0f7bb648
#define H_PoFxPowerControl 0x3cf302d7
#define H_PoFxPowerOnCrashdumpDevice 0x26fe858a
#define H_PoFxProcessorNotification 0xe25eb241
#define H_PoFxRegisterCoreDevice 0x05906c1b
#define H_PoFxRegisterCrashdumpDevice 0x18639733
#define H_PoFxRegisterDevice 0x1422cc58
#define H_PoFxRegisterPlugin 0x844149f3
#define H_PoFxRegisterPluginEx 0x527cc3e8
#define H_PoFxRegisterPrimaryDevice 0xc5075844
#define H_PoFxReportDevicePoweredOn 0xff3c5543
#define H_PoFxSetComponentLatency 0x7cd791ee
#define H_PoFxSetComponentResidency 0x5ab672ac
#define H_PoFxSetComponentWake 0x6260b466
#define H_PoFxSetDeviceIdleTimeout 0x3a3f5acb
#define H_PoFxStartDevicePowerManagement 0x6274b929
#define H_PoFxUnregisterDevice 0xdf35e01a
#define H_PoGetProcessorIdleAccounting 0x629697f9
#define H_PoGetSystemWake 0x5f631aa1
#define H_PoInitiateProcessorWake 0xdb04bf63
#define H_PoLatencySensitivityHint 0x171c47de
#define H_PoNotifyVSyncChange 0x0c47d795
#define H_PoQueryWatchdogTime 0xb900f662
#define H_PoQueueShutdownWorkItem 0xc146af9b
#define H_PoReenableSleepStates 0x8dda94e9
#define H_PoRegisterCoalescingCallback 0x997a6630
#define H_PoRegisterDeviceForIdleDetection 0x8ec98e00
#define H_PoRegisterDeviceNotify 0xc91a61fe
#define H_PoRegisterPowerSettingCallback 0xf52accb3
#define H_PoRegisterSystemState 0xd9cd67cf
#define H_PoRequestPowerIrp 0x85bda0fc
#define H_PoRequestShutdownEvent 0xe54884ec
#define H_PoSetDeviceBusyEx 0x18a1b2db
#define H_PoSetFixedWakeSource 0xb9027d50
#define H_PoSetHiberRange 0x38c29817
#define H_PoSetPowerRequest 0xd446f66f
#define H_PoSetPowerState 0x2c4b519b
#define H_PoSetSystemState 0x532fd84a
#define H_PoSetSystemWake 0x5e231aa1
#define H_PoSetUserPresent 0x25c20025
#define H_PoShutdownBugCheck 0x54bf9a8f
#define H_PoStartDeviceBusy 0x9bfe5c8a
#define H_PoStartNextPowerIrp 0x8a69f621
#define H_PoUnregisterCoalescingCallback 0x3928e0c0
#define H_PoUnregisterPowerSettingCallback 0x5496e4a7
#define H_PoUnregisterSystemState 0x9ab537e6
#define H_PoUserShutdownCancelled 0x823bfba2
#define H_PoUserShutdownInitiated 0x4983e71a
#define H_ProbeForRead 0xc898772c
#define H_ProbeForWrite 0x1ed99e01
#define H_PsAcquireProcessExitSynchronization 0xd25698bf
#define H_PsAssignImpersonationToken 0x454fb8c4
#define H_PsChargePoolQuota 0x882afbf9
#define H_PsChargeProcessNonPagedPoolQuota 0x67b5a45d
#define H_PsChargeProcessPagedPoolQuota 0xd6443108
#define H_PsChargeProcessPoolQuota 0xcda3d4ab
#define H_PsChargeProcessWakeCounter 0x527dc88c
#define H_PsCreateSystemThread 0x3d3a79a0
#define H_PsCreateSystemThreadEx 0x9e682db6
#define H_PsDereferenceImpersonationToken 0x3c26c142
#define H_PsDereferenceKernelStack 0x073aef13
#define H_PsDereferencePrimaryToken 0x1fbe9272
#define H_PsDisableImpersonation 0xbb868983
#define H_PsEnterPriorityRegion 0xc54f6f3a
#define H_PsEstablishWin32Callouts 0x85442b7a
#define H_PsGetContextThread 0x67e6d66f
#define H_PsGetCurrentProcess 0x3409ad11
#define H_PsGetCurrentProcessId 0x6b4469e6
#define H_PsGetCurrentProcessSessionId 0x5434bdf2
#define H_PsGetCurrentProcessWin32Process 0x1e058088
#define H_PsGetCurrentProcessWow64Process 0x9b098104
#define H_PsGetCurrentThread 0x67c9916c
#define H_PsGetCurrentThreadId 0x645b3d16
#define H_PsGetCurrentThreadPreviousMode 0xd9a42de1
#define H_PsGetCurrentThreadProcess 0xdea0f5f6
#define H_PsGetCurrentThreadProcessId 0x3d7d934c
#define H_PsGetCurrentThreadStackBase 0xd855364d
#define H_PsGetCurrentThreadStackLimit 0xcb9ca098
#define H_PsGetCurrentThreadTeb 0x2d99cbd0
#define H_PsGetCurrentThreadWin32Thread 0x5212234f
#define H_PsGetCurrentThreadWin32ThreadAndEnterCriticalRegion 0xe826979a
#define H_PsGetJobLock 0xcc78c676
#define H_PsGetJobSessionId 0xe129d794
#define H_PsGetJobUIRestrictionsClass 0x51d70ac4
#define H_PsGetProcessCommonJob 0xb2c013cf
#define H_PsGetProcessCreateTimeQuadPart 0xae75762a
#define H_PsGetProcessDebugPort 0x321d6199
#define H_PsGetProcessExitProcessCalled 0x1e3b9bbf
#define H_PsGetProcessExitStatus 0xd26749c8
#define H_PsGetProcessExitTime 0xcc080f8c
#define H_PsGetProcessId 0x89a1a282
#define H_PsGetProcessImageFileName 0x8804b8c4
#define H_PsGetProcessInheritedFromUniqueProcessId 0x44d8ecc3
#define H_PsGetProcessJob 0xd0d184a6
#define H_PsGetProcessPeb 0xd0d701a6
#define H_PsGetProcessPriorityClass 0xfa89217c
#define H_PsGetProcessProtection 0x2acc056e
#define H_PsGetProcessSectionBaseAddress 0x5cece017
#define H_PsGetProcessSecurityPort 0x0795be00
#define H_PsGetProcessSessionId 0xc2fd7839
#define H_PsGetProcessSessionIdEx 0x5e0e5247
#define H_PsGetProcessSignatureLevel 0xc0a2186a
#define H_PsGetProcessWin32Process 0xa77759b0
#define H_PsGetProcessWin32WindowStation 0x49875696
#define H_PsGetProcessWow64Process 0x227b583c
#define H_PsGetThreadExitStatus 0xd043769c
#define H_PsGetThreadFreezeCount 0x0ef9e4d3
#define H_PsGetThreadHardErrorsAreDisabled 0x0767c061
#define H_PsGetThreadId 0xad9ef680
#define H_PsGetThreadProcess 0x908ea940
#define H_PsGetThreadProcessId 0xaa5000c7
#define H_PsGetThreadSessionId 0x6af93047
#define H_PsGetThreadTeb 0xcf7c00b4
#define H_PsGetThreadWin32Thread 0x36f0c684
#define H_PsGetVersion 0xcb993022
#define H_PsImpersonateClient 0xba37414a
#define H_PsInitialSystemProcess 0x4aeeec23
#define H_PsIsCurrentThreadPrefetching 0x07ec7a05
#define H_PsIsDiskCountersEnabled 0xe68eb1df
#define H_PsIsProcessBeingDebugged 0xdef02ef6
#define H_PsIsProtectedProcess 0x020f2948
#define H_PsIsProtectedProcessLight 0xdd40be30
#define H_PsIsSystemProcess 0x04cde1b8
#define H_PsIsSystemThread 0x35a819f5
#define H_PsIsThreadImpersonating 0xbf0e21d0
#define H_PsIsThreadTerminating 0xbb8c9a9a
#define H_PsJobType 0x7a79531b
#define H_PsLeavePriorityRegion 0x80da4964
#define H_PsLookupProcessByProcessId 0x368339ec
#define H_PsLookupProcessThreadByCid 0xe5e1145e
#define H_PsLookupThreadByThreadId 0x875aeaab
#define H_PsProcessType 0x50579bc3
#define H_PsQueryProcessAttributesByToken 0x34049589
#define H_PsQueryProcessExceptionFlags 0x1007a9ab
#define H_PsQueryTotalCycleTimeProcess 0xe8b7b08a
#define H_PsReferenceImpersonationToken 0xf5655800
#define H_PsReferenceKernelStack 0xcb9b8bb2
#define H_PsReferencePrimaryToken 0x4f0cc294
#define H_PsReferenceProcessFilePointer 0x89c6d00b
#define H_PsReleaseProcessExitSynchronization 0xfe22182c
#define H_PsReleaseProcessWakeCounter 0x564d5b40
#define H_PsRemoveCreateThreadNotifyRoutine 0x2500ab74
#define H_PsRemoveLoadImageNotifyRoutine 0x7f7b965f
#define H_PsRestoreImpersonation 0xbb939138
#define H_PsResumeProcess 0x8c9e87dc
#define H_PsReturnPoolQuota 0x617a1ef8
#define H_PsReturnProcessNonPagedPoolQuota 0xc67fa78f
#define H_PsReturnProcessPagedPoolQuota 0x865aa406
#define H_PsRevertThreadToSelf 0xe1959972
#define H_PsRevertToSelf 0x18a6fdb9
#define H_PsSetContextThread 0x67e6fe6f
#define H_PsSetCreateProcessNotifyRoutine 0xa8a33f0a
#define H_PsSetCreateProcessNotifyRoutineEx 0xcfc288d0
#define H_PsSetCreateThreadNotifyRoutine 0x468182c3
#define H_PsSetCurrentThreadPrefetching 0x6ffa7801
#define H_PsSetLegoNotifyRoutine 0xbc4f30c8
#define H_PsSetLoadImageNotifyRoutine 0x76362d43
#define H_PsSetProcessPriorityByClass 0x6bbb97fb
#define H_PsSetProcessPriorityClass 0xaa89217c
#define H_PsSetProcessSecurityPort 0x0735be00
#define H_PsSetProcessWin32Process 0xa7d759b0
#define H_PsSetProcessWindowStation 0x14c4d2ce
#define H_PsSetThreadHardErrorsAreDisabled 0x07676061
#define H_PsSetThreadWin32Thread 0x36f0c404
#define H_PsSuspendProcess 0x8deee799
#define H_PsTerminateSystemThread 0x29bc2901
#define H_PsThreadType 0x2271f381
#define H_PsUILanguageComitted 0xe7ada662
#define H_PsUpdateDiskCounters 0x49b69f3f
#define H_PsWrapApcWow64Thread 0x9dcb1e6b
#define H_RtlAbsoluteToSelfRelativeSD 0x801c4f6b
#define H_RtlAddAccessAllowedAce 0xdee47ca4
#define H_RtlAddAccessAllowedAceEx 0x1f291541
#define H_RtlAddAce 0x1e79c3eb
#define H_RtlAddAtomToAtomTable 0x0bcd5ba3
#define H_RtlAddAtomToAtomTableEx 0x56e8e00b
#define H_RtlAddRange 0x40b8347a
#define H_RtlAddResourceAttributeAce 0x5edb123b
#define H_RtlAllocateHeap 0x9016cd2b
#define H_RtlAnsiCharToUnicodeChar 0x8357b65d
#define H_RtlAnsiStringToUnicodeSize 0xf3dcad62
#define H_RtlAnsiStringToUnicodeString 0xfa5b8b91
#define H_RtlAppendAsciizToString 0xc80ca623
#define H_RtlAppendStringToString 0xc57e8620
#define H_RtlAppendUnicodeStringToString 0xec0de045
#define H_RtlAppendUnicodeToString 0xbce565e4
#define H_RtlAreAllAccessesGranted 0x558e5670
#define H_RtlAreAnyAccessesGranted 0x558c7c70
#define H_RtlAreBitsClear 0x59829e97
#define H_RtlAreBitsSet 0xb991629b
#define H_RtlAssert 0x6c90cb7b
#define H_RtlAvlInsertNodeEx 0x86c15230
#define H_RtlAvlRemoveNode 0x581e3329
#define H_RtlCaptureContext 0x021e8782
#define H_RtlCaptureStackBackTrace 0xfac61f42
#define H_RtlCharToInteger 0x7c5b682f
#define H_RtlCheckPortableOperatingSystem 0x389454b0
#define H_RtlCheckRegistryKey 0x5cb0ce5b
#define H_RtlCheckTokenCapability 0x7b8259e6
#define H_RtlCheckTokenMembership 0xcc581b51
#define H_RtlCheckTokenMembershipEx 0x06d451ee
#define H_RtlClearAllBits 0x8f77076c
#define H_RtlClearBit 0x72d71360
#define H_RtlClearBits 0x6b89b04a
#define H_RtlCmDecodeMemIoResource 0xdc427f98
#define H_RtlCmEncodeMemIoResource 0x99c27f98
#define H_RtlCompareAltitudes 0x5a005aa5
#define H_RtlCompareMemory 0x586331bc
#define H_RtlCompareMemoryUlong 0x9e827a80
#define H_RtlCompareString 0x4b82bf53
#define H_RtlCompareUnicodeString 0x31983038
#define H_RtlCompareUnicodeStrings 0xcc181c6b
#define H_RtlCompressBuffer 0x2791296e
#define H_RtlCompressChunks 0xf5f32e66
#define H_RtlComputeCrc32 0x687b7023
#define H_RtlContractHashTable 0x9594943c
#define H_RtlConvertSidToUnicodeString 0x13a7ef52
#define H_RtlCopyBitMap 0x745c0dc7
#define H_RtlCopyLuid 0xf51adfd9
#define H_RtlCopyLuidAndAttributesArray 0x915303e5
#define H_RtlCopyMemory 0xb7748436
#define H_RtlCopyMemoryNonTemporal 0x73d3e2da
#define H_RtlCopyRangeList 0xef649b25
#define H_RtlCopySid 0x7bedfbb2
#define H_RtlCopySidAndAttributesArray 0x72eecaff
#define H_RtlCopyString 0xa4950ad9
#define H_RtlCopyUnicodeString 0x5a8dee17
#define H_RtlCrc32 0x3efd0ad6
#define H_RtlCrc64 0x3efd0850
#define H_RtlCreateAcl 0x0f5fbbd5
#define H_RtlCreateAtomTable 0x32f9ed35
#define H_RtlCreateAtomTableEx 0x7b4d6e46
#define H_RtlCreateHashTable 0x3bd39d1d
#define H_RtlCreateHashTableEx 0xe7476c0c
#define H_RtlCreateHeap 0xaefc6c77
#define H_RtlCreateRegistryKey 0xf1ee6626
#define H_RtlCreateSecurityDescriptor 0xdb1593cf
#define H_RtlCreateSystemVolumeInformationFolder 0x70659088
#define H_RtlCreateUnicodeString 0x773cb5ca
#define H_RtlCreateUserThread 0xe9e0a4f7
#define H_RtlCultureNameToLCID 0x0956caeb
#define H_RtlCustomCPToUnicodeN 0x0d7a34e2
#define H_RtlDecompressBuffer 0x9daf495f
#define H_RtlDecompressBufferEx 0xd257c593
#define H_RtlDecompressChunks 0x4fcd4e57
#define H_RtlDecompressFragment 0xc35a0c27
#define H_RtlDelete 0x0f70c843
#define H_RtlDeleteAce 0x08719ffc
#define H_RtlDeleteAtomFromAtomTable 0xc0790325
#define H_RtlDeleteElementGenericTable 0xc6e93be8
#define H_RtlDeleteElementGenericTableAvl 0x7d08a64b
#define H_RtlDeleteElementGenericTableAvlEx 0x2992fdba
#define H_RtlDeleteHashTable 0x83431d01
#define H_RtlDeleteNoSplay 0xae9c1565
#define H_RtlDeleteOwnersRanges 0xfc05369e
#define H_RtlDeleteRange 0x57bdf1f8
#define H_RtlDeleteRegistryValue 0x8285ce9e
#define H_RtlDescribeChunk 0xab82e565
#define H_RtlDestroyAtomTable 0xa8f3db77
#define H_RtlDestroyHeap 0xfdbd2abf
#define H_RtlDowncaseUnicodeChar 0x0be1aabd
#define H_RtlDowncaseUnicodeString 0xa8c9b51e
#define H_RtlDuplicateUnicodeString 0x0a0af4a8
#define H_RtlEmptyAtomTable 0xa35dfd86
#define H_RtlEndEnumerationHashTable 0xa604b57e
#define H_RtlEndWeakEnumerationHashTable 0xb720c245
#define H_RtlEnumerateEntryHashTable 0xc42a256a
#define H_RtlEnumerateGenericTable 0x2a0450f0
#define H_RtlEnumerateGenericTableAvl 0x1e153be6
#define H_RtlEnumerateGenericTableLikeADirectory 0x34c408f5
#define H_RtlEnumerateGenericTableWithoutSplaying 0xac714ef0
#define H_RtlEnumerateGenericTableWithoutSplayingAvl 0xde05f545
#define H_RtlEqualLuid 0x8c74fedc
#define H_RtlEqualSid 0x711f27f0
#define H_RtlEqualString 0x2cd45482
#define H_RtlEqualUnicodeString 0xe63afe95
#define H_RtlEqualWnfChangeStamps 0xe5db853e
#define H_RtlEthernetAddressToStringA 0xabc0f1dc
#define H_RtlEthernetAddressToStringW 0xabc0f1ca
#define H_RtlEthernetStringToAddressA 0xa0aa260f
#define H_RtlEthernetStringToAddressW 0xa0aa2619
#define H_RtlExpandHashTable 0xea839b85
#define H_RtlExtractBitMap 0x9b0b1a0c
#define H_RtlFillMemory 0x314cd034
#define H_RtlFindAceByType 0xbf347013
#define H_RtlFindClearBits 0x0fee731d
#define H_RtlFindClearBitsAndSet 0x555884c5
#define H_RtlFindClearRuns 0x0de97e1d
#define H_RtlFindClosestEncodableLength 0x32f3dd26
#define H_RtlFindFirstRunClear 0xdb1742d9
#define H_RtlFindLastBackwardRunClear 0x8dbaff6d
#define H_RtlFindLeastSignificantBit 0xbf59bfd6
#define H_RtlFindLongestRunClear 0x01a9da5d
#define H_RtlFindMessage 0x47fcd57c
#define H_RtlFindMostSignificantBit 0x0206e8ac
#define H_RtlFindNextForwardRunClear 0x8f8d2eae
#define H_RtlFindRange 0x6ae255f6
#define H_RtlFindSetBits 0x31dea4ea
#define H_RtlFindSetBitsAndClear 0xd72b1572
#define H_RtlFindUnicodePrefix 0xd06b21e8
#define H_RtlFormatCurrentUserKeyPath 0xdbca9545
#define H_RtlFormatMessage 0xbbfc1daf
#define H_RtlFreeAnsiString 0x97c3812a
#define H_RtlFreeHeap 0x3594af64
#define H_RtlFreeOemString 0xe5ff3d59
#define H_RtlFreeRangeList 0xef373e6b
#define H_RtlFreeUnicodeString 0xba88d443
#define H_RtlGUIDFromString 0x018e1969
#define H_RtlGenerate8dot3Name 0x987a3592
#define H_RtlGenerateClass5Guid 0x01c0d112
#define H_RtlGetAce 0x0c79c3db
#define H_RtlGetAppContainerNamedObjectPath 0x17c9b3da
#define H_RtlGetAppContainerParent 0x7de8b891
#define H_RtlGetAppContainerSidType 0x9279869a
#define H_RtlGetCallersAddress 0x94e42468
#define H_RtlGetCompressionWorkSpaceSize 0xab8ab8e2
#define H_RtlGetDaclSecurityDescriptor 0xcb91ba6b
#define H_RtlGetDefaultCodePage 0x2313363d
#define H_RtlGetElementGenericTable 0x9ace3fe0
#define H_RtlGetElementGenericTableAvl 0xfc0322ab
#define H_RtlGetEnabledExtendedFeatures 0xb2dd8bb1
#define H_RtlGetFirstRange 0x21a264b0
#define H_RtlGetGroupSecurityDescriptor 0xc034ffaa
#define H_RtlGetIntegerAtom 0x62fad307
#define H_RtlGetLastRange 0xc6308471
#define H_RtlGetNextEntryHashTable 0x5e146856
#define H_RtlGetNextRange 0xc438a871
#define H_RtlGetNtGlobalFlags 0x1cdfeae2
#define H_RtlGetOwnerSecurityDescriptor 0x601cedaa
#define H_RtlGetProductInfo 0x18fe19fb
#define H_RtlGetSaclSecurityDescriptor 0xcb29ba6b
#define H_RtlGetSetBootStatusData 0xeb5357cd
#define H_RtlGetThreadLangIdByIndex 0xf690c57c
#define H_RtlGetVersion 0xcebdb7e2
#define H_RtlHashUnicodeString 0xd286f21b
#define H_RtlIdnToAscii 0xff8b3cb1
#define H_RtlIdnToNameprepUnicode 0x8bb40f29
#define H_RtlIdnToUnicode 0x6e6d9d6f
#define H_RtlImageDirectoryEntryToData 0x503f7b28
#define H_RtlImageNtHeader 0xdd39fd14
#define H_RtlImageNtHeaderEx 0x7f4515b6
#define H_RtlInitAnsiString 0xee02056a
#define H_RtlInitAnsiStringEx 0x815a9978
#define H_RtlInitCodePageTable 0x0e8f0780
#define H_RtlInitEnumerationHashTable 0x1eaead34
#define H_RtlInitString 0xa5a73edc
#define H_RtlInitUnicodeString 0x3287ec73
#define H_RtlInitUnicodeStringEx 0xfb1cee59
#define H_RtlInitWeakEnumerationHashTable 0x1caa63c1
#define H_RtlInitializeBitMap 0x02474d3f
#define H_RtlInitializeGenericTable 0x792a8e7c
#define H_RtlInitializeGenericTableAvl 0xcf9f5e3d
#define H_RtlInitializeRangeList 0xf06a584d
#define H_RtlInitializeSid 0xa1ea3802
#define H_RtlInitializeUnicodePrefix 0xb99ec1a6
#define H_RtlInsertElementGenericTable 0xbae954de
#define H_RtlInsertElementGenericTableAvl 0x9bc72646
#define H_RtlInsertElementGenericTableFull 0xe373e321
#define H_RtlInsertElementGenericTableFullAvl 0x642c1510
#define H_RtlInsertEntryHashTable 0x5856c974
#define H_RtlInsertUnicodePrefix 0x13c97e57
#define H_RtlInt64ToUnicodeString 0x2d5e9be0
#define H_RtlIntegerToChar 0x7b2c82b5
#define H_RtlIntegerToUnicode 0xd494cfc5
#define H_RtlIntegerToUnicodeString 0x1d6562aa
#define H_RtlInterlockedClearBitRun 0x6e2983f1
#define H_RtlInterlockedSetBitRun 0x42a13d03
#define H_RtlInterlockedSetClearRun 0x7fadfecd
#define H_RtlInvertRangeList 0x5541a757
#define H_RtlInvertRangeListEx 0x69d5f7a8
#define H_RtlIoDecodeMemIoResource 0xdc42dfd8
#define H_RtlIoEncodeMemIoResource 0x99c2dfd8
#define H_RtlIpv4AddressToStringA 0xae5a0a31
#define H_RtlIpv4AddressToStringExA 0x828d57d7
#define H_RtlIpv4AddressToStringExW 0x828d57c1
#define H_RtlIpv4AddressToStringW 0xae5a0a27
#define H_RtlIpv4StringToAddressA 0xa530dde2
#define H_RtlIpv4StringToAddressExA 0x3779950d
#define H_RtlIpv4StringToAddressExW 0x3779951b
#define H_RtlIpv4StringToAddressW 0xa530ddf4
#define H_RtlIpv6AddressToStringA 0xae580a31
#define H_RtlIpv6AddressToStringExA 0x028d57d7
#define H_RtlIpv6AddressToStringExW 0x028d57c1
#define H_RtlIpv6AddressToStringW 0xae580a27
#define H_RtlIpv6StringToAddressA 0xa532dde2
#define H_RtlIpv6StringToAddressExA 0xb779950d
#define H_RtlIpv6StringToAddressExW 0xb779951b
#define H_RtlIpv6StringToAddressW 0xa532ddf4
#define H_RtlIsGenericTableEmpty 0x68043796
#define H_RtlIsGenericTableEmptyAvl 0xf2dd7bea
#define H_RtlIsNameLegalDOS8Dot3 0x3387f37d
#define H_RtlIsNormalizedString 0x06bb6de8
#define H_RtlIsNtDdiVersionAvailable 0x0f343d03
#define H_RtlIsRangeAvailable 0x21a47b03
#define H_RtlIsServicePackVersionInstalled 0xc4afa749
#define H_RtlIsUntrustedObject 0x989553f9
#define H_RtlIsValidOemCharacter 0xeaa4c233
#define H_RtlLCIDToCultureName 0xa630dcf2
#define H_RtlLengthRequiredSid 0xa47e8626
#define H_RtlLengthSecurityDescriptor 0xc74df3d2
#define H_RtlLengthSid 0x01d512ed
#define H_RtlLoadString 0x24b77ede
#define H_RtlLocalTimeToSystemTime 0x6eaf5ad8
#define H_RtlLockBootStatusData 0xfcccbdd2
#define H_RtlLookupAtomInAtomTable 0x8dbd9726
#define H_RtlLookupElementGenericTable 0xca99215c
#define H_RtlLookupElementGenericTableAvl 0x2b892848
#define H_RtlLookupElementGenericTableFull 0xc474e479
#define H_RtlLookupElementGenericTableFullAvl 0x8f28f5f0
#define H_RtlLookupEntryHashTable 0x1658c7c4
#define H_RtlLookupFirstMatchingElementGenericTableAvl 0x80d73432
#define H_RtlLookupFunctionEntry 0x1818a7f8
#define H_RtlMapGenericMask 0xb93f4ae5
#define H_RtlMapSecurityErrorToNtStatus 0xd4713da2
#define H_RtlMergeRangeLists 0xaab94501
#define H_RtlMoveMemory 0xb778f431
#define H_RtlMultiByteToUnicodeN 0x3db7b4df
#define H_RtlMultiByteToUnicodeSize 0x985dcb93
#define H_RtlNextUnicodePrefix 0x506339b0
#define H_RtlNormalizeString 0x71f680b5
#define H_RtlNtStatusToDosError 0x19ea3dcc
#define H_RtlNtStatusToDosErrorNoTeb 0x22a4dc86
#define H_RtlNumberGenericTableElements 0xc3118c40
#define H_RtlNumberGenericTableElementsAvl 0x8808195d
#define H_RtlNumberOfClearBits 0x4b935f65
#define H_RtlNumberOfClearBitsInRange 0x92f003b6
#define H_RtlNumberOfSetBits 0x803fb51e
#define H_RtlNumberOfSetBitsInRange 0x460794ef
#define H_RtlNumberOfSetBitsUlongPtr 0xf2dfb6b9
#define H_RtlOemStringToCountedUnicodeString 0x44804416
#define H_RtlOemStringToUnicodeSize 0x46fcebd2
#define H_RtlOemStringToUnicodeString 0xebf7a6d9
#define H_RtlOemToUnicodeN 0x36ae9739
#define H_RtlOpenCurrentUser 0x17706c40
#define H_RtlOwnerAcesPresent 0x77acfb58
#define H_RtlPcToFileHeader 0xb388d6e5
#define H_RtlPinAtomInAtomTable 0x834048e3
#define H_RtlPrefetchMemoryNonTemporal 0xa237d1f4
#define H_RtlPrefixString 0x0e438801
#define H_RtlPrefixUnicodeString 0x5f3cbbba
#define H_RtlQueryAtomInAtomTable 0x92788244
#define H_RtlQueryDynamicTimeZoneInformation 0x320fa71e
#define H_RtlQueryElevationFlags 0xcff5d945
#define H_RtlQueryInformationAcl 0xa68dd129
#define H_RtlQueryModuleInformation 0xd8cbdf31
#define H_RtlQueryPackageIdentity 0x3c60a85b
#define H_RtlQueryRegistryValues 0x0756c64e
#define H_RtlQueryRegistryValuesEx 0xb193a32d
#define H_RtlQueryTimeZoneInformation 0xf1d1860a
#define H_RtlQueryValidationRunlevel 0x28e00ccd
#define H_RtlRaiseException 0x5d242d81
#define H_RtlRandom 0x4f30857b
#define H_RtlRandomEx 0x215ef134
#define H_RtlRbInsertNodeEx 0x8f076285
#define H_RtlRbRemoveNode 0x9aca1431
#define H_RtlRealPredecessor 0x3cdc2d9e
#define H_RtlRealSuccessor 0x68bb6549
#define H_RtlRemoveEntryHashTable 0x975cc252
#define H_RtlRemoveUnicodePrefix 0x5e576a41
#define H_RtlReplaceSidInSd 0x3cfc1428
#define H_RtlReserveChunk 0x7e32f475
#define H_RtlRestoreContext 0x8a5ee78f
#define H_RtlRunOnceBeginInitialize 0x98c051be
#define H_RtlRunOnceComplete 0x6f83e7f0
#define H_RtlRunOnceExecuteOnce 0x6370249b
#define H_RtlRunOnceInitialize 0xbbf736a8
#define H_RtlSecondsSince1970ToTime 0x48e224c6
#define H_RtlSecondsSince1980ToTime 0x48fc24c6
#define H_RtlSelfRelativeToAbsoluteSD 0xff99a21a
#define H_RtlSelfRelativeToAbsoluteSD2 0xccd10d4d
#define H_RtlSetAllBits 0x289de63c
#define H_RtlSetBit 0x0c79066a
#define H_RtlSetBits 0x3c833575
#define H_RtlSetControlSecurityDescriptor 0x8717293f
#define H_RtlSetDaclSecurityDescriptor 0xcb91ae6b
#define H_RtlSetDynamicTimeZoneInformation 0xaddb4508
#define H_RtlSetGroupSecurityDescriptor 0xc03effaa
#define H_RtlSetOwnerSecurityDescriptor 0x6016edaa
#define H_RtlSetPortableOperatingSystem 0xd8c4f168
#define H_RtlSetSaclSecurityDescriptor 0xcb29ae6b
#define H_RtlSetTimeZoneInformation 0x80dac9e0
#define H_RtlSidHashInitialize 0x83040dc0
#define H_RtlSidHashLookup 0xf920c461
#define H_RtlSizeHeap 0x35bec39c
#define H_RtlSplay 0x3ebee39c
#define H_RtlStringFromGUID 0x20641c7f
#define H_RtlSubAuthorityCountSid 0xeb0a309f
#define H_RtlSubAuthoritySid 0xa0b7db9e
#define H_RtlSubtreePredecessor 0x84193f9d
#define H_RtlSubtreeSuccessor 0x20b5865d
#define H_RtlSystemTimeToLocalTime 0x6199a816
#define H_RtlTestBit 0x4a49e7f2
#define H_RtlTimeFieldsToTime 0xc267cb82
#define H_RtlTimeToElapsedTimeFields 0x16fe6c84
#define H_RtlTimeToSecondsSince1970 0x43c95150
#define H_RtlTimeToSecondsSince1980 0x43c956d0
#define H_RtlTimeToTimeFields 0x2156961c
#define H_RtlTraceDatabaseAdd 0x7c23e184
#define H_RtlTraceDatabaseCreate 0x50b7fe08
#define H_RtlTraceDatabaseDestroy 0x395e9de8
#define H_RtlTraceDatabaseEnumerate 0xd5ca2cfb
#define H_RtlTraceDatabaseFind 0x1113875a
#define H_RtlTraceDatabaseLock 0x105201d5
#define H_RtlTraceDatabaseUnlock 0x91947537
#define H_RtlTraceDatabaseValidate 0x7da96200
#define H_RtlUTF8ToUnicodeN 0xff48ae18
#define H_RtlUnicodeStringToAnsiSize 0xc65e5060
#define H_RtlUnicodeStringToAnsiString 0x451b06f1
#define H_RtlUnicodeStringToCountedOemString 0x342378a3
#define H_RtlUnicodeStringToInteger 0x7a019aa8
#define H_RtlUnicodeStringToOemSize 0xecc20d66
#define H_RtlUnicodeStringToOemString 0x525a8c56
#define H_RtlUnicodeToCustomCPN 0x6afc6fcb
#define H_RtlUnicodeToMultiByteN 0x8940f04e
#define H_RtlUnicodeToMultiByteSize 0x0a6b557b
#define H_RtlUnicodeToOemN 0x736d9fca
#define H_RtlUnicodeToUTF8N 0x10e51e76
#define H_RtlUnlockBootStatusData 0x276f59f3
#define H_RtlUnwind 0xbc13c5ca
#define H_RtlUnwindEx 0xf1728dfc
#define H_RtlUpcaseUnicodeChar 0x08d4d171
#define H_RtlUpcaseUnicodeString 0xf63ab5d3
#define H_RtlUpcaseUnicodeStringToAnsiString 0xfd8829b3
#define H_RtlUpcaseUnicodeStringToCountedOemString 0x789e7241
#define H_RtlUpcaseUnicodeStringToOemString 0xd72baa08
#define H_RtlUpcaseUnicodeToCustomCPN 0xfd5d3382
#define H_RtlUpcaseUnicodeToMultiByteN 0x59eed485
#define H_RtlUpcaseUnicodeToOemN 0x4199b443
#define H_RtlUpperChar 0x7d91aeeb
#define H_RtlUpperString 0xa9dc2882
#define H_RtlValidRelativeSecurityDescriptor 0x63553d68
#define H_RtlValidSecurityDescriptor 0x1fa65831
#define H_RtlValidSid 0xf0396738
#define H_RtlValidateUnicodeString 0x6b4f94c4
#define H_RtlVerifyVersionInfo 0x63e5e035
#define H_RtlVirtualUnwind 0xfe545ca3
#define H_RtlVolumeDeviceToDosName 0x2be2fb25
#define H_RtlWalkFrameChain 0xf194b093
#define H_RtlWeaklyEnumerateEntryHashTable 0xd9a5d83c
#define H_RtlWriteRegistryValue 0x9909b594
#define H_RtlZeroHeap 0x95acf3dc
#define H_RtlZeroMemory 0x3d70dc3a
#define H_RtlxAnsiStringToUnicodeSize 0xe74fb54a
#define H_RtlxOemStringToUnicodeSize 0x16d5cde2
#define H_RtlxUnicodeStringToAnsiSize 0xd2cd4848
#define H_RtlxUnicodeStringToOemSize 0xbceb2b56
#define H_SeAccessCheck 0xdbee0e06
#define H_SeAccessCheckEx 0x83819403
#define H_SeAccessCheckFromState 0x158ac20b
#define H_SeAccessCheckFromStateEx 0xb082e79a
#define H_SeAccessCheckWithHint 0x8f6e3edc
#define H_SeAdjustAccessStateForTrustLabel 0x0ebbe683
#define H_SeAppendPrivileges 0x11d27356
#define H_SeAssignSecurity 0xbfc33083
#define H_SeAssignSecurityEx 0xcc20cd08
#define H_SeAuditHardLinkCreation 0x988ca988
#define H_SeAuditHardLinkCreationWithTransaction 0x0959bdb7
#define H_SeAuditTransactionStateChange 0x7813fb06
#define H_SeAuditingAnyFileEventsWithContext 0xfdafeeaa
#define H_SeAuditingAnyFileEventsWithContextEx 0xfbaa9d93
#define H_SeAuditingFileEvents 0xc30a3661
#define H_SeAuditingFileEventsWithContext 0x5c845e9f
#define H_SeAuditingFileEventsWithContextEx 0x17a7f5d9
#define H_SeAuditingFileOrGlobalEvents 0xccff42e7
#define H_SeAuditingHardLinkEvents 0xaf1eccd9
#define H_SeAuditingHardLinkEventsWithContext 0xc3d3531d
#define H_SeAuditingWithTokenForSubcategory 0x1655ec5b
#define H_SeCaptureSecurityDescriptor 0xd3d47c4b
#define H_SeCaptureSubjectContext 0xe775f013
#define H_SeCaptureSubjectContextEx 0x7c04db25
#define H_SeCloseObjectAuditAlarm 0x08c401f8
#define H_SeCloseObjectAuditAlarmForNonObObject 0xd628afdf
#define H_SeComputeAutoInheritByObjectType 0x8c68dd12
#define H_SeCreateAccessState 0x20dce00a
#define H_SeCreateAccessStateEx 0x3802aacf
#define H_SeCreateClientSecurity 0x844f65fb
#define H_SeCreateClientSecurityEx 0xd97ec3eb
#define H_SeCreateClientSecurityFromSubjectContext 0x504d2f70
#define H_SeCreateClientSecurityFromSubjectContextEx 0x4bdc36eb
#define H_SeDeassignSecurity 0x96261e82
#define H_SeDeleteAccessState 0x00dbce2e
#define H_SeDeleteObjectAuditAlarm 0x524e615c
#define H_SeDeleteObjectAuditAlarmWithTransaction 0x8cc81422
#define H_SeExamineSacl 0x3e9f67e6
#define H_SeExports 0x5e37adb4
#define H_SeFilterToken 0xa42fd4cf
#define H_SeFreePrivileges 0x89463743
#define H_SeGetCachedSigningLevel 0x62e9c8bc
#define H_SeGetLinkedToken 0x9d735169
#define H_SeGetLogonSessionToken 0xb811b384
#define H_SeImpersonateClient 0x7137414a
#define H_SeImpersonateClientEx 0xd052beb5
#define H_SeIsParentOfChildAppContainer 0xb2b8dfa7
#define H_SeLocateProcessImageName 0x023c1036
#define H_SeLockSubjectContext 0xf443e46f
#define H_SeMarkLogonSessionForTerminationNotification 0x00881e3c
#define H_SeOpenObjectAuditAlarm 0x8e7d659e
#define H_SeOpenObjectAuditAlarmForNonObObject 0xcccd3e45
#define H_SeOpenObjectAuditAlarmWithTransaction 0xeac1919a
#define H_SeOpenObjectForDeleteAuditAlarm 0x8aeb9394
#define H_SeOpenObjectForDeleteAuditAlarmWithTransaction 0xc72d8593
#define H_SePrivilegeCheck 0x56ff74e5
#define H_SePrivilegeObjectAuditAlarm 0x7eac711e
#define H_SePublicDefaultDacl 0x3a173c26
#define H_SeQueryAuthenticationIdToken 0x6adade50
#define H_SeQueryInformationToken 0xb9839be8
#define H_SeQuerySecureBootPolicyValue 0xb641236d
#define H_SeQuerySecurityAttributesToken 0x59f79e10
#define H_SeQuerySecurityDescriptorInfo 0x4d094dab
#define H_SeQuerySessionIdToken 0x4699706d
#define H_SeRegisterImageVerificationCallback 0x90640fb3
#define H_SeRegisterLogonSessionTerminatedRoutine 0x09ddd324
#define H_SeReleaseSecurityDescriptor 0xa35d2d63
#define H_SeReleaseSubjectContext 0xefe0e294
#define H_SeReportSecurityEvent 0xbbc3ff3d
#define H_SeReportSecurityEventWithSubCategory 0x76cf100d
#define H_SeSecurityAttributePresent 0xe0d03476
#define H_SeSetAccessStateGenericMapping 0x55b6916e
#define H_SeSetAuditParameter 0x71f1a840
#define H_SeSetSecurityAttributesToken 0x1dee175a
#define H_SeSetSecurityDescriptorInfo 0xd9817eb9
#define H_SeSetSecurityDescriptorInfoEx 0x5fae5498
#define H_SeShouldCheckForAccessRightsFromParent 0xdae829f7
#define H_SeSinglePrivilegeCheck 0x2306509f
#define H_SeSrpAccessCheck 0x41fc0990
#define H_SeSystemDefaultDacl 0xfad5e462
#define H_SeSystemDefaultSd 0x563be0d2
#define H_SeTokenFromAccessInformation 0x4fa408fe
#define H_SeTokenImpersonationLevel 0x58c0f8cf
#define H_SeTokenIsAdmin 0x671cc885
#define H_SeTokenIsRestricted 0x25a70220
#define H_SeTokenIsWriteRestricted 0x8eeb629a
#define H_SeTokenObjectType 0xd6a49f8e
#define H_SeTokenType 0x3e41c3b9
#define H_SeUnlockSubjectContext 0x26ddb4ad
#define H_SeUnregisterImageVerificationCallback 0xacc58a16
#define H_SeUnregisterLogonSessionTerminatedRoutine 0x5a17cb7e
#define H_SeValidSecurityDescriptor 0x1f8f7f23
#define H_TmCancelPropagationRequest 0x50368110
#define H_TmCommitComplete 0xe7e3d7c8
#define H_TmCommitEnlistment 0xe7d06a76
#define H_TmCommitTransaction 0x071af27b
#define H_TmCreateEnlistment 0xa713ce42
#define H_TmCurrentTransaction 0x105cc289
#define H_TmDereferenceEnlistmentKey 0xdfff723c
#define H_TmEnableCallbacks 0x99b3c6b0
#define H_TmEndPropagationRequest 0x84068df4
#define H_TmEnlistmentObjectType 0x72d90d35
#define H_TmFreezeTransactions 0x16f42dd0
#define H_TmGetTransactionId 0x611bfca2
#define H_TmInitSystem 0x70a47b16
#define H_TmInitSystemPhase2 0x1dd0a9f6
#define H_TmInitializeTransactionManager 0x4b5ab620
#define H_TmIsKTMCommitCoordinator 0xfdcad220
#define H_TmIsTransactionActive 0x59a851a4
#define H_TmPrePrepareComplete 0x96321cab
#define H_TmPrePrepareEnlistment 0x9508b602
#define H_TmPrepareComplete 0xfe140630
#define H_TmPrepareEnlistment 0x13ae6c0b
#define H_TmPropagationComplete 0xe46af34e
#define H_TmPropagationFailed 0xc4289502
#define H_TmReadOnlyEnlistment 0xb4e88780
#define H_TmRecoverEnlistment 0xeadea7d3
#define H_TmRecoverResourceManager 0xed3c811c
#define H_TmRecoverTransactionManager 0xadc3c504
#define H_TmReferenceEnlistmentKey 0xfed51f1c
#define H_TmRenameTransactionManager 0x60d60b6f
#define H_TmRequestOutcomeEnlistment 0xd077b343
#define H_TmResourceManagerObjectType 0x8eb31cf5
#define H_TmRollbackComplete 0xaabf38c4
#define H_TmRollbackEnlistment 0xdc137921
#define H_TmRollbackTransaction 0xe69359e6
#define H_TmSetCurrentTransaction 0x0331f4ae
#define H_TmSinglePhaseReject 0x34a180f8
#define H_TmThawTransactions 0x86cb457f
#define H_TmTransactionManagerObjectType 0x4ad79560
#define H_TmTransactionObjectType 0x2ac8990b
#define H_VerSetConditionMask 0x969e82a5
#define H_VfFailDeviceNode 0x0a2bc3c3
#define H_VfFailDriver 0x4ce4d976
#define H_VfFailSystemBIOS 0x17a8224e
#define H_VfInsertContext 0x28edaea3
#define H_VfIsVerificationEnabled 0x547c5549
#define H_VfQueryDeviceContext 0x69eb0fac
#define H_VfQueryDispatchTable 0x4969c4ca
#define H_VfQueryDriverContext 0xefc7e863
#define H_VfQueryIrpContext 0xb270e029
#define H_VfQueryThreadContext 0x6ae8aeaa
#define H_VfRemoveContext 0x2d7ec926
#define H_WheaAddErrorSource 0xa9c72184
#define H_WheaAttemptPhysicalPageOffline 0x8f387f35
#define H_WheaConfigureErrorSource 0x77070149
#define H_WheaDeferredRecoveryService 0x4dab034c
#define H_WheaGetErrorSource 0x99d52184
#define H_WheaInitializeDeferredRecoveryObject 0x90836f63
#define H_WheaInitializeRecordHeader 0xd7022594
#define H_WheaRegisterInUsePageOfflineNotification 0x77e9a23b
#define H_WheaReportHwError 0xa1cef9ee
#define H_WheaRequestDeferredRecovery 0x0b0d4038
#define H_WheaUnregisterInUsePageOfflineNotification 0x6b8d8790
#define H_WmiGetClock 0xd41473b9
#define H_WmiQueryTraceInformation 0xb84db92a
#define H_WmiTraceMessage 0xb329d925
#define H_WmiTraceMessageVa 0x764947ab
#define H_XIPDispatch 0xdcb48053
#define H_ZwAccessCheckAndAuditAlarm 0xda97e599
#define H_ZwAddBootEntry 0xc87abfed
#define H_ZwAddDriverEntry 0x3a7c8a0a
#define H_ZwAdjustPrivilegesToken 0x6fe44add
#define H_ZwAlertThread 0xee125234
#define H_ZwAllocateLocallyUniqueId 0xeebcb8b9
#define H_ZwAllocateVirtualMemory 0x594aa9e4
#define H_ZwAlpcAcceptConnectPort 0xd1b5248e
#define H_ZwAlpcCancelMessage 0x7c5ca6ae
#define H_ZwAlpcConnectPort 0xc390f19b
#define H_ZwAlpcConnectPortEx 0x3c66d21c
#define H_ZwAlpcCreatePort 0xa61ed3ed
#define H_ZwAlpcCreatePortSection 0x994074fd
#define H_ZwAlpcCreateResourceReserve 0xd51939f8
#define H_ZwAlpcCreateSectionView 0x5c009971
#define H_ZwAlpcCreateSecurityContext 0x9107fb71
#define H_ZwAlpcDeletePortSection 0xb9475ad9
#define H_ZwAlpcDeleteResourceReserve 0x97194b1a
#define H_ZwAlpcDeleteSectionView 0x7c07b755
#define H_ZwAlpcDeleteSecurityContext 0xd3078993
#define H_ZwAlpcDisconnectPort 0x62b4fc4f
#define H_ZwAlpcQueryInformation 0x6dde551b
#define H_ZwAlpcSendWaitReceivePort 0x42c755b4
#define H_ZwAlpcSetInformation 0x06f7c424
#define H_ZwAssignProcessToJobObject 0xaf79ba4a
#define H_ZwAssociateWaitCompletionPacket 0x94665382
#define H_ZwCancelIoFile 0x4c69bc5e
#define H_ZwCancelIoFileEx 0x6f17b1e2
#define H_ZwCancelTimer 0xa6521567
#define H_ZwClearEvent 0xb219ad01
#define H_ZwClose 0x3d9a9259
#define H_ZwCloseObjectAuditAlarm 0x2c5401f8
#define H_ZwCommitComplete 0xe7e3cba0
#define H_ZwCommitEnlistment 0xe0ca6a76
#define H_ZwCommitTransaction 0x8a1af278
#define H_ZwConnectPort 0xa1689e9c
#define H_ZwCreateDirectoryObject 0x16d8da3b
#define H_ZwCreateEnlistment 0xa009ce42
#define H_ZwCreateEvent 0x3eb454a1
#define H_ZwCreateFile 0xaa1aac22
#define H_ZwCreateIoCompletion 0xf6ea4fb3
#define H_ZwCreateJobObject 0x718029b7
#define H_ZwCreateKey 0x8f57734d
#define H_ZwCreateKeyTransacted 0xbb625a33
#define H_ZwCreateResourceManager 0xbb87cd96
#define H_ZwCreateSection 0x766f60db
#define H_ZwCreateSymbolicLinkObject 0x98b355f9
#define H_ZwCreateTimer 0x2d565126
#define H_ZwCreateTransaction 0xebc8e858
#define H_ZwCreateTransactionManager 0x3c89126d
#define H_ZwCreateWaitCompletionPacket 0xb41bfe8f
#define H_ZwCreateWnfStateName 0x4d99ca2a
#define H_ZwDeleteBootEntry 0xb194d4fc
#define H_ZwDeleteDriverEntry 0xa0b8d471
#define H_ZwDeleteFile 0x3d08bc21
#define H_ZwDeleteKey 0x8879576d
#define H_ZwDeleteValueKey 0x1f42593a
#define H_ZwDeleteWnfStateData 0x4f4ed4be
#define H_ZwDeleteWnfStateName 0x4e0ed83a
#define H_ZwDeviceIoControlFile 0x385d3092
#define H_ZwDisplayString 0x9ed4ac6b
#define H_ZwDuplicateObject 0x63e792e8
#define H_ZwDuplicateToken 0x89678f29
#define H_ZwEnumerateBootEntries 0x16c56643
#define H_ZwEnumerateDriverEntries 0xab2d1222
#define H_ZwEnumerateKey 0x7601824e
#define H_ZwEnumerateTransactionObject 0xeba3d42a
#define H_ZwEnumerateValueKey 0xec84f025
#define H_ZwFlushBuffersFile 0x5c5d7cfb
#define H_ZwFlushBuffersFileEx 0x5f3ef5ef
#define H_ZwFlushInstructionCache 0x45f1e847
#define H_ZwFlushKey 0x4a9f417b
#define H_ZwFlushVirtualMemory 0x8e79973f
#define H_ZwFreeVirtualMemory 0xbed3922c
#define H_ZwFsControlFile 0x9a5dcc87
#define H_ZwGetNotificationResourceManager 0x2907137e
#define H_ZwImpersonateAnonymousToken 0x8c3dd0f9
#define H_ZwInitiatePowerAction 0xb1714e82
#define H_ZwIsProcessInJob 0x3123d117
#define H_ZwLoadDriver 0x42f57d33
#define H_ZwLoadKey 0x467dc187
#define H_ZwLoadKeyEx 0x7061f367
#define H_ZwLockFile 0xcf43c956
#define H_ZwLockProductActivationKeys 0xed3d44a0
#define H_ZwLockVirtualMemory 0xced9a834
#define H_ZwMakeTemporaryObject 0x128ce9d3
#define H_ZwMapViewOfSection 0x594d9a3c
#define H_ZwModifyBootEntry 0xe1155056
#define H_ZwModifyDriverEntry 0xc1924051
#define H_ZwNotifyChangeKey 0xc074a308
#define H_ZwNotifyChangeSession 0x227d5dde
#define H_ZwOpenDirectoryObject 0xf5f05c90
#define H_ZwOpenEnlistment 0xdc6cde97
#define H_ZwOpenEvent 0x9138f6bb
#define H_ZwOpenFile 0x9f45b566
#define H_ZwOpenJobObject 0xdb78e396
#define H_ZwOpenKey 0x073dcd7f
#define H_ZwOpenKeyEx 0x735fe337
#define H_ZwOpenKeyTransacted 0xa1cdd691
#define H_ZwOpenKeyTransactedEx 0x75a44a8b
#define H_ZwOpenProcess 0x9c0ac99d
#define H_ZwOpenProcessToken 0xadacbe07
#define H_ZwOpenProcessTokenEx 0x2f81c993
#define H_ZwOpenResourceManager 0x58af4b3d
#define H_ZwOpenSection 0x5ee9cb38
#define H_ZwOpenSession 0x5e09cb39
#define H_ZwOpenSymbolicLinkObject 0x4dcf30e9
#define H_ZwOpenThread 0x7e9997a5
#define H_ZwOpenThreadToken 0xb9364fc0
#define H_ZwOpenThreadTokenEx 0x93f00cb5
#define H_ZwOpenTimer 0x82daf33c
#define H_ZwOpenTransaction 0xd94082e6
#define H_ZwOpenTransactionManager 0xe9f5777d
#define H_ZwPowerInformation 0xeb6c4f48
#define H_ZwPrePrepareComplete 0x16321d6d
#define H_ZwPrePrepareEnlistment 0x95791602
#define H_ZwPrepareComplete 0xfe1a3230
#define H_ZwPrepareEnlistment 0x9eae6c08
#define H_ZwPropagationComplete 0xe46a100e
#define H_ZwPropagationFailed 0x49289501
#define H_ZwProtectVirtualMemory 0x3836c63e
#define H_ZwPulseEvent 0x2b0be5b0
#define H_ZwQueryBootEntryOrder 0x7cad4f0e
#define H_ZwQueryBootOptions 0xa08a5b94
#define H_ZwQueryDefaultLocale 0x2d18dd25
#define H_ZwQueryDefaultUILanguage 0xda37fe2b
#define H_ZwQueryDirectoryFile 0x5f8b35d6
#define H_ZwQueryDirectoryObject 0x7db5a65e
#define H_ZwQueryDriverEntryOrder 0x4d18d2ef
#define H_ZwQueryEaFile 0xf8916431
#define H_ZwQueryFullAttributesFile 0xf1a4e4fd
#define H_ZwQueryInformationEnlistment 0x242b98d7
#define H_ZwQueryInformationFile 0x0f7ba4b7
#define H_ZwQueryInformationJobObject 0x5a886d1a
#define H_ZwQueryInformationProcess 0xa638ce5f
#define H_ZwQueryInformationResourceManager 0x9a95793a
#define H_ZwQueryInformationThread 0xfaedf3aa
#define H_ZwQueryInformationToken 0x9d139be8
#define H_ZwQueryInformationTransaction 0xfae3a29a
#define H_ZwQueryInformationTransactionManager 0xa90d303b
#define H_ZwQueryInstallUILanguage 0x737aee7f
#define H_ZwQueryKey 0x589125fb
#define H_ZwQueryLicenseValue 0xa5ae8438
#define H_ZwQueryObject 0xcd1263f0
#define H_ZwQueryQuotaInformationFile 0x8986a55c
#define H_ZwQuerySection 0x1b1305b0
#define H_ZwQuerySecurityAttributesToken 0x59f7d730
#define H_ZwQuerySecurityObject 0xa00ca1e1
#define H_ZwQuerySymbolicLinkObject 0x141e3856
#define H_ZwQuerySystemEnvironmentValueEx 0x8bce8721
#define H_ZwQuerySystemInformation 0xbc44a131
#define H_ZwQuerySystemInformationEx 0x284c4de9
#define H_ZwQueryValueKey 0x9801cd8c
#define H_ZwQueryVirtualMemory 0x0de0b73b
#define H_ZwQueryVolumeInformationFile 0xd3967eb1
#define H_ZwQueryWnfStateData 0x273aedf5
#define H_ZwQueryWnfStateNameInformation 0x246b8a38
#define H_ZwReadFile 0x3f7fe146
#define H_ZwReadOnlyEnlistment 0x34e88646
#define H_ZwRecoverEnlistment 0x67dea7d0
#define H_ZwRecoverResourceManager 0x853c8100
#define H_ZwRecoverTransactionManager 0xae4ec504
#define H_ZwRemoveIoCompletion 0xee7d5f8b
#define H_ZwRemoveIoCompletionEx 0x57e2d967
#define H_ZwRenameKey 0x0e795f4c
#define H_ZwReplaceKey 0x59581c1e
#define H_ZwRequestPort 0xbe5e9a59
#define H_ZwRequestWaitReplyPort 0x178eb1ff
#define H_ZwResetEvent 0x3b35bd39
#define H_ZwRestoreKey 0x495e7c6f
#define H_ZwRollbackComplete 0xada538c4
#define H_ZwRollbackEnlistment 0x5c1378e7
#define H_ZwRollbackTransaction 0xe693baa6
#define H_ZwSaveKey 0x365dbdf6
#define H_ZwSaveKeyEx 0x6f7daf6f
#define H_ZwSecureConnectPort 0xb0391505
#define H_ZwSetBootEntryOrder 0xbd9ecfd8
#define H_ZwSetBootOptions 0x3c8ced9d
#define H_ZwSetCachedSigningLevel 0x467888bc
#define H_ZwSetDefaultLocale 0x809aba24
#define H_ZwSetDefaultUILanguage 0xc0efd85b
#define H_ZwSetDriverEntryOrder 0xad2d62a3
#define H_ZwSetEaFile 0xcb11b2f0
#define H_ZwSetEvent 0x297ee0fd
#define H_ZwSetInformationEnlistment 0x25861ab0
#define H_ZwSetInformationFile 0x96bbcfd7
#define H_ZwSetInformationJobObject 0x948b361e
#define H_ZwSetInformationKey 0x652e318a
#define H_ZwSetInformationObject 0x4335d412
#define H_ZwSetInformationProcess 0xca2bf652
#define H_ZwSetInformationResourceManager 0x97f96a02
#define H_ZwSetInformationThread 0xe035d5da
#define H_ZwSetInformationToken 0x7d262ba4
#define H_ZwSetInformationTransaction 0x2c22911a
#define H_ZwSetInformationVirtualMemory 0x6225ab74
#define H_ZwSetQuotaInformationFile 0x4785fe58
#define H_ZwSetSecurityObject 0x613f2137
#define H_ZwSetSystemEnvironmentValueEx 0xc72eb291
#define H_ZwSetSystemInformation 0xa69c8741
#define H_ZwSetSystemTime 0x9e411989
#define H_ZwSetTimer 0x3a9ce57a
#define H_ZwSetTimerEx 0x395eac5f
#define H_ZwSetValueKey 0xadb1816c
#define H_ZwSetVolumeInformationFile 0xd23bfcd6
#define H_ZwTerminateJobObject 0x430f2852
#define H_ZwTerminateProcess 0xb318a842
#define H_ZwTraceEvent 0x2c11a5b2
#define H_ZwTranslateFilePath 0xf37ca0e5
#define H_ZwUnloadDriver 0x95849b61
#define H_ZwUnloadKey 0xc94f573c
#define H_ZwUnloadKeyEx 0xd5cf10ab
#define H_ZwUnlockFile 0x56089491
#define H_ZwUnlockVirtualMemory 0x027c06d7
#define H_ZwUnmapViewOfSection 0x9ed4d161
#define H_ZwUpdateWnfStateData 0x47dbc4ae
#define H_ZwWaitForMultipleObjects 0x936ca7a2
#define H_ZwWaitForSingleObject 0xc546da13
#define H_ZwWriteFile 0x8f3fc778
#define H_ZwYieldExecution 0xd5f7ee1d
#define H___C_specific_handler 0xc0c6447c
#define H___chkstk 0x8dc38575
#define H___misaligned_access 0xb890ecd2
#define H__i64toa_s 0x122ab654
#define H__i64tow_s 0x122f3654
#define H__itoa 0xfd3d37e4
#define H__itoa_s 0x4df910bc
#define H__itow 0xfd3d37f2
#define H__itow_s 0x4dfc90bc
#define H__local_unwind 0x0ade933b
#define H__ltoa_s 0x4df91094
#define H__ltow_s 0x4dfc9094
#define H__makepath_s 0xff4daf83
#define H__purecall 0x0399a1fa
#define H__setjmp 0x5e9bc96e
#define H__setjmpex 0xf25ba55e
#define H__snprintf 0x72dc01e1
#define H__snprintf_s 0x00787344
#define H__snscanf_s 0xee0460c2
#define H__snwprintf 0xdee665ce
#define H__snwprintf_s 0x9973984a
#define H__snwscanf_s 0xf3367714
#define H__splitpath_s 0x7242008d
#define H__stricmp 0x2d873957
#define H__strlwr 0x4e5a446d
#define H__strnicmp 0xb2df2566
#define H__strnset 0x2d633d53
#define H__strnset_s 0xcf54e4ab
#define H__strrev 0x4e5dcd69
#define H__strset 0x4e5d8d6b
#define H__strset_s 0x635afc64
#define H__strtoui64 0x0d5390bd
#define H__strupr 0x4e5c07ed
#define H__swprintf 0x72dc65e1
#define H__ui64toa_s 0xb82ab67b
#define H__ui64tow_s 0xb82f367b
#define H__ultoa_s 0x4d47b894
#define H__ultow_s 0x4d423894
#define H__vsnprintf 0xdbdc01ce
#define H__vsnprintf_s 0x00739904
#define H__vsnwprintf 0x5ee6721a
#define H__vsnwprintf_s 0x9c86b84a
#define H__vswprintf 0xdbdc65ce
#define H__wcsicmp 0x3d8729ef
#define H__wcslwr 0x3e7a444c
#define H__wcsnicmp 0xb2d7796e
#define H__wcsnset 0x3d632deb
#define H__wcsnset_s 0xcb7ae0ab
#define H__wcsrev 0x3e7dcd48
#define H__wcsset_s 0x6352a06c
#define H__wcsupr 0x3e7c07cc
#define H__wmakepath_s 0xff464583
#define H__wsplitpath_s 0x77b7008d
#define H__wtoi 0xfefd37ec
#define H__wtol 0xfefd37e9
#define H_atoi 0x0c3d37e9
#define H_atol 0x0c3d37ec
#define H_bsearch 0x5c3d3a76
#define H_bsearch_s 0x4e9db8fc
#define H_isdigit 0x4d38536a
#define H_islower 0xcdfc556c
#define H_isprint 0x0e5bd0eb
#define H_isspace 0x3e19d67a
#define H_isupper 0x5e1d956d
#define H_isxdigit 0x4dea3b32
#define H_longjmp 0xecfb058e
#define H_mbstowcs 0x4d267a6c
#define H_mbtowc 0x2e9bf88d
#define H_memchr 0x5db8f71c
#define H_memcmp 0x5db8f59e
#define H_memcpy 0x5db8fb17
#define H_memcpy_s 0x3ec5f89d
#define H_memmove 0xddba4c4b
#define H_memmove_s 0x9312d89d
#define H_memset 0x5dbcf19a
#define H_psMUITest 0xea7e4f58
#define H_qsort 0x1e7bf973
#define H_rand 0x0e587764
#define H_sprintf 0x2d3a75e1
#define H_sprintf_s 0x9d7864bd
#define H_srand 0x3e587763
#define H_sscanf_s 0x1d3e60ed
#define H_strcat 0x4e58f36b
#define H_strcat_s 0x3cdafc65
#define H_strchr 0x4e58f7ed
#define H_strcmp 0x4e58f56f
#define H_strcpy 0x4e58fbe6
#define H_strcpy_s 0x3ef9bc65
#define H_strlen 0x4e5b3171
#define H_strncat 0x2dd93f53
#define H_strncat_s 0x4fd4e485
#define H_strncmp 0x2dd93957
#define H_strncpy 0x2dd937de
#define H_strncpy_s 0x4df7a485
#define H_strnlen 0x2ddafd49
#define H_strrchr 0x2e593bd5
#define H_strspn 0x4e5cfbf1
#define H_strstr 0x4e5cf9ed
#define H_strtok_s 0x4d1d3c64
#define H_swprintf 0x2ddc65e1
#define H_swprintf_s 0x19786484
#define H_swscanf_s 0x6e3660ed
#define H_tolower 0xcdfc218c
#define H_toupper 0x5e1de18d
#define H_towlower 0xcd144d4c
#define H_towupper 0x5ef58d4d
#define H_vDbgPrintEx 0xff127ccc
#define H_vDbgPrintExWithPrefix 0x01cf256b
#define H_vsprintf 0x2dd675e1
#define H_vsprintf_s 0x9d786486
#define H_vswprintf_s 0x19787904
#define H_wcscat 0x3e78f34a
#define H_wcscat_s 0x3cd2a06d
#define H_wcschr 0x3e78f7cc
#define H_wcscmp 0x3e78f54e
#define H_wcscpy 0x3e78fbc7
#define H_wcscpy_s 0x3ef1e06d
#define H_wcscspn 0x3c7d2771
#define H_wcslen 0x3e7b3150
#define H_wcsncat 0x3dd92feb
#define H_wcsncat_s 0x4bfae085
#define H_wcsncmp 0x3dd929ef
#define H_wcsncpy 0x3dd92766
#define H_wcsncpy_s 0x49d9a085
#define H_wcsnlen 0x3ddaedf1
#define H_wcsrchr 0x3e592b6d
#define H_wcsspn 0x3e7cfbd0
#define H_wcsstr 0x3e7cf9cc
#define H_wcstombs 0x4d14feec
#define H_wcstoul 0x3e9a25f3
#define H_wctomb 0x3e9bf55c

#define I_AlpcGetHeaderSize ImportFunc<I_MODULE_NT, H_AlpcGetHeaderSize>
#define I_AlpcGetMessageAttribute ImportFunc<I_MODULE_NT, H_AlpcGetMessageAttribute>
#define I_AlpcInitializeMessageAttribute ImportFunc<I_MODULE_NT, H_AlpcInitializeMessageAttribute>
#define I_BgkDisplayCharacter ImportFunc<I_MODULE_NT, H_BgkDisplayCharacter>
#define I_BgkGetConsoleState ImportFunc<I_MODULE_NT, H_BgkGetConsoleState>
#define I_BgkGetCursorState ImportFunc<I_MODULE_NT, H_BgkGetCursorState>
#define I_BgkSetCursor ImportFunc<I_MODULE_NT, H_BgkSetCursor>
#define I_CcAddDirtyPagesToExternalCache ImportFunc<I_MODULE_NT, H_CcAddDirtyPagesToExternalCache>
#define I_CcCanIWrite ImportFunc<I_MODULE_NT, H_CcCanIWrite>
#define I_CcCoherencyFlushAndPurgeCache ImportFunc<I_MODULE_NT, H_CcCoherencyFlushAndPurgeCache>
#define I_CcCopyRead ImportFunc<I_MODULE_NT, H_CcCopyRead>
#define I_CcCopyReadEx ImportFunc<I_MODULE_NT, H_CcCopyReadEx>
#define I_CcCopyWrite ImportFunc<I_MODULE_NT, H_CcCopyWrite>
#define I_CcCopyWriteEx ImportFunc<I_MODULE_NT, H_CcCopyWriteEx>
#define I_CcCopyWriteWontFlush ImportFunc<I_MODULE_NT, H_CcCopyWriteWontFlush>
#define I_CcDeductDirtyPagesFromExternalCache ImportFunc<I_MODULE_NT, H_CcDeductDirtyPagesFromExternalCache>
#define I_CcDeferWrite ImportFunc<I_MODULE_NT, H_CcDeferWrite>
#define I_CcFastCopyRead ImportFunc<I_MODULE_NT, H_CcFastCopyRead>
#define I_CcFastCopyWrite ImportFunc<I_MODULE_NT, H_CcFastCopyWrite>
#define I_CcFastMdlReadWait ImportFunc<I_MODULE_NT, H_CcFastMdlReadWait>
#define I_CcFlushCache ImportFunc<I_MODULE_NT, H_CcFlushCache>
#define I_CcFlushCacheToLsn ImportFunc<I_MODULE_NT, H_CcFlushCacheToLsn>
#define I_CcGetDirtyPages ImportFunc<I_MODULE_NT, H_CcGetDirtyPages>
#define I_CcGetFileObjectFromBcb ImportFunc<I_MODULE_NT, H_CcGetFileObjectFromBcb>
#define I_CcGetFileObjectFromSectionPtrs ImportFunc<I_MODULE_NT, H_CcGetFileObjectFromSectionPtrs>
#define I_CcGetFileObjectFromSectionPtrsRef ImportFunc<I_MODULE_NT, H_CcGetFileObjectFromSectionPtrsRef>
#define I_CcGetFlushedValidData ImportFunc<I_MODULE_NT, H_CcGetFlushedValidData>
#define I_CcGetLsnForFileObject ImportFunc<I_MODULE_NT, H_CcGetLsnForFileObject>
#define I_CcInitializeCacheMap ImportFunc<I_MODULE_NT, H_CcInitializeCacheMap>
#define I_CcIsThereDirtyData ImportFunc<I_MODULE_NT, H_CcIsThereDirtyData>
#define I_CcIsThereDirtyDataEx ImportFunc<I_MODULE_NT, H_CcIsThereDirtyDataEx>
#define I_CcIsThereDirtyLoggedPages ImportFunc<I_MODULE_NT, H_CcIsThereDirtyLoggedPages>
#define I_CcMapData ImportFunc<I_MODULE_NT, H_CcMapData>
#define I_CcMdlRead ImportFunc<I_MODULE_NT, H_CcMdlRead>
#define I_CcMdlReadComplete ImportFunc<I_MODULE_NT, H_CcMdlReadComplete>
#define I_CcMdlWriteAbort ImportFunc<I_MODULE_NT, H_CcMdlWriteAbort>
#define I_CcMdlWriteComplete ImportFunc<I_MODULE_NT, H_CcMdlWriteComplete>
#define I_CcPinMappedData ImportFunc<I_MODULE_NT, H_CcPinMappedData>
#define I_CcPinRead ImportFunc<I_MODULE_NT, H_CcPinRead>
#define I_CcPrepareMdlWrite ImportFunc<I_MODULE_NT, H_CcPrepareMdlWrite>
#define I_CcPreparePinWrite ImportFunc<I_MODULE_NT, H_CcPreparePinWrite>
#define I_CcPurgeCacheSection ImportFunc<I_MODULE_NT, H_CcPurgeCacheSection>
#define I_CcRegisterExternalCache ImportFunc<I_MODULE_NT, H_CcRegisterExternalCache>
#define I_CcRemapBcb ImportFunc<I_MODULE_NT, H_CcRemapBcb>
#define I_CcRepinBcb ImportFunc<I_MODULE_NT, H_CcRepinBcb>
#define I_CcScheduleReadAhead ImportFunc<I_MODULE_NT, H_CcScheduleReadAhead>
#define I_CcScheduleReadAheadEx ImportFunc<I_MODULE_NT, H_CcScheduleReadAheadEx>
#define I_CcSetAdditionalCacheAttributes ImportFunc<I_MODULE_NT, H_CcSetAdditionalCacheAttributes>
#define I_CcSetAdditionalCacheAttributesEx ImportFunc<I_MODULE_NT, H_CcSetAdditionalCacheAttributesEx>
#define I_CcSetBcbOwnerPointer ImportFunc<I_MODULE_NT, H_CcSetBcbOwnerPointer>
#define I_CcSetDirtyPageThreshold ImportFunc<I_MODULE_NT, H_CcSetDirtyPageThreshold>
#define I_CcSetDirtyPinnedData ImportFunc<I_MODULE_NT, H_CcSetDirtyPinnedData>
#define I_CcSetFileSizes ImportFunc<I_MODULE_NT, H_CcSetFileSizes>
#define I_CcSetFileSizesEx ImportFunc<I_MODULE_NT, H_CcSetFileSizesEx>
#define I_CcSetLogHandleForFile ImportFunc<I_MODULE_NT, H_CcSetLogHandleForFile>
#define I_CcSetLogHandleForFileEx ImportFunc<I_MODULE_NT, H_CcSetLogHandleForFileEx>
#define I_CcSetLoggedDataThreshold ImportFunc<I_MODULE_NT, H_CcSetLoggedDataThreshold>
#define I_CcSetParallelFlushFile ImportFunc<I_MODULE_NT, H_CcSetParallelFlushFile>
#define I_CcSetReadAheadGranularity ImportFunc<I_MODULE_NT, H_CcSetReadAheadGranularity>
#define I_CcSetReadAheadGranularityEx ImportFunc<I_MODULE_NT, H_CcSetReadAheadGranularityEx>
#define I_CcTestControl ImportFunc<I_MODULE_NT, H_CcTestControl>
#define I_CcUninitializeCacheMap ImportFunc<I_MODULE_NT, H_CcUninitializeCacheMap>
#define I_CcUnmapFileOffsetFromSystemCache ImportFunc<I_MODULE_NT, H_CcUnmapFileOffsetFromSystemCache>
#define I_CcUnpinData ImportFunc<I_MODULE_NT, H_CcUnpinData>
#define I_CcUnpinDataForThread ImportFunc<I_MODULE_NT, H_CcUnpinDataForThread>
#define I_CcUnpinRepinnedBcb ImportFunc<I_MODULE_NT, H_CcUnpinRepinnedBcb>
#define I_CcUnregisterExternalCache ImportFunc<I_MODULE_NT, H_CcUnregisterExternalCache>
#define I_CcWaitForCurrentLazyWriterActivity ImportFunc<I_MODULE_NT, H_CcWaitForCurrentLazyWriterActivity>
#define I_CcZeroData ImportFunc<I_MODULE_NT, H_CcZeroData>
#define I_CcZeroDataOnDisk ImportFunc<I_MODULE_NT, H_CcZeroDataOnDisk>
#define I_CmCallbackGetKeyObjectID ImportFunc<I_MODULE_NT, H_CmCallbackGetKeyObjectID>
#define I_CmCallbackGetKeyObjectIDEx ImportFunc<I_MODULE_NT, H_CmCallbackGetKeyObjectIDEx>
#define I_CmCallbackReleaseKeyObjectIDEx ImportFunc<I_MODULE_NT, H_CmCallbackReleaseKeyObjectIDEx>
#define I_CmGetBoundTransaction ImportFunc<I_MODULE_NT, H_CmGetBoundTransaction>
#define I_CmGetCallbackVersion ImportFunc<I_MODULE_NT, H_CmGetCallbackVersion>
#define I_CmKeyObjectType ImportFunc<I_MODULE_NT, H_CmKeyObjectType>
#define I_CmRegisterCallback ImportFunc<I_MODULE_NT, H_CmRegisterCallback>
#define I_CmRegisterCallbackEx ImportFunc<I_MODULE_NT, H_CmRegisterCallbackEx>
#define I_CmSetCallbackObjectContext ImportFunc<I_MODULE_NT, H_CmSetCallbackObjectContext>
#define I_CmUnRegisterCallback ImportFunc<I_MODULE_NT, H_CmUnRegisterCallback>
#define I_DbgBreakPoint ImportFunc<I_MODULE_NT, H_DbgBreakPoint>
#define I_DbgBreakPointWithStatus ImportFunc<I_MODULE_NT, H_DbgBreakPointWithStatus>
#define I_DbgCommandString ImportFunc<I_MODULE_NT, H_DbgCommandString>
#define I_DbgLoadImageSymbols ImportFunc<I_MODULE_NT, H_DbgLoadImageSymbols>
#define I_DbgPrint ImportFunc<I_MODULE_NT, H_DbgPrint>
#define I_DbgPrintEx ImportFunc<I_MODULE_NT, H_DbgPrintEx>
#define I_DbgPrintReturnControlC ImportFunc<I_MODULE_NT, H_DbgPrintReturnControlC>
#define I_DbgPrompt ImportFunc<I_MODULE_NT, H_DbgPrompt>
#define I_DbgQueryDebugFilterState ImportFunc<I_MODULE_NT, H_DbgQueryDebugFilterState>
#define I_DbgSetDebugFilterState ImportFunc<I_MODULE_NT, H_DbgSetDebugFilterState>
#define I_DbgSetDebugPrintCallback ImportFunc<I_MODULE_NT, H_DbgSetDebugPrintCallback>
#define I_DbgkLkmdRegisterCallback ImportFunc<I_MODULE_NT, H_DbgkLkmdRegisterCallback>
#define I_DbgkLkmdUnregisterCallback ImportFunc<I_MODULE_NT, H_DbgkLkmdUnregisterCallback>
#define I_DbgkWerCaptureLiveKernelDump ImportFunc<I_MODULE_NT, H_DbgkWerCaptureLiveKernelDump>
#define I_EmClientQueryRuleState ImportFunc<I_MODULE_NT, H_EmClientQueryRuleState>
#define I_EmClientRuleDeregisterNotification ImportFunc<I_MODULE_NT, H_EmClientRuleDeregisterNotification>
#define I_EmClientRuleEvaluate ImportFunc<I_MODULE_NT, H_EmClientRuleEvaluate>
#define I_EmClientRuleRegisterNotification ImportFunc<I_MODULE_NT, H_EmClientRuleRegisterNotification>
#define I_EmProviderDeregister ImportFunc<I_MODULE_NT, H_EmProviderDeregister>
#define I_EmProviderDeregisterEntry ImportFunc<I_MODULE_NT, H_EmProviderDeregisterEntry>
#define I_EmProviderRegister ImportFunc<I_MODULE_NT, H_EmProviderRegister>
#define I_EmProviderRegisterEntry ImportFunc<I_MODULE_NT, H_EmProviderRegisterEntry>
#define I_EmpProviderRegister ImportFunc<I_MODULE_NT, H_EmpProviderRegister>
#define I_EtwActivityIdControl ImportFunc<I_MODULE_NT, H_EtwActivityIdControl>
#define I_EtwEnableTrace ImportFunc<I_MODULE_NT, H_EtwEnableTrace>
#define I_EtwEventEnabled ImportFunc<I_MODULE_NT, H_EtwEventEnabled>
#define I_EtwProviderEnabled ImportFunc<I_MODULE_NT, H_EtwProviderEnabled>
#define I_EtwRegister ImportFunc<I_MODULE_NT, H_EtwRegister>
#define I_EtwRegisterClassicProvider ImportFunc<I_MODULE_NT, H_EtwRegisterClassicProvider>
#define I_EtwSendTraceBuffer ImportFunc<I_MODULE_NT, H_EtwSendTraceBuffer>
#define I_EtwUnregister ImportFunc<I_MODULE_NT, H_EtwUnregister>
#define I_EtwWrite ImportFunc<I_MODULE_NT, H_EtwWrite>
#define I_EtwWriteEndScenario ImportFunc<I_MODULE_NT, H_EtwWriteEndScenario>
#define I_EtwWriteEx ImportFunc<I_MODULE_NT, H_EtwWriteEx>
#define I_EtwWriteStartScenario ImportFunc<I_MODULE_NT, H_EtwWriteStartScenario>
#define I_EtwWriteString ImportFunc<I_MODULE_NT, H_EtwWriteString>
#define I_EtwWriteTransfer ImportFunc<I_MODULE_NT, H_EtwWriteTransfer>
#define I_ExAcquireCacheAwarePushLockExclusive ImportFunc<I_MODULE_NT, H_ExAcquireCacheAwarePushLockExclusive>
#define I_ExAcquireCacheAwarePushLockExclusiveEx ImportFunc<I_MODULE_NT, H_ExAcquireCacheAwarePushLockExclusiveEx>
#define I_ExAcquireCacheAwarePushLockSharedEx ImportFunc<I_MODULE_NT, H_ExAcquireCacheAwarePushLockSharedEx>
#define I_ExAcquireFastMutex ImportFunc<I_MODULE_NT, H_ExAcquireFastMutex>
#define I_ExAcquireFastMutexUnsafe ImportFunc<I_MODULE_NT, H_ExAcquireFastMutexUnsafe>
#define I_ExAcquirePushLockExclusiveEx ImportFunc<I_MODULE_NT, H_ExAcquirePushLockExclusiveEx>
#define I_ExAcquirePushLockSharedEx ImportFunc<I_MODULE_NT, H_ExAcquirePushLockSharedEx>
#define I_ExAcquireResourceExclusiveLite ImportFunc<I_MODULE_NT, H_ExAcquireResourceExclusiveLite>
#define I_ExAcquireResourceSharedLite ImportFunc<I_MODULE_NT, H_ExAcquireResourceSharedLite>
#define I_ExAcquireRundownProtection ImportFunc<I_MODULE_NT, H_ExAcquireRundownProtection>
#define I_ExAcquireRundownProtectionCacheAware ImportFunc<I_MODULE_NT, H_ExAcquireRundownProtectionCacheAware>
#define I_ExAcquireRundownProtectionCacheAwareEx ImportFunc<I_MODULE_NT, H_ExAcquireRundownProtectionCacheAwareEx>
#define I_ExAcquireRundownProtectionEx ImportFunc<I_MODULE_NT, H_ExAcquireRundownProtectionEx>
#define I_ExAcquireSharedStarveExclusive ImportFunc<I_MODULE_NT, H_ExAcquireSharedStarveExclusive>
#define I_ExAcquireSharedWaitForExclusive ImportFunc<I_MODULE_NT, H_ExAcquireSharedWaitForExclusive>
#define I_ExAcquireSpinLockExclusive ImportFunc<I_MODULE_NT, H_ExAcquireSpinLockExclusive>
#define I_ExAcquireSpinLockExclusiveAtDpcLevel ImportFunc<I_MODULE_NT, H_ExAcquireSpinLockExclusiveAtDpcLevel>
#define I_ExAcquireSpinLockShared ImportFunc<I_MODULE_NT, H_ExAcquireSpinLockShared>
#define I_ExAcquireSpinLockSharedAtDpcLevel ImportFunc<I_MODULE_NT, H_ExAcquireSpinLockSharedAtDpcLevel>
#define I_ExAllocateCacheAwarePushLock ImportFunc<I_MODULE_NT, H_ExAllocateCacheAwarePushLock>
#define I_ExAllocateCacheAwareRundownProtection ImportFunc<I_MODULE_NT, H_ExAllocateCacheAwareRundownProtection>
#define I_ExAllocatePool ImportFunc<I_MODULE_NT, H_ExAllocatePool>
#define I_ExAllocatePoolWithQuota ImportFunc<I_MODULE_NT, H_ExAllocatePoolWithQuota>
#define I_ExAllocatePoolWithQuotaTag ImportFunc<I_MODULE_NT, H_ExAllocatePoolWithQuotaTag>
#define I_ExAllocatePoolWithTag ImportFunc<I_MODULE_NT, H_ExAllocatePoolWithTag>
#define I_ExAllocatePoolWithTagPriority ImportFunc<I_MODULE_NT, H_ExAllocatePoolWithTagPriority>
#define I_ExAllocateTimer ImportFunc<I_MODULE_NT, H_ExAllocateTimer>
#define I_ExBlockOnAddressPushLock ImportFunc<I_MODULE_NT, H_ExBlockOnAddressPushLock>
#define I_ExBlockPushLock ImportFunc<I_MODULE_NT, H_ExBlockPushLock>
#define I_ExCancelTimer ImportFunc<I_MODULE_NT, H_ExCancelTimer>
#define I_ExCompositionObjectType ImportFunc<I_MODULE_NT, H_ExCompositionObjectType>
#define I_ExConvertExclusiveToSharedLite ImportFunc<I_MODULE_NT, H_ExConvertExclusiveToSharedLite>
#define I_ExCreateCallback ImportFunc<I_MODULE_NT, H_ExCreateCallback>
#define I_ExDeleteLookasideListEx ImportFunc<I_MODULE_NT, H_ExDeleteLookasideListEx>
#define I_ExDeleteNPagedLookasideList ImportFunc<I_MODULE_NT, H_ExDeleteNPagedLookasideList>
#define I_ExDeletePagedLookasideList ImportFunc<I_MODULE_NT, H_ExDeletePagedLookasideList>
#define I_ExDeleteResourceLite ImportFunc<I_MODULE_NT, H_ExDeleteResourceLite>
#define I_ExDeleteTimer ImportFunc<I_MODULE_NT, H_ExDeleteTimer>
#define I_ExDesktopObjectType ImportFunc<I_MODULE_NT, H_ExDesktopObjectType>
#define I_ExDisableResourceBoostLite ImportFunc<I_MODULE_NT, H_ExDisableResourceBoostLite>
#define I_ExEnterCriticalRegionAndAcquireFastMutexUnsafe ImportFunc<I_MODULE_NT, H_ExEnterCriticalRegionAndAcquireFastMutexUnsafe>
#define I_ExEnterCriticalRegionAndAcquireResourceExclusive ImportFunc<I_MODULE_NT, H_ExEnterCriticalRegionAndAcquireResourceExclusive>
#define I_ExEnterCriticalRegionAndAcquireResourceShared ImportFunc<I_MODULE_NT, H_ExEnterCriticalRegionAndAcquireResourceShared>
#define I_ExEnterCriticalRegionAndAcquireSharedWaitForExclusive ImportFunc<I_MODULE_NT, H_ExEnterCriticalRegionAndAcquireSharedWaitForExclusive>
#define I_ExEnterPriorityRegionAndAcquireResourceExclusive ImportFunc<I_MODULE_NT, H_ExEnterPriorityRegionAndAcquireResourceExclusive>
#define I_ExEnterPriorityRegionAndAcquireResourceShared ImportFunc<I_MODULE_NT, H_ExEnterPriorityRegionAndAcquireResourceShared>
#define I_ExEnumHandleTable ImportFunc<I_MODULE_NT, H_ExEnumHandleTable>
#define I_ExEventObjectType ImportFunc<I_MODULE_NT, H_ExEventObjectType>
#define I_ExExtendZone ImportFunc<I_MODULE_NT, H_ExExtendZone>
#define I_ExFetchLicenseData ImportFunc<I_MODULE_NT, H_ExFetchLicenseData>
#define I_ExFlushLookasideListEx ImportFunc<I_MODULE_NT, H_ExFlushLookasideListEx>
#define I_ExFreeCacheAwarePushLock ImportFunc<I_MODULE_NT, H_ExFreeCacheAwarePushLock>
#define I_ExFreeCacheAwareRundownProtection ImportFunc<I_MODULE_NT, H_ExFreeCacheAwareRundownProtection>
#define I_ExFreePool ImportFunc<I_MODULE_NT, H_ExFreePool>
#define I_ExFreePoolWithTag ImportFunc<I_MODULE_NT, H_ExFreePoolWithTag>
#define I_ExGetCurrentProcessorCounts ImportFunc<I_MODULE_NT, H_ExGetCurrentProcessorCounts>
#define I_ExGetCurrentProcessorCpuUsage ImportFunc<I_MODULE_NT, H_ExGetCurrentProcessorCpuUsage>
#define I_ExGetExclusiveWaiterCount ImportFunc<I_MODULE_NT, H_ExGetExclusiveWaiterCount>
#define I_ExGetFirmwareEnvironmentVariable ImportFunc<I_MODULE_NT, H_ExGetFirmwareEnvironmentVariable>
#define I_ExGetLicenseTamperState ImportFunc<I_MODULE_NT, H_ExGetLicenseTamperState>
#define I_ExGetPreviousMode ImportFunc<I_MODULE_NT, H_ExGetPreviousMode>
#define I_ExGetSharedWaiterCount ImportFunc<I_MODULE_NT, H_ExGetSharedWaiterCount>
#define I_ExInitializeLookasideListEx ImportFunc<I_MODULE_NT, H_ExInitializeLookasideListEx>
#define I_ExInitializeNPagedLookasideList ImportFunc<I_MODULE_NT, H_ExInitializeNPagedLookasideList>
#define I_ExInitializePagedLookasideList ImportFunc<I_MODULE_NT, H_ExInitializePagedLookasideList>
#define I_ExInitializePushLock ImportFunc<I_MODULE_NT, H_ExInitializePushLock>
#define I_ExInitializeResourceLite ImportFunc<I_MODULE_NT, H_ExInitializeResourceLite>
#define I_ExInitializeRundownProtection ImportFunc<I_MODULE_NT, H_ExInitializeRundownProtection>
#define I_ExInitializeRundownProtectionCacheAware ImportFunc<I_MODULE_NT, H_ExInitializeRundownProtectionCacheAware>
#define I_ExInitializeZone ImportFunc<I_MODULE_NT, H_ExInitializeZone>
#define I_ExInterlockedAddLargeInteger ImportFunc<I_MODULE_NT, H_ExInterlockedAddLargeInteger>
#define I_ExInterlockedAddUlong ImportFunc<I_MODULE_NT, H_ExInterlockedAddUlong>
#define I_ExInterlockedExtendZone ImportFunc<I_MODULE_NT, H_ExInterlockedExtendZone>
#define I_ExInterlockedInsertHeadList ImportFunc<I_MODULE_NT, H_ExInterlockedInsertHeadList>
#define I_ExInterlockedInsertTailList ImportFunc<I_MODULE_NT, H_ExInterlockedInsertTailList>
#define I_ExInterlockedPopEntryList ImportFunc<I_MODULE_NT, H_ExInterlockedPopEntryList>
#define I_ExInterlockedPushEntryList ImportFunc<I_MODULE_NT, H_ExInterlockedPushEntryList>
#define I_ExInterlockedRemoveHeadList ImportFunc<I_MODULE_NT, H_ExInterlockedRemoveHeadList>
#define I_ExIsProcessorFeaturePresent ImportFunc<I_MODULE_NT, H_ExIsProcessorFeaturePresent>
#define I_ExIsResourceAcquiredExclusiveLite ImportFunc<I_MODULE_NT, H_ExIsResourceAcquiredExclusiveLite>
#define I_ExIsResourceAcquiredSharedLite ImportFunc<I_MODULE_NT, H_ExIsResourceAcquiredSharedLite>
#define I_ExLocalTimeToSystemTime ImportFunc<I_MODULE_NT, H_ExLocalTimeToSystemTime>
#define I_ExNotifyBootDeviceRemoval ImportFunc<I_MODULE_NT, H_ExNotifyBootDeviceRemoval>
#define I_ExNotifyCallback ImportFunc<I_MODULE_NT, H_ExNotifyCallback>
#define I_ExQueryDepthSList ImportFunc<I_MODULE_NT, H_ExQueryDepthSList>
#define I_ExQueryFastCacheAppOrigin ImportFunc<I_MODULE_NT, H_ExQueryFastCacheAppOrigin>
#define I_ExQueryFastCacheDevLicense ImportFunc<I_MODULE_NT, H_ExQueryFastCacheDevLicense>
#define I_ExQueryPoolBlockSize ImportFunc<I_MODULE_NT, H_ExQueryPoolBlockSize>
#define I_ExQueryTimerResolution ImportFunc<I_MODULE_NT, H_ExQueryTimerResolution>
#define I_ExQueryWnfStateData ImportFunc<I_MODULE_NT, H_ExQueryWnfStateData>
#define I_ExQueueWorkItem ImportFunc<I_MODULE_NT, H_ExQueueWorkItem>
#define I_ExRaiseAccessViolation ImportFunc<I_MODULE_NT, H_ExRaiseAccessViolation>
#define I_ExRaiseDatatypeMisalignment ImportFunc<I_MODULE_NT, H_ExRaiseDatatypeMisalignment>
#define I_ExRaiseException ImportFunc<I_MODULE_NT, H_ExRaiseException>
#define I_ExRaiseHardError ImportFunc<I_MODULE_NT, H_ExRaiseHardError>
#define I_ExRaiseStatus ImportFunc<I_MODULE_NT, H_ExRaiseStatus>
#define I_ExReInitializeRundownProtection ImportFunc<I_MODULE_NT, H_ExReInitializeRundownProtection>
#define I_ExReInitializeRundownProtectionCacheAware ImportFunc<I_MODULE_NT, H_ExReInitializeRundownProtectionCacheAware>
#define I_ExRealTimeIsUniversal ImportFunc<I_MODULE_NT, H_ExRealTimeIsUniversal>
#define I_ExRegisterBootDevice ImportFunc<I_MODULE_NT, H_ExRegisterBootDevice>
#define I_ExRegisterCallback ImportFunc<I_MODULE_NT, H_ExRegisterCallback>
#define I_ExRegisterExtension ImportFunc<I_MODULE_NT, H_ExRegisterExtension>
#define I_ExReinitializeResourceLite ImportFunc<I_MODULE_NT, H_ExReinitializeResourceLite>
#define I_ExReleaseCacheAwarePushLockExclusive ImportFunc<I_MODULE_NT, H_ExReleaseCacheAwarePushLockExclusive>
#define I_ExReleaseCacheAwarePushLockExclusiveEx ImportFunc<I_MODULE_NT, H_ExReleaseCacheAwarePushLockExclusiveEx>
#define I_ExReleaseCacheAwarePushLockSharedEx ImportFunc<I_MODULE_NT, H_ExReleaseCacheAwarePushLockSharedEx>
#define I_ExReleaseFastMutex ImportFunc<I_MODULE_NT, H_ExReleaseFastMutex>
#define I_ExReleaseFastMutexUnsafe ImportFunc<I_MODULE_NT, H_ExReleaseFastMutexUnsafe>
#define I_ExReleaseFastMutexUnsafeAndLeaveCriticalRegion ImportFunc<I_MODULE_NT, H_ExReleaseFastMutexUnsafeAndLeaveCriticalRegion>
#define I_ExReleasePushLockEx ImportFunc<I_MODULE_NT, H_ExReleasePushLockEx>
#define I_ExReleasePushLockExclusiveEx ImportFunc<I_MODULE_NT, H_ExReleasePushLockExclusiveEx>
#define I_ExReleasePushLockSharedEx ImportFunc<I_MODULE_NT, H_ExReleasePushLockSharedEx>
#define I_ExReleaseResourceAndLeaveCriticalRegion ImportFunc<I_MODULE_NT, H_ExReleaseResourceAndLeaveCriticalRegion>
#define I_ExReleaseResourceAndLeavePriorityRegion ImportFunc<I_MODULE_NT, H_ExReleaseResourceAndLeavePriorityRegion>
#define I_ExReleaseResourceForThreadLite ImportFunc<I_MODULE_NT, H_ExReleaseResourceForThreadLite>
#define I_ExReleaseResourceLite ImportFunc<I_MODULE_NT, H_ExReleaseResourceLite>
#define I_ExReleaseRundownProtection ImportFunc<I_MODULE_NT, H_ExReleaseRundownProtection>
#define I_ExReleaseRundownProtectionCacheAware ImportFunc<I_MODULE_NT, H_ExReleaseRundownProtectionCacheAware>
#define I_ExReleaseRundownProtectionCacheAwareEx ImportFunc<I_MODULE_NT, H_ExReleaseRundownProtectionCacheAwareEx>
#define I_ExReleaseRundownProtectionEx ImportFunc<I_MODULE_NT, H_ExReleaseRundownProtectionEx>
#define I_ExReleaseSpinLockExclusive ImportFunc<I_MODULE_NT, H_ExReleaseSpinLockExclusive>
#define I_ExReleaseSpinLockExclusiveFromDpcLevel ImportFunc<I_MODULE_NT, H_ExReleaseSpinLockExclusiveFromDpcLevel>
#define I_ExReleaseSpinLockShared ImportFunc<I_MODULE_NT, H_ExReleaseSpinLockShared>
#define I_ExReleaseSpinLockSharedFromDpcLevel ImportFunc<I_MODULE_NT, H_ExReleaseSpinLockSharedFromDpcLevel>
#define I_ExRundownCompleted ImportFunc<I_MODULE_NT, H_ExRundownCompleted>
#define I_ExRundownCompletedCacheAware ImportFunc<I_MODULE_NT, H_ExRundownCompletedCacheAware>
#define I_ExSemaphoreObjectType ImportFunc<I_MODULE_NT, H_ExSemaphoreObjectType>
#define I_ExSetFirmwareEnvironmentVariable ImportFunc<I_MODULE_NT, H_ExSetFirmwareEnvironmentVariable>
#define I_ExSetLicenseTamperState ImportFunc<I_MODULE_NT, H_ExSetLicenseTamperState>
#define I_ExSetResourceOwnerPointer ImportFunc<I_MODULE_NT, H_ExSetResourceOwnerPointer>
#define I_ExSetResourceOwnerPointerEx ImportFunc<I_MODULE_NT, H_ExSetResourceOwnerPointerEx>
#define I_ExSetTimer ImportFunc<I_MODULE_NT, H_ExSetTimer>
#define I_ExSetTimerResolution ImportFunc<I_MODULE_NT, H_ExSetTimerResolution>
#define I_ExSizeOfRundownProtectionCacheAware ImportFunc<I_MODULE_NT, H_ExSizeOfRundownProtectionCacheAware>
#define I_ExSubscribeWnfStateChange ImportFunc<I_MODULE_NT, H_ExSubscribeWnfStateChange>
#define I_ExSystemExceptionFilter ImportFunc<I_MODULE_NT, H_ExSystemExceptionFilter>
#define I_ExSystemTimeToLocalTime ImportFunc<I_MODULE_NT, H_ExSystemTimeToLocalTime>
#define I_ExTimedWaitForUnblockPushLock ImportFunc<I_MODULE_NT, H_ExTimedWaitForUnblockPushLock>
#define I_ExTryAcquirePushLockExclusiveEx ImportFunc<I_MODULE_NT, H_ExTryAcquirePushLockExclusiveEx>
#define I_ExTryAcquirePushLockSharedEx ImportFunc<I_MODULE_NT, H_ExTryAcquirePushLockSharedEx>
#define I_ExTryConvertPushLockSharedToExclusiveEx ImportFunc<I_MODULE_NT, H_ExTryConvertPushLockSharedToExclusiveEx>
#define I_ExTryConvertSharedSpinLockExclusive ImportFunc<I_MODULE_NT, H_ExTryConvertSharedSpinLockExclusive>
#define I_ExTryQueueWorkItem ImportFunc<I_MODULE_NT, H_ExTryQueueWorkItem>
#define I_ExTryToAcquireFastMutex ImportFunc<I_MODULE_NT, H_ExTryToAcquireFastMutex>
#define I_ExTryToAcquireResourceExclusiveLite ImportFunc<I_MODULE_NT, H_ExTryToAcquireResourceExclusiveLite>
#define I_ExUnblockOnAddressPushLockEx ImportFunc<I_MODULE_NT, H_ExUnblockOnAddressPushLockEx>
#define I_ExUnblockPushLockEx ImportFunc<I_MODULE_NT, H_ExUnblockPushLockEx>
#define I_ExUnregisterCallback ImportFunc<I_MODULE_NT, H_ExUnregisterCallback>
#define I_ExUnregisterExtension ImportFunc<I_MODULE_NT, H_ExUnregisterExtension>
#define I_ExUnsubscribeWnfStateChange ImportFunc<I_MODULE_NT, H_ExUnsubscribeWnfStateChange>
#define I_ExUuidCreate ImportFunc<I_MODULE_NT, H_ExUuidCreate>
#define I_ExVerifySuite ImportFunc<I_MODULE_NT, H_ExVerifySuite>
#define I_ExWaitForRundownProtectionRelease ImportFunc<I_MODULE_NT, H_ExWaitForRundownProtectionRelease>
#define I_ExWaitForRundownProtectionReleaseCacheAware ImportFunc<I_MODULE_NT, H_ExWaitForRundownProtectionReleaseCacheAware>
#define I_ExWaitForUnblockPushLock ImportFunc<I_MODULE_NT, H_ExWaitForUnblockPushLock>
#define I_ExWindowStationObjectType ImportFunc<I_MODULE_NT, H_ExWindowStationObjectType>
#define I_ExfAcquirePushLockExclusive ImportFunc<I_MODULE_NT, H_ExfAcquirePushLockExclusive>
#define I_ExfAcquirePushLockShared ImportFunc<I_MODULE_NT, H_ExfAcquirePushLockShared>
#define I_ExfReleasePushLock ImportFunc<I_MODULE_NT, H_ExfReleasePushLock>
#define I_ExfReleasePushLockExclusive ImportFunc<I_MODULE_NT, H_ExfReleasePushLockExclusive>
#define I_ExfReleasePushLockShared ImportFunc<I_MODULE_NT, H_ExfReleasePushLockShared>
#define I_ExfTryAcquirePushLockShared ImportFunc<I_MODULE_NT, H_ExfTryAcquirePushLockShared>
#define I_ExfTryToWakePushLock ImportFunc<I_MODULE_NT, H_ExfTryToWakePushLock>
#define I_ExfUnblockPushLock ImportFunc<I_MODULE_NT, H_ExfUnblockPushLock>
#define I_ExpInterlockedFlushSList ImportFunc<I_MODULE_NT, H_ExpInterlockedFlushSList>
#define I_ExpInterlockedPopEntrySList ImportFunc<I_MODULE_NT, H_ExpInterlockedPopEntrySList>
#define I_ExpInterlockedPushEntrySList ImportFunc<I_MODULE_NT, H_ExpInterlockedPushEntrySList>
#define I_FirstEntrySList ImportFunc<I_MODULE_NT, H_FirstEntrySList>
#define I_FsRtlAcknowledgeEcp ImportFunc<I_MODULE_NT, H_FsRtlAcknowledgeEcp>
#define I_FsRtlAcquireEofLock ImportFunc<I_MODULE_NT, H_FsRtlAcquireEofLock>
#define I_FsRtlAcquireFileExclusive ImportFunc<I_MODULE_NT, H_FsRtlAcquireFileExclusive>
#define I_FsRtlAcquireHeaderMutex ImportFunc<I_MODULE_NT, H_FsRtlAcquireHeaderMutex>
#define I_FsRtlAddBaseMcbEntry ImportFunc<I_MODULE_NT, H_FsRtlAddBaseMcbEntry>
#define I_FsRtlAddBaseMcbEntryEx ImportFunc<I_MODULE_NT, H_FsRtlAddBaseMcbEntryEx>
#define I_FsRtlAddLargeMcbEntry ImportFunc<I_MODULE_NT, H_FsRtlAddLargeMcbEntry>
#define I_FsRtlAddMcbEntry ImportFunc<I_MODULE_NT, H_FsRtlAddMcbEntry>
#define I_FsRtlAddToTunnelCache ImportFunc<I_MODULE_NT, H_FsRtlAddToTunnelCache>
#define I_FsRtlAllocateExtraCreateParameter ImportFunc<I_MODULE_NT, H_FsRtlAllocateExtraCreateParameter>
#define I_FsRtlAllocateExtraCreateParameterFromLookasideList ImportFunc<I_MODULE_NT, H_FsRtlAllocateExtraCreateParameterFromLookasideList>
#define I_FsRtlAllocateExtraCreateParameterList ImportFunc<I_MODULE_NT, H_FsRtlAllocateExtraCreateParameterList>
#define I_FsRtlAllocateFileLock ImportFunc<I_MODULE_NT, H_FsRtlAllocateFileLock>
#define I_FsRtlAllocatePool ImportFunc<I_MODULE_NT, H_FsRtlAllocatePool>
#define I_FsRtlAllocatePoolWithQuota ImportFunc<I_MODULE_NT, H_FsRtlAllocatePoolWithQuota>
#define I_FsRtlAllocatePoolWithQuotaTag ImportFunc<I_MODULE_NT, H_FsRtlAllocatePoolWithQuotaTag>
#define I_FsRtlAllocatePoolWithTag ImportFunc<I_MODULE_NT, H_FsRtlAllocatePoolWithTag>
#define I_FsRtlAllocateResource ImportFunc<I_MODULE_NT, H_FsRtlAllocateResource>
#define I_FsRtlAreNamesEqual ImportFunc<I_MODULE_NT, H_FsRtlAreNamesEqual>
#define I_FsRtlAreThereCurrentOrInProgressFileLocks ImportFunc<I_MODULE_NT, H_FsRtlAreThereCurrentOrInProgressFileLocks>
#define I_FsRtlAreThereWaitingFileLocks ImportFunc<I_MODULE_NT, H_FsRtlAreThereWaitingFileLocks>
#define I_FsRtlAreVolumeStartupApplicationsComplete ImportFunc<I_MODULE_NT, H_FsRtlAreVolumeStartupApplicationsComplete>
#define I_FsRtlBalanceReads ImportFunc<I_MODULE_NT, H_FsRtlBalanceReads>
#define I_FsRtlCancellableWaitForMultipleObjects ImportFunc<I_MODULE_NT, H_FsRtlCancellableWaitForMultipleObjects>
#define I_FsRtlCancellableWaitForSingleObject ImportFunc<I_MODULE_NT, H_FsRtlCancellableWaitForSingleObject>
#define I_FsRtlChangeBackingFileObject ImportFunc<I_MODULE_NT, H_FsRtlChangeBackingFileObject>
#define I_FsRtlCheckLockForOplockRequest ImportFunc<I_MODULE_NT, H_FsRtlCheckLockForOplockRequest>
#define I_FsRtlCheckLockForReadAccess ImportFunc<I_MODULE_NT, H_FsRtlCheckLockForReadAccess>
#define I_FsRtlCheckLockForWriteAccess ImportFunc<I_MODULE_NT, H_FsRtlCheckLockForWriteAccess>
#define I_FsRtlCheckOplock ImportFunc<I_MODULE_NT, H_FsRtlCheckOplock>
#define I_FsRtlCheckOplockEx ImportFunc<I_MODULE_NT, H_FsRtlCheckOplockEx>
#define I_FsRtlCheckUpperOplock ImportFunc<I_MODULE_NT, H_FsRtlCheckUpperOplock>
#define I_FsRtlCopyRead ImportFunc<I_MODULE_NT, H_FsRtlCopyRead>
#define I_FsRtlCopyWrite ImportFunc<I_MODULE_NT, H_FsRtlCopyWrite>
#define I_FsRtlCreateSectionForDataScan ImportFunc<I_MODULE_NT, H_FsRtlCreateSectionForDataScan>
#define I_FsRtlCurrentBatchOplock ImportFunc<I_MODULE_NT, H_FsRtlCurrentBatchOplock>
#define I_FsRtlCurrentOplock ImportFunc<I_MODULE_NT, H_FsRtlCurrentOplock>
#define I_FsRtlCurrentOplockH ImportFunc<I_MODULE_NT, H_FsRtlCurrentOplockH>
#define I_FsRtlDeleteExtraCreateParameterLookasideList ImportFunc<I_MODULE_NT, H_FsRtlDeleteExtraCreateParameterLookasideList>
#define I_FsRtlDeleteKeyFromTunnelCache ImportFunc<I_MODULE_NT, H_FsRtlDeleteKeyFromTunnelCache>
#define I_FsRtlDeleteTunnelCache ImportFunc<I_MODULE_NT, H_FsRtlDeleteTunnelCache>
#define I_FsRtlDeregisterUncProvider ImportFunc<I_MODULE_NT, H_FsRtlDeregisterUncProvider>
#define I_FsRtlDismountComplete ImportFunc<I_MODULE_NT, H_FsRtlDismountComplete>
#define I_FsRtlDissectDbcs ImportFunc<I_MODULE_NT, H_FsRtlDissectDbcs>
#define I_FsRtlDissectName ImportFunc<I_MODULE_NT, H_FsRtlDissectName>
#define I_FsRtlDoesDbcsContainWildCards ImportFunc<I_MODULE_NT, H_FsRtlDoesDbcsContainWildCards>
#define I_FsRtlDoesNameContainWildCards ImportFunc<I_MODULE_NT, H_FsRtlDoesNameContainWildCards>
#define I_FsRtlFastCheckLockForRead ImportFunc<I_MODULE_NT, H_FsRtlFastCheckLockForRead>
#define I_FsRtlFastCheckLockForWrite ImportFunc<I_MODULE_NT, H_FsRtlFastCheckLockForWrite>
#define I_FsRtlFastUnlockAll ImportFunc<I_MODULE_NT, H_FsRtlFastUnlockAll>
#define I_FsRtlFastUnlockAllByKey ImportFunc<I_MODULE_NT, H_FsRtlFastUnlockAllByKey>
#define I_FsRtlFastUnlockSingle ImportFunc<I_MODULE_NT, H_FsRtlFastUnlockSingle>
#define I_FsRtlFindExtraCreateParameter ImportFunc<I_MODULE_NT, H_FsRtlFindExtraCreateParameter>
#define I_FsRtlFindInTunnelCache ImportFunc<I_MODULE_NT, H_FsRtlFindInTunnelCache>
#define I_FsRtlFreeExtraCreateParameter ImportFunc<I_MODULE_NT, H_FsRtlFreeExtraCreateParameter>
#define I_FsRtlFreeExtraCreateParameterList ImportFunc<I_MODULE_NT, H_FsRtlFreeExtraCreateParameterList>
#define I_FsRtlFreeFileLock ImportFunc<I_MODULE_NT, H_FsRtlFreeFileLock>
#define I_FsRtlGetEcpListFromIrp ImportFunc<I_MODULE_NT, H_FsRtlGetEcpListFromIrp>
#define I_FsRtlGetFileNameInformation ImportFunc<I_MODULE_NT, H_FsRtlGetFileNameInformation>
#define I_FsRtlGetFileSize ImportFunc<I_MODULE_NT, H_FsRtlGetFileSize>
#define I_FsRtlGetIoAtEof ImportFunc<I_MODULE_NT, H_FsRtlGetIoAtEof>
#define I_FsRtlGetNextBaseMcbEntry ImportFunc<I_MODULE_NT, H_FsRtlGetNextBaseMcbEntry>
#define I_FsRtlGetNextExtraCreateParameter ImportFunc<I_MODULE_NT, H_FsRtlGetNextExtraCreateParameter>
#define I_FsRtlGetNextFileLock ImportFunc<I_MODULE_NT, H_FsRtlGetNextFileLock>
#define I_FsRtlGetNextLargeMcbEntry ImportFunc<I_MODULE_NT, H_FsRtlGetNextLargeMcbEntry>
#define I_FsRtlGetNextMcbEntry ImportFunc<I_MODULE_NT, H_FsRtlGetNextMcbEntry>
#define I_FsRtlGetSectorSizeInformation ImportFunc<I_MODULE_NT, H_FsRtlGetSectorSizeInformation>
#define I_FsRtlGetSupportedFeatures ImportFunc<I_MODULE_NT, H_FsRtlGetSupportedFeatures>
#define I_FsRtlGetVirtualDiskNestingLevel ImportFunc<I_MODULE_NT, H_FsRtlGetVirtualDiskNestingLevel>
#define I_FsRtlHeatInit ImportFunc<I_MODULE_NT, H_FsRtlHeatInit>
#define I_FsRtlHeatLogIo ImportFunc<I_MODULE_NT, H_FsRtlHeatLogIo>
#define I_FsRtlHeatLogTierMove ImportFunc<I_MODULE_NT, H_FsRtlHeatLogTierMove>
#define I_FsRtlHeatUninit ImportFunc<I_MODULE_NT, H_FsRtlHeatUninit>
#define I_FsRtlIncrementCcFastMdlReadWait ImportFunc<I_MODULE_NT, H_FsRtlIncrementCcFastMdlReadWait>
#define I_FsRtlIncrementCcFastReadNoWait ImportFunc<I_MODULE_NT, H_FsRtlIncrementCcFastReadNoWait>
#define I_FsRtlIncrementCcFastReadNotPossible ImportFunc<I_MODULE_NT, H_FsRtlIncrementCcFastReadNotPossible>
#define I_FsRtlIncrementCcFastReadResourceMiss ImportFunc<I_MODULE_NT, H_FsRtlIncrementCcFastReadResourceMiss>
#define I_FsRtlIncrementCcFastReadWait ImportFunc<I_MODULE_NT, H_FsRtlIncrementCcFastReadWait>
#define I_FsRtlInitExtraCreateParameterLookasideList ImportFunc<I_MODULE_NT, H_FsRtlInitExtraCreateParameterLookasideList>
#define I_FsRtlInitializeBaseMcb ImportFunc<I_MODULE_NT, H_FsRtlInitializeBaseMcb>
#define I_FsRtlInitializeBaseMcbEx ImportFunc<I_MODULE_NT, H_FsRtlInitializeBaseMcbEx>
#define I_FsRtlInitializeEofLock ImportFunc<I_MODULE_NT, H_FsRtlInitializeEofLock>
#define I_FsRtlInitializeExtraCreateParameter ImportFunc<I_MODULE_NT, H_FsRtlInitializeExtraCreateParameter>
#define I_FsRtlInitializeExtraCreateParameterList ImportFunc<I_MODULE_NT, H_FsRtlInitializeExtraCreateParameterList>
#define I_FsRtlInitializeFileLock ImportFunc<I_MODULE_NT, H_FsRtlInitializeFileLock>
#define I_FsRtlInitializeLargeMcb ImportFunc<I_MODULE_NT, H_FsRtlInitializeLargeMcb>
#define I_FsRtlInitializeMcb ImportFunc<I_MODULE_NT, H_FsRtlInitializeMcb>
#define I_FsRtlInitializeOplock ImportFunc<I_MODULE_NT, H_FsRtlInitializeOplock>
#define I_FsRtlInitializeTunnelCache ImportFunc<I_MODULE_NT, H_FsRtlInitializeTunnelCache>
#define I_FsRtlInsertExtraCreateParameter ImportFunc<I_MODULE_NT, H_FsRtlInsertExtraCreateParameter>
#define I_FsRtlInsertPerFileContext ImportFunc<I_MODULE_NT, H_FsRtlInsertPerFileContext>
#define I_FsRtlInsertPerFileObjectContext ImportFunc<I_MODULE_NT, H_FsRtlInsertPerFileObjectContext>
#define I_FsRtlInsertPerStreamContext ImportFunc<I_MODULE_NT, H_FsRtlInsertPerStreamContext>
#define I_FsRtlInsertReservedPerFileContext ImportFunc<I_MODULE_NT, H_FsRtlInsertReservedPerFileContext>
#define I_FsRtlInsertReservedPerStreamContext ImportFunc<I_MODULE_NT, H_FsRtlInsertReservedPerStreamContext>
#define I_FsRtlIsDbcsInExpression ImportFunc<I_MODULE_NT, H_FsRtlIsDbcsInExpression>
#define I_FsRtlIsEcpAcknowledged ImportFunc<I_MODULE_NT, H_FsRtlIsEcpAcknowledged>
#define I_FsRtlIsEcpFromUserMode ImportFunc<I_MODULE_NT, H_FsRtlIsEcpFromUserMode>
#define I_FsRtlIsFatDbcsLegal ImportFunc<I_MODULE_NT, H_FsRtlIsFatDbcsLegal>
#define I_FsRtlIsHpfsDbcsLegal ImportFunc<I_MODULE_NT, H_FsRtlIsHpfsDbcsLegal>
#define I_FsRtlIsNameInExpression ImportFunc<I_MODULE_NT, H_FsRtlIsNameInExpression>
#define I_FsRtlIsNtstatusExpected ImportFunc<I_MODULE_NT, H_FsRtlIsNtstatusExpected>
#define I_FsRtlIsPagingFile ImportFunc<I_MODULE_NT, H_FsRtlIsPagingFile>
#define I_FsRtlIsSystemPagingFile ImportFunc<I_MODULE_NT, H_FsRtlIsSystemPagingFile>
#define I_FsRtlIsTotalDeviceFailure ImportFunc<I_MODULE_NT, H_FsRtlIsTotalDeviceFailure>
#define I_FsRtlIssueDeviceIoControl ImportFunc<I_MODULE_NT, H_FsRtlIssueDeviceIoControl>
#define I_FsRtlKernelFsControlFile ImportFunc<I_MODULE_NT, H_FsRtlKernelFsControlFile>
#define I_FsRtlLegalAnsiCharacterArray ImportFunc<I_MODULE_NT, H_FsRtlLegalAnsiCharacterArray>
#define I_FsRtlLogCcFlushError ImportFunc<I_MODULE_NT, H_FsRtlLogCcFlushError>
#define I_FsRtlLookupBaseMcbEntry ImportFunc<I_MODULE_NT, H_FsRtlLookupBaseMcbEntry>
#define I_FsRtlLookupLargeMcbEntry ImportFunc<I_MODULE_NT, H_FsRtlLookupLargeMcbEntry>
#define I_FsRtlLookupLastBaseMcbEntry ImportFunc<I_MODULE_NT, H_FsRtlLookupLastBaseMcbEntry>
#define I_FsRtlLookupLastBaseMcbEntryAndIndex ImportFunc<I_MODULE_NT, H_FsRtlLookupLastBaseMcbEntryAndIndex>
#define I_FsRtlLookupLastLargeMcbEntry ImportFunc<I_MODULE_NT, H_FsRtlLookupLastLargeMcbEntry>
#define I_FsRtlLookupLastLargeMcbEntryAndIndex ImportFunc<I_MODULE_NT, H_FsRtlLookupLastLargeMcbEntryAndIndex>
#define I_FsRtlLookupLastMcbEntry ImportFunc<I_MODULE_NT, H_FsRtlLookupLastMcbEntry>
#define I_FsRtlLookupMcbEntry ImportFunc<I_MODULE_NT, H_FsRtlLookupMcbEntry>
#define I_FsRtlLookupPerFileContext ImportFunc<I_MODULE_NT, H_FsRtlLookupPerFileContext>
#define I_FsRtlLookupPerFileObjectContext ImportFunc<I_MODULE_NT, H_FsRtlLookupPerFileObjectContext>
#define I_FsRtlLookupPerStreamContextInternal ImportFunc<I_MODULE_NT, H_FsRtlLookupPerStreamContextInternal>
#define I_FsRtlLookupReservedPerFileContext ImportFunc<I_MODULE_NT, H_FsRtlLookupReservedPerFileContext>
#define I_FsRtlLookupReservedPerStreamContext ImportFunc<I_MODULE_NT, H_FsRtlLookupReservedPerStreamContext>
#define I_FsRtlMdlRead ImportFunc<I_MODULE_NT, H_FsRtlMdlRead>
#define I_FsRtlMdlReadComplete ImportFunc<I_MODULE_NT, H_FsRtlMdlReadComplete>
#define I_FsRtlMdlReadCompleteDev ImportFunc<I_MODULE_NT, H_FsRtlMdlReadCompleteDev>
#define I_FsRtlMdlReadDev ImportFunc<I_MODULE_NT, H_FsRtlMdlReadDev>
#define I_FsRtlMdlReadEx ImportFunc<I_MODULE_NT, H_FsRtlMdlReadEx>
#define I_FsRtlMdlWriteComplete ImportFunc<I_MODULE_NT, H_FsRtlMdlWriteComplete>
#define I_FsRtlMdlWriteCompleteDev ImportFunc<I_MODULE_NT, H_FsRtlMdlWriteCompleteDev>
#define I_FsRtlMupGetProviderIdFromName ImportFunc<I_MODULE_NT, H_FsRtlMupGetProviderIdFromName>
#define I_FsRtlMupGetProviderInfoFromFileObject ImportFunc<I_MODULE_NT, H_FsRtlMupGetProviderInfoFromFileObject>
#define I_FsRtlNormalizeNtstatus ImportFunc<I_MODULE_NT, H_FsRtlNormalizeNtstatus>
#define I_FsRtlNotifyChangeDirectory ImportFunc<I_MODULE_NT, H_FsRtlNotifyChangeDirectory>
#define I_FsRtlNotifyCleanup ImportFunc<I_MODULE_NT, H_FsRtlNotifyCleanup>
#define I_FsRtlNotifyCleanupAll ImportFunc<I_MODULE_NT, H_FsRtlNotifyCleanupAll>
#define I_FsRtlNotifyFilterChangeDirectory ImportFunc<I_MODULE_NT, H_FsRtlNotifyFilterChangeDirectory>
#define I_FsRtlNotifyFilterReportChange ImportFunc<I_MODULE_NT, H_FsRtlNotifyFilterReportChange>
#define I_FsRtlNotifyFullChangeDirectory ImportFunc<I_MODULE_NT, H_FsRtlNotifyFullChangeDirectory>
#define I_FsRtlNotifyFullReportChange ImportFunc<I_MODULE_NT, H_FsRtlNotifyFullReportChange>
#define I_FsRtlNotifyInitializeSync ImportFunc<I_MODULE_NT, H_FsRtlNotifyInitializeSync>
#define I_FsRtlNotifyReportChange ImportFunc<I_MODULE_NT, H_FsRtlNotifyReportChange>
#define I_FsRtlNotifyUninitializeSync ImportFunc<I_MODULE_NT, H_FsRtlNotifyUninitializeSync>
#define I_FsRtlNotifyVolumeEvent ImportFunc<I_MODULE_NT, H_FsRtlNotifyVolumeEvent>
#define I_FsRtlNotifyVolumeEventEx ImportFunc<I_MODULE_NT, H_FsRtlNotifyVolumeEventEx>
#define I_FsRtlNumberOfRunsInBaseMcb ImportFunc<I_MODULE_NT, H_FsRtlNumberOfRunsInBaseMcb>
#define I_FsRtlNumberOfRunsInLargeMcb ImportFunc<I_MODULE_NT, H_FsRtlNumberOfRunsInLargeMcb>
#define I_FsRtlNumberOfRunsInMcb ImportFunc<I_MODULE_NT, H_FsRtlNumberOfRunsInMcb>
#define I_FsRtlOplockBreakH ImportFunc<I_MODULE_NT, H_FsRtlOplockBreakH>
#define I_FsRtlOplockBreakToNone ImportFunc<I_MODULE_NT, H_FsRtlOplockBreakToNone>
#define I_FsRtlOplockBreakToNoneEx ImportFunc<I_MODULE_NT, H_FsRtlOplockBreakToNoneEx>
#define I_FsRtlOplockFsctrl ImportFunc<I_MODULE_NT, H_FsRtlOplockFsctrl>
#define I_FsRtlOplockFsctrlEx ImportFunc<I_MODULE_NT, H_FsRtlOplockFsctrlEx>
#define I_FsRtlOplockIsFastIoPossible ImportFunc<I_MODULE_NT, H_FsRtlOplockIsFastIoPossible>
#define I_FsRtlOplockIsSharedRequest ImportFunc<I_MODULE_NT, H_FsRtlOplockIsSharedRequest>
#define I_FsRtlOplockKeysEqual ImportFunc<I_MODULE_NT, H_FsRtlOplockKeysEqual>
#define I_FsRtlPostPagingFileStackOverflow ImportFunc<I_MODULE_NT, H_FsRtlPostPagingFileStackOverflow>
#define I_FsRtlPostStackOverflow ImportFunc<I_MODULE_NT, H_FsRtlPostStackOverflow>
#define I_FsRtlPrepareMdlWrite ImportFunc<I_MODULE_NT, H_FsRtlPrepareMdlWrite>
#define I_FsRtlPrepareMdlWriteDev ImportFunc<I_MODULE_NT, H_FsRtlPrepareMdlWriteDev>
#define I_FsRtlPrepareMdlWriteEx ImportFunc<I_MODULE_NT, H_FsRtlPrepareMdlWriteEx>
#define I_FsRtlPrepareToReuseEcp ImportFunc<I_MODULE_NT, H_FsRtlPrepareToReuseEcp>
#define I_FsRtlPrivateLock ImportFunc<I_MODULE_NT, H_FsRtlPrivateLock>
#define I_FsRtlProcessFileLock ImportFunc<I_MODULE_NT, H_FsRtlProcessFileLock>
#define I_FsRtlQueryCachedVdl ImportFunc<I_MODULE_NT, H_FsRtlQueryCachedVdl>
#define I_FsRtlQueryKernelEaFile ImportFunc<I_MODULE_NT, H_FsRtlQueryKernelEaFile>
#define I_FsRtlQueryMaximumVirtualDiskNestingLevel ImportFunc<I_MODULE_NT, H_FsRtlQueryMaximumVirtualDiskNestingLevel>
#define I_FsRtlRegisterFileSystemFilterCallbacks ImportFunc<I_MODULE_NT, H_FsRtlRegisterFileSystemFilterCallbacks>
#define I_FsRtlRegisterFltMgrCalls ImportFunc<I_MODULE_NT, H_FsRtlRegisterFltMgrCalls>
#define I_FsRtlRegisterMupCalls ImportFunc<I_MODULE_NT, H_FsRtlRegisterMupCalls>
#define I_FsRtlRegisterUncProvider ImportFunc<I_MODULE_NT, H_FsRtlRegisterUncProvider>
#define I_FsRtlRegisterUncProviderEx ImportFunc<I_MODULE_NT, H_FsRtlRegisterUncProviderEx>
#define I_FsRtlReleaseEofLock ImportFunc<I_MODULE_NT, H_FsRtlReleaseEofLock>
#define I_FsRtlReleaseFile ImportFunc<I_MODULE_NT, H_FsRtlReleaseFile>
#define I_FsRtlReleaseFileNameInformation ImportFunc<I_MODULE_NT, H_FsRtlReleaseFileNameInformation>
#define I_FsRtlReleaseHeaderMutex ImportFunc<I_MODULE_NT, H_FsRtlReleaseHeaderMutex>
#define I_FsRtlRemoveBaseMcbEntry ImportFunc<I_MODULE_NT, H_FsRtlRemoveBaseMcbEntry>
#define I_FsRtlRemoveDotsFromPath ImportFunc<I_MODULE_NT, H_FsRtlRemoveDotsFromPath>
#define I_FsRtlRemoveExtraCreateParameter ImportFunc<I_MODULE_NT, H_FsRtlRemoveExtraCreateParameter>
#define I_FsRtlRemoveLargeMcbEntry ImportFunc<I_MODULE_NT, H_FsRtlRemoveLargeMcbEntry>
#define I_FsRtlRemoveMcbEntry ImportFunc<I_MODULE_NT, H_FsRtlRemoveMcbEntry>
#define I_FsRtlRemovePerFileContext ImportFunc<I_MODULE_NT, H_FsRtlRemovePerFileContext>
#define I_FsRtlRemovePerFileObjectContext ImportFunc<I_MODULE_NT, H_FsRtlRemovePerFileObjectContext>
#define I_FsRtlRemovePerStreamContext ImportFunc<I_MODULE_NT, H_FsRtlRemovePerStreamContext>
#define I_FsRtlRemoveReservedPerFileContext ImportFunc<I_MODULE_NT, H_FsRtlRemoveReservedPerFileContext>
#define I_FsRtlRemoveReservedPerStreamContext ImportFunc<I_MODULE_NT, H_FsRtlRemoveReservedPerStreamContext>
#define I_FsRtlResetBaseMcb ImportFunc<I_MODULE_NT, H_FsRtlResetBaseMcb>
#define I_FsRtlResetLargeMcb ImportFunc<I_MODULE_NT, H_FsRtlResetLargeMcb>
#define I_FsRtlSendModernAppTermination ImportFunc<I_MODULE_NT, H_FsRtlSendModernAppTermination>
#define I_FsRtlSetEcpListIntoIrp ImportFunc<I_MODULE_NT, H_FsRtlSetEcpListIntoIrp>
#define I_FsRtlSetKernelEaFile ImportFunc<I_MODULE_NT, H_FsRtlSetKernelEaFile>
#define I_FsRtlSplitBaseMcb ImportFunc<I_MODULE_NT, H_FsRtlSplitBaseMcb>
#define I_FsRtlSplitLargeMcb ImportFunc<I_MODULE_NT, H_FsRtlSplitLargeMcb>
#define I_FsRtlSyncVolumes ImportFunc<I_MODULE_NT, H_FsRtlSyncVolumes>
#define I_FsRtlTeardownPerFileContexts ImportFunc<I_MODULE_NT, H_FsRtlTeardownPerFileContexts>
#define I_FsRtlTeardownPerStreamContexts ImportFunc<I_MODULE_NT, H_FsRtlTeardownPerStreamContexts>
#define I_FsRtlTruncateBaseMcb ImportFunc<I_MODULE_NT, H_FsRtlTruncateBaseMcb>
#define I_FsRtlTruncateLargeMcb ImportFunc<I_MODULE_NT, H_FsRtlTruncateLargeMcb>
#define I_FsRtlTruncateMcb ImportFunc<I_MODULE_NT, H_FsRtlTruncateMcb>
#define I_FsRtlTryToAcquireHeaderMutex ImportFunc<I_MODULE_NT, H_FsRtlTryToAcquireHeaderMutex>
#define I_FsRtlUninitializeBaseMcb ImportFunc<I_MODULE_NT, H_FsRtlUninitializeBaseMcb>
#define I_FsRtlUninitializeFileLock ImportFunc<I_MODULE_NT, H_FsRtlUninitializeFileLock>
#define I_FsRtlUninitializeLargeMcb ImportFunc<I_MODULE_NT, H_FsRtlUninitializeLargeMcb>
#define I_FsRtlUninitializeMcb ImportFunc<I_MODULE_NT, H_FsRtlUninitializeMcb>
#define I_FsRtlUninitializeOplock ImportFunc<I_MODULE_NT, H_FsRtlUninitializeOplock>
#define I_FsRtlUpdateDiskCounters ImportFunc<I_MODULE_NT, H_FsRtlUpdateDiskCounters>
#define I_FsRtlUpperOplockFsctrl ImportFunc<I_MODULE_NT, H_FsRtlUpperOplockFsctrl>
#define I_FsRtlValidateReparsePointBuffer ImportFunc<I_MODULE_NT, H_FsRtlValidateReparsePointBuffer>
#define I_HalDispatchTable ImportFunc<I_MODULE_NT, H_HalDispatchTable>
#define I_HalExamineMBR ImportFunc<I_MODULE_NT, H_HalExamineMBR>
#define I_HalPrivateDispatchTable ImportFunc<I_MODULE_NT, H_HalPrivateDispatchTable>
#define I_HeadlessDispatch ImportFunc<I_MODULE_NT, H_HeadlessDispatch>
#define I_HvlGetLpIndexFromApicId ImportFunc<I_MODULE_NT, H_HvlGetLpIndexFromApicId>
#define I_HvlPerformEndOfInterrupt ImportFunc<I_MODULE_NT, H_HvlPerformEndOfInterrupt>
#define I_HvlQueryActiveHypervisorProcessorCount ImportFunc<I_MODULE_NT, H_HvlQueryActiveHypervisorProcessorCount>
#define I_HvlQueryActiveProcessors ImportFunc<I_MODULE_NT, H_HvlQueryActiveProcessors>
#define I_HvlQueryConnection ImportFunc<I_MODULE_NT, H_HvlQueryConnection>
#define I_HvlQueryHypervisorProcessorNodeNumber ImportFunc<I_MODULE_NT, H_HvlQueryHypervisorProcessorNodeNumber>
#define I_HvlQueryNumaDistance ImportFunc<I_MODULE_NT, H_HvlQueryNumaDistance>
#define I_HvlQueryProcessorTopology ImportFunc<I_MODULE_NT, H_HvlQueryProcessorTopology>
#define I_HvlQueryProcessorTopologyCount ImportFunc<I_MODULE_NT, H_HvlQueryProcessorTopologyCount>
#define I_HvlQueryProcessorTopologyHighestId ImportFunc<I_MODULE_NT, H_HvlQueryProcessorTopologyHighestId>
#define I_HvlRegisterInterruptCallback ImportFunc<I_MODULE_NT, H_HvlRegisterInterruptCallback>
#define I_HvlRegisterWheaErrorNotification ImportFunc<I_MODULE_NT, H_HvlRegisterWheaErrorNotification>
#define I_HvlUnregisterInterruptCallback ImportFunc<I_MODULE_NT, H_HvlUnregisterInterruptCallback>
#define I_HvlUnregisterWheaErrorNotification ImportFunc<I_MODULE_NT, H_HvlUnregisterWheaErrorNotification>
#define I_InbvAcquireDisplayOwnership ImportFunc<I_MODULE_NT, H_InbvAcquireDisplayOwnership>
#define I_InbvCheckDisplayOwnership ImportFunc<I_MODULE_NT, H_InbvCheckDisplayOwnership>
#define I_InbvDisplayString ImportFunc<I_MODULE_NT, H_InbvDisplayString>
#define I_InbvEnableBootDriver ImportFunc<I_MODULE_NT, H_InbvEnableBootDriver>
#define I_InbvEnableDisplayString ImportFunc<I_MODULE_NT, H_InbvEnableDisplayString>
#define I_InbvInstallDisplayStringFilter ImportFunc<I_MODULE_NT, H_InbvInstallDisplayStringFilter>
#define I_InbvIsBootDriverInstalled ImportFunc<I_MODULE_NT, H_InbvIsBootDriverInstalled>
#define I_InbvNotifyDisplayOwnershipChange ImportFunc<I_MODULE_NT, H_InbvNotifyDisplayOwnershipChange>
#define I_InbvNotifyDisplayOwnershipLost ImportFunc<I_MODULE_NT, H_InbvNotifyDisplayOwnershipLost>
#define I_InbvResetDisplay ImportFunc<I_MODULE_NT, H_InbvResetDisplay>
#define I_InbvSetScrollRegion ImportFunc<I_MODULE_NT, H_InbvSetScrollRegion>
#define I_InbvSetTextColor ImportFunc<I_MODULE_NT, H_InbvSetTextColor>
#define I_InbvSolidColorFill ImportFunc<I_MODULE_NT, H_InbvSolidColorFill>
#define I_InitSafeBootMode ImportFunc<I_MODULE_NT, H_InitSafeBootMode>
#define I_InitializeSListHead ImportFunc<I_MODULE_NT, H_InitializeSListHead>
#define I_InterlockedPushListSList ImportFunc<I_MODULE_NT, H_InterlockedPushListSList>
#define I_IoAcquireCancelSpinLock ImportFunc<I_MODULE_NT, H_IoAcquireCancelSpinLock>
#define I_IoAcquireRemoveLockEx ImportFunc<I_MODULE_NT, H_IoAcquireRemoveLockEx>
#define I_IoAcquireVpbSpinLock ImportFunc<I_MODULE_NT, H_IoAcquireVpbSpinLock>
#define I_IoAdapterObjectType ImportFunc<I_MODULE_NT, H_IoAdapterObjectType>
#define I_IoAdjustStackSizeForRedirection ImportFunc<I_MODULE_NT, H_IoAdjustStackSizeForRedirection>
#define I_IoAllocateAdapterChannel ImportFunc<I_MODULE_NT, H_IoAllocateAdapterChannel>
#define I_IoAllocateController ImportFunc<I_MODULE_NT, H_IoAllocateController>
#define I_IoAllocateDriverObjectExtension ImportFunc<I_MODULE_NT, H_IoAllocateDriverObjectExtension>
#define I_IoAllocateErrorLogEntry ImportFunc<I_MODULE_NT, H_IoAllocateErrorLogEntry>
#define I_IoAllocateIrp ImportFunc<I_MODULE_NT, H_IoAllocateIrp>
#define I_IoAllocateMdl ImportFunc<I_MODULE_NT, H_IoAllocateMdl>
#define I_IoAllocateMiniCompletionPacket ImportFunc<I_MODULE_NT, H_IoAllocateMiniCompletionPacket>
#define I_IoAllocateSfioStreamIdentifier ImportFunc<I_MODULE_NT, H_IoAllocateSfioStreamIdentifier>
#define I_IoAllocateWorkItem ImportFunc<I_MODULE_NT, H_IoAllocateWorkItem>
#define I_IoApplyPriorityInfoThread ImportFunc<I_MODULE_NT, H_IoApplyPriorityInfoThread>
#define I_IoAssignResources ImportFunc<I_MODULE_NT, H_IoAssignResources>
#define I_IoAttachDevice ImportFunc<I_MODULE_NT, H_IoAttachDevice>
#define I_IoAttachDeviceByPointer ImportFunc<I_MODULE_NT, H_IoAttachDeviceByPointer>
#define I_IoAttachDeviceToDeviceStack ImportFunc<I_MODULE_NT, H_IoAttachDeviceToDeviceStack>
#define I_IoAttachDeviceToDeviceStackSafe ImportFunc<I_MODULE_NT, H_IoAttachDeviceToDeviceStackSafe>
#define I_IoBoostThreadIo ImportFunc<I_MODULE_NT, H_IoBoostThreadIo>
#define I_IoBuildAsynchronousFsdRequest ImportFunc<I_MODULE_NT, H_IoBuildAsynchronousFsdRequest>
#define I_IoBuildDeviceIoControlRequest ImportFunc<I_MODULE_NT, H_IoBuildDeviceIoControlRequest>
#define I_IoBuildPartialMdl ImportFunc<I_MODULE_NT, H_IoBuildPartialMdl>
#define I_IoBuildSynchronousFsdRequest ImportFunc<I_MODULE_NT, H_IoBuildSynchronousFsdRequest>
#define I_IoCallDriver ImportFunc<I_MODULE_NT, H_IoCallDriver>
#define I_IoCancelFileOpen ImportFunc<I_MODULE_NT, H_IoCancelFileOpen>
#define I_IoCancelIrp ImportFunc<I_MODULE_NT, H_IoCancelIrp>
#define I_IoCheckDesiredAccess ImportFunc<I_MODULE_NT, H_IoCheckDesiredAccess>
#define I_IoCheckEaBufferValidity ImportFunc<I_MODULE_NT, H_IoCheckEaBufferValidity>
#define I_IoCheckFunctionAccess ImportFunc<I_MODULE_NT, H_IoCheckFunctionAccess>
#define I_IoCheckQuerySetFileInformation ImportFunc<I_MODULE_NT, H_IoCheckQuerySetFileInformation>
#define I_IoCheckQuerySetVolumeInformation ImportFunc<I_MODULE_NT, H_IoCheckQuerySetVolumeInformation>
#define I_IoCheckQuotaBufferValidity ImportFunc<I_MODULE_NT, H_IoCheckQuotaBufferValidity>
#define I_IoCheckShareAccess ImportFunc<I_MODULE_NT, H_IoCheckShareAccess>
#define I_IoCheckShareAccessEx ImportFunc<I_MODULE_NT, H_IoCheckShareAccessEx>
#define I_IoClearActivityIdThread ImportFunc<I_MODULE_NT, H_IoClearActivityIdThread>
#define I_IoClearDependency ImportFunc<I_MODULE_NT, H_IoClearDependency>
#define I_IoClearIrpExtraCreateParameter ImportFunc<I_MODULE_NT, H_IoClearIrpExtraCreateParameter>
#define I_IoCompleteRequest ImportFunc<I_MODULE_NT, H_IoCompleteRequest>
#define I_IoCompletionObjectType ImportFunc<I_MODULE_NT, H_IoCompletionObjectType>
#define I_IoConnectInterrupt ImportFunc<I_MODULE_NT, H_IoConnectInterrupt>
#define I_IoConnectInterruptEx ImportFunc<I_MODULE_NT, H_IoConnectInterruptEx>
#define I_IoConvertFileHandleToKernelHandle ImportFunc<I_MODULE_NT, H_IoConvertFileHandleToKernelHandle>
#define I_IoCopyDeviceObjectHint ImportFunc<I_MODULE_NT, H_IoCopyDeviceObjectHint>
#define I_IoCreateArcName ImportFunc<I_MODULE_NT, H_IoCreateArcName>
#define I_IoCreateController ImportFunc<I_MODULE_NT, H_IoCreateController>
#define I_IoCreateDevice ImportFunc<I_MODULE_NT, H_IoCreateDevice>
#define I_IoCreateDisk ImportFunc<I_MODULE_NT, H_IoCreateDisk>
#define I_IoCreateDriver ImportFunc<I_MODULE_NT, H_IoCreateDriver>
#define I_IoCreateFile ImportFunc<I_MODULE_NT, H_IoCreateFile>
#define I_IoCreateFileEx ImportFunc<I_MODULE_NT, H_IoCreateFileEx>
#define I_IoCreateFileSpecifyDeviceObjectHint ImportFunc<I_MODULE_NT, H_IoCreateFileSpecifyDeviceObjectHint>
#define I_IoCreateNotificationEvent ImportFunc<I_MODULE_NT, H_IoCreateNotificationEvent>
#define I_IoCreateStreamFileObject ImportFunc<I_MODULE_NT, H_IoCreateStreamFileObject>
#define I_IoCreateStreamFileObjectEx ImportFunc<I_MODULE_NT, H_IoCreateStreamFileObjectEx>
#define I_IoCreateStreamFileObjectEx2 ImportFunc<I_MODULE_NT, H_IoCreateStreamFileObjectEx2>
#define I_IoCreateStreamFileObjectLite ImportFunc<I_MODULE_NT, H_IoCreateStreamFileObjectLite>
#define I_IoCreateSymbolicLink ImportFunc<I_MODULE_NT, H_IoCreateSymbolicLink>
#define I_IoCreateSynchronizationEvent ImportFunc<I_MODULE_NT, H_IoCreateSynchronizationEvent>
#define I_IoCreateSystemThread ImportFunc<I_MODULE_NT, H_IoCreateSystemThread>
#define I_IoCreateUnprotectedSymbolicLink ImportFunc<I_MODULE_NT, H_IoCreateUnprotectedSymbolicLink>
#define I_IoCsqInitialize ImportFunc<I_MODULE_NT, H_IoCsqInitialize>
#define I_IoCsqInitializeEx ImportFunc<I_MODULE_NT, H_IoCsqInitializeEx>
#define I_IoCsqInsertIrp ImportFunc<I_MODULE_NT, H_IoCsqInsertIrp>
#define I_IoCsqInsertIrpEx ImportFunc<I_MODULE_NT, H_IoCsqInsertIrpEx>
#define I_IoCsqRemoveIrp ImportFunc<I_MODULE_NT, H_IoCsqRemoveIrp>
#define I_IoCsqRemoveNextIrp ImportFunc<I_MODULE_NT, H_IoCsqRemoveNextIrp>
#define I_IoDecrementKeepAliveCount ImportFunc<I_MODULE_NT, H_IoDecrementKeepAliveCount>
#define I_IoDeleteAllDependencyRelations ImportFunc<I_MODULE_NT, H_IoDeleteAllDependencyRelations>
#define I_IoDeleteController ImportFunc<I_MODULE_NT, H_IoDeleteController>
#define I_IoDeleteDevice ImportFunc<I_MODULE_NT, H_IoDeleteDevice>
#define I_IoDeleteDriver ImportFunc<I_MODULE_NT, H_IoDeleteDriver>
#define I_IoDeleteSymbolicLink ImportFunc<I_MODULE_NT, H_IoDeleteSymbolicLink>
#define I_IoDetachDevice ImportFunc<I_MODULE_NT, H_IoDetachDevice>
#define I_IoDeviceHandlerObjectSize ImportFunc<I_MODULE_NT, H_IoDeviceHandlerObjectSize>
#define I_IoDeviceHandlerObjectType ImportFunc<I_MODULE_NT, H_IoDeviceHandlerObjectType>
#define I_IoDeviceObjectType ImportFunc<I_MODULE_NT, H_IoDeviceObjectType>
#define I_IoDisconnectInterrupt ImportFunc<I_MODULE_NT, H_IoDisconnectInterrupt>
#define I_IoDisconnectInterruptEx ImportFunc<I_MODULE_NT, H_IoDisconnectInterruptEx>
#define I_IoDriverObjectType ImportFunc<I_MODULE_NT, H_IoDriverObjectType>
#define I_IoDuplicateDependency ImportFunc<I_MODULE_NT, H_IoDuplicateDependency>
#define I_IoEnqueueIrp ImportFunc<I_MODULE_NT, H_IoEnqueueIrp>
#define I_IoEnumerateDeviceObjectList ImportFunc<I_MODULE_NT, H_IoEnumerateDeviceObjectList>
#define I_IoEnumerateRegisteredFiltersList ImportFunc<I_MODULE_NT, H_IoEnumerateRegisteredFiltersList>
#define I_IoFastQueryNetworkAttributes ImportFunc<I_MODULE_NT, H_IoFastQueryNetworkAttributes>
#define I_IoFileObjectType ImportFunc<I_MODULE_NT, H_IoFileObjectType>
#define I_IoForwardAndCatchIrp ImportFunc<I_MODULE_NT, H_IoForwardAndCatchIrp>
#define I_IoForwardIrpSynchronously ImportFunc<I_MODULE_NT, H_IoForwardIrpSynchronously>
#define I_IoFreeController ImportFunc<I_MODULE_NT, H_IoFreeController>
#define I_IoFreeErrorLogEntry ImportFunc<I_MODULE_NT, H_IoFreeErrorLogEntry>
#define I_IoFreeIrp ImportFunc<I_MODULE_NT, H_IoFreeIrp>
#define I_IoFreeMdl ImportFunc<I_MODULE_NT, H_IoFreeMdl>
#define I_IoFreeMiniCompletionPacket ImportFunc<I_MODULE_NT, H_IoFreeMiniCompletionPacket>
#define I_IoFreeSfioStreamIdentifier ImportFunc<I_MODULE_NT, H_IoFreeSfioStreamIdentifier>
#define I_IoFreeWorkItem ImportFunc<I_MODULE_NT, H_IoFreeWorkItem>
#define I_IoGetActivityIdIrp ImportFunc<I_MODULE_NT, H_IoGetActivityIdIrp>
#define I_IoGetActivityIdThread ImportFunc<I_MODULE_NT, H_IoGetActivityIdThread>
#define I_IoGetAffinityInterrupt ImportFunc<I_MODULE_NT, H_IoGetAffinityInterrupt>
#define I_IoGetAttachedDevice ImportFunc<I_MODULE_NT, H_IoGetAttachedDevice>
#define I_IoGetAttachedDeviceReference ImportFunc<I_MODULE_NT, H_IoGetAttachedDeviceReference>
#define I_IoGetBaseFileSystemDeviceObject ImportFunc<I_MODULE_NT, H_IoGetBaseFileSystemDeviceObject>
#define I_IoGetBootDiskInformation ImportFunc<I_MODULE_NT, H_IoGetBootDiskInformation>
#define I_IoGetBootDiskInformationLite ImportFunc<I_MODULE_NT, H_IoGetBootDiskInformationLite>
#define I_IoGetConfigurationInformation ImportFunc<I_MODULE_NT, H_IoGetConfigurationInformation>
#define I_IoGetContainerInformation ImportFunc<I_MODULE_NT, H_IoGetContainerInformation>
#define I_IoGetCurrentProcess ImportFunc<I_MODULE_NT, H_IoGetCurrentProcess>
#define I_IoGetDeviceAttachmentBaseRef ImportFunc<I_MODULE_NT, H_IoGetDeviceAttachmentBaseRef>
#define I_IoGetDeviceInterfaceAlias ImportFunc<I_MODULE_NT, H_IoGetDeviceInterfaceAlias>
#define I_IoGetDeviceInterfacePropertyData ImportFunc<I_MODULE_NT, H_IoGetDeviceInterfacePropertyData>
#define I_IoGetDeviceInterfaces ImportFunc<I_MODULE_NT, H_IoGetDeviceInterfaces>
#define I_IoGetDeviceNumaNode ImportFunc<I_MODULE_NT, H_IoGetDeviceNumaNode>
#define I_IoGetDeviceObjectPointer ImportFunc<I_MODULE_NT, H_IoGetDeviceObjectPointer>
#define I_IoGetDeviceProperty ImportFunc<I_MODULE_NT, H_IoGetDeviceProperty>
#define I_IoGetDevicePropertyData ImportFunc<I_MODULE_NT, H_IoGetDevicePropertyData>
#define I_IoGetDeviceToVerify ImportFunc<I_MODULE_NT, H_IoGetDeviceToVerify>
#define I_IoGetDiskDeviceObject ImportFunc<I_MODULE_NT, H_IoGetDiskDeviceObject>
#define I_IoGetDmaAdapter ImportFunc<I_MODULE_NT, H_IoGetDmaAdapter>
#define I_IoGetDriverObjectExtension ImportFunc<I_MODULE_NT, H_IoGetDriverObjectExtension>
#define I_IoGetFileObjectGenericMapping ImportFunc<I_MODULE_NT, H_IoGetFileObjectGenericMapping>
#define I_IoGetGenericIrpExtension ImportFunc<I_MODULE_NT, H_IoGetGenericIrpExtension>
#define I_IoGetInitialStack ImportFunc<I_MODULE_NT, H_IoGetInitialStack>
#define I_IoGetInitiatorProcess ImportFunc<I_MODULE_NT, H_IoGetInitiatorProcess>
#define I_IoGetIoPriorityHint ImportFunc<I_MODULE_NT, H_IoGetIoPriorityHint>
#define I_IoGetIrpExtraCreateParameter ImportFunc<I_MODULE_NT, H_IoGetIrpExtraCreateParameter>
#define I_IoGetLowerDeviceObject ImportFunc<I_MODULE_NT, H_IoGetLowerDeviceObject>
#define I_IoGetOplockKeyContext ImportFunc<I_MODULE_NT, H_IoGetOplockKeyContext>
#define I_IoGetOplockKeyContextEx ImportFunc<I_MODULE_NT, H_IoGetOplockKeyContextEx>
#define I_IoGetPagingIoPriority ImportFunc<I_MODULE_NT, H_IoGetPagingIoPriority>
#define I_IoGetRelatedDeviceObject ImportFunc<I_MODULE_NT, H_IoGetRelatedDeviceObject>
#define I_IoGetRequestorProcess ImportFunc<I_MODULE_NT, H_IoGetRequestorProcess>
#define I_IoGetRequestorProcessId ImportFunc<I_MODULE_NT, H_IoGetRequestorProcessId>
#define I_IoGetRequestorSessionId ImportFunc<I_MODULE_NT, H_IoGetRequestorSessionId>
#define I_IoGetSfioStreamIdentifier ImportFunc<I_MODULE_NT, H_IoGetSfioStreamIdentifier>
#define I_IoGetStackLimits ImportFunc<I_MODULE_NT, H_IoGetStackLimits>
#define I_IoGetSymlinkSupportInformation ImportFunc<I_MODULE_NT, H_IoGetSymlinkSupportInformation>
#define I_IoGetTopLevelIrp ImportFunc<I_MODULE_NT, H_IoGetTopLevelIrp>
#define I_IoGetTransactionParameterBlock ImportFunc<I_MODULE_NT, H_IoGetTransactionParameterBlock>
#define I_IoIncrementKeepAliveCount ImportFunc<I_MODULE_NT, H_IoIncrementKeepAliveCount>
#define I_IoInitializeIrp ImportFunc<I_MODULE_NT, H_IoInitializeIrp>
#define I_IoInitializeMiniCompletionPacket ImportFunc<I_MODULE_NT, H_IoInitializeMiniCompletionPacket>
#define I_IoInitializeRemoveLockEx ImportFunc<I_MODULE_NT, H_IoInitializeRemoveLockEx>
#define I_IoInitializeTimer ImportFunc<I_MODULE_NT, H_IoInitializeTimer>
#define I_IoInitializeWorkItem ImportFunc<I_MODULE_NT, H_IoInitializeWorkItem>
#define I_IoInvalidateDeviceRelations ImportFunc<I_MODULE_NT, H_IoInvalidateDeviceRelations>
#define I_IoInvalidateDeviceState ImportFunc<I_MODULE_NT, H_IoInvalidateDeviceState>
#define I_IoIs32bitProcess ImportFunc<I_MODULE_NT, H_IoIs32bitProcess>
#define I_IoIsActivityTracingEnabled ImportFunc<I_MODULE_NT, H_IoIsActivityTracingEnabled>
#define I_IoIsFileObjectIgnoringSharing ImportFunc<I_MODULE_NT, H_IoIsFileObjectIgnoringSharing>
#define I_IoIsFileOriginRemote ImportFunc<I_MODULE_NT, H_IoIsFileOriginRemote>
#define I_IoIsInitiator32bitProcess ImportFunc<I_MODULE_NT, H_IoIsInitiator32bitProcess>
#define I_IoIsOperationSynchronous ImportFunc<I_MODULE_NT, H_IoIsOperationSynchronous>
#define I_IoIsSystemThread ImportFunc<I_MODULE_NT, H_IoIsSystemThread>
#define I_IoIsValidIrpStatus ImportFunc<I_MODULE_NT, H_IoIsValidIrpStatus>
#define I_IoIsValidNameGraftingBuffer ImportFunc<I_MODULE_NT, H_IoIsValidNameGraftingBuffer>
#define I_IoIsWdmVersionAvailable ImportFunc<I_MODULE_NT, H_IoIsWdmVersionAvailable>
#define I_IoLoadCrashDumpDriver ImportFunc<I_MODULE_NT, H_IoLoadCrashDumpDriver>
#define I_IoMakeAssociatedIrp ImportFunc<I_MODULE_NT, H_IoMakeAssociatedIrp>
#define I_IoOpenDeviceInterfaceRegistryKey ImportFunc<I_MODULE_NT, H_IoOpenDeviceInterfaceRegistryKey>
#define I_IoOpenDeviceRegistryKey ImportFunc<I_MODULE_NT, H_IoOpenDeviceRegistryKey>
#define I_IoPageRead ImportFunc<I_MODULE_NT, H_IoPageRead>
#define I_IoPropagateActivityIdToThread ImportFunc<I_MODULE_NT, H_IoPropagateActivityIdToThread>
#define I_IoPropagateIrpExtension ImportFunc<I_MODULE_NT, H_IoPropagateIrpExtension>
#define I_IoQueryDeviceDescription ImportFunc<I_MODULE_NT, H_IoQueryDeviceDescription>
#define I_IoQueryFileDosDeviceName ImportFunc<I_MODULE_NT, H_IoQueryFileDosDeviceName>
#define I_IoQueryFileInformation ImportFunc<I_MODULE_NT, H_IoQueryFileInformation>
#define I_IoQueryFullDriverPath ImportFunc<I_MODULE_NT, H_IoQueryFullDriverPath>
#define I_IoQueryVolumeInformation ImportFunc<I_MODULE_NT, H_IoQueryVolumeInformation>
#define I_IoQueueThreadIrp ImportFunc<I_MODULE_NT, H_IoQueueThreadIrp>
#define I_IoQueueWorkItem ImportFunc<I_MODULE_NT, H_IoQueueWorkItem>
#define I_IoQueueWorkItemEx ImportFunc<I_MODULE_NT, H_IoQueueWorkItemEx>
#define I_IoQueueWorkItemToNode ImportFunc<I_MODULE_NT, H_IoQueueWorkItemToNode>
#define I_IoRaiseHardError ImportFunc<I_MODULE_NT, H_IoRaiseHardError>
#define I_IoRaiseInformationalHardError ImportFunc<I_MODULE_NT, H_IoRaiseInformationalHardError>
#define I_IoReadDiskSignature ImportFunc<I_MODULE_NT, H_IoReadDiskSignature>
#define I_IoReadOperationCount ImportFunc<I_MODULE_NT, H_IoReadOperationCount>
#define I_IoReadPartitionTable ImportFunc<I_MODULE_NT, H_IoReadPartitionTable>
#define I_IoReadPartitionTableEx ImportFunc<I_MODULE_NT, H_IoReadPartitionTableEx>
#define I_IoReadTransferCount ImportFunc<I_MODULE_NT, H_IoReadTransferCount>
#define I_IoRegisterBootDriverCallback ImportFunc<I_MODULE_NT, H_IoRegisterBootDriverCallback>
#define I_IoRegisterBootDriverReinitialization ImportFunc<I_MODULE_NT, H_IoRegisterBootDriverReinitialization>
#define I_IoRegisterContainerNotification ImportFunc<I_MODULE_NT, H_IoRegisterContainerNotification>
#define I_IoRegisterDeviceInterface ImportFunc<I_MODULE_NT, H_IoRegisterDeviceInterface>
#define I_IoRegisterDriverReinitialization ImportFunc<I_MODULE_NT, H_IoRegisterDriverReinitialization>
#define I_IoRegisterFileSystem ImportFunc<I_MODULE_NT, H_IoRegisterFileSystem>
#define I_IoRegisterFsRegistrationChange ImportFunc<I_MODULE_NT, H_IoRegisterFsRegistrationChange>
#define I_IoRegisterFsRegistrationChangeMountAware ImportFunc<I_MODULE_NT, H_IoRegisterFsRegistrationChangeMountAware>
#define I_IoRegisterIoTracking ImportFunc<I_MODULE_NT, H_IoRegisterIoTracking>
#define I_IoRegisterLastChanceShutdownNotification ImportFunc<I_MODULE_NT, H_IoRegisterLastChanceShutdownNotification>
#define I_IoRegisterPlugPlayNotification ImportFunc<I_MODULE_NT, H_IoRegisterPlugPlayNotification>
#define I_IoRegisterPriorityCallback ImportFunc<I_MODULE_NT, H_IoRegisterPriorityCallback>
#define I_IoRegisterShutdownNotification ImportFunc<I_MODULE_NT, H_IoRegisterShutdownNotification>
#define I_IoReleaseCancelSpinLock ImportFunc<I_MODULE_NT, H_IoReleaseCancelSpinLock>
#define I_IoReleaseRemoveLockAndWaitEx ImportFunc<I_MODULE_NT, H_IoReleaseRemoveLockAndWaitEx>
#define I_IoReleaseRemoveLockEx ImportFunc<I_MODULE_NT, H_IoReleaseRemoveLockEx>
#define I_IoReleaseVpbSpinLock ImportFunc<I_MODULE_NT, H_IoReleaseVpbSpinLock>
#define I_IoRemoveShareAccess ImportFunc<I_MODULE_NT, H_IoRemoveShareAccess>
#define I_IoReplaceFileObjectName ImportFunc<I_MODULE_NT, H_IoReplaceFileObjectName>
#define I_IoReplacePartitionUnit ImportFunc<I_MODULE_NT, H_IoReplacePartitionUnit>
#define I_IoReportDetectedDevice ImportFunc<I_MODULE_NT, H_IoReportDetectedDevice>
#define I_IoReportHalResourceUsage ImportFunc<I_MODULE_NT, H_IoReportHalResourceUsage>
#define I_IoReportInterruptActive ImportFunc<I_MODULE_NT, H_IoReportInterruptActive>
#define I_IoReportInterruptInactive ImportFunc<I_MODULE_NT, H_IoReportInterruptInactive>
#define I_IoReportResourceForDetection ImportFunc<I_MODULE_NT, H_IoReportResourceForDetection>
#define I_IoReportResourceUsage ImportFunc<I_MODULE_NT, H_IoReportResourceUsage>
#define I_IoReportRootDevice ImportFunc<I_MODULE_NT, H_IoReportRootDevice>
#define I_IoReportTargetDeviceChange ImportFunc<I_MODULE_NT, H_IoReportTargetDeviceChange>
#define I_IoReportTargetDeviceChangeAsynchronous ImportFunc<I_MODULE_NT, H_IoReportTargetDeviceChangeAsynchronous>
#define I_IoRequestDeviceEject ImportFunc<I_MODULE_NT, H_IoRequestDeviceEject>
#define I_IoRequestDeviceEjectEx ImportFunc<I_MODULE_NT, H_IoRequestDeviceEjectEx>
#define I_IoReserveDependency ImportFunc<I_MODULE_NT, H_IoReserveDependency>
#define I_IoResolveDependency ImportFunc<I_MODULE_NT, H_IoResolveDependency>
#define I_IoRetrievePriorityInfo ImportFunc<I_MODULE_NT, H_IoRetrievePriorityInfo>
#define I_IoReuseIrp ImportFunc<I_MODULE_NT, H_IoReuseIrp>
#define I_IoSetActivityIdIrp ImportFunc<I_MODULE_NT, H_IoSetActivityIdIrp>
#define I_IoSetActivityIdThread ImportFunc<I_MODULE_NT, H_IoSetActivityIdThread>
#define I_IoSetCompletionRoutineEx ImportFunc<I_MODULE_NT, H_IoSetCompletionRoutineEx>
#define I_IoSetDependency ImportFunc<I_MODULE_NT, H_IoSetDependency>
#define I_IoSetDeviceInterfacePropertyData ImportFunc<I_MODULE_NT, H_IoSetDeviceInterfacePropertyData>
#define I_IoSetDeviceInterfaceState ImportFunc<I_MODULE_NT, H_IoSetDeviceInterfaceState>
#define I_IoSetDevicePropertyData ImportFunc<I_MODULE_NT, H_IoSetDevicePropertyData>
#define I_IoSetDeviceToVerify ImportFunc<I_MODULE_NT, H_IoSetDeviceToVerify>
#define I_IoSetFileObjectIgnoreSharing ImportFunc<I_MODULE_NT, H_IoSetFileObjectIgnoreSharing>
#define I_IoSetFileOrigin ImportFunc<I_MODULE_NT, H_IoSetFileOrigin>
#define I_IoSetGenericIrpExtension ImportFunc<I_MODULE_NT, H_IoSetGenericIrpExtension>
#define I_IoSetHardErrorOrVerifyDevice ImportFunc<I_MODULE_NT, H_IoSetHardErrorOrVerifyDevice>
#define I_IoSetInformation ImportFunc<I_MODULE_NT, H_IoSetInformation>
#define I_IoSetIoCompletion ImportFunc<I_MODULE_NT, H_IoSetIoCompletion>
#define I_IoSetIoCompletionEx ImportFunc<I_MODULE_NT, H_IoSetIoCompletionEx>
#define I_IoSetIoPriorityHint ImportFunc<I_MODULE_NT, H_IoSetIoPriorityHint>
#define I_IoSetIoPriorityHintIntoFileObject ImportFunc<I_MODULE_NT, H_IoSetIoPriorityHintIntoFileObject>
#define I_IoSetIoPriorityHintIntoThread ImportFunc<I_MODULE_NT, H_IoSetIoPriorityHintIntoThread>
#define I_IoSetIrpExtraCreateParameter ImportFunc<I_MODULE_NT, H_IoSetIrpExtraCreateParameter>
#define I_IoSetMasterIrpStatus ImportFunc<I_MODULE_NT, H_IoSetMasterIrpStatus>
#define I_IoSetPartitionInformation ImportFunc<I_MODULE_NT, H_IoSetPartitionInformation>
#define I_IoSetPartitionInformationEx ImportFunc<I_MODULE_NT, H_IoSetPartitionInformationEx>
#define I_IoSetShareAccess ImportFunc<I_MODULE_NT, H_IoSetShareAccess>
#define I_IoSetShareAccessEx ImportFunc<I_MODULE_NT, H_IoSetShareAccessEx>
#define I_IoSetStartIoAttributes ImportFunc<I_MODULE_NT, H_IoSetStartIoAttributes>
#define I_IoSetSystemPartition ImportFunc<I_MODULE_NT, H_IoSetSystemPartition>
#define I_IoSetThreadHardErrorMode ImportFunc<I_MODULE_NT, H_IoSetThreadHardErrorMode>
#define I_IoSetTopLevelIrp ImportFunc<I_MODULE_NT, H_IoSetTopLevelIrp>
#define I_IoSizeofGenericIrpExtension ImportFunc<I_MODULE_NT, H_IoSizeofGenericIrpExtension>
#define I_IoSizeofWorkItem ImportFunc<I_MODULE_NT, H_IoSizeofWorkItem>
#define I_IoStartNextPacket ImportFunc<I_MODULE_NT, H_IoStartNextPacket>
#define I_IoStartNextPacketByKey ImportFunc<I_MODULE_NT, H_IoStartNextPacketByKey>
#define I_IoStartPacket ImportFunc<I_MODULE_NT, H_IoStartPacket>
#define I_IoStartTimer ImportFunc<I_MODULE_NT, H_IoStartTimer>
#define I_IoStatisticsLock ImportFunc<I_MODULE_NT, H_IoStatisticsLock>
#define I_IoStopTimer ImportFunc<I_MODULE_NT, H_IoStopTimer>
#define I_IoSynchronousCallDriver ImportFunc<I_MODULE_NT, H_IoSynchronousCallDriver>
#define I_IoSynchronousInvalidateDeviceRelations ImportFunc<I_MODULE_NT, H_IoSynchronousInvalidateDeviceRelations>
#define I_IoSynchronousPageWrite ImportFunc<I_MODULE_NT, H_IoSynchronousPageWrite>
#define I_IoTestDependency ImportFunc<I_MODULE_NT, H_IoTestDependency>
#define I_IoThreadToProcess ImportFunc<I_MODULE_NT, H_IoThreadToProcess>
#define I_IoTransferActivityId ImportFunc<I_MODULE_NT, H_IoTransferActivityId>
#define I_IoTranslateBusAddress ImportFunc<I_MODULE_NT, H_IoTranslateBusAddress>
#define I_IoTryQueueWorkItem ImportFunc<I_MODULE_NT, H_IoTryQueueWorkItem>
#define I_IoUninitializeWorkItem ImportFunc<I_MODULE_NT, H_IoUninitializeWorkItem>
#define I_IoUnregisterBootDriverCallback ImportFunc<I_MODULE_NT, H_IoUnregisterBootDriverCallback>
#define I_IoUnregisterContainerNotification ImportFunc<I_MODULE_NT, H_IoUnregisterContainerNotification>
#define I_IoUnregisterFileSystem ImportFunc<I_MODULE_NT, H_IoUnregisterFileSystem>
#define I_IoUnregisterFsRegistrationChange ImportFunc<I_MODULE_NT, H_IoUnregisterFsRegistrationChange>
#define I_IoUnregisterIoTracking ImportFunc<I_MODULE_NT, H_IoUnregisterIoTracking>
#define I_IoUnregisterPlugPlayNotification ImportFunc<I_MODULE_NT, H_IoUnregisterPlugPlayNotification>
#define I_IoUnregisterPlugPlayNotificationEx ImportFunc<I_MODULE_NT, H_IoUnregisterPlugPlayNotificationEx>
#define I_IoUnregisterPriorityCallback ImportFunc<I_MODULE_NT, H_IoUnregisterPriorityCallback>
#define I_IoUnregisterShutdownNotification ImportFunc<I_MODULE_NT, H_IoUnregisterShutdownNotification>
#define I_IoUpdateShareAccess ImportFunc<I_MODULE_NT, H_IoUpdateShareAccess>
#define I_IoValidateDeviceIoControlAccess ImportFunc<I_MODULE_NT, H_IoValidateDeviceIoControlAccess>
#define I_IoVerifyPartitionTable ImportFunc<I_MODULE_NT, H_IoVerifyPartitionTable>
#define I_IoVerifyVolume ImportFunc<I_MODULE_NT, H_IoVerifyVolume>
#define I_IoVolumeDeviceToDosName ImportFunc<I_MODULE_NT, H_IoVolumeDeviceToDosName>
#define I_IoVolumeDeviceToGuid ImportFunc<I_MODULE_NT, H_IoVolumeDeviceToGuid>
#define I_IoVolumeDeviceToGuidPath ImportFunc<I_MODULE_NT, H_IoVolumeDeviceToGuidPath>
#define I_IoWMIAllocateInstanceIds ImportFunc<I_MODULE_NT, H_IoWMIAllocateInstanceIds>
#define I_IoWMIDeviceObjectToInstanceName ImportFunc<I_MODULE_NT, H_IoWMIDeviceObjectToInstanceName>
#define I_IoWMIDeviceObjectToProviderId ImportFunc<I_MODULE_NT, H_IoWMIDeviceObjectToProviderId>
#define I_IoWMIExecuteMethod ImportFunc<I_MODULE_NT, H_IoWMIExecuteMethod>
#define I_IoWMIHandleToInstanceName ImportFunc<I_MODULE_NT, H_IoWMIHandleToInstanceName>
#define I_IoWMIOpenBlock ImportFunc<I_MODULE_NT, H_IoWMIOpenBlock>
#define I_IoWMIQueryAllData ImportFunc<I_MODULE_NT, H_IoWMIQueryAllData>
#define I_IoWMIQueryAllDataMultiple ImportFunc<I_MODULE_NT, H_IoWMIQueryAllDataMultiple>
#define I_IoWMIQuerySingleInstance ImportFunc<I_MODULE_NT, H_IoWMIQuerySingleInstance>
#define I_IoWMIQuerySingleInstanceMultiple ImportFunc<I_MODULE_NT, H_IoWMIQuerySingleInstanceMultiple>
#define I_IoWMIRegistrationControl ImportFunc<I_MODULE_NT, H_IoWMIRegistrationControl>
#define I_IoWMISetNotificationCallback ImportFunc<I_MODULE_NT, H_IoWMISetNotificationCallback>
#define I_IoWMISetSingleInstance ImportFunc<I_MODULE_NT, H_IoWMISetSingleInstance>
#define I_IoWMISetSingleItem ImportFunc<I_MODULE_NT, H_IoWMISetSingleItem>
#define I_IoWMISuggestInstanceName ImportFunc<I_MODULE_NT, H_IoWMISuggestInstanceName>
#define I_IoWMIWriteEvent ImportFunc<I_MODULE_NT, H_IoWMIWriteEvent>
#define I_IoWithinStackLimits ImportFunc<I_MODULE_NT, H_IoWithinStackLimits>
#define I_IoWriteErrorLogEntry ImportFunc<I_MODULE_NT, H_IoWriteErrorLogEntry>
#define I_IoWriteOperationCount ImportFunc<I_MODULE_NT, H_IoWriteOperationCount>
#define I_IoWritePartitionTable ImportFunc<I_MODULE_NT, H_IoWritePartitionTable>
#define I_IoWritePartitionTableEx ImportFunc<I_MODULE_NT, H_IoWritePartitionTableEx>
#define I_IoWriteTransferCount ImportFunc<I_MODULE_NT, H_IoWriteTransferCount>
#define I_IofCallDriver ImportFunc<I_MODULE_NT, H_IofCallDriver>
#define I_IofCompleteRequest ImportFunc<I_MODULE_NT, H_IofCompleteRequest>
#define I_KdAcquireDebuggerLock ImportFunc<I_MODULE_NT, H_KdAcquireDebuggerLock>
#define I_KdChangeOption ImportFunc<I_MODULE_NT, H_KdChangeOption>
#define I_KdDebuggerEnabled ImportFunc<I_MODULE_NT, H_KdDebuggerEnabled>
#define I_KdDebuggerNotPresent ImportFunc<I_MODULE_NT, H_KdDebuggerNotPresent>
#define I_KdDeregisterPowerHandler ImportFunc<I_MODULE_NT, H_KdDeregisterPowerHandler>
#define I_KdDisableDebugger ImportFunc<I_MODULE_NT, H_KdDisableDebugger>
#define I_KdEnableDebugger ImportFunc<I_MODULE_NT, H_KdEnableDebugger>
#define I_KdEnteredDebugger ImportFunc<I_MODULE_NT, H_KdEnteredDebugger>
#define I_KdLogDbgPrint ImportFunc<I_MODULE_NT, H_KdLogDbgPrint>
#define I_KdPollBreakIn ImportFunc<I_MODULE_NT, H_KdPollBreakIn>
#define I_KdPowerTransition ImportFunc<I_MODULE_NT, H_KdPowerTransition>
#define I_KdRefreshDebuggerNotPresent ImportFunc<I_MODULE_NT, H_KdRefreshDebuggerNotPresent>
#define I_KdRegisterPowerHandler ImportFunc<I_MODULE_NT, H_KdRegisterPowerHandler>
#define I_KdReleaseDebuggerLock ImportFunc<I_MODULE_NT, H_KdReleaseDebuggerLock>
#define I_KdSystemDebugControl ImportFunc<I_MODULE_NT, H_KdSystemDebugControl>
#define I_KeAcquireGuardedMutex ImportFunc<I_MODULE_NT, H_KeAcquireGuardedMutex>
#define I_KeAcquireGuardedMutexUnsafe ImportFunc<I_MODULE_NT, H_KeAcquireGuardedMutexUnsafe>
#define I_KeAcquireInStackQueuedSpinLock ImportFunc<I_MODULE_NT, H_KeAcquireInStackQueuedSpinLock>
#define I_KeAcquireInStackQueuedSpinLockAtDpcLevel ImportFunc<I_MODULE_NT, H_KeAcquireInStackQueuedSpinLockAtDpcLevel>
#define I_KeAcquireInStackQueuedSpinLockForDpc ImportFunc<I_MODULE_NT, H_KeAcquireInStackQueuedSpinLockForDpc>
#define I_KeAcquireInStackQueuedSpinLockRaiseToSynch ImportFunc<I_MODULE_NT, H_KeAcquireInStackQueuedSpinLockRaiseToSynch>
#define I_KeAcquireInterruptSpinLock ImportFunc<I_MODULE_NT, H_KeAcquireInterruptSpinLock>
#define I_KeAcquireQueuedSpinLock ImportFunc<I_MODULE_NT, H_KeAcquireQueuedSpinLock>
#define I_KeAcquireQueuedSpinLockRaiseToSynch ImportFunc<I_MODULE_NT, H_KeAcquireQueuedSpinLockRaiseToSynch>
#define I_KeAcquireSpinLockAtDpcLevel ImportFunc<I_MODULE_NT, H_KeAcquireSpinLockAtDpcLevel>
#define I_KeAcquireSpinLockForDpc ImportFunc<I_MODULE_NT, H_KeAcquireSpinLockForDpc>
#define I_KeAcquireSpinLockRaiseToDpc ImportFunc<I_MODULE_NT, H_KeAcquireSpinLockRaiseToDpc>
#define I_KeAcquireSpinLockRaiseToSynch ImportFunc<I_MODULE_NT, H_KeAcquireSpinLockRaiseToSynch>
#define I_KeAddGroupAffinityEx ImportFunc<I_MODULE_NT, H_KeAddGroupAffinityEx>
#define I_KeAddProcessorAffinityEx ImportFunc<I_MODULE_NT, H_KeAddProcessorAffinityEx>
#define I_KeAddProcessorGroupAffinity ImportFunc<I_MODULE_NT, H_KeAddProcessorGroupAffinity>
#define I_KeAddSystemServiceTable ImportFunc<I_MODULE_NT, H_KeAddSystemServiceTable>
#define I_KeAlertThread ImportFunc<I_MODULE_NT, H_KeAlertThread>
#define I_KeAllocateCalloutStack ImportFunc<I_MODULE_NT, H_KeAllocateCalloutStack>
#define I_KeAllocateCalloutStackEx ImportFunc<I_MODULE_NT, H_KeAllocateCalloutStackEx>
#define I_KeAndAffinityEx ImportFunc<I_MODULE_NT, H_KeAndAffinityEx>
#define I_KeAndGroupAffinityEx ImportFunc<I_MODULE_NT, H_KeAndGroupAffinityEx>
#define I_KeAreAllApcsDisabled ImportFunc<I_MODULE_NT, H_KeAreAllApcsDisabled>
#define I_KeAreApcsDisabled ImportFunc<I_MODULE_NT, H_KeAreApcsDisabled>
#define I_KeAttachProcess ImportFunc<I_MODULE_NT, H_KeAttachProcess>
#define I_KeBugCheck ImportFunc<I_MODULE_NT, H_KeBugCheck>
#define I_KeBugCheckEx ImportFunc<I_MODULE_NT, H_KeBugCheckEx>
#define I_KeCancelTimer ImportFunc<I_MODULE_NT, H_KeCancelTimer>
#define I_KeCapturePersistentThreadState ImportFunc<I_MODULE_NT, H_KeCapturePersistentThreadState>
#define I_KeCheckProcessorAffinityEx ImportFunc<I_MODULE_NT, H_KeCheckProcessorAffinityEx>
#define I_KeCheckProcessorGroupAffinity ImportFunc<I_MODULE_NT, H_KeCheckProcessorGroupAffinity>
#define I_KeClearEvent ImportFunc<I_MODULE_NT, H_KeClearEvent>
#define I_KeClockInterruptNotify ImportFunc<I_MODULE_NT, H_KeClockInterruptNotify>
#define I_KeClockTimerPowerChange ImportFunc<I_MODULE_NT, H_KeClockTimerPowerChange>
#define I_KeComplementAffinityEx ImportFunc<I_MODULE_NT, H_KeComplementAffinityEx>
#define I_KeCopyAffinityEx ImportFunc<I_MODULE_NT, H_KeCopyAffinityEx>
#define I_KeCountSetBitsAffinityEx ImportFunc<I_MODULE_NT, H_KeCountSetBitsAffinityEx>
#define I_KeCountSetBitsGroupAffinity ImportFunc<I_MODULE_NT, H_KeCountSetBitsGroupAffinity>
#define I_KeDelayExecutionThread ImportFunc<I_MODULE_NT, H_KeDelayExecutionThread>
#define I_KeDeregisterBugCheckCallback ImportFunc<I_MODULE_NT, H_KeDeregisterBugCheckCallback>
#define I_KeDeregisterBugCheckReasonCallback ImportFunc<I_MODULE_NT, H_KeDeregisterBugCheckReasonCallback>
#define I_KeDeregisterNmiCallback ImportFunc<I_MODULE_NT, H_KeDeregisterNmiCallback>
#define I_KeDeregisterProcessorChangeCallback ImportFunc<I_MODULE_NT, H_KeDeregisterProcessorChangeCallback>
#define I_KeDetachProcess ImportFunc<I_MODULE_NT, H_KeDetachProcess>
#define I_KeDispatchSecondaryInterrupt ImportFunc<I_MODULE_NT, H_KeDispatchSecondaryInterrupt>
#define I_KeEnterCriticalRegion ImportFunc<I_MODULE_NT, H_KeEnterCriticalRegion>
#define I_KeEnterGuardedRegion ImportFunc<I_MODULE_NT, H_KeEnterGuardedRegion>
#define I_KeEnterKernelDebugger ImportFunc<I_MODULE_NT, H_KeEnterKernelDebugger>
#define I_KeEnumerateNextProcessor ImportFunc<I_MODULE_NT, H_KeEnumerateNextProcessor>
#define I_KeExpandKernelStackAndCallout ImportFunc<I_MODULE_NT, H_KeExpandKernelStackAndCallout>
#define I_KeExpandKernelStackAndCalloutEx ImportFunc<I_MODULE_NT, H_KeExpandKernelStackAndCalloutEx>
#define I_KeFindConfigurationEntry ImportFunc<I_MODULE_NT, H_KeFindConfigurationEntry>
#define I_KeFindConfigurationNextEntry ImportFunc<I_MODULE_NT, H_KeFindConfigurationNextEntry>
#define I_KeFindFirstSetLeftAffinityEx ImportFunc<I_MODULE_NT, H_KeFindFirstSetLeftAffinityEx>
#define I_KeFindFirstSetLeftGroupAffinity ImportFunc<I_MODULE_NT, H_KeFindFirstSetLeftGroupAffinity>
#define I_KeFindFirstSetRightAffinityEx ImportFunc<I_MODULE_NT, H_KeFindFirstSetRightAffinityEx>
#define I_KeFindFirstSetRightGroupAffinity ImportFunc<I_MODULE_NT, H_KeFindFirstSetRightGroupAffinity>
#define I_KeFirstGroupAffinityEx ImportFunc<I_MODULE_NT, H_KeFirstGroupAffinityEx>
#define I_KeFlushEntireTb ImportFunc<I_MODULE_NT, H_KeFlushEntireTb>
#define I_KeFlushQueuedDpcs ImportFunc<I_MODULE_NT, H_KeFlushQueuedDpcs>
#define I_KeForceEnableNx ImportFunc<I_MODULE_NT, H_KeForceEnableNx>
#define I_KeFreeCalloutStack ImportFunc<I_MODULE_NT, H_KeFreeCalloutStack>
#define I_KeGenericCallDpc ImportFunc<I_MODULE_NT, H_KeGenericCallDpc>
#define I_KeGetClockOwner ImportFunc<I_MODULE_NT, H_KeGetClockOwner>
#define I_KeGetClockTimerResolution ImportFunc<I_MODULE_NT, H_KeGetClockTimerResolution>
#define I_KeGetCurrentIrql ImportFunc<I_MODULE_NT, H_KeGetCurrentIrql>
#define I_KeGetCurrentNodeNumber ImportFunc<I_MODULE_NT, H_KeGetCurrentNodeNumber>
#define I_KeGetCurrentProcessorNumberEx ImportFunc<I_MODULE_NT, H_KeGetCurrentProcessorNumberEx>
#define I_KeGetCurrentThread ImportFunc<I_MODULE_NT, H_KeGetCurrentThread>
#define I_KeGetNextClockTickDuration ImportFunc<I_MODULE_NT, H_KeGetNextClockTickDuration>
#define I_KeGetProcessorIndexFromNumber ImportFunc<I_MODULE_NT, H_KeGetProcessorIndexFromNumber>
#define I_KeGetProcessorNumberFromIndex ImportFunc<I_MODULE_NT, H_KeGetProcessorNumberFromIndex>
#define I_KeGetRecommendedSharedDataAlignment ImportFunc<I_MODULE_NT, H_KeGetRecommendedSharedDataAlignment>
#define I_KeGetXSaveFeatureFlags ImportFunc<I_MODULE_NT, H_KeGetXSaveFeatureFlags>
#define I_KeHwPolicyLocateResource ImportFunc<I_MODULE_NT, H_KeHwPolicyLocateResource>
#define I_KeInitializeAffinityEx ImportFunc<I_MODULE_NT, H_KeInitializeAffinityEx>
#define I_KeInitializeApc ImportFunc<I_MODULE_NT, H_KeInitializeApc>
#define I_KeInitializeCrashDumpHeader ImportFunc<I_MODULE_NT, H_KeInitializeCrashDumpHeader>
#define I_KeInitializeDeviceQueue ImportFunc<I_MODULE_NT, H_KeInitializeDeviceQueue>
#define I_KeInitializeDpc ImportFunc<I_MODULE_NT, H_KeInitializeDpc>
#define I_KeInitializeEnumerationContext ImportFunc<I_MODULE_NT, H_KeInitializeEnumerationContext>
#define I_KeInitializeEnumerationContextFromGroup ImportFunc<I_MODULE_NT, H_KeInitializeEnumerationContextFromGroup>
#define I_KeInitializeEvent ImportFunc<I_MODULE_NT, H_KeInitializeEvent>
#define I_KeInitializeGuardedMutex ImportFunc<I_MODULE_NT, H_KeInitializeGuardedMutex>
#define I_KeInitializeInterrupt ImportFunc<I_MODULE_NT, H_KeInitializeInterrupt>
#define I_KeInitializeMutant ImportFunc<I_MODULE_NT, H_KeInitializeMutant>
#define I_KeInitializeMutex ImportFunc<I_MODULE_NT, H_KeInitializeMutex>
#define I_KeInitializeQueue ImportFunc<I_MODULE_NT, H_KeInitializeQueue>
#define I_KeInitializeSecondaryInterruptServices ImportFunc<I_MODULE_NT, H_KeInitializeSecondaryInterruptServices>
#define I_KeInitializeSemaphore ImportFunc<I_MODULE_NT, H_KeInitializeSemaphore>
#define I_KeInitializeSpinLock ImportFunc<I_MODULE_NT, H_KeInitializeSpinLock>
#define I_KeInitializeThreadedDpc ImportFunc<I_MODULE_NT, H_KeInitializeThreadedDpc>
#define I_KeInitializeTimer ImportFunc<I_MODULE_NT, H_KeInitializeTimer>
#define I_KeInitializeTimerEx ImportFunc<I_MODULE_NT, H_KeInitializeTimerEx>
#define I_KeInsertByKeyDeviceQueue ImportFunc<I_MODULE_NT, H_KeInsertByKeyDeviceQueue>
#define I_KeInsertDeviceQueue ImportFunc<I_MODULE_NT, H_KeInsertDeviceQueue>
#define I_KeInsertHeadQueue ImportFunc<I_MODULE_NT, H_KeInsertHeadQueue>
#define I_KeInsertQueue ImportFunc<I_MODULE_NT, H_KeInsertQueue>
#define I_KeInsertQueueApc ImportFunc<I_MODULE_NT, H_KeInsertQueueApc>
#define I_KeInsertQueueDpc ImportFunc<I_MODULE_NT, H_KeInsertQueueDpc>
#define I_KeInterlockedClearProcessorAffinityEx ImportFunc<I_MODULE_NT, H_KeInterlockedClearProcessorAffinityEx>
#define I_KeInterlockedSetProcessorAffinityEx ImportFunc<I_MODULE_NT, H_KeInterlockedSetProcessorAffinityEx>
#define I_KeInvalidateAllCaches ImportFunc<I_MODULE_NT, H_KeInvalidateAllCaches>
#define I_KeInvalidateRangeAllCaches ImportFunc<I_MODULE_NT, H_KeInvalidateRangeAllCaches>
#define I_KeIpiGenericCall ImportFunc<I_MODULE_NT, H_KeIpiGenericCall>
#define I_KeIsAttachedProcess ImportFunc<I_MODULE_NT, H_KeIsAttachedProcess>
#define I_KeIsEmptyAffinityEx ImportFunc<I_MODULE_NT, H_KeIsEmptyAffinityEx>
#define I_KeIsEqualAffinityEx ImportFunc<I_MODULE_NT, H_KeIsEqualAffinityEx>
#define I_KeIsExecutingDpc ImportFunc<I_MODULE_NT, H_KeIsExecutingDpc>
#define I_KeIsSingleGroupAffinityEx ImportFunc<I_MODULE_NT, H_KeIsSingleGroupAffinityEx>
#define I_KeIsSubsetAffinityEx ImportFunc<I_MODULE_NT, H_KeIsSubsetAffinityEx>
#define I_KeIsWaitListEmpty ImportFunc<I_MODULE_NT, H_KeIsWaitListEmpty>
#define I_KeLastBranchMSR ImportFunc<I_MODULE_NT, H_KeLastBranchMSR>
#define I_KeLeaveCriticalRegion ImportFunc<I_MODULE_NT, H_KeLeaveCriticalRegion>
#define I_KeLeaveGuardedRegion ImportFunc<I_MODULE_NT, H_KeLeaveGuardedRegion>
#define I_KeLoadMTRR ImportFunc<I_MODULE_NT, H_KeLoadMTRR>
#define I_KeLoaderBlock ImportFunc<I_MODULE_NT, H_KeLoaderBlock>
#define I_KeLowerIrql ImportFunc<I_MODULE_NT, H_KeLowerIrql>
#define I_KeNumberProcessors ImportFunc<I_MODULE_NT, H_KeNumberProcessors>
#define I_KeOrAffinityEx ImportFunc<I_MODULE_NT, H_KeOrAffinityEx>
#define I_KeProcessorGroupAffinity ImportFunc<I_MODULE_NT, H_KeProcessorGroupAffinity>
#define I_KeProfileInterruptWithSource ImportFunc<I_MODULE_NT, H_KeProfileInterruptWithSource>
#define I_KePulseEvent ImportFunc<I_MODULE_NT, H_KePulseEvent>
#define I_KeQueryActiveGroupCount ImportFunc<I_MODULE_NT, H_KeQueryActiveGroupCount>
#define I_KeQueryActiveProcessorAffinity ImportFunc<I_MODULE_NT, H_KeQueryActiveProcessorAffinity>
#define I_KeQueryActiveProcessorCount ImportFunc<I_MODULE_NT, H_KeQueryActiveProcessorCount>
#define I_KeQueryActiveProcessorCountEx ImportFunc<I_MODULE_NT, H_KeQueryActiveProcessorCountEx>
#define I_KeQueryActiveProcessors ImportFunc<I_MODULE_NT, H_KeQueryActiveProcessors>
#define I_KeQueryDpcWatchdogInformation ImportFunc<I_MODULE_NT, H_KeQueryDpcWatchdogInformation>
#define I_KeQueryEffectivePriorityThread ImportFunc<I_MODULE_NT, H_KeQueryEffectivePriorityThread>
#define I_KeQueryGroupAffinity ImportFunc<I_MODULE_NT, H_KeQueryGroupAffinity>
#define I_KeQueryGroupAffinityEx ImportFunc<I_MODULE_NT, H_KeQueryGroupAffinityEx>
#define I_KeQueryHardwareCounterConfiguration ImportFunc<I_MODULE_NT, H_KeQueryHardwareCounterConfiguration>
#define I_KeQueryHighestNodeNumber ImportFunc<I_MODULE_NT, H_KeQueryHighestNodeNumber>
#define I_KeQueryInterruptTimePrecise ImportFunc<I_MODULE_NT, H_KeQueryInterruptTimePrecise>
#define I_KeQueryLogicalProcessorRelationship ImportFunc<I_MODULE_NT, H_KeQueryLogicalProcessorRelationship>
#define I_KeQueryMaximumGroupCount ImportFunc<I_MODULE_NT, H_KeQueryMaximumGroupCount>
#define I_KeQueryMaximumProcessorCount ImportFunc<I_MODULE_NT, H_KeQueryMaximumProcessorCount>
#define I_KeQueryMaximumProcessorCountEx ImportFunc<I_MODULE_NT, H_KeQueryMaximumProcessorCountEx>
#define I_KeQueryNodeActiveAffinity ImportFunc<I_MODULE_NT, H_KeQueryNodeActiveAffinity>
#define I_KeQueryNodeMaximumProcessorCount ImportFunc<I_MODULE_NT, H_KeQueryNodeMaximumProcessorCount>
#define I_KeQueryPrcbAddress ImportFunc<I_MODULE_NT, H_KeQueryPrcbAddress>
#define I_KeQueryPriorityThread ImportFunc<I_MODULE_NT, H_KeQueryPriorityThread>
#define I_KeQueryRuntimeThread ImportFunc<I_MODULE_NT, H_KeQueryRuntimeThread>
#define I_KeQuerySystemTimePrecise ImportFunc<I_MODULE_NT, H_KeQuerySystemTimePrecise>
#define I_KeQueryTimeIncrement ImportFunc<I_MODULE_NT, H_KeQueryTimeIncrement>
#define I_KeQueryTotalCycleTimeThread ImportFunc<I_MODULE_NT, H_KeQueryTotalCycleTimeThread>
#define I_KeQueryUnbiasedInterruptTime ImportFunc<I_MODULE_NT, H_KeQueryUnbiasedInterruptTime>
#define I_KeRaiseIrqlToDpcLevel ImportFunc<I_MODULE_NT, H_KeRaiseIrqlToDpcLevel>
#define I_KeRaiseUserException ImportFunc<I_MODULE_NT, H_KeRaiseUserException>
#define I_KeReadStateEvent ImportFunc<I_MODULE_NT, H_KeReadStateEvent>
#define I_KeReadStateMutant ImportFunc<I_MODULE_NT, H_KeReadStateMutant>
#define I_KeReadStateMutex ImportFunc<I_MODULE_NT, H_KeReadStateMutex>
#define I_KeReadStateQueue ImportFunc<I_MODULE_NT, H_KeReadStateQueue>
#define I_KeReadStateSemaphore ImportFunc<I_MODULE_NT, H_KeReadStateSemaphore>
#define I_KeReadStateTimer ImportFunc<I_MODULE_NT, H_KeReadStateTimer>
#define I_KeRegisterBugCheckCallback ImportFunc<I_MODULE_NT, H_KeRegisterBugCheckCallback>
#define I_KeRegisterBugCheckReasonCallback ImportFunc<I_MODULE_NT, H_KeRegisterBugCheckReasonCallback>
#define I_KeRegisterNmiCallback ImportFunc<I_MODULE_NT, H_KeRegisterNmiCallback>
#define I_KeRegisterProcessorChangeCallback ImportFunc<I_MODULE_NT, H_KeRegisterProcessorChangeCallback>
#define I_KeReleaseGuardedMutex ImportFunc<I_MODULE_NT, H_KeReleaseGuardedMutex>
#define I_KeReleaseGuardedMutexUnsafe ImportFunc<I_MODULE_NT, H_KeReleaseGuardedMutexUnsafe>
#define I_KeReleaseInStackQueuedSpinLock ImportFunc<I_MODULE_NT, H_KeReleaseInStackQueuedSpinLock>
#define I_KeReleaseInStackQueuedSpinLockForDpc ImportFunc<I_MODULE_NT, H_KeReleaseInStackQueuedSpinLockForDpc>
#define I_KeReleaseInStackQueuedSpinLockFromDpcLevel ImportFunc<I_MODULE_NT, H_KeReleaseInStackQueuedSpinLockFromDpcLevel>
#define I_KeReleaseInterruptSpinLock ImportFunc<I_MODULE_NT, H_KeReleaseInterruptSpinLock>
#define I_KeReleaseMutant ImportFunc<I_MODULE_NT, H_KeReleaseMutant>
#define I_KeReleaseMutex ImportFunc<I_MODULE_NT, H_KeReleaseMutex>
#define I_KeReleaseQueuedSpinLock ImportFunc<I_MODULE_NT, H_KeReleaseQueuedSpinLock>
#define I_KeReleaseSemaphore ImportFunc<I_MODULE_NT, H_KeReleaseSemaphore>
#define I_KeReleaseSpinLock ImportFunc<I_MODULE_NT, H_KeReleaseSpinLock>
#define I_KeReleaseSpinLockForDpc ImportFunc<I_MODULE_NT, H_KeReleaseSpinLockForDpc>
#define I_KeReleaseSpinLockFromDpcLevel ImportFunc<I_MODULE_NT, H_KeReleaseSpinLockFromDpcLevel>
#define I_KeRemoveByKeyDeviceQueue ImportFunc<I_MODULE_NT, H_KeRemoveByKeyDeviceQueue>
#define I_KeRemoveByKeyDeviceQueueIfBusy ImportFunc<I_MODULE_NT, H_KeRemoveByKeyDeviceQueueIfBusy>
#define I_KeRemoveDeviceQueue ImportFunc<I_MODULE_NT, H_KeRemoveDeviceQueue>
#define I_KeRemoveEntryDeviceQueue ImportFunc<I_MODULE_NT, H_KeRemoveEntryDeviceQueue>
#define I_KeRemoveGroupAffinityEx ImportFunc<I_MODULE_NT, H_KeRemoveGroupAffinityEx>
#define I_KeRemoveProcessorAffinityEx ImportFunc<I_MODULE_NT, H_KeRemoveProcessorAffinityEx>
#define I_KeRemoveProcessorGroupAffinity ImportFunc<I_MODULE_NT, H_KeRemoveProcessorGroupAffinity>
#define I_KeRemoveQueue ImportFunc<I_MODULE_NT, H_KeRemoveQueue>
#define I_KeRemoveQueueDpc ImportFunc<I_MODULE_NT, H_KeRemoveQueueDpc>
#define I_KeRemoveQueueDpcEx ImportFunc<I_MODULE_NT, H_KeRemoveQueueDpcEx>
#define I_KeRemoveQueueEx ImportFunc<I_MODULE_NT, H_KeRemoveQueueEx>
#define I_KeRemoveSystemServiceTable ImportFunc<I_MODULE_NT, H_KeRemoveSystemServiceTable>
#define I_KeResetEvent ImportFunc<I_MODULE_NT, H_KeResetEvent>
#define I_KeRestoreExtendedProcessorState ImportFunc<I_MODULE_NT, H_KeRestoreExtendedProcessorState>
#define I_KeRestoreFloatingPointState ImportFunc<I_MODULE_NT, H_KeRestoreFloatingPointState>
#define I_KeRevertToUserAffinityThread ImportFunc<I_MODULE_NT, H_KeRevertToUserAffinityThread>
#define I_KeRevertToUserAffinityThreadEx ImportFunc<I_MODULE_NT, H_KeRevertToUserAffinityThreadEx>
#define I_KeRevertToUserGroupAffinityThread ImportFunc<I_MODULE_NT, H_KeRevertToUserGroupAffinityThread>
#define I_KeRundownQueue ImportFunc<I_MODULE_NT, H_KeRundownQueue>
#define I_KeSaveExtendedProcessorState ImportFunc<I_MODULE_NT, H_KeSaveExtendedProcessorState>
#define I_KeSaveFloatingPointState ImportFunc<I_MODULE_NT, H_KeSaveFloatingPointState>
#define I_KeSaveStateForHibernate ImportFunc<I_MODULE_NT, H_KeSaveStateForHibernate>
#define I_KeSetActualBasePriorityThread ImportFunc<I_MODULE_NT, H_KeSetActualBasePriorityThread>
#define I_KeSetAffinityThread ImportFunc<I_MODULE_NT, H_KeSetAffinityThread>
#define I_KeSetBasePriorityThread ImportFunc<I_MODULE_NT, H_KeSetBasePriorityThread>
#define I_KeSetCoalescableTimer ImportFunc<I_MODULE_NT, H_KeSetCoalescableTimer>
#define I_KeSetDmaIoCoherency ImportFunc<I_MODULE_NT, H_KeSetDmaIoCoherency>
#define I_KeSetEvent ImportFunc<I_MODULE_NT, H_KeSetEvent>
#define I_KeSetEventBoostPriority ImportFunc<I_MODULE_NT, H_KeSetEventBoostPriority>
#define I_KeSetHardwareCounterConfiguration ImportFunc<I_MODULE_NT, H_KeSetHardwareCounterConfiguration>
#define I_KeSetIdealProcessorThread ImportFunc<I_MODULE_NT, H_KeSetIdealProcessorThread>
#define I_KeSetImportanceDpc ImportFunc<I_MODULE_NT, H_KeSetImportanceDpc>
#define I_KeSetKernelStackSwapEnable ImportFunc<I_MODULE_NT, H_KeSetKernelStackSwapEnable>
#define I_KeSetPriorityThread ImportFunc<I_MODULE_NT, H_KeSetPriorityThread>
#define I_KeSetProfileIrql ImportFunc<I_MODULE_NT, H_KeSetProfileIrql>
#define I_KeSetSystemAffinityThread ImportFunc<I_MODULE_NT, H_KeSetSystemAffinityThread>
#define I_KeSetSystemAffinityThreadEx ImportFunc<I_MODULE_NT, H_KeSetSystemAffinityThreadEx>
#define I_KeSetSystemGroupAffinityThread ImportFunc<I_MODULE_NT, H_KeSetSystemGroupAffinityThread>
#define I_KeSetTargetProcessorDpc ImportFunc<I_MODULE_NT, H_KeSetTargetProcessorDpc>
#define I_KeSetTargetProcessorDpcEx ImportFunc<I_MODULE_NT, H_KeSetTargetProcessorDpcEx>
#define I_KeSetTimer ImportFunc<I_MODULE_NT, H_KeSetTimer>
#define I_KeSetTimerEx ImportFunc<I_MODULE_NT, H_KeSetTimerEx>
#define I_KeSignalCallDpcDone ImportFunc<I_MODULE_NT, H_KeSignalCallDpcDone>
#define I_KeSignalCallDpcSynchronize ImportFunc<I_MODULE_NT, H_KeSignalCallDpcSynchronize>
#define I_KeStackAttachProcess ImportFunc<I_MODULE_NT, H_KeStackAttachProcess>
#define I_KeStallWhileFrozen ImportFunc<I_MODULE_NT, H_KeStallWhileFrozen>
#define I_KeStartDynamicProcessor ImportFunc<I_MODULE_NT, H_KeStartDynamicProcessor>
#define I_KeSubtractAffinityEx ImportFunc<I_MODULE_NT, H_KeSubtractAffinityEx>
#define I_KeSweepLocalCaches ImportFunc<I_MODULE_NT, H_KeSweepLocalCaches>
#define I_KeSynchronizeExecution ImportFunc<I_MODULE_NT, H_KeSynchronizeExecution>
#define I_KeTestAlertThread ImportFunc<I_MODULE_NT, H_KeTestAlertThread>
#define I_KeTestSpinLock ImportFunc<I_MODULE_NT, H_KeTestSpinLock>
#define I_KeTryToAcquireGuardedMutex ImportFunc<I_MODULE_NT, H_KeTryToAcquireGuardedMutex>
#define I_KeTryToAcquireQueuedSpinLock ImportFunc<I_MODULE_NT, H_KeTryToAcquireQueuedSpinLock>
#define I_KeTryToAcquireQueuedSpinLockRaiseToSynch ImportFunc<I_MODULE_NT, H_KeTryToAcquireQueuedSpinLockRaiseToSynch>
#define I_KeTryToAcquireSpinLockAtDpcLevel ImportFunc<I_MODULE_NT, H_KeTryToAcquireSpinLockAtDpcLevel>
#define I_KeUnstackDetachProcess ImportFunc<I_MODULE_NT, H_KeUnstackDetachProcess>
#define I_KeUserModeCallback ImportFunc<I_MODULE_NT, H_KeUserModeCallback>
#define I_KeWaitForMultipleObjects ImportFunc<I_MODULE_NT, H_KeWaitForMultipleObjects>
#define I_KeWaitForMutexObject ImportFunc<I_MODULE_NT, H_KeWaitForMutexObject>
#define I_KeWaitForSingleObject ImportFunc<I_MODULE_NT, H_KeWaitForSingleObject>
#define I_KeWriteProtectPAT ImportFunc<I_MODULE_NT, H_KeWriteProtectPAT>
#define I_KfRaiseIrql ImportFunc<I_MODULE_NT, H_KfRaiseIrql>
#define I_KiBugCheckData ImportFunc<I_MODULE_NT, H_KiBugCheckData>
#define I_KiCheckForKernelApcDelivery ImportFunc<I_MODULE_NT, H_KiCheckForKernelApcDelivery>
#define I_KitLogFeatureUsage ImportFunc<I_MODULE_NT, H_KitLogFeatureUsage>
#define I_KseQueryDeviceData ImportFunc<I_MODULE_NT, H_KseQueryDeviceData>
#define I_KseQueryDeviceDataList ImportFunc<I_MODULE_NT, H_KseQueryDeviceDataList>
#define I_KseQueryDeviceFlags ImportFunc<I_MODULE_NT, H_KseQueryDeviceFlags>
#define I_KseRegisterShim ImportFunc<I_MODULE_NT, H_KseRegisterShim>
#define I_KseRegisterShimEx ImportFunc<I_MODULE_NT, H_KseRegisterShimEx>
#define I_KseSetDeviceFlags ImportFunc<I_MODULE_NT, H_KseSetDeviceFlags>
#define I_KseUnregisterShim ImportFunc<I_MODULE_NT, H_KseUnregisterShim>
#define I_LdrAccessResource ImportFunc<I_MODULE_NT, H_LdrAccessResource>
#define I_LdrEnumResources ImportFunc<I_MODULE_NT, H_LdrEnumResources>
#define I_LdrFindResourceDirectory_U ImportFunc<I_MODULE_NT, H_LdrFindResourceDirectory_U>
#define I_LdrFindResourceEx_U ImportFunc<I_MODULE_NT, H_LdrFindResourceEx_U>
#define I_LdrFindResource_U ImportFunc<I_MODULE_NT, H_LdrFindResource_U>
#define I_LdrResFindResource ImportFunc<I_MODULE_NT, H_LdrResFindResource>
#define I_LdrResFindResourceDirectory ImportFunc<I_MODULE_NT, H_LdrResFindResourceDirectory>
#define I_LdrResSearchResource ImportFunc<I_MODULE_NT, H_LdrResSearchResource>
#define I_LpcPortObjectType ImportFunc<I_MODULE_NT, H_LpcPortObjectType>
#define I_LpcReplyWaitReplyPort ImportFunc<I_MODULE_NT, H_LpcReplyWaitReplyPort>
#define I_LpcRequestPort ImportFunc<I_MODULE_NT, H_LpcRequestPort>
#define I_LpcRequestWaitReplyPort ImportFunc<I_MODULE_NT, H_LpcRequestWaitReplyPort>
#define I_LpcRequestWaitReplyPortEx ImportFunc<I_MODULE_NT, H_LpcRequestWaitReplyPortEx>
#define I_LpcSendWaitReceivePort ImportFunc<I_MODULE_NT, H_LpcSendWaitReceivePort>
#define I_LsaCallAuthenticationPackage ImportFunc<I_MODULE_NT, H_LsaCallAuthenticationPackage>
#define I_LsaDeregisterLogonProcess ImportFunc<I_MODULE_NT, H_LsaDeregisterLogonProcess>
#define I_LsaFreeReturnBuffer ImportFunc<I_MODULE_NT, H_LsaFreeReturnBuffer>
#define I_LsaLogonUser ImportFunc<I_MODULE_NT, H_LsaLogonUser>
#define I_LsaLookupAuthenticationPackage ImportFunc<I_MODULE_NT, H_LsaLookupAuthenticationPackage>
#define I_LsaRegisterLogonProcess ImportFunc<I_MODULE_NT, H_LsaRegisterLogonProcess>
#define I_Mm64BitPhysicalAddress ImportFunc<I_MODULE_NT, H_Mm64BitPhysicalAddress>
#define I_MmAddPhysicalMemory ImportFunc<I_MODULE_NT, H_MmAddPhysicalMemory>
#define I_MmAddVerifierThunks ImportFunc<I_MODULE_NT, H_MmAddVerifierThunks>
#define I_MmAdjustWorkingSetSize ImportFunc<I_MODULE_NT, H_MmAdjustWorkingSetSize>
#define I_MmAdvanceMdl ImportFunc<I_MODULE_NT, H_MmAdvanceMdl>
#define I_MmAllocateContiguousMemory ImportFunc<I_MODULE_NT, H_MmAllocateContiguousMemory>
#define I_MmAllocateContiguousMemorySpecifyCache ImportFunc<I_MODULE_NT, H_MmAllocateContiguousMemorySpecifyCache>
#define I_MmAllocateContiguousMemorySpecifyCacheNode ImportFunc<I_MODULE_NT, H_MmAllocateContiguousMemorySpecifyCacheNode>
#define I_MmAllocateContiguousNodeMemory ImportFunc<I_MODULE_NT, H_MmAllocateContiguousNodeMemory>
#define I_MmAllocateMappingAddress ImportFunc<I_MODULE_NT, H_MmAllocateMappingAddress>
#define I_MmAllocateMdlForIoSpace ImportFunc<I_MODULE_NT, H_MmAllocateMdlForIoSpace>
#define I_MmAllocateNodePagesForMdlEx ImportFunc<I_MODULE_NT, H_MmAllocateNodePagesForMdlEx>
#define I_MmAllocateNonCachedMemory ImportFunc<I_MODULE_NT, H_MmAllocateNonCachedMemory>
#define I_MmAllocatePagesForMdl ImportFunc<I_MODULE_NT, H_MmAllocatePagesForMdl>
#define I_MmAllocatePagesForMdlEx ImportFunc<I_MODULE_NT, H_MmAllocatePagesForMdlEx>
#define I_MmAreMdlPagesCached ImportFunc<I_MODULE_NT, H_MmAreMdlPagesCached>
#define I_MmBadPointer ImportFunc<I_MODULE_NT, H_MmBadPointer>
#define I_MmBuildMdlForNonPagedPool ImportFunc<I_MODULE_NT, H_MmBuildMdlForNonPagedPool>
#define I_MmCanFileBeTruncated ImportFunc<I_MODULE_NT, H_MmCanFileBeTruncated>
#define I_MmCommitSessionMappedView ImportFunc<I_MODULE_NT, H_MmCommitSessionMappedView>
#define I_MmCopyMemory ImportFunc<I_MODULE_NT, H_MmCopyMemory>
#define I_MmCopyVirtualMemory ImportFunc<I_MODULE_NT, H_MmCopyVirtualMemory>
#define I_MmCreateMdl ImportFunc<I_MODULE_NT, H_MmCreateMdl>
#define I_MmCreateMirror ImportFunc<I_MODULE_NT, H_MmCreateMirror>
#define I_MmCreateSection ImportFunc<I_MODULE_NT, H_MmCreateSection>
#define I_MmDisableModifiedWriteOfSection ImportFunc<I_MODULE_NT, H_MmDisableModifiedWriteOfSection>
#define I_MmDoesFileHaveUserWritableReferences ImportFunc<I_MODULE_NT, H_MmDoesFileHaveUserWritableReferences>
#define I_MmFlushImageSection ImportFunc<I_MODULE_NT, H_MmFlushImageSection>
#define I_MmForceSectionClosed ImportFunc<I_MODULE_NT, H_MmForceSectionClosed>
#define I_MmFreeContiguousMemory ImportFunc<I_MODULE_NT, H_MmFreeContiguousMemory>
#define I_MmFreeContiguousMemorySpecifyCache ImportFunc<I_MODULE_NT, H_MmFreeContiguousMemorySpecifyCache>
#define I_MmFreeMappingAddress ImportFunc<I_MODULE_NT, H_MmFreeMappingAddress>
#define I_MmFreeNonCachedMemory ImportFunc<I_MODULE_NT, H_MmFreeNonCachedMemory>
#define I_MmFreePagesFromMdl ImportFunc<I_MODULE_NT, H_MmFreePagesFromMdl>
#define I_MmGetCacheAttribute ImportFunc<I_MODULE_NT, H_MmGetCacheAttribute>
#define I_MmGetMaximumFileSectionSize ImportFunc<I_MODULE_NT, H_MmGetMaximumFileSectionSize>
#define I_MmGetPhysicalAddress ImportFunc<I_MODULE_NT, H_MmGetPhysicalAddress>
#define I_MmGetPhysicalMemoryRanges ImportFunc<I_MODULE_NT, H_MmGetPhysicalMemoryRanges>
#define I_MmGetSystemRoutineAddress ImportFunc<I_MODULE_NT, H_MmGetSystemRoutineAddress>
#define I_MmGetVirtualForPhysical ImportFunc<I_MODULE_NT, H_MmGetVirtualForPhysical>
#define I_MmGrowKernelStack ImportFunc<I_MODULE_NT, H_MmGrowKernelStack>
#define I_MmHighestUserAddress ImportFunc<I_MODULE_NT, H_MmHighestUserAddress>
#define I_MmIsAddressValid ImportFunc<I_MODULE_NT, H_MmIsAddressValid>
#define I_MmIsDriverSuspectForVerifier ImportFunc<I_MODULE_NT, H_MmIsDriverSuspectForVerifier>
#define I_MmIsDriverVerifying ImportFunc<I_MODULE_NT, H_MmIsDriverVerifying>
#define I_MmIsDriverVerifyingByAddress ImportFunc<I_MODULE_NT, H_MmIsDriverVerifyingByAddress>
#define I_MmIsIoSpaceActive ImportFunc<I_MODULE_NT, H_MmIsIoSpaceActive>
#define I_MmIsNonPagedSystemAddressValid ImportFunc<I_MODULE_NT, H_MmIsNonPagedSystemAddressValid>
#define I_MmIsRecursiveIoFault ImportFunc<I_MODULE_NT, H_MmIsRecursiveIoFault>
#define I_MmIsThisAnNtAsSystem ImportFunc<I_MODULE_NT, H_MmIsThisAnNtAsSystem>
#define I_MmIsVerifierEnabled ImportFunc<I_MODULE_NT, H_MmIsVerifierEnabled>
#define I_MmLockPagableDataSection ImportFunc<I_MODULE_NT, H_MmLockPagableDataSection>
#define I_MmLockPagableImageSection ImportFunc<I_MODULE_NT, H_MmLockPagableImageSection>
#define I_MmLockPagableSectionByHandle ImportFunc<I_MODULE_NT, H_MmLockPagableSectionByHandle>
#define I_MmMapIoSpace ImportFunc<I_MODULE_NT, H_MmMapIoSpace>
#define I_MmMapLockedPages ImportFunc<I_MODULE_NT, H_MmMapLockedPages>
#define I_MmMapLockedPagesSpecifyCache ImportFunc<I_MODULE_NT, H_MmMapLockedPagesSpecifyCache>
#define I_MmMapLockedPagesWithReservedMapping ImportFunc<I_MODULE_NT, H_MmMapLockedPagesWithReservedMapping>
#define I_MmMapMemoryDumpMdl ImportFunc<I_MODULE_NT, H_MmMapMemoryDumpMdl>
#define I_MmMapUserAddressesToPage ImportFunc<I_MODULE_NT, H_MmMapUserAddressesToPage>
#define I_MmMapVideoDisplay ImportFunc<I_MODULE_NT, H_MmMapVideoDisplay>
#define I_MmMapViewInSessionSpace ImportFunc<I_MODULE_NT, H_MmMapViewInSessionSpace>
#define I_MmMapViewInSessionSpaceEx ImportFunc<I_MODULE_NT, H_MmMapViewInSessionSpaceEx>
#define I_MmMapViewInSystemSpace ImportFunc<I_MODULE_NT, H_MmMapViewInSystemSpace>
#define I_MmMapViewInSystemSpaceEx ImportFunc<I_MODULE_NT, H_MmMapViewInSystemSpaceEx>
#define I_MmMapViewOfSection ImportFunc<I_MODULE_NT, H_MmMapViewOfSection>
#define I_MmMarkPhysicalMemoryAsBad ImportFunc<I_MODULE_NT, H_MmMarkPhysicalMemoryAsBad>
#define I_MmMarkPhysicalMemoryAsGood ImportFunc<I_MODULE_NT, H_MmMarkPhysicalMemoryAsGood>
#define I_MmMdlPageContentsState ImportFunc<I_MODULE_NT, H_MmMdlPageContentsState>
#define I_MmMdlPagesAreZero ImportFunc<I_MODULE_NT, H_MmMdlPagesAreZero>
#define I_MmPageEntireDriver ImportFunc<I_MODULE_NT, H_MmPageEntireDriver>
#define I_MmPrefetchPages ImportFunc<I_MODULE_NT, H_MmPrefetchPages>
#define I_MmPrefetchVirtualAddresses ImportFunc<I_MODULE_NT, H_MmPrefetchVirtualAddresses>
#define I_MmProbeAndLockPages ImportFunc<I_MODULE_NT, H_MmProbeAndLockPages>
#define I_MmProbeAndLockProcessPages ImportFunc<I_MODULE_NT, H_MmProbeAndLockProcessPages>
#define I_MmProbeAndLockSelectedPages ImportFunc<I_MODULE_NT, H_MmProbeAndLockSelectedPages>
#define I_MmProtectMdlSystemAddress ImportFunc<I_MODULE_NT, H_MmProtectMdlSystemAddress>
#define I_MmQuerySystemSize ImportFunc<I_MODULE_NT, H_MmQuerySystemSize>
#define I_MmRemovePhysicalMemory ImportFunc<I_MODULE_NT, H_MmRemovePhysicalMemory>
#define I_MmResetDriverPaging ImportFunc<I_MODULE_NT, H_MmResetDriverPaging>
#define I_MmRotatePhysicalView ImportFunc<I_MODULE_NT, H_MmRotatePhysicalView>
#define I_MmSectionObjectType ImportFunc<I_MODULE_NT, H_MmSectionObjectType>
#define I_MmSecureVirtualMemory ImportFunc<I_MODULE_NT, H_MmSecureVirtualMemory>
#define I_MmSetAddressRangeModified ImportFunc<I_MODULE_NT, H_MmSetAddressRangeModified>
#define I_MmSetBankedSection ImportFunc<I_MODULE_NT, H_MmSetBankedSection>
#define I_MmSizeOfMdl ImportFunc<I_MODULE_NT, H_MmSizeOfMdl>
#define I_MmSystemRangeStart ImportFunc<I_MODULE_NT, H_MmSystemRangeStart>
#define I_MmTrimAllSystemPagableMemory ImportFunc<I_MODULE_NT, H_MmTrimAllSystemPagableMemory>
#define I_MmUnlockPagableImageSection ImportFunc<I_MODULE_NT, H_MmUnlockPagableImageSection>
#define I_MmUnlockPages ImportFunc<I_MODULE_NT, H_MmUnlockPages>
#define I_MmUnmapIoSpace ImportFunc<I_MODULE_NT, H_MmUnmapIoSpace>
#define I_MmUnmapLockedPages ImportFunc<I_MODULE_NT, H_MmUnmapLockedPages>
#define I_MmUnmapReservedMapping ImportFunc<I_MODULE_NT, H_MmUnmapReservedMapping>
#define I_MmUnmapVideoDisplay ImportFunc<I_MODULE_NT, H_MmUnmapVideoDisplay>
#define I_MmUnmapViewInSessionSpace ImportFunc<I_MODULE_NT, H_MmUnmapViewInSessionSpace>
#define I_MmUnmapViewInSystemSpace ImportFunc<I_MODULE_NT, H_MmUnmapViewInSystemSpace>
#define I_MmUnmapViewOfSection ImportFunc<I_MODULE_NT, H_MmUnmapViewOfSection>
#define I_MmUnsecureVirtualMemory ImportFunc<I_MODULE_NT, H_MmUnsecureVirtualMemory>
#define I_MmUserProbeAddress ImportFunc<I_MODULE_NT, H_MmUserProbeAddress>
#define I_NlsAnsiCodePage ImportFunc<I_MODULE_NT, H_NlsAnsiCodePage>
#define I_NlsLeadByteInfo ImportFunc<I_MODULE_NT, H_NlsLeadByteInfo>
#define I_NlsMbCodePageTag ImportFunc<I_MODULE_NT, H_NlsMbCodePageTag>
#define I_NlsMbOemCodePageTag ImportFunc<I_MODULE_NT, H_NlsMbOemCodePageTag>
#define I_NlsOemCodePage ImportFunc<I_MODULE_NT, H_NlsOemCodePage>
#define I_NlsOemLeadByteInfo ImportFunc<I_MODULE_NT, H_NlsOemLeadByteInfo>
#define I_NtAddAtom ImportFunc<I_MODULE_NT, H_NtAddAtom>
#define I_NtAdjustPrivilegesToken ImportFunc<I_MODULE_NT, H_NtAdjustPrivilegesToken>
#define I_NtAllocateLocallyUniqueId ImportFunc<I_MODULE_NT, H_NtAllocateLocallyUniqueId>
#define I_NtAllocateUuids ImportFunc<I_MODULE_NT, H_NtAllocateUuids>
#define I_NtAllocateVirtualMemory ImportFunc<I_MODULE_NT, H_NtAllocateVirtualMemory>
#define I_NtBuildGUID ImportFunc<I_MODULE_NT, H_NtBuildGUID>
#define I_NtBuildLab ImportFunc<I_MODULE_NT, H_NtBuildLab>
#define I_NtBuildNumber ImportFunc<I_MODULE_NT, H_NtBuildNumber>
#define I_NtClose ImportFunc<I_MODULE_NT, H_NtClose>
#define I_NtCommitComplete ImportFunc<I_MODULE_NT, H_NtCommitComplete>
#define I_NtCommitEnlistment ImportFunc<I_MODULE_NT, H_NtCommitEnlistment>
#define I_NtCommitTransaction ImportFunc<I_MODULE_NT, H_NtCommitTransaction>
#define I_NtConnectPort ImportFunc<I_MODULE_NT, H_NtConnectPort>
#define I_NtCreateEnlistment ImportFunc<I_MODULE_NT, H_NtCreateEnlistment>
#define I_NtCreateEvent ImportFunc<I_MODULE_NT, H_NtCreateEvent>
#define I_NtCreateFile ImportFunc<I_MODULE_NT, H_NtCreateFile>
#define I_NtCreateResourceManager ImportFunc<I_MODULE_NT, H_NtCreateResourceManager>
#define I_NtCreateSection ImportFunc<I_MODULE_NT, H_NtCreateSection>
#define I_NtCreateTransaction ImportFunc<I_MODULE_NT, H_NtCreateTransaction>
#define I_NtCreateTransactionManager ImportFunc<I_MODULE_NT, H_NtCreateTransactionManager>
#define I_NtDeleteAtom ImportFunc<I_MODULE_NT, H_NtDeleteAtom>
#define I_NtDeleteFile ImportFunc<I_MODULE_NT, H_NtDeleteFile>
#define I_NtDeviceIoControlFile ImportFunc<I_MODULE_NT, H_NtDeviceIoControlFile>
#define I_NtDuplicateObject ImportFunc<I_MODULE_NT, H_NtDuplicateObject>
#define I_NtDuplicateToken ImportFunc<I_MODULE_NT, H_NtDuplicateToken>
#define I_NtEnumerateTransactionObject ImportFunc<I_MODULE_NT, H_NtEnumerateTransactionObject>
#define I_NtFindAtom ImportFunc<I_MODULE_NT, H_NtFindAtom>
#define I_NtFreeVirtualMemory ImportFunc<I_MODULE_NT, H_NtFreeVirtualMemory>
#define I_NtFreezeTransactions ImportFunc<I_MODULE_NT, H_NtFreezeTransactions>
#define I_NtFsControlFile ImportFunc<I_MODULE_NT, H_NtFsControlFile>
#define I_NtGetEnvironmentVariableEx ImportFunc<I_MODULE_NT, H_NtGetEnvironmentVariableEx>
#define I_NtGetNotificationResourceManager ImportFunc<I_MODULE_NT, H_NtGetNotificationResourceManager>
#define I_NtGlobalFlag ImportFunc<I_MODULE_NT, H_NtGlobalFlag>
#define I_NtLockFile ImportFunc<I_MODULE_NT, H_NtLockFile>
#define I_NtMakePermanentObject ImportFunc<I_MODULE_NT, H_NtMakePermanentObject>
#define I_NtMapViewOfSection ImportFunc<I_MODULE_NT, H_NtMapViewOfSection>
#define I_NtNotifyChangeDirectoryFile ImportFunc<I_MODULE_NT, H_NtNotifyChangeDirectoryFile>
#define I_NtOpenEnlistment ImportFunc<I_MODULE_NT, H_NtOpenEnlistment>
#define I_NtOpenFile ImportFunc<I_MODULE_NT, H_NtOpenFile>
#define I_NtOpenProcess ImportFunc<I_MODULE_NT, H_NtOpenProcess>
#define I_NtOpenProcessToken ImportFunc<I_MODULE_NT, H_NtOpenProcessToken>
#define I_NtOpenProcessTokenEx ImportFunc<I_MODULE_NT, H_NtOpenProcessTokenEx>
#define I_NtOpenResourceManager ImportFunc<I_MODULE_NT, H_NtOpenResourceManager>
#define I_NtOpenThread ImportFunc<I_MODULE_NT, H_NtOpenThread>
#define I_NtOpenThreadToken ImportFunc<I_MODULE_NT, H_NtOpenThreadToken>
#define I_NtOpenThreadTokenEx ImportFunc<I_MODULE_NT, H_NtOpenThreadTokenEx>
#define I_NtOpenTransaction ImportFunc<I_MODULE_NT, H_NtOpenTransaction>
#define I_NtOpenTransactionManager ImportFunc<I_MODULE_NT, H_NtOpenTransactionManager>
#define I_NtPrePrepareComplete ImportFunc<I_MODULE_NT, H_NtPrePrepareComplete>
#define I_NtPrePrepareEnlistment ImportFunc<I_MODULE_NT, H_NtPrePrepareEnlistment>
#define I_NtPrepareComplete ImportFunc<I_MODULE_NT, H_NtPrepareComplete>
#define I_NtPrepareEnlistment ImportFunc<I_MODULE_NT, H_NtPrepareEnlistment>
#define I_NtPropagationComplete ImportFunc<I_MODULE_NT, H_NtPropagationComplete>
#define I_NtPropagationFailed ImportFunc<I_MODULE_NT, H_NtPropagationFailed>
#define I_NtQueryDirectoryFile ImportFunc<I_MODULE_NT, H_NtQueryDirectoryFile>
#define I_NtQueryEaFile ImportFunc<I_MODULE_NT, H_NtQueryEaFile>
#define I_NtQueryEnvironmentVariableInfoEx ImportFunc<I_MODULE_NT, H_NtQueryEnvironmentVariableInfoEx>
#define I_NtQueryInformationAtom ImportFunc<I_MODULE_NT, H_NtQueryInformationAtom>
#define I_NtQueryInformationEnlistment ImportFunc<I_MODULE_NT, H_NtQueryInformationEnlistment>
#define I_NtQueryInformationFile ImportFunc<I_MODULE_NT, H_NtQueryInformationFile>
#define I_NtQueryInformationProcess ImportFunc<I_MODULE_NT, H_NtQueryInformationProcess>
#define I_NtQueryInformationResourceManager ImportFunc<I_MODULE_NT, H_NtQueryInformationResourceManager>
#define I_NtQueryInformationThread ImportFunc<I_MODULE_NT, H_NtQueryInformationThread>
#define I_NtQueryInformationToken ImportFunc<I_MODULE_NT, H_NtQueryInformationToken>
#define I_NtQueryInformationTransaction ImportFunc<I_MODULE_NT, H_NtQueryInformationTransaction>
#define I_NtQueryInformationTransactionManager ImportFunc<I_MODULE_NT, H_NtQueryInformationTransactionManager>
#define I_NtQueryQuotaInformationFile ImportFunc<I_MODULE_NT, H_NtQueryQuotaInformationFile>
#define I_NtQuerySecurityAttributesToken ImportFunc<I_MODULE_NT, H_NtQuerySecurityAttributesToken>
#define I_NtQuerySecurityObject ImportFunc<I_MODULE_NT, H_NtQuerySecurityObject>
#define I_NtQuerySystemInformation ImportFunc<I_MODULE_NT, H_NtQuerySystemInformation>
#define I_NtQuerySystemInformationEx ImportFunc<I_MODULE_NT, H_NtQuerySystemInformationEx>
#define I_NtQueryVolumeInformationFile ImportFunc<I_MODULE_NT, H_NtQueryVolumeInformationFile>
#define I_NtReadFile ImportFunc<I_MODULE_NT, H_NtReadFile>
#define I_NtReadOnlyEnlistment ImportFunc<I_MODULE_NT, H_NtReadOnlyEnlistment>
#define I_NtRecoverEnlistment ImportFunc<I_MODULE_NT, H_NtRecoverEnlistment>
#define I_NtRecoverResourceManager ImportFunc<I_MODULE_NT, H_NtRecoverResourceManager>
#define I_NtRecoverTransactionManager ImportFunc<I_MODULE_NT, H_NtRecoverTransactionManager>
#define I_NtRequestPort ImportFunc<I_MODULE_NT, H_NtRequestPort>
#define I_NtRequestWaitReplyPort ImportFunc<I_MODULE_NT, H_NtRequestWaitReplyPort>
#define I_NtRollbackComplete ImportFunc<I_MODULE_NT, H_NtRollbackComplete>
#define I_NtRollbackEnlistment ImportFunc<I_MODULE_NT, H_NtRollbackEnlistment>
#define I_NtRollbackTransaction ImportFunc<I_MODULE_NT, H_NtRollbackTransaction>
#define I_NtSetCachedSigningLevel ImportFunc<I_MODULE_NT, H_NtSetCachedSigningLevel>
#define I_NtSetEaFile ImportFunc<I_MODULE_NT, H_NtSetEaFile>
#define I_NtSetEvent ImportFunc<I_MODULE_NT, H_NtSetEvent>
#define I_NtSetInformationEnlistment ImportFunc<I_MODULE_NT, H_NtSetInformationEnlistment>
#define I_NtSetInformationFile ImportFunc<I_MODULE_NT, H_NtSetInformationFile>
#define I_NtSetInformationProcess ImportFunc<I_MODULE_NT, H_NtSetInformationProcess>
#define I_NtSetInformationResourceManager ImportFunc<I_MODULE_NT, H_NtSetInformationResourceManager>
#define I_NtSetInformationThread ImportFunc<I_MODULE_NT, H_NtSetInformationThread>
#define I_NtSetInformationToken ImportFunc<I_MODULE_NT, H_NtSetInformationToken>
#define I_NtSetInformationTransaction ImportFunc<I_MODULE_NT, H_NtSetInformationTransaction>
#define I_NtSetInformationVirtualMemory ImportFunc<I_MODULE_NT, H_NtSetInformationVirtualMemory>
#define I_NtSetQuotaInformationFile ImportFunc<I_MODULE_NT, H_NtSetQuotaInformationFile>
#define I_NtSetSecurityObject ImportFunc<I_MODULE_NT, H_NtSetSecurityObject>
#define I_NtSetVolumeInformationFile ImportFunc<I_MODULE_NT, H_NtSetVolumeInformationFile>
#define I_NtShutdownSystem ImportFunc<I_MODULE_NT, H_NtShutdownSystem>
#define I_NtThawTransactions ImportFunc<I_MODULE_NT, H_NtThawTransactions>
#define I_NtTraceControl ImportFunc<I_MODULE_NT, H_NtTraceControl>
#define I_NtTraceEvent ImportFunc<I_MODULE_NT, H_NtTraceEvent>
#define I_NtUnlockFile ImportFunc<I_MODULE_NT, H_NtUnlockFile>
#define I_NtVdmControl ImportFunc<I_MODULE_NT, H_NtVdmControl>
#define I_NtWaitForSingleObject ImportFunc<I_MODULE_NT, H_NtWaitForSingleObject>
#define I_NtWriteFile ImportFunc<I_MODULE_NT, H_NtWriteFile>
#define I_ObAssignSecurity ImportFunc<I_MODULE_NT, H_ObAssignSecurity>
#define I_ObCheckCreateObjectAccess ImportFunc<I_MODULE_NT, H_ObCheckCreateObjectAccess>
#define I_ObCheckObjectAccess ImportFunc<I_MODULE_NT, H_ObCheckObjectAccess>
#define I_ObCloseHandle ImportFunc<I_MODULE_NT, H_ObCloseHandle>
#define I_ObCreateObject ImportFunc<I_MODULE_NT, H_ObCreateObject>
#define I_ObCreateObjectType ImportFunc<I_MODULE_NT, H_ObCreateObjectType>
#define I_ObDeleteCapturedInsertInfo ImportFunc<I_MODULE_NT, H_ObDeleteCapturedInsertInfo>
#define I_ObDereferenceObject ImportFunc<I_MODULE_NT, H_ObDereferenceObject>
#define I_ObDereferenceObjectDeferDelete ImportFunc<I_MODULE_NT, H_ObDereferenceObjectDeferDelete>
#define I_ObDereferenceObjectDeferDeleteWithTag ImportFunc<I_MODULE_NT, H_ObDereferenceObjectDeferDeleteWithTag>
#define I_ObDereferenceSecurityDescriptor ImportFunc<I_MODULE_NT, H_ObDereferenceSecurityDescriptor>
#define I_ObDuplicateObject ImportFunc<I_MODULE_NT, H_ObDuplicateObject>
#define I_ObFindHandleForObject ImportFunc<I_MODULE_NT, H_ObFindHandleForObject>
#define I_ObGetFilterVersion ImportFunc<I_MODULE_NT, H_ObGetFilterVersion>
#define I_ObGetObjectSecurity ImportFunc<I_MODULE_NT, H_ObGetObjectSecurity>
#define I_ObGetObjectType ImportFunc<I_MODULE_NT, H_ObGetObjectType>
#define I_ObInsertObject ImportFunc<I_MODULE_NT, H_ObInsertObject>
#define I_ObIsDosDeviceLocallyMapped ImportFunc<I_MODULE_NT, H_ObIsDosDeviceLocallyMapped>
#define I_ObIsKernelHandle ImportFunc<I_MODULE_NT, H_ObIsKernelHandle>
#define I_ObLogSecurityDescriptor ImportFunc<I_MODULE_NT, H_ObLogSecurityDescriptor>
#define I_ObMakeTemporaryObject ImportFunc<I_MODULE_NT, H_ObMakeTemporaryObject>
#define I_ObOpenObjectByName ImportFunc<I_MODULE_NT, H_ObOpenObjectByName>
#define I_ObOpenObjectByPointer ImportFunc<I_MODULE_NT, H_ObOpenObjectByPointer>
#define I_ObOpenObjectByPointerWithTag ImportFunc<I_MODULE_NT, H_ObOpenObjectByPointerWithTag>
#define I_ObQueryNameInfo ImportFunc<I_MODULE_NT, H_ObQueryNameInfo>
#define I_ObQueryNameString ImportFunc<I_MODULE_NT, H_ObQueryNameString>
#define I_ObQueryObjectAuditingByHandle ImportFunc<I_MODULE_NT, H_ObQueryObjectAuditingByHandle>
#define I_ObReferenceObjectByHandle ImportFunc<I_MODULE_NT, H_ObReferenceObjectByHandle>
#define I_ObReferenceObjectByHandleWithTag ImportFunc<I_MODULE_NT, H_ObReferenceObjectByHandleWithTag>
#define I_ObReferenceObjectByName ImportFunc<I_MODULE_NT, H_ObReferenceObjectByName>
#define I_ObReferenceObjectByPointer ImportFunc<I_MODULE_NT, H_ObReferenceObjectByPointer>
#define I_ObReferenceObjectByPointerWithTag ImportFunc<I_MODULE_NT, H_ObReferenceObjectByPointerWithTag>
#define I_ObReferenceObjectSafe ImportFunc<I_MODULE_NT, H_ObReferenceObjectSafe>
#define I_ObReferenceObjectSafeWithTag ImportFunc<I_MODULE_NT, H_ObReferenceObjectSafeWithTag>
#define I_ObReferenceSecurityDescriptor ImportFunc<I_MODULE_NT, H_ObReferenceSecurityDescriptor>
#define I_ObRegisterCallbacks ImportFunc<I_MODULE_NT, H_ObRegisterCallbacks>
#define I_ObReleaseObjectSecurity ImportFunc<I_MODULE_NT, H_ObReleaseObjectSecurity>
#define I_ObSetHandleAttributes ImportFunc<I_MODULE_NT, H_ObSetHandleAttributes>
#define I_ObSetSecurityDescriptorInfo ImportFunc<I_MODULE_NT, H_ObSetSecurityDescriptorInfo>
#define I_ObSetSecurityObjectByPointer ImportFunc<I_MODULE_NT, H_ObSetSecurityObjectByPointer>
#define I_ObUnRegisterCallbacks ImportFunc<I_MODULE_NT, H_ObUnRegisterCallbacks>
#define I_ObWaitForMultipleObjects ImportFunc<I_MODULE_NT, H_ObWaitForMultipleObjects>
#define I_ObWaitForSingleObject ImportFunc<I_MODULE_NT, H_ObWaitForSingleObject>
#define I_ObfDereferenceObject ImportFunc<I_MODULE_NT, H_ObfDereferenceObject>
#define I_ObfDereferenceObjectWithTag ImportFunc<I_MODULE_NT, H_ObfDereferenceObjectWithTag>
#define I_ObfReferenceObject ImportFunc<I_MODULE_NT, H_ObfReferenceObject>
#define I_ObfReferenceObjectWithTag ImportFunc<I_MODULE_NT, H_ObfReferenceObjectWithTag>
#define I_POGOBuffer ImportFunc<I_MODULE_NT, H_POGOBuffer>
#define I_PcwAddInstance ImportFunc<I_MODULE_NT, H_PcwAddInstance>
#define I_PcwCloseInstance ImportFunc<I_MODULE_NT, H_PcwCloseInstance>
#define I_PcwCreateInstance ImportFunc<I_MODULE_NT, H_PcwCreateInstance>
#define I_PcwRegister ImportFunc<I_MODULE_NT, H_PcwRegister>
#define I_PcwUnregister ImportFunc<I_MODULE_NT, H_PcwUnregister>
#define I_PfFileInfoNotify ImportFunc<I_MODULE_NT, H_PfFileInfoNotify>
#define I_PfxFindPrefix ImportFunc<I_MODULE_NT, H_PfxFindPrefix>
#define I_PfxInitialize ImportFunc<I_MODULE_NT, H_PfxInitialize>
#define I_PfxInsertPrefix ImportFunc<I_MODULE_NT, H_PfxInsertPrefix>
#define I_PfxRemovePrefix ImportFunc<I_MODULE_NT, H_PfxRemovePrefix>
#define I_PoCallDriver ImportFunc<I_MODULE_NT, H_PoCallDriver>
#define I_PoCancelDeviceNotify ImportFunc<I_MODULE_NT, H_PoCancelDeviceNotify>
#define I_PoClearPowerRequest ImportFunc<I_MODULE_NT, H_PoClearPowerRequest>
#define I_PoCreatePowerRequest ImportFunc<I_MODULE_NT, H_PoCreatePowerRequest>
#define I_PoDeletePowerRequest ImportFunc<I_MODULE_NT, H_PoDeletePowerRequest>
#define I_PoDisableSleepStates ImportFunc<I_MODULE_NT, H_PoDisableSleepStates>
#define I_PoEndDeviceBusy ImportFunc<I_MODULE_NT, H_PoEndDeviceBusy>
#define I_PoFxActivateComponent ImportFunc<I_MODULE_NT, H_PoFxActivateComponent>
#define I_PoFxCompleteDevicePowerNotRequired ImportFunc<I_MODULE_NT, H_PoFxCompleteDevicePowerNotRequired>
#define I_PoFxCompleteIdleCondition ImportFunc<I_MODULE_NT, H_PoFxCompleteIdleCondition>
#define I_PoFxCompleteIdleState ImportFunc<I_MODULE_NT, H_PoFxCompleteIdleState>
#define I_PoFxIdleComponent ImportFunc<I_MODULE_NT, H_PoFxIdleComponent>
#define I_PoFxNotifySurprisePowerOn ImportFunc<I_MODULE_NT, H_PoFxNotifySurprisePowerOn>
#define I_PoFxPowerControl ImportFunc<I_MODULE_NT, H_PoFxPowerControl>
#define I_PoFxPowerOnCrashdumpDevice ImportFunc<I_MODULE_NT, H_PoFxPowerOnCrashdumpDevice>
#define I_PoFxProcessorNotification ImportFunc<I_MODULE_NT, H_PoFxProcessorNotification>
#define I_PoFxRegisterCoreDevice ImportFunc<I_MODULE_NT, H_PoFxRegisterCoreDevice>
#define I_PoFxRegisterCrashdumpDevice ImportFunc<I_MODULE_NT, H_PoFxRegisterCrashdumpDevice>
#define I_PoFxRegisterDevice ImportFunc<I_MODULE_NT, H_PoFxRegisterDevice>
#define I_PoFxRegisterPlugin ImportFunc<I_MODULE_NT, H_PoFxRegisterPlugin>
#define I_PoFxRegisterPluginEx ImportFunc<I_MODULE_NT, H_PoFxRegisterPluginEx>
#define I_PoFxRegisterPrimaryDevice ImportFunc<I_MODULE_NT, H_PoFxRegisterPrimaryDevice>
#define I_PoFxReportDevicePoweredOn ImportFunc<I_MODULE_NT, H_PoFxReportDevicePoweredOn>
#define I_PoFxSetComponentLatency ImportFunc<I_MODULE_NT, H_PoFxSetComponentLatency>
#define I_PoFxSetComponentResidency ImportFunc<I_MODULE_NT, H_PoFxSetComponentResidency>
#define I_PoFxSetComponentWake ImportFunc<I_MODULE_NT, H_PoFxSetComponentWake>
#define I_PoFxSetDeviceIdleTimeout ImportFunc<I_MODULE_NT, H_PoFxSetDeviceIdleTimeout>
#define I_PoFxStartDevicePowerManagement ImportFunc<I_MODULE_NT, H_PoFxStartDevicePowerManagement>
#define I_PoFxUnregisterDevice ImportFunc<I_MODULE_NT, H_PoFxUnregisterDevice>
#define I_PoGetProcessorIdleAccounting ImportFunc<I_MODULE_NT, H_PoGetProcessorIdleAccounting>
#define I_PoGetSystemWake ImportFunc<I_MODULE_NT, H_PoGetSystemWake>
#define I_PoInitiateProcessorWake ImportFunc<I_MODULE_NT, H_PoInitiateProcessorWake>
#define I_PoLatencySensitivityHint ImportFunc<I_MODULE_NT, H_PoLatencySensitivityHint>
#define I_PoNotifyVSyncChange ImportFunc<I_MODULE_NT, H_PoNotifyVSyncChange>
#define I_PoQueryWatchdogTime ImportFunc<I_MODULE_NT, H_PoQueryWatchdogTime>
#define I_PoQueueShutdownWorkItem ImportFunc<I_MODULE_NT, H_PoQueueShutdownWorkItem>
#define I_PoReenableSleepStates ImportFunc<I_MODULE_NT, H_PoReenableSleepStates>
#define I_PoRegisterCoalescingCallback ImportFunc<I_MODULE_NT, H_PoRegisterCoalescingCallback>
#define I_PoRegisterDeviceForIdleDetection ImportFunc<I_MODULE_NT, H_PoRegisterDeviceForIdleDetection>
#define I_PoRegisterDeviceNotify ImportFunc<I_MODULE_NT, H_PoRegisterDeviceNotify>
#define I_PoRegisterPowerSettingCallback ImportFunc<I_MODULE_NT, H_PoRegisterPowerSettingCallback>
#define I_PoRegisterSystemState ImportFunc<I_MODULE_NT, H_PoRegisterSystemState>
#define I_PoRequestPowerIrp ImportFunc<I_MODULE_NT, H_PoRequestPowerIrp>
#define I_PoRequestShutdownEvent ImportFunc<I_MODULE_NT, H_PoRequestShutdownEvent>
#define I_PoSetDeviceBusyEx ImportFunc<I_MODULE_NT, H_PoSetDeviceBusyEx>
#define I_PoSetFixedWakeSource ImportFunc<I_MODULE_NT, H_PoSetFixedWakeSource>
#define I_PoSetHiberRange ImportFunc<I_MODULE_NT, H_PoSetHiberRange>
#define I_PoSetPowerRequest ImportFunc<I_MODULE_NT, H_PoSetPowerRequest>
#define I_PoSetPowerState ImportFunc<I_MODULE_NT, H_PoSetPowerState>
#define I_PoSetSystemState ImportFunc<I_MODULE_NT, H_PoSetSystemState>
#define I_PoSetSystemWake ImportFunc<I_MODULE_NT, H_PoSetSystemWake>
#define I_PoSetUserPresent ImportFunc<I_MODULE_NT, H_PoSetUserPresent>
#define I_PoShutdownBugCheck ImportFunc<I_MODULE_NT, H_PoShutdownBugCheck>
#define I_PoStartDeviceBusy ImportFunc<I_MODULE_NT, H_PoStartDeviceBusy>
#define I_PoStartNextPowerIrp ImportFunc<I_MODULE_NT, H_PoStartNextPowerIrp>
#define I_PoUnregisterCoalescingCallback ImportFunc<I_MODULE_NT, H_PoUnregisterCoalescingCallback>
#define I_PoUnregisterPowerSettingCallback ImportFunc<I_MODULE_NT, H_PoUnregisterPowerSettingCallback>
#define I_PoUnregisterSystemState ImportFunc<I_MODULE_NT, H_PoUnregisterSystemState>
#define I_PoUserShutdownCancelled ImportFunc<I_MODULE_NT, H_PoUserShutdownCancelled>
#define I_PoUserShutdownInitiated ImportFunc<I_MODULE_NT, H_PoUserShutdownInitiated>
#define I_ProbeForRead ImportFunc<I_MODULE_NT, H_ProbeForRead>
#define I_ProbeForWrite ImportFunc<I_MODULE_NT, H_ProbeForWrite>
#define I_PsAcquireProcessExitSynchronization ImportFunc<I_MODULE_NT, H_PsAcquireProcessExitSynchronization>
#define I_PsAssignImpersonationToken ImportFunc<I_MODULE_NT, H_PsAssignImpersonationToken>
#define I_PsChargePoolQuota ImportFunc<I_MODULE_NT, H_PsChargePoolQuota>
#define I_PsChargeProcessNonPagedPoolQuota ImportFunc<I_MODULE_NT, H_PsChargeProcessNonPagedPoolQuota>
#define I_PsChargeProcessPagedPoolQuota ImportFunc<I_MODULE_NT, H_PsChargeProcessPagedPoolQuota>
#define I_PsChargeProcessPoolQuota ImportFunc<I_MODULE_NT, H_PsChargeProcessPoolQuota>
#define I_PsChargeProcessWakeCounter ImportFunc<I_MODULE_NT, H_PsChargeProcessWakeCounter>
#define I_PsCreateSystemThread ImportFunc<I_MODULE_NT, H_PsCreateSystemThread>
#define I_PsCreateSystemThreadEx ImportFunc<I_MODULE_NT, H_PsCreateSystemThreadEx>
#define I_PsDereferenceImpersonationToken ImportFunc<I_MODULE_NT, H_PsDereferenceImpersonationToken>
#define I_PsDereferenceKernelStack ImportFunc<I_MODULE_NT, H_PsDereferenceKernelStack>
#define I_PsDereferencePrimaryToken ImportFunc<I_MODULE_NT, H_PsDereferencePrimaryToken>
#define I_PsDisableImpersonation ImportFunc<I_MODULE_NT, H_PsDisableImpersonation>
#define I_PsEnterPriorityRegion ImportFunc<I_MODULE_NT, H_PsEnterPriorityRegion>
#define I_PsEstablishWin32Callouts ImportFunc<I_MODULE_NT, H_PsEstablishWin32Callouts>
#define I_PsGetContextThread ImportFunc<I_MODULE_NT, H_PsGetContextThread>
#define I_PsGetCurrentProcess ImportFunc<I_MODULE_NT, H_PsGetCurrentProcess>
#define I_PsGetCurrentProcessId ImportFunc<I_MODULE_NT, H_PsGetCurrentProcessId>
#define I_PsGetCurrentProcessSessionId ImportFunc<I_MODULE_NT, H_PsGetCurrentProcessSessionId>
#define I_PsGetCurrentProcessWin32Process ImportFunc<I_MODULE_NT, H_PsGetCurrentProcessWin32Process>
#define I_PsGetCurrentProcessWow64Process ImportFunc<I_MODULE_NT, H_PsGetCurrentProcessWow64Process>
#define I_PsGetCurrentThread ImportFunc<I_MODULE_NT, H_PsGetCurrentThread>
#define I_PsGetCurrentThreadId ImportFunc<I_MODULE_NT, H_PsGetCurrentThreadId>
#define I_PsGetCurrentThreadPreviousMode ImportFunc<I_MODULE_NT, H_PsGetCurrentThreadPreviousMode>
#define I_PsGetCurrentThreadProcess ImportFunc<I_MODULE_NT, H_PsGetCurrentThreadProcess>
#define I_PsGetCurrentThreadProcessId ImportFunc<I_MODULE_NT, H_PsGetCurrentThreadProcessId>
#define I_PsGetCurrentThreadStackBase ImportFunc<I_MODULE_NT, H_PsGetCurrentThreadStackBase>
#define I_PsGetCurrentThreadStackLimit ImportFunc<I_MODULE_NT, H_PsGetCurrentThreadStackLimit>
#define I_PsGetCurrentThreadTeb ImportFunc<I_MODULE_NT, H_PsGetCurrentThreadTeb>
#define I_PsGetCurrentThreadWin32Thread ImportFunc<I_MODULE_NT, H_PsGetCurrentThreadWin32Thread>
#define I_PsGetCurrentThreadWin32ThreadAndEnterCriticalRegion ImportFunc<I_MODULE_NT, H_PsGetCurrentThreadWin32ThreadAndEnterCriticalRegion>
#define I_PsGetJobLock ImportFunc<I_MODULE_NT, H_PsGetJobLock>
#define I_PsGetJobSessionId ImportFunc<I_MODULE_NT, H_PsGetJobSessionId>
#define I_PsGetJobUIRestrictionsClass ImportFunc<I_MODULE_NT, H_PsGetJobUIRestrictionsClass>
#define I_PsGetProcessCommonJob ImportFunc<I_MODULE_NT, H_PsGetProcessCommonJob>
#define I_PsGetProcessCreateTimeQuadPart ImportFunc<I_MODULE_NT, H_PsGetProcessCreateTimeQuadPart>
#define I_PsGetProcessDebugPort ImportFunc<I_MODULE_NT, H_PsGetProcessDebugPort>
#define I_PsGetProcessExitProcessCalled ImportFunc<I_MODULE_NT, H_PsGetProcessExitProcessCalled>
#define I_PsGetProcessExitStatus ImportFunc<I_MODULE_NT, H_PsGetProcessExitStatus>
#define I_PsGetProcessExitTime ImportFunc<I_MODULE_NT, H_PsGetProcessExitTime>
#define I_PsGetProcessId ImportFunc<I_MODULE_NT, H_PsGetProcessId>
#define I_PsGetProcessImageFileName ImportFunc<I_MODULE_NT, H_PsGetProcessImageFileName>
#define I_PsGetProcessInheritedFromUniqueProcessId ImportFunc<I_MODULE_NT, H_PsGetProcessInheritedFromUniqueProcessId>
#define I_PsGetProcessJob ImportFunc<I_MODULE_NT, H_PsGetProcessJob>
#define I_PsGetProcessPeb ImportFunc<I_MODULE_NT, H_PsGetProcessPeb>
#define I_PsGetProcessPriorityClass ImportFunc<I_MODULE_NT, H_PsGetProcessPriorityClass>
#define I_PsGetProcessProtection ImportFunc<I_MODULE_NT, H_PsGetProcessProtection>
#define I_PsGetProcessSectionBaseAddress ImportFunc<I_MODULE_NT, H_PsGetProcessSectionBaseAddress>
#define I_PsGetProcessSecurityPort ImportFunc<I_MODULE_NT, H_PsGetProcessSecurityPort>
#define I_PsGetProcessSessionId ImportFunc<I_MODULE_NT, H_PsGetProcessSessionId>
#define I_PsGetProcessSessionIdEx ImportFunc<I_MODULE_NT, H_PsGetProcessSessionIdEx>
#define I_PsGetProcessSignatureLevel ImportFunc<I_MODULE_NT, H_PsGetProcessSignatureLevel>
#define I_PsGetProcessWin32Process ImportFunc<I_MODULE_NT, H_PsGetProcessWin32Process>
#define I_PsGetProcessWin32WindowStation ImportFunc<I_MODULE_NT, H_PsGetProcessWin32WindowStation>
#define I_PsGetProcessWow64Process ImportFunc<I_MODULE_NT, H_PsGetProcessWow64Process>
#define I_PsGetThreadExitStatus ImportFunc<I_MODULE_NT, H_PsGetThreadExitStatus>
#define I_PsGetThreadFreezeCount ImportFunc<I_MODULE_NT, H_PsGetThreadFreezeCount>
#define I_PsGetThreadHardErrorsAreDisabled ImportFunc<I_MODULE_NT, H_PsGetThreadHardErrorsAreDisabled>
#define I_PsGetThreadId ImportFunc<I_MODULE_NT, H_PsGetThreadId>
#define I_PsGetThreadProcess ImportFunc<I_MODULE_NT, H_PsGetThreadProcess>
#define I_PsGetThreadProcessId ImportFunc<I_MODULE_NT, H_PsGetThreadProcessId>
#define I_PsGetThreadSessionId ImportFunc<I_MODULE_NT, H_PsGetThreadSessionId>
#define I_PsGetThreadTeb ImportFunc<I_MODULE_NT, H_PsGetThreadTeb>
#define I_PsGetThreadWin32Thread ImportFunc<I_MODULE_NT, H_PsGetThreadWin32Thread>
#define I_PsGetVersion ImportFunc<I_MODULE_NT, H_PsGetVersion>
#define I_PsImpersonateClient ImportFunc<I_MODULE_NT, H_PsImpersonateClient>
#define I_PsInitialSystemProcess ImportFunc<I_MODULE_NT, H_PsInitialSystemProcess>
#define I_PsIsCurrentThreadPrefetching ImportFunc<I_MODULE_NT, H_PsIsCurrentThreadPrefetching>
#define I_PsIsDiskCountersEnabled ImportFunc<I_MODULE_NT, H_PsIsDiskCountersEnabled>
#define I_PsIsProcessBeingDebugged ImportFunc<I_MODULE_NT, H_PsIsProcessBeingDebugged>
#define I_PsIsProtectedProcess ImportFunc<I_MODULE_NT, H_PsIsProtectedProcess>
#define I_PsIsProtectedProcessLight ImportFunc<I_MODULE_NT, H_PsIsProtectedProcessLight>
#define I_PsIsSystemProcess ImportFunc<I_MODULE_NT, H_PsIsSystemProcess>
#define I_PsIsSystemThread ImportFunc<I_MODULE_NT, H_PsIsSystemThread>
#define I_PsIsThreadImpersonating ImportFunc<I_MODULE_NT, H_PsIsThreadImpersonating>
#define I_PsIsThreadTerminating ImportFunc<I_MODULE_NT, H_PsIsThreadTerminating>
#define I_PsJobType ImportFunc<I_MODULE_NT, H_PsJobType>
#define I_PsLeavePriorityRegion ImportFunc<I_MODULE_NT, H_PsLeavePriorityRegion>
#define I_PsLookupProcessByProcessId ImportFunc<I_MODULE_NT, H_PsLookupProcessByProcessId>
#define I_PsLookupProcessThreadByCid ImportFunc<I_MODULE_NT, H_PsLookupProcessThreadByCid>
#define I_PsLookupThreadByThreadId ImportFunc<I_MODULE_NT, H_PsLookupThreadByThreadId>
#define I_PsProcessType ImportFunc<I_MODULE_NT, H_PsProcessType>
#define I_PsQueryProcessAttributesByToken ImportFunc<I_MODULE_NT, H_PsQueryProcessAttributesByToken>
#define I_PsQueryProcessExceptionFlags ImportFunc<I_MODULE_NT, H_PsQueryProcessExceptionFlags>
#define I_PsQueryTotalCycleTimeProcess ImportFunc<I_MODULE_NT, H_PsQueryTotalCycleTimeProcess>
#define I_PsReferenceImpersonationToken ImportFunc<I_MODULE_NT, H_PsReferenceImpersonationToken>
#define I_PsReferenceKernelStack ImportFunc<I_MODULE_NT, H_PsReferenceKernelStack>
#define I_PsReferencePrimaryToken ImportFunc<I_MODULE_NT, H_PsReferencePrimaryToken>
#define I_PsReferenceProcessFilePointer ImportFunc<I_MODULE_NT, H_PsReferenceProcessFilePointer>
#define I_PsReleaseProcessExitSynchronization ImportFunc<I_MODULE_NT, H_PsReleaseProcessExitSynchronization>
#define I_PsReleaseProcessWakeCounter ImportFunc<I_MODULE_NT, H_PsReleaseProcessWakeCounter>
#define I_PsRemoveCreateThreadNotifyRoutine ImportFunc<I_MODULE_NT, H_PsRemoveCreateThreadNotifyRoutine>
#define I_PsRemoveLoadImageNotifyRoutine ImportFunc<I_MODULE_NT, H_PsRemoveLoadImageNotifyRoutine>
#define I_PsRestoreImpersonation ImportFunc<I_MODULE_NT, H_PsRestoreImpersonation>
#define I_PsResumeProcess ImportFunc<I_MODULE_NT, H_PsResumeProcess>
#define I_PsReturnPoolQuota ImportFunc<I_MODULE_NT, H_PsReturnPoolQuota>
#define I_PsReturnProcessNonPagedPoolQuota ImportFunc<I_MODULE_NT, H_PsReturnProcessNonPagedPoolQuota>
#define I_PsReturnProcessPagedPoolQuota ImportFunc<I_MODULE_NT, H_PsReturnProcessPagedPoolQuota>
#define I_PsRevertThreadToSelf ImportFunc<I_MODULE_NT, H_PsRevertThreadToSelf>
#define I_PsRevertToSelf ImportFunc<I_MODULE_NT, H_PsRevertToSelf>
#define I_PsSetContextThread ImportFunc<I_MODULE_NT, H_PsSetContextThread>
#define I_PsSetCreateProcessNotifyRoutine ImportFunc<I_MODULE_NT, H_PsSetCreateProcessNotifyRoutine>
#define I_PsSetCreateProcessNotifyRoutineEx ImportFunc<I_MODULE_NT, H_PsSetCreateProcessNotifyRoutineEx>
#define I_PsSetCreateThreadNotifyRoutine ImportFunc<I_MODULE_NT, H_PsSetCreateThreadNotifyRoutine>
#define I_PsSetCurrentThreadPrefetching ImportFunc<I_MODULE_NT, H_PsSetCurrentThreadPrefetching>
#define I_PsSetLegoNotifyRoutine ImportFunc<I_MODULE_NT, H_PsSetLegoNotifyRoutine>
#define I_PsSetLoadImageNotifyRoutine ImportFunc<I_MODULE_NT, H_PsSetLoadImageNotifyRoutine>
#define I_PsSetProcessPriorityByClass ImportFunc<I_MODULE_NT, H_PsSetProcessPriorityByClass>
#define I_PsSetProcessPriorityClass ImportFunc<I_MODULE_NT, H_PsSetProcessPriorityClass>
#define I_PsSetProcessSecurityPort ImportFunc<I_MODULE_NT, H_PsSetProcessSecurityPort>
#define I_PsSetProcessWin32Process ImportFunc<I_MODULE_NT, H_PsSetProcessWin32Process>
#define I_PsSetProcessWindowStation ImportFunc<I_MODULE_NT, H_PsSetProcessWindowStation>
#define I_PsSetThreadHardErrorsAreDisabled ImportFunc<I_MODULE_NT, H_PsSetThreadHardErrorsAreDisabled>
#define I_PsSetThreadWin32Thread ImportFunc<I_MODULE_NT, H_PsSetThreadWin32Thread>
#define I_PsSuspendProcess ImportFunc<I_MODULE_NT, H_PsSuspendProcess>
#define I_PsTerminateSystemThread ImportFunc<I_MODULE_NT, H_PsTerminateSystemThread>
#define I_PsThreadType ImportFunc<I_MODULE_NT, H_PsThreadType>
#define I_PsUILanguageComitted ImportFunc<I_MODULE_NT, H_PsUILanguageComitted>
#define I_PsUpdateDiskCounters ImportFunc<I_MODULE_NT, H_PsUpdateDiskCounters>
#define I_PsWrapApcWow64Thread ImportFunc<I_MODULE_NT, H_PsWrapApcWow64Thread>
#define I_RtlAbsoluteToSelfRelativeSD ImportFunc<I_MODULE_NT, H_RtlAbsoluteToSelfRelativeSD>
#define I_RtlAddAccessAllowedAce ImportFunc<I_MODULE_NT, H_RtlAddAccessAllowedAce>
#define I_RtlAddAccessAllowedAceEx ImportFunc<I_MODULE_NT, H_RtlAddAccessAllowedAceEx>
#define I_RtlAddAce ImportFunc<I_MODULE_NT, H_RtlAddAce>
#define I_RtlAddAtomToAtomTable ImportFunc<I_MODULE_NT, H_RtlAddAtomToAtomTable>
#define I_RtlAddAtomToAtomTableEx ImportFunc<I_MODULE_NT, H_RtlAddAtomToAtomTableEx>
#define I_RtlAddRange ImportFunc<I_MODULE_NT, H_RtlAddRange>
#define I_RtlAddResourceAttributeAce ImportFunc<I_MODULE_NT, H_RtlAddResourceAttributeAce>
#define I_RtlAllocateHeap ImportFunc<I_MODULE_NT, H_RtlAllocateHeap>
#define I_RtlAnsiCharToUnicodeChar ImportFunc<I_MODULE_NT, H_RtlAnsiCharToUnicodeChar>
#define I_RtlAnsiStringToUnicodeSize ImportFunc<I_MODULE_NT, H_RtlAnsiStringToUnicodeSize>
#define I_RtlAnsiStringToUnicodeString ImportFunc<I_MODULE_NT, H_RtlAnsiStringToUnicodeString>
#define I_RtlAppendAsciizToString ImportFunc<I_MODULE_NT, H_RtlAppendAsciizToString>
#define I_RtlAppendStringToString ImportFunc<I_MODULE_NT, H_RtlAppendStringToString>
#define I_RtlAppendUnicodeStringToString ImportFunc<I_MODULE_NT, H_RtlAppendUnicodeStringToString>
#define I_RtlAppendUnicodeToString ImportFunc<I_MODULE_NT, H_RtlAppendUnicodeToString>
#define I_RtlAreAllAccessesGranted ImportFunc<I_MODULE_NT, H_RtlAreAllAccessesGranted>
#define I_RtlAreAnyAccessesGranted ImportFunc<I_MODULE_NT, H_RtlAreAnyAccessesGranted>
#define I_RtlAreBitsClear ImportFunc<I_MODULE_NT, H_RtlAreBitsClear>
#define I_RtlAreBitsSet ImportFunc<I_MODULE_NT, H_RtlAreBitsSet>
#define I_RtlAssert ImportFunc<I_MODULE_NT, H_RtlAssert>
#define I_RtlAvlInsertNodeEx ImportFunc<I_MODULE_NT, H_RtlAvlInsertNodeEx>
#define I_RtlAvlRemoveNode ImportFunc<I_MODULE_NT, H_RtlAvlRemoveNode>
#define I_RtlCaptureContext ImportFunc<I_MODULE_NT, H_RtlCaptureContext>
#define I_RtlCaptureStackBackTrace ImportFunc<I_MODULE_NT, H_RtlCaptureStackBackTrace>
#define I_RtlCharToInteger ImportFunc<I_MODULE_NT, H_RtlCharToInteger>
#define I_RtlCheckPortableOperatingSystem ImportFunc<I_MODULE_NT, H_RtlCheckPortableOperatingSystem>
#define I_RtlCheckRegistryKey ImportFunc<I_MODULE_NT, H_RtlCheckRegistryKey>
#define I_RtlCheckTokenCapability ImportFunc<I_MODULE_NT, H_RtlCheckTokenCapability>
#define I_RtlCheckTokenMembership ImportFunc<I_MODULE_NT, H_RtlCheckTokenMembership>
#define I_RtlCheckTokenMembershipEx ImportFunc<I_MODULE_NT, H_RtlCheckTokenMembershipEx>
#define I_RtlClearAllBits ImportFunc<I_MODULE_NT, H_RtlClearAllBits>
#define I_RtlClearBit ImportFunc<I_MODULE_NT, H_RtlClearBit>
#define I_RtlClearBits ImportFunc<I_MODULE_NT, H_RtlClearBits>
#define I_RtlCmDecodeMemIoResource ImportFunc<I_MODULE_NT, H_RtlCmDecodeMemIoResource>
#define I_RtlCmEncodeMemIoResource ImportFunc<I_MODULE_NT, H_RtlCmEncodeMemIoResource>
#define I_RtlCompareAltitudes ImportFunc<I_MODULE_NT, H_RtlCompareAltitudes>
#define I_RtlCompareMemory ImportFunc<I_MODULE_NT, H_RtlCompareMemory>
#define I_RtlCompareMemoryUlong ImportFunc<I_MODULE_NT, H_RtlCompareMemoryUlong>
#define I_RtlCompareString ImportFunc<I_MODULE_NT, H_RtlCompareString>
#define I_RtlCompareUnicodeString ImportFunc<I_MODULE_NT, H_RtlCompareUnicodeString>
#define I_RtlCompareUnicodeStrings ImportFunc<I_MODULE_NT, H_RtlCompareUnicodeStrings>
#define I_RtlCompressBuffer ImportFunc<I_MODULE_NT, H_RtlCompressBuffer>
#define I_RtlCompressChunks ImportFunc<I_MODULE_NT, H_RtlCompressChunks>
#define I_RtlComputeCrc32 ImportFunc<I_MODULE_NT, H_RtlComputeCrc32>
#define I_RtlContractHashTable ImportFunc<I_MODULE_NT, H_RtlContractHashTable>
#define I_RtlConvertSidToUnicodeString ImportFunc<I_MODULE_NT, H_RtlConvertSidToUnicodeString>
#define I_RtlCopyBitMap ImportFunc<I_MODULE_NT, H_RtlCopyBitMap>
#define I_RtlCopyLuid ImportFunc<I_MODULE_NT, H_RtlCopyLuid>
#define I_RtlCopyLuidAndAttributesArray ImportFunc<I_MODULE_NT, H_RtlCopyLuidAndAttributesArray>
#define I_RtlCopyMemory ImportFunc<I_MODULE_NT, H_RtlCopyMemory>
#define I_RtlCopyMemoryNonTemporal ImportFunc<I_MODULE_NT, H_RtlCopyMemoryNonTemporal>
#define I_RtlCopyRangeList ImportFunc<I_MODULE_NT, H_RtlCopyRangeList>
#define I_RtlCopySid ImportFunc<I_MODULE_NT, H_RtlCopySid>
#define I_RtlCopySidAndAttributesArray ImportFunc<I_MODULE_NT, H_RtlCopySidAndAttributesArray>
#define I_RtlCopyString ImportFunc<I_MODULE_NT, H_RtlCopyString>
#define I_RtlCopyUnicodeString ImportFunc<I_MODULE_NT, H_RtlCopyUnicodeString>
#define I_RtlCrc32 ImportFunc<I_MODULE_NT, H_RtlCrc32>
#define I_RtlCrc64 ImportFunc<I_MODULE_NT, H_RtlCrc64>
#define I_RtlCreateAcl ImportFunc<I_MODULE_NT, H_RtlCreateAcl>
#define I_RtlCreateAtomTable ImportFunc<I_MODULE_NT, H_RtlCreateAtomTable>
#define I_RtlCreateAtomTableEx ImportFunc<I_MODULE_NT, H_RtlCreateAtomTableEx>
#define I_RtlCreateHashTable ImportFunc<I_MODULE_NT, H_RtlCreateHashTable>
#define I_RtlCreateHashTableEx ImportFunc<I_MODULE_NT, H_RtlCreateHashTableEx>
#define I_RtlCreateHeap ImportFunc<I_MODULE_NT, H_RtlCreateHeap>
#define I_RtlCreateRegistryKey ImportFunc<I_MODULE_NT, H_RtlCreateRegistryKey>
#define I_RtlCreateSecurityDescriptor ImportFunc<I_MODULE_NT, H_RtlCreateSecurityDescriptor>
#define I_RtlCreateSystemVolumeInformationFolder ImportFunc<I_MODULE_NT, H_RtlCreateSystemVolumeInformationFolder>
#define I_RtlCreateUnicodeString ImportFunc<I_MODULE_NT, H_RtlCreateUnicodeString>
#define I_RtlCreateUserThread ImportFunc<I_MODULE_NT, H_RtlCreateUserThread>
#define I_RtlCultureNameToLCID ImportFunc<I_MODULE_NT, H_RtlCultureNameToLCID>
#define I_RtlCustomCPToUnicodeN ImportFunc<I_MODULE_NT, H_RtlCustomCPToUnicodeN>
#define I_RtlDecompressBuffer ImportFunc<I_MODULE_NT, H_RtlDecompressBuffer>
#define I_RtlDecompressBufferEx ImportFunc<I_MODULE_NT, H_RtlDecompressBufferEx>
#define I_RtlDecompressChunks ImportFunc<I_MODULE_NT, H_RtlDecompressChunks>
#define I_RtlDecompressFragment ImportFunc<I_MODULE_NT, H_RtlDecompressFragment>
#define I_RtlDelete ImportFunc<I_MODULE_NT, H_RtlDelete>
#define I_RtlDeleteAce ImportFunc<I_MODULE_NT, H_RtlDeleteAce>
#define I_RtlDeleteAtomFromAtomTable ImportFunc<I_MODULE_NT, H_RtlDeleteAtomFromAtomTable>
#define I_RtlDeleteElementGenericTable ImportFunc<I_MODULE_NT, H_RtlDeleteElementGenericTable>
#define I_RtlDeleteElementGenericTableAvl ImportFunc<I_MODULE_NT, H_RtlDeleteElementGenericTableAvl>
#define I_RtlDeleteElementGenericTableAvlEx ImportFunc<I_MODULE_NT, H_RtlDeleteElementGenericTableAvlEx>
#define I_RtlDeleteHashTable ImportFunc<I_MODULE_NT, H_RtlDeleteHashTable>
#define I_RtlDeleteNoSplay ImportFunc<I_MODULE_NT, H_RtlDeleteNoSplay>
#define I_RtlDeleteOwnersRanges ImportFunc<I_MODULE_NT, H_RtlDeleteOwnersRanges>
#define I_RtlDeleteRange ImportFunc<I_MODULE_NT, H_RtlDeleteRange>
#define I_RtlDeleteRegistryValue ImportFunc<I_MODULE_NT, H_RtlDeleteRegistryValue>
#define I_RtlDescribeChunk ImportFunc<I_MODULE_NT, H_RtlDescribeChunk>
#define I_RtlDestroyAtomTable ImportFunc<I_MODULE_NT, H_RtlDestroyAtomTable>
#define I_RtlDestroyHeap ImportFunc<I_MODULE_NT, H_RtlDestroyHeap>
#define I_RtlDowncaseUnicodeChar ImportFunc<I_MODULE_NT, H_RtlDowncaseUnicodeChar>
#define I_RtlDowncaseUnicodeString ImportFunc<I_MODULE_NT, H_RtlDowncaseUnicodeString>
#define I_RtlDuplicateUnicodeString ImportFunc<I_MODULE_NT, H_RtlDuplicateUnicodeString>
#define I_RtlEmptyAtomTable ImportFunc<I_MODULE_NT, H_RtlEmptyAtomTable>
#define I_RtlEndEnumerationHashTable ImportFunc<I_MODULE_NT, H_RtlEndEnumerationHashTable>
#define I_RtlEndWeakEnumerationHashTable ImportFunc<I_MODULE_NT, H_RtlEndWeakEnumerationHashTable>
#define I_RtlEnumerateEntryHashTable ImportFunc<I_MODULE_NT, H_RtlEnumerateEntryHashTable>
#define I_RtlEnumerateGenericTable ImportFunc<I_MODULE_NT, H_RtlEnumerateGenericTable>
#define I_RtlEnumerateGenericTableAvl ImportFunc<I_MODULE_NT, H_RtlEnumerateGenericTableAvl>
#define I_RtlEnumerateGenericTableLikeADirectory ImportFunc<I_MODULE_NT, H_RtlEnumerateGenericTableLikeADirectory>
#define I_RtlEnumerateGenericTableWithoutSplaying ImportFunc<I_MODULE_NT, H_RtlEnumerateGenericTableWithoutSplaying>
#define I_RtlEnumerateGenericTableWithoutSplayingAvl ImportFunc<I_MODULE_NT, H_RtlEnumerateGenericTableWithoutSplayingAvl>
#define I_RtlEqualLuid ImportFunc<I_MODULE_NT, H_RtlEqualLuid>
#define I_RtlEqualSid ImportFunc<I_MODULE_NT, H_RtlEqualSid>
#define I_RtlEqualString ImportFunc<I_MODULE_NT, H_RtlEqualString>
#define I_RtlEqualUnicodeString ImportFunc<I_MODULE_NT, H_RtlEqualUnicodeString>
#define I_RtlEqualWnfChangeStamps ImportFunc<I_MODULE_NT, H_RtlEqualWnfChangeStamps>
#define I_RtlEthernetAddressToStringA ImportFunc<I_MODULE_NT, H_RtlEthernetAddressToStringA>
#define I_RtlEthernetAddressToStringW ImportFunc<I_MODULE_NT, H_RtlEthernetAddressToStringW>
#define I_RtlEthernetStringToAddressA ImportFunc<I_MODULE_NT, H_RtlEthernetStringToAddressA>
#define I_RtlEthernetStringToAddressW ImportFunc<I_MODULE_NT, H_RtlEthernetStringToAddressW>
#define I_RtlExpandHashTable ImportFunc<I_MODULE_NT, H_RtlExpandHashTable>
#define I_RtlExtractBitMap ImportFunc<I_MODULE_NT, H_RtlExtractBitMap>
#define I_RtlFillMemory ImportFunc<I_MODULE_NT, H_RtlFillMemory>
#define I_RtlFindAceByType ImportFunc<I_MODULE_NT, H_RtlFindAceByType>
#define I_RtlFindClearBits ImportFunc<I_MODULE_NT, H_RtlFindClearBits>
#define I_RtlFindClearBitsAndSet ImportFunc<I_MODULE_NT, H_RtlFindClearBitsAndSet>
#define I_RtlFindClearRuns ImportFunc<I_MODULE_NT, H_RtlFindClearRuns>
#define I_RtlFindClosestEncodableLength ImportFunc<I_MODULE_NT, H_RtlFindClosestEncodableLength>
#define I_RtlFindFirstRunClear ImportFunc<I_MODULE_NT, H_RtlFindFirstRunClear>
#define I_RtlFindLastBackwardRunClear ImportFunc<I_MODULE_NT, H_RtlFindLastBackwardRunClear>
#define I_RtlFindLeastSignificantBit ImportFunc<I_MODULE_NT, H_RtlFindLeastSignificantBit>
#define I_RtlFindLongestRunClear ImportFunc<I_MODULE_NT, H_RtlFindLongestRunClear>
#define I_RtlFindMessage ImportFunc<I_MODULE_NT, H_RtlFindMessage>
#define I_RtlFindMostSignificantBit ImportFunc<I_MODULE_NT, H_RtlFindMostSignificantBit>
#define I_RtlFindNextForwardRunClear ImportFunc<I_MODULE_NT, H_RtlFindNextForwardRunClear>
#define I_RtlFindRange ImportFunc<I_MODULE_NT, H_RtlFindRange>
#define I_RtlFindSetBits ImportFunc<I_MODULE_NT, H_RtlFindSetBits>
#define I_RtlFindSetBitsAndClear ImportFunc<I_MODULE_NT, H_RtlFindSetBitsAndClear>
#define I_RtlFindUnicodePrefix ImportFunc<I_MODULE_NT, H_RtlFindUnicodePrefix>
#define I_RtlFormatCurrentUserKeyPath ImportFunc<I_MODULE_NT, H_RtlFormatCurrentUserKeyPath>
#define I_RtlFormatMessage ImportFunc<I_MODULE_NT, H_RtlFormatMessage>
#define I_RtlFreeAnsiString ImportFunc<I_MODULE_NT, H_RtlFreeAnsiString>
#define I_RtlFreeHeap ImportFunc<I_MODULE_NT, H_RtlFreeHeap>
#define I_RtlFreeOemString ImportFunc<I_MODULE_NT, H_RtlFreeOemString>
#define I_RtlFreeRangeList ImportFunc<I_MODULE_NT, H_RtlFreeRangeList>
#define I_RtlFreeUnicodeString ImportFunc<I_MODULE_NT, H_RtlFreeUnicodeString>
#define I_RtlGUIDFromString ImportFunc<I_MODULE_NT, H_RtlGUIDFromString>
#define I_RtlGenerate8dot3Name ImportFunc<I_MODULE_NT, H_RtlGenerate8dot3Name>
#define I_RtlGenerateClass5Guid ImportFunc<I_MODULE_NT, H_RtlGenerateClass5Guid>
#define I_RtlGetAce ImportFunc<I_MODULE_NT, H_RtlGetAce>
#define I_RtlGetAppContainerNamedObjectPath ImportFunc<I_MODULE_NT, H_RtlGetAppContainerNamedObjectPath>
#define I_RtlGetAppContainerParent ImportFunc<I_MODULE_NT, H_RtlGetAppContainerParent>
#define I_RtlGetAppContainerSidType ImportFunc<I_MODULE_NT, H_RtlGetAppContainerSidType>
#define I_RtlGetCallersAddress ImportFunc<I_MODULE_NT, H_RtlGetCallersAddress>
#define I_RtlGetCompressionWorkSpaceSize ImportFunc<I_MODULE_NT, H_RtlGetCompressionWorkSpaceSize>
#define I_RtlGetDaclSecurityDescriptor ImportFunc<I_MODULE_NT, H_RtlGetDaclSecurityDescriptor>
#define I_RtlGetDefaultCodePage ImportFunc<I_MODULE_NT, H_RtlGetDefaultCodePage>
#define I_RtlGetElementGenericTable ImportFunc<I_MODULE_NT, H_RtlGetElementGenericTable>
#define I_RtlGetElementGenericTableAvl ImportFunc<I_MODULE_NT, H_RtlGetElementGenericTableAvl>
#define I_RtlGetEnabledExtendedFeatures ImportFunc<I_MODULE_NT, H_RtlGetEnabledExtendedFeatures>
#define I_RtlGetFirstRange ImportFunc<I_MODULE_NT, H_RtlGetFirstRange>
#define I_RtlGetGroupSecurityDescriptor ImportFunc<I_MODULE_NT, H_RtlGetGroupSecurityDescriptor>
#define I_RtlGetIntegerAtom ImportFunc<I_MODULE_NT, H_RtlGetIntegerAtom>
#define I_RtlGetLastRange ImportFunc<I_MODULE_NT, H_RtlGetLastRange>
#define I_RtlGetNextEntryHashTable ImportFunc<I_MODULE_NT, H_RtlGetNextEntryHashTable>
#define I_RtlGetNextRange ImportFunc<I_MODULE_NT, H_RtlGetNextRange>
#define I_RtlGetNtGlobalFlags ImportFunc<I_MODULE_NT, H_RtlGetNtGlobalFlags>
#define I_RtlGetOwnerSecurityDescriptor ImportFunc<I_MODULE_NT, H_RtlGetOwnerSecurityDescriptor>
#define I_RtlGetProductInfo ImportFunc<I_MODULE_NT, H_RtlGetProductInfo>
#define I_RtlGetSaclSecurityDescriptor ImportFunc<I_MODULE_NT, H_RtlGetSaclSecurityDescriptor>
#define I_RtlGetSetBootStatusData ImportFunc<I_MODULE_NT, H_RtlGetSetBootStatusData>
#define I_RtlGetThreadLangIdByIndex ImportFunc<I_MODULE_NT, H_RtlGetThreadLangIdByIndex>
#define I_RtlGetVersion ImportFunc<I_MODULE_NT, H_RtlGetVersion>
#define I_RtlHashUnicodeString ImportFunc<I_MODULE_NT, H_RtlHashUnicodeString>
#define I_RtlIdnToAscii ImportFunc<I_MODULE_NT, H_RtlIdnToAscii>
#define I_RtlIdnToNameprepUnicode ImportFunc<I_MODULE_NT, H_RtlIdnToNameprepUnicode>
#define I_RtlIdnToUnicode ImportFunc<I_MODULE_NT, H_RtlIdnToUnicode>
#define I_RtlImageDirectoryEntryToData ImportFunc<I_MODULE_NT, H_RtlImageDirectoryEntryToData>
#define I_RtlImageNtHeader ImportFunc<I_MODULE_NT, H_RtlImageNtHeader>
#define I_RtlImageNtHeaderEx ImportFunc<I_MODULE_NT, H_RtlImageNtHeaderEx>
#define I_RtlInitAnsiString ImportFunc<I_MODULE_NT, H_RtlInitAnsiString>
#define I_RtlInitAnsiStringEx ImportFunc<I_MODULE_NT, H_RtlInitAnsiStringEx>
#define I_RtlInitCodePageTable ImportFunc<I_MODULE_NT, H_RtlInitCodePageTable>
#define I_RtlInitEnumerationHashTable ImportFunc<I_MODULE_NT, H_RtlInitEnumerationHashTable>
#define I_RtlInitString ImportFunc<I_MODULE_NT, H_RtlInitString>
#define I_RtlInitUnicodeString ImportFunc<I_MODULE_NT, H_RtlInitUnicodeString>
#define I_RtlInitUnicodeStringEx ImportFunc<I_MODULE_NT, H_RtlInitUnicodeStringEx>
#define I_RtlInitWeakEnumerationHashTable ImportFunc<I_MODULE_NT, H_RtlInitWeakEnumerationHashTable>
#define I_RtlInitializeBitMap ImportFunc<I_MODULE_NT, H_RtlInitializeBitMap>
#define I_RtlInitializeGenericTable ImportFunc<I_MODULE_NT, H_RtlInitializeGenericTable>
#define I_RtlInitializeGenericTableAvl ImportFunc<I_MODULE_NT, H_RtlInitializeGenericTableAvl>
#define I_RtlInitializeRangeList ImportFunc<I_MODULE_NT, H_RtlInitializeRangeList>
#define I_RtlInitializeSid ImportFunc<I_MODULE_NT, H_RtlInitializeSid>
#define I_RtlInitializeUnicodePrefix ImportFunc<I_MODULE_NT, H_RtlInitializeUnicodePrefix>
#define I_RtlInsertElementGenericTable ImportFunc<I_MODULE_NT, H_RtlInsertElementGenericTable>
#define I_RtlInsertElementGenericTableAvl ImportFunc<I_MODULE_NT, H_RtlInsertElementGenericTableAvl>
#define I_RtlInsertElementGenericTableFull ImportFunc<I_MODULE_NT, H_RtlInsertElementGenericTableFull>
#define I_RtlInsertElementGenericTableFullAvl ImportFunc<I_MODULE_NT, H_RtlInsertElementGenericTableFullAvl>
#define I_RtlInsertEntryHashTable ImportFunc<I_MODULE_NT, H_RtlInsertEntryHashTable>
#define I_RtlInsertUnicodePrefix ImportFunc<I_MODULE_NT, H_RtlInsertUnicodePrefix>
#define I_RtlInt64ToUnicodeString ImportFunc<I_MODULE_NT, H_RtlInt64ToUnicodeString>
#define I_RtlIntegerToChar ImportFunc<I_MODULE_NT, H_RtlIntegerToChar>
#define I_RtlIntegerToUnicode ImportFunc<I_MODULE_NT, H_RtlIntegerToUnicode>
#define I_RtlIntegerToUnicodeString ImportFunc<I_MODULE_NT, H_RtlIntegerToUnicodeString>
#define I_RtlInterlockedClearBitRun ImportFunc<I_MODULE_NT, H_RtlInterlockedClearBitRun>
#define I_RtlInterlockedSetBitRun ImportFunc<I_MODULE_NT, H_RtlInterlockedSetBitRun>
#define I_RtlInterlockedSetClearRun ImportFunc<I_MODULE_NT, H_RtlInterlockedSetClearRun>
#define I_RtlInvertRangeList ImportFunc<I_MODULE_NT, H_RtlInvertRangeList>
#define I_RtlInvertRangeListEx ImportFunc<I_MODULE_NT, H_RtlInvertRangeListEx>
#define I_RtlIoDecodeMemIoResource ImportFunc<I_MODULE_NT, H_RtlIoDecodeMemIoResource>
#define I_RtlIoEncodeMemIoResource ImportFunc<I_MODULE_NT, H_RtlIoEncodeMemIoResource>
#define I_RtlIpv4AddressToStringA ImportFunc<I_MODULE_NT, H_RtlIpv4AddressToStringA>
#define I_RtlIpv4AddressToStringExA ImportFunc<I_MODULE_NT, H_RtlIpv4AddressToStringExA>
#define I_RtlIpv4AddressToStringExW ImportFunc<I_MODULE_NT, H_RtlIpv4AddressToStringExW>
#define I_RtlIpv4AddressToStringW ImportFunc<I_MODULE_NT, H_RtlIpv4AddressToStringW>
#define I_RtlIpv4StringToAddressA ImportFunc<I_MODULE_NT, H_RtlIpv4StringToAddressA>
#define I_RtlIpv4StringToAddressExA ImportFunc<I_MODULE_NT, H_RtlIpv4StringToAddressExA>
#define I_RtlIpv4StringToAddressExW ImportFunc<I_MODULE_NT, H_RtlIpv4StringToAddressExW>
#define I_RtlIpv4StringToAddressW ImportFunc<I_MODULE_NT, H_RtlIpv4StringToAddressW>
#define I_RtlIpv6AddressToStringA ImportFunc<I_MODULE_NT, H_RtlIpv6AddressToStringA>
#define I_RtlIpv6AddressToStringExA ImportFunc<I_MODULE_NT, H_RtlIpv6AddressToStringExA>
#define I_RtlIpv6AddressToStringExW ImportFunc<I_MODULE_NT, H_RtlIpv6AddressToStringExW>
#define I_RtlIpv6AddressToStringW ImportFunc<I_MODULE_NT, H_RtlIpv6AddressToStringW>
#define I_RtlIpv6StringToAddressA ImportFunc<I_MODULE_NT, H_RtlIpv6StringToAddressA>
#define I_RtlIpv6StringToAddressExA ImportFunc<I_MODULE_NT, H_RtlIpv6StringToAddressExA>
#define I_RtlIpv6StringToAddressExW ImportFunc<I_MODULE_NT, H_RtlIpv6StringToAddressExW>
#define I_RtlIpv6StringToAddressW ImportFunc<I_MODULE_NT, H_RtlIpv6StringToAddressW>
#define I_RtlIsGenericTableEmpty ImportFunc<I_MODULE_NT, H_RtlIsGenericTableEmpty>
#define I_RtlIsGenericTableEmptyAvl ImportFunc<I_MODULE_NT, H_RtlIsGenericTableEmptyAvl>
#define I_RtlIsNameLegalDOS8Dot3 ImportFunc<I_MODULE_NT, H_RtlIsNameLegalDOS8Dot3>
#define I_RtlIsNormalizedString ImportFunc<I_MODULE_NT, H_RtlIsNormalizedString>
#define I_RtlIsNtDdiVersionAvailable ImportFunc<I_MODULE_NT, H_RtlIsNtDdiVersionAvailable>
#define I_RtlIsRangeAvailable ImportFunc<I_MODULE_NT, H_RtlIsRangeAvailable>
#define I_RtlIsServicePackVersionInstalled ImportFunc<I_MODULE_NT, H_RtlIsServicePackVersionInstalled>
#define I_RtlIsUntrustedObject ImportFunc<I_MODULE_NT, H_RtlIsUntrustedObject>
#define I_RtlIsValidOemCharacter ImportFunc<I_MODULE_NT, H_RtlIsValidOemCharacter>
#define I_RtlLCIDToCultureName ImportFunc<I_MODULE_NT, H_RtlLCIDToCultureName>
#define I_RtlLengthRequiredSid ImportFunc<I_MODULE_NT, H_RtlLengthRequiredSid>
#define I_RtlLengthSecurityDescriptor ImportFunc<I_MODULE_NT, H_RtlLengthSecurityDescriptor>
#define I_RtlLengthSid ImportFunc<I_MODULE_NT, H_RtlLengthSid>
#define I_RtlLoadString ImportFunc<I_MODULE_NT, H_RtlLoadString>
#define I_RtlLocalTimeToSystemTime ImportFunc<I_MODULE_NT, H_RtlLocalTimeToSystemTime>
#define I_RtlLockBootStatusData ImportFunc<I_MODULE_NT, H_RtlLockBootStatusData>
#define I_RtlLookupAtomInAtomTable ImportFunc<I_MODULE_NT, H_RtlLookupAtomInAtomTable>
#define I_RtlLookupElementGenericTable ImportFunc<I_MODULE_NT, H_RtlLookupElementGenericTable>
#define I_RtlLookupElementGenericTableAvl ImportFunc<I_MODULE_NT, H_RtlLookupElementGenericTableAvl>
#define I_RtlLookupElementGenericTableFull ImportFunc<I_MODULE_NT, H_RtlLookupElementGenericTableFull>
#define I_RtlLookupElementGenericTableFullAvl ImportFunc<I_MODULE_NT, H_RtlLookupElementGenericTableFullAvl>
#define I_RtlLookupEntryHashTable ImportFunc<I_MODULE_NT, H_RtlLookupEntryHashTable>
#define I_RtlLookupFirstMatchingElementGenericTableAvl ImportFunc<I_MODULE_NT, H_RtlLookupFirstMatchingElementGenericTableAvl>
#define I_RtlLookupFunctionEntry ImportFunc<I_MODULE_NT, H_RtlLookupFunctionEntry>
#define I_RtlMapGenericMask ImportFunc<I_MODULE_NT, H_RtlMapGenericMask>
#define I_RtlMapSecurityErrorToNtStatus ImportFunc<I_MODULE_NT, H_RtlMapSecurityErrorToNtStatus>
#define I_RtlMergeRangeLists ImportFunc<I_MODULE_NT, H_RtlMergeRangeLists>
#define I_RtlMoveMemory ImportFunc<I_MODULE_NT, H_RtlMoveMemory>
#define I_RtlMultiByteToUnicodeN ImportFunc<I_MODULE_NT, H_RtlMultiByteToUnicodeN>
#define I_RtlMultiByteToUnicodeSize ImportFunc<I_MODULE_NT, H_RtlMultiByteToUnicodeSize>
#define I_RtlNextUnicodePrefix ImportFunc<I_MODULE_NT, H_RtlNextUnicodePrefix>
#define I_RtlNormalizeString ImportFunc<I_MODULE_NT, H_RtlNormalizeString>
#define I_RtlNtStatusToDosError ImportFunc<I_MODULE_NT, H_RtlNtStatusToDosError>
#define I_RtlNtStatusToDosErrorNoTeb ImportFunc<I_MODULE_NT, H_RtlNtStatusToDosErrorNoTeb>
#define I_RtlNumberGenericTableElements ImportFunc<I_MODULE_NT, H_RtlNumberGenericTableElements>
#define I_RtlNumberGenericTableElementsAvl ImportFunc<I_MODULE_NT, H_RtlNumberGenericTableElementsAvl>
#define I_RtlNumberOfClearBits ImportFunc<I_MODULE_NT, H_RtlNumberOfClearBits>
#define I_RtlNumberOfClearBitsInRange ImportFunc<I_MODULE_NT, H_RtlNumberOfClearBitsInRange>
#define I_RtlNumberOfSetBits ImportFunc<I_MODULE_NT, H_RtlNumberOfSetBits>
#define I_RtlNumberOfSetBitsInRange ImportFunc<I_MODULE_NT, H_RtlNumberOfSetBitsInRange>
#define I_RtlNumberOfSetBitsUlongPtr ImportFunc<I_MODULE_NT, H_RtlNumberOfSetBitsUlongPtr>
#define I_RtlOemStringToCountedUnicodeString ImportFunc<I_MODULE_NT, H_RtlOemStringToCountedUnicodeString>
#define I_RtlOemStringToUnicodeSize ImportFunc<I_MODULE_NT, H_RtlOemStringToUnicodeSize>
#define I_RtlOemStringToUnicodeString ImportFunc<I_MODULE_NT, H_RtlOemStringToUnicodeString>
#define I_RtlOemToUnicodeN ImportFunc<I_MODULE_NT, H_RtlOemToUnicodeN>
#define I_RtlOpenCurrentUser ImportFunc<I_MODULE_NT, H_RtlOpenCurrentUser>
#define I_RtlOwnerAcesPresent ImportFunc<I_MODULE_NT, H_RtlOwnerAcesPresent>
#define I_RtlPcToFileHeader ImportFunc<I_MODULE_NT, H_RtlPcToFileHeader>
#define I_RtlPinAtomInAtomTable ImportFunc<I_MODULE_NT, H_RtlPinAtomInAtomTable>
#define I_RtlPrefetchMemoryNonTemporal ImportFunc<I_MODULE_NT, H_RtlPrefetchMemoryNonTemporal>
#define I_RtlPrefixString ImportFunc<I_MODULE_NT, H_RtlPrefixString>
#define I_RtlPrefixUnicodeString ImportFunc<I_MODULE_NT, H_RtlPrefixUnicodeString>
#define I_RtlQueryAtomInAtomTable ImportFunc<I_MODULE_NT, H_RtlQueryAtomInAtomTable>
#define I_RtlQueryDynamicTimeZoneInformation ImportFunc<I_MODULE_NT, H_RtlQueryDynamicTimeZoneInformation>
#define I_RtlQueryElevationFlags ImportFunc<I_MODULE_NT, H_RtlQueryElevationFlags>
#define I_RtlQueryInformationAcl ImportFunc<I_MODULE_NT, H_RtlQueryInformationAcl>
#define I_RtlQueryModuleInformation ImportFunc<I_MODULE_NT, H_RtlQueryModuleInformation>
#define I_RtlQueryPackageIdentity ImportFunc<I_MODULE_NT, H_RtlQueryPackageIdentity>
#define I_RtlQueryRegistryValues ImportFunc<I_MODULE_NT, H_RtlQueryRegistryValues>
#define I_RtlQueryRegistryValuesEx ImportFunc<I_MODULE_NT, H_RtlQueryRegistryValuesEx>
#define I_RtlQueryTimeZoneInformation ImportFunc<I_MODULE_NT, H_RtlQueryTimeZoneInformation>
#define I_RtlQueryValidationRunlevel ImportFunc<I_MODULE_NT, H_RtlQueryValidationRunlevel>
#define I_RtlRaiseException ImportFunc<I_MODULE_NT, H_RtlRaiseException>
#define I_RtlRandom ImportFunc<I_MODULE_NT, H_RtlRandom>
#define I_RtlRandomEx ImportFunc<I_MODULE_NT, H_RtlRandomEx>
#define I_RtlRbInsertNodeEx ImportFunc<I_MODULE_NT, H_RtlRbInsertNodeEx>
#define I_RtlRbRemoveNode ImportFunc<I_MODULE_NT, H_RtlRbRemoveNode>
#define I_RtlRealPredecessor ImportFunc<I_MODULE_NT, H_RtlRealPredecessor>
#define I_RtlRealSuccessor ImportFunc<I_MODULE_NT, H_RtlRealSuccessor>
#define I_RtlRemoveEntryHashTable ImportFunc<I_MODULE_NT, H_RtlRemoveEntryHashTable>
#define I_RtlRemoveUnicodePrefix ImportFunc<I_MODULE_NT, H_RtlRemoveUnicodePrefix>
#define I_RtlReplaceSidInSd ImportFunc<I_MODULE_NT, H_RtlReplaceSidInSd>
#define I_RtlReserveChunk ImportFunc<I_MODULE_NT, H_RtlReserveChunk>
#define I_RtlRestoreContext ImportFunc<I_MODULE_NT, H_RtlRestoreContext>
#define I_RtlRunOnceBeginInitialize ImportFunc<I_MODULE_NT, H_RtlRunOnceBeginInitialize>
#define I_RtlRunOnceComplete ImportFunc<I_MODULE_NT, H_RtlRunOnceComplete>
#define I_RtlRunOnceExecuteOnce ImportFunc<I_MODULE_NT, H_RtlRunOnceExecuteOnce>
#define I_RtlRunOnceInitialize ImportFunc<I_MODULE_NT, H_RtlRunOnceInitialize>
#define I_RtlSecondsSince1970ToTime ImportFunc<I_MODULE_NT, H_RtlSecondsSince1970ToTime>
#define I_RtlSecondsSince1980ToTime ImportFunc<I_MODULE_NT, H_RtlSecondsSince1980ToTime>
#define I_RtlSelfRelativeToAbsoluteSD ImportFunc<I_MODULE_NT, H_RtlSelfRelativeToAbsoluteSD>
#define I_RtlSelfRelativeToAbsoluteSD2 ImportFunc<I_MODULE_NT, H_RtlSelfRelativeToAbsoluteSD2>
#define I_RtlSetAllBits ImportFunc<I_MODULE_NT, H_RtlSetAllBits>
#define I_RtlSetBit ImportFunc<I_MODULE_NT, H_RtlSetBit>
#define I_RtlSetBits ImportFunc<I_MODULE_NT, H_RtlSetBits>
#define I_RtlSetControlSecurityDescriptor ImportFunc<I_MODULE_NT, H_RtlSetControlSecurityDescriptor>
#define I_RtlSetDaclSecurityDescriptor ImportFunc<I_MODULE_NT, H_RtlSetDaclSecurityDescriptor>
#define I_RtlSetDynamicTimeZoneInformation ImportFunc<I_MODULE_NT, H_RtlSetDynamicTimeZoneInformation>
#define I_RtlSetGroupSecurityDescriptor ImportFunc<I_MODULE_NT, H_RtlSetGroupSecurityDescriptor>
#define I_RtlSetOwnerSecurityDescriptor ImportFunc<I_MODULE_NT, H_RtlSetOwnerSecurityDescriptor>
#define I_RtlSetPortableOperatingSystem ImportFunc<I_MODULE_NT, H_RtlSetPortableOperatingSystem>
#define I_RtlSetSaclSecurityDescriptor ImportFunc<I_MODULE_NT, H_RtlSetSaclSecurityDescriptor>
#define I_RtlSetTimeZoneInformation ImportFunc<I_MODULE_NT, H_RtlSetTimeZoneInformation>
#define I_RtlSidHashInitialize ImportFunc<I_MODULE_NT, H_RtlSidHashInitialize>
#define I_RtlSidHashLookup ImportFunc<I_MODULE_NT, H_RtlSidHashLookup>
#define I_RtlSizeHeap ImportFunc<I_MODULE_NT, H_RtlSizeHeap>
#define I_RtlSplay ImportFunc<I_MODULE_NT, H_RtlSplay>
#define I_RtlStringFromGUID ImportFunc<I_MODULE_NT, H_RtlStringFromGUID>
#define I_RtlSubAuthorityCountSid ImportFunc<I_MODULE_NT, H_RtlSubAuthorityCountSid>
#define I_RtlSubAuthoritySid ImportFunc<I_MODULE_NT, H_RtlSubAuthoritySid>
#define I_RtlSubtreePredecessor ImportFunc<I_MODULE_NT, H_RtlSubtreePredecessor>
#define I_RtlSubtreeSuccessor ImportFunc<I_MODULE_NT, H_RtlSubtreeSuccessor>
#define I_RtlSystemTimeToLocalTime ImportFunc<I_MODULE_NT, H_RtlSystemTimeToLocalTime>
#define I_RtlTestBit ImportFunc<I_MODULE_NT, H_RtlTestBit>
#define I_RtlTimeFieldsToTime ImportFunc<I_MODULE_NT, H_RtlTimeFieldsToTime>
#define I_RtlTimeToElapsedTimeFields ImportFunc<I_MODULE_NT, H_RtlTimeToElapsedTimeFields>
#define I_RtlTimeToSecondsSince1970 ImportFunc<I_MODULE_NT, H_RtlTimeToSecondsSince1970>
#define I_RtlTimeToSecondsSince1980 ImportFunc<I_MODULE_NT, H_RtlTimeToSecondsSince1980>
#define I_RtlTimeToTimeFields ImportFunc<I_MODULE_NT, H_RtlTimeToTimeFields>
#define I_RtlTraceDatabaseAdd ImportFunc<I_MODULE_NT, H_RtlTraceDatabaseAdd>
#define I_RtlTraceDatabaseCreate ImportFunc<I_MODULE_NT, H_RtlTraceDatabaseCreate>
#define I_RtlTraceDatabaseDestroy ImportFunc<I_MODULE_NT, H_RtlTraceDatabaseDestroy>
#define I_RtlTraceDatabaseEnumerate ImportFunc<I_MODULE_NT, H_RtlTraceDatabaseEnumerate>
#define I_RtlTraceDatabaseFind ImportFunc<I_MODULE_NT, H_RtlTraceDatabaseFind>
#define I_RtlTraceDatabaseLock ImportFunc<I_MODULE_NT, H_RtlTraceDatabaseLock>
#define I_RtlTraceDatabaseUnlock ImportFunc<I_MODULE_NT, H_RtlTraceDatabaseUnlock>
#define I_RtlTraceDatabaseValidate ImportFunc<I_MODULE_NT, H_RtlTraceDatabaseValidate>
#define I_RtlUTF8ToUnicodeN ImportFunc<I_MODULE_NT, H_RtlUTF8ToUnicodeN>
#define I_RtlUnicodeStringToAnsiSize ImportFunc<I_MODULE_NT, H_RtlUnicodeStringToAnsiSize>
#define I_RtlUnicodeStringToAnsiString ImportFunc<I_MODULE_NT, H_RtlUnicodeStringToAnsiString>
#define I_RtlUnicodeStringToCountedOemString ImportFunc<I_MODULE_NT, H_RtlUnicodeStringToCountedOemString>
#define I_RtlUnicodeStringToInteger ImportFunc<I_MODULE_NT, H_RtlUnicodeStringToInteger>
#define I_RtlUnicodeStringToOemSize ImportFunc<I_MODULE_NT, H_RtlUnicodeStringToOemSize>
#define I_RtlUnicodeStringToOemString ImportFunc<I_MODULE_NT, H_RtlUnicodeStringToOemString>
#define I_RtlUnicodeToCustomCPN ImportFunc<I_MODULE_NT, H_RtlUnicodeToCustomCPN>
#define I_RtlUnicodeToMultiByteN ImportFunc<I_MODULE_NT, H_RtlUnicodeToMultiByteN>
#define I_RtlUnicodeToMultiByteSize ImportFunc<I_MODULE_NT, H_RtlUnicodeToMultiByteSize>
#define I_RtlUnicodeToOemN ImportFunc<I_MODULE_NT, H_RtlUnicodeToOemN>
#define I_RtlUnicodeToUTF8N ImportFunc<I_MODULE_NT, H_RtlUnicodeToUTF8N>
#define I_RtlUnlockBootStatusData ImportFunc<I_MODULE_NT, H_RtlUnlockBootStatusData>
#define I_RtlUnwind ImportFunc<I_MODULE_NT, H_RtlUnwind>
#define I_RtlUnwindEx ImportFunc<I_MODULE_NT, H_RtlUnwindEx>
#define I_RtlUpcaseUnicodeChar ImportFunc<I_MODULE_NT, H_RtlUpcaseUnicodeChar>
#define I_RtlUpcaseUnicodeString ImportFunc<I_MODULE_NT, H_RtlUpcaseUnicodeString>
#define I_RtlUpcaseUnicodeStringToAnsiString ImportFunc<I_MODULE_NT, H_RtlUpcaseUnicodeStringToAnsiString>
#define I_RtlUpcaseUnicodeStringToCountedOemString ImportFunc<I_MODULE_NT, H_RtlUpcaseUnicodeStringToCountedOemString>
#define I_RtlUpcaseUnicodeStringToOemString ImportFunc<I_MODULE_NT, H_RtlUpcaseUnicodeStringToOemString>
#define I_RtlUpcaseUnicodeToCustomCPN ImportFunc<I_MODULE_NT, H_RtlUpcaseUnicodeToCustomCPN>
#define I_RtlUpcaseUnicodeToMultiByteN ImportFunc<I_MODULE_NT, H_RtlUpcaseUnicodeToMultiByteN>
#define I_RtlUpcaseUnicodeToOemN ImportFunc<I_MODULE_NT, H_RtlUpcaseUnicodeToOemN>
#define I_RtlUpperChar ImportFunc<I_MODULE_NT, H_RtlUpperChar>
#define I_RtlUpperString ImportFunc<I_MODULE_NT, H_RtlUpperString>
#define I_RtlValidRelativeSecurityDescriptor ImportFunc<I_MODULE_NT, H_RtlValidRelativeSecurityDescriptor>
#define I_RtlValidSecurityDescriptor ImportFunc<I_MODULE_NT, H_RtlValidSecurityDescriptor>
#define I_RtlValidSid ImportFunc<I_MODULE_NT, H_RtlValidSid>
#define I_RtlValidateUnicodeString ImportFunc<I_MODULE_NT, H_RtlValidateUnicodeString>
#define I_RtlVerifyVersionInfo ImportFunc<I_MODULE_NT, H_RtlVerifyVersionInfo>
#define I_RtlVirtualUnwind ImportFunc<I_MODULE_NT, H_RtlVirtualUnwind>
#define I_RtlVolumeDeviceToDosName ImportFunc<I_MODULE_NT, H_RtlVolumeDeviceToDosName>
#define I_RtlWalkFrameChain ImportFunc<I_MODULE_NT, H_RtlWalkFrameChain>
#define I_RtlWeaklyEnumerateEntryHashTable ImportFunc<I_MODULE_NT, H_RtlWeaklyEnumerateEntryHashTable>
#define I_RtlWriteRegistryValue ImportFunc<I_MODULE_NT, H_RtlWriteRegistryValue>
#define I_RtlZeroHeap ImportFunc<I_MODULE_NT, H_RtlZeroHeap>
#define I_RtlZeroMemory ImportFunc<I_MODULE_NT, H_RtlZeroMemory>
#define I_RtlxAnsiStringToUnicodeSize ImportFunc<I_MODULE_NT, H_RtlxAnsiStringToUnicodeSize>
#define I_RtlxOemStringToUnicodeSize ImportFunc<I_MODULE_NT, H_RtlxOemStringToUnicodeSize>
#define I_RtlxUnicodeStringToAnsiSize ImportFunc<I_MODULE_NT, H_RtlxUnicodeStringToAnsiSize>
#define I_RtlxUnicodeStringToOemSize ImportFunc<I_MODULE_NT, H_RtlxUnicodeStringToOemSize>
#define I_SeAccessCheck ImportFunc<I_MODULE_NT, H_SeAccessCheck>
#define I_SeAccessCheckEx ImportFunc<I_MODULE_NT, H_SeAccessCheckEx>
#define I_SeAccessCheckFromState ImportFunc<I_MODULE_NT, H_SeAccessCheckFromState>
#define I_SeAccessCheckFromStateEx ImportFunc<I_MODULE_NT, H_SeAccessCheckFromStateEx>
#define I_SeAccessCheckWithHint ImportFunc<I_MODULE_NT, H_SeAccessCheckWithHint>
#define I_SeAdjustAccessStateForTrustLabel ImportFunc<I_MODULE_NT, H_SeAdjustAccessStateForTrustLabel>
#define I_SeAppendPrivileges ImportFunc<I_MODULE_NT, H_SeAppendPrivileges>
#define I_SeAssignSecurity ImportFunc<I_MODULE_NT, H_SeAssignSecurity>
#define I_SeAssignSecurityEx ImportFunc<I_MODULE_NT, H_SeAssignSecurityEx>
#define I_SeAuditHardLinkCreation ImportFunc<I_MODULE_NT, H_SeAuditHardLinkCreation>
#define I_SeAuditHardLinkCreationWithTransaction ImportFunc<I_MODULE_NT, H_SeAuditHardLinkCreationWithTransaction>
#define I_SeAuditTransactionStateChange ImportFunc<I_MODULE_NT, H_SeAuditTransactionStateChange>
#define I_SeAuditingAnyFileEventsWithContext ImportFunc<I_MODULE_NT, H_SeAuditingAnyFileEventsWithContext>
#define I_SeAuditingAnyFileEventsWithContextEx ImportFunc<I_MODULE_NT, H_SeAuditingAnyFileEventsWithContextEx>
#define I_SeAuditingFileEvents ImportFunc<I_MODULE_NT, H_SeAuditingFileEvents>
#define I_SeAuditingFileEventsWithContext ImportFunc<I_MODULE_NT, H_SeAuditingFileEventsWithContext>
#define I_SeAuditingFileEventsWithContextEx ImportFunc<I_MODULE_NT, H_SeAuditingFileEventsWithContextEx>
#define I_SeAuditingFileOrGlobalEvents ImportFunc<I_MODULE_NT, H_SeAuditingFileOrGlobalEvents>
#define I_SeAuditingHardLinkEvents ImportFunc<I_MODULE_NT, H_SeAuditingHardLinkEvents>
#define I_SeAuditingHardLinkEventsWithContext ImportFunc<I_MODULE_NT, H_SeAuditingHardLinkEventsWithContext>
#define I_SeAuditingWithTokenForSubcategory ImportFunc<I_MODULE_NT, H_SeAuditingWithTokenForSubcategory>
#define I_SeCaptureSecurityDescriptor ImportFunc<I_MODULE_NT, H_SeCaptureSecurityDescriptor>
#define I_SeCaptureSubjectContext ImportFunc<I_MODULE_NT, H_SeCaptureSubjectContext>
#define I_SeCaptureSubjectContextEx ImportFunc<I_MODULE_NT, H_SeCaptureSubjectContextEx>
#define I_SeCloseObjectAuditAlarm ImportFunc<I_MODULE_NT, H_SeCloseObjectAuditAlarm>
#define I_SeCloseObjectAuditAlarmForNonObObject ImportFunc<I_MODULE_NT, H_SeCloseObjectAuditAlarmForNonObObject>
#define I_SeComputeAutoInheritByObjectType ImportFunc<I_MODULE_NT, H_SeComputeAutoInheritByObjectType>
#define I_SeCreateAccessState ImportFunc<I_MODULE_NT, H_SeCreateAccessState>
#define I_SeCreateAccessStateEx ImportFunc<I_MODULE_NT, H_SeCreateAccessStateEx>
#define I_SeCreateClientSecurity ImportFunc<I_MODULE_NT, H_SeCreateClientSecurity>
#define I_SeCreateClientSecurityEx ImportFunc<I_MODULE_NT, H_SeCreateClientSecurityEx>
#define I_SeCreateClientSecurityFromSubjectContext ImportFunc<I_MODULE_NT, H_SeCreateClientSecurityFromSubjectContext>
#define I_SeCreateClientSecurityFromSubjectContextEx ImportFunc<I_MODULE_NT, H_SeCreateClientSecurityFromSubjectContextEx>
#define I_SeDeassignSecurity ImportFunc<I_MODULE_NT, H_SeDeassignSecurity>
#define I_SeDeleteAccessState ImportFunc<I_MODULE_NT, H_SeDeleteAccessState>
#define I_SeDeleteObjectAuditAlarm ImportFunc<I_MODULE_NT, H_SeDeleteObjectAuditAlarm>
#define I_SeDeleteObjectAuditAlarmWithTransaction ImportFunc<I_MODULE_NT, H_SeDeleteObjectAuditAlarmWithTransaction>
#define I_SeExamineSacl ImportFunc<I_MODULE_NT, H_SeExamineSacl>
#define I_SeExports ImportFunc<I_MODULE_NT, H_SeExports>
#define I_SeFilterToken ImportFunc<I_MODULE_NT, H_SeFilterToken>
#define I_SeFreePrivileges ImportFunc<I_MODULE_NT, H_SeFreePrivileges>
#define I_SeGetCachedSigningLevel ImportFunc<I_MODULE_NT, H_SeGetCachedSigningLevel>
#define I_SeGetLinkedToken ImportFunc<I_MODULE_NT, H_SeGetLinkedToken>
#define I_SeGetLogonSessionToken ImportFunc<I_MODULE_NT, H_SeGetLogonSessionToken>
#define I_SeImpersonateClient ImportFunc<I_MODULE_NT, H_SeImpersonateClient>
#define I_SeImpersonateClientEx ImportFunc<I_MODULE_NT, H_SeImpersonateClientEx>
#define I_SeIsParentOfChildAppContainer ImportFunc<I_MODULE_NT, H_SeIsParentOfChildAppContainer>
#define I_SeLocateProcessImageName ImportFunc<I_MODULE_NT, H_SeLocateProcessImageName>
#define I_SeLockSubjectContext ImportFunc<I_MODULE_NT, H_SeLockSubjectContext>
#define I_SeMarkLogonSessionForTerminationNotification ImportFunc<I_MODULE_NT, H_SeMarkLogonSessionForTerminationNotification>
#define I_SeOpenObjectAuditAlarm ImportFunc<I_MODULE_NT, H_SeOpenObjectAuditAlarm>
#define I_SeOpenObjectAuditAlarmForNonObObject ImportFunc<I_MODULE_NT, H_SeOpenObjectAuditAlarmForNonObObject>
#define I_SeOpenObjectAuditAlarmWithTransaction ImportFunc<I_MODULE_NT, H_SeOpenObjectAuditAlarmWithTransaction>
#define I_SeOpenObjectForDeleteAuditAlarm ImportFunc<I_MODULE_NT, H_SeOpenObjectForDeleteAuditAlarm>
#define I_SeOpenObjectForDeleteAuditAlarmWithTransaction ImportFunc<I_MODULE_NT, H_SeOpenObjectForDeleteAuditAlarmWithTransaction>
#define I_SePrivilegeCheck ImportFunc<I_MODULE_NT, H_SePrivilegeCheck>
#define I_SePrivilegeObjectAuditAlarm ImportFunc<I_MODULE_NT, H_SePrivilegeObjectAuditAlarm>
#define I_SePublicDefaultDacl ImportFunc<I_MODULE_NT, H_SePublicDefaultDacl>
#define I_SeQueryAuthenticationIdToken ImportFunc<I_MODULE_NT, H_SeQueryAuthenticationIdToken>
#define I_SeQueryInformationToken ImportFunc<I_MODULE_NT, H_SeQueryInformationToken>
#define I_SeQuerySecureBootPolicyValue ImportFunc<I_MODULE_NT, H_SeQuerySecureBootPolicyValue>
#define I_SeQuerySecurityAttributesToken ImportFunc<I_MODULE_NT, H_SeQuerySecurityAttributesToken>
#define I_SeQuerySecurityDescriptorInfo ImportFunc<I_MODULE_NT, H_SeQuerySecurityDescriptorInfo>
#define I_SeQuerySessionIdToken ImportFunc<I_MODULE_NT, H_SeQuerySessionIdToken>
#define I_SeRegisterImageVerificationCallback ImportFunc<I_MODULE_NT, H_SeRegisterImageVerificationCallback>
#define I_SeRegisterLogonSessionTerminatedRoutine ImportFunc<I_MODULE_NT, H_SeRegisterLogonSessionTerminatedRoutine>
#define I_SeReleaseSecurityDescriptor ImportFunc<I_MODULE_NT, H_SeReleaseSecurityDescriptor>
#define I_SeReleaseSubjectContext ImportFunc<I_MODULE_NT, H_SeReleaseSubjectContext>
#define I_SeReportSecurityEvent ImportFunc<I_MODULE_NT, H_SeReportSecurityEvent>
#define I_SeReportSecurityEventWithSubCategory ImportFunc<I_MODULE_NT, H_SeReportSecurityEventWithSubCategory>
#define I_SeSecurityAttributePresent ImportFunc<I_MODULE_NT, H_SeSecurityAttributePresent>
#define I_SeSetAccessStateGenericMapping ImportFunc<I_MODULE_NT, H_SeSetAccessStateGenericMapping>
#define I_SeSetAuditParameter ImportFunc<I_MODULE_NT, H_SeSetAuditParameter>
#define I_SeSetSecurityAttributesToken ImportFunc<I_MODULE_NT, H_SeSetSecurityAttributesToken>
#define I_SeSetSecurityDescriptorInfo ImportFunc<I_MODULE_NT, H_SeSetSecurityDescriptorInfo>
#define I_SeSetSecurityDescriptorInfoEx ImportFunc<I_MODULE_NT, H_SeSetSecurityDescriptorInfoEx>
#define I_SeShouldCheckForAccessRightsFromParent ImportFunc<I_MODULE_NT, H_SeShouldCheckForAccessRightsFromParent>
#define I_SeSinglePrivilegeCheck ImportFunc<I_MODULE_NT, H_SeSinglePrivilegeCheck>
#define I_SeSrpAccessCheck ImportFunc<I_MODULE_NT, H_SeSrpAccessCheck>
#define I_SeSystemDefaultDacl ImportFunc<I_MODULE_NT, H_SeSystemDefaultDacl>
#define I_SeSystemDefaultSd ImportFunc<I_MODULE_NT, H_SeSystemDefaultSd>
#define I_SeTokenFromAccessInformation ImportFunc<I_MODULE_NT, H_SeTokenFromAccessInformation>
#define I_SeTokenImpersonationLevel ImportFunc<I_MODULE_NT, H_SeTokenImpersonationLevel>
#define I_SeTokenIsAdmin ImportFunc<I_MODULE_NT, H_SeTokenIsAdmin>
#define I_SeTokenIsRestricted ImportFunc<I_MODULE_NT, H_SeTokenIsRestricted>
#define I_SeTokenIsWriteRestricted ImportFunc<I_MODULE_NT, H_SeTokenIsWriteRestricted>
#define I_SeTokenObjectType ImportFunc<I_MODULE_NT, H_SeTokenObjectType>
#define I_SeTokenType ImportFunc<I_MODULE_NT, H_SeTokenType>
#define I_SeUnlockSubjectContext ImportFunc<I_MODULE_NT, H_SeUnlockSubjectContext>
#define I_SeUnregisterImageVerificationCallback ImportFunc<I_MODULE_NT, H_SeUnregisterImageVerificationCallback>
#define I_SeUnregisterLogonSessionTerminatedRoutine ImportFunc<I_MODULE_NT, H_SeUnregisterLogonSessionTerminatedRoutine>
#define I_SeValidSecurityDescriptor ImportFunc<I_MODULE_NT, H_SeValidSecurityDescriptor>
#define I_TmCancelPropagationRequest ImportFunc<I_MODULE_NT, H_TmCancelPropagationRequest>
#define I_TmCommitComplete ImportFunc<I_MODULE_NT, H_TmCommitComplete>
#define I_TmCommitEnlistment ImportFunc<I_MODULE_NT, H_TmCommitEnlistment>
#define I_TmCommitTransaction ImportFunc<I_MODULE_NT, H_TmCommitTransaction>
#define I_TmCreateEnlistment ImportFunc<I_MODULE_NT, H_TmCreateEnlistment>
#define I_TmCurrentTransaction ImportFunc<I_MODULE_NT, H_TmCurrentTransaction>
#define I_TmDereferenceEnlistmentKey ImportFunc<I_MODULE_NT, H_TmDereferenceEnlistmentKey>
#define I_TmEnableCallbacks ImportFunc<I_MODULE_NT, H_TmEnableCallbacks>
#define I_TmEndPropagationRequest ImportFunc<I_MODULE_NT, H_TmEndPropagationRequest>
#define I_TmEnlistmentObjectType ImportFunc<I_MODULE_NT, H_TmEnlistmentObjectType>
#define I_TmFreezeTransactions ImportFunc<I_MODULE_NT, H_TmFreezeTransactions>
#define I_TmGetTransactionId ImportFunc<I_MODULE_NT, H_TmGetTransactionId>
#define I_TmInitSystem ImportFunc<I_MODULE_NT, H_TmInitSystem>
#define I_TmInitSystemPhase2 ImportFunc<I_MODULE_NT, H_TmInitSystemPhase2>
#define I_TmInitializeTransactionManager ImportFunc<I_MODULE_NT, H_TmInitializeTransactionManager>
#define I_TmIsKTMCommitCoordinator ImportFunc<I_MODULE_NT, H_TmIsKTMCommitCoordinator>
#define I_TmIsTransactionActive ImportFunc<I_MODULE_NT, H_TmIsTransactionActive>
#define I_TmPrePrepareComplete ImportFunc<I_MODULE_NT, H_TmPrePrepareComplete>
#define I_TmPrePrepareEnlistment ImportFunc<I_MODULE_NT, H_TmPrePrepareEnlistment>
#define I_TmPrepareComplete ImportFunc<I_MODULE_NT, H_TmPrepareComplete>
#define I_TmPrepareEnlistment ImportFunc<I_MODULE_NT, H_TmPrepareEnlistment>
#define I_TmPropagationComplete ImportFunc<I_MODULE_NT, H_TmPropagationComplete>
#define I_TmPropagationFailed ImportFunc<I_MODULE_NT, H_TmPropagationFailed>
#define I_TmReadOnlyEnlistment ImportFunc<I_MODULE_NT, H_TmReadOnlyEnlistment>
#define I_TmRecoverEnlistment ImportFunc<I_MODULE_NT, H_TmRecoverEnlistment>
#define I_TmRecoverResourceManager ImportFunc<I_MODULE_NT, H_TmRecoverResourceManager>
#define I_TmRecoverTransactionManager ImportFunc<I_MODULE_NT, H_TmRecoverTransactionManager>
#define I_TmReferenceEnlistmentKey ImportFunc<I_MODULE_NT, H_TmReferenceEnlistmentKey>
#define I_TmRenameTransactionManager ImportFunc<I_MODULE_NT, H_TmRenameTransactionManager>
#define I_TmRequestOutcomeEnlistment ImportFunc<I_MODULE_NT, H_TmRequestOutcomeEnlistment>
#define I_TmResourceManagerObjectType ImportFunc<I_MODULE_NT, H_TmResourceManagerObjectType>
#define I_TmRollbackComplete ImportFunc<I_MODULE_NT, H_TmRollbackComplete>
#define I_TmRollbackEnlistment ImportFunc<I_MODULE_NT, H_TmRollbackEnlistment>
#define I_TmRollbackTransaction ImportFunc<I_MODULE_NT, H_TmRollbackTransaction>
#define I_TmSetCurrentTransaction ImportFunc<I_MODULE_NT, H_TmSetCurrentTransaction>
#define I_TmSinglePhaseReject ImportFunc<I_MODULE_NT, H_TmSinglePhaseReject>
#define I_TmThawTransactions ImportFunc<I_MODULE_NT, H_TmThawTransactions>
#define I_TmTransactionManagerObjectType ImportFunc<I_MODULE_NT, H_TmTransactionManagerObjectType>
#define I_TmTransactionObjectType ImportFunc<I_MODULE_NT, H_TmTransactionObjectType>
#define I_VerSetConditionMask ImportFunc<I_MODULE_NT, H_VerSetConditionMask>
#define I_VfFailDeviceNode ImportFunc<I_MODULE_NT, H_VfFailDeviceNode>
#define I_VfFailDriver ImportFunc<I_MODULE_NT, H_VfFailDriver>
#define I_VfFailSystemBIOS ImportFunc<I_MODULE_NT, H_VfFailSystemBIOS>
#define I_VfInsertContext ImportFunc<I_MODULE_NT, H_VfInsertContext>
#define I_VfIsVerificationEnabled ImportFunc<I_MODULE_NT, H_VfIsVerificationEnabled>
#define I_VfQueryDeviceContext ImportFunc<I_MODULE_NT, H_VfQueryDeviceContext>
#define I_VfQueryDispatchTable ImportFunc<I_MODULE_NT, H_VfQueryDispatchTable>
#define I_VfQueryDriverContext ImportFunc<I_MODULE_NT, H_VfQueryDriverContext>
#define I_VfQueryIrpContext ImportFunc<I_MODULE_NT, H_VfQueryIrpContext>
#define I_VfQueryThreadContext ImportFunc<I_MODULE_NT, H_VfQueryThreadContext>
#define I_VfRemoveContext ImportFunc<I_MODULE_NT, H_VfRemoveContext>
#define I_WheaAddErrorSource ImportFunc<I_MODULE_NT, H_WheaAddErrorSource>
#define I_WheaAttemptPhysicalPageOffline ImportFunc<I_MODULE_NT, H_WheaAttemptPhysicalPageOffline>
#define I_WheaConfigureErrorSource ImportFunc<I_MODULE_NT, H_WheaConfigureErrorSource>
#define I_WheaDeferredRecoveryService ImportFunc<I_MODULE_NT, H_WheaDeferredRecoveryService>
#define I_WheaGetErrorSource ImportFunc<I_MODULE_NT, H_WheaGetErrorSource>
#define I_WheaInitializeDeferredRecoveryObject ImportFunc<I_MODULE_NT, H_WheaInitializeDeferredRecoveryObject>
#define I_WheaInitializeRecordHeader ImportFunc<I_MODULE_NT, H_WheaInitializeRecordHeader>
#define I_WheaRegisterInUsePageOfflineNotification ImportFunc<I_MODULE_NT, H_WheaRegisterInUsePageOfflineNotification>
#define I_WheaReportHwError ImportFunc<I_MODULE_NT, H_WheaReportHwError>
#define I_WheaRequestDeferredRecovery ImportFunc<I_MODULE_NT, H_WheaRequestDeferredRecovery>
#define I_WheaUnregisterInUsePageOfflineNotification ImportFunc<I_MODULE_NT, H_WheaUnregisterInUsePageOfflineNotification>
#define I_WmiGetClock ImportFunc<I_MODULE_NT, H_WmiGetClock>
#define I_WmiQueryTraceInformation ImportFunc<I_MODULE_NT, H_WmiQueryTraceInformation>
#define I_WmiTraceMessage ImportFunc<I_MODULE_NT, H_WmiTraceMessage>
#define I_WmiTraceMessageVa ImportFunc<I_MODULE_NT, H_WmiTraceMessageVa>
#define I_XIPDispatch ImportFunc<I_MODULE_NT, H_XIPDispatch>
#define I_ZwAccessCheckAndAuditAlarm ImportFunc<I_MODULE_NT, H_ZwAccessCheckAndAuditAlarm>
#define I_ZwAddBootEntry ImportFunc<I_MODULE_NT, H_ZwAddBootEntry>
#define I_ZwAddDriverEntry ImportFunc<I_MODULE_NT, H_ZwAddDriverEntry>
#define I_ZwAdjustPrivilegesToken ImportFunc<I_MODULE_NT, H_ZwAdjustPrivilegesToken>
#define I_ZwAlertThread ImportFunc<I_MODULE_NT, H_ZwAlertThread>
#define I_ZwAllocateLocallyUniqueId ImportFunc<I_MODULE_NT, H_ZwAllocateLocallyUniqueId>
#define I_ZwAllocateVirtualMemory ImportFunc<I_MODULE_NT, H_ZwAllocateVirtualMemory>
#define I_ZwAlpcAcceptConnectPort ImportFunc<I_MODULE_NT, H_ZwAlpcAcceptConnectPort>
#define I_ZwAlpcCancelMessage ImportFunc<I_MODULE_NT, H_ZwAlpcCancelMessage>
#define I_ZwAlpcConnectPort ImportFunc<I_MODULE_NT, H_ZwAlpcConnectPort>
#define I_ZwAlpcConnectPortEx ImportFunc<I_MODULE_NT, H_ZwAlpcConnectPortEx>
#define I_ZwAlpcCreatePort ImportFunc<I_MODULE_NT, H_ZwAlpcCreatePort>
#define I_ZwAlpcCreatePortSection ImportFunc<I_MODULE_NT, H_ZwAlpcCreatePortSection>
#define I_ZwAlpcCreateResourceReserve ImportFunc<I_MODULE_NT, H_ZwAlpcCreateResourceReserve>
#define I_ZwAlpcCreateSectionView ImportFunc<I_MODULE_NT, H_ZwAlpcCreateSectionView>
#define I_ZwAlpcCreateSecurityContext ImportFunc<I_MODULE_NT, H_ZwAlpcCreateSecurityContext>
#define I_ZwAlpcDeletePortSection ImportFunc<I_MODULE_NT, H_ZwAlpcDeletePortSection>
#define I_ZwAlpcDeleteResourceReserve ImportFunc<I_MODULE_NT, H_ZwAlpcDeleteResourceReserve>
#define I_ZwAlpcDeleteSectionView ImportFunc<I_MODULE_NT, H_ZwAlpcDeleteSectionView>
#define I_ZwAlpcDeleteSecurityContext ImportFunc<I_MODULE_NT, H_ZwAlpcDeleteSecurityContext>
#define I_ZwAlpcDisconnectPort ImportFunc<I_MODULE_NT, H_ZwAlpcDisconnectPort>
#define I_ZwAlpcQueryInformation ImportFunc<I_MODULE_NT, H_ZwAlpcQueryInformation>
#define I_ZwAlpcSendWaitReceivePort ImportFunc<I_MODULE_NT, H_ZwAlpcSendWaitReceivePort>
#define I_ZwAlpcSetInformation ImportFunc<I_MODULE_NT, H_ZwAlpcSetInformation>
#define I_ZwAssignProcessToJobObject ImportFunc<I_MODULE_NT, H_ZwAssignProcessToJobObject>
#define I_ZwAssociateWaitCompletionPacket ImportFunc<I_MODULE_NT, H_ZwAssociateWaitCompletionPacket>
#define I_ZwCancelIoFile ImportFunc<I_MODULE_NT, H_ZwCancelIoFile>
#define I_ZwCancelIoFileEx ImportFunc<I_MODULE_NT, H_ZwCancelIoFileEx>
#define I_ZwCancelTimer ImportFunc<I_MODULE_NT, H_ZwCancelTimer>
#define I_ZwClearEvent ImportFunc<I_MODULE_NT, H_ZwClearEvent>
#define I_ZwClose ImportFunc<I_MODULE_NT, H_ZwClose>
#define I_ZwCloseObjectAuditAlarm ImportFunc<I_MODULE_NT, H_ZwCloseObjectAuditAlarm>
#define I_ZwCommitComplete ImportFunc<I_MODULE_NT, H_ZwCommitComplete>
#define I_ZwCommitEnlistment ImportFunc<I_MODULE_NT, H_ZwCommitEnlistment>
#define I_ZwCommitTransaction ImportFunc<I_MODULE_NT, H_ZwCommitTransaction>
#define I_ZwConnectPort ImportFunc<I_MODULE_NT, H_ZwConnectPort>
#define I_ZwCreateDirectoryObject ImportFunc<I_MODULE_NT, H_ZwCreateDirectoryObject>
#define I_ZwCreateEnlistment ImportFunc<I_MODULE_NT, H_ZwCreateEnlistment>
#define I_ZwCreateEvent ImportFunc<I_MODULE_NT, H_ZwCreateEvent>
#define I_ZwCreateFile ImportFunc<I_MODULE_NT, H_ZwCreateFile>
#define I_ZwCreateIoCompletion ImportFunc<I_MODULE_NT, H_ZwCreateIoCompletion>
#define I_ZwCreateJobObject ImportFunc<I_MODULE_NT, H_ZwCreateJobObject>
#define I_ZwCreateKey ImportFunc<I_MODULE_NT, H_ZwCreateKey>
#define I_ZwCreateKeyTransacted ImportFunc<I_MODULE_NT, H_ZwCreateKeyTransacted>
#define I_ZwCreateResourceManager ImportFunc<I_MODULE_NT, H_ZwCreateResourceManager>
#define I_ZwCreateSection ImportFunc<I_MODULE_NT, H_ZwCreateSection>
#define I_ZwCreateSymbolicLinkObject ImportFunc<I_MODULE_NT, H_ZwCreateSymbolicLinkObject>
#define I_ZwCreateTimer ImportFunc<I_MODULE_NT, H_ZwCreateTimer>
#define I_ZwCreateTransaction ImportFunc<I_MODULE_NT, H_ZwCreateTransaction>
#define I_ZwCreateTransactionManager ImportFunc<I_MODULE_NT, H_ZwCreateTransactionManager>
#define I_ZwCreateWaitCompletionPacket ImportFunc<I_MODULE_NT, H_ZwCreateWaitCompletionPacket>
#define I_ZwCreateWnfStateName ImportFunc<I_MODULE_NT, H_ZwCreateWnfStateName>
#define I_ZwDeleteBootEntry ImportFunc<I_MODULE_NT, H_ZwDeleteBootEntry>
#define I_ZwDeleteDriverEntry ImportFunc<I_MODULE_NT, H_ZwDeleteDriverEntry>
#define I_ZwDeleteFile ImportFunc<I_MODULE_NT, H_ZwDeleteFile>
#define I_ZwDeleteKey ImportFunc<I_MODULE_NT, H_ZwDeleteKey>
#define I_ZwDeleteValueKey ImportFunc<I_MODULE_NT, H_ZwDeleteValueKey>
#define I_ZwDeleteWnfStateData ImportFunc<I_MODULE_NT, H_ZwDeleteWnfStateData>
#define I_ZwDeleteWnfStateName ImportFunc<I_MODULE_NT, H_ZwDeleteWnfStateName>
#define I_ZwDeviceIoControlFile ImportFunc<I_MODULE_NT, H_ZwDeviceIoControlFile>
#define I_ZwDisplayString ImportFunc<I_MODULE_NT, H_ZwDisplayString>
#define I_ZwDuplicateObject ImportFunc<I_MODULE_NT, H_ZwDuplicateObject>
#define I_ZwDuplicateToken ImportFunc<I_MODULE_NT, H_ZwDuplicateToken>
#define I_ZwEnumerateBootEntries ImportFunc<I_MODULE_NT, H_ZwEnumerateBootEntries>
#define I_ZwEnumerateDriverEntries ImportFunc<I_MODULE_NT, H_ZwEnumerateDriverEntries>
#define I_ZwEnumerateKey ImportFunc<I_MODULE_NT, H_ZwEnumerateKey>
#define I_ZwEnumerateTransactionObject ImportFunc<I_MODULE_NT, H_ZwEnumerateTransactionObject>
#define I_ZwEnumerateValueKey ImportFunc<I_MODULE_NT, H_ZwEnumerateValueKey>
#define I_ZwFlushBuffersFile ImportFunc<I_MODULE_NT, H_ZwFlushBuffersFile>
#define I_ZwFlushBuffersFileEx ImportFunc<I_MODULE_NT, H_ZwFlushBuffersFileEx>
#define I_ZwFlushInstructionCache ImportFunc<I_MODULE_NT, H_ZwFlushInstructionCache>
#define I_ZwFlushKey ImportFunc<I_MODULE_NT, H_ZwFlushKey>
#define I_ZwFlushVirtualMemory ImportFunc<I_MODULE_NT, H_ZwFlushVirtualMemory>
#define I_ZwFreeVirtualMemory ImportFunc<I_MODULE_NT, H_ZwFreeVirtualMemory>
#define I_ZwFsControlFile ImportFunc<I_MODULE_NT, H_ZwFsControlFile>
#define I_ZwGetNotificationResourceManager ImportFunc<I_MODULE_NT, H_ZwGetNotificationResourceManager>
#define I_ZwImpersonateAnonymousToken ImportFunc<I_MODULE_NT, H_ZwImpersonateAnonymousToken>
#define I_ZwInitiatePowerAction ImportFunc<I_MODULE_NT, H_ZwInitiatePowerAction>
#define I_ZwIsProcessInJob ImportFunc<I_MODULE_NT, H_ZwIsProcessInJob>
#define I_ZwLoadDriver ImportFunc<I_MODULE_NT, H_ZwLoadDriver>
#define I_ZwLoadKey ImportFunc<I_MODULE_NT, H_ZwLoadKey>
#define I_ZwLoadKeyEx ImportFunc<I_MODULE_NT, H_ZwLoadKeyEx>
#define I_ZwLockFile ImportFunc<I_MODULE_NT, H_ZwLockFile>
#define I_ZwLockProductActivationKeys ImportFunc<I_MODULE_NT, H_ZwLockProductActivationKeys>
#define I_ZwLockVirtualMemory ImportFunc<I_MODULE_NT, H_ZwLockVirtualMemory>
#define I_ZwMakeTemporaryObject ImportFunc<I_MODULE_NT, H_ZwMakeTemporaryObject>
#define I_ZwMapViewOfSection ImportFunc<I_MODULE_NT, H_ZwMapViewOfSection>
#define I_ZwModifyBootEntry ImportFunc<I_MODULE_NT, H_ZwModifyBootEntry>
#define I_ZwModifyDriverEntry ImportFunc<I_MODULE_NT, H_ZwModifyDriverEntry>
#define I_ZwNotifyChangeKey ImportFunc<I_MODULE_NT, H_ZwNotifyChangeKey>
#define I_ZwNotifyChangeSession ImportFunc<I_MODULE_NT, H_ZwNotifyChangeSession>
#define I_ZwOpenDirectoryObject ImportFunc<I_MODULE_NT, H_ZwOpenDirectoryObject>
#define I_ZwOpenEnlistment ImportFunc<I_MODULE_NT, H_ZwOpenEnlistment>
#define I_ZwOpenEvent ImportFunc<I_MODULE_NT, H_ZwOpenEvent>
#define I_ZwOpenFile ImportFunc<I_MODULE_NT, H_ZwOpenFile>
#define I_ZwOpenJobObject ImportFunc<I_MODULE_NT, H_ZwOpenJobObject>
#define I_ZwOpenKey ImportFunc<I_MODULE_NT, H_ZwOpenKey>
#define I_ZwOpenKeyEx ImportFunc<I_MODULE_NT, H_ZwOpenKeyEx>
#define I_ZwOpenKeyTransacted ImportFunc<I_MODULE_NT, H_ZwOpenKeyTransacted>
#define I_ZwOpenKeyTransactedEx ImportFunc<I_MODULE_NT, H_ZwOpenKeyTransactedEx>
#define I_ZwOpenProcess ImportFunc<I_MODULE_NT, H_ZwOpenProcess>
#define I_ZwOpenProcessToken ImportFunc<I_MODULE_NT, H_ZwOpenProcessToken>
#define I_ZwOpenProcessTokenEx ImportFunc<I_MODULE_NT, H_ZwOpenProcessTokenEx>
#define I_ZwOpenResourceManager ImportFunc<I_MODULE_NT, H_ZwOpenResourceManager>
#define I_ZwOpenSection ImportFunc<I_MODULE_NT, H_ZwOpenSection>
#define I_ZwOpenSession ImportFunc<I_MODULE_NT, H_ZwOpenSession>
#define I_ZwOpenSymbolicLinkObject ImportFunc<I_MODULE_NT, H_ZwOpenSymbolicLinkObject>
#define I_ZwOpenThread ImportFunc<I_MODULE_NT, H_ZwOpenThread>
#define I_ZwOpenThreadToken ImportFunc<I_MODULE_NT, H_ZwOpenThreadToken>
#define I_ZwOpenThreadTokenEx ImportFunc<I_MODULE_NT, H_ZwOpenThreadTokenEx>
#define I_ZwOpenTimer ImportFunc<I_MODULE_NT, H_ZwOpenTimer>
#define I_ZwOpenTransaction ImportFunc<I_MODULE_NT, H_ZwOpenTransaction>
#define I_ZwOpenTransactionManager ImportFunc<I_MODULE_NT, H_ZwOpenTransactionManager>
#define I_ZwPowerInformation ImportFunc<I_MODULE_NT, H_ZwPowerInformation>
#define I_ZwPrePrepareComplete ImportFunc<I_MODULE_NT, H_ZwPrePrepareComplete>
#define I_ZwPrePrepareEnlistment ImportFunc<I_MODULE_NT, H_ZwPrePrepareEnlistment>
#define I_ZwPrepareComplete ImportFunc<I_MODULE_NT, H_ZwPrepareComplete>
#define I_ZwPrepareEnlistment ImportFunc<I_MODULE_NT, H_ZwPrepareEnlistment>
#define I_ZwPropagationComplete ImportFunc<I_MODULE_NT, H_ZwPropagationComplete>
#define I_ZwPropagationFailed ImportFunc<I_MODULE_NT, H_ZwPropagationFailed>
#define I_ZwProtectVirtualMemory ImportFunc<I_MODULE_NT, H_ZwProtectVirtualMemory>
#define I_ZwPulseEvent ImportFunc<I_MODULE_NT, H_ZwPulseEvent>
#define I_ZwQueryBootEntryOrder ImportFunc<I_MODULE_NT, H_ZwQueryBootEntryOrder>
#define I_ZwQueryBootOptions ImportFunc<I_MODULE_NT, H_ZwQueryBootOptions>
#define I_ZwQueryDefaultLocale ImportFunc<I_MODULE_NT, H_ZwQueryDefaultLocale>
#define I_ZwQueryDefaultUILanguage ImportFunc<I_MODULE_NT, H_ZwQueryDefaultUILanguage>
#define I_ZwQueryDirectoryFile ImportFunc<I_MODULE_NT, H_ZwQueryDirectoryFile>
#define I_ZwQueryDirectoryObject ImportFunc<I_MODULE_NT, H_ZwQueryDirectoryObject>
#define I_ZwQueryDriverEntryOrder ImportFunc<I_MODULE_NT, H_ZwQueryDriverEntryOrder>
#define I_ZwQueryEaFile ImportFunc<I_MODULE_NT, H_ZwQueryEaFile>
#define I_ZwQueryFullAttributesFile ImportFunc<I_MODULE_NT, H_ZwQueryFullAttributesFile>
#define I_ZwQueryInformationEnlistment ImportFunc<I_MODULE_NT, H_ZwQueryInformationEnlistment>
#define I_ZwQueryInformationFile ImportFunc<I_MODULE_NT, H_ZwQueryInformationFile>
#define I_ZwQueryInformationJobObject ImportFunc<I_MODULE_NT, H_ZwQueryInformationJobObject>
#define I_ZwQueryInformationProcess ImportFunc<I_MODULE_NT, H_ZwQueryInformationProcess>
#define I_ZwQueryInformationResourceManager ImportFunc<I_MODULE_NT, H_ZwQueryInformationResourceManager>
#define I_ZwQueryInformationThread ImportFunc<I_MODULE_NT, H_ZwQueryInformationThread>
#define I_ZwQueryInformationToken ImportFunc<I_MODULE_NT, H_ZwQueryInformationToken>
#define I_ZwQueryInformationTransaction ImportFunc<I_MODULE_NT, H_ZwQueryInformationTransaction>
#define I_ZwQueryInformationTransactionManager ImportFunc<I_MODULE_NT, H_ZwQueryInformationTransactionManager>
#define I_ZwQueryInstallUILanguage ImportFunc<I_MODULE_NT, H_ZwQueryInstallUILanguage>
#define I_ZwQueryKey ImportFunc<I_MODULE_NT, H_ZwQueryKey>
#define I_ZwQueryLicenseValue ImportFunc<I_MODULE_NT, H_ZwQueryLicenseValue>
#define I_ZwQueryObject ImportFunc<I_MODULE_NT, H_ZwQueryObject>
#define I_ZwQueryQuotaInformationFile ImportFunc<I_MODULE_NT, H_ZwQueryQuotaInformationFile>
#define I_ZwQuerySection ImportFunc<I_MODULE_NT, H_ZwQuerySection>
#define I_ZwQuerySecurityAttributesToken ImportFunc<I_MODULE_NT, H_ZwQuerySecurityAttributesToken>
#define I_ZwQuerySecurityObject ImportFunc<I_MODULE_NT, H_ZwQuerySecurityObject>
#define I_ZwQuerySymbolicLinkObject ImportFunc<I_MODULE_NT, H_ZwQuerySymbolicLinkObject>
#define I_ZwQuerySystemEnvironmentValueEx ImportFunc<I_MODULE_NT, H_ZwQuerySystemEnvironmentValueEx>
#define I_ZwQuerySystemInformation ImportFunc<I_MODULE_NT, H_ZwQuerySystemInformation>
#define I_ZwQuerySystemInformationEx ImportFunc<I_MODULE_NT, H_ZwQuerySystemInformationEx>
#define I_ZwQueryValueKey ImportFunc<I_MODULE_NT, H_ZwQueryValueKey>
#define I_ZwQueryVirtualMemory ImportFunc<I_MODULE_NT, H_ZwQueryVirtualMemory>
#define I_ZwQueryVolumeInformationFile ImportFunc<I_MODULE_NT, H_ZwQueryVolumeInformationFile>
#define I_ZwQueryWnfStateData ImportFunc<I_MODULE_NT, H_ZwQueryWnfStateData>
#define I_ZwQueryWnfStateNameInformation ImportFunc<I_MODULE_NT, H_ZwQueryWnfStateNameInformation>
#define I_ZwReadFile ImportFunc<I_MODULE_NT, H_ZwReadFile>
#define I_ZwReadOnlyEnlistment ImportFunc<I_MODULE_NT, H_ZwReadOnlyEnlistment>
#define I_ZwRecoverEnlistment ImportFunc<I_MODULE_NT, H_ZwRecoverEnlistment>
#define I_ZwRecoverResourceManager ImportFunc<I_MODULE_NT, H_ZwRecoverResourceManager>
#define I_ZwRecoverTransactionManager ImportFunc<I_MODULE_NT, H_ZwRecoverTransactionManager>
#define I_ZwRemoveIoCompletion ImportFunc<I_MODULE_NT, H_ZwRemoveIoCompletion>
#define I_ZwRemoveIoCompletionEx ImportFunc<I_MODULE_NT, H_ZwRemoveIoCompletionEx>
#define I_ZwRenameKey ImportFunc<I_MODULE_NT, H_ZwRenameKey>
#define I_ZwReplaceKey ImportFunc<I_MODULE_NT, H_ZwReplaceKey>
#define I_ZwRequestPort ImportFunc<I_MODULE_NT, H_ZwRequestPort>
#define I_ZwRequestWaitReplyPort ImportFunc<I_MODULE_NT, H_ZwRequestWaitReplyPort>
#define I_ZwResetEvent ImportFunc<I_MODULE_NT, H_ZwResetEvent>
#define I_ZwRestoreKey ImportFunc<I_MODULE_NT, H_ZwRestoreKey>
#define I_ZwRollbackComplete ImportFunc<I_MODULE_NT, H_ZwRollbackComplete>
#define I_ZwRollbackEnlistment ImportFunc<I_MODULE_NT, H_ZwRollbackEnlistment>
#define I_ZwRollbackTransaction ImportFunc<I_MODULE_NT, H_ZwRollbackTransaction>
#define I_ZwSaveKey ImportFunc<I_MODULE_NT, H_ZwSaveKey>
#define I_ZwSaveKeyEx ImportFunc<I_MODULE_NT, H_ZwSaveKeyEx>
#define I_ZwSecureConnectPort ImportFunc<I_MODULE_NT, H_ZwSecureConnectPort>
#define I_ZwSetBootEntryOrder ImportFunc<I_MODULE_NT, H_ZwSetBootEntryOrder>
#define I_ZwSetBootOptions ImportFunc<I_MODULE_NT, H_ZwSetBootOptions>
#define I_ZwSetCachedSigningLevel ImportFunc<I_MODULE_NT, H_ZwSetCachedSigningLevel>
#define I_ZwSetDefaultLocale ImportFunc<I_MODULE_NT, H_ZwSetDefaultLocale>
#define I_ZwSetDefaultUILanguage ImportFunc<I_MODULE_NT, H_ZwSetDefaultUILanguage>
#define I_ZwSetDriverEntryOrder ImportFunc<I_MODULE_NT, H_ZwSetDriverEntryOrder>
#define I_ZwSetEaFile ImportFunc<I_MODULE_NT, H_ZwSetEaFile>
#define I_ZwSetEvent ImportFunc<I_MODULE_NT, H_ZwSetEvent>
#define I_ZwSetInformationEnlistment ImportFunc<I_MODULE_NT, H_ZwSetInformationEnlistment>
#define I_ZwSetInformationFile ImportFunc<I_MODULE_NT, H_ZwSetInformationFile>
#define I_ZwSetInformationJobObject ImportFunc<I_MODULE_NT, H_ZwSetInformationJobObject>
#define I_ZwSetInformationKey ImportFunc<I_MODULE_NT, H_ZwSetInformationKey>
#define I_ZwSetInformationObject ImportFunc<I_MODULE_NT, H_ZwSetInformationObject>
#define I_ZwSetInformationProcess ImportFunc<I_MODULE_NT, H_ZwSetInformationProcess>
#define I_ZwSetInformationResourceManager ImportFunc<I_MODULE_NT, H_ZwSetInformationResourceManager>
#define I_ZwSetInformationThread ImportFunc<I_MODULE_NT, H_ZwSetInformationThread>
#define I_ZwSetInformationToken ImportFunc<I_MODULE_NT, H_ZwSetInformationToken>
#define I_ZwSetInformationTransaction ImportFunc<I_MODULE_NT, H_ZwSetInformationTransaction>
#define I_ZwSetInformationVirtualMemory ImportFunc<I_MODULE_NT, H_ZwSetInformationVirtualMemory>
#define I_ZwSetQuotaInformationFile ImportFunc<I_MODULE_NT, H_ZwSetQuotaInformationFile>
#define I_ZwSetSecurityObject ImportFunc<I_MODULE_NT, H_ZwSetSecurityObject>
#define I_ZwSetSystemEnvironmentValueEx ImportFunc<I_MODULE_NT, H_ZwSetSystemEnvironmentValueEx>
#define I_ZwSetSystemInformation ImportFunc<I_MODULE_NT, H_ZwSetSystemInformation>
#define I_ZwSetSystemTime ImportFunc<I_MODULE_NT, H_ZwSetSystemTime>
#define I_ZwSetTimer ImportFunc<I_MODULE_NT, H_ZwSetTimer>
#define I_ZwSetTimerEx ImportFunc<I_MODULE_NT, H_ZwSetTimerEx>
#define I_ZwSetValueKey ImportFunc<I_MODULE_NT, H_ZwSetValueKey>
#define I_ZwSetVolumeInformationFile ImportFunc<I_MODULE_NT, H_ZwSetVolumeInformationFile>
#define I_ZwTerminateJobObject ImportFunc<I_MODULE_NT, H_ZwTerminateJobObject>
#define I_ZwTerminateProcess ImportFunc<I_MODULE_NT, H_ZwTerminateProcess>
#define I_ZwTraceEvent ImportFunc<I_MODULE_NT, H_ZwTraceEvent>
#define I_ZwTranslateFilePath ImportFunc<I_MODULE_NT, H_ZwTranslateFilePath>
#define I_ZwUnloadDriver ImportFunc<I_MODULE_NT, H_ZwUnloadDriver>
#define I_ZwUnloadKey ImportFunc<I_MODULE_NT, H_ZwUnloadKey>
#define I_ZwUnloadKeyEx ImportFunc<I_MODULE_NT, H_ZwUnloadKeyEx>
#define I_ZwUnlockFile ImportFunc<I_MODULE_NT, H_ZwUnlockFile>
#define I_ZwUnlockVirtualMemory ImportFunc<I_MODULE_NT, H_ZwUnlockVirtualMemory>
#define I_ZwUnmapViewOfSection ImportFunc<I_MODULE_NT, H_ZwUnmapViewOfSection>
#define I_ZwUpdateWnfStateData ImportFunc<I_MODULE_NT, H_ZwUpdateWnfStateData>
#define I_ZwWaitForMultipleObjects ImportFunc<I_MODULE_NT, H_ZwWaitForMultipleObjects>
#define I_ZwWaitForSingleObject ImportFunc<I_MODULE_NT, H_ZwWaitForSingleObject>
#define I_ZwWriteFile ImportFunc<I_MODULE_NT, H_ZwWriteFile>
#define I_ZwYieldExecution ImportFunc<I_MODULE_NT, H_ZwYieldExecution>
#define I___C_specific_handler ImportFunc<I_MODULE_NT, H___C_specific_handler>
#define I___chkstk ImportFunc<I_MODULE_NT, H___chkstk>
#define I___misaligned_access ImportFunc<I_MODULE_NT, H___misaligned_access>
#define I__i64toa_s ImportFunc<I_MODULE_NT, H__i64toa_s>
#define I__i64tow_s ImportFunc<I_MODULE_NT, H__i64tow_s>
#define I__itoa ImportFunc<I_MODULE_NT, H__itoa>
#define I__itoa_s ImportFunc<I_MODULE_NT, H__itoa_s>
#define I__itow ImportFunc<I_MODULE_NT, H__itow>
#define I__itow_s ImportFunc<I_MODULE_NT, H__itow_s>
#define I__local_unwind ImportFunc<I_MODULE_NT, H__local_unwind>
#define I__ltoa_s ImportFunc<I_MODULE_NT, H__ltoa_s>
#define I__ltow_s ImportFunc<I_MODULE_NT, H__ltow_s>
#define I__makepath_s ImportFunc<I_MODULE_NT, H__makepath_s>
#define I__purecall ImportFunc<I_MODULE_NT, H__purecall>
#define I__setjmp ImportFunc<I_MODULE_NT, H__setjmp>
#define I__setjmpex ImportFunc<I_MODULE_NT, H__setjmpex>
#define I__snprintf ImportFunc<I_MODULE_NT, H__snprintf>
#define I__snprintf_s ImportFunc<I_MODULE_NT, H__snprintf_s>
#define I__snscanf_s ImportFunc<I_MODULE_NT, H__snscanf_s>
#define I__snwprintf ImportFunc<I_MODULE_NT, H__snwprintf>
#define I__snwprintf_s ImportFunc<I_MODULE_NT, H__snwprintf_s>
#define I__snwscanf_s ImportFunc<I_MODULE_NT, H__snwscanf_s>
#define I__splitpath_s ImportFunc<I_MODULE_NT, H__splitpath_s>
#define I__stricmp ImportFunc<I_MODULE_NT, H__stricmp>
#define I__strlwr ImportFunc<I_MODULE_NT, H__strlwr>
#define I__strnicmp ImportFunc<I_MODULE_NT, H__strnicmp>
#define I__strnset ImportFunc<I_MODULE_NT, H__strnset>
#define I__strnset_s ImportFunc<I_MODULE_NT, H__strnset_s>
#define I__strrev ImportFunc<I_MODULE_NT, H__strrev>
#define I__strset ImportFunc<I_MODULE_NT, H__strset>
#define I__strset_s ImportFunc<I_MODULE_NT, H__strset_s>
#define I__strtoui64 ImportFunc<I_MODULE_NT, H__strtoui64>
#define I__strupr ImportFunc<I_MODULE_NT, H__strupr>
#define I__swprintf ImportFunc<I_MODULE_NT, H__swprintf>
#define I__ui64toa_s ImportFunc<I_MODULE_NT, H__ui64toa_s>
#define I__ui64tow_s ImportFunc<I_MODULE_NT, H__ui64tow_s>
#define I__ultoa_s ImportFunc<I_MODULE_NT, H__ultoa_s>
#define I__ultow_s ImportFunc<I_MODULE_NT, H__ultow_s>
#define I__vsnprintf ImportFunc<I_MODULE_NT, H__vsnprintf>
#define I__vsnprintf_s ImportFunc<I_MODULE_NT, H__vsnprintf_s>
#define I__vsnwprintf ImportFunc<I_MODULE_NT, H__vsnwprintf>
#define I__vsnwprintf_s ImportFunc<I_MODULE_NT, H__vsnwprintf_s>
#define I__vswprintf ImportFunc<I_MODULE_NT, H__vswprintf>
#define I__wcsicmp ImportFunc<I_MODULE_NT, H__wcsicmp>
#define I__wcslwr ImportFunc<I_MODULE_NT, H__wcslwr>
#define I__wcsnicmp ImportFunc<I_MODULE_NT, H__wcsnicmp>
#define I__wcsnset ImportFunc<I_MODULE_NT, H__wcsnset>
#define I__wcsnset_s ImportFunc<I_MODULE_NT, H__wcsnset_s>
#define I__wcsrev ImportFunc<I_MODULE_NT, H__wcsrev>
#define I__wcsset_s ImportFunc<I_MODULE_NT, H__wcsset_s>
#define I__wcsupr ImportFunc<I_MODULE_NT, H__wcsupr>
#define I__wmakepath_s ImportFunc<I_MODULE_NT, H__wmakepath_s>
#define I__wsplitpath_s ImportFunc<I_MODULE_NT, H__wsplitpath_s>
#define I__wtoi ImportFunc<I_MODULE_NT, H__wtoi>
#define I__wtol ImportFunc<I_MODULE_NT, H__wtol>
#define I_atoi ImportFunc<I_MODULE_NT, H_atoi>
#define I_atol ImportFunc<I_MODULE_NT, H_atol>
#define I_bsearch ImportFunc<I_MODULE_NT, H_bsearch>
#define I_bsearch_s ImportFunc<I_MODULE_NT, H_bsearch_s>
#define I_isdigit ImportFunc<I_MODULE_NT, H_isdigit>
#define I_islower ImportFunc<I_MODULE_NT, H_islower>
#define I_isprint ImportFunc<I_MODULE_NT, H_isprint>
#define I_isspace ImportFunc<I_MODULE_NT, H_isspace>
#define I_isupper ImportFunc<I_MODULE_NT, H_isupper>
#define I_isxdigit ImportFunc<I_MODULE_NT, H_isxdigit>
#define I_longjmp ImportFunc<I_MODULE_NT, H_longjmp>
#define I_mbstowcs ImportFunc<I_MODULE_NT, H_mbstowcs>
#define I_mbtowc ImportFunc<I_MODULE_NT, H_mbtowc>
#define I_memchr ImportFunc<I_MODULE_NT, H_memchr>
#define I_memcmp ImportFunc<I_MODULE_NT, H_memcmp>
#define I_memcpy ImportFunc<I_MODULE_NT, H_memcpy>
#define I_memcpy_s ImportFunc<I_MODULE_NT, H_memcpy_s>
#define I_memmove ImportFunc<I_MODULE_NT, H_memmove>
#define I_memmove_s ImportFunc<I_MODULE_NT, H_memmove_s>
#define I_memset ImportFunc<I_MODULE_NT, H_memset>
#define I_psMUITest ImportFunc<I_MODULE_NT, H_psMUITest>
#define I_qsort ImportFunc<I_MODULE_NT, H_qsort>
#define I_rand ImportFunc<I_MODULE_NT, H_rand>
#define I_sprintf ImportFunc<I_MODULE_NT, H_sprintf>
#define I_sprintf_s ImportFunc<I_MODULE_NT, H_sprintf_s>
#define I_srand ImportFunc<I_MODULE_NT, H_srand>
#define I_sscanf_s ImportFunc<I_MODULE_NT, H_sscanf_s>
#define I_strcat ImportFunc<I_MODULE_NT, H_strcat>
#define I_strcat_s ImportFunc<I_MODULE_NT, H_strcat_s>
#define I_strchr ImportFunc<I_MODULE_NT, H_strchr>
#define I_strcmp ImportFunc<I_MODULE_NT, H_strcmp>
#define I_strcpy ImportFunc<I_MODULE_NT, H_strcpy>
#define I_strcpy_s ImportFunc<I_MODULE_NT, H_strcpy_s>
#define I_strlen ImportFunc<I_MODULE_NT, H_strlen>
#define I_strncat ImportFunc<I_MODULE_NT, H_strncat>
#define I_strncat_s ImportFunc<I_MODULE_NT, H_strncat_s>
#define I_strncmp ImportFunc<I_MODULE_NT, H_strncmp>
#define I_strncpy ImportFunc<I_MODULE_NT, H_strncpy>
#define I_strncpy_s ImportFunc<I_MODULE_NT, H_strncpy_s>
#define I_strnlen ImportFunc<I_MODULE_NT, H_strnlen>
#define I_strrchr ImportFunc<I_MODULE_NT, H_strrchr>
#define I_strspn ImportFunc<I_MODULE_NT, H_strspn>
#define I_strstr ImportFunc<I_MODULE_NT, H_strstr>
#define I_strtok_s ImportFunc<I_MODULE_NT, H_strtok_s>
#define I_swprintf ImportFunc<I_MODULE_NT, H_swprintf>
#define I_swprintf_s ImportFunc<I_MODULE_NT, H_swprintf_s>
#define I_swscanf_s ImportFunc<I_MODULE_NT, H_swscanf_s>
#define I_tolower ImportFunc<I_MODULE_NT, H_tolower>
#define I_toupper ImportFunc<I_MODULE_NT, H_toupper>
#define I_towlower ImportFunc<I_MODULE_NT, H_towlower>
#define I_towupper ImportFunc<I_MODULE_NT, H_towupper>
#define I_vDbgPrintEx ImportFunc<I_MODULE_NT, H_vDbgPrintEx>
#define I_vDbgPrintExWithPrefix ImportFunc<I_MODULE_NT, H_vDbgPrintExWithPrefix>
#define I_vsprintf ImportFunc<I_MODULE_NT, H_vsprintf>
#define I_vsprintf_s ImportFunc<I_MODULE_NT, H_vsprintf_s>
#define I_vswprintf_s ImportFunc<I_MODULE_NT, H_vswprintf_s>
#define I_wcscat ImportFunc<I_MODULE_NT, H_wcscat>
#define I_wcscat_s ImportFunc<I_MODULE_NT, H_wcscat_s>
#define I_wcschr ImportFunc<I_MODULE_NT, H_wcschr>
#define I_wcscmp ImportFunc<I_MODULE_NT, H_wcscmp>
#define I_wcscpy ImportFunc<I_MODULE_NT, H_wcscpy>
#define I_wcscpy_s ImportFunc<I_MODULE_NT, H_wcscpy_s>
#define I_wcscspn ImportFunc<I_MODULE_NT, H_wcscspn>
#define I_wcslen ImportFunc<I_MODULE_NT, H_wcslen>
#define I_wcsncat ImportFunc<I_MODULE_NT, H_wcsncat>
#define I_wcsncat_s ImportFunc<I_MODULE_NT, H_wcsncat_s>
#define I_wcsncmp ImportFunc<I_MODULE_NT, H_wcsncmp>
#define I_wcsncpy ImportFunc<I_MODULE_NT, H_wcsncpy>
#define I_wcsncpy_s ImportFunc<I_MODULE_NT, H_wcsncpy_s>
#define I_wcsnlen ImportFunc<I_MODULE_NT, H_wcsnlen>
#define I_wcsrchr ImportFunc<I_MODULE_NT, H_wcsrchr>
#define I_wcsspn ImportFunc<I_MODULE_NT, H_wcsspn>
#define I_wcsstr ImportFunc<I_MODULE_NT, H_wcsstr>
#define I_wcstombs ImportFunc<I_MODULE_NT, H_wcstombs>
#define I_wcstoul ImportFunc<I_MODULE_NT, H_wcstoul>
#define I_wctomb ImportFunc<I_MODULE_NT, H_wctomb>
