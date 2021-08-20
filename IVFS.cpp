#include <iostream>
#include <iomanip>
#include <fstream>
#include <queue>
#include <string>
#include <array>
#include <sstream>
#include <string_view>

#include <cassert>
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

// Общие константы
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

// Подсистема ilist
// 
// класс для работы со списком inode -- ilist
class IList {
	std::string _filename;
	size_t _size;
public:
	/*
		* std::string filename	-- имя файла-хранилища ilist на диске
		* size_t size			-- размер ilist
		* bool reinitialize		-- флаг обнуления существующего файла
	*/
	IList(std::string filename, size_t size, bool reinitialize = true)
		: _filename(filename)
		, _size(size)
	{
		if (reinitialize){
			Clear();

			for (size_t id = 0; id < _size; ++id) {
				INode empty_node;
				WriteINode(id, empty_node);
			}
		}
	}

	/* Получить дескриптор файла по его inode id
		* size_t inode_id -- 
	*/
	File* GetFile(size_t inode_id) {
		File* fd = new File;
		
		// прочитать нужный INode
		INode* inode_obj = ReadINode(inode_id);
		if (!inode_obj) return nullptr;

		// построить из него файловый дескриптор
		fd->inode_id = inode_id;
		fd->inode_obj = *inode_obj;
		
		
		return fd;
	}
	
	/* Прочитать содержимое inode с идентификатором inode_id в объект INode
		* size_t inode_id --
	*/
	INode* ReadINode(size_t inode_id) {
		// открыть файл
		std::ifstream _ilist_stream(_filename, std::ios::binary);

		if (inode_id > _size) { // если inode_id больше чем размер ilist
			return nullptr;
		}

		// сдвигаем указатель на нужный inode
		_ilist_stream.seekg(sizeof(INode) * inode_id);

		// читаем в объект inode
		INode* inode_obj = new INode;
		_ilist_stream.read(reinterpret_cast<char*>(inode_obj), sizeof(INode));
		return inode_obj;
	}

	/* Записать новое значение inode для заданного inode_id
		* size_t inode_id --
		* INode inode_obj --
	*/
	bool WriteINode(size_t inode_id, INode inode_obj) {
		std::ofstream _ilist_stream(_filename, std::ios::in | std::ios::binary);

		if (inode_id > _size) { // если inode_id больше чем размер ilist
			return false;
		}
		// сдвигаем указатель на нужный inode
		_ilist_stream.seekp(sizeof(INode) * inode_id);
		// пишем объект inode в файл
		_ilist_stream.write(reinterpret_cast<char*>(&inode_obj), sizeof(INode));
		_ilist_stream.flush();
		return true;
	}

	void Clear() {
		std::fstream ilist_stream(_filename, std::ios::out | std::ios::binary | std::ios::trunc);
	}

	/*
	* Возвращает INode* на свободную inode.
	* Если такой нет, возвращает nullptr;
	*/
	std::pair<INode*,size_t> GetFreeINode() {
		for (size_t inode_id = 2; inode_id < _size; ++inode_id) {
			INode* inode = ReadINode(inode_id);
			if (inode && inode->FILETYPE == 0) return { inode, inode_id};
		}
		return { nullptr,0 };
	}

	friend std::ostream& operator<<(std::ostream& out, IList& ilist);
};

std::ostream& operator<<(std::ostream& out, const INode& inode) {
	out << "type: " << inode.FILETYPE << " | "
		<< "byte_size: " << inode.byte_size << " | "
		<< "blocks_count: " << inode.blocks_count << " | "
		<< "blocks: ";
	for (size_t block_id = 0; block_id < N_BLOCKS; ++block_id) {
		out << inode.data_blocks[block_id] << ' ';
	}
	return out;
}

std::ostream& operator<<(std::ostream& out, IList& ilist) {
	for (size_t id = 0; id < ilist._size; ++id) {
		INode* inode_obj = ilist.ReadINode(id);
		if (!inode_obj) break;
		out << *inode_obj << '\n';
	}
	return out;
}


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
		ilist.WriteINode(fd->inode_id, fd->inode_obj);
		
		data_storage_stream.flush();
		return write_count;
	}

	size_t Read	(File* fd, char* buf, size_t buf_len) {
		std::ifstream data_storage_stream(_filename, ios::binary);
		auto& data_blocks = fd->inode_obj.data_blocks;

		
		size_t read_count = 0;

		size_t block_number = fd->gpos / _block_size;
		size_t current_block = data_blocks[block_number];

		data_storage_stream.seekg(current_block * _block_size + (fd->gpos % _block_size));

		for (size_t i = fd->gpos; i < buf_len && i < fd->inode_obj.byte_size; ++i) {
			if (i % _block_size == 0) {
				block_number = i / _block_size;
				current_block = data_blocks[block_number];
				// смещаемся на (номер блока)*(размер блока)
				data_storage_stream.seekg(current_block * _block_size);
			}
			data_storage_stream.get(buf[read_count++]);
		}

		return read_count;
	}
};


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
};

//class Directories {
//public:
//	
//	/*
//	* Создание корневой директории
//	*/
//	void CreateRoot() {
//
//	}
//};

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
	test_append();
	return 0;
}