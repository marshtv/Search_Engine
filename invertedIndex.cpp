//
// Created by marshtv on 03.01.2024.
//

#include "invertedIndex.h"
#include <iostream>
#include <regex>
#include <mutex>

std::vector<std::string> InvertedIndex::convertTextToUniqWords(const std::string& _text) {
	// Вектор слов
	std::vector<std::string> words;

	// Текст не должен быть пустым
	if (_text.empty())
		std::cout << "Text is EMPTY!" << std::endl;
	else {

		// Регулярное выражение для выделения слов из строки
		std::regex regexWordRule{R"((\w+))"};

		// Временная строка инициализируется параметром функции,
		// чтобы оставить оригинальный текст без изменений.
		std::string textTemp = _text;

		// Стартовый строковый итератор регулярных выражений
		auto wordBegin = std::sregex_iterator(textTemp.begin(),
											  textTemp.end(),
											  regexWordRule);

		// Конечный строковый итератор регулярных выражений
		auto wordEnd = std::sregex_iterator();

		// В цикле добавляем выбранные слова в вектор
		for (auto &it = wordBegin; it != wordEnd; ++it) {
			words.push_back(it->str());
		}
	}

	// Возвращаем значение вектора слов
	return words;
}

// Метод возвращает количество вхождения слова в строке
size_t getWordCountInString(const std::string& _text, const std::string& _regexStr) {
	std::string regexStr = "(\\b(" + _regexStr + "))([^,. ]*)";
	std::regex re(regexStr);
	auto wordBegin = std::sregex_iterator(_text.begin(), _text.end(), re);
	auto wordEnd = std::sregex_iterator();
	return std::distance(wordBegin, wordEnd);
}

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& _inputDocs) {
	if (!_inputDocs.empty()) {
		std::vector<std::vector<std::string>> docsByWords;
		for (const auto & inputDoc : _inputDocs) {
			bool isAvailable = false;
			if (!docs.empty()) {
				for (const auto &doc: docs) {
					if (inputDoc == doc) {
						isAvailable = true;
						break;
					}
				}
				if (!isAvailable) {
					docs.push_back(inputDoc);
					docsByWords.push_back(convertTextToUniqWords(inputDoc));
				}
			} else {
				docs.push_back(inputDoc);
				docsByWords.push_back(convertTextToUniqWords(inputDoc));
			}
		}
		for (auto &words: docsByWords) {
			for (auto &word: words) {
				this->freqDictionary.emplace(word, GetWordCount(word));
			}
		}
	} else {
		std::cout << "File InputDocs is EMPTY!!!" << std::endl;
	}
}

std::mutex mtxIdx;
std::vector<Entry> InvertedIndex::GetWordCount(const std::string& _word) {
	std::lock_guard<std::mutex> lg(mtxIdx);
	std::vector<Entry> tempEntryVector;
	if (!_word.empty()) {
		if (auto search = freqDictionary.find(_word); search != freqDictionary.end()) {
			return search->second;
		} else {
			for (size_t i = 0; i < this->docs.size(); ++i) {
				size_t countTemp = getWordCountInString(docs[i], _word);
				if (countTemp != 0) {
					tempEntryVector.emplace_back(i,countTemp);
				}
			}
		}
	} else {
		std::cout << "Input _word is EMPTY!" << std::endl;
	}
	return tempEntryVector;
}