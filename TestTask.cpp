#include "TestTask.h"


namespace TestTask {

	// ������� ���� � readonly ������. ���� ��� ������ ����� - ������� nullptr
	File* IVFS::Open(const char* name) {
		std::lock_guard<std::mutex> global_fs_lock(fs_mutex);

		std::string name_str(name);
		if (name_str[0] != '/') name_str = "/" + name_str;

		File* file = fs.TranslateNameToFd(name_str);
		if (!file) {
			return nullptr;
		}
		else {
			file->openmode = OpenMode::RDONLY;
			return file;
		}
	}

	// ������� ��� ������� ���� � writeonly ������. ���� �����, �� ������� ��� ������ �������������, ���������� � ����
	File* IVFS::Create(const char* name) {
		std::lock_guard<std::mutex> global_fs_lock(fs_mutex);

		std::string name_str(name);
		if (name_str[0] != '/') name_str = "/" + name_str;

		File* file_fd = fs.TranslateNameToFd(name_str);
		file_fd = file_fd ? file_fd : fs.MkFile(name_str);
		file_fd->openmode = OpenMode::WRONLY;
		return file_fd;
	}

	// ��������� ������ �� �����. ������������ �������� - ������� ������� ���� ������� ���������
	size_t IVFS::Read(File* f, char* buff, size_t len) {
		std::lock_guard<std::mutex> global_fs_lock(fs_mutex);

		if (!f) return 0;

		if (f->openmode != OpenMode::WRONLY && f->openmode != OpenMode::CLOSED) {
			return fs.Read(f, buff, len);
		}
		else {
			return 0;
		}
	}

	// �������� ������ � ����. ������������ �������� - ������� ������� ���� ������� ��������
	size_t IVFS::Write(File* f, const char* buff, size_t len) {
		std::lock_guard<std::mutex> global_fs_lock(fs_mutex);

		if (!f) return 0;

		if (f->openmode != OpenMode::RDONLY && f->openmode != OpenMode::CLOSED) {
			return fs.Write(f, buff, len);
		}
		else {
			return 0;
		}
	}

	// ������� ����	
	void IVFS::Close(File* f) {
		std::lock_guard<std::mutex> global_fs_lock(fs_mutex);

		if (f) f->openmode = OpenMode::CLOSED;
	}

}