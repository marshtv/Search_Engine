//
// Created by marsh on 30.01.2024.
//

#include "searchServer.h"
#include <iostream>

std::string FindPath(const std::string& word){
	int a = 0;
	for (int i = 0; i < word.size(); i++)
		if (word[i] == '\\')
			a = i;

	std::string str;
	for(int i = 0; i < a; i++)
		str += word[i];

	//std::cout << str << std::endl;

	return str;
}

int main(int argc, char* argv[]) {
	std::string mainPath = FindPath(std::string(argv[0]));
	InvertedIndex idx(mainPath);
	std::vector<std::string> inputDocs{"moscow is the capital of russia",
									   "union of soviet socialist republics",
									   "several facts about olimpic games"};
	idx.UpdateDocumentBase(inputDocs);
	SearchServer searchServer(idx);
	std::vector<std::string> queries{"moscow is capital", "socialist republic", "olimpic games", "troubles"};
	std::vector<std::vector<std::pair<size_t, float>>> result = searchServer.search(queries);
	for (auto& i : result) {
		for (auto& it : i) {
			std::cout << "{ docId: " << it.first << ", index: " << it.second << " }" << std::endl;
		}
	}

	return EXIT_SUCCESS;
}