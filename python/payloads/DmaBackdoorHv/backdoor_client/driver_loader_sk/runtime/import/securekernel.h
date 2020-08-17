#define I_MODULE_SK 0 

#define H_DbgPrintEx 0xff12614c
#define H_EtwRegister 0xe9d8b4b6
#define H_EtwSetInformation 0xe65ec729
#define H_EtwUnregister 0x74c63826
#define H_EtwWrite 0x7ed1a9d8
#define H_EtwWriteTransfer 0x36aea855
#define H_ExAcquireFastMutex 0xb5db4261
#define H_ExAcquirePushLockExclusiveEx 0xd0a1da76
#define H_ExAcquirePushLockSharedEx 0x27ede09a
#define H_ExAcquireResourceExclusiveLite 0x0400d64d
#define H_ExAcquireResourceSharedLite 0xb1c2f003
#define H_ExAllocatePoolWithTag 0xdbe85b00
#define H_ExAllocateTimer 0x6ae53bff
#define H_ExCreateCallback 0xb6f3c515
#define H_ExDeletePagedLookasideList 0x63954ca0
#define H_ExDeleteResourceLite 0x324e392b
#define H_ExEventObjectType 0x1e53c34a
#define H_ExFreePoolWithTag 0x34d52a84
#define H_ExInitializePagedLookasideList 0x4801aeb3
#define H_ExInitializeResourceLite 0xb684dc13
#define H_ExIsResourceAcquiredSharedLite 0x0dd28da2
#define H_ExNotifyCallback 0xc321e71c
#define H_ExQueryDepthSList 0xf2600594
#define H_ExRegisterExtension 0x98e7f637
#define H_ExReleaseFastMutex 0x5cda6439
#define H_ExReleasePushLockExclusiveEx 0x90e84c4c
#define H_ExReleasePushLockSharedEx 0x6b5c3298
#define H_ExReleaseResourceLite 0x9ac37c91
#define H_ExSetTimer 0xb59ce575
#define H_ExSubscribeWnfStateChange 0xe5f95f1e
#define H_ExUnsubscribeWnfStateChange 0x74854f32
#define H_ExpInterlockedPopEntrySList 0x6f47b744
#define H_ExpInterlockedPushEntrySList 0xccd167c7
#define H_IoAllocateIrp 0x24f00609
#define H_IoAllocateWorkItem 0x9e8176b2
#define H_IoBuildDeviceIoControlRequest 0x7ce4cfc2
#define H_IoCreateDevice 0xf1cb9b73
#define H_IoDeleteDevice 0x75cb7eb7
#define H_IoGetDeviceObjectPointer 0xde00a6b3
#define H_IoQueueWorkItem 0x218c3526
#define H_IoQueueWorkItemEx 0x0d49aa9b
#define H_IoRegisterPlugPlayNotification 0xc87727bf
#define H_IoReuseIrp 0xd317eefb
#define H_IoUnregisterPlugPlayNotificationEx 0xf1eaf40a
#define H_IoWMIRegistrationControl 0xaa368fce
#define H_IofCallDriver 0xc879f974
#define H_IofCompleteRequest 0xb043e4e6
#define H_IumDebugNumToString 0xce42b3c7
#define H_IumDebugPrintNt 0x46cafcb4
#define H_KeAcquireSpinLockRaiseToDpc 0xf6720183
#define H_KeBugCheck 0xd89ca6f2
#define H_KeBugCheckEx 0x29bc94df
#define H_KeDelayExecutionThread 0x55e51cb6
#define H_KeEnterCriticalRegion 0xd7648f37
#define H_KeEnterGuardedRegion 0xcea1bdef
#define H_KeGetCurrentIrql 0x858728f7
#define H_KeGetCurrentProcessorNumberEx 0x1c4a305e
#define H_KeGetCurrentThread 0x6a5f916c
#define H_KeInitializeEvent 0xc4dfcd0f
#define H_KeInitializeMutex 0x44bb8883
#define H_KeInitializeSpinLock 0xa6cf7272
#define H_KeLeaveCriticalRegion 0x92f1a969
#define H_KeLeaveGuardedRegion 0x722a97a3
#define H_KeQueryPerformanceCounter 0x6ef075f4
#define H_KeQueryUnbiasedInterruptTime 0x9aa633dd
#define H_KeReleaseMutex 0xe524a4a0
#define H_KeReleaseSpinLock 0x22bb4197
#define H_KeResetEvent 0x3b3799b9
#define H_KeRestoreExtendedProcessorState 0x16cc3ac0
#define H_KeSaveExtendedProcessorState 0xd27099ed
#define H_KeSetEvent 0xbb7ee0f5
#define H_KeWaitForSingleObject 0xc547c853
#define H_MmAllocateMappingAddress 0x89674677
#define H_MmFreeMappingAddress 0xadfaa0cc
#define H_MmFreePagesFromMdl 0x10e3fc42
#define H_MmGetSystemRoutineAddress 0x50b70ff2
#define H_MmMapLockedPagesSpecifyCache 0x0a990c95
#define H_MmMapLockedPagesWithReservedMapping 0x8e06877d
#define H_MmUnmapLockedPages 0xa71ddd5a
#define H_MmUnmapReservedMapping 0xf060c723
#define H_NtQuerySystemInformation 0xb044a119
#define H_ObReferenceObjectByHandle 0x6eb16dbd
#define H_ObSetSecurityObjectByPointer 0xcaf1e115
#define H_ObfDereferenceObject 0x5eb6990b
#define H_ObfReferenceObject 0xca109673
#define H_PsGetCurrentProcess 0x3409ad11
#define H_PsGetProcessCreateTimeQuadPart 0xae75762a
#define H_PsLookupProcessByProcessId 0x368339ec
#define H_PsSetCreateProcessNotifyRoutine 0xa8a33f0a
#define H_RtlAnsiStringToUnicodeString 0xfa5b8b91
#define H_RtlAppendUnicodeStringToString 0xec0de045
#define H_RtlAppendUnicodeToString 0xbce565e4
#define H_RtlAssert 0x6c90cb7b
#define H_RtlAvlInsertNodeEx 0x86c15230
#define H_RtlAvlRemoveNode 0x581e3329
#define H_RtlClearAllBits 0x8f77076c
#define H_RtlClearBit 0x72d71360
#define H_RtlCompareMemory 0x586331bc
#define H_RtlCompareUnicodeString 0x31983038
#define H_RtlCompareUnicodeStrings 0xcc181c6b
#define H_RtlCopyUnicodeString 0x5a8dee17
#define H_RtlDuplicateUnicodeString 0x0a0af4a8
#define H_RtlEqualUnicodeString 0xe63afe95
#define H_RtlFindExportedRoutineByName 0x04c8edda
#define H_RtlFindNextForwardRunClear 0x8f8d2eae
#define H_RtlFindSetBits 0x31dea4ea
#define H_RtlFreeUnicodeString 0xba88d443
#define H_RtlGetEnabledExtendedFeatures 0xb2dd8bb1
#define H_RtlGetPersistedStateLocation 0x257ea080
#define H_RtlGetVersion 0xcebdb7e2
#define H_RtlImageNtHeaderEx 0x7f4515b6
#define H_RtlInitUnicodeString 0x3287ec73
#define H_RtlInitUnicodeStringEx 0xfb1cee59
#define H_RtlInitializeBitMap 0x02474d3f
#define H_RtlIntegerToUnicodeString 0x1d6562aa
#define H_RtlNtStatusToDosError 0x19ea3dcc
#define H_RtlNtStatusToDosErrorNoTeb 0x22a4dc86
#define H_RtlNumberOfSetBits 0x803fb51e
#define H_RtlPrefixUnicodeString 0x5f3cbbba
#define H_RtlQueryRegistryValuesEx 0xb193a32d
#define H_RtlRbInsertNodeEx 0x8f076285
#define H_RtlRbRemoveNode 0x9aca1431
#define H_RtlSetBit 0x0c79066a
#define H_RtlSetBits 0x3c833575
#define H_RtlTimeFieldsToTime 0xc267cb82
#define H_RtlUTF8ToUnicodeN 0xff48ae18
#define H_RtlUnicodeStringToInteger 0x7a019aa8
#define H_RtlUnicodeToUTF8N 0x10e51e76
#define H_RtlUpcaseUnicodeChar 0x08d4d171
#define H_SeAuditFipsCryptoSelftests 0x6b16c86e
#define H_SeCaptureSubjectContext 0xe775f013
#define H_SeLockSubjectContext 0xf443e46f
#define H_SeQueryAuthenticationIdToken 0x6adade50
#define H_SeQuerySecureBootPlatformManifest 0x4b429759
#define H_SeQuerySecureBootPolicyValue 0xb641236d
#define H_SeReleaseSubjectContext 0xefe0e294
#define H_SeReportSecurityEventWithSubCategory 0x76cf100d
#define H_SeSetAuditParameter 0x71f1a840
#define H_SeUnlockSubjectContext 0x26ddb4ad
#define H_ShvlCompleteIntercept 0xd52ef4c3
#define H_ShvlEnableVpVtlForPartition 0x59c5ec68
#define H_ShvlGetInterceptData 0x25aeb539
#define H_ShvlGetPartitionProperty 0x9d65e0e4
#define H_ShvlGetVpRegisters 0x7be49e36
#define H_ShvlLockSparseGpaPageMapping 0x230a4521
#define H_ShvlModifySparseSpaPageHostAccess 0x40382f3e
#define H_ShvlSetPartitionProperty 0x9d65e264
#define H_ShvlSetVpRegisters 0xdbe49e36
#define H_ShvlUnlockSparseGpaPageMapping 0x821959e5
#define H_SkAcquirePushLockExclusive 0xe2305187
#define H_SkAcquirePushLockShared 0x51109fb7
#define H_SkAllocateNormalModePool 0x9f913710
#define H_SkAllocatePool 0xab644b7e
#define H_SkFreeNormalModePool 0x230cd256
#define H_SkFreePool 0xb1963ceb
#define H_SkGetIdkSignatureForData 0x38eaa5b6
#define H_SkInitializePushLock 0x46c965a1
#define H_SkIsSecureKernel 0xe484fa6d
#define H_SkQuerySecureKernelInformation 0xce8db7c9
#define H_SkQuerySystemTime 0x8d702ce5
#define H_SkReleasePushLockExclusive 0xbad950a1
#define H_SkReleasePushLockShared 0x1919ad70
#define H_SkSystemExceptionFilter 0x8746d0d4
#define H_SkciCreateSecureImage 0x85d9d4d4
#define H_SkciFinalizeSecureImageHash 0x85bc86c3
#define H_SkciFinishImageValidation 0x2ae0c668
#define H_SkciFreeImageContext 0xd6e2cfcb
#define H_SkeCacheInvalidatePage 0x8f424113
#define H_SkeZeroPages 0xd6f8437c
#define H_SkmmFreeReservedMapping 0xb2d4ea28
#define H_SkmmFreeSecureAllocation 0xf096ee7c
#define H_SkmmMapMdl 0xf5c8c42b
#define H_SkmmMapMdlWithReservedMapping 0x0903a8f3
#define H_SkmmReleasePageRestriction 0x05437513
#define H_SkmmReserveMappingAddress 0x8b17f01c
#define H_SkmmRestrictPage 0xd0272a2c
#define H_SkmmUnmapMdl 0xf3b2b3f4
#define H_SkobCreateHandle 0x82187b3e
#define H_SkobCreateObject 0xb2983c97
#define H_SkobDereferenceObject 0x1cb3ad88
#define H_SkobReferenceObject 0x181d9e67
#define H_SkobReferenceObjectByHandle 0x7a44cdbd
#define H_VslExchangeEntropy 0xc4de2e6e
#define H_ZwClose 0x3d9a9259
#define H_ZwCreateKey 0x8f57734d
#define H_ZwDeleteKey 0x8879576d
#define H_ZwDeleteValueKey 0x1f42593a
#define H_ZwEnumerateKey 0x7601824e
#define H_ZwEnumerateValueKey 0xec84f025
#define H_ZwLoadDriver 0x42f57d33
#define H_ZwOpenKey 0x073dcd7f
#define H_ZwQueryInformationProcess 0xa638ce5f
#define H_ZwQueryKey 0x589125fb
#define H_ZwQuerySystemInformation 0xbc44a131
#define H_ZwQueryValueKey 0x9801cd8c
#define H_ZwSetSystemInformation 0xa69c8741
#define H_ZwSetValueKey 0xadb1816c
#define H_ZwUnloadDriver 0x95849b61
#define H___C_specific_handler 0xc0c6447c
#define H___GSHandlerCheck 0x64f94176
#define H___GSHandlerCheck_SEH 0x6dbb76df
#define H___chkstk 0x8dc38575
#define H__invalid_parameter 0x0d5a1d08
#define H__local_unwind 0x0ade933b
#define H__ultow_s 0x4d423894
#define H__vsnwprintf 0x5ee6721a
#define H__wcsicmp 0x3d8729ef
#define H__wcsnicmp 0xb2d7796e
#define H_atoi 0x0c3d37e9
#define H_atol 0x0c3d37ec
#define H_bsearch 0x5c3d3a76
#define H_bsearch_s 0x4e9db8fc
#define H_isdigit 0x4d38536a
#define H_memcmp 0x5db8f59e
#define H_memcpy 0x5db8fb17
#define H_memmove 0xddba4c4b
#define H_memset 0x5dbcf19a
#define H_qsort 0x1e7bf973
#define H_strnlen 0x2ddafd49
#define H_wcscmp 0x3e78f54e
#define H_wcscpy_s 0x3ef1e06d
#define H_wcsncmp 0x3dd929ef

