#include "imports.h"

auto walk_module_exports( const uintptr_t image_base, const size_t image_size, void ( *callback )( const char *, uintptr_t, size_t, uintptr_t ) ) -> bool
{
    const auto dos = reinterpret_cast< IMAGE_DOS_HEADER * > ( image_base );

    const auto nt = reinterpret_cast< IMAGE_NT_HEADERS * >( image_base + dos->e_lfanew );

    const auto export_directory = reinterpret_cast< IMAGE_EXPORT_DIRECTORY * >( image_base + nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress );

    const auto address = reinterpret_cast< unsigned long * >( image_base + export_directory->AddressOfFunctions );

    const auto name = reinterpret_cast< unsigned long * >( image_base + export_directory->AddressOfNames );

    const auto ordinal = reinterpret_cast< unsigned short * >( image_base + export_directory->AddressOfNameOrdinals );

    for ( size_t i = 0; i < export_directory->NumberOfNames; i++ )
    {
        callback( reinterpret_cast< char *> ( image_base + name[i] ), image_base + address[ordinal[i]], image_base, image_size );
    }

    return true;
}

auto driver_entry( ) -> NTSTATUS
{
    auto callback = []( const char *name, uintptr_t function_address, uintptr_t image_base, size_t image_size ) -> void
    {
        if (
            *reinterpret_cast< unsigned char * >( function_address + 0 ) == 0x48 && // mov
                *reinterpret_cast< unsigned char * >( function_address + 1 ) == 0xB8 && // rax
                *reinterpret_cast< unsigned char * >( function_address + 10 ) == 0xFF && // jmp
                *reinterpret_cast< unsigned char * >( function_address + 11 ) == 0xE0 && // rax
            ( *reinterpret_cast< uintptr_t * > ( function_address + 2 ) < image_base || // handler is less then driver start address
            *reinterpret_cast< uintptr_t * > ( function_address + 2 ) > image_base + image_size ) // handler is bigger then driver start address
            )
        {
            dbg( "%s nigga whatchu thinking", name );
        }
    };

    uintptr_t image_base, image_size;
    utils::get_kernel_module( "dxgkrnl.sys", &image_base, &image_size );

    walk_module_exports( image_base, image_size, callback );

    return STATUS_SUCCESS;
}
