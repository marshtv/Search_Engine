//
// Created by marshtv on 29.12.2023.
//

#include "converterjson.h"

std::vector<std::string> ConverterJSON::GetTextDocuments() {
	// Объявляем JSON-словарь, и присваиваем ему извлечённое содержимое файла "config.json"
	nlohmann::json configJsonDic = getDicFromJsonFile("config.json");

	// Создаёт вектор строк для сохранения конкретных строк
	std::vector<std::string> docFilePathList, docTextList;

	// В цикле добавляем в вектор нужные строки из ветки "files"
	for (const auto& it : configJsonDic["files"]) {
		std::ifstream docFile("../" + std::string(it));
		if (!docFile.is_open()) {
			std::cout << "Wrong Path " << "../" + std::string(it) <<
			" or file doesn't exist!" << std::endl;
			break;
		} else {
			std::string text;
			while (getline(docFile, text));
			docFile.close();
			docTextList.push_back(text);
		}
	}

	// Возвращает вектор содержимого файлов
	return docTextList;
}

int ConverterJSON::GetResponsesLimit() {
	// Объявляем JSON-словарь, и присваиваем ему извлечённое содержимое файла "config.json"
	nlohmann::json configJsonDic = getDicFromJsonFile("config.json");

	// возвращаем значение искомого ключа словаря
	return configJsonDic["config"]["max_responses"];
}

std::vector<std::string> ConverterJSON::GetRequests() {
	// Объявляем JSON-словарь
	nlohmann::json requestsInJsonDic;

	// Объявляем вектор строк
	std::vector<std::string> requestsList;

	// Присваиваем словарю извлечённое содержимое файла "requests.json"
	requestsInJsonDic = getDicFromJsonFile("requests.json");

	// В цикле из JSON-словаря заполняем вектор значениями ключа "requests"
	for (const auto& it : requestsInJsonDic["requests"]) {
		requestsList.push_back(it);
	}

	// Возвращаем вектор запросов
	return requestsList;
}

void ConverterJSON::putAnswers(const std::vector<std::vector<std::pair<size_t, float>>>& _answers) {
	// Формируем строку, имитирующую JSON-формат
	std::string answersStr = "{\n\t\"answers\":{\n\t\t";
	for (int i = 0; i < _answers.size(); ++i) {
		float count = 0;
		for (auto &it: _answers[i]) {
			if (it.second > count) {
				count += it.second;
				break;
			}
		}
		if (count == 0) {
			answersStr += "\"request00" + std::to_string(i) + "\":{\n\t\t\t" + R"("result":"false")";
		} else {
			answersStr += "\"request00" + std::to_string(i) + "\":{\n\t\t\t" + R"("result":"true",)"
				+ "\n\t\t\t\"relevance\":[\n\t\t\t\t";
			for (int j = 0; j < _answers[i].size(); ++j) {
				answersStr += "{\n\t\t\t\t\t\"docid\":" +
					std::to_string(_answers[i][j].first) + ",\"rank\":" +
					std::to_string(_answers[i][j].second) + "\n\t\t\t\t}";
				if (j < _answers[i].size() - 1)
					answersStr += ",\n\t\t\t\t";
			}
			answersStr += "\n\t\t\t]";
		}
		if (i < _answers.size() - 1) {
			answersStr += "\n\t\t},\n\t\t";
		} else
			answersStr += "\n\t\t}";
	}
	answersStr += "\n\t}\n}";

	std::cout << answersStr << std::endl;

	// Парсим строку в виде JSON-словаря и сохраняем в файл "../answers.json"
	putDicToJsonFile("answers.json", nlohmann::json::parse(answersStr));
}