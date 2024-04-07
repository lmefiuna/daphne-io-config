#ifndef _CONFIG_FILE_NOT_FOUND_EXCEPTION_H_
#define _CONFIG_FILE_NOT_FOUND_EXCEPTION_H_

#include <exception>
#include <sstream>
#include <string>

namespace DAPHNE
{
namespace ConfigReader
{
class ErrorOpeningFileException : public std::exception
{
 private:
  std::string errorMessage;

 public:
  ErrorOpeningFileException(const std::string &filePath)
  {
    std::ostringstream ss;
    ss << "Error in opening file " << filePath << std::endl;
    this->errorMessage = ss.str();
  }

  const char *what() const noexcept override
  {
    return this->errorMessage.c_str();
  }
};

class MissingPropertyException : public std::exception
{
 private:
  std::string errorMessage;

 public:
  MissingPropertyException(const std::string &propertyName)
  {
    std::ostringstream ss;
    ss << "Missing property " << propertyName << std::endl;
    this->errorMessage = ss.str();
  }

  const char *what() const noexcept override
  {
    return this->errorMessage.c_str();
  }
};
}  // namespace ConfigReader
}  // namespace DAPHNE
#endif