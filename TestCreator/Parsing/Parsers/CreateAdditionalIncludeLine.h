#ifndef TESTCREATOR_PARSERS_CREATEADDITIONALINCLUDELINE_H
#define TESTCREATOR_PARSERS_CREATEADDITIONALINCLUDELINE_H

#include <string>
#include <vector>

auto CreateAdditionalIncludeLine(std::vector<std::string>::iterator& FirstToken)
    -> std::string;

#endif /* TESTCREATOR_PARSERS_CREATEADDITIONALINCLUDELINE_H */
