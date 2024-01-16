#include "Logging.h"

void Log(std::ostream& Output, std::string Level, std::string Message) {
    Output << "<";
    Output << Level;
    Output << ">";
    Output << " ";
    Output << Message;
    Output << std::endl;
}