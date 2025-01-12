#pragma once

#include <iostream>
#include <string>
#include <mutex>
#define SYNC_THREAD_BY_MUXTEX
//#define MAKE_STATIC_OBJECT

namespace nsSingleton {
    class Database
    {
    private:
        int mRecord;
        std::string mName;

        // Pointer help to create new this->class because contractor side in private level
        static Database* mInstancePtr;

#ifdef SYNC_THREAD_BY_MUXTEX
        // Sync clock thread when check mInstance variable
        static std::mutex mLocker;
#endif // SYNC_THREAD_BY_MUTEX        

        // Contractor set in private level to prevent creating new class
        Database(std::string name)
        {
            mName = name;
            mRecord = 0;
        }

        // Contractor set in private level to prevent delete method
        ~Database()
        {
        }
    public:

        void editRecord(std::string operation)
        {
            std::cout << "Performing a " << operation <<
                "operation on record " << mRecord <<
                " in database " << mName << std::endl;
        }

        std::string getName()
        {
            return mName;
        }

        #ifdef SYNC_THREAD_BY_MUXTEX
        // Help to create new this->class because contractor side in private level
        static Database* getInstance(std::string name)
        {
            if (nullptr == mInstancePtr) { //Save time
                mLocker.lock();// Ensure that checking mInstance have to sync ALL thread
                if (nullptr == mInstancePtr) { // Ensure that just once oject use
                    mInstancePtr = new Database(name);
                }
                mLocker.unlock();
            }
            return mInstancePtr;
        }
        #elif MAKE_STATIC_OBJECT
        static Database* getInstance(std::string name)
        {
            static Database mInstance(name);
            return &mInstance;
        }
        #else 
        static Database* getInstance(string name)
        {
            if (nullptr == mInstancePtr) {
                mInstancePtr = new Database(name);
            }
            return mInstancePtr;
        }
        #endif // SYNC_THREAD_BY_MUXTEX          
                        
    };
    // Create mInstancePtr->nullptr
    Database* Database::mInstancePtr = nullptr;
    
#ifdef SYNC_THREAD_BY_MUXTEX
    // Create clock thread
    std::mutex Database::mLocker;
#endif // SYNC_THREAD_BY_MUXTEX


    void programTest() {
        Database* database;
        database = Database::getInstance("products");
        std::cout << "This is the " << database->getName() << " database." << std::endl;
        database = Database::getInstance("employees");
        std::cout << "This is the " << database->getName() << " database.\n" << std::endl;

        Database* database2;
        database2 = Database::getInstance("employees");
        std::cout << "This is the " << database2->getName() << " database2.\n" << std::endl;


        
    }
        
}
