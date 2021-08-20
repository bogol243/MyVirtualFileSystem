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
		size_t blocks_count = fd->inode_obj.blocks_count;

		size_t current_block = data_blocks[fd->ppos / _block_size];
		size_t write_count = 0;

		data_storage_stream.seekp(current_block * _block_size + (fd->ppos % _block_size));

		for (size_t i = fd->ppos; i < fd->ppos + data_len; ++i) {
			if (i % _block_size == 0) { // ����� �� ����� ��������� �����
				current_block = GetFreeBlock();
				data_blocks[fd->inode_obj.blocks_count++] = current_block;
				// ��������� �� (����� �����)*(������ �����)
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
				// ��������� �� (����� �����)*(������ �����)
				data_storage_stream.seekg(current_block * _block_size);
			}
			data_storage_stream.get(buf[read_count++]);
		}

		return read_count;
	}
};


// ���������� ���������� � ���������� ���. 
// �� ��������� �� � ������ ����� �� �����. ������ ���������� ��������� � vfs ��� �����.

/*
* ������������ ������ � ����������
*/
struct DirRecord {
	size_t inode_id;
	char name[16];
};

class Directory {
	File* _fd;  // ��� ����� ������ ����������
public:
	/*
	* File* fd -- �������� ���������� ����� ����������
	*/
	Directory(File* fd)
		: _fd(fd) {

	}
};

//class Directories {
//public:
//	
//	/*
//	* �������� �������� ����������
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