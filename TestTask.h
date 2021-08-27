#pragma once
#include <iostream>
#include <sstream>
#include <string>

#include "ilist.h"
#include "data_storage.h"
#include "common_structures.h"
#include "my_vfs.h"

namespace TestTask {
	// ��������� File* -- �������� ����������, ��������� � ����� common_structures.h

	// ��������� ��� �������������� � ����������� �������� ��������
	struct IVFS {
		std::mutex fs_mutex;

		// ������ ����������� �������� �������
		MYVFS fs{ VFSSettings()
			.SetBlockSize(256)
			.SetReinitialize(true)
			.SetIListCapacity(2000) };

		// ������� ���� � readonly ������. ���� ��� ������ ����� - ������� nullptr
		File* Open(const char* name);

		// ������� ��� ������� ���� � writeonly ������. ���� �����, �� ������� ��� ������ �������������, ���������� � ����
		File* Create(const char* name);

		// ��������� ������ �� �����. ������������ �������� - ������� ������� ���� ������� ���������
		size_t Read(File* f, char* buff, size_t len);

		// �������� ������ � ����. ������������ �������� - ������� ������� ���� ������� ��������
		size_t Write(File* f, const char* buff, size_t len);

		// ������� ����	
		void Close(File* f);
	};

}