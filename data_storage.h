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
	* std::string filename	-- ��� �����-��������� ������� ������ �� �����
	* size_t block_size		-- ������ ����� ������ (� ������)
	*/
	DataStorage(std::string filename, size_t block_size, bool reinitialize = true);

	/*
	* �������� ����� �� ����� ����� ��������������
	*/
	void Clear();

	/*
	* �������� id ���������� ���������� ����� � ������� ������
	*/
	size_t GetFreeBlock();

	/*
	* �������� ������ � ����� ����� ���������� �� ���� �� ��� �������� ��������� ������
	*/
	size_t Append(File* fd, const char* data, size_t data_len);

	/*
	* �������� ������ � ����, ������ ����� ������������� � �������� ��������� ��������� ������ (fd->ppos)
	*/
	size_t Write(File* fd, const char* data, size_t data_len);

	size_t Read(File* fd, char* buf, size_t buf_len);
};

