#pragma once

#include <iostream>
#include <string>
namespace nsFactoryMethod {

        // define enum type for connection types
    enum class eConnectionType
    {
        CONNECTION_TYPE_ORACLE ,
        CONNECTION_TYPE_SQLSERVER,
        CONNECTION_TYPE_MYSQL,
        CONNECTION_TYPE_DEFAULT
    };
    class NativeWindow
    {   
    public:
        // define interfaces
            virtual std::string description() = 0;
            /*virtual void setDbName(std::string dbName) = 0;
            virtual void setUsername(std::string username) = 0;
            virtual void setPassword(std::string password) = 0;
            virtual bool initialize() = 0;*/
            virtual ~NativeWindow() { std::cout << "NativeWindow was been removed" << std::endl; };
            NativeWindow() { std::cout << "NativeWindow was been add" << std::endl; };
    protected:
        //NativeWindow() = default;
        NativeWindow(NativeWindow&) = default;
    };

    class OracleConnection : public NativeWindow
    {
    public:
        // implement Connection's interfaces
        virtual std::string description() override
        {
            return "OracleConnection";
        };
        OracleConnection() { std::cout << "OracleConnection was been add" << std::endl; };
        ~OracleConnection() { std::cout << "OracleConnection was been removed" << std::endl; };
    private:
        //ex:(class) LibMethodConfig* mWindow;
    };

    class SqlServerConnection : public NativeWindow
    {
    public:
        // implement Connection's interfaces
         virtual std::string description() override
         {
            return "SqlServerConnection";
         };
         SqlServerConnection() { std::cout << "SqlServerConnection was been add" << std::endl; };
         ~SqlServerConnection() { std::cout << "SqlServerConnection was been removed" << std::endl; };
    };

    class MySqlConnection : public NativeWindow
    {
    public:
        // implement Connection's interfaces
        virtual std::string description() override
        {
            return "MySqlConnection";
        };
        MySqlConnection() { std::cout << "MySqlConnection was been add" << std::endl; };
        ~MySqlConnection() { std::cout << "MySqlConnection was been removed" << std::endl; };
    };
    
    class WindowPlatform {
    public:       

        //Factory method
        static NativeWindow* createConnection(eConnectionType connectionType);
    private:
        WindowPlatform() = default;
        WindowPlatform(WindowPlatform&) = default;
        ~WindowPlatform() = default;
    };
    // Ez to Extending
    NativeWindow* WindowPlatform::createConnection(eConnectionType connectionType)
    {   
        NativeWindow* connection = nullptr;
        switch (connectionType)
        {
        case eConnectionType::CONNECTION_TYPE_ORACLE:
            connection = new OracleConnection();
            break;
        case eConnectionType::CONNECTION_TYPE_SQLSERVER:
            connection = new SqlServerConnection();
            break;
        case eConnectionType::CONNECTION_TYPE_MYSQL:
            connection = new MySqlConnection();
            break;
        default:
            connection = new OracleConnection(); // default is Oracle
            break;
        }

        return connection;//dynamic_cast<NativeWindow*>(connection);
    }
    class Client {
    public:
        NativeWindow* mConfig;
        Client(eConnectionType connection) {
            mConfig = WindowPlatform::createConnection(connection);
        }
        ~Client() {
            std::cout << "Client destructor" << std::endl;
            delete mConfig;
        }        
    private:
    };

    void programTest() {
        Client* clientProtocol = new Client(eConnectionType::CONNECTION_TYPE_SQLSERVER); 
        Client* clientProtocol2 =  clientProtocol;
        std::cout << clientProtocol->mConfig->description() << std::endl;

        clientProtocol = new Client(eConnectionType::CONNECTION_TYPE_MYSQL);
        std::cout << clientProtocol->mConfig->description() << std::endl;
        delete clientProtocol;
        
        std::cout << clientProtocol2->mConfig->description() << std::endl;
        delete clientProtocol2;
    }
}