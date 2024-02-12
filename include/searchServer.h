//
// Created by marshtv on 05.01.2024.
//

#ifndef SEARCH_ENGINE_SEARCHSERVER_H
#define SEARCH_ENGINE_SEARCHSERVER_H

#pragma once

#include "converterjson.h"
#include "invertedIndex.h"

class SearchServer {
	public:
	/**
	* @param idx в конструктор класса передаётся ссылка на класс InvertedIndex,
	* чтобы SearchServer мог узнать частоту слов встречаемых в запросе
	*/
	explicit SearchServer(InvertedIndex& _idx) : idx(_idx) {};

	SearchServer(const SearchServer& other) { idx = other.idx; };

	SearchServer& operator=(const SearchServer& other) {
		if (this == &other)
			return *this;
		idx = other.idx;
		return *this;
	};

	~SearchServer() = default;

	/**
	 * Метод обработки поисковых запросов
	 * @param queries_input поисковые запросы взятые из файла requests.json
	 * @return возвращает отсортированный список релевантных ответов для заданных запросов
	*/
	std::vector<std::vector<std::pair<size_t, float>>> search(const std::vector<std::string>& queriesInput);

private:
	InvertedIndex idx;
};

#endif //SEARCH_ENGINE_SEARCHSERVER_H
