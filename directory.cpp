//#include "directory.h"
//
//	/*
//	* File* fd -- файловый дескриптор файла директории
//	*/
//	Directory::Directory(File* fd)
//		: _fd(fd) {
//
//	}
//
//	/* Добавить файл в директорию
//	* File* fd			-- файловый дескриптор
//	* std::string name	-- имя добавляемого файла
//	*/
//	void Directory::AddFile(File* new_file_fd, std::string name) {
//		DirRecord new_record;
//		new_record.inode_id = new_file_fd->inode_id;
//		std::memcpy(new_record.name, name.c_str(), name.size() + 1 < 16 ? name.size() + 1 : 16);
//		ds.Append(_fd, reinterpret_cast<char*>(&new_record), sizeof(DirRecord));
//	}
//
//	DirRecord* Directory::FindRecord(const char name[16]) {
//		DirRecord* dir_record = new DirRecord;
//
//		ds.Read(_fd, reinterpret_cast<char*>(dir_record), sizeof(DirRecord));
//
//		if (std::strcmp(dir_record->name, name) == 0) {
//			return dir_record;
//		}
//		delete dir_record;
//		return nullptr;
//	}
//
//	size_t Directory::GetINodeByName(const char name[16]) {
//		if (std::strcmp(name, "/") == 0) return 2; // root handle
//
//		auto record = FindRecord(name);
//		if (!record) return 0;
//		return record->inode_id;
//	}
//
//std::ostream& operator<< (std::ostream& out, Directory dir) {
//	DirRecord dir_record;
//	size_t bytes_read = 0;
//	while (ds.Read(dir._fd, reinterpret_cast<char*>(&dir_record), sizeof(DirRecord)) > 0) {
//		out << dir_record.name << " -> " << dir_record.inode_id << '\n';
//	}
//	ds.Seekg(dir._fd, 0);
//	return out;
//}
