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
//// ���������
//
//// ��������� ����������� ��� ������������ �������
//// ��������� :
//// * ��� ��
//// * ������ � ������
//// * ������ ������� inode
//// * ������ ��������� ������
//// * ������ ��������� inode
//// 
//// * �����
//// * ������ �����
//
//
///* inode
//���� -> ��������� ����������
//	������ ��, ����� ����������� � ����� �����
//	* ��� �����
//		* ������� ����
//		* �������
//			* ������ ��� ����� -- ����� inode
//			* ���������� 14 -- ���
//			* ����������� ���� ����� . � .. ������ �� ���� � �� ��������
//			* �������� �������
//				* ����� inode = 2
//				* . -> 2
//				* .. -> 2
//			* ���������� ����� �����
//				* �������������� ����� � inode
//					* / -- �� �����
//					* ./ -- �� �������� ��������
//					* ' ' -- �� �������� ��������
//	* ���������� ������ ������ -- ��� // ���� ��������� �������������, ����� ���������� ��� ������ ������ ����
//	* �������������� ��������� � ������
//	* ������ ����� � ������
//	* ������� �������
//	* ������ ������� ������ ������ [13]
//		* ������ 10�� -- ������ ���������
//		* ��������� 256�� -- ��������� ����������. ����� ���������� �����
//		* ������� ��������� ���� -- ������ 256 ������� ��������� ������
//		* ������� ��������� ���� -- ������ 256 ������� ������� ��������� ������
//*/
//
///* ������ inode :
//	����� ������������ ������ (���������� ����� ���������� ������ ����� ������� � �������� �������)
//
//
//*/
//
//// ����� �������� ������
//
//
//
//
//namespace TestTask
//{
//	// �������� ����������
//	struct File {
//		size_t inode;
//		size_t rofft;
//		size_t wofft;
//	}; // �� ������ ����� ��� ������ ������ ���������� ���� ���������
// 
//
//	struct VFS{
//		size_t _num_buckets = 4; // ������������ ���������� ���������� ������ �� �����
//		size_t chunk_size = 4096; // 4�
//
//		// ������� ���� � readonly ������. ���� ��� ������ ����� - ������� nullptr
//		File* Open(const char* name) {
//			if (TODO_NOFILE) {
//				return nullptr;
//			}
//
//		}
//
//		// ������� ��� ������� ���� � writeonly ������. ���� �����, �� ������� ��� ������ �������������, ���������� � ����
//		File* Create(const char* name) {
//
//		}
//
//		// ��������� ������ �� �����. ������������ �������� - ������� ������� ���� ������� ���������
//		size_t Read(File* f, char* buff, size_t len) {
//
//		}
//		
//		// �������� ������ � ����. ������������ �������� - ������� ������� ���� ������� ��������
//		size_t Write(File* f, char* buff, size_t len) {
//
//		}
//		
//		// ������� ����	
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
//���� -> ��������� ����������
//	������ ��, ����� ����������� � ����� �����
//	* ��� �����
//		* ������� ����
//		* �������
//			* ������ ��� ����� -- ����� inode
//			* ���������� 14 -- ���
//			* ����������� ���� ����� . � .. ������ �� ���� � �� ��������
//			* �������� �������
//				* ����� inode = 2
//				* . -> 2
//				* .. -> 2
//			* ���������� ����� �����
//				* �������������� ����� � inode
//					* / -- �� �����
//					* ./ -- �� �������� ��������
//					* ' ' -- �� �������� ��������
//	* ���������� ������ ������ -- ��� // ���� ��������� �������������, ����� ���������� ��� ������ ������ ����
//	* �������������� ��������� � ������
//	* ������ ����� � ������
//	* ������� �������
//	* ������ ������� ������ ������ [13]
//		* ������ 10�� -- ������ ���������
//		* ��������� 256�� -- ��������� ����������. ����� ���������� �����
//		* ������� ��������� ���� -- ������ 256 ������� ��������� ������
//		* ������� ��������� ���� -- ������ 256 ������� ������� ��������� ������
//*/
//
////struct INode {
////	size_t FILETYPE = 0; // 0 -- ������ inode, 1 --���� , 2 -- �������
////	size_t inode_id;
////	char name[16];
////	
////	size_t ref_count; // ���������� ���
////	size_t byte_size;
////	std::array<size_t,4096> data_blocks;
////};
//
//
//struct INode {
//	size_t FILETYPE = 0; // 0 -- ������ inode, 1 --���� , 2 -- �������
//	size_t ref_count; // ���������� ���
//	size_t byte_size;
//	std::array<size_t, 10> data_blocks;
//};
//
//void load_fs(std::string name) {
//	std::fstream file;
//	file.open(name, std::ios::in | std::ios::binary);
//	uint32_t fstype = 0;
//	file.read(reinterpret_cast<char*>(&fstype), sizeof(fstype));
//	std::cout << "fs type: " << fstype;
//}
//
//struct SuperBlock {
//	uint32_t size = 4096;
//	uint32_t block_size = 1024;
//	uint32_t inode_count = 128;
//	std::array<INode, 128> inode_list;
//};
//
//struct File {
//	size_t inode_id = 0;
//	size_t byte_size = 0;
//	size_t gpos = 0;
//	size_t ppos = 0;
//	size_t n_blocks = 0;
//	std::array<size_t, 4096> blocks;
//};
//
//
//// ��������� ������ �� �����. ������������ �������� - ������� ������� ���� ������� ���������
//size_t Read(File* file, char* buff, size_t len) {
//	std::fstream f_data("data.txt", std::ios::in | std::ios::binary);
//	size_t block_size = 1024;
//	size_t cur_block = 0;
//
//
//	size_t read_count = 0;
//	auto it = file->blocks.begin();
//
//	for (size_t i = 0; i < len; ++i) {
//		if (i % block_size == 0) {
//			cur_block = *it++;
//			f_data.seekg(cur_block * block_size);
//		}
//
//		f_data.get(buff[i]);
//		if (++read_count >= file->byte_size) break;
//	}
//
//	return read_count;
//}
//
//size_t last_block = 1;
//size_t last_inode = 3;
//File* f_root = nullptr;
//SuperBlock sb;
//
//
//size_t get_free_block() {
//	return last_block++;
//}
//
//size_t get_free_inode() {
//	return last_inode++;
//}
//
//// �������� ������ � ����. ������������ �������� - ������� ������� ���� ������� ��������
//size_t Write(File* file, const char* buff, size_t len) {
//	std::fstream f_data("data.txt", std::ios::out | std::ios::binary);
//	size_t block_size = 1024;
//	size_t cur_block = 0;
//
//	size_t write_count = len;
//	auto it = file->blocks.begin();
//	for (size_t i = 0; i < len; ++i) {
//		if (i % block_size == 0) {
//			cur_block = get_free_block();
//			file->n_blocks++;
//			f_data.seekp(cur_block * block_size);
//			if (it != file->blocks.end()) *it++ = cur_block;
//		}
//		f_data << buff[i];
//	}
//
//	file->byte_size = write_count;
//	return write_count;
//}
//
//struct Dir {
//	std::unordered_map<std::string, uint32_t> data;
//
//	Dir() = default;
//	
//	Dir(File* dir_file) {
//		//��������� ������ ���������� � �����
//		data.clear();
//		char buf[4096];
//		auto bytes_read = Read(dir_file, buf, 4096);
//		std::istringstream dir_data_stream(std::string(buf, bytes_read));
//		while (dir_data_stream) {
//			std::string filename;
//			size_t inode_id;
//			dir_data_stream >> filename >> inode_id;
//			data[filename] = inode_id;
//		}
//	}
//
//	void AddFile(std::string name, size_t inode_id) {
//		data[name] = inode_id;
//	}
//		
//	std::string toString() const{
//		std::ostringstream out_string;
//		for (const auto& elem: data) {
//			auto name = elem.first;
//			auto inode_id = elem.second;
//			out_string << name << ' ' << inode_id << '\n';
//		}
//		return out_string.str();
//	}
//	
//};
//
//
//File* CreateRoot() {
//	// ������� � ��������� inode
//	INode& inode_root = sb.inode_list[2];
//	inode_root.FILETYPE = 2;	// �������
//	inode_root.ref_count = 2;	// . � ..
//	inode_root.inode_id = 2;	// �� ��� ��� ���:)
//
//	// ������� � ��������� ���� ����������
//	Dir root;
//	// ������ �� ����
//	root.data["."] = 2;
//	root.data[".."] = 2;
//
//
//	// �������� ���� ���������� �� ����
//	auto dir_str = root.toString();
//	File* fd_res = new File;
//	inode_root.byte_size = Write(fd_res, dir_str.c_str(), dir_str.size());
//	inode_root.data_blocks = fd_res->blocks;
//	fd_res->inode_id = 2;
//	f_root = fd_res;
//	return fd_res;
//}
//
//void AddToDir(File* dir, File* file, const char* name) {
//	Dir dir_obj(dir);
//	dir_obj.AddFile(std::string(name), file->inode_id);
//	auto dir_str = dir_obj.toString();
//	Write(dir, dir_str.c_str(), dir_str.size());
//}
//
//// ������� ��� ������� ���� � writeonly ������. ���� �����, �� ������� ��� ������ �������������, ���������� � ����
//File* Create(const char* name) {
//	File* res = new File;
//	//������ ���� � �����
//
//	res->inode_id = get_free_inode();
//	AddToDir(f_root, res, name);
//	return nullptr;
//}
//
//
//void InitFs() {
//	f_root = Create("/");
//	Dir root;
//	// ������ �� ����
//	root.data["."] = 2;
//	root.data[".."] = 2;
//
//	auto dir_str = root.toString();
//	File f;
//	Write(&f, dir_str.c_str(), dir_str.size());
//}
//
//
//
//
//
//
//
//
//void test_write_read() {
//	using namespace std;
//	string t1 = "hello, world!";
//	File f;
//	Write(&f, t1.c_str(), t1.size());
//	char buf[20];
//	
//	size_t byte_count = Read(&f, buf, 20);
//
//	string t1_read(buf, byte_count);
//
//	cout << "expected: \t" << t1 << '\n'
//		<< "got: \t\t" << t1_read << endl;
//}
//
//
//
//struct DirRecord {
//	size_t inode_id;
//	char name[16];
//};
//
////struct INode {
////	size_t FILETYPE = 0; // 0 -- ������ inode, 1 -- ���� , 2 -- �������
////	size_t ref_count; // ���������� ���
////	size_t byte_size;
////	std::array<size_t, 10> data_blocks;
////};
//
//File* GetInodeByName(File* root_dir, std::string name) {
//	// ����� � ����������
//
//	std::fstream ilist_stream("inode_list.txt", std::ios::in | std::ios::binary);
//	ilist_stream.seekg(sizeof(INode) * root_dir->inode_id);
//
//	INode dir_inode_obj;
//	ilist_stream.read(reinterpret_cast<char*>(&dir_inode_obj), sizeof(dir_inode_obj));
//
//	for (size_t block = 0; block < 10; ++block) {
//		dir_inode_obj.data_blocks[block];
//
//	}
//}
//
//size_t TranslateName(const char* name) {
//	std::string str_name(name);
//	std::vector<std::string> dirs;
//
//	size_t prev_pos = 1;
//	size_t pos = str_name.find('/', 1);
//	for (; pos != str_name.npos;) {
//		pos = str_name.find('/', prev_pos + 1);
//		dirs.push_back(str_name.substr(prev_pos, pos - prev_pos));
//		prev_pos = pos + 1;
//	}
//	//std::string filename = dirs.back();
//	//dirs.pop_back();
//
//	File* current_root = f_root;
//	for (const auto& dir : dirs) {
//		current_root = GetInodeByName(current_root, dir);
//	}
//	return current_root;
//}
//
//
//
//File* Open(const char* name) {
//	size_t inode = TranslateName(name);
//
//
//
//	return nullptr;
//}

