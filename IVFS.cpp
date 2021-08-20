#include <iostream>
#include <iomanip>
#include <fstream>
#include <queue>
#include <string>
#include <array>
#include <sstream>
#include <string_view>

#include <cassert>
#include <cstring>

#include "ilist.h"
//
//#include <unordered_map>
//
//const bool TODO_NOFILE = false;
//
//// структура
//
//// Суперблок считывается при монтировании системы
//// суперблок :
//// * тип ФС
//// * размер в блоках
//// * размер массива inode
//// * список свободных блоков
//// * список свободных inode
//// 
//// * флаги
//// * размер блока
//
//
///* inode
//Файл -> индексный дескриптор
//	хранит всё, кроме содержимого и имени файла
//	* тип файла
//		* обычный файл
//		* каталог
//			* первые два байта -- номер inode
//			* оставшиеся 14 -- имя
//			* обязательно есть файлы . и .. ссылки на себя и на родителя
//			* корневой каталог
//				* номер inode = 2
//				* . -> 2
//				* .. -> 2
//			* трансляция имени файла
//				* преобразование имени в inode
//					* / -- от корня
//					* ./ -- от текущего каталога
//					* ' ' -- от текущего каталога
//	* количества жёстких ссылок -- имён // файл удаляется автоматически, когда количество имён станет равным нулю
//	* идентификаторы владельца и группы
//	* размер файла в байтах
//	* времена доступа
//	* массив адресов блоков данных [13]
//		* первые 10кб -- прямая адресация
//		* следующие 256кб -- косвенная адрессация. адрес косвенного блока
//		* двойной косвенный блок -- хранит 256 адресов косвенных блоков
//		* тройной косвенный блок -- хранит 256 адресов двойных косвенных блоков
//*/
//
///* массив inode :
//	имеет ограниченный размер (определяет какое количество файлов можно создать в файловой системе)
//
//
//*/
//
//// блоки хранения данных
//
//
//
//
//namespace TestTask
//{
//	// файловый дескриптор
//	struct File {
//		size_t inode;
//		size_t rofft;
//		size_t wofft;
//	}; // Вы имеете право как угодно задать содержимое этой структуры
// 
//
//	struct VFS{
//		size_t _num_buckets = 4; // максимальное количество физических файлов на диске
//		size_t chunk_size = 4096; // 4К
//
//		// Открыть файл в readonly режиме. Если нет такого файла - вернуть nullptr
//		File* Open(const char* name) {
//			if (TODO_NOFILE) {
//				return nullptr;
//			}
//
//		}
//
//		// Открыть или создать файл в writeonly режиме. Если нужно, то создать все нужные поддиректории, упомянутые в пути
//		File* Create(const char* name) {
//
//		}
//
//		// Прочитать данные из файла. Возвращаемое значение - сколько реально байт удалось прочитать
//		size_t Read(File* f, char* buff, size_t len) {
//
//		}
//		
//		// Записать данные в файл. Возвращаемое значение - сколько реально байт удалось записать
//		size_t Write(File* f, char* buff, size_t len) {
//
//		}
//		
//		// Закрыть файл	
//		void Close(File* f) {
//		}
//	};
//
//}
//
//void init_fs_bin(std::string name){
//	std::fstream file(name,std::ios::out | std::ios::binary);
//	uint32_t myuint = 1488;
//	file.write(reinterpret_cast<char*>(&myuint), sizeof(myuint)); // ideally, you should memcpy it to a char buffer.
//}
//
///* inode
//Файл -> индексный дескриптор
//	хранит всё, кроме содержимого и имени файла
//	* тип файла
//		* обычный файл
//		* каталог
//			* первые два байта -- номер inode
//			* оставшиеся 14 -- имя
//			* обязательно есть файлы . и .. ссылки на себя и на родителя
//			* корневой каталог
//				* номер inode = 2
//				* . -> 2
//				* .. -> 2
//			* трансляция имени файла
//				* преобразование имени в inode
//					* / -- от корня
//					* ./ -- от текущего каталога
//					* ' ' -- от текущего каталога
//	* количества жёстких ссылок -- имён // файл удаляется автоматически, когда количество имён станет равным нулю
//	* идентификаторы владельца и группы
//	* размер файла в байтах
//	* времена доступа
//	* массив адресов блоков данных [13]
//		* первые 10кб -- прямая адресация
//		* следующие 256кб -- косвенная адрессация. адрес косвенного блока
//		* двойной косвенный блок -- хранит 256 адресов косвенных блоков
//		* тройной косвенный блок -- хранит 256 адресов двойных косвенных блоков
//*/


