//
// Created by marshtv on 29.12.2023.
//

#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"

class ConverterJSON {
public:
	ConverterJSON() = default;
	~ConverterJSON() = default;

	// Функция извлекает содержимое файла JSON в словарь nlohmann
	nlohmann::json getDicFromJsonFile(const std::string& _jsonFilePath) {
		nlohmann::json jsonDic;

		// Открывает файл *.json для чтения
		std::ifstream inJsonFile("../" + _jsonFilePath);

		// Проверка на открытие/существование файла *.json
		if (!inJsonFile.is_open()) {
			std::cout << "File with path \"" << "../" + _jsonFilePath << "\" doesn't exist!" << std::endl;
			return jsonDic;
		}

		// Парсим содержимое JSON-файла в объявленный JSON-словарь
		jsonDic = nlohmann::json::parse(inJsonFile);

		// Закрываем файл
		inJsonFile.close();

		// Возвращаем содержимое файла в виде JSON-словаря
		return jsonDic;
	}

// Функция сохраняет содержимое словаря в файл JSON
	void putDicToJsonFile(const std::string& _jsonFilePath, const nlohmann::json& _jsonDic) {
		// Открывает файл *.json для записи с нуля
		std::ofstream outJsonFile("../" + _jsonFilePath, std::ios::trunc);

		// Проверка на открытие/существование файла *.json
		if (!outJsonFile.is_open()) {
			std::cout << "File with path \"" << "../" + _jsonFilePath << "\" doesn't exist!" << std::endl;
		} else {
			// Записываем содержимое JSON-словаря в файл *.json
			outJsonFile << _jsonDic;

			// Закрываем файл
			outJsonFile.close();
		}
	}

	/**
	 * Метод обновления файла конфигурации config.json
	 * @param _docs - Список содержимого документов
	*/
	void UpdateConfigJsonFile(const std::vector<std::string>& _docs);

	/**
	* Метод получения содержимого JSON-файлов
	* @return Возвращает список с содержимым файлов перечисленных
	* в config.json
	*/
	std::vector<std::string> GetTextDocuments();

	/**
	* Метод считывает поле max_responses из файла config.json для определения предельного
	* количества ответов на один запрос
	* @return Возвращает int значение
	*/
	int GetResponsesLimit();

	/**
	* Метод сохраняет запросы в файл requests.json
	*/
	void putRequests(const std::vector<std::string>& _requests);

	/**
	* Метод получения запросов из файла requests.json
	* @return возвращает список запросов из файла requests.json
	*/
	std::vector<std::string> GetRequests();

	/**
	* Положить в файл answers.json результаты поисковых запросов
	*/
	void putAnswers(const std::vector<std::vector<std::pair<size_t, float>>>& _answers);
};

#endif //SEARCH_ENGINE_CONVERTERJSON_H
