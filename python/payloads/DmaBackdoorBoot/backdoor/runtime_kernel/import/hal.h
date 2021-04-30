#define I_MODULE_HAL 1 
 
#define H_HalAcpiGetTableEx 0xc606e827
#define H_HalAcquireDisplayOwnership 0x00c6e167
#define H_HalAdjustResourceList 0x09dbb27f
#define H_HalAllProcessorsStarted 0xe7ba2c30
#define H_HalAllocateAdapterChannel 0x359ded5e
#define H_HalAllocateCommonBuffer 0x023f7cae
#define H_HalAllocateCrashDumpRegisters 0xcdd06847
#define H_HalAllocateHardwareCounters 0xc663bfd8
#define H_HalAssignSlotResources 0x021b6879
#define H_HalBugCheckSystem 0xecaeaf34
#define H_HalCalibratePerformanceCounter 0x323ad436
#define H_HalClearSoftwareInterrupt 0x8bb1cc09
#define H_HalConvertDeviceIdtToIrql 0x1f740eb5
#define H_HalDisableInterrupt 0x497079dc
#define H_HalDisplayString 0x46d43c87
#define H_HalDmaAllocateCrashDumpRegistersEx 0xc221953d
#define H_HalDmaFreeCrashDumpRegistersEx 0xfdb81713
#define H_HalEnableInterrupt 0xb5fd297e
#define H_HalEnumerateEnvironmentVariablesEx 0x9faef1e6
#define H_HalEnumerateProcessors 0xaf892932
#define H_HalFlushCommonBuffer 0x59c8a3dc
#define H_HalFreeCommonBuffer 0xafb2feca
#define H_HalFreeHardwareCounters 0x8cbb67fe
#define H_HalGetAdapter 0xff780ef7
#define H_HalGetBusData 0xd9fd4aed
#define H_HalGetBusDataByOffset 0x8180e9c0
#define H_HalGetEnvironmentVariable 0x53304d1d
#define H_HalGetEnvironmentVariableEx 0x13477634
#define H_HalGetInterruptTargetInformation 0xc77b4915
#define H_HalGetInterruptVector 0x7d313ce1
#define H_HalGetMemoryCachingRequirements 0xe0d795b1
#define H_HalGetMessageRoutingInfo 0x232f3b4d
#define H_HalGetProcessorIdByNtNumber 0x2f7c5cbf
#define H_HalGetVectorInput 0x4023d360
#define H_HalHandleMcheck 0x5709fed1
#define H_HalHandleNMI 0x84790599
#define H_HalInitSystem 0x7422db56
#define H_HalInitializeBios 0xf7262ea3
#define H_HalInitializeOnResume 0x511509e6
#define H_HalInitializeProcessor 0xf57b826b
#define H_HalIsHyperThreadingEnabled 0x71825be0
#define H_HalMakeBeep 0xb4c2e79e
#define H_HalPerformEndOfInterrupt 0x7d66d133
#define H_HalProcessorIdle 0xa9d0336c
#define H_HalQueryDisplayParameters 0xb7fb07c5
#define H_HalQueryEnvironmentVariableInfoEx 0xaab89426
#define H_HalQueryMaximumProcessorCount 0x8d3ef6dd
#define H_HalQueryRealTimeClock 0x8c95b941
#define H_HalReadDmaCounter 0xa60ac71d
#define H_HalRegisterDynamicProcessor 0x4f7809a2
#define H_HalRegisterErrataCallbacks 0xc9b88257
#define H_HalReportResourceUsage 0xbe078260
#define H_HalRequestClockInterrupt 0x04cd9851
#define H_HalRequestDeferredRecoveryServiceInterrupt 0x34cf2ae3
#define H_HalRequestIpi 0x5ffd946f
#define H_HalRequestIpiSpecifyVector 0xca712496
#define H_HalRequestSoftwareInterrupt 0x3f10f86b
#define H_HalReturnToFirmware 0x26c720dc
#define H_HalSendNMI 0x8d4ae9c3
#define H_HalSendSoftwareInterrupt 0x75ca7458
#define H_HalSetBusData 0xd9fd4ae7
#define H_HalSetBusDataByOffset 0x8b80e9c0
#define H_HalSetDisplayParameters 0xe0fad9c2
#define H_HalSetEnvironmentVariable 0x53904d1d
#define H_HalSetEnvironmentVariableEx 0x1347761c
#define H_HalSetProfileInterval 0x84d3b444
#define H_HalSetRealTimeClock 0xfc885934
#define H_HalStartDynamicProcessor 0x06954573
#define H_HalStartNextProcessor 0x8385b926
#define H_HalStartProfileInterrupt 0x67fc0fab
#define H_HalStopProfileInterrupt 0x8d13fbde
#define H_HalSystemVectorDispatchEntry 0xcd4a6d35
#define H_HalTranslateBusAddress 0x668727ee
#define H_IoFlushAdapterBuffers 0xb10134b4
#define H_IoFreeAdapterChannel 0x9500433d
#define H_IoFreeMapRegisters 0x66e4d38f
#define H_IoMapTransfer 0x1a4d8a8c
#define H_KdComPortInUse 0x1342c9b5
#define H_KdHvComPortInUse 0x0a625e25
#define H_KeFlushWriteBuffer 0xbfe8bcaf
#define H_KeQueryPerformanceCounter 0x6ef075f4
#define H_KeStallExecutionProcessor 0xcc8aeec4
#define H_x86BiosAllocateBuffer 0x1293e7d7
#define H_x86BiosCall 0x0efdcf0f
#define H_x86BiosFreeBuffer 0x38f7a8dc
#define H_x86BiosReadMemory 0x2e9de724
#define H_x86BiosWriteMemory 0x469477e5

