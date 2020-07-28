// MummySolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>
#include <vector>
#include <array>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <queue>
#include <algorithm>

enum Slot { SOne, STwo, SThree, SFour, SFive, SSix, SCenter };
enum Chiplet { COne, CTwo, CThree, CFour, CFive, CSix, CNone };
enum MoveStringType { SlotBased, ChipletBased };

const std::map<Slot, std::vector<Slot>> openSlotToMovableSlotMap = {
	{SOne, {SSix, STwo, SCenter}},
	{STwo, {SOne, SThree, SCenter}},
	{SThree, {STwo, SFour, SCenter}},
	{SFour, {SThree, SFive, SCenter}},
	{SFive, {SFour, SSix, SCenter}},
	{SSix, {SFive, SOne, SCenter}},
	{SCenter, {SOne, STwo, SThree, SFour, SFive, SSix}}
};

template<typename T>
std::vector<T> enumVectorFromIntVector(std::vector<int> const& intVector) {
	std::vector<T> enumVector;
	enumVector.reserve(intVector.size());

	std::transform(intVector.begin(), intVector.end(), std::back_inserter(enumVector),
		[](int i) { return static_cast<T>(i); });
	return enumVector;
}

template<typename T, std::enable_if_t<std::is_enum_v<T>, int> = 0>
std::string joinEnumVector(std::vector<T> enumVector, std::string delimter, bool rebaseToOne)
{
	std::string joinedString;

	for (unsigned int i = 0; i < enumVector.size(); i++)
	{
		joinedString += std::to_string(enumVector.at(i) + static_cast<int>(rebaseToOne));
		
		if (i != enumVector.size() - 1)
			joinedString += delimter;
	}

	return joinedString;
}

class Path
{
public:
	static inline const std::array<Chiplet, 7> nullSlotState = { CNone, CNone, CNone, CNone, CNone, CNone, CNone };
	static inline const std::array<Chiplet, 7> targetSlotState = { COne, CTwo, CThree, CFour, CFive, CSix, CNone };

	std::array<Chiplet, 7> mInitialSlotStates = nullSlotState;
	std::array<Chiplet, 7> mCurrentSlotStates = nullSlotState;
	std::vector<Slot> mMoves;

	Path() {}
	Path(Chiplet slotOne, Chiplet slotTwo, Chiplet slotThree, Chiplet slotFour, Chiplet slotFive, Chiplet slotSix, Chiplet slotCenter)
	{
		mCurrentSlotStates.at(0) = slotOne;
		mCurrentSlotStates.at(1) = slotTwo;
		mCurrentSlotStates.at(2) = slotThree;
		mCurrentSlotStates.at(3) = slotFour;
		mCurrentSlotStates.at(4) = slotFive;
		mCurrentSlotStates.at(5) = slotSix;
		mCurrentSlotStates.at(6) = slotCenter;
		mInitialSlotStates = mCurrentSlotStates;
	}

private:
	int currentEmptySlotIndex() { return std::distance(mCurrentSlotStates.begin(), std::find(mCurrentSlotStates.begin(), mCurrentSlotStates.end(), CNone)); }

	void makeMove(Slot from)
	{
		assert(from != currentEmptySlotIndex()); // Should never occur

		mCurrentSlotStates.at(currentEmptySlotIndex()) = mCurrentSlotStates.at(from);
		mCurrentSlotStates.at(from) = Chiplet::CNone;

		mMoves.push_back(from);
	}

	std::vector<Chiplet> chipletMovseFromSlotMoves()
	{
		std::vector<Chiplet> chipletMoves;

		Path reWalker = Path(*this);
		reWalker.reset();

		for (Slot slotMove : mMoves)
		{
			chipletMoves.push_back(reWalker.getCurrentSlotState(slotMove));
			reWalker.makeMove(slotMove);
		}

		return chipletMoves;
	}

public:
	bool isNull() { return mCurrentSlotStates == nullSlotState; }
	bool isComplete() { return mCurrentSlotStates == targetSlotState; }
	bool isInitialState() { return mCurrentSlotStates == mInitialSlotStates; }
	bool isBackwardsMove(Slot slotMove) { return mMoves.size() == 0 ? false : (mMoves.size() == 1 ? slotMove == SCenter : slotMove == *(mMoves.end() - 2)); }
	int moveCount() { return mMoves.size(); }
	std::vector<Slot> getMoves() { return mMoves; }
	Chiplet getCurrentSlotState(Slot slotNum) { return mCurrentSlotStates.at(slotNum); }
	
