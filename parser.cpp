#include <iostream>
#include "TreeCSV.h"

int
main (int argc, char *argv[])
{
        TreeCSV treeCSV;
        if (argc == 1) 
                treeCSV = TreeCSV("source-file.csv");
        else
                treeCSV = TreeCSV(argv[1]);

        treeCSV.printHeader();

        return 0;
}
