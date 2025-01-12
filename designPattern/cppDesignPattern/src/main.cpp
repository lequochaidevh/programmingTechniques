#include "pch.h"
#include "Behavioral/Observer.h"
#include "Behavioral/ObserverSourceMaking.h"
#include "Behavioral/State.h"
#include "Behavioral/Command.h"
#include "Behavioral/Strategy.h"
#include "Behavioral/ChainOfResonsibility.h"

#include "Creational/Singleton.h"
#include "Creational/FactoryMethod.h"
#include "Creational/AbstractFactory.h"

#include "Structural/Decorator.h"
#include "Structural/Adapter.h"

#include <iostream>

int main() {
	nsChainOfResponsibility::programTest();

	std::cout << "------ END MAIN ------ \n";
	return 0;
}

