#pragma once
#include <iostream>
#include <vector>

namespace nsObserver {
    class IObserver; //define prototype
    class ISubject
    {
    public:
        virtual void registerObserver(IObserver* o) = 0;
        virtual void removeObserver(IObserver* o) = 0;
        virtual void notifyObservers() = 0;
    };  
    
    class IObserver
    {
    public:
        virtual void update(std::string operation, std::string record) = 0;
    };

    class Database : public ISubject
    {
    private:
        std::vector<IObserver*> mObservers; //can design list at ISubject (private)
        std::string mOperation;
        std::string mRecord;

    public:
        Database() {}

        void registerObserver(IObserver* o) {
            mObservers.push_back(o);
        }

        void removeObserver(IObserver* o) {
            auto observer = find(mObservers.begin(), mObservers.end(), o);
            if (observer != mObservers.end()) {
                mObservers.erase(observer, observer + 1); // remove observer from mObservers
            }
        }
        void notifyObservers() {
            for (auto& o : mObservers) {
                o->update(mOperation, mRecord);
            }
        }
        void editRecord(std::string operation, std::string record) {
            mOperation = operation;
            mRecord = record;
            notifyObservers();
        }
    };

    class Client : public IObserver
    {
    public:
        Client() {}

        void update(std::string operation, std::string record) {
            std::cout << "Client: " << operation << " operation was performed on " << record << std::endl;
        }
    };

    class Developer : public IObserver
    {
    public:
        Developer() {}

        void update(std::string operation, std::string record) {
            std::cout << "Developer: " << operation << " operation was performed on " << record << std::endl;
        }
    };

    class Boss : public IObserver
    {
    public:
        Boss() {}

        void update(std::string operation, std::string record) {
            std::cout << "Boss: " << operation << " operation was performed on " << record << std::endl;
        }
    };
    void programTest() {

        Database database;
        Developer dev;
        Client client;
        Boss boss;
        database.registerObserver(&dev);
        database.registerObserver(&client);
        database.registerObserver(&boss);
        database.editRecord("delete", "record1");

        // remove boss
        database.removeObserver(&boss);
        database.editRecord("modify", "record2");

    }
}