#define I_HalAcpiGetTableEx ImportFunc<I_MODULE_HAL, H_HalAcpiGetTableEx>
#define I_HalAcquireDisplayOwnership ImportFunc<I_MODULE_HAL, H_HalAcquireDisplayOwnership>
#define I_HalAdjustResourceList ImportFunc<I_MODULE_HAL, H_HalAdjustResourceList>
#define I_HalAllProcessorsStarted ImportFunc<I_MODULE_HAL, H_HalAllProcessorsStarted>
#define I_HalAllocateAdapterChannel ImportFunc<I_MODULE_HAL, H_HalAllocateAdapterChannel>
#define I_HalAllocateCommonBuffer ImportFunc<I_MODULE_HAL, H_HalAllocateCommonBuffer>
#define I_HalAllocateCrashDumpRegisters ImportFunc<I_MODULE_HAL, H_HalAllocateCrashDumpRegisters>
#define I_HalAllocateHardwareCounters ImportFunc<I_MODULE_HAL, H_HalAllocateHardwareCounters>
#define I_HalAssignSlotResources ImportFunc<I_MODULE_HAL, H_HalAssignSlotResources>
#define I_HalBugCheckSystem ImportFunc<I_MODULE_HAL, H_HalBugCheckSystem>
#define I_HalCalibratePerformanceCounter ImportFunc<I_MODULE_HAL, H_HalCalibratePerformanceCounter>
#define I_HalClearSoftwareInterrupt ImportFunc<I_MODULE_HAL, H_HalClearSoftwareInterrupt>
#define I_HalConvertDeviceIdtToIrql ImportFunc<I_MODULE_HAL, H_HalConvertDeviceIdtToIrql>
#define I_HalDisableInterrupt ImportFunc<I_MODULE_HAL, H_HalDisableInterrupt>
#define I_HalDisplayString ImportFunc<I_MODULE_HAL, H_HalDisplayString>
#define I_HalDmaAllocateCrashDumpRegistersEx ImportFunc<I_MODULE_HAL, H_HalDmaAllocateCrashDumpRegistersEx>
#define I_HalDmaFreeCrashDumpRegistersEx ImportFunc<I_MODULE_HAL, H_HalDmaFreeCrashDumpRegistersEx>
#define I_HalEnableInterrupt ImportFunc<I_MODULE_HAL, H_HalEnableInterrupt>
#define I_HalEnumerateEnvironmentVariablesEx ImportFunc<I_MODULE_HAL, H_HalEnumerateEnvironmentVariablesEx>
#define I_HalEnumerateProcessors ImportFunc<I_MODULE_HAL, H_HalEnumerateProcessors>
#define I_HalFlushCommonBuffer ImportFunc<I_MODULE_HAL, H_HalFlushCommonBuffer>
#define I_HalFreeCommonBuffer ImportFunc<I_MODULE_HAL, H_HalFreeCommonBuffer>
#define I_HalFreeHardwareCounters ImportFunc<I_MODULE_HAL, H_HalFreeHardwareCounters>
#define I_HalGetAdapter ImportFunc<I_MODULE_HAL, H_HalGetAdapter>
#define I_HalGetBusData ImportFunc<I_MODULE_HAL, H_HalGetBusData>
#define I_HalGetBusDataByOffset ImportFunc<I_MODULE_HAL, H_HalGetBusDataByOffset>
#define I_HalGetEnvironmentVariable ImportFunc<I_MODULE_HAL, H_HalGetEnvironmentVariable>
#define I_HalGetEnvironmentVariableEx ImportFunc<I_MODULE_HAL, H_HalGetEnvironmentVariableEx>
#define I_HalGetInterruptTargetInformation ImportFunc<I_MODULE_HAL, H_HalGetInterruptTargetInformation>
#define I_HalGetInterruptVector ImportFunc<I_MODULE_HAL, H_HalGetInterruptVector>
#define I_HalGetMemoryCachingRequirements ImportFunc<I_MODULE_HAL, H_HalGetMemoryCachingRequirements>
#define I_HalGetMessageRoutingInfo ImportFunc<I_MODULE_HAL, H_HalGetMessageRoutingInfo>
#define I_HalGetProcessorIdByNtNumber ImportFunc<I_MODULE_HAL, H_HalGetProcessorIdByNtNumber>
#define I_HalGetVectorInput ImportFunc<I_MODULE_HAL, H_HalGetVectorInput>
#define I_HalHandleMcheck ImportFunc<I_MODULE_HAL, H_HalHandleMcheck>
#define I_HalHandleNMI ImportFunc<I_MODULE_HAL, H_HalHandleNMI>
#define I_HalInitSystem ImportFunc<I_MODULE_HAL, H_HalInitSystem>
#define I_HalInitializeBios ImportFunc<I_MODULE_HAL, H_HalInitializeBios>
#define I_HalInitializeOnResume ImportFunc<I_MODULE_HAL, H_HalInitializeOnResume>
#define I_HalInitializeProcessor ImportFunc<I_MODULE_HAL, H_HalInitializeProcessor>
#define I_HalIsHyperThreadingEnabled ImportFunc<I_MODULE_HAL, H_HalIsHyperThreadingEnabled>
#define I_HalMakeBeep ImportFunc<I_MODULE_HAL, H_HalMakeBeep>
#define I_HalPerformEndOfInterrupt ImportFunc<I_MODULE_HAL, H_HalPerformEndOfInterrupt>
#define I_HalProcessorIdle ImportFunc<I_MODULE_HAL, H_HalProcessorIdle>
#define I_HalQueryDisplayParameters ImportFunc<I_MODULE_HAL, H_HalQueryDisplayParameters>
#define I_HalQueryEnvironmentVariableInfoEx ImportFunc<I_MODULE_HAL, H_HalQueryEnvironmentVariableInfoEx>
#define I_HalQueryMaximumProcessorCount ImportFunc<I_MODULE_HAL, H_HalQueryMaximumProcessorCount>
#define I_HalQueryRealTimeClock ImportFunc<I_MODULE_HAL, H_HalQueryRealTimeClock>
#define I_HalReadDmaCounter ImportFunc<I_MODULE_HAL, H_HalReadDmaCounter>
#define I_HalRegisterDynamicProcessor ImportFunc<I_MODULE_HAL, H_HalRegisterDynamicProcessor>
#define I_HalRegisterErrataCallbacks ImportFunc<I_MODULE_HAL, H_HalRegisterErrataCallbacks>
#define I_HalReportResourceUsage ImportFunc<I_MODULE_HAL, H_HalReportResourceUsage>
#define I_HalRequestClockInterrupt ImportFunc<I_MODULE_HAL, H_HalRequestClockInterrupt>
#define I_HalRequestDeferredRecoveryServiceInterrupt ImportFunc<I_MODULE_HAL, H_HalRequestDeferredRecoveryServiceInterrupt>
#define I_HalRequestIpi ImportFunc<I_MODULE_HAL, H_HalRequestIpi>
#define I_HalRequestIpiSpecifyVector ImportFunc<I_MODULE_HAL, H_HalRequestIpiSpecifyVector>
#define I_HalRequestSoftwareInterrupt ImportFunc<I_MODULE_HAL, H_HalRequestSoftwareInterrupt>
#define I_HalReturnToFirmware ImportFunc<I_MODULE_HAL, H_HalReturnToFirmware>
#define I_HalSendNMI ImportFunc<I_MODULE_HAL, H_HalSendNMI>
#define I_HalSendSoftwareInterrupt ImportFunc<I_MODULE_HAL, H_HalSendSoftwareInterrupt>
#define I_HalSetBusData ImportFunc<I_MODULE_HAL, H_HalSetBusData>
#define I_HalSetBusDataByOffset ImportFunc<I_MODULE_HAL, H_HalSetBusDataByOffset>
#define I_HalSetDisplayParameters ImportFunc<I_MODULE_HAL, H_HalSetDisplayParameters>
#define I_HalSetEnvironmentVariable ImportFunc<I_MODULE_HAL, H_HalSetEnvironmentVariable>
#define I_HalSetEnvironmentVariableEx ImportFunc<I_MODULE_HAL, H_HalSetEnvironmentVariableEx>
#define I_HalSetProfileInterval ImportFunc<I_MODULE_HAL, H_HalSetProfileInterval>
#define I_HalSetRealTimeClock ImportFunc<I_MODULE_HAL, H_HalSetRealTimeClock>
#define I_HalStartDynamicProcessor ImportFunc<I_MODULE_HAL, H_HalStartDynamicProcessor>
#define I_HalStartNextProcessor ImportFunc<I_MODULE_HAL, H_HalStartNextProcessor>
#define I_HalStartProfileInterrupt ImportFunc<I_MODULE_HAL, H_HalStartProfileInterrupt>
#define I_HalStopProfileInterrupt ImportFunc<I_MODULE_HAL, H_HalStopProfileInterrupt>
#define I_HalSystemVectorDispatchEntry ImportFunc<I_MODULE_HAL, H_HalSystemVectorDispatchEntry>
#define I_HalTranslateBusAddress ImportFunc<I_MODULE_HAL, H_HalTranslateBusAddress>
#define I_IoFlushAdapterBuffers ImportFunc<I_MODULE_HAL, H_IoFlushAdapterBuffers>
#define I_IoFreeAdapterChannel ImportFunc<I_MODULE_HAL, H_IoFreeAdapterChannel>
#define I_IoFreeMapRegisters ImportFunc<I_MODULE_HAL, H_IoFreeMapRegisters>
#define I_IoMapTransfer ImportFunc<I_MODULE_HAL, H_IoMapTransfer>
#define I_KdComPortInUse ImportFunc<I_MODULE_HAL, H_KdComPortInUse>
#define I_KdHvComPortInUse ImportFunc<I_MODULE_HAL, H_KdHvComPortInUse>
#define I_KeFlushWriteBuffer ImportFunc<I_MODULE_HAL, H_KeFlushWriteBuffer>
#define I_KeQueryPerformanceCounter ImportFunc<I_MODULE_HAL, H_KeQueryPerformanceCounter>
#define I_KeStallExecutionProcessor ImportFunc<I_MODULE_HAL, H_KeStallExecutionProcessor>
#define I_x86BiosAllocateBuffer ImportFunc<I_MODULE_HAL, H_x86BiosAllocateBuffer>
#define I_x86BiosCall ImportFunc<I_MODULE_HAL, H_x86BiosCall>
#define I_x86BiosFreeBuffer ImportFunc<I_MODULE_HAL, H_x86BiosFreeBuffer>
#define I_x86BiosReadMemory ImportFunc<I_MODULE_HAL, H_x86BiosReadMemory>
#define I_x86BiosWriteMemory ImportFunc<I_MODULE_HAL, H_x86BiosWriteMemory>
