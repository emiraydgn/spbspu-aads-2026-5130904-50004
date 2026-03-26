#include "logic.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
  try
  {
    if (argc > 2)
    {
      std::cerr << "Invalid command line\n";
      return 1;
    }

    if (argc == 2)
    {
      std::ifstream input(argv[1]);
      if (!input)
      {
        std::cerr << "Cannot open file\n";
        return 1;
      }
      return aydogan::run(input, std::cout, std::cerr);
    }

    return aydogan::run(std::cin, std::cout, std::cerr);
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
}
