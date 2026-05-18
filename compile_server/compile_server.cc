#include "compiler.hpp"
#include "runner.hpp"
int main()
{
    Compiler::Compile("hello");
    Runner::Run("hello", 1, 40 * 1024);
}
