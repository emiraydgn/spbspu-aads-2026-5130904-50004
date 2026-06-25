#include <exception>
#include <iostream>
#include <stdexcept>

#include "logic.hpp"

int main()
{
  aydogan::SequenceList data;

  try
  {
    aydogan::readInput(std::cin, data);

    if (data.empty())
    {
      std::cout << "0\n";
      return 0;
    }

    aydogan::printNames(data, std::cout);
    return aydogan::printRowsAndSums(data, std::cout, std::cerr);
  }
  catch (const std::overflow_error& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 2;
  }
  catch (...)
  {
    std::cerr << "Unknown error\n";
    return 2;
  }
}
