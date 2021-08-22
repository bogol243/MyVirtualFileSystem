#pragma once
#include <array>

const size_t N_BLOCKS = 10;
// Общие структуры
struct INode {
	size_t FILETYPE = 0; // 0 -- пустой inode, 1 --файл , 2 -- каталог
	//size_t ref_count; // количество имён
	size_t byte_size = 0;
	size_t blocks_count = 0;
	std::array<size_t, N_BLOCKS> data_blocks = { 0 };
};

struct File {
	size_t inode_id = 0;
	size_t gpos = 0;		//get pos
	size_t ppos = 0;		//put pos
	INode inode_obj;
};

struct DirRecord {
	size_t inode_id;
	char name[16];
};
