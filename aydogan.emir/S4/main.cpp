#include "dataset_commands.hpp"

#include <exception>
#include <iostream>

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Invalid command line\n";
    return 1;
  }

  try
  {
    aydogan::DatasetCommands processor;
    processor.loadFromFile(argv[1]);
    processor.run(std::cin, std::cout);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  catch (...)
  {
    std::cerr << "Unknown error\n";
    return 1;
  }

  return 0;
}
