/* Name : Kiran D Badiger
   Date : 11/10/2024
   Description : Encoding code for stegnography */


//Header File Inclusions
#include "encode.h"
#include "common.h"
#include "types.h"
#include<string.h>

/* Function Definitions are below*/

/* 
 * Inputs: argument vector (array of pointers)
 * Output: Check for encoding or decoding or unsupported options
 * Return Value: e_encode, e_decode or e_unsupported based on -e or -d
 */
OperationType check_operation_type(char *argv[])
{
    if(!strcmp(argv[1],"-e")) //Compare argv[2] is encode option?
       return e_encode;
    else if(!strcmp(argv[1],"-d")) //Compare argv[2] is decode option?
       return e_decode;
    return e_unsupported;   
}

/* 
 * Inputs: argument vector (array of pointers), src_image_fname, secret_fname, stego_image_fname
 * Outputs: Store the names of source, destination and secret file names
 * Return value: e_encode, e_decode based on errors
 * Description: Read the file extension from user and validate it
*/
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    //Compare argv[2] is .bmp extension?
    if (!strcmp(strstr(argv[2], "."), ".bmp"))
    {
        printf("INFO: %s file Validation is Done\n", argv[2]);
        encInfo->src_image_fname = argv[2];

        //Compare argv[3] is either .txt or .c or .sh extension?
        if (!strcmp(strstr(argv[3], "."), ".txt") || !strcmp(strstr(argv[3], "."), ".c") || !strcmp(strstr(argv[3], "."), ".sh")) 
        {
            printf("INFO: %s File extension Validation is Done\n", argv[3]);
            encInfo->secret_fname = argv[3];

            //Check if argv[4] exits
            if (argv[4] != NULL)
            {
                if (!strcmp(strstr(argv[4], "."), ".bmp")) //Check if argv[4] has .bmp extension
                {
                    printf("INFO: Output file extension of %s is validation Done.\n", argv[4]);
                    encInfo->stego_image_fname = argv[4];
                    return e_success;
                }
                else
                {
                    printf("INFO: Output file extension of %s is not .bmp\n", argv[4]);
                    return e_failure;
                }
            }
            else
            {
                printf("INFO: Output file not present. Creating default stego.bmp file\n");
                encInfo->stego_image_fname = "stego.bmp";
                return e_success;
            }
        }
        else
        {
            printf("INFO: Secret File is not .txt (or) .c (or) .sh extension\n");
        }
    }
    else
    {
        printf("INFO: Source File Image is not .bmp extension\n");
    }
}

/*
 * Inputs: Strcture EncodeInfo as input
 * Outputs: Printing INFO messages
 * Return Value: Returning e_success if all functions execute correct or else return e_failure
 * Description: All the functions are called here and the INFO messages are printed
*/
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo)==e_success) //Check for e_success from function()
    {
        printf("INFO: Opening required files\n");
        printf("INFO: Opened %s\nINFO: Opened %s\nINFO: Done\n", encInfo -> src_image_fname, encInfo -> secret_fname);

        //Check for e_success from function()
        if(check_capacity(encInfo)==e_success)
        {
            printf("\n## Encoding Procedure Started ##\n");
            printf("INFO: Checking for %s size\n", encInfo -> secret_fname);
            printf("INFO: Done. Found OK\n");
        }
        else
        {
            printf("INFO: Check capacity is not Done:\n");
            return e_failure;
        }

        encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

        printf("INFO: Checking for %s capacity to handle %s\n", encInfo -> src_image_fname, encInfo -> secret_fname);
        printf("INFO: Image capacity size is: %d bytes\n", encInfo -> image_capacity);
        printf("INFO: Encoding data size is: %ld bytes\n", encInfo -> size_secret_file);
        printf("INFO: Done. Founded Greater Image Size\n");
        
        //Check for e_success from function()
        if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
        {
            printf("INFO: Opening output file\nINFO: Opened %s\nINFO: Copying Image Header\n", encInfo -> stego_image_fname);
            printf("INFO: Copied Successfully\n");
        }
        else
        { 
            printf("INFO: Copying is Not done\n");
        }
    }
    else
    {
        printf("INFO: Opening file failed\n");
    }
   
    //Check for e_success from function()
    if (encode_magic_string(MAGIC_STRING,encInfo) == e_success)
    {
        printf("INFO: Encoded Successfully\n");
    }
    else
    {
        printf("INFO: Encode Failed\n");
    }

    //Check for e_success from function()
    if (encode_secret_file_extn_size(encInfo->secret_extrn_size,encInfo) == e_success)
    {
        printf("INFO: Encoding %s File Extension Size\n", encInfo -> secret_fname);
        printf("INFO: Encoded Successfully\n");
    }
    else
    {
        printf("INFO: Encode Failed\n");
    }

    //Check for e_success from function()
    if (encode_secret_file_extn(encInfo->extn_secret_file,encInfo) == e_success)
    {
        printf("INFO: Encoding %s File Extension\n", encInfo -> secret_fname);
        printf("INFO: Encoded Successfully\n");
    }
    else
    {
        printf("INFO: Encode Failed\n");
    }

    //Check for e_success from function()
    if (encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_success)
    {
        printf("INFO: Encoding %s File Size\n", encInfo -> secret_fname);
        printf("INFO: Encoded Successfully\n");
    }
    else
    {
        printf("INFO: Encode Failed\n");
    }

    //Check for e_success from function()
    if (encode_secret_file_data(encInfo) == e_success)
    {
        printf("INFO: Encoding %s File\n", encInfo -> secret_fname);
        printf("INFO: Encoded Successfully\n");
    }
    else
    {
        printf("INFO: Encode Failed\n");
    }
    
    //This function call is used to copy remaining image data
    copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}

