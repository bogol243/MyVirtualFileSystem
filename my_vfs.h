#pragma once
#include "common_structures.h"
#include "ilist.h"
#include "data_storage.h"

#include <vector>
#include <unordered_map>
#include <stack>
#include <iostream>

class MYVFS {
	IList _ilist;
	DataStorage _ds;
	File* root_fd;
public:
	MYVFS();

	MYVFS(VFSSettings settings);
	
	~MYVFS();
	/*
	* Записать данные в область данных
	*/
	size_t Write(File* fd, const char* data, size_t data_len);

	/*
	* Считать даные из области данных
	*/
	size_t Read(File* fd, char* buf, size_t buf_len) const;

	void AddFileToDir(File* file_fd, File* dir_fd, std::string filename);
	
	std::optional<DirRecord> FindRecordInDir(File* dir_fd, const char name[16]);

	File* GetFileByNameInDir(File* dir_fd, const char name[16]);

	File* CreateNewFile(size_t FILETYPE=1);
	
	File* CreateNewDir(File* parent_dir);

	std::vector<std::string> SplitPath(const std::string& name);

	File* TranslateNameToFd(std::string name);

	File* MkFile(std::string name);

	void PrintTree(std::ostream& cout = std::cout);

	std::optional<DirRecord> GetNextRecord(File* dir_fd);

};