#define I_DbgPrintEx ImportFunc<I_MODULE_SK, H_DbgPrintEx>
#define I_EtwRegister ImportFunc<I_MODULE_SK, H_EtwRegister>
#define I_EtwSetInformation ImportFunc<I_MODULE_SK, H_EtwSetInformation>
#define I_EtwUnregister ImportFunc<I_MODULE_SK, H_EtwUnregister>
#define I_EtwWrite ImportFunc<I_MODULE_SK, H_EtwWrite>
#define I_EtwWriteTransfer ImportFunc<I_MODULE_SK, H_EtwWriteTransfer>
#define I_ExAcquireFastMutex ImportFunc<I_MODULE_SK, H_ExAcquireFastMutex>
#define I_ExAcquirePushLockExclusiveEx ImportFunc<I_MODULE_SK, H_ExAcquirePushLockExclusiveEx>
#define I_ExAcquirePushLockSharedEx ImportFunc<I_MODULE_SK, H_ExAcquirePushLockSharedEx>
#define I_ExAcquireResourceExclusiveLite ImportFunc<I_MODULE_SK, H_ExAcquireResourceExclusiveLite>
#define I_ExAcquireResourceSharedLite ImportFunc<I_MODULE_SK, H_ExAcquireResourceSharedLite>
#define I_ExAllocatePoolWithTag ImportFunc<I_MODULE_SK, H_ExAllocatePoolWithTag>
#define I_ExAllocateTimer ImportFunc<I_MODULE_SK, H_ExAllocateTimer>
#define I_ExCreateCallback ImportFunc<I_MODULE_SK, H_ExCreateCallback>
#define I_ExDeletePagedLookasideList ImportFunc<I_MODULE_SK, H_ExDeletePagedLookasideList>
#define I_ExDeleteResourceLite ImportFunc<I_MODULE_SK, H_ExDeleteResourceLite>
#define I_ExEventObjectType ImportFunc<I_MODULE_SK, H_ExEventObjectType>
#define I_ExFreePoolWithTag ImportFunc<I_MODULE_SK, H_ExFreePoolWithTag>
#define I_ExInitializePagedLookasideList ImportFunc<I_MODULE_SK, H_ExInitializePagedLookasideList>
#define I_ExInitializeResourceLite ImportFunc<I_MODULE_SK, H_ExInitializeResourceLite>
#define I_ExIsResourceAcquiredSharedLite ImportFunc<I_MODULE_SK, H_ExIsResourceAcquiredSharedLite>
#define I_ExNotifyCallback ImportFunc<I_MODULE_SK, H_ExNotifyCallback>
#define I_ExQueryDepthSList ImportFunc<I_MODULE_SK, H_ExQueryDepthSList>
#define I_ExRegisterExtension ImportFunc<I_MODULE_SK, H_ExRegisterExtension>
#define I_ExReleaseFastMutex ImportFunc<I_MODULE_SK, H_ExReleaseFastMutex>
#define I_ExReleasePushLockExclusiveEx ImportFunc<I_MODULE_SK, H_ExReleasePushLockExclusiveEx>
#define I_ExReleasePushLockSharedEx ImportFunc<I_MODULE_SK, H_ExReleasePushLockSharedEx>
#define I_ExReleaseResourceLite ImportFunc<I_MODULE_SK, H_ExReleaseResourceLite>
#define I_ExSetTimer ImportFunc<I_MODULE_SK, H_ExSetTimer>
#define I_ExSubscribeWnfStateChange ImportFunc<I_MODULE_SK, H_ExSubscribeWnfStateChange>
#define I_ExUnsubscribeWnfStateChange ImportFunc<I_MODULE_SK, H_ExUnsubscribeWnfStateChange>
#define I_ExpInterlockedPopEntrySList ImportFunc<I_MODULE_SK, H_ExpInterlockedPopEntrySList>
#define I_ExpInterlockedPushEntrySList ImportFunc<I_MODULE_SK, H_ExpInterlockedPushEntrySList>
#define I_IoAllocateIrp ImportFunc<I_MODULE_SK, H_IoAllocateIrp>
#define I_IoAllocateWorkItem ImportFunc<I_MODULE_SK, H_IoAllocateWorkItem>
#define I_IoBuildDeviceIoControlRequest ImportFunc<I_MODULE_SK, H_IoBuildDeviceIoControlRequest>
#define I_IoCreateDevice ImportFunc<I_MODULE_SK, H_IoCreateDevice>
#define I_IoDeleteDevice ImportFunc<I_MODULE_SK, H_IoDeleteDevice>
#define I_IoGetDeviceObjectPointer ImportFunc<I_MODULE_SK, H_IoGetDeviceObjectPointer>
#define I_IoQueueWorkItem ImportFunc<I_MODULE_SK, H_IoQueueWorkItem>
#define I_IoQueueWorkItemEx ImportFunc<I_MODULE_SK, H_IoQueueWorkItemEx>
#define I_IoRegisterPlugPlayNotification ImportFunc<I_MODULE_SK, H_IoRegisterPlugPlayNotification>
#define I_IoReuseIrp ImportFunc<I_MODULE_SK, H_IoReuseIrp>
#define I_IoUnregisterPlugPlayNotificationEx ImportFunc<I_MODULE_SK, H_IoUnregisterPlugPlayNotificationEx>
#define I_IoWMIRegistrationControl ImportFunc<I_MODULE_SK, H_IoWMIRegistrationControl>
#define I_IofCallDriver ImportFunc<I_MODULE_SK, H_IofCallDriver>
#define I_IofCompleteRequest ImportFunc<I_MODULE_SK, H_IofCompleteRequest>
#define I_IumDebugNumToString ImportFunc<I_MODULE_SK, H_IumDebugNumToString>
#define I_IumDebugPrintNt ImportFunc<I_MODULE_SK, H_IumDebugPrintNt>
#define I_KeAcquireSpinLockRaiseToDpc ImportFunc<I_MODULE_SK, H_KeAcquireSpinLockRaiseToDpc>
#define I_KeBugCheck ImportFunc<I_MODULE_SK, H_KeBugCheck>
#define I_KeBugCheckEx ImportFunc<I_MODULE_SK, H_KeBugCheckEx>
#define I_KeDelayExecutionThread ImportFunc<I_MODULE_SK, H_KeDelayExecutionThread>
#define I_KeEnterCriticalRegion ImportFunc<I_MODULE_SK, H_KeEnterCriticalRegion>
#define I_KeEnterGuardedRegion ImportFunc<I_MODULE_SK, H_KeEnterGuardedRegion>
#define I_KeGetCurrentIrql ImportFunc<I_MODULE_SK, H_KeGetCurrentIrql>
#define I_KeGetCurrentProcessorNumberEx ImportFunc<I_MODULE_SK, H_KeGetCurrentProcessorNumberEx>
#define I_KeGetCurrentThread ImportFunc<I_MODULE_SK, H_KeGetCurrentThread>
#define I_KeInitializeEvent ImportFunc<I_MODULE_SK, H_KeInitializeEvent>
#define I_KeInitializeMutex ImportFunc<I_MODULE_SK, H_KeInitializeMutex>
#define I_KeInitializeSpinLock ImportFunc<I_MODULE_SK, H_KeInitializeSpinLock>
#define I_KeLeaveCriticalRegion ImportFunc<I_MODULE_SK, H_KeLeaveCriticalRegion>
#define I_KeLeaveGuardedRegion ImportFunc<I_MODULE_SK, H_KeLeaveGuardedRegion>
#define I_KeQueryPerformanceCounter ImportFunc<I_MODULE_SK, H_KeQueryPerformanceCounter>
#define I_KeQueryUnbiasedInterruptTime ImportFunc<I_MODULE_SK, H_KeQueryUnbiasedInterruptTime>
#define I_KeReleaseMutex ImportFunc<I_MODULE_SK, H_KeReleaseMutex>
#define I_KeReleaseSpinLock ImportFunc<I_MODULE_SK, H_KeReleaseSpinLock>
#define I_KeResetEvent ImportFunc<I_MODULE_SK, H_KeResetEvent>
#define I_KeRestoreExtendedProcessorState ImportFunc<I_MODULE_SK, H_KeRestoreExtendedProcessorState>
#define I_KeSaveExtendedProcessorState ImportFunc<I_MODULE_SK, H_KeSaveExtendedProcessorState>
#define I_KeSetEvent ImportFunc<I_MODULE_SK, H_KeSetEvent>
#define I_KeWaitForSingleObject ImportFunc<I_MODULE_SK, H_KeWaitForSingleObject>
#define I_MmAllocateMappingAddress ImportFunc<I_MODULE_SK, H_MmAllocateMappingAddress>
#define I_MmFreeMappingAddress ImportFunc<I_MODULE_SK, H_MmFreeMappingAddress>
#define I_MmFreePagesFromMdl ImportFunc<I_MODULE_SK, H_MmFreePagesFromMdl>
#define I_MmGetSystemRoutineAddress ImportFunc<I_MODULE_SK, H_MmGetSystemRoutineAddress>
#define I_MmMapLockedPagesSpecifyCache ImportFunc<I_MODULE_SK, H_MmMapLockedPagesSpecifyCache>
#define I_MmMapLockedPagesWithReservedMapping ImportFunc<I_MODULE_SK, H_MmMapLockedPagesWithReservedMapping>
#define I_MmUnmapLockedPages ImportFunc<I_MODULE_SK, H_MmUnmapLockedPages>
#define I_MmUnmapReservedMapping ImportFunc<I_MODULE_SK, H_MmUnmapReservedMapping>
#define I_NtQuerySystemInformation ImportFunc<I_MODULE_SK, H_NtQuerySystemInformation>
#define I_ObReferenceObjectByHandle ImportFunc<I_MODULE_SK, H_ObReferenceObjectByHandle>
#define I_ObSetSecurityObjectByPointer ImportFunc<I_MODULE_SK, H_ObSetSecurityObjectByPointer>
#define I_ObfDereferenceObject ImportFunc<I_MODULE_SK, H_ObfDereferenceObject>
#define I_ObfReferenceObject ImportFunc<I_MODULE_SK, H_ObfReferenceObject>
#define I_PsGetCurrentProcess ImportFunc<I_MODULE_SK, H_PsGetCurrentProcess>
#define I_PsGetProcessCreateTimeQuadPart ImportFunc<I_MODULE_SK, H_PsGetProcessCreateTimeQuadPart>
#define I_PsLookupProcessByProcessId ImportFunc<I_MODULE_SK, H_PsLookupProcessByProcessId>
#define I_PsSetCreateProcessNotifyRoutine ImportFunc<I_MODULE_SK, H_PsSetCreateProcessNotifyRoutine>
#define I_RtlAnsiStringToUnicodeString ImportFunc<I_MODULE_SK, H_RtlAnsiStringToUnicodeString>
#define I_RtlAppendUnicodeStringToString ImportFunc<I_MODULE_SK, H_RtlAppendUnicodeStringToString>
#define I_RtlAppendUnicodeToString ImportFunc<I_MODULE_SK, H_RtlAppendUnicodeToString>
#define I_RtlAssert ImportFunc<I_MODULE_SK, H_RtlAssert>
#define I_RtlAvlInsertNodeEx ImportFunc<I_MODULE_SK, H_RtlAvlInsertNodeEx>
#define I_RtlAvlRemoveNode ImportFunc<I_MODULE_SK, H_RtlAvlRemoveNode>
#define I_RtlClearAllBits ImportFunc<I_MODULE_SK, H_RtlClearAllBits>
#define I_RtlClearBit ImportFunc<I_MODULE_SK, H_RtlClearBit>
#define I_RtlCompareMemory ImportFunc<I_MODULE_SK, H_RtlCompareMemory>
#define I_RtlCompareUnicodeString ImportFunc<I_MODULE_SK, H_RtlCompareUnicodeString>
#define I_RtlCompareUnicodeStrings ImportFunc<I_MODULE_SK, H_RtlCompareUnicodeStrings>
#define I_RtlCopyUnicodeString ImportFunc<I_MODULE_SK, H_RtlCopyUnicodeString>
#define I_RtlDuplicateUnicodeString ImportFunc<I_MODULE_SK, H_RtlDuplicateUnicodeString>
#define I_RtlEqualUnicodeString ImportFunc<I_MODULE_SK, H_RtlEqualUnicodeString>
#define I_RtlFindExportedRoutineByName ImportFunc<I_MODULE_SK, H_RtlFindExportedRoutineByName>
#define I_RtlFindNextForwardRunClear ImportFunc<I_MODULE_SK, H_RtlFindNextForwardRunClear>
#define I_RtlFindSetBits ImportFunc<I_MODULE_SK, H_RtlFindSetBits>
#define I_RtlFreeUnicodeString ImportFunc<I_MODULE_SK, H_RtlFreeUnicodeString>
#define I_RtlGetEnabledExtendedFeatures ImportFunc<I_MODULE_SK, H_RtlGetEnabledExtendedFeatures>
#define I_RtlGetPersistedStateLocation ImportFunc<I_MODULE_SK, H_RtlGetPersistedStateLocation>
#define I_RtlGetVersion ImportFunc<I_MODULE_SK, H_RtlGetVersion>
#define I_RtlImageNtHeaderEx ImportFunc<I_MODULE_SK, H_RtlImageNtHeaderEx>
#define I_RtlInitUnicodeString ImportFunc<I_MODULE_SK, H_RtlInitUnicodeString>
#define I_RtlInitUnicodeStringEx ImportFunc<I_MODULE_SK, H_RtlInitUnicodeStringEx>
#define I_RtlInitializeBitMap ImportFunc<I_MODULE_SK, H_RtlInitializeBitMap>
#define I_RtlIntegerToUnicodeString ImportFunc<I_MODULE_SK, H_RtlIntegerToUnicodeString>
#define I_RtlNtStatusToDosError ImportFunc<I_MODULE_SK, H_RtlNtStatusToDosError>
#define I_RtlNtStatusToDosErrorNoTeb ImportFunc<I_MODULE_SK, H_RtlNtStatusToDosErrorNoTeb>
#define I_RtlNumberOfSetBits ImportFunc<I_MODULE_SK, H_RtlNumberOfSetBits>
#define I_RtlPrefixUnicodeString ImportFunc<I_MODULE_SK, H_RtlPrefixUnicodeString>
#define I_RtlQueryRegistryValuesEx ImportFunc<I_MODULE_SK, H_RtlQueryRegistryValuesEx>
#define I_RtlRbInsertNodeEx ImportFunc<I_MODULE_SK, H_RtlRbInsertNodeEx>
#define I_RtlRbRemoveNode ImportFunc<I_MODULE_SK, H_RtlRbRemoveNode>
#define I_RtlSetBit ImportFunc<I_MODULE_SK, H_RtlSetBit>
#define I_RtlSetBits ImportFunc<I_MODULE_SK, H_RtlSetBits>
#define I_RtlTimeFieldsToTime ImportFunc<I_MODULE_SK, H_RtlTimeFieldsToTime>
#define I_RtlUTF8ToUnicodeN ImportFunc<I_MODULE_SK, H_RtlUTF8ToUnicodeN>
#define I_RtlUnicodeStringToInteger ImportFunc<I_MODULE_SK, H_RtlUnicodeStringToInteger>
#define I_RtlUnicodeToUTF8N ImportFunc<I_MODULE_SK, H_RtlUnicodeToUTF8N>
#define I_RtlUpcaseUnicodeChar ImportFunc<I_MODULE_SK, H_RtlUpcaseUnicodeChar>
#define I_SeAuditFipsCryptoSelftests ImportFunc<I_MODULE_SK, H_SeAuditFipsCryptoSelftests>
#define I_SeCaptureSubjectContext ImportFunc<I_MODULE_SK, H_SeCaptureSubjectContext>
#define I_SeLockSubjectContext ImportFunc<I_MODULE_SK, H_SeLockSubjectContext>
#define I_SeQueryAuthenticationIdToken ImportFunc<I_MODULE_SK, H_SeQueryAuthenticationIdToken>
#define I_SeQuerySecureBootPlatformManifest ImportFunc<I_MODULE_SK, H_SeQuerySecureBootPlatformManifest>
#define I_SeQuerySecureBootPolicyValue ImportFunc<I_MODULE_SK, H_SeQuerySecureBootPolicyValue>
#define I_SeReleaseSubjectContext ImportFunc<I_MODULE_SK, H_SeReleaseSubjectContext>
#define I_SeReportSecurityEventWithSubCategory ImportFunc<I_MODULE_SK, H_SeReportSecurityEventWithSubCategory>
#define I_SeSetAuditParameter ImportFunc<I_MODULE_SK, H_SeSetAuditParameter>
#define I_SeUnlockSubjectContext ImportFunc<I_MODULE_SK, H_SeUnlockSubjectContext>
#define I_ShvlCompleteIntercept ImportFunc<I_MODULE_SK, H_ShvlCompleteIntercept>
#define I_ShvlEnableVpVtlForPartition ImportFunc<I_MODULE_SK, H_ShvlEnableVpVtlForPartition>
#define I_ShvlGetInterceptData ImportFunc<I_MODULE_SK, H_ShvlGetInterceptData>
#define I_ShvlGetPartitionProperty ImportFunc<I_MODULE_SK, H_ShvlGetPartitionProperty>
#define I_ShvlGetVpRegisters ImportFunc<I_MODULE_SK, H_ShvlGetVpRegisters>
#define I_ShvlLockSparseGpaPageMapping ImportFunc<I_MODULE_SK, H_ShvlLockSparseGpaPageMapping>
#define I_ShvlModifySparseSpaPageHostAccess ImportFunc<I_MODULE_SK, H_ShvlModifySparseSpaPageHostAccess>
#define I_ShvlSetPartitionProperty ImportFunc<I_MODULE_SK, H_ShvlSetPartitionProperty>
#define I_ShvlSetVpRegisters ImportFunc<I_MODULE_SK, H_ShvlSetVpRegisters>
#define I_ShvlUnlockSparseGpaPageMapping ImportFunc<I_MODULE_SK, H_ShvlUnlockSparseGpaPageMapping>
#define I_SkAcquirePushLockExclusive ImportFunc<I_MODULE_SK, H_SkAcquirePushLockExclusive>
#define I_SkAcquirePushLockShared ImportFunc<I_MODULE_SK, H_SkAcquirePushLockShared>
#define I_SkAllocateNormalModePool ImportFunc<I_MODULE_SK, H_SkAllocateNormalModePool>
#define I_SkAllocatePool ImportFunc<I_MODULE_SK, H_SkAllocatePool>
#define I_SkFreeNormalModePool ImportFunc<I_MODULE_SK, H_SkFreeNormalModePool>
#define I_SkFreePool ImportFunc<I_MODULE_SK, H_SkFreePool>
#define I_SkGetIdkSignatureForData ImportFunc<I_MODULE_SK, H_SkGetIdkSignatureForData>
#define I_SkInitializePushLock ImportFunc<I_MODULE_SK, H_SkInitializePushLock>
#define I_SkIsSecureKernel ImportFunc<I_MODULE_SK, H_SkIsSecureKernel>
#define I_SkQuerySecureKernelInformation ImportFunc<I_MODULE_SK, H_SkQuerySecureKernelInformation>
#define I_SkQuerySystemTime ImportFunc<I_MODULE_SK, H_SkQuerySystemTime>
#define I_SkReleasePushLockExclusive ImportFunc<I_MODULE_SK, H_SkReleasePushLockExclusive>
#define I_SkReleasePushLockShared ImportFunc<I_MODULE_SK, H_SkReleasePushLockShared>
#define I_SkSystemExceptionFilter ImportFunc<I_MODULE_SK, H_SkSystemExceptionFilter>
#define I_SkciCreateSecureImage ImportFunc<I_MODULE_SK, H_SkciCreateSecureImage>
#define I_SkciFinalizeSecureImageHash ImportFunc<I_MODULE_SK, H_SkciFinalizeSecureImageHash>
#define I_SkciFinishImageValidation ImportFunc<I_MODULE_SK, H_SkciFinishImageValidation>
#define I_SkciFreeImageContext ImportFunc<I_MODULE_SK, H_SkciFreeImageContext>
#define I_SkeCacheInvalidatePage ImportFunc<I_MODULE_SK, H_SkeCacheInvalidatePage>
#define I_SkeZeroPages ImportFunc<I_MODULE_SK, H_SkeZeroPages>
#define I_SkmmFreeReservedMapping ImportFunc<I_MODULE_SK, H_SkmmFreeReservedMapping>
#define I_SkmmFreeSecureAllocation ImportFunc<I_MODULE_SK, H_SkmmFreeSecureAllocation>
#define I_SkmmMapMdl ImportFunc<I_MODULE_SK, H_SkmmMapMdl>
#define I_SkmmMapMdlWithReservedMapping ImportFunc<I_MODULE_SK, H_SkmmMapMdlWithReservedMapping>
#define I_SkmmReleasePageRestriction ImportFunc<I_MODULE_SK, H_SkmmReleasePageRestriction>
#define I_SkmmReserveMappingAddress ImportFunc<I_MODULE_SK, H_SkmmReserveMappingAddress>
#define I_SkmmRestrictPage ImportFunc<I_MODULE_SK, H_SkmmRestrictPage>
#define I_SkmmUnmapMdl ImportFunc<I_MODULE_SK, H_SkmmUnmapMdl>
#define I_SkobCreateHandle ImportFunc<I_MODULE_SK, H_SkobCreateHandle>
#define I_SkobCreateObject ImportFunc<I_MODULE_SK, H_SkobCreateObject>
#define I_SkobDereferenceObject ImportFunc<I_MODULE_SK, H_SkobDereferenceObject>
#define I_SkobReferenceObject ImportFunc<I_MODULE_SK, H_SkobReferenceObject>
#define I_SkobReferenceObjectByHandle ImportFunc<I_MODULE_SK, H_SkobReferenceObjectByHandle>
#define I_VslExchangeEntropy ImportFunc<I_MODULE_SK, H_VslExchangeEntropy>
#define I_ZwClose ImportFunc<I_MODULE_SK, H_ZwClose>
#define I_ZwCreateKey ImportFunc<I_MODULE_SK, H_ZwCreateKey>
#define I_ZwDeleteKey ImportFunc<I_MODULE_SK, H_ZwDeleteKey>
#define I_ZwDeleteValueKey ImportFunc<I_MODULE_SK, H_ZwDeleteValueKey>
#define I_ZwEnumerateKey ImportFunc<I_MODULE_SK, H_ZwEnumerateKey>
#define I_ZwEnumerateValueKey ImportFunc<I_MODULE_SK, H_ZwEnumerateValueKey>
#define I_ZwLoadDriver ImportFunc<I_MODULE_SK, H_ZwLoadDriver>
#define I_ZwOpenKey ImportFunc<I_MODULE_SK, H_ZwOpenKey>
#define I_ZwQueryInformationProcess ImportFunc<I_MODULE_SK, H_ZwQueryInformationProcess>
#define I_ZwQueryKey ImportFunc<I_MODULE_SK, H_ZwQueryKey>
#define I_ZwQuerySystemInformation ImportFunc<I_MODULE_SK, H_ZwQuerySystemInformation>
#define I_ZwQueryValueKey ImportFunc<I_MODULE_SK, H_ZwQueryValueKey>
#define I_ZwSetSystemInformation ImportFunc<I_MODULE_SK, H_ZwSetSystemInformation>
#define I_ZwSetValueKey ImportFunc<I_MODULE_SK, H_ZwSetValueKey>
#define I_ZwUnloadDriver ImportFunc<I_MODULE_SK, H_ZwUnloadDriver>
#define I___C_specific_handler ImportFunc<I_MODULE_SK, H___C_specific_handler>
#define I___GSHandlerCheck ImportFunc<I_MODULE_SK, H___GSHandlerCheck>
#define I___GSHandlerCheck_SEH ImportFunc<I_MODULE_SK, H___GSHandlerCheck_SEH>
#define I___chkstk ImportFunc<I_MODULE_SK, H___chkstk>
#define I__invalid_parameter ImportFunc<I_MODULE_SK, H__invalid_parameter>
#define I__local_unwind ImportFunc<I_MODULE_SK, H__local_unwind>
#define I__ultow_s ImportFunc<I_MODULE_SK, H__ultow_s>
#define I__vsnwprintf ImportFunc<I_MODULE_SK, H__vsnwprintf>
#define I__wcsicmp ImportFunc<I_MODULE_SK, H__wcsicmp>
#define I__wcsnicmp ImportFunc<I_MODULE_SK, H__wcsnicmp>
#define I_atoi ImportFunc<I_MODULE_SK, H_atoi>
#define I_atol ImportFunc<I_MODULE_SK, H_atol>
#define I_bsearch ImportFunc<I_MODULE_SK, H_bsearch>
#define I_bsearch_s ImportFunc<I_MODULE_SK, H_bsearch_s>
#define I_isdigit ImportFunc<I_MODULE_SK, H_isdigit>
#define I_memcmp ImportFunc<I_MODULE_SK, H_memcmp>
#define I_memcpy ImportFunc<I_MODULE_SK, H_memcpy>
#define I_memmove ImportFunc<I_MODULE_SK, H_memmove>
#define I_memset ImportFunc<I_MODULE_SK, H_memset>
#define I_qsort ImportFunc<I_MODULE_SK, H_qsort>
#define I_strnlen ImportFunc<I_MODULE_SK, H_strnlen>
#define I_wcscmp ImportFunc<I_MODULE_SK, H_wcscmp>
#define I_wcscpy_s ImportFunc<I_MODULE_SK, H_wcscpy_s>
#define I_wcsncmp ImportFunc<I_MODULE_SK, H_wcsncmp>
