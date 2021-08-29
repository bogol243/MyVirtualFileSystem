#include "ilist.h"

using namespace std;

	IList::IList() {
	}

	IList::IList(string filename, size_t size, bool reinitialize)
		: filename_(filename)
		, size_(size)
	{
		auto mode = std::ios::in | std::ios::out | std::ios::binary;
		if (reinitialize) {
			mode = mode | std::ios::trunc;
			ilist_stream_ = move(fstream(filename_, mode));

			for (auto id = 0; id < size_; ++id) {
				INode empty_node;
				WriteINode(id, empty_node);
			}
		}
		else {
			ilist_stream_ = move(fstream(filename_, mode));
		}
	}

	/*
	* �������� ���� �� ������� ������ �� �������������� ��� inode
	*/
	File* IList::GetFile(size_t inode_id){
		File* fd = new File;

		// ��������� ������ INode
		auto inode_obj = ReadINode(inode_id);
		if (!inode_obj) return nullptr;

		// ��������� �� ���� �������� ����������
		fd->inode_id = inode_id;
		fd->inode_obj = *inode_obj;


		return fd;
	}

	/* ��������� ���������� inode � ��������������� inode_id � ������ INode
	*/
	optional<INode> IList::ReadINode(size_t inode_id){
		// ������� ����
		//ifstream ilist_stream_(filename_, ios::binary);

		if (inode_id > size_) { // ���� inode_id ������ ��� ������ ilist
			return nullopt;
		}

		// �������� ��������� �� ������ inode
		ilist_stream_.seekg(sizeof(INode) * inode_id);

		// ������ � ������ inode
		INode inode_obj;
		ilist_stream_.read(reinterpret_cast<char*>(&inode_obj), sizeof(INode));
		return inode_obj;
	}

	/* �������� ����� �������� inode ��� ��������� inode_id
	*/
	bool IList::WriteINode(size_t inode_id, INode inode_obj) {
		//ofstream ilist_stream_(filename_, ios::in | ios::out | ios::binary);

		if (inode_id > size_) { // ���� inode_id ������ ��� ������ ilist
			return false;
		}
		// �������� ��������� �� ������ inode
		ilist_stream_.seekp(sizeof(INode) * inode_id);
		// ����� ������ inode � ����
		ilist_stream_.write(reinterpret_cast<char*>(&inode_obj), sizeof(INode));
		ilist_stream_.flush();
		return true;
	}

	/*
	* ���������� id ��������� inode.
	* ���� ����� ��� -- ���������� 0
	*/
	size_t IList::GetFreeINode() {
		for (size_t inode_id = 2; inode_id < size_; ++inode_id) {
			auto inode = ReadINode(inode_id);
			if (inode && inode->filetype == Filetype::EMPTY) return inode_id;
		}
		return 0;
	}

ostream& operator<<(ostream& out, const INode& inode) {
	out << "type: " << static_cast<size_t>(inode.filetype) << " | "
		<< "byte_size: " << inode.byte_size << " | "
		<< "blocks_count: " << inode.blocks_count << " | "
		<< "blocks: ";
	for (size_t block_id = 0; block_id < N_DATA_BLOCKS; ++block_id) {
		out << inode.data_blocks[block_id] << ' ';
	}
	return out;
}

ostream& operator<<(ostream& out, IList& ilist) {
	for (size_t id = 0; id < ilist.size_; ++id) {
		auto inode_obj = ilist.ReadINode(id);
		if (!inode_obj) break;
		out << *inode_obj << '\n';
	}
	return out;
}