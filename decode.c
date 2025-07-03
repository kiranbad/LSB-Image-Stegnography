/* Name : Kiran D Badiger
   Date : 11/10/2024
   Description : Decoding code for stegnography */

//Header File Inclusions
#include "decode.h"
#include "common.h"
#include "types.h"
#include <unistd.h>

/* 
 * Inputs: argument vector (array of pointers), output_fname, stego_image_fname
 * Outputs: Store the names of stego.bmp and secret file names
 * Return value: e_encode, e_decode based on errors
 * Description: Read the file extension from user and validate it
*/
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (!strcmp(strstr(argv[2], "."), ".bmp"))
    {
        
        printf("INFO: %s File .bmp validation is Done\n", argv[2]);

        //If validation is true, store stego.bmp and output file name in stego_image_fname, output_fname
        decInfo -> stego_image_fname = argv[2];
        decInfo -> output_fname = argv[3];

        return e_success;
    }
    else
    {
        printf("INFO: .bmp extension is not present\n");
        return e_failure;
    }
}


/*
 * Inputs: DecodeInfo structure members (stego_image_fname)
 * Outputs: Other Function calls 
 * Return Value: e_success or e_failure
 * Description: This function is calling all the other functioins required to decode stego.bmp
*/
Status do_decoding(DecodeInfo *decInfo)
{
    printf("\nINFO: ## Decoding Procedure Started ##\n");
    if (open_decode_files(decInfo) == e_success)
    {
        printf("INFO: Opened %s\n", decInfo -> stego_image_fname);
    }
    else
    {
        printf("INFO: Opening file failed\n");
    }

    if (skip_header(decInfo) == e_success)
    {
        printf("INFO: Skipped 54 Bytes\n");
    }

    if (decode_magic_string(decInfo) == e_success)
    {
        
        printf("INFO: Decoded successfully\n");
    }
    else
    {
        printf("INFO: Decoding Failed\n");
    }

    if (decode_secret_file_extn_size(decInfo) == e_success)
    {
        
        printf("INFO: Decoded successfully\n");
    }
    else
    {
        printf("INFO: Decoding Failed\n");
    }

    if (decode_secret_file_extn(decInfo) == e_success)
    {
        
        printf("INFO: Decoded successfully\n");
    }
    else
    {
        printf("INFO: Decoding Failed\n");
    }

    if (decode_secret_file_data_size(decInfo) == e_success)
    {
        printf("INFO: Decoded successfully\n");
    }
    else
    {
        printf("INFO: Decoding Failed\n");
    }

    if (deocde_secret_file_data(decInfo) == e_success)
    {
        printf("INFO: Decoded successfully\n");
    }
    else
    {
        printf("INFO: Decoding Failed\n");
    }

    return e_success;
}

/*
 * Inputs: DecodeInfo structure (stego_image_fname, fptr_stego_image, fptr_stego_image)
 * Outputs: fptr_stego_image
 * Return Value: e_success or e_failure
 * Description: This function is used to open stego.bmp image filein read mode. Validate the file pointer
*/
Status open_decode_files(DecodeInfo *decInfo)
{
    printf("INFO: Opening required files\n");

    //fopen(pass stego.bmp file)
    decInfo -> fptr_stego_image = fopen(decInfo -> stego_image_fname, "r");

    //Error Handling
    if (decInfo -> fptr_stego_image == NULL)
    {
        printf("Error: Invalid File\n");
        return e_failure;
    }

    return e_success;
}

/*
 * Inputs: fptr_stego_image
 * Outputs: fseek()
 * Return Value: e_success or e_failure
 * Description: This function skips the first 54 Bytes of header using fseek() function
*/
Status skip_header(DecodeInfo *decInfo)
{
    printf("INFO: Skipping Header...\n");
    fseek(decInfo -> fptr_stego_image, 54, SEEK_SET);

    return e_success;
}

/*
 * Inputs: DecodeInfo structure
 * Outputs: magic_string_buffer which is #*
 * Return Value: e_success or e_failure
 * Description: This function decoded 2 Bytes of magic string and checks if it equal to #*
*/
Status decode_magic_string(DecodeInfo *decInfo)
{
    printf("INFO: Decoding Magic String Signature\n");

    //Initialising image buffer of size 8 Bytes
    char image_buffer[8];

    //This buffer is used to store magic string(#*)
    char magic_string_buffer[4];

    int i;

    //Run a loop for 2 times because there are two characters in magic string
    for (i = 0; i < 2; i++)
    {
        //Read 8 Bytes of data from fptr_stego_image
        fread(image_buffer, 1, 8, decInfo ->fptr_stego_image);

        //Decode LSB of 8 bytes and construct magic string character by character
        char ch = decode_byte_to_lsb(image_buffer);

        //Append single character into magic_string_buffer
        magic_string_buffer[i] = ch;
    }

    //Terminate magic_string_buffer with null character 
    magic_string_buffer[i] = '\0';

    //Compare with #*
    if (!strcmp(magic_string_buffer, "#*"))
    {
        return e_success;
    }

    return e_failure;
}

