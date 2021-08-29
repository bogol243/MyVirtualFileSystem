#pragma once
#include <fstream>
#include "common_structures.h"
#include "fd_tools.h"

class DataStorage {

	std::string filename_ = "datastorage.txt";
	std::fstream data_storage_stream_;
	size_t block_size_ =  1024;
	size_t end_block_ = 1;

public:

	DataStorage() = default;

	/*
	* std::string filename	-- им€ файла-хранилища области данных на диске
	* size_t block_size		-- размер блока данных (в байтах)
	*/
	DataStorage(std::string filename, size_t block_size, bool reinitialize = true);

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

	/*
	* ѕрочитать данные из файла, чтение будет производитьс€ с текущего положени€ указател€ чтени€ (fd->gpos)
	*/
	size_t Read(File* fd, char* buf, size_t buf_len);

	size_t ComputeBlockNumber(File* file);
};