// управлятор ilist-a
IList ilist("ilist.txt", 10, true);

// Подсистема DataStorage
class DataStorage {
	using ios = std::ios;

	std::string _filename;
	size_t _block_size;
	size_t _end_block = 1;
public:

	/*
	* std::string filename	-- имя файла-хранилища области данных на диске
	* size_t block_size		-- размер блока данных (в байтах)
	*/
	DataStorage(std::string filename, size_t block_size, bool reinitialize = true)
		: _filename(filename)
		, _block_size(block_size) {
		if (reinitialize) {
			Clear();
		}
	}

	void Clear() {
		std::fstream data_storage_stream(_filename, std::ios::out | std::ios::binary | std::ios::trunc);
	}

	size_t GetFreeBlock() {
		return _end_block++;
	}

	void Seekg(File* fd, size_t offset) {
		fd->gpos = offset;
	}

	void Seekp(File* fd, size_t offset) {
		fd->ppos = offset;
	}

	void SeekgEnd(File* fd) {
		fd->gpos = fd->inode_obj.byte_size;
	}

	void SeekpEnd(File* fd) {
		fd->ppos = fd->inode_obj.byte_size;
	}

	size_t Append(File* fd, const char* data, size_t data_len) {
		SeekpEnd(fd);
		size_t bytes_written = Write(fd, data, data_len);
		Seekp(fd, 0);
		return bytes_written;
	}

	size_t Write(File* fd, const char* data, size_t data_len) {
		std::ofstream data_storage_stream(_filename, ios::in | ios::binary);
		auto& data_blocks = fd->inode_obj.data_blocks;
		size_t blocks_count = fd->inode_obj.blocks_count;

		size_t current_block = data_blocks[fd->ppos / _block_size];
		size_t write_count = 0;

		data_storage_stream.seekp(current_block * _block_size + (fd->ppos % _block_size));

		for (size_t i = fd->ppos; i < fd->ppos + data_len; ++i) {
			if (i % _block_size == 0) { // дошли до конца выданного блока
				current_block = GetFreeBlock();
				data_blocks[fd->inode_obj.blocks_count++] = current_block;
				// смещаемся на (номер блока)*(размер блока)
				data_storage_stream.seekp(current_block * _block_size);
			}
			//data_storage_stream.seekp(current_block * _block_size + i % _block_size);
			data_storage_stream.put(data[write_count++]);
			data_storage_stream.flush();
		}

		//sync INode state (rewrite old)
		fd->inode_obj.byte_size += write_count;
		fd->ppos += write_count;
		ilist.WriteINode(fd->inode_id, fd->inode_obj);
		
		data_storage_stream.flush();
		return write_count;
	}

	size_t Read(File* fd, char* buf, size_t buf_len) {
		std::ifstream data_storage_stream(_filename, ios::binary);
		auto& data_blocks = fd->inode_obj.data_blocks;

		
		size_t read_count = 0;

		size_t block_number = fd->gpos / _block_size;
		size_t current_block = data_blocks[block_number];

		data_storage_stream.seekg(current_block * _block_size + (fd->gpos % _block_size));

		for (size_t i = fd->gpos; read_count < buf_len && i < fd->inode_obj.byte_size; ++i) {
			if (i % _block_size == 0) {
				block_number = i / _block_size;
				current_block = data_blocks[block_number];
				// смещаемся на (номер блока)*(размер блока)
				data_storage_stream.seekg(current_block * _block_size);
			}
			data_storage_stream.get(buf[read_count++]);
		}
		fd->gpos += read_count;
		return read_count;
	}
};

DataStorage ds("datastorage.txt", 32);

// Подсистема директорий и трансляции имён. 
// Не привязана ни к какому файлу на диске. Данные директорий храняться в vfs как файлы.

/*
* Представляет запись в директории
*/
struct DirRecord {
	size_t inode_id;
	char name[16];
};

class Directory {
	File* _fd;  // тут лежат данные директории
public:
	/*
	* File* fd -- файловый дескриптор файла директории
	*/
	Directory(File* fd)
		: _fd(fd) {

	}

