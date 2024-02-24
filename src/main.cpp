//
// Created by marsh on 30.01.2024.
//

#include "searchServer.h"
#include <iostream>

std::string FindPath(const std::string& word){
	int a = 0;
	for (int i = 0; i < word.size(); i++)
		if (word[i] == '\\' || word[i] == '/')
			a = i;

	std::string str;
	for(int i = 0; i < a; i++)
		str += word[i];

	return str;
}

int main(int argc, char* argv[]) {
	std::string mainPath = FindPath(std::string(argv[0]));
	InvertedIndex idx(mainPath);
	ConverterJSON conv(mainPath);

	idx.UpdateDocumentBase(conv.GetTextDocuments());

	SearchServer searchServer(idx);

	std::vector<std::vector<std::pair<size_t, float>>> result = searchServer.search(conv.GetRequests());
	for (auto& i : result) {
		for (auto& it : i) {
			std::cout << "{ docId: " << it.first << ", index: " << it.second << " }" << std::endl;
		}
	}

	return EXIT_SUCCESS;
}