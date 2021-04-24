//////////////////////////////////////////////////////////////////////////
///
/// Copyright (c) 2020 Prof. AJ Bieszczad. All rights reserved.
///
//////////////////////////////////////////////////////////////////////////

#include "disk.h"

disk_t disk;

/***
 *
 * Verifies correctness of the logical block address, and then translates it to a cylinder-head-sector equivalent.
 *
 */
CIDEV_RET_CODE lba2chs(lba_t lba, chs_t *chs)
{
    if (lba >= MAX_LOGICAL_BLOCK)
        return CIDEV_ADDRESS_ERROR;

// todo: implement
    //translates to cylinders
    chs->cyl = (lba/NUM_OF_SECTS)%NUM_OF_CYLS;
    //translates to Head
    chs->head = lba/(NUM_OF_SECTS*NUM_OF_CYLS);
    //translates to section
    chs->sect = lba % NUM_OF_SECTS;

    return CIDEV_SUCCESS;
}

/***
 *
 * Verifies correctness of the cylinder-head-sector address, and then translates it to a logical block address.
 *
 */
CIDEV_RET_CODE chs2lba(chs_t *chs, lba_t *lba)
{
// todo: implement
    long c = (NUM_OF_SECTS*chs->cyl);

    long h = ((NUM_OF_CYLS*NUM_OF_SECTS)* chs->head);


    long s = chs->sect;


    *lba =  c + h + s;

    if (*lba >= MAX_LOGICAL_BLOCK)
        return CIDEV_ADDRESS_ERROR;

    return CIDEV_SUCCESS;
}
/***
 *
 * Verifies the parameters, then allocates space for the requested data (using
 * the caller-provided pointer buffer), copies the data from the disk to the buffer,
 * and appends '\0' to terminate the string.
 *
 */
CIDEV_RET_CODE readDisk(lba_t lba, unsigned int size, char **buffer)
{
// todo: verify parameters
    if (lba >= MAX_LOGICAL_BLOCK)
        return CIDEV_ADDRESS_ERROR;

    chs_t chs;

    *buffer = malloc((size+1)*sizeof(char)); // todo: modify as required
    (*buffer)[0] = '\0';
    CIDEV_RET_CODE errCode = CIDEV_SUCCESS;

    // todo: implement
    int num_of_chars = 0;
    int loop = 0;
    while (num_of_chars < size){
        lba2chs(lba + loop, &chs);
        for (int i = 0 ; i < SECT_SIZE && num_of_chars < size; i++){
            num_of_chars++;
            char currentChar;
//            char currentChar =  disk[chs.cyl][chs.head][chs.sect][i];
            memcpy(&currentChar,&disk[chs.cyl][chs.head][chs.sect][i],1);
            strncat(*buffer,&currentChar,1);
        }
        loop++;
    }

    return errCode;
}

/***
 *
 * An auxiliary function to fill a single disk block with '.'
 *
*/
CIDEV_RET_CODE clearBlock(lba_t lba)
{
    char *writeBuffer;
#ifdef __DEBUG_DISK
    writeBuffer = malloc(SECT_SIZE);
#else
    writeBuffer = kmalloc(SECT_SIZE, GFP_USER);
#endif
    if (writeBuffer == NULL)
        return CIDEV_SPACE_ERROR;

    for (int i = 0; i < SECT_SIZE; i++)
        writeBuffer[i] = '.';

    CIDEV_RET_CODE errCode = writeDisk(lba, writeBuffer);

#ifdef __DEBUG_DISK
    free(writeBuffer);
#else
    kfree(writeBuffer);
#endif

    return errCode;
}

/***
 *
 * Validates the parameters, and then writes the caller-provided data to the disk starting at the block pointed
 * to by the logical block address.
 *
 */
CIDEV_RET_CODE writeDisk(lba_t lba, char *buffer)
{
// todo: verify the parameters

    if (lba >= MAX_LOGICAL_BLOCK)
        return CIDEV_ADDRESS_ERROR;
    int size = strlen(buffer);
    if ( ((size/SECT_SIZE)+1) + lba >= MAX_LOGICAL_BLOCK){
        return CIDEV_ADDRESS_ERROR;
    }
    CIDEV_RET_CODE errCode = CIDEV_SUCCESS;

    chs_t chs;

// todo: implement

    lba2chs(lba,&chs);
    int loop = 0;
    int num_of_chars = 0;
    while (num_of_chars != strlen(buffer)){
        lba2chs(lba+loop,&chs);
        for (int i = 0; i < SECT_SIZE && num_of_chars != strlen(buffer); ++i) {
            num_of_chars++;
            memcpy(&disk[chs.cyl][chs.head][chs.sect][i],&buffer[i+(loop*SECT_SIZE)],1);
//            disk[chs.cyl][chs.head][chs.sect][i] = buffer[i+(loop*SECT_SIZE)];
        }
        loop++;
    }

    return errCode;
}