	/* Добавить файл в директорию
	* File* fd			-- файловый дескриптор
	* std::string name	-- имя добавляемого файла
	*/
	void AddFile(File* new_file_fd, std::string name) {
		DirRecord new_record;
		new_record.inode_id = new_file_fd->inode_id;
		std::memcpy(new_record.name, name.c_str(), name.size()+1 < 16 ? name.size()+1 : 16);
		ds.Append(_fd, reinterpret_cast<char*>(&new_record), sizeof(DirRecord));
		
	}

	DirRecord* FindRecord(const char name[16]) {
		DirRecord* dir_record = new DirRecord;

		ds.Read(_fd, reinterpret_cast<char*>(dir_record), sizeof(DirRecord));
		
		if (std::strcmp(dir_record->name, name) == 0) {
			return dir_record;
		}
		delete dir_record;
		return nullptr;
	}

	size_t GetINodeByName(const char name[16]) {
		if (std::strcmp(name, "/") == 0) return 2; // root handle

		auto record = FindRecord(name);
		if (!record) return 0;
		return record->inode_id;
	}

	friend std::ostream& operator<< (std::ostream& out, Directory dir);
};

std::ostream& operator<< (std::ostream& out, Directory dir) {
	DirRecord dir_record;
	size_t bytes_read = 0;
	while (ds.Read(dir._fd, reinterpret_cast<char*>(&dir_record), sizeof(DirRecord)) > 0) {
		out << dir_record.name << " -> " << dir_record.inode_id << '\n';
	}
	ds.Seekg(dir._fd, 0);
	return out;
}

// Собираем всё вместе

File* GetFile(size_t FILETYPE = 1) {
	File* new_file = new File;
	auto node_and_id = ilist.GetFreeINode();
	auto new_node_ptr = node_and_id.first;
	auto new_node_id = node_and_id.second;
	
	if (!new_node_ptr) return nullptr;

	new_file->inode_obj = *new_node_ptr;
	new_file->inode_id = new_node_id;

	new_file->inode_obj.FILETYPE = FILETYPE;

	ilist.WriteINode(new_file->inode_id, new_file->inode_obj);

	return new_file;
}

File* GetDir() {
	return GetFile(2);
}

File* root_fd = GetDir();

std::vector<std::string> SplitPath(const std::string& name) {
	size_t pos = name.find('/');
	std::vector<std::string> dirs;
	while (pos != name.npos) {
		size_t next_pos = name.find('/', pos + 1);
		dirs.push_back(name.substr(pos + 1, next_pos - pos - 1));
		pos = next_pos;
	}
	return dirs;
}


size_t TranslateNameToINodeId(std::string name) {
	std::vector<std::string> dirs = SplitPath(name);

	Directory* dir_ptr = new Directory(root_fd);
	File* file_fd = nullptr;
	for (const auto& name_part : dirs) {
		auto inode_id = dir_ptr->GetINodeByName(name_part.c_str());
		if (inode_id < 3) /*всё плохо*/ return 0;
		delete dir_ptr;
		file_fd = ilist.GetFile(inode_id);
		if (!file_fd) return 0;
		dir_ptr = new Directory(file_fd);
	}
	return file_fd ? file_fd->inode_id : 0;
}

File* MkFile(std::string name) {
	auto path = SplitPath(name);
	std::string file_name = path.back();
	path.pop_back();

	Directory* dir_ptr = new Directory(root_fd);
	File* dir_fd = nullptr;

	// проходим по всем директориям, если они есть идём дальше, если нет, создаём новые
	for (const auto& name_part : path) {
		auto inode_id = dir_ptr->GetINodeByName(name_part.c_str());
		
		if (inode_id < 3) { // не нашли 
			dir_fd = GetDir(); // создаём в этой директории файл
			dir_ptr->AddFile(dir_fd, name_part);	
		}
		else {
			dir_fd = ilist.GetFile(inode_id);
			if (!dir_fd) return 0;
		}

		delete dir_ptr;
		dir_ptr = new Directory(dir_fd);
	}
	
	File* new_file = nullptr;
	if (dir_ptr) {
		new_file = GetFile();
		if (!new_file) return nullptr;
		dir_ptr->AddFile(new_file,file_name);
	}
	
	return new_file;
}



// тесты



void test_name_translator() {
	Directory root(root_fd);
	
	auto test_fd = GetDir();
	Directory test(test_fd);

	auto test2_fd = GetDir();
	Directory test2(test2_fd);

	File* file_fd = GetFile();
	
	root.AddFile(test_fd,"test");
	test.AddFile(test2_fd, "test2");
	test2.AddFile(file_fd, "file.txt");
	
	size_t inode_id = TranslateNameToINodeId("/test/test2/file.txt");
}

