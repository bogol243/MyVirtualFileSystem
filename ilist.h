#pragma once
#include <string>
#include <array>
#include <fstream>
#include <optional>

#include "common_structures.h"


class IList {
	std::string filename_ = "ilist.txt";
	std::fstream ilist_stream_;
	size_t size_ = 10;
public:
	IList();

	/*
		* std::string filename	-- ��� �����-��������� ilist �� �����
		* size_t size			-- ������ ilist
		* bool reinitialize		-- ���� ��������� ������������� �����
	*/
	IList(std::string filename, size_t size, bool reinitialize = true);

	/* �������� ���������� ����� �� ��� inode id
		* size_t inode_id --
	*/
	File* GetFile(size_t inode_id);

	/* ��������� ���������� inode � ��������������� inode_id � ������ INode
		* size_t inode_id --
	*/
	std::optional<INode> ReadINode(size_t inode_id);

	/* �������� ����� �������� inode ��� ��������� inode_id
		* size_t inode_id --
		* INode inode_obj --
	*/
	bool WriteINode(size_t inode_id, INode inode_obj);

	/*
	* ���������� id ��������� inode.
	* ���� ����� ���, ���������� 0
	*/
	size_t GetFreeINode();

	friend std::ostream& operator<<(std::ostream& out, IList& ilist);
};

std::ostream& operator<<(std::ostream& out, const INode& inode);

std::ostream& operator<<(std::ostream& out, IList& ilist);