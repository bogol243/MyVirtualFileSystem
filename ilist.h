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
		* std::string filename	-- имя файла-хранилища ilist на диске
		* size_t size			-- размер ilist
		* bool reinitialize		-- флаг обнуления существующего файла
	*/
	IList(std::string filename, size_t size, bool reinitialize = true);

	/* Получить дескриптор файла по его inode id
		* size_t inode_id --
	*/
	File* GetFile(size_t inode_id);

	/* Прочитать содержимое inode с идентификатором inode_id в объект INode
		* size_t inode_id --
	*/
	std::optional<INode> ReadINode(size_t inode_id);

	/* Записать новое значение inode для заданного inode_id
		* size_t inode_id --
		* INode inode_obj --
	*/
	bool WriteINode(size_t inode_id, INode inode_obj);

	/*
	* Возвращает id свобойной inode.
	* Если такой нет, возвращает 0
	*/
	size_t GetFreeINode();

	friend std::ostream& operator<<(std::ostream& out, IList& ilist);
};

std::ostream& operator<<(std::ostream& out, const INode& inode);

std::ostream& operator<<(std::ostream& out, IList& ilist);