#pragma once
#include <iostream>
#include <random> 
namespace nsState {

    /*class WaitingState;
    class ReceivingFormState;
    class RentApartmentState;
    class FullyRentedState;*/

    class IState
    {
    public:
        virtual void getForm() = 0;
        virtual void checkForm() = 0;
        virtual void rentApartment() = 0;
        virtual void dispenseKeys() = 0;
    };

    class IRentalRobot
    {
    public:
        virtual void getForm() = 0;
        virtual void checkForm() = 0;
        virtual void setState(IState* state) = 0;
        virtual IState* getState() = 0;
        virtual IState* getWaitingState() = 0;
        virtual IState* getReceivingFormState() = 0;
        virtual IState* getRentApartmentState() = 0;
        virtual IState* getFullyRentedState() = 0;
        virtual int getCount() = 0;
        virtual void setCount(int count) = 0;
    };

    class WaitingState : public IState
    {
    private:
        IRentalRobot* mRobot;

    public:
        WaitingState(IRentalRobot* robot)
        {
            mRobot = robot;
        }

        void getForm()
        {
            mRobot->setState(mRobot->getReceivingFormState());
            std::cout << "Thanks for the form." << std::endl;
        }

        void checkForm()
        {
            std::cout << "You have to submit an form." << std::endl;
        }

        void rentApartment()
        {
            std::cout << "You have to submit an form." << std::endl;
        }

        void dispenseKeys()
        {
            std::cout << "You have to submit an form." << std::endl;
        }
    };

    class ReceivingFormState : public IState
    {
    private:
        IRentalRobot* mRobot;
        std::random_device mRandomGenerator;

    public:
        ReceivingFormState(IRentalRobot* robot)
        {
            mRobot = robot;
        }

        void getForm()
        {
            std::cout << "We already got your application." << std::endl;
        }

        void checkForm()
        {
            // simulate the form checking
            std::uniform_int_distribution<int> int_distribution(0, 9);
            bool isFormOk = (int_distribution(mRandomGenerator) > 3);

            if (isFormOk && mRobot->getCount() > 0) {
                std::cout << "Congratulations, you were approved." << std::endl;
                mRobot->setState(mRobot->getRentApartmentState());
                //mRobot->getState()->rentApartment();
                //std::cout << "-----------------------------" << std::endl;
            }
            else {
                mRobot->setState(mRobot->getWaitingState());
                std::cout << "Sorry, you were not approved." << std::endl;
                std::cout << "-----------------------------" << std::endl;
                //mRobot->getState()->rentApartment();
            }
        }

        void rentApartment()
        {
            std::cout << "You must have your application checked." << std::endl;
        }

        void dispenseKeys()
        {
            std::cout << "You must have your application checked." << std::endl;
        }
    };

    class RentApartmentState : public IState
    {
    private:
        IRentalRobot* mRobot;

    public:
        RentApartmentState(IRentalRobot* robot)
        {
            mRobot = robot;
        }

        void getForm()
        {
            std::cout << "Hang on, we're renting you an apartment." << std::endl;
        }

        void checkForm()
        {
            std::cout << "Hang on, we're renting you an apartment." << std::endl;
        }

        void rentApartment()
        {
            mRobot->setCount(mRobot->getCount() - 1);
            std::cout << "Renting you an apartment...." << std::endl;
            dispenseKeys();
        }

        void dispenseKeys()
        {
            if (mRobot->getCount() > 0) {
                mRobot->setState(mRobot->getWaitingState());
            }
            else {
                mRobot->setState(mRobot->getFullyRentedState());
            }
            std::cout << "Here are your keys!" << std::endl;
        }
    };

    class FullyRentedState : public IState
    {
    private:
        IRentalRobot* mRobot;

    public:
        FullyRentedState(IRentalRobot* robot)
        {
            mRobot = robot;
        }

        void getForm()
        {
            std::cout << "Sorry, we're fully rented." << std::endl;
        }

        void checkForm()
        {
            std::cout << "Sorry, we're fully rented." << std::endl;
        }

        void rentApartment()
        {
            std::cout << "Sorry, we're fully rented." << std::endl;
        }

        void dispenseKeys()
        {
            std::cout << "Sorry, we're fully rented." << std::endl;
        }
    };

    class RentalRobot : public IRentalRobot
    {
    private:
        IState* mCurentState;
        IState* mWaitingState;
        IState* mReceivingFormState;
        IState* mRentApartmentState;
        IState* mFullyRentedState;
        int mCount;
        
    public:
        int MAXROOM;
        RentalRobot(int count)
        {
            mCount = count;
            MAXROOM = count;
            mWaitingState = new WaitingState(this);
            mReceivingFormState = new ReceivingFormState(this);
            mRentApartmentState = new RentApartmentState(this);
            mFullyRentedState = new FullyRentedState(this);
            mCurentState = mWaitingState;
        }

        ~RentalRobot()
        {
            if (mWaitingState != nullptr) {
                delete mWaitingState;
                mWaitingState = nullptr;
            }

            if (mReceivingFormState != nullptr) {
                delete mReceivingFormState;
                mReceivingFormState = nullptr;
            }

            if (mRentApartmentState != nullptr) {
                delete mRentApartmentState;
                mRentApartmentState = nullptr;
            }

            if (mFullyRentedState != nullptr) {
                delete mFullyRentedState;
                mFullyRentedState = nullptr;
            }

            mCurentState = nullptr;
        }

        void getForm()
        {
            mCurentState->getForm();
        }

        void checkForm()
        {
            mCurentState->checkForm();
        }

        void rentApartment()
        {
            mCurentState->rentApartment();
        }

        void setState(IState* state)
        {
            mCurentState = state;
        }

        IState* getState()
        {
            return mCurentState;
        }

        IState* getWaitingState()
        {
            return mWaitingState;
        }

        IState* getReceivingFormState()
        {
            return mReceivingFormState;
        }

        IState* getRentApartmentState()
        {
            return mRentApartmentState;
        }

        IState* getFullyRentedState()
        {
            return mFullyRentedState;
        }

        int getCount()
        {
            return mCount;
        }
        
        void setCount(int count)
        {
            mCount = count;
        }

        int afterFreeRoom(int count) {
            mCount = (MAXROOM > (mCount + count) ? (mCount + count) : MAXROOM);
            mCurentState = getWaitingState();
            return mCount;
        }

    };

    void programTest() {

        RentalRobot robot(20); // init 20 room
        for (int i = 0; i < 100; i++) {
            robot.getForm();
            robot.checkForm();
            robot.getState()->rentApartment();
            std::cout << "-----------------------------" << std::endl;

            std::random_device mHaveFreeRoom;
            std::uniform_int_distribution<int> i_HaveFreeRoom(0, 9);
            std::random_device mFreeRoomNumber;
            std::uniform_int_distribution<int> i_FreeRoomNumber(1, 3);
            if (i_HaveFreeRoom(mHaveFreeRoom) > 7) { // percent return room 1/10
                int iTemp = i_FreeRoomNumber(mFreeRoomNumber);
                int infor = robot.afterFreeRoom(iTemp);

                std::cout << "Free Room Successfuly" << std::endl;
                std::cout << "Have " << infor << " Free Room" << std::endl;
                std::cout << "-----------------------------" << std::endl;
            }
        }
        
        
    }

}
