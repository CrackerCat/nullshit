#include "imports.h"

auto walk_module_exports( IMAGE image ) -> bool
{
    if ( !image.base || image.size <= 0x1000 )
    {
        return false;
    }

    const auto dos = reinterpret_cast< IMAGE_DOS_HEADER * > ( image.base );

    if ( dos->e_magic != IMAGE_DOS_SIGNATURE )
    {
        return false;
    }

    const auto nt = reinterpret_cast< IMAGE_NT_HEADERS * >( image.base + dos->e_lfanew );

    if ( nt->Signature != IMAGE_NT_SIGNATURE )
    {
        return false;
    }

    const auto export_directory = reinterpret_cast< IMAGE_EXPORT_DIRECTORY * >( image.base + nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress );

    const auto address = reinterpret_cast< unsigned long * >( image.base + export_directory->AddressOfFunctions );

    const auto name = reinterpret_cast< unsigned long * >( image.base + export_directory->AddressOfNames );

    const auto ordinal = reinterpret_cast< unsigned short * >( image.base + export_directory->AddressOfNameOrdinals );

    for ( size_t i = 0; i < export_directory->NumberOfNames; i++ )
    {
        const auto function_address = image.base + address[ordinal[i]];

        if ( *reinterpret_cast< unsigned char * >( function_address + 0 ) == 0x48 && // mov
            *reinterpret_cast< unsigned char * >( function_address + 1 ) == 0xB8 && // rax
            *reinterpret_cast< unsigned char * >( function_address + 10 ) == 0xFF && // jmp
            *reinterpret_cast< unsigned char * >( function_address + 11 ) == 0xE0 && // rax
            ( *reinterpret_cast< uintptr_t * > ( function_address + 2 ) < image.base || // handler is less then driver start address
                *reinterpret_cast< uintptr_t * > ( function_address + 2 ) > image.base + image.size ) ) // handler is bigger then driver start address
        {
            dbg( "%s nigga whatchu thinking", reinterpret_cast< char * > ( image.base + name[i] ) );
        }
    }

    return true;
}

auto driver_entry( ) -> NTSTATUS
{
    walk_module_exports( utils::get_kernel_module( "win32kfull.sys" ) );
    walk_module_exports( utils::get_kernel_module( "win32kbase.sys" ) );
    walk_module_exports( utils::get_kernel_module( "win32k.sys" ) );
    walk_module_exports( utils::get_kernel_module( "dxgkrnl.sys" ) );
    walk_module_exports( utils::get_kernel_module( "ntoskrnl.exe" ) );

    return STATUS_SUCCESS;
}
