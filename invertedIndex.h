//
// Created by marshtv on 03.01.2024.
//

#ifndef SEARCH_ENGINE_INVERTEDINDEX_H
#define SEARCH_ENGINE_INVERTEDINDEX_H

#pragma once

#include <string>
#include <vector>
#include <map>
#include "converterjson.h"

struct Entry {
	size_t docId, count;
	// Данный оператор необходим для проведения тестовых сценариев
	bool operator ==(const Entry& other) const {
		return (docId == other.docId &&
				count == other.count);
	}
};

class InvertedIndex {
public:
	InvertedIndex() = default;
	InvertedIndex(const InvertedIndex& other) {
		this->docs.clear();
		this->docs.shrink_to_fit();
		this->docs = other.docs;

		this->freqDictionary.clear();
		this->freqDictionary = other.freqDictionary;
	}
	InvertedIndex& operator=(const InvertedIndex& other) {
		if (this == &other)
			return *this;
		this->docs.clear();
		this->docs.shrink_to_fit();
		this->docs = other.docs;

		this->freqDictionary.clear();
		this->freqDictionary = other.freqDictionary;
		return  *this;
	}
	~InvertedIndex() = default;

	size_t GetDocsSize() {
		return this->docs.size();
	}

	// Метод нахождения и удаления ненужных слов
	std::string replaceString(const std::string& subject, const std::string &search);

	// Метод разбивает текст на уникальные слова
	std::vector<std::string> convertTextToUniqWords(const std::string& _text);

	/**
	 * Обновить или заполнить базу документов, по которой будем совершать поиск
	 *
	 * @param textsStr содержимое документов
	*/
	void UpdateDocumentBase(const std::vector<std::string>& _inputDocs);

	/**
	 * Метод определяет количество вхождений слова word в загруженной базе документов
	 * @param word слово, частоту вхождений которого необходимо определить
	 * @return возвращает подготовленный список с частотой слов
	*/
	std::vector<Entry> GetWordCount(const std::string& _word);

private:
	// Список содержимого документов
	std::vector<std::string> docs;
	// Частотный словарь<слово, вектор структур<Entry{docId, count}>>
	std::map<std::string, std::vector<Entry>> freqDictionary;
};

#endif //SEARCH_ENGINE_INVERTEDINDEX_H
