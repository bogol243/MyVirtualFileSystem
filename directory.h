//#pragma once
//#include <string>
//#include "common_structures.h"
//#include "data_storage.h"
//
//class Directory {
//	File* _fd;  // тут лежат данные директории
//public:
//	/*
//	* File* fd -- файловый дескриптор файла директории
//	*/
//	Directory(File* fd);
//
//	/* Добавить файл в директорию
//	* File* fd			-- файловый дескриптор
//	* std::string name	-- имя добавляемого файла
//	*/
//	void AddFile(File* new_file_fd, std::string name);
//
//	DirRecord* FindRecord(const char name[16]);
//
//	size_t GetINodeByName(const char name[16]);
//
//	friend std::ostream& operator<< (std::ostream& out, Directory dir);
//};
//
//std::ostream& operator<< (std::ostream& out, Directory dir);
