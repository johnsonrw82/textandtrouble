/**
* File:		main.cpp
* Author:	Ryan Johnson
* Email:	johnsonrw82@cs.fullerton.edu
* Purpose:	
*/

#include "TroubleTicket.hpp"

#include <queue>
#include <stack>
#include <string>
#include <typeinfo>

// anonymous namespace
namespace {
	// template function to peek at the top element of the container
	template<typename T>
	TroubleTicket const& peek(T & container) {
		return container.top();
	}

	// specialization function for looking at the first element of a queue
	template<>
	TroubleTicket const& peek(std::queue<TroubleTicket> & container) {
		return container.front();
	}

	// print menu used for asking the user for priority
	void printPriorityMenu() {
		// choose a priority
		std::cout << "Select a priority:\n";
		// iterate over the choices to print them out
		int count = 1;
		for (int p = TroubleTicket::Priority::NONE; p <= TroubleTicket::Priority::CRITICAL; ++p) {
			std::cout << count << ". " << static_cast<TroubleTicket::Priority::Type>(p) << '\n';
			count++;
		}
		std::cout << "Your choice: ";
	}

	// template function to create a ticket and insert it into the container
	template<typename T>
	void createTicket(T & container) {
		std::string response;
		TroubleTicket::Priority::Type priority;

		// while not an acceptable response
		while (response != "1" &&
			response != "2" &&
			response != "3" &&
			response != "4" &&
			response != "5") {

			// print the menu
			printPriorityMenu();

			// get the response
			std::cin >> response;

			// if the response matches one of the priorities, use it
			if (response == "1") {
				priority = TroubleTicket::Priority::NONE;
			}
			else if (response == "2") {
				priority = TroubleTicket::Priority::LOW;
			}
			else if (response == "3") {
				priority = TroubleTicket::Priority::NORMAL;
			}
			else if (response == "4") {
				priority = TroubleTicket::Priority::HIGH;
			}
			else if (response == "5") {
				priority = TroubleTicket::Priority::CRITICAL;
			}
			else {
				std::cerr << "Invalid choice!\n";
			}
		}

		// ask the user for a description
		std::string desc;
		std::cout << "Enter a description:\n";

		// ignore any newlines in the stream
		std::cin.ignore();
		// get the response
		std::getline(std::cin, desc);
		// as long as there was no error
		if ( std::cin ) {
			TroubleTicket t(desc, priority);
			container.push(t);
			// log the ticket
			std::clog << "Inserted: " << t << '\n';
		}
		else {
			std::cerr << "Bad input, try again!\n";
		}
	}

	// template function to pull a ticket from the container and work it
	template<typename T>
	void workTicket(T & container) {
		// if a ticket is in the container
		if (container.size() > 0) {
			// peek at it
			const TroubleTicket t = peek(container);
			// pop it off
			container.pop();
			// log the removal
			std::clog << "Removed: " << t << '\n';
		}
	}

	// print the main menu
	void printMenu() {
		std::cout << "Please choose an option:\n";
		std::cout << "1. Enter a Ticket\n";
		std::cout << "2. Work a Ticket\n";
		std::cout << "3. Quit\n";
		std::cout << "Your choice: ";
	}

	// template function to simulate a work session
	template<typename T>
	void simulateSession() {
		std::clog << "Simulating ticket session using: " << typeid(T).name() << "\n\n";

		T container;
		std::string response;
		while (response != "3") {
			std::cout << "Container contains " << container.size() << " tickets.\n\n";

			printMenu();	// display menu to the user
			std::cin >> response;

			// create
			if (response == "1") {
				createTicket(container);
			}
			// work
			else if (response == "2") {
				if (container.size() == 0) {
					std::cerr << "No tickets remain!\n";
				}
				else {
					workTicket(container);
				}
			}
			// finish
			else if (response != "3") {
				std::cerr << "Invalid option!\n";
			}
		}

		std::cout << "Done!\n\n";
	}
}

// main function
int main() {
	// simulate sessions
	simulateSession<std::queue<TroubleTicket>>();
	simulateSession<std::priority_queue<TroubleTicket>>();
	simulateSession<std::stack<TroubleTicket>>();

	return 0;
}