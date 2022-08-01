#include "postgresql.hpp"

#ifdef POSTGRESQL

//--------------------------------------------------------------------------------
//					    SQLLITE INTERFACE  IMPLEMENTATION
//--------------------------------------------------------------------------------

Posdtgres::Posdtgres
(std::string str) : mConnexion
  {"                        \
    dbname = test_sys       \
    user = sys_user         \
    password = 123321       \
    hostaddr = 127.0.0.1    \
    port = 5432             \
  "}
{
    if (mConnexion.is_open()) 
    {
        //std::cout << "Opened database successfully: " << mConnexion.dbname() << std::endl;
        WRITE_LOG("Opened_database_successfully");
    } 
    else 
    {
        WRITE_LOG("Can't_open_database");
        //std::cout << "Can't open database" << std::endl;
    }
}

void 
Posdtgres::select
(
    std::string aTableName,
    std::string aColum,
    std::string aConditon,
    int aStatementID
)

{
    if (aColum == "") aColum = "*";
    if (aConditon != "") aConditon = " WHERE " + aConditon;
    std::string statement = "SELECT " + aColum + " FROM " + aTableName + aConditon;
    
    prepare(statement, aStatementID);
}

void 
Posdtgres::update
(
    std::string aTableName,
    std::string aValue,
    std::string aConditon,
    int aStatementID
)
{
    std::string statement = "UPDATE " + aTableName + " SET " + aValue + " WHERE " + aConditon;
    prepare(statement, aStatementID);
}

const unsigned char* 
Posdtgres::getTextFromRow(int aColumNumber, int aStatementID)
{
    auto str = mResultIterator[aStatementID][aColumNumber].as<const char*>();
    return (const unsigned char* ) str;
}



const char* 
Posdtgres::getText16FromRow(int aColumNumber, int aStatementID)
{
    auto str = mResultIterator[aStatementID][aColumNumber].as<std::string>();
    return str.c_str();
}

int 
Posdtgres::getIntFromRow(int aColumNumber, int aStatementID)
{
    return mResultIterator[aStatementID][aColumNumber].as<int>();
}

int64_t 
Posdtgres::getInt64FromRow(int aColumNumber, int aStatementID)
{
        return mResultIterator[aStatementID][aColumNumber].as<int64_t>();
}

void 
Posdtgres::closeStatment(int aStatementID)
{
    if (mStatement.size() <= aStatementID) return;
    mStatement[aStatementID]->commit();
    mStatement[aStatementID] = nullptr;
    while (mStatement.size() >= 1 && mStatement[aStatementID] == nullptr) 
    {
        mStatement.pop_back();
        mResult.pop_back();
        mResultIterator.pop_back();
    }
}

int 
Posdtgres::step(int aStatementID)
{
    mResultIterator[aStatementID]++;
    return 0;
}

char* 
Posdtgres::toAscii(const unsigned char* s)
{
    //TODO: use my defines
    int cnt = 0;
    while (s[cnt++]);
    char* result = (char*) malloc(sizeof(char) * (cnt));
    result[cnt - 1] = 0;
    for (int i = 0; s[i];) result[i] = s[i++];
    return result;
    //TODO end
}

void 
Posdtgres::prepare(std::string aStatment, int aStatementID)
{
    if (mStatement.size() < aStatementID + 1)
    {
        mStatement.resize(aStatementID + 1);
        mResult.resize(aStatementID + 1);
        mResultIterator.resize(aStatementID + 1);
    }

    try
    {
            mStatement[aStatementID] = std::make_unique<pqxx::work>(mConnexion);
            mResult[aStatementID] = pqxx::result( mStatement[aStatementID]->exec(aStatment.c_str()));
            mResultIterator[aStatementID] = --mResult[aStatementID].begin();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        std::cout << e.what() << '\n';
        WRITE_ERROR(e.what());
    }
}

#endif // !POSTGRESQL