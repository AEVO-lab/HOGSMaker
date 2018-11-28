//
// Created by Manuel on 22/11/2018.
//

#ifndef HOGSMAKER_OMAPARSER_H
#define HOGSMAKER_OMAPARSER_H

#include "TreeSource/div/tinydir.h"
#include "TreeSource/div/util.h"
#include <string>

using namespace std;

class OMAParser {

    void FindConnectedComponents(double score_threshold);

    void ReadSpeciesDir(string dirname);
};


#endif //HOGSMAKER_OMAPARSER_H
