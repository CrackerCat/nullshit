namespace utils
{
    auto get_system_information( const SYSTEM_INFORMATION_CLASS information_class ) -> const void *
    {
        unsigned long size = 32;
        char buffer[32];

        ZwQuerySystemInformation( information_class, buffer, size, &size );

        const auto info = ExAllocatePoolZero( NonPagedPool, size, 'KLOK' );

        if ( !info )
        {
            return nullptr;
        }

        if ( ZwQuerySystemInformation( information_class, info, size, &size ) != STATUS_SUCCESS )
        {
            ExFreePool( info );
            return nullptr;
        }

        return info;
    }

    auto get_kernel_module( const char *name ) -> IMAGE
    {
        const auto to_lower = []( char *string ) -> const char *
        {
            for ( char *pointer = string; *pointer != '\0'; ++pointer )
            {
                *pointer = ( char )( short )tolower( *pointer );
            }

            return string;
        };

        const auto info = ( PRTL_PROCESS_MODULES )get_system_information( system_module_information );

        if ( !info )
        {
            return { 0, 0 };
        }

        for ( auto i = 0ull; i < info->number_of_modules; ++i )
        {
            const auto &module = info->modules[i];

            if ( strcmp( to_lower( ( char * )module.full_path_name + module.offset_to_file_name ), name ) == 0 )
            {
                ExFreePool( info );

                return { reinterpret_cast< uintptr_t > ( module.image_base ), static_cast< size_t > ( module.image_size ) };
            }
        }

        ExFreePool( info );

        return { 0, 0 };
    }
}
