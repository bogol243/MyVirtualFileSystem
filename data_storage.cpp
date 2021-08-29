#include "data_storage.h"


/*
* std::string filename	-- имя файла-хранилища области данных на диске
* size_t block_size		-- размер блока данных (в байтах)
*/
DataStorage::DataStorage(std::string filename, size_t block_size, bool reinitialize)
	: filename_(filename)
	, block_size_(block_size) {
	auto mode = std::ios::in | std::ios::out | std::ios::binary;
	if (reinitialize) {
		mode = mode | std::ios::trunc;
		data_storage_stream_ = std::move(std::fstream (filename_, mode));
	}
	else {
		data_storage_stream_ = std::move(std::fstream(filename_, mode));
	}
}

size_t DataStorage::GetFreeBlock() {
	return end_block_++;
}



size_t DataStorage::Append(File* fd, const char* data, size_t data_len) {
	SeekpEnd(fd);
	size_t bytes_written = Write(fd, data, data_len);
	Seekp(fd, 0);
	return bytes_written;
}

size_t DataStorage::Write(File* fd, const char* data, size_t data_len) {
	// информация о расположении интересующего файла в хранилище
	auto& data_blocks = fd->inode_obj.data_blocks;
	size_t& blocks_count = fd->inode_obj.blocks_count;

	// для прохода по списку блоков
	size_t block_number = fd->gpos / block_size_;
	block_number = block_number <= N_DATA_BLOCKS ? block_number : N_DATA_BLOCKS - 1; // превышен максимальный размер файла
	size_t current_block = data_blocks[block_number];
	size_t write_count = 0;

	//смещение на начало области чтения
	data_storage_stream_.seekp(current_block * block_size_ + (fd->ppos % block_size_));

	for (auto i = fd->ppos; i < fd->ppos + data_len; ++i) {
		if (i % block_size_ == 0) { // дошли до конца выданного блока
			if(blocks_count == N_DATA_BLOCKS) return write_count;	// достигнут максимальный размер файла в блоках
			
			current_block = GetFreeBlock();							// получение следующего свободного блока
			data_blocks[blocks_count++] = current_block;			// добавление в массив блоков для этого файла
			
			data_storage_stream_.seekp(current_block * block_size_);	// смещаемся на (номер блока)*(размер блока)
		}
		data_storage_stream_.put(data[write_count++]);	// пишем данные по одному байту
		
		data_storage_stream_.flush();					// синхронизация с диском для удобства отладки
	}

	// прочитали всё успешно, обновляем информацию в дескрипторе
	fd->inode_obj.byte_size += write_count;
	fd->ppos += write_count;

	data_storage_stream_.flush();
	return write_count;
}

size_t DataStorage::Read(File* fd, char* buf, size_t buf_len) {
	// информация о расположении интересующего файла в хранилище
	auto& data_blocks = fd->inode_obj.data_blocks;
	size_t read_count = 0;

	// для прохода по списку блоков
	size_t block_number = fd->gpos / block_size_;
	block_number = block_number <= N_DATA_BLOCKS ? block_number : N_DATA_BLOCKS - 1; // превышен максимальный размер файла
	size_t current_block = data_blocks[block_number];

	data_storage_stream_.seekg(current_block * block_size_ + (fd->gpos % block_size_));

	for (auto i = fd->gpos; read_count < buf_len && i < fd->inode_obj.byte_size; ++i) {
		if (i % block_size_ == 0) {
			block_number = i / block_size_;
			if (block_number > N_DATA_BLOCKS) return read_count; // превышен максимальный размер файла
			current_block = data_blocks[block_number];
			// смещаемся на (номер блока)*(размер блока)
			data_storage_stream_.seekg(current_block * block_size_);
		}
		data_storage_stream_.get(buf[read_count++]);
	}
	fd->gpos += read_count;
	return read_count;
}

size_t DataStorage::ComputeBlockNumber(File* file) {
	return file->gpos / block_size_;
}