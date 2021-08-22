#pragma once
#include <fstream>
#include "common_structures.h"

class DataStorage {
	using ios = std::ios;

	std::string _filename = "datastorage.txt";
	size_t _block_size =  1024;
	size_t _end_block = 1;

public:

	DataStorage() = default;

	/*
	* std::string filename	-- имя файла-хранилища области данных на диске
	* size_t block_size		-- размер блока данных (в байтах)
	*/
	DataStorage(std::string filename, size_t block_size, bool reinitialize = true);

	void Clear();

	size_t GetFreeBlock();

	void Seekg(File* fd, size_t offset);

	void Seekp(File* fd, size_t offset);

	void SeekgEnd(File* fd);

	void SeekpEnd(File* fd);

	size_t Append(File* fd, const char* data, size_t data_len);

	size_t Write(File* fd, const char* data, size_t data_len);

	size_t Read(File* fd, char* buf, size_t buf_len);
};

