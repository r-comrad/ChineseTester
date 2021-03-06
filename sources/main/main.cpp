#include <string>

#include "tester/core.hpp"
#include "domain/error_message.hpp"

#include "path.hpp"

//--------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    int num = 0;
    std::string databasePath = "";

    // For debug
    #if     defined(SQLITE)
    num = 3;
    databasePath = DATABASE_PATH;
    #elif   defined(POSTGRESQL)
    num = 2;
    #endif

    if (argc > 1)
    {
        num = std::stoi(std::string(argv[1]));
    }

    WRITE_LOG("ID:", num);
    WRITE_LOG_ENDL();


    test::Core core(databasePath);

    core.run(num);
    
    return 0;
}

//--------------------------------------------------------------------------------
