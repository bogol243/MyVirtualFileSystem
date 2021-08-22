#include "ilist.h"

using namespace std;

	IList::IList() {
	}

	IList::IList(string filename, size_t size, bool reinitialize)
		: _filename(filename)
		, _size(size)
	{
		if (reinitialize) {
			Clear();

			for (size_t id = 0; id < _size; ++id) {
				INode empty_node;
				WriteINode(id, empty_node);
			}
		}
	}


	File* IList::GetFile(size_t inode_id) {
		File* fd = new File;

		// прочитать нужный INode
		auto inode_obj = ReadINode(inode_id);
		if (!inode_obj) return nullptr;

		// построить из него файловый дескриптор
		fd->inode_id = inode_id;
		fd->inode_obj = *inode_obj;


		return fd;
	}

	/* Прочитать содержимое inode с идентификатором inode_id в объект INode
		* size_t inode_id --
	*/
	optional<INode> IList::ReadINode(size_t inode_id) {
		// открыть файл
		ifstream _ilist_stream(_filename, ios::binary);

		if (inode_id > _size) { // если inode_id больше чем размер ilist
			return nullopt;
		}

		// сдвигаем указатель на нужный inode
		_ilist_stream.seekg(sizeof(INode) * inode_id);

		// читаем в объект inode
		INode inode_obj;
		_ilist_stream.read(reinterpret_cast<char*>(&inode_obj), sizeof(INode));
		return inode_obj;
	}

	/* Записать новое значение inode для заданного inode_id
		* size_t inode_id --
		* INode inode_obj --
	*/
	bool IList::WriteINode(size_t inode_id, INode inode_obj) {
		ofstream _ilist_stream(_filename, ios::in | ios::out | ios::binary);

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

	void IList::Clear() {
		fstream ilist_stream(_filename, ios::out | ios::binary | ios::trunc);
	}

	/*
	* Возвращает INode* на свободную inode.
	* Если такой нет, возвращает nullptr;
	*/
	size_t /*pair<INode*, size_t>*/ IList::GetFreeINode() {
		for (size_t inode_id = 2; inode_id < _size; ++inode_id) {
			auto inode = ReadINode(inode_id);
			if (inode && inode->FILETYPE == 0) return inode_id;
		}
		return 0;
	}

ostream& operator<<(ostream& out, const INode& inode) {
	out << "type: " << inode.FILETYPE << " | "
		<< "byte_size: " << inode.byte_size << " | "
		<< "blocks_count: " << inode.blocks_count << " | "
		<< "blocks: ";
	for (size_t block_id = 0; block_id < N_BLOCKS; ++block_id) {
		out << inode.data_blocks[block_id] << ' ';
	}
	return out;
}

ostream& operator<<(ostream& out, IList& ilist) {
	for (size_t id = 0; id < ilist._size; ++id) {
		auto inode_obj = ilist.ReadINode(id);
		if (!inode_obj) break;
		out << *inode_obj << '\n';
	}
	return out;
}