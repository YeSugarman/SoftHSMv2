#pragma once
#include <cstdint>
#include <string>
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

    virtual uint8_t* Get_Kek_By_Info(long userId, int info_len, std::string kekInfo) = 0;
    virtual uint8_t* Encrypt_data_key(long userId, uint8_t kekId, uint8_t key_length, uint8_t* arrRandom) = 0;
    virtual uint8_t* Decrypt_data_key(long userId, uint8_t kekId, uint8_t encryptedKeyLength, uint8_t* encryptedKey) = 0;

	//virtual uint8_t* read(int magic, int command, int dataLength, int* data, int crc) = 0;
	//virtual void write(int* magic, int* status, int* dataLength, int* result, int* crc) = 0;

};