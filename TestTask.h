#pragma once
#include <iostream>
#include <sstream>
#include <string>

#include "ilist.h"
#include "data_storage.h"
#include "common_structures.h"
#include "my_vfs.h"

namespace TestTask {
	// структура File* -- файловый дескриптор, объявлена в файле common_structures.h

	// интерфейс для взаимодействия с виртуальной файловой системой
	struct IVFS {
		std::mutex fs_mutex;

		// объект виртуальной файловой системы
		MYVFS fs{ VFSSettings()
			.SetBlockSize(256)
			.SetReinitialize(true)
			.SetIListCapacity(2000) };

		// Открыть файл в readonly режиме. Если нет такого файла - вернуть nullptr
		File* Open(const char* name);

		// Открыть или создать файл в writeonly режиме. Если нужно, то создать все нужные поддиректории, упомянутые в пути
		File* Create(const char* name);

		// Прочитать данные из файла. Возвращаемое значение - сколько реально байт удалось прочитать
		size_t Read(File* f, char* buff, size_t len);

		// Записать данные в файл. Возвращаемое значение - сколько реально байт удалось записать
		size_t Write(File* f, const char* buff, size_t len);

		// Закрыть файл	
		void Close(File* f);
	};

}