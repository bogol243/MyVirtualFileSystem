#include "data_storage.h"


/*
* std::string filename	-- имя файла-хранилища области данных на диске
* size_t block_size		-- размер блока данных (в байтах)
*/
DataStorage::DataStorage(std::string filename, size_t block_size, bool reinitialize)
	: _filename(filename)
	, _block_size(block_size) {
	if (reinitialize) {
		data_storage_stream = std::move(std::fstream (_filename, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc));
	}
	else {
		data_storage_stream = std::move(std::fstream(_filename, std::ios::in | std::ios::out | std::ios::binary));
	}
}

void DataStorage::Clear() {
	//std::fstream data_storage_stream(_filename, std::ios::out | std::ios::binary | std::ios::trunc);
}

size_t DataStorage::GetFreeBlock() {
	return _end_block++;
}



size_t DataStorage::Append(File* fd, const char* data, size_t data_len) {
	SeekpEnd(fd);
	size_t bytes_written = Write(fd, data, data_len);
	Seekp(fd, 0);
	return bytes_written;
}

size_t DataStorage::Write(File* fd, const char* data, size_t data_len) {
	//std::ofstream data_storage_stream(_filename, ios::in | ios::binary); // открытие файла-хранцилища данных
	
	// информация о расположении интересующего файла в хранилище
	auto& data_blocks = fd->inode_obj.data_blocks;
	size_t& blocks_count = fd->inode_obj.blocks_count;

	// для прохода по списку блоков
	size_t block_number = fd->gpos / _block_size;
	block_number = block_number <= N_DATA_BLOCKS ? block_number : N_DATA_BLOCKS - 1; // превышен максимальный размер файла
	size_t current_block = data_blocks[block_number];
	size_t write_count = 0;

	//смещение на начало области чтения
	data_storage_stream.seekp(current_block * _block_size + (fd->ppos % _block_size));

	for (size_t i = fd->ppos; i < fd->ppos + data_len; ++i) {
		if (i % _block_size == 0) { // дошли до конца выданного блока
			
			if(blocks_count == N_DATA_BLOCKS) return write_count;	// достигнут максимальный размер файла в блоках
			
			current_block = GetFreeBlock();							// получение следующего свободного блока
			data_blocks[blocks_count++] = current_block;			// добавление в массив блоков для этого файла
			
			data_storage_stream.seekp(current_block * _block_size);	// смещаемся на (номер блока)*(размер блока)
		}

		data_storage_stream.put(data[write_count++]);	// пишем данные по одному байту
		
		data_storage_stream.flush();					// синхронизация с диском для удобства отладки
	}

	// прочитали всё успешно, обновляем информацию в дескрипторе
	fd->inode_obj.byte_size += write_count;
	fd->ppos += write_count;

	data_storage_stream.flush();
	return write_count;
}

size_t DataStorage::Read(File* fd, char* buf, size_t buf_len) {

	//std::ifstream data_storage_stream(_filename, ios::binary); // открытие файла-хранцилища данных

	// информация о расположении интересующего файла в хранилище
	auto& data_blocks = fd->inode_obj.data_blocks;


	size_t read_count = 0;

	// для прохода по списку блоков
	size_t block_number = fd->gpos / _block_size;
	block_number = block_number <= N_DATA_BLOCKS ? block_number : N_DATA_BLOCKS - 1; // превышен максимальный размер файла
	size_t current_block = data_blocks[block_number];

	data_storage_stream.seekg(current_block * _block_size + (fd->gpos % _block_size));

	for (size_t i = fd->gpos; read_count < buf_len && i < fd->inode_obj.byte_size; ++i) {
		if (i % _block_size == 0) {
			block_number = i / _block_size;
			
			if (block_number > N_DATA_BLOCKS) return read_count; // превышен максимальный размер файла
			current_block = data_blocks[block_number];

			// смещаемся на (номер блока)*(размер блока)
			data_storage_stream.seekg(current_block * _block_size);
		}
		data_storage_stream.get(buf[read_count++]);
	}
	fd->gpos += read_count;
	return read_count;
}