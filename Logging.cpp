#include "Logging.h"

void Log(std::ostream& Output, const std::string& Level,
         const std::string& Message) {
  Output << "<";
  Output << Level;
  Output << ">";
  Output << " ";
  Output << Message;
  Output << std::endl;
}
