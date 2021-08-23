#include <iostream>
#include <sstream>
#include <string>

#include "ilist.h"
#include "data_storage.h"
#include "common_structures.h"
#include "my_vfs.h"
#include "tests.h"

#include <future>
#include <cassert>

namespace TestTask{
	

	// структура File* -- файловый дескриптор, объявлена в файле common_structures.h

	struct VFS{

		MYVFS fs{ VFSSettings()
			.SetBlockSize(256)
			.SetReinitialize(true)};

		// Открыть файл в readonly режиме. Если нет такого файла - вернуть nullptr
		File* Open(const char* name) {
			
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

		// Открыть или создать файл в writeonly режиме. Если нужно, то создать все нужные поддиректории, упомянутые в пути
		File* Create(const char* name) {
			std::string name_str(name);
			if (name_str[0] != '/') name_str = "/" + name_str;
			
			File* file_fd = fs.TranslateNameToFd(name_str);
			file_fd = file_fd ? file_fd : fs.MkFile(name_str);
			file_fd->openmode = OpenMode::WRONLY;
			return file_fd;
		}

		// Прочитать данные из файла. Возвращаемое значение - сколько реально байт удалось прочитать
		size_t Read(File* f, char* buff, size_t len) {
			if (!f) return 0;

			if (f->openmode != OpenMode::WRONLY && f->openmode != OpenMode::CLOSED) {
				return fs.Read(f, buff, len);
			}
			else {
				return 0;
			}
		}
		
		// Записать данные в файл. Возвращаемое значение - сколько реально байт удалось записать
		size_t Write(File* f, const char* buff, size_t len) {
			if (!f) return 0;

			if (f->openmode != OpenMode::RDONLY && f->openmode != OpenMode::CLOSED) {
				return fs.Write(f, buff, len);
			}
			else {
				return 0;
			}
		}
		
		// Закрыть файл	
		void Close(File* f) {
			if(f) f->openmode = OpenMode::CLOSED;
		}
	};

}


// тесты


void test_1() {
	TestTask::VFS vfs;

	auto fd = vfs.Create("file1");
	char data[] = "hello world";
	vfs.Write(fd, data, 11);
	vfs.Close(fd);
	
	fd = vfs.Open("file1");
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

void test_close() {
	TestTask::VFS vfs;
	auto fd = vfs.Create("file1");
	vfs.Write(fd, "hello!", 6);
	vfs.Close(fd);
	char buf[10];
	assert(vfs.Read(fd, buf, 10) == 0);
	assert(vfs.Write(fd, " world!", 7) == 0);
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

	auto fd = vfs.Create("file1");
	vfs.Write(fd, "some data", 10);
	vfs.Close(fd);

	fd = vfs.Open("file1");

	assert(fd != nullptr); //файл открылся успешно
	size_t bytes_written = vfs.Write(fd, "hello!", 6);

	assert(bytes_written == 0);					// байты не записались
	char buf[20];
	size_t bytes_read = vfs.Read(fd, buf, 20);

	assert(std::strcmp(buf, "some data") == 0); // данные не изменились
}

void test_multithreading_read_from_different_fd_single_file() {
	TestTask::VFS vfs;
	
	std::string data(500, 'A');
	auto fd = vfs.Create("file1");
	vfs.Write(fd, data.c_str(), data.size() + 1);
	vfs.Close(fd);
	delete fd;

	fd = vfs.Open("file1");

	auto test_func = 
		[&vfs,data]() {
			auto fd = vfs.Open("file1");
			char buf[501];
			vfs.Read(fd, buf, 501);
			assert(std::strcmp(buf, data.c_str()) == 0);
	};
	
	const size_t N = 10000;
	std::vector<std::future<void>> futures;
	for (size_t i = 0; i < N; ++i) {
		futures.push_back(std::async(test_func));
	}

}

void test_multithreading_read_from_same_fd_single_file() {
	TestTask::VFS vfs;
	

	std::string data(500, 'A');
	auto fd = vfs.Create("file");
	vfs.Write(fd, data.c_str(), data.size() + 1);
	vfs.Close(fd);
	delete fd;

	fd = vfs.Open("file");
	std::atomic<size_t> total_bytes_read = 0;
	auto test_func =
		[&vfs,&fd,&total_bytes_read]() {

		char buf[20];
		size_t bytes_read = vfs.Read(fd, buf, 20);
		total_bytes_read += bytes_read;
	};
	
	{	
		std::vector<std::future<void>> futures;
		for (size_t i = 0; i < 30; ++i) {
			futures.push_back(std::async(test_func));
		}
	}


	assert(total_bytes_read == 501);
}

void test_multithreading_read_from_different_fd_multiple_files() {
	TestTask::VFS vfs;

	std::vector<std::string> files_data;
	std::vector<File*> files_fds;
	const size_t NUM_FILES = 500;

	for (int i = 0; i < NUM_FILES; ++i) {
		std::string data(500, 'A'+ i % 20);
		files_data.push_back(data);
		
		auto filename = std::string("file") + std::to_string(i);
		auto fd = vfs.Create(filename.c_str());
		
		vfs.Write(fd, data.c_str(), data.size() + 1);

		// Reopen for read
		vfs.Close(fd);
		delete fd;
		fd = vfs.Open(filename.c_str());

		files_fds.push_back(fd);
	}

	auto test_func =
		[&vfs](std::string data, File* fd) {

		char buf[500+1];
		vfs.Read(fd, buf, 500+1);
		assert(std::strcmp(buf, data.c_str()) == 0);
	};

	std::vector<std::future<void>> futures;

	for (size_t i = 0; i < NUM_FILES; ++i) {
		futures.push_back(std::async(test_func,files_data[i],files_fds[i]));
	}

}


// чтение одного файла через один файловый дескриптор из нескольких потоков:
//	1) если считывается байт больше или равно размеру файла -- файл считывается первым захватившим потоком.
//	2) если считывается байт меньше чем размер файла -- порядок не гарантируется
void test_multithreading_read_from_same_fd() {
	
	
	TestTask::VFS vfs;

	std::string data(500,'A');
	auto fd = vfs.Create("file1");
	vfs.Write(fd, data.c_str(), data.size()+1);
	vfs.Close(fd);
	delete fd;

	fd = vfs.Open("file1");
	
	std::atomic<size_t> count;
	auto test_func =
		[&vfs,&fd, data,&count]() {
		char buf[510];
		vfs.Read(fd, buf, 510);
		count += static_cast<size_t>(std::strcmp(buf, data.c_str()) == 0);
	};

	const size_t N = 1000;
	{
		std::vector<std::future<void>> futures;
		for (size_t i = 0; i < N; ++i) {
			futures.push_back(std::async(test_func));
		}
	}
	assert(count == 1);
}


void run_tests() {
	test_1();
	test_dir_tree();
	test_close();
	test_open_root();
	test_open_absent();
	test_open_read();
	test_open_write();

	std::cout << "Tests OK" << std::endl;
}

void run_multithreading_tests() {
	test_multithreading_read_from_different_fd_single_file();
	test_multithreading_read_from_same_fd_single_file();

	test_multithreading_read_from_different_fd_multiple_files();

	test_multithreading_read_from_same_fd();

	std::cout << "Multithreading tests OK" << std::endl;
}


int main() {
	run_tests();
	run_multithreading_tests();
	return 0;
}