/*
 * Inputs: iamge_buffer
 * Outputs: Bitwise Operations
 * Return Value: e_success or e_failure
 * Description: This function initialises single character to zero. Use bitwise operations from image_buffer to get LSB of 8 bytes
*/
char decode_byte_to_lsb(char *image_buffer)
{
    //Initialise character to zero
    char ch = 0;

    //Run a loop for 8 times
    for (int i = 0; i < 8; i++)
    {
        //Use Bitwise operations
        ch = ch | ((image_buffer[i] & 0x01) << i);
    }

    return ch;
}

/*
 * Inputs: DecodeInfo structure
 * Outputs: decode_size_to_lsb()
 * Return Value: e_success or e_failure
 * Description: This function initialises image_buffer with 32 Bytes. Read 32 Bytes from fptr_stego_image file.
 * Call decode_size_to_lsb() function and store extn_size variable
*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    printf("INFO: Decoding Secret File extension Size\n");

    //Initialise image_buffer with 32 Bytes
    char image_buffer[32];

    //Read 32 Bytes of data from fptr_stego_image file
    fread(image_buffer, 1, 32, decInfo ->fptr_stego_image);

    //Store the size of secret file extension
    decInfo -> extn_size = decode_size_to_lsb(image_buffer); 
    return e_success;
}

/*
 * Inputs: image_buffer 
 * Outputs: Bitwise Operations
 * Return Value: e_success or e_failure
 * Description: This function initilises size to 0. Use bitwise operations to decode the size of secret extension 
*/
int decode_size_to_lsb(char *image_buffer)
{
    //Initialise size variable to zero
    int size = 0;
    int i;

    //Run a loop for 32 Bytes
    for (i = 0; i < 32; i++)
    {
        //Using Bitwise Operations to decode size to lsb
        size = size | ((image_buffer[i] & 0x01) << i);
    }

    return size;
}

/*
 * Inputs: DecodeInfo Structure
 * Outputs: decode_byte_to_lsb()
 * Return Value: e_success or e_failure
 * Description: This function decodes the extension of secret file which is 4 Bytes.
 *  It can be either .txt or .c or .sh
*/
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    printf("INFO: Decoding Secret File extension\n");
    
    //Initialise image_buffer with size 8
    char image_buffer[8];

    //This buffer is used to store the name of output file name
    char temporary_buffer[decInfo -> extn_size];

    int i;

    //Run a loop for extn_size times
    for (i = 0; i < decInfo -> extn_size; i++)
    {   

        //Read 8 Bytes of data from fptr_stego_image file
        fread(image_buffer, 1, 8, decInfo -> fptr_stego_image);

        //Call decode_byte_to_lsb() function
        char ch = decode_byte_to_lsb(image_buffer);

        //Append charaters of extension of secret file into buffer
        temporary_buffer[i] = ch;
    }

    //Terminate buffer with null character
    temporary_buffer[i] = '\0';

    //This array is used to concatenate the extension of output file with the name
    char output_name_with_extn[50];
    strcpy(output_name_with_extn, decInfo -> output_fname);
    strcat(output_name_with_extn, temporary_buffer);
    strcpy(decInfo->output_fname, output_name_with_extn);

    //Upon getting output.txt or .c or .sh file open it in write mode
    decInfo->fptr_output = fopen(decInfo->output_fname, "w");
    if (decInfo->fptr_output == NULL)
    {
        printf("Error: Opening File\n");
        return e_failure;
    }

    return e_success;
}

/*
 * Inputs: DecodeInfo Structure
 * Outputs: decode_size_to_lsb()
 * Return Value: e_success or e_failure
 * Description: This function initialises buffer of size 32 Bytes. Decodes secret file data size.
*/
Status decode_secret_file_data_size(DecodeInfo *decInfo)
{
    printf("INFO: Decoding Secret File Data Size\n");

    //Initializes buffer of size 32 Bytes
    char image_buffer[32];

    //Read 32 Bytes of data from fptr_stego_image.
    fread(image_buffer, 1, 32, decInfo -> fptr_stego_image);

    //Call decode_size_to_lsb() function to decode size of output file and store it in (int)size_output_file
    decInfo -> size_output_file = decode_size_to_lsb(image_buffer);

    return e_success;
}

/*
 * Inputs: DecodeInfo Structure 
 * Outputs: Decode secret file data from stego.bmp
 * Return Value: e_success or e_failure
 * Description: This function is used to decode the secret message present in the stego.bmp and 
 * append that into output.txt or .c or .sh
*/
Status deocde_secret_file_data(DecodeInfo *decInfo)
{
    printf("INFO: Decoding Secret File Data\n");

    //Initialise 8 Bytes of image buffer
    char image_buffer[8];

    //This buffer is used to store decoded data from stego image
    char temporary_buffer[decInfo -> size_output_file];
    int i;

    //Run a loop for output size file times
    for (i = 0; i < decInfo -> size_output_file; i++)
    {   
        //Read 8 Bytes of data from fptr_stego_image
        fread(image_buffer, 1, 8, decInfo -> fptr_stego_image);

        //Decode byte by byte from steog image and store it in single character ch
        char ch = decode_byte_to_lsb(image_buffer);

        //Append each decoded character into temporary buffer
        temporary_buffer[i] = ch;

        //Write single character for output file size times into output file 
        fwrite(&ch, 1, 1, decInfo -> fptr_output);
    }

    return e_success;
}
