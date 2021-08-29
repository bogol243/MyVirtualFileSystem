#pragma once
#include <array>
#include <mutex>
#include <string>

const size_t N_DATA_BLOCKS = 1000;
// Общие структуры

struct VFSSettings {
	size_t block_size_ = 256;
	std::string ilist_filename_ = "ilist.txt";
	std::string datastorage_filename_ = "datastorage.txt";
	size_t ilist_capacity_ = 10;
	bool reinitialize_ = false;
	
	VFSSettings& SetBlockSize(size_t block_size) {
		block_size_ = block_size;
		return *this;
	}

	VFSSettings& SetIListFilename(std::string ilist_filename) {
		ilist_filename_ = ilist_filename;
		return *this;
	}

	VFSSettings& SetDataStorageFilename(std::string datastorage_filename) {
		datastorage_filename_ = datastorage_filename;
		return *this;
	}

	VFSSettings& SetIListCapacity (size_t ilist_capacity) {
		ilist_capacity_ = ilist_capacity;
		return *this;
	}

	VFSSettings& SetReinitialize(bool reinitialize) {
		reinitialize_ = reinitialize;
		return *this;
	}
};

enum class Filetype {
	EMPTY,
	REGULAR_FILE,
	DIRECTORY
};

struct INode {
	Filetype filetype = Filetype::EMPTY;
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

	void SetOpenMode(OpenMode mode){
		openmode = mode;
	}
};

struct DirRecord {
	size_t inode_id;
	char name[16];
};
