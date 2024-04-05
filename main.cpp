//
// Created by marsh on 30.01.2024.
//

#include "searchServer.h"
#include <iostream>

int main(int argc, char* argv[]) {
	InvertedIndex idx;
	ConverterJSON conv;

	idx.UpdateDocumentBase(conv.GetTextDocuments());

	SearchServer searchServer(idx);

	std::vector<std::vector<std::pair<size_t, float>>> result = searchServer.search(conv.GetRequests());
	std::cout << "------------------------" << std::endl;
	for (auto& i : result) {
		for (auto& it : i) {
			std::cout << "{ docId: " << it.first << ", index: " << it.second << " }" << std::endl;
		}
		std::cout << "------------------------" << std::endl;
	}

	return EXIT_SUCCESS;
}