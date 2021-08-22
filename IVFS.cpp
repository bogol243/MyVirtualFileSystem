#include <iostream>
#include <sstream>
#include <string>

#include "ilist.h"
#include "data_storage.h"
#include "directory.h"
#include "my_vfs.h"
#include "tests.h"

#include <cassert>

namespace TestTask{
	

	// структура File* -- файловый дескриптор, объявлена в файле common_structures.h

	struct VFS{
		MYVFS fs;

		// Открыть файл в readonly режиме. Если нет такого файла - вернуть nullptr
		File* Open(const char* name) {
			std::string name_str(name);
			if (name_str[0] != '/') name_str = "/" + name_str;

			return fs.TranslateNameToFd(name_str);
		}

		// Открыть или создать файл в writeonly режиме. Если нужно, то создать все нужные поддиректории, упомянутые в пути
		File* Create(const char* name) {
			std::string name_str(name);
			if (name_str[0] != '/') name_str = "/" + name_str;
			
			File* file_fd = fs.TranslateNameToFd(name_str);
			return file_fd ? file_fd : fs.MkFile(name_str);
		}

		// Прочитать данные из файла. Возвращаемое значение - сколько реально байт удалось прочитать
		size_t Read(File* f, char* buff, size_t len) {
			return fs.Read(f, buff, len);
		}
		
		// Записать данные в файл. Возвращаемое значение - сколько реально байт удалось записать
		size_t Write(File* f, const char* buff, size_t len) {
			return fs.Write(f, buff, len);
		}
		
		// Закрыть файл	
		void Close(File* f) {
			delete f;
			f = nullptr;
		}
	};

}



void test_1() {
	TestTask::VFS vfs;

	auto fd = vfs.Create("file1");
	char data[] = "hello world";
	vfs.Write(fd, data, 11);

	char buf[20];
	size_t bytes_read = vfs.Read(fd, buf, 20);
	std::string str_res(buf, bytes_read);

	assert(strcmp(data, str_res.c_str()) == 0);
}

void test_dir_tree() {
	TestTask::VFS vfs;

	vfs.Create("file1");
	vfs.Create("file2");
	vfs.Create("/dir/file1_d");
	vfs.Create("/dir/file2_d");
	vfs.Create("/dir/dir2/file1");
	
	std::string expected("file1\nfile2\ndir\n\tfile1_d\n\tfile2_d\n\tdir2\n\t\tfile1\n");

	std::ostringstream result_oss;
	vfs.fs.PrintTree(result_oss);

	assert(expected == result_oss.str());
}

void test_open_root() {
	TestTask::VFS vfs;
	auto fd = vfs.Open("/");
	
	assert(fd != nullptr);				// рут есть
	assert(fd->inode_id = 2);			// имеет inode_id = 2
	assert(fd->inode_obj.FILETYPE = 2); // рут это директория
}

void test_open_absent() {
	TestTask::VFS vfs;
	auto fd = vfs.Open("absent_file");
	
	assert(fd == nullptr);
}

void test_close() { // TODO FIX CLOSE
	TestTask::VFS vfs;
	auto fd = vfs.Create("file1");
	vfs.Close(fd);

	try {
		char buf[10];
		vfs.Read(fd, buf, 10);
		assert(false);
	}
	catch (...){}
}

void test_open_read() {
	TestTask::VFS vfs;

	auto fd1 = vfs.Create("file1");
	std::string file1_data = "this is data, that contains in file1, and some extra chars";

	vfs.Write(fd1, file1_data.c_str(), file1_data.size());


	char buf[200];

	vfs.Close(fd1);
	
	auto fd2 = vfs.Open("file1");

	size_t bytes_read = vfs.Read(fd2, buf, 200);

	std::string res_string(buf, bytes_read);

	assert(file1_data == res_string);
}

void test_open_write() {
	// если файл открыт в режиме readonly, запись в него невозможна, функция write будет записывать 0 байт
	// файл не должен помняться при попытке записи в readonly режиме
	TestTask::VFS vfs;

	auto fd1 = vfs.Create("file1");
	vfs.Close(fd1);

	auto fd2 = vfs.Open("file1");

	assert(fd2 != nullptr); //файл открылся успешно
	size_t bytes_written = vfs.Write(fd2, "hello!", 6);

	assert(bytes_written == 0);
}

void run_tests() {
	test_1();
	test_dir_tree();
	//test_close();
	test_open_root();
	test_open_absent();
	test_open_read();
	//test_open_write();

	std::cout << "Tests OK" << std::endl;
}

int main() {
	run_tests();
	return 0;
}