// ����� ���������
const size_t N_BLOCKS = 10;

// ����� ���������
struct INode {
	size_t FILETYPE = 0; // 0 -- ������ inode, 1 --���� , 2 -- �������
	//size_t ref_count; // ���������� ���
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

// ���������� ilist
// 
// ����� ��� ������ �� ������� inode -- ilist
class IList {
	std::string _filename;
	size_t _size;
public:
	/*
		* std::string filename	-- ��� �����-��������� ilist �� �����
		* size_t size			-- ������ ilist
		* bool reinitialize		-- ���� ��������� ������������� �����
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

	/* �������� ���������� ����� �� ��� inode id
		* size_t inode_id -- 
	*/
	File* GetFile(size_t inode_id) {
		File* fd = new File;
		
		// ��������� ������ INode
		INode* inode_obj = ReadINode(inode_id);
		if (!inode_obj) return nullptr;

		// ��������� �� ���� �������� ����������
		fd->inode_id = inode_id;
		fd->inode_obj = *inode_obj;
		
		
		return fd;
	}
	
	/* ��������� ���������� inode � ��������������� inode_id � ������ INode
		* size_t inode_id --
	*/
	INode* ReadINode(size_t inode_id) {
		// ������� ����
		std::ifstream _ilist_stream(_filename, std::ios::binary);

		if (inode_id > _size) { // ���� inode_id ������ ��� ������ ilist
			return nullptr;
		}

		// �������� ��������� �� ������ inode
		_ilist_stream.seekg(sizeof(INode) * inode_id);

		// ������ � ������ inode
		INode* inode_obj = new INode;
		_ilist_stream.read(reinterpret_cast<char*>(inode_obj), sizeof(INode));
		return inode_obj;
	}

