#include <iostream>
//
#include <OptFrame/Scanner++/Scanner.hpp>
#include <OptFrame/Util/Matrix.hpp>
#include <OptFrame/printable/printable.hpp>

using namespace optframe;
using namespace scannerpp;

int
main()
{
   Matrix<double> m(1);         //1x1
   std::cout << m << std::endl; // printable
   //
   Scanner scanner("Hello World");
   std::cout << scanner.next() << std::endl; // Hello
   std::cout << scanner.next() << std::endl; // World

   return 0;
}