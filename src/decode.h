#ifndef DECODE_H
#define DECODE_H

enum INSTRUCTION_TYPE {DataProcessing, Multiply, SingleDataTransfer, Branch};
enum INSTRUCTION_TYPE decode(unsigned int instruction);

#endif
