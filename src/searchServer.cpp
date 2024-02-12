//
// Created by marshtv on 05.01.2024.
//

//#include <iostream>
#include "searchServer.h"
#include <string>
#include <cassert>
#include <algorithm>
#include <thread>
#include <limits>
#include <cmath>

// Метод сравнивает числа типа float
bool isEqualFloat(float x, float y) {
	return (std::fabs( x - y ) <=
	std::numeric_limits<float>::epsilon() * std::fmax(std::fabs(x), std::fabs(y)));
}

std::vector<std::vector<std::pair<size_t, float>>> SearchServer::search(
	const std::vector<std::string>& _queriesInput) {
	// список запросов не должен быть пустым
	assert(!_queriesInput.empty());

	// сохраняем список запросов в файл "requests.json"
	ConverterJSON converterJson(idx.GetPath());
	converterJson.putRequests(_queriesInput);

	// Список релевантности документов по всем запросам
	std::vector<std::vector<std::pair<size_t, float>>> relativeIndexVecAllQueries, topRelativeIndexes;

	// Запускаем цикл для обработки каждого запроса
	for (const auto & query : _queriesInput) {
		std::vector<std::pair<size_t, float>> relativeIndexVec;
		std::vector<std::pair<std::string, std::vector<Entry>>> freqVec;

		// Отсеиваем только уникальные слова, а затем разбиваем строку запроса на слова
		std::vector<std::string> queryWords = idx.convertTextToUniqWords(query);

		/**
		 * Получаем вектор индексов встречающихся уникальных слов во всех документах,
		 * собираем в пары слова и вектора индексов, а пары собираем в вектор по всему запросу,
		 * сортируем его по возрастанию значения count.
		*/
		for (auto &wordIt: queryWords) {
			std::vector<Entry> sortedIndexVec;
			std::thread threadWordCount([&]() {
				sortedIndexVec = idx.GetWordCount(wordIt);
			});
			threadWordCount.join();
			// сортируем полученный вектор с помощью std::sort() и лямбда-функции в аргументе сравнения.
			std::sort(sortedIndexVec.begin(),
					  sortedIndexVec.end(),
					  [](auto &left, auto &right) {
						  return left.count < right.count;
					  });
			freqVec.emplace_back(wordIt, sortedIndexVec);
		}

		/**
		 * Вычисляем абсолютную релевантность каждого документа, создаём пару с id документа,
		 * собираем пары в вектор,
		 * а за одно получаем максимальное значение релевантности среди всех документов.
		*/
		size_t relevanceAbsMax = 0;
		std::vector<std::pair<size_t, size_t>> relevanceAbsVec;
		for (int id = 0; id < converterJson.GetTextDocuments().size(); ++id) {
			size_t relevanceAbsolute = 0;
			for (auto &d: freqVec) {
				for (auto &e: d.second) {
					if (e.docId == id) {
						relevanceAbsolute += e.count;
						if (relevanceAbsolute > relevanceAbsMax)
							relevanceAbsMax = relevanceAbsolute;
						break;
					}
				}
			}
			if (relevanceAbsolute > 0)
				relevanceAbsVec.emplace_back(id, relevanceAbsolute);
		}

		/**
		 * полученные абсолютные релевантности делим на максимальную релевантность по всем документам,
		 * создаём пару с id документа и собираем в вектор.
		*/
		for (auto& it : relevanceAbsVec) {
			relativeIndexVec.emplace_back(it.first, float(it.second) / float(relevanceAbsMax));
		}
		relativeIndexVecAllQueries.emplace_back(relativeIndexVec);

		// Сортируем индексы по убыванию (дорого, но документов, к счастью, не много, как правило)
		for (auto &idxVec: relativeIndexVecAllQueries) {
			for (int i = 0; i < idxVec.size(); ++i) {
				for (int j = 0; j < idxVec.size() - 1; ++j) {
					if (std::isless(idxVec[j].second, idxVec[j + 1].second)) {
						std::swap(idxVec[j], idxVec[j + 1]);
					}
				}
			}
		}
	}

	// Сохраняем результаты релевантности по всем запросам в файл "answers.json"
	converterJson.putAnswers(relativeIndexVecAllQueries);

	// Отбираем топ результатов релевантности (количество "max_responses" берём из файла config.json)
	for (auto & itVec : relativeIndexVecAllQueries) {
		std::vector<std::pair<size_t, float>> tempVec;
		int respLimit = converterJson.GetResponsesLimit();
		for (int j = 0; (j < itVec.size() && j < respLimit); ++j) {
			if (!isEqualFloat(itVec[j].second,0))
				tempVec.emplace_back(itVec[j]);
			else
				tempVec.emplace_back();
		}
		topRelativeIndexes.push_back(tempVec);
	}

	return topRelativeIndexes;
}