void test_dir_find_record() {
	File* directory_fd = GetDir();
	Directory dir(directory_fd);

	File* test_file_fd1 = GetFile();
	File* test_file_fd2 = GetFile();
	File* test_file_fd3 = GetFile();

	
	dir.AddFile(test_file_fd1, "file1");

	dir.AddFile(test_file_fd2, "file2");

	dir.AddFile(test_file_fd3, "file3");


	size_t inode_id = dir.GetINodeByName("file1");
}


void test_dir() {
	File* test_dir_file = GetFile(2);

	Directory dir(test_dir_file);
	
	File* test_f1 = GetFile();
	dir.AddFile(test_f1, "file1");
	std::string t1_str = "test string to test multiple splitted files.";
	ds.Write(test_f1, t1_str.c_str(), t1_str.size());

	File* test_f2 = GetFile();
	std::string t2_str = "Some stuff from file2";
	ds.Write(test_f2, t2_str.c_str(), t2_str.size());
	dir.AddFile(test_f2, "file2");

	std::string t1_str2 = " Other stuff from file1";
	ds.Write(test_f1, t1_str2.c_str(), t1_str2.size());

	char buf[200];
	size_t bytes_read = ds.Read(test_f1, buf, 200);
	std::string t1_res_str(buf, bytes_read);

	bytes_read = ds.Read(test_f2, buf, 200);
	std::string t2_res_str(buf, bytes_read);

	std::cout << dir;
}

void test_data_storage_smoke() {
	DataStorage ds("datastorage.txt", 32);
	File* test_f = new File;
	auto node_and_id = ilist.GetFreeINode();
	auto new_node_ptr = node_and_id.first;
	auto new_node_id = node_and_id.second;

	if (new_node_ptr) {
		test_f->inode_obj = *new_node_ptr;
		test_f->inode_id = new_node_id;
	}
	
	std::string test_data = "heres some test data to write into data_storage";
	ds.Write(test_f, test_data.c_str(), test_data.size());

	char buffer[60];
	auto bytes_read = ds.Read(test_f, buffer, 60);

	std::string res(buffer,bytes_read);
	
	assert(test_data == res);
	std::cout << "res: " << res << '\n';
	std::cout << ilist;
}

void test_random_access_read() {
	DataStorage ds("datastorage.txt", 32);
	File* test_f = new File;
	auto node_and_id = ilist.GetFreeINode();
	auto new_node_ptr = node_and_id.first;
	auto new_node_id = node_and_id.second;

	if (new_node_ptr) {
		test_f->inode_obj = *new_node_ptr;
		test_f->inode_id = new_node_id;
	}

	std::string test_data = "heres some test data to write into data_storage";
	ds.Write(test_f, test_data.c_str(), test_data.size());

	test_data = test_data.substr(40);
	char buffer[60];

	ds.Seekg(test_f, 40);
	auto bytes_read = ds.Read(test_f, buffer, 60);

	std::string res(buffer, bytes_read);

	assert(test_data == res);
	std::cout << "res: " << res << '\n';
	std::cout << ilist;
}

void test_append() {
	DataStorage ds("datastorage.txt", 32);
	File* test_f = new File;
	auto node_and_id = ilist.GetFreeINode();
	auto new_node_ptr = node_and_id.first;
	auto new_node_id = node_and_id.second;

	if (new_node_ptr) {
		test_f->inode_obj = *new_node_ptr;
		test_f->inode_id = new_node_id;
	}

	std::string test_data = "heres some test data to write into data_storage";
	ds.Write(test_f, test_data.c_str(), test_data.size());

	std::string appendix = " ,bitchass nigga";
	ds.Append(test_f, appendix.c_str(), appendix.size());

	test_data = test_data + appendix;
	char buffer[100];
	auto bytes_read = ds.Read(test_f, buffer, 100);

	std::string res(buffer, bytes_read);

	assert(test_data == res);
	std::cout << "res: " << res << '\n';
	std::cout << ilist;
}

void test_ilist() {
	IList ilist("ilist.txt", 10, true);


	std::cout << ilist;
}

int main() {
	//test_data_storage_smoke();
	//test_random_access_read();
	//test_append();
	//test_dir();
	//test_dir_find_record();
	test_name_translator();
	return 0;
}