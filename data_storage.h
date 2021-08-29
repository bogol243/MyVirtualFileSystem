#pragma once
#include <fstream>
#include "common_structures.h"
#include "fd_tools.h"

class DataStorage {
	using ios = std::ios;

	std::string _filename = "datastorage.txt";
	std::fstream data_storage_stream;
	size_t _block_size =  1024;
	size_t _end_block = 1;

public:

	DataStorage() = default;

	/*
	* std::string filename	-- им€ файла-хранилища области данных на диске
	* size_t block_size		-- размер блока данных (в байтах)
	*/
	DataStorage(std::string filename, size_t block_size, bool reinitialize = true);

	/*
	* ќчистить файлы на диске перед инициализацией
	*/
	void Clear();

	/*
	* ѕолучить id следующего свободного блока в области данных
	*/
	size_t GetFreeBlock();

	/*
	* ƒописать данные в конец файла независимо от того на что указывал указатель записи
	*/
	size_t Append(File* fd, const char* data, size_t data_len);

	/*
	* «аписать данные в файл, запись будет производитьс€ с текущего положени€ указател€ записи (fd->ppos)
	*/
	size_t Write(File* fd, const char* data, size_t data_len);

	size_t Read(File* fd, char* buf, size_t buf_len);
};

