#pragma once
#include <string>
#include <array>
#include <fstream>
#include <optional>

#include "common_structures.h"


class IList {
	std::string _filename = "ilist.txt";
	size_t _size = 10;
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
	INode* ReadINode(size_t inode_id);

	/* Записать новое значение inode для заданного inode_id
		* size_t inode_id --
		* INode inode_obj --
	*/
	bool WriteINode(size_t inode_id, INode inode_obj);

	void Clear();

	/*
	* Возвращает INode* на свободную inode.
	* Если такой нет, возвращает nullptr;
	*/
	size_t/*std::pair<INode*, size_t>*/ GetFreeINode();

	friend std::ostream& operator<<(std::ostream& out, IList& ilist);
};

std::ostream& operator<<(std::ostream& out, const INode& inode);

std::ostream& operator<<(std::ostream& out, IList& ilist);