#pragma once
#include "common_structures.h"
#include "ilist.h"
#include "data_storage.h"

#include <vector>
#include <unordered_map>
#include <stack>
#include <iostream>

class MYVFS {
	IList ilist_;
	DataStorage ds_;
	File* root_fd_;
public:
	MYVFS();

	MYVFS(const VFSSettings& settings);
	
	~MYVFS();
	/*
	* Записать данные в область данных
	*/
	size_t Write(File* fd, const char* data, size_t data_len);

	/*
	* Считать даные из области данных
	*/
	size_t Read(File* fd, char* buf, size_t buf_len);

	void AddFileToDir(File* file_fd, File* dir_fd, std::string filename);
	
	std::optional<DirRecord> FindRecordInDir(File* dir_fd, const char* name);

	File* GetFileByNameInDir(File* dir_fd, const char* name);

	File* CreateNewFile(Filetype filetype = Filetype::REGULAR_FILE);
	
	File* CreateNewDir();

	std::vector<std::string> SplitPath(const std::string& name);

	File* TranslateNameToFd(std::string name);

	File* MkFile(std::string name);

	void PrintTree(std::ostream& cout = std::cout);

	std::optional<DirRecord> GetNextRecord(File* dir_fd);

};