/*
 * Inputs: Strcture EncodeInfo as input 
 * Outputs: Storing the image size, file size
 * Return Value: e_success or e_failure
 * Description: This function calculates image size, file size, checks if enough capacity is present in image for encoding process
*/
Status check_capacity(EncodeInfo *encInfo)
{
    //Calls get_image_size_for_bmp() function and stored in image_capacity
    encInfo -> image_capacity=get_image_size_for_bmp(encInfo -> fptr_src_image);

    //Calls get_file_size() function to store the size of secret file
    encInfo -> size_secret_file=get_file_size(encInfo -> fptr_secret);

    //Copy the extension of secret file into encInfo -> extn_secret_file
    strcpy(encInfo -> extn_secret_file,strstr(encInfo->secret_fname,"."));

    //Calculate the length of entesion of secret file and store it in encInfo->secret_extrn_size
    encInfo->secret_extrn_size = strlen(strstr(encInfo->secret_fname,"."));

    //Check whether there is enough space in the Source Image to encode the bytes of secret file
    if(encInfo->image_capacity > 54 + ((2 + 4 + 4 + encInfo->secret_extrn_size + encInfo->size_secret_file)*8))
    {
        return e_success;
    }

    return e_failure;
}

/*
 * Inputs: File Pointer
 * Outputs: Setting file Pointer to the end of File
 * Return Value: size of file
 * Description: This function calculates the size of Secret File
*/
uint get_file_size(FILE *fptr)
{
    //Jump to the end of the file 
    fseek(fptr,0,SEEK_END);

    //ftell() function tells the current position of the file pointer
    return ftell(fptr);
}

/*
 * Inputs: fptr_src_image, fptr_dest_image
 * Outputs: Setting fptr_src_image to Start, Reading 54 Bytes
 * Return Value: e_success or e_failure
 * Description: This function sets fptr_src_image to beginning. Reads 54 Bytes(BMP Header) from Image and stores in stego.bmp
*/
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    //Initialise a buffer to store the header of .bmp file
    char bmp_buffer[54];

    //Jump to the beginning of the source image file
    fseek(fptr_src_image,0,SEEK_SET);

    //Read 54 Bytes of data from source image file
    fread(bmp_buffer,1,54,fptr_src_image);

    //Writes 54 Bytesof data to stego image file
    fwrite(bmp_buffer,1,54,fptr_dest_image);

    if(54 == get_file_size(fptr_dest_image))
    {
        return e_success;
    }
    
    else
    {
        return e_failure;
    }
}

/*
 * Inputs: magic_string, EncodeInfo Structure members
 * Outputs: Function call to encode_data_to_image
 * Return Value: e_success or e_failure
 * Description: This function encodes magic string which is a macro defined in common.h->(#*)
*/
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    //Function calls done to encode data to iamge
    encode_data_to_image(magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image);
    printf("INFO: Encoding Magic String\n");
    return e_success;
}

/*
 * Inputs: data, size, fptr_src_image, fptr_stego_image
 * Outputs: Reads and Writes 8 bytes and calls encode_byte_to_lsb() function
 * Return Value: e_success or e_failure
 * Description: This function initializes image_buffer. Reads 8 Bytes from original and writes in output file. 
*/
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    //Initialise buffer of size 8 Bytes
    char image_buffer[8];

    //Run a loop for size times
    for(int i=0; i<size; i++)
    {
        fread(image_buffer,8,1,fptr_src_image);//Reading 8 bytes from original file.
        encode_byte_to_lsb(data[i],image_buffer);
        fwrite(image_buffer,8,1,fptr_stego_image);//Writing encoded data in output file.
    }
}

/*
 * Inputs: data, character pointer image_buffer
 * Outputs: Encodes data in LSB
 * Return Value: e_success or e_failure
 * Description: This function runs a loop for 8 times for 8 Bytes does following operation
*/
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    //Run a loop for 8 times
    for(int i=0; i<8; i++)
    {
        //Perform bitwise operations on data and store in image buffer
        image_buffer[i] = ((image_buffer[i] & 0xFE) | ((data >> i) & 0x01));
    }
    return e_success;
}

