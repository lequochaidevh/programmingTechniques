#pragma once
#include <iostream>
#include <vector>

namespace nsChainOfResponsibility {
    /**
 * The Handler interface declares a method for building the chain of handlers.
 * It also declares a method for executing a request.
 */
    class IHandler {
    public:
        virtual IHandler* SetNext(IHandler* handler) = 0;
        virtual std::string Handle(std::string request) = 0;
    };
    /**
     * The default chaining behavior can be implemented inside a base handler class.
     */
    class AbstractHandler : public IHandler {
        /**
         * @var Handler
         */
    private:
        IHandler* mNextHandler;

    public:
        AbstractHandler() : mNextHandler(nullptr) {
        }
        IHandler* SetNext(IHandler* handler) override {
            this->mNextHandler = handler;
            // Returning a handler from here will let us link handlers in a convenient
            // way like this:
            // $monkey->setNext($squirrel)->setNext($dog);
            return handler;
        }
        std::string Handle(std::string request) override {
            if (this->mNextHandler) {
                return this->mNextHandler->Handle(request);
            }

            return {};
        }
    };
    /**
     * All Concrete Handlers either handle a request or pass it to the next handler
     * in the chain.
     */
    class MonkeyHandler : public AbstractHandler {
    public:
        std::string Handle(std::string request) override {
            if (request == "Banana") {
                return "Monkey: I'll eat the " + request + ".\n";
            }
            else {
                return AbstractHandler::Handle(request);
            }
        }
    };
    class SquirrelHandler : public AbstractHandler {
    public:
        std::string Handle(std::string request) override {
            if (request == "Nut") {
                return "Squirrel: I'll eat the " + request + ".\n";
            }
            else {
                return AbstractHandler::Handle(request);
            }
        }
    };
    class DogHandler : public AbstractHandler {
    public:
        std::string Handle(std::string request) override {
            if (request == "MeatBall") {
                return "Dog: I'll eat the " + request + ".\n";
            }
            else {
                return AbstractHandler::Handle(request);
            }
        }
    };
    /**
     * The client code is usually suited to work with a single handler. In most
     * cases, it is not even aware that the handler is part of a chain.
     */
    void ClientCode(IHandler& handler) {
        std::vector<std::string> food = { "Nut", "Banana", "Cup of coffee" };
        for (const std::string& f : food) {
            std::cout << "Client: Who wants a " << f << "?\n";
            const std::string result = handler.Handle(f);
            if (!result.empty()) {
                std::cout << "  " << result;
            }
            else {
                std::cout << "  " << f << " was left untouched.\n";
            }
        }
    }
    /**
     * The other part of the client code constructs the actual chain.
     */
    void programTest() {
        MonkeyHandler* monkey = new MonkeyHandler;
        SquirrelHandler* squirrel = new SquirrelHandler;
        DogHandler* dog = new DogHandler;

        // Link all object to make chain and each one have a responsibility specifically
        monkey->SetNext(squirrel)->SetNext(dog);

        /**
         * The client should be able to send a request to any handler, not just the
         * first one in the chain.
         */
        std::cout << "Chain: Monkey > Squirrel > Dog\n\n";
        ClientCode(*monkey);
        std::cout << "\n";
        std::cout << "Subchain: Squirrel > Dog\n\n";
        ClientCode(*squirrel);

        delete monkey;
        delete squirrel;
        delete dog;
    }
}