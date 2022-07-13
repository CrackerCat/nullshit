#define dbg(content, ...) DbgPrintEx( 0, 0, content, __VA_ARGS__ )

typedef struct _RTL_PROCESS_MODULE_INFORMATION
{
	HANDLE section;
	PVOID mapped_base;
	PVOID image_base;
	ULONG image_size;
	ULONG flags;
	USHORT load_order_index;
	USHORT init_order_index;
	USHORT load_count;
	USHORT offset_to_file_name;
	UCHAR  full_path_name[256];
} RTL_PROCESS_MODULE_INFORMATION, *PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES
{
	ULONG number_of_modules;
	RTL_PROCESS_MODULE_INFORMATION modules[1];
} RTL_PROCESS_MODULES, *PRTL_PROCESS_MODULES;

typedef enum _SYSTEM_INFORMATION_CLASS
{
	system_basic_information,
	system_processor_information,
	system_performance_information,
	system_time_of_day_information,
	system_path_information,
	system_process_information,
	system_call_count_information,
	system_device_information,
	system_processor_performance_information,
	system_flags_information,
	system_call_time_information,
	system_module_information,
	system_locks_information,
	system_stack_trace_information,
	system_paged_pool_information,
	system_non_paged_pool_information,
	system_handle_information,
	system_object_information,
	system_page_file_information,
	system_vdm_instemul_information,
	system_vdm_bop_information,
	system_file_cache_information,
	system_pool_tag_information,
	system_interrupt_information,
	system_dpc_behavior_information,
	system_full_memory_information,
	system_load_gdi_driver_information,
	system_unload_gdi_driver_information,
	system_time_adjustment_information,
	system_summary_memory_information,
	system_next_event_id_information,
	system_event_ids_information,
	system_crash_dump_information,
	system_exception_information,
	system_crash_dump_state_information,
	system_kernel_debugger_information,
	system_context_switch_information,
	system_registry_quota_information,
	system_extend_service_table_information,
	system_priority_seperation,
	system_plug_play_bus_information,
	system_dock_information,
	system_processor_speed_information,
	system_current_time_zone_information,
	system_lookaside_information,
	system_bigpool_information = 0x42
} SYSTEM_INFORMATION_CLASS, *PSYSTEM_INFORMATION_CLASS;

typedef struct _IMAGE
{
	uintptr_t base;
	size_t size;
}IMAGE, *PIMAGE;

extern "C"
{
	__declspec( dllimport ) NTSTATUS __stdcall ZwQuerySystemInformation( SYSTEM_INFORMATION_CLASS, void *, unsigned long, unsigned long * );
}
