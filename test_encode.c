/* Name : Kiran D Badiger
   Date : 11/10/2024
   Description : Main Function Defination*/

//Header File Inclusions
#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <unistd.h>

//Main function Definition
int main(int argc, char *argv[])
{
    //Check whether argument count from Command Line are less than 4
    if (argc < 4)
    {
        //Print error message and Usage message
        printf("Error : Insufficient Arguments\n");
        printf("Usage:-\nFor Encoding-> ./a.out -e <.bmp file> <secret file>\nFor Decoding-> ./a.out -d <.bmp file> <output file>\n");

        return 1;
    }
    else
    {
        //checks whether a user has choosen encode or not
        if (check_operation_type(argv) == e_encode)
        {
            printf("INFO: ## ENCODING OPERATION IS SELECTED ##\n");

            //Creation of a variable encInfo of datatype EncodeInfo
            EncodeInfo encInfo;

            //If this function returns e_success, reading and validations of arguments is done
            if (read_and_validate_encode_args(argv, &encInfo) == e_success)
            {
                printf("Read and Validation for encoding is success\n");

                //If this function return e_success, encoding procedure is done
                if (do_encoding(&encInfo) == e_success)
                {
                    printf("Encoding is Completed\n");
                }
            }
        }

        //checks whether a user has choosen decode or not
        if (check_operation_type(argv) == e_decode)
        {
            printf("INFO: ## DECODING OPERATION IS SELECTED ##\n");
            //Creation of a variable decInfo of datatype decodeInfo
            DecodeInfo decInfo;

            //If this function returns e_success, reading and validations of arguments is done
            if (read_and_validate_decode_args(argv, &decInfo) == e_success)
            {
                printf("Read and validation for decoding is success\n");

                //If this function return e_success, decoding procedure is done
                if (do_decoding(&decInfo) == e_success)
                {
                    printf("Decoding is Completed\n");
                }
            }
        }
    }
    return 0;
}