/*
 * Inputs: file_extn, EncodeInfo Structure
 * Outputs: Calls encode_data_to_image() function
 * Return Value: e_success or e_failure
 * Description: This function encodes secret file extension which is either .txt or .c or .sh
*/
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    //Call this function to encode secret file extension
    encode_data_to_image((char *)file_extn,strlen(file_extn),encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}

/*
 * Inputs: file_size, EncodeInfo Structure
 * Outputs: Reads 32 Bytes and Writes 32 Bytes
 * Return Value: e_success or e_failure
 * Description: This function reads 32 bytes from source image and writes into LSB of 32 Bytes of Stego Image
*/
Status encode_secret_file_size(int file_size,EncodeInfo *encInfo)
{
    //Initialise 32 Bytes buffer array
    char ext_buffer[32];

    //Read 32 Bytes of data from source image
    fread(ext_buffer,1,32,encInfo->fptr_src_image);

    //Run a loop for 32 times
    for(int i=0; i<32; i++)
    {
        //Do bitwise operations on size of file
        ext_buffer[i]=(ext_buffer[i]&0xFE | ((file_size & (1<<i))>>i));
    }

    //Write 32 Bytes of data into stego image
    fwrite(ext_buffer,1,32,encInfo->fptr_stego_image);
    return e_success;
}

/*
 * Inputs: EncodeInfo Structure
 * Outputs: Reading size_Secret_file times of data from secret file and calls encode_data_to_image() function
 * Return Value: e_success or e_failure buffer array. Rewinds secret file pointer to set at beginning. 
 * Calls encode_data_to_image() function
 * Description: This function initialises 
*/
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    //Initislise buffer of size of secret file
    char buffer[encInfo->size_secret_file];

    //Rewind to the beginning of the file
    rewind(encInfo -> fptr_secret);

    //Read secret file size times from secret file
    fread(buffer,encInfo->size_secret_file,1,encInfo->fptr_secret);

    //Call this function to encode data of secret file
    encode_data_to_image(buffer,encInfo->size_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}

/*
 * Inputs: extn_size, EncodeInfo Structure
 * Outputs: Reads, writes 32 Bytes of data
 * Return Value: e_success or e_failure
 * Description: This function initialises buffer array of size 32 Bytes which is used to store 32 Bytes of data from source image 
 * which contains the information about file extension size
*/
Status encode_secret_file_extn_size(long extn_size, EncodeInfo *encInfo)
{
    //Initialise buffer of size 32 Bytes
    char buffer[32];

    //Read 32 Bytes of data from source image file
    fread(buffer,1,32,encInfo->fptr_src_image);

    //Calls this function to encode extension of secret file size
    encode_size_to_lsb(extn_size,buffer);

    //Write 32 Bytes of data into stego image file
    fwrite(buffer,1,32,encInfo->fptr_stego_image);
    return e_success;
}

/*
 * Inputs: size, buffer
 * Outputs: Encode
 * Return Value: e_success or e_failure
 * Description: Run a loop for 32 times to encode 4 Bytes of size into LSB
*/
Status encode_size_to_lsb(int size,char *buffer)
{
    //Run a loop for 32 times
    for(int i = 0; i < 32; i++)
    {
        //Perform bitwise operations on size to encode and place into the LSB
      buffer[i] = (buffer[i] & 0xFE) | ((size >> i) & 0x01);  
    }
    return e_success;
}

/*
 * Inputs: size, EncodeInfo Structure
 * Outputs: Reads, writes 32 Bytes of data
 * Return Value: e_success or e_failure
 * Description: This function initialises 32 Bytes of data. Reads 32 Bytes of data from source image, 
 * encodes it and writes into stego image
*/
Status encode_secret_file_data_size(int size,EncodeInfo *encInfo)
{
    //Initialise buffer of size 32 Bytes
    char buffer[32];

    //Read 32 Bytes of data from source image file
    fread(buffer,1,32,encInfo->fptr_src_image);

    //Calls this function to encode secret file size into LSB
    encode_size_to_lsb(size,buffer);

    //Writes 32 Bytes of data into stego image file
    fwrite(buffer,1,32,encInfo->fptr_stego_image);
}

/*
 * Inputs: fptr_src, fptr_dest
 * Outputs: Copy remaining image data
 * Return Value: e_success or e_failure
 * Description: This function copies the remaining amount of data until the End of File
*/
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    //Initialise a integer named byte
    int byte;

    //Run a loop until End of file
    while((byte=fgetc(fptr_src))!=EOF)
    {
        //put individual character into stego image file until EOF
        if(fputc(byte,fptr_dest)==EOF)
        {
            return e_failure;
        }
    }
    return e_success;

}

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    //Initialise height and width variables of type unsigned int
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}