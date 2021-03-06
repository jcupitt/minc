#include  <volume_io.h>

int  main(
    int   argc,
    char  *argv[] )
{
    Volume     volume;
    Status     status;
    int        x, y, z, sizes[N_DIMENSIONS];
    char       *input_filename, *output_filename, *history;
    double     min_value, max_value, value, new_value, new_voxel;
    BOOLEAN    thresholding;

    if( argc < 3 )
    {
        print( "Usage: %s  input_volume output_volume  [min max new_value]\n",
               argv[0] );
        return( 1 );
    }

    input_filename = argv[1];
    output_filename = argv[2];

    if( argc == 6 )
    {
        (void) sscanf( argv[3], "%lf", &min_value );
        (void) sscanf( argv[4], "%lf", &max_value );
        (void) sscanf( argv[5], "%lf", &new_value );
        thresholding = TRUE;
    }
    else
        thresholding = FALSE;

    status = input_volume( input_filename, 3, File_order_dimension_names,
                      NC_UNSPECIFIED, FALSE, 0.0, 0.0,
                      TRUE, &volume, (minc_input_options *) NULL ) ;

    if( status != OK )
        return( 1 );

    /* --- convert new_value to voxel */

    new_voxel = CONVERT_VALUE_TO_VOXEL( volume, new_value );

    get_volume_sizes( volume, sizes );

    if( thresholding )
    {
        for_less( x, 0, sizes[X] )
        {
            for_less( y, 0, sizes[Y] )
            {
                for_less( z, 0, sizes[Z] )
                {
                    GET_VALUE_3D( value, volume, x, y, z );
                    if( value < min_value || value > max_value )
                    {
                        SET_VOXEL_3D( volume, x, y, z, new_voxel );
                    }
                }
            }
        }
    }

    history = "Converted volume to byte";

    status = output_volume( output_filename, NC_BYTE, FALSE, 0.0, 255.0,
                            volume, history, (minc_output_options *) NULL );

    return( 0 );
}
