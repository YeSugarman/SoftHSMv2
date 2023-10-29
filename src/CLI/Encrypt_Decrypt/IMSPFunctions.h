#pragma once

class IMSPFunctions
{
public:

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

	virtual void read(int magic, int command, int dataLenght, int* data, int crc) = 0;

	virtual void write(int* magic, int* status, int* dataLenght, int* result, int* crc) = 0;
};