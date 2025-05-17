#pragma once
#include <string>
#include <vector>
#include "csv/rapidcsv.h"
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct
{
    char* string;
    size_t size;
} Response;

struct GoogleDriveCSV {
	std::string Link;
	std::string DownloadPath;
};


inline static size_t write_chunk(void* data, size_t size, size_t nemb, void* userdata)
{
    size_t real_size = size * nemb;

    Response* response = (Response*)userdata;

    char* ptr = (char*)realloc(response->string, response->size + real_size + 1);

    if (ptr == NULL)
    {
        return CURLE_WRITE_ERROR;
    }

    response->string = ptr;

    memcpy(&(response->string[response->size]), data, real_size);

    response->size += real_size;

    response->string[response->size] = '\0';

    return real_size;
}


inline static int download(const GoogleDriveCSV& csv)
{
    CURL* curl;
    CURLcode result;



    curl = curl_easy_init();

    if (curl == NULL)
    {
        std::cout << "Bastard" << std::endl;
        return -1;
    }

    //init response struct
    Response response;
    response.string = (char*)malloc(1);
    response.size = 0;

    //set url
    curl_easy_setopt(curl, CURLOPT_URL, csv.Link.c_str());

    //set function and write location for data
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_chunk);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);

    result = curl_easy_perform(curl);

    char* location;
    result = curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &location);

    while ((result == CURLE_OK) && location)
    {
        free(response.string);

        response.string = (char*)malloc(1);
        response.size = 0;

        curl_easy_setopt(curl, CURLOPT_URL, location);

        result = curl_easy_perform(curl);

        result = curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &location);
    }

    if (result != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
    }

    curl_easy_cleanup(curl);

    std::ofstream file(csv.DownloadPath);
	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << csv.DownloadPath << std::endl;
		free(response.string);
		return -1;
	}

    file << response.string;

    free(response.string);

    return 0;
}


inline static GoogleDriveCSV NameToCSV(std::string Name) {
	GoogleDriveCSV csv;
	if (Name == "Pinatas") {
		csv.Link = "https://docs.google.com/spreadsheets/d/1aDNqIG20d9SmkZbNmC2aXLRLQkQv_o7VF3wBTgdVj24/edit?gid=0#gid=0";
		csv.DownloadPath = "Data\\Sheets\\Pinata\\Pinata.csv";
	}
	else if (Name == "Items") {
		csv.Link = "https://docs.google.com/spreadsheets/d/1WVXzygv90DXesOcSY4VYa7jxFyLSHaZNUJKSryXlIu0/edit?gid=0#gid=0";
		csv.DownloadPath = "Data\\Sheets\\Items\\Items.csv";
	}
	else if (Name == "Areas") {
		csv.Link = "https://docs.google.com/spreadsheets/d/1u8JaVB85Y7yddWttgP13swhsC_StUIsEUUpXcjKajqE/edit?gid=0#gid=0";
		csv.DownloadPath = "Data\\Sheets\\Areas\\Areas.csv";
	}
	else if (Name == "Mainland") {
		csv.Link = "https://docs.google.com/spreadsheets/d/1APwml4ieV2fQPjtr7BMaeHFGpGEZLoxmrfRw6cNcxbc/edit?gid=273423548#gid=273423548";
		csv.DownloadPath = "Data\\Sheets\\Terrain\\Mainland.csv";
	}
	else if (Name == "Arctic") {
		csv.Link = "https://docs.google.com/spreadsheets/d/1APwml4ieV2fQPjtr7BMaeHFGpGEZLoxmrfRw6cNcxbc/edit?gid=0#gid=0";
		csv.DownloadPath = "Data\\Sheets\\Terrain\\Arctic.csv";
	}
	else if (Name == "Desert") {
		csv.Link = "https://docs.google.com/spreadsheets/d/1APwml4ieV2fQPjtr7BMaeHFGpGEZLoxmrfRw6cNcxbc/edit?gid=2118371872#gid=2118371872";
		csv.DownloadPath = "Data\\Sheets\\Terrain\\Desert.csv";
	}
	else if (Name == "Fresh-Water") {
		csv.Link = "https://docs.google.com/spreadsheets/d/1APwml4ieV2fQPjtr7BMaeHFGpGEZLoxmrfRw6cNcxbc/edit?gid=1340074649#gid=1340074649";
		csv.DownloadPath = "Data\\Sheets\\Terrain\\Fresh-Water.csv";
	}
	else if (Name == "Jungle") {
		csv.Link = "https://docs.google.com/spreadsheets/d/1APwml4ieV2fQPjtr7BMaeHFGpGEZLoxmrfRw6cNcxbc/edit?gid=998770695#gid=998770695";
		csv.DownloadPath = "Data\\Sheets\\Terrain\\Jungle.csv";
	}
	else if (Name == "Ocean") {
		csv.Link = "https://docs.google.com/spreadsheets/d/1APwml4ieV2fQPjtr7BMaeHFGpGEZLoxmrfRw6cNcxbc/edit?gid=1776671493#gid=1776671493";
		csv.DownloadPath = "Data\\Sheets\\Terrain\\Ocean.csv";
	}
	else if (Name == "Trench-Abyss") {
		csv.Link = "https://docs.google.com/spreadsheets/d/1APwml4ieV2fQPjtr7BMaeHFGpGEZLoxmrfRw6cNcxbc/edit?gid=1424423500#gid=1424423500";
		csv.DownloadPath = "Data\\Sheets\\Terrain\\Trench-Abyss.csv";
	}

	return csv;
}

namespace csv {

	//Will only get items from the column with the index
	inline static std::vector<std::string> Load(std::string Name, int ColumnIndex) {
		GoogleDriveCSV csv = NameToCSV(Name);

		//download
		download(csv);

		rapidcsv::Document doc(csv.DownloadPath);
		std::vector<std::string> ColumnData = doc.GetColumn<std::string>(ColumnIndex);
		return ColumnData;
	}

	//Will only get items from the column with the name
	inline static std::vector<std::string> Load(std::string Name, std::string ColumnName) {
		GoogleDriveCSV csv = NameToCSV(Name);

		//download
		download(csv);

		rapidcsv::Document doc(csv.DownloadPath);
		std::vector<std::string> ColumnData = doc.GetColumn<std::string>(ColumnName);
		return ColumnData;
	}

	//Will only get items from first column if Filter Column contains the Filter String on the same row
	inline static std::vector<std::string> Load(std::string Name, int FilterColumnIndex, std::string FilterString) {
		GoogleDriveCSV csv = NameToCSV(Name);

		//download
		download(csv);

		rapidcsv::Document doc(csv.DownloadPath);
		std::vector<std::string> ColumnData = doc.GetColumn<std::string>(FilterColumnIndex);
		std::vector<std::string> FilteredData;
		for (size_t i = 0; i < ColumnData.size(); ++i) {
			if (ColumnData[i] == FilterString) {
				FilteredData.push_back(doc.GetCell<std::string>(0, i));
			}
		}
		return FilteredData;
	}

}