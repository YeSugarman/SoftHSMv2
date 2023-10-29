#pragma once
#include <cstdint>
#define MAGIC 35

class IMSPFunctions
{
public:

    //typedef enum {
    //    Delete_All = 102,
    //    Delete_Partition,
    //    Get_Partition_Info,
    //    def_Get_Kek_By_Info,
    //    Write_Kek,
    //    def_Encrypt_data_key,
    //    def_Decrypt_data_key
    //}Commands;

    typedef enum {
        CRC_EXCEPTION,
        NOT_EXIST_COMMAND,
        PARAMETERS_NOT_MATCH,
        NOT_EXIST_KEK,
        NOT_EXIST_PARTINION,
        COMMAND_TIME_OUT,
        NO_ADDRESS_FOR_KEK
    }Exceptions;

    typedef enum {
        EXCEPTION = 64,
        SUCCESS = 36
    }STATUS;

    virtual uint8_t* Get_Kek_By_Info(int dataLenght, int* data, int crc) = 0;
    virtual uint8_t* Encrypt_data_key(int dataLenght, int* data, int crc) = 0;
    virtual uint8_t* Decrypt_data_key(int dataLenght, int* data, int crc) = 0;

	//virtual uint8_t* read(int magic, int command, int dataLenght, int* data, int crc) = 0;
	//virtual void write(int* magic, int* status, int* dataLenght, int* result, int* crc) = 0;

};