	void reset()
	{
		mCurrentSlotStates = mInitialSlotStates;
		mMoves.clear();
	}

	std::string getMovesString(MoveStringType stringBase) 
	{ 
		if (stringBase == SlotBased)
			return joinEnumVector(mMoves, ", ", true);
		else
			return joinEnumVector(chipletMovseFromSlotMoves(), ", ", true);
	}

	std::vector<Path> generateMoveDecendants()
	{
		std::vector<Path> decendents;
		std::vector<Slot> possibleMoves = openSlotToMovableSlotMap.at(static_cast<Slot>(currentEmptySlotIndex()));

		for (Slot move : possibleMoves)
		{
			if (!isBackwardsMove(move)) // Disallow backwards moves
			{
				decendents.push_back(Path(*this)); // Add copy of current instance to decendents vector
				decendents.back().makeMove(move); // Advance copy to the next move
			}
		}

		return decendents;
	}
};

// Checks that "pattern" (* = zero or more of any character, ? = any single character) matches "check". Do not use consecutive *
bool wildCardMatch(std::string pattern, std::string check)
{
	// Return true if the end of both strings were reached successfully
	if (pattern.empty() && check.empty())
		return true;

	// Ensure characters after a '*' are present in checked string
	if (pattern.size() > 1 && pattern.front() == '*' && check.empty())
		return false;

	// Proceed if current characters match or current pattern character is '?'
	if ((!pattern.empty() && pattern.front() == '?') || (!pattern.empty() && !check.empty() && pattern.front() == check.front()))
	{
		std::string patternNext = pattern;
		std::string checkNext = check;

		patternNext.erase(0, 1);
		checkNext.erase(0, 1);

		return wildCardMatch(patternNext, checkNext);
	}

	// Handle '*' character until all wild cards are accounted for
	if (!pattern.empty() && pattern.front() == '*')
	{
		std::string patternNext = pattern;
		std::string checkNext = check;

		patternNext.erase(0, 1);
		checkNext.erase(0, 1);

		return wildCardMatch(patternNext, check) || wildCardMatch(pattern, checkNext); // Either there is another wildcard from '*' or all have been handled
	}

	return false;
}

// Same as wildCardMatch but '*' wildcards must contain at least 1 character
bool wildCardMatchMin(std::string pattern, std::string check) { return check.size() >= pattern.size() && wildCardMatch(pattern, check); }

bool hasNumberDuplicates(std::vector<int> check)
{
	std::sort(check.begin(), check.end());
	return std::unique(check.begin(), check.end()) != check.end();
}

// Determines the shortest path to solve the puzzle based on its starting conditions. If multiple paths with the same length are found, only the first is reported.
// Paths are allocated on the heap to prevent potential stack overflows
Path getShortestPath(Path initialPath)
{
	// Shortest Path tracker
	std::unique_ptr<Path> shortestPath = std::make_unique<Path>();

	// FIFO Processing queue for level order traversal of the working path tree
	std::queue<std::unique_ptr<Path>> pathProgressionQueue;

	// Load queue with initial puzzle state
	pathProgressionQueue.push(std::make_unique<Path>(initialPath));

	// Process all paths until the shortest possible is found
	while (!pathProgressionQueue.empty())
	{
		// Current path (must be done in 2 steps due to unique_ptr use)
		std::unique_ptr<Path> currentPath = std::move(pathProgressionQueue.front()); // Transfer ownership of queue front to this pointer
		pathProgressionQueue.pop(); // Remove the now null pointer from the front of the queue

		// DEBUG MOVE COUNT
		int currentLevel = currentPath->moveCount();

		// If the path has reached the target then mark it as the current shortest
		if (currentPath->isComplete())
			shortestPath = std::move(currentPath);
		else if(shortestPath->isNull() || currentPath->moveCount() < shortestPath->moveCount() - 1) 
		{
			// Add next level nodes of currentPath to queue. Don't bother if at best their completion length will be equal to the known shortest path
			std::vector<Path> decedents = currentPath->generateMoveDecendants();

			// Only add each node if hasn't happened to wrap back around to the initial puzzle state
			for (Path decendent : decedents)
				if (!decendent.isInitialState())
					pathProgressionQueue.push(std::make_unique<Path>(decendent));
		}
	}

	return Path(*shortestPath); // Make a new copy of Path using the contents of what shortestPath points at
}

