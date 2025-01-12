#pragma once

#include <iostream>

namespace nsAdapter {

    class VendorInterface
    {
    public:
        virtual void setName(std::string name) = 0;
        virtual std::string getName() = 0;
    };

    class Vendor : public VendorInterface
    {
    private:
        std::string mName;

    public:
        void setName(std::string name)
        {
            mName = name;
        }

        std::string getName()
        {
            return mName;
        }
    };

    class UserInterface
    {
    public:
        virtual void setFirstName(std::string firstName) = 0;
        virtual void setLastName(std::string lastName) = 0;
        virtual std::string getFirstName() = 0;
        virtual std::string getLastName() = 0;
    };

    class User : public UserInterface
    {
    private:
        std::string mFirstname;
        std::string mLastname;
    public:
        void setFirstName(std::string firstName)
        {
            mFirstname = firstName;
            std::cout << "Set first name successfuly \n";
        }

        void setLastName(std::string lastName)
        {
            mLastname = lastName;
            std::cout << "Set last name successfuly \n";
        }

        std::string getFirstName()
        {
            return mFirstname;
        }

        std::string getLastName()
        {
            return mLastname;
        }
    };

    class VendorToUserAdapter : public UserInterface
    {
    private:
        Vendor mVendor;
        std::string mFirstname;
        std::string mLastname;

    public:
        VendorToUserAdapter(Vendor vendor)
        {
            mVendor = vendor;

            // split first name and last name
            unsigned int splitPostion = vendor.getName().find_first_of(" ");
            if (splitPostion != std::string::npos)
            {
                mFirstname = vendor.getName().substr(0, splitPostion + 1);
                mLastname = vendor.getName().substr(splitPostion + 1, vendor.getName().length() - mFirstname.length());
            }
        }

        void setFirstName(std::string firstName)
        {
            mFirstname = firstName;
        }

        void setLastName(std::string lastName)
        {
            mLastname = lastName;
        }

        std::string getFirstName()
        {
            return mFirstname;
        }

        std::string getLastName()
        {
            return mLastname;
        }

    };
    void programTest() {
        // create vendor object
        Vendor vendor;
        vendor.setName("Le Quoc Hai"); //-> vendor method call func

        // create adapter
        VendorToUserAdapter adapter(vendor);
        cout << "First name: " << adapter.getFirstName() << endl;
        cout << "Last name: " << adapter.getLastName() << endl;

        adapter.setFirstName("HAI");
        adapter.setLastName("LE QUOC");
        cout << "First name: " << adapter.getFirstName() << endl;
        cout << "Last name: " << adapter.getLastName() << endl;

    }
}