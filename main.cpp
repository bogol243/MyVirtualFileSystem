#include <iostream>
#include <sstream>
#include <string>
#include <random>

#include "ilist.h"
#include "data_storage.h"
#include "common_structures.h"
#include "my_vfs.h"
#include "TestTask.h"

#include <future>
#include <cassert>


// �����

// Smoke ���� �������
// 
// ��������� ���������:
//		���� ��������, ������ ��������, ����������� ��������� ������
void test_1() {
	TestTask::IVFS vfs;

	auto fd = vfs.Create("file1");
	char data[] = "hello world";
	vfs.Write(fd, data, 12);
	vfs.Close(fd);
	
	fd = vfs.Open("file1");
	char buf[20];
	size_t bytes_read = vfs.Read(fd, buf, 20);
	std::string str_res(buf, bytes_read);

	assert(strcmp(data, buf) == 0);
}

// ���� ��������� ������� �������������� ������ �������� �������
//
void test_dir_tree() {
	TestTask::IVFS vfs;

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

// �������� root ����� (���� "/")
//
// ��������� ���������: 
//		������� �� nullptr
//		inode_id ������������ ����� ����� 2
//		��� ����� -- ����������
void test_open_root() {
	TestTask::IVFS vfs;
	auto fd = vfs.Open("/");
	
	assert(fd != nullptr);				// ��� ����
	assert(fd->inode_id = 2);			// ����� inode_id = 2
	assert(fd->inode_obj.FILETYPE = 2); // ��� ��� ����������
}

// �������� ��������������� �����
//
// ��������� ���������: 
//		����� Open ����� nullptr
void test_open_absent() {
	TestTask::IVFS vfs;
	auto fd = vfs.Open("absent_file");
	
	assert(fd == nullptr);
}

// �������� �����
//
// ��������� ���������: 
//		����� �������� ������ � ���� � ������ �� ����� ����������� �����������
void test_close() {
	TestTask::IVFS vfs;
	auto fd = vfs.Create("file1");
	vfs.Write(fd, "hello!", 6);
	vfs.Close(fd);
	char buf[10];
	assert(vfs.Read(fd, buf, 10) == 0);
	assert(vfs.Write(fd, " world!", 7) == 0);
}

// ������ �� ����� ��������� �� ������
//
// ��������� ���������: 
//		���� ������� ������, ��������� ������ ������������� ���������
void test_open_read() {
	TestTask::IVFS vfs;

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

// ������ � ���� �������� �� ������
//
// ��������� ���������: 
//		������ ����� �� ������ ����������
//		���������� ���������� ���� == 0
//		������ �� ������ ����������
void test_open_write() {

	TestTask::IVFS vfs;

	auto fd = vfs.Create("file1");
	vfs.Write(fd, "some data", 10);
	vfs.Close(fd);

	fd = vfs.Open("file1");

	assert(fd != nullptr); //���� �������� �������
	size_t bytes_written = vfs.Write(fd, "hello!", 6);

	assert(bytes_written == 0);					// ����� �� ����������
	char buf[20];
	size_t bytes_read = vfs.Read(fd, buf, 20);

	assert(std::strcmp(buf, "some data") == 0); // ������ �� ����������
}

// ������ ���������� ������ ����� ����� ������ �������� ����������� �� ���������� �������
//
// ��������� ���������: ������ ���� ������ ��������� ���������
void test_multithreading_read_from_different_fd_single_file() {
	TestTask::IVFS vfs;
	
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

// ���������� ������ ������ ����� ����� ���� �������� ���������� �� ���������� �������
//
// ��������� ���������: ���� ����������_�������*������_�����>=������ �����, ����� ���������� ��������� ����
// ����� ����� ������� �����. ������� ���������� �� �������������.
void test_multithreading_read_from_same_fd_single_file() {
	TestTask::IVFS vfs;
	

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


// ���������� ������ ������ ����� ����� ������ �������� ����������� �� ���������� �������
//
// ��������� ���������: ������ ���� ������ ��������� ���������
void test_multithreading_read_from_different_fd_multiple_files() {
	TestTask::IVFS vfs;

	std::vector<std::string> files_data;
	std::vector<File*> files_fds;
	const size_t NUM_FILES = 500;

	for (size_t i = 0; i < NUM_FILES; ++i) {
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


// ���������� ������ ����� ��������� ����� ���� �������� ���������� �� ���������� �������
//
// ��������� ���������: ���� ����������� ������ ����������� �������.
void test_multithreading_read_from_same_fd() {
	
	
	TestTask::IVFS vfs;

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



// ������ � ������ �����, ����� ������ �������� ����������� � ��������� �������
//
// ��������� ���������: ����� �������� ���������� ���� ������ ����� �������� ������ �� ���� �������
void test_multithreading_write_to_different_fd_multiple_files() {
	TestTask::IVFS vfs;

	std::vector<std::string> files_data;
	std::vector<File*> files_fds;
	const size_t NUM_FILES = 100;

	for (size_t i = 0; i < NUM_FILES; ++i) {
		std::string data(100, 'A' + i % 20);
		files_data.push_back(data);

		auto filename = std::string("file") + std::to_string(i);
		auto fd = vfs.Create(filename.c_str());
		files_fds.push_back(fd);
	}

	std::atomic<size_t> total_bytes_written = 0;
	auto test_func =
		[&vfs,&total_bytes_written](std::string data, File* fd) {
		
		total_bytes_written += vfs.Write(fd, data.c_str(),data.size());
	};
	
	{
		std::vector<std::future<void>> futures;
		for (size_t i = 0; i < NUM_FILES; ++i) {
			futures.push_back(std::async(test_func, files_data[i], files_fds[i]));
		}
	}

	assert(total_bytes_written == NUM_FILES * 100);
}

// ������ � ���� ����� ���� �������� ���������� � ��������� �������
//
// ��������� ���������: ����� �������� ���������� ���� ������ ����� �������� ������ �� ���� �������,
// ������ ������������� ����� ����� ����� ���� �����. ������� ������ �� ������������� � �� �����������
void test_multithreading_write_to_same_fd() {


	TestTask::IVFS vfs;

	
	auto fd = vfs.Create("file1");

	std::atomic<size_t> total_bytes_written = 0;
	
	auto test_func =
		[&vfs, &fd, &total_bytes_written]() {
		std::string data(20, 'A');

		total_bytes_written += vfs.Write(fd, data.c_str(), 20);
	};

	const size_t N = 100;
	{
		std::vector<std::future<void>> futures;
		for (size_t i = 0; i < N; ++i) {
			futures.push_back(std::async(test_func));
		}
	}

	assert(fd->inode_obj.byte_size == N * 20 && total_bytes_written == N * 20);
}

// TODO
// ���� ���� � ������������� �������� � VFS.
// � ��������� ������� 1000 ��� ������� � �������� �����, � ��������� ��� ���������� ����������.
//
// ��������� ���������: 
void test_multithreading_read_write() {
	TestTask::IVFS vfs;
	
	std::atomic<size_t> file_number = 0;
	std::atomic<size_t> fail_count = 0;

	auto test_func =
		[&vfs,&file_number,&fail_count]() {
		std::string filename = "file" + std::to_string(file_number++);
		std::string file_data('A' + (file_number % 20), 200);

		// �������� � ������
		auto fd = vfs.Create(filename.c_str());
		vfs.Write(fd, file_data.c_str(), 201);
		vfs.Close(fd);
		delete fd;

		// ������
		char buf[201];
		fd = vfs.Open(filename.c_str());
		vfs.Read(fd, buf, 201);

		// �������� ������
		fail_count += (strcmp(buf, file_data.c_str()) != 0);
	};

	// ������������ ����� ����������� �������
	const size_t N = 1000;
	{
		std::vector<std::future<void>> futures;
		for (size_t i = 0; i < N; ++i) {
			futures.push_back(std::async(test_func));
		}
	}

	// ��� ������ �����
	assert(fail_count == 0);
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

	test_multithreading_write_to_different_fd_multiple_files();

	test_multithreading_write_to_same_fd();

	//test_multithreading_read_write();

	std::cout << "Multithreading tests OK" << std::endl;
}


int main() {
	run_tests();
	//run_multithreading_tests();
	return 0;
}