int main()
{
	// Input storage
	bool userInputIsValid = false;
	std::string userInput;
	std::vector<int> startingValues;

	// Infinite loop for solving as many puzzles as desired
	for (; ; )
	{
		// Input prompt
		std::string mainPrompt = "Input the starting values of slots 1-6 in clockwise order as a comma separated list.";
		std::cout << mainPrompt << std::endl;

		// Get user input until it is valid
		while (!userInputIsValid)
		{
			std::cout << "Starting Values: ";
			std::getline(std::cin, userInput);
			std::cout << std::endl; // Prepare for return message

			userInput.erase(std::remove(userInput.begin(), userInput.end(), ' '), userInput.end()); // Remove all spaces

			if (!wildCardMatchMin("*,*,*,*,*,*", userInput)) // Check for valid format
				std::cout << "Invalid input format. Try again." << std::endl << std::endl;
			else if (!std::all_of(userInput.begin(), userInput.end(), [](char c) { return c == ',' || std::isdigit(c); })) // Check for only numbers or commas
				std::cout << "Positive numbers only please." << std::endl << std::endl;
			else
			{
				// String stream for extracting numbers
				std::stringstream ss(userInput);

				// Add all numbers to vector
				for (int num; ss >> num;) {
					startingValues.push_back(num - 1); // Minus 1 since enums are zero referenced
					if (ss.peek() == ',')
						ss.ignore();
				}


				// Ensure all input numbers are valid
				if (*std::max_element(startingValues.begin(), startingValues.end()) > 5 || *std::min_element(startingValues.begin(), startingValues.end()) < 0 || hasNumberDuplicates(startingValues))
				{
					startingValues.clear();
					std::cout << "Starting values must all be unique and between 1 and 6 (inclusive)." << std::endl;
				}
				else
					userInputIsValid = true;
			}
		}

		// Notify user calculation has begun
		std::cout << "Calculating shortest moveset..." << std::endl << std::endl;

		// Create initial Path
		std::vector<Chiplet> startingChiplets(enumVectorFromIntVector<Chiplet>(startingValues)); // Cast whole vector from int to Chiplet
		Path initialPath(startingChiplets.at(0), startingChiplets.at(1), startingChiplets.at(2),
			             startingChiplets.at(3), startingChiplets.at(4), startingChiplets.at(5), CNone); // Center is always empty to start

		// Get shortest path
		Path shortestPath = getShortestPath(initialPath);

		// Print results
		std::string slotMoves = shortestPath.getMovesString(SlotBased);
		std::replace(slotMoves.begin(), slotMoves.end(), '7', 'C'); // "C" for center

		std::cout << "Fewest Moves: " << shortestPath.moveCount() << std::endl
				  << "Move Path (Slot Number): " << slotMoves << std::endl
				  << "Move Path (Chiplet Number): " << shortestPath.getMovesString(ChipletBased) << std::endl << std::endl;

		// Print divider
		std::cout << std::string(mainPrompt.size(), '-') << std::endl << std::endl;

		// Reset input
		userInputIsValid = false;
		startingValues.clear();


	}

	//// Print exit prompt
	//std::cout << "Press any key to exit...";
	//std::cin.ignore();
}



