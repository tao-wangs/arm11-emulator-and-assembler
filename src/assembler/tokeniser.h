#ifndef __TOKENISER_H__
#define __TOKENISER_H__

char **tok(char* instr, uint32_t ops);
void freeTok(char** tokens);

#endif
