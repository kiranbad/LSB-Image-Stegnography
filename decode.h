#ifndef DECODE_H
#define DECODE_H
#include <stdio.h>
#include <string.h>
#include "types.h"

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

/* 
 * Structure to store information required for
 * decoding secret file 
 */
typedef struct _DecodeInfo
{
    /* Output File Info */
    char *output_fname;
    FILE *fptr_output;
    char extn_output_file[MAX_FILE_SUFFIX];
    char output_data[MAX_SECRET_BUF_SIZE];
    long size_output_file;
    long extn_size;

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

} DecodeInfo;

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding*/
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointer of o/p stego image*/
Status open_decode_files(DecodeInfo *decInfo);

/* Skip 54 Bytes of header file*/
Status skip_header(DecodeInfo *decInfo);

/* Decode Magic String*/
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode a byte from LSB of image data*/
char decode_byte_to_lsb(char *image_buffer);

/* Decode extension size of secret file */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode size*/
int decode_size_to_lsb(char *image_buffer);

/* Decode secret file extension which is either .txt or .c or .sh*/
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode Secret File Data size which is present inside secret file*/
Status decode_secret_file_data_size(DecodeInfo *decInfo);

/* Decode Secret file data */
Status deocde_secret_file_data(DecodeInfo *decInfo);
#endif