	/* �������� ����� �������� inode ��� ��������� inode_id
		* size_t inode_id --
		* INode inode_obj --
	*/
	bool WriteINode(size_t inode_id, INode inode_obj) {
		std::ofstream _ilist_stream(_filename, std::ios::in | std::ios::binary);

		if (inode_id > _size) { // ���� inode_id ������ ��� ������ ilist
			return false;
		}
		// �������� ��������� �� ������ inode
		_ilist_stream.seekp(sizeof(INode) * inode_id);
		// ����� ������ inode � ����
		_ilist_stream.write(reinterpret_cast<char*>(&inode_obj), sizeof(INode));
		_ilist_stream.flush();
		return true;
	}

	void Clear() {
		std::fstream ilist_stream(_filename, std::ios::out | std::ios::binary | std::ios::trunc);
	}

	/*
	* ���������� INode* �� ��������� inode.
	* ���� ����� ���, ���������� nullptr;
	*/
	std::pair<INode*,size_t> GetFreeINode() {
		for (size_t inode_id = 0; inode_id < _size; ++inode_id) {
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


// ���������� ilist-a
IList ilist("ilist.txt", 10, true);

// ���������� DataStorage
class DataStorage {
	using ios = std::ios;

	std::string _filename;
	size_t _block_size;
	size_t _end_block = 1;
public:

	/*
	* std::string filename	-- ��� �����-��������� ������� ������ �� �����
	* size_t block_size		-- ������ ����� ������ (� ������)
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
		size_t blocks_count = 0;

		size_t current_block = 0;
		size_t write_count = 0;

		for (size_t i = 0; i < data_len; ++i) {
			if (i % _block_size) { // ����� �� ����� ��������� �����
				current_block = GetFreeBlock();
				data_blocks[blocks_count] = current_block;
				fd->inode_obj.blocks_count++;
				// ��������� �� (����� �����)*(������ �����)
				data_storage_stream.seekp(current_block * _block_size);
			}
			data_storage_stream.put(data[i]);
			write_count++;
		}

		//sync INode state (rewrite old)
		ilist.WriteINode(fd->inode_id, fd->inode_obj);
		return write_count;
	}

	size_t Read	(File* fd, char* buf, size_t buf_len) {
		std::ifstream data_storage_stream(_filename, ios::binary);
		auto& data_blocks = fd->inode_obj.data_blocks;

		size_t current_block = 0;
		size_t read_count = 0;
		size_t block_number = 0;

		for (size_t i = 0; i < buf_len && i < fd->inode_obj.byte_size; ++i) {
			if (i % _block_size) { 
				current_block = data_blocks[block_number++];

				// ��������� �� (����� �����)*(������ �����)
				data_storage_stream.seekg(current_block * _block_size);
			}
			data_storage_stream.get(buf[i]);
			read_count++;
		}

		return read_count;
	}
};

void test_ilist() {
	IList ilist("ilist.txt", 10, true);
	std::cout << ilist;
}

int main() {
	test_ilist();
	return 0;
}