//
// Created by marshtv on 03.01.2024.
//

#include "invertedIndex.h"
#include <filesystem>
#include <cassert>
#include <fstream>
#include <regex>

// Метод удаления не нужных слов или символов
std::string InvertedIndex::replaceString(const std::string& subject,
						  const std::string &search) {
	std::regex e("(\\b(" + search + "))([^,. ]*)");
	return std::regex_replace(subject, e, "");
}

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

// Метод удаления файлов из директории, исключая файл CMakeLists.txt
void eraseDBFiles(const std::string& _folder) {
	for (auto& dirEntry : std::filesystem::directory_iterator(_folder)) {
		if ((std::filesystem::path)dirEntry != std::filesystem::path(_folder + "\\CMakeLists.txt")){
			std::filesystem::remove(dirEntry);
		}
	}
}

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& _inputDocs) {
	if (!_inputDocs.empty()) {
		this->docs.clear();
		this->docs.shrink_to_fit();
		this->freqDictionary.clear();
		eraseDBFiles(this->path + "\\resources");

		ConverterJSON converterJson(this->path);
		converterJson.UpdateConfigJsonFile(_inputDocs);

		std::vector<std::vector<std::string>> docsByWords;
		for (int i = 0; i < _inputDocs.size(); ++i) {
			std::string filePath;
			if (i < 10) {
				filePath = "resources\\file00" + std::to_string(i) + ".txt";
			} else if (i > 9 && i < 100) {
				filePath = "resources\\file0" + std::to_string(i) + ".txt";
			} else if (i > 99 && i < 1000) {
				filePath = "resources\\file" + std::to_string(i) + ".txt";
			}
			std::ofstream textFile(this->path + "\\" + filePath);
			if (!textFile.is_open()) {
				std::cout << "File with path: \"" << this->path + "\\" + filePath << "\" doesn't exist!" << std::endl;
				break;
			} else {
				textFile << _inputDocs[i];
				textFile.close();
				docs.push_back(_inputDocs[i]);
				std::vector<std::string> words = convertTextToUniqWords(_inputDocs[i]);
				docsByWords.push_back(words);
			}
		}


		for (auto &words: docsByWords) {
			for (auto &word: words) {
				/*std::thread threadCount([this, &word]() {
					GetWordCount(word);
				});
				threadCount.detach();*/

				GetWordCount(word);
			}
		}
	} else {
		std::cout << "File Inputdocs is EMPTY!!!" << std::endl;
	}
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& _word) {
	std::vector<Entry> tempEntryVector;
	if (!_word.empty()) {
		for (auto &it: this->freqDictionary) {
			if (it.first == _word) {
				return freqDictionary[_word];
			}
		}
		for (int i = 0; i < this->docs.size(); ++i) {
			size_t countTemp = getWordCountInString(docs[i], _word);
			if (countTemp != 0) {
				tempEntryVector.emplace_back(static_cast<size_t>(i),
											 static_cast<size_t>(countTemp));
			}
		}
		this->freqDictionary.emplace(_word, tempEntryVector);
	} else {
		std::cout << "Input _word is EMPTY!" << std::endl;
	}

	return tempEntryVector;
}