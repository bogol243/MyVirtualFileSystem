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
	* std::string filename	-- ��� �����-��������� ������� ������ �� �����
	* size_t block_size		-- ������ ����� ������ (� ������)
	*/
	DataStorage(std::string filename, size_t block_size, bool reinitialize = true);

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

	/*
	* ��������� ������ �� �����, ������ ����� ������������� � �������� ��������� ��������� ������ (fd->gpos)
	*/
	size_t Read(File* fd, char* buf, size_t buf_len);

	size_t ComputeBlockNumber(File* file);
};

