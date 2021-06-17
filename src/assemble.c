#include <stdlib.h>
#include "utility.h"
#include "hash.h"
#include "firstpass.h"
#include "secondpass.h"

int main(int argc, char **argv) {

    long int max_labels = fSize(argv[1]);
    hashTable *labels = createHashTable(max_labels);
    uint32_t last_addr = firstPass(labels, argv[1]);
    
    secondPass(labels, argv[1], argv[2], last_addr);

        
    return EXIT_SUCCESS;
}
