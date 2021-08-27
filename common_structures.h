#pragma once
#include <array>
#include <mutex>
#include <string>

const size_t N_DATA_BLOCKS = 1000;
// Общие структуры

struct VFSSettings {
	size_t block_size = 256;
	std::string ilist_filename = "ilist.txt";
	std::string datastorage_filename = "datastorage.txt";
	size_t ilist_capacity = 10;
	bool reinitialize = false;
	
	VFSSettings& SetBlockSize(size_t block_size) {
		this->block_size = block_size;
		return *this;
	}

	VFSSettings& SetIListFilename(std::string ilist_filename) {
		this->ilist_filename = ilist_filename;
		return *this;
	}

	VFSSettings& SetDataStorageFilename(std::string datastorage_filename) {
		this->datastorage_filename = datastorage_filename;
		return *this;
	}

	VFSSettings& SetIListCapacity (size_t ilist_capacity) {
		this->ilist_capacity = ilist_capacity;
		return *this;
	}

	VFSSettings& SetReinitialize(bool reinitialize) {
		this->reinitialize = reinitialize;
		return *this;
	}
};

struct INode {
	size_t FILETYPE = 0; // 0 -- пустой inode, 1 --файл , 2 -- каталог
	//size_t ref_count; // количество имён
	size_t byte_size = 0;
	size_t blocks_count = 0;
	std::array<size_t, N_DATA_BLOCKS> data_blocks = { 0 };
};

enum class OpenMode {
	RDONLY,
	WRONLY,
	RDWR,
	CLOSED
};

struct File {
	size_t inode_id = 0;
	size_t gpos = 0;		//get pos
	size_t ppos = 0;		//put pos
	OpenMode openmode = OpenMode::CLOSED;
	INode inode_obj;
};

struct DirRecord {
	size_t inode_id;
	char name[16];
};
