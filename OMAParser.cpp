//
// Created by Manuel on 22/11/2018.
//

#include "OMAParser.h"

void OMAParser::FindConnectedComponents(double score_threshold)
{
    tinydir_dir dir;
    tinydir_open(&dir, "./AllAll/");

    while (dir.has_next)
    {
        tinydir_file file;
        tinydir_readfile(&dir, &file);

        string name = string(file.name);
        if (file.is_dir)
        {
            printf("/");
        }
        printf("\n");

        tinydir_next(&dir);
    }

    tinydir_close(&dir);
}


void OMAParser::ReadSpeciesDir(string dirname)
{
    tinydir_dir dir;
    tinydir_open(&dir, dirname.c_str());

    while (dir.has_next)
    {
        tinydir_file file;
        tinydir_readfile(&dir, &file);

        string name = string(file.name);
        if (Util::GetFileExtension(name) == "gz")
        {
            //unzip
        }
        printf("\n");

        tinydir_next(&dir);
    }

    tinydir_close(&dir);
}