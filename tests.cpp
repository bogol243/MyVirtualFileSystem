#include "tests.h"

//void test_name_translator() {
//
//	Directory root(root_fd);
//
//	auto test_fd = GetDir();
//	Directory test(test_fd);
//
//	auto test2_fd = GetDir();
//	Directory test2(test2_fd);
//
//	File* file_fd = GetFile();
//
//	root.AddFile(test_fd, "test");
//	test.AddFile(test2_fd, "test2");
//	test2.AddFile(file_fd, "file.txt");
//
//	size_t inode_id = TranslateNameToINodeId("/test/test2/file.txt");
//}
//
//void test_dir_find_record() {
//	File* directory_fd = GetDir();
//	Directory dir(directory_fd);
//
//	File* test_file_fd1 = GetFile();
//	File* test_file_fd2 = GetFile();
//	File* test_file_fd3 = GetFile();
//
//
//	dir.AddFile(test_file_fd1, "file1");
//
//	dir.AddFile(test_file_fd2, "file2");
//
//	dir.AddFile(test_file_fd3, "file3");
//
//
//	size_t inode_id = dir.GetINodeByName("file1");
//}
//
//
//void test_dir() {
//	File* test_dir_file = GetFile(2);
//
//	Directory dir(test_dir_file);
//
//	File* test_f1 = GetFile();
//	dir.AddFile(test_f1, "file1");
//	std::string t1_str = "test string to test multiple splitted files.";
//	ds.Write(test_f1, t1_str.c_str(), t1_str.size());
//
//	File* test_f2 = GetFile();
//	std::string t2_str = "Some stuff from file2";
//	ds.Write(test_f2, t2_str.c_str(), t2_str.size());
//	dir.AddFile(test_f2, "file2");
//
//	std::string t1_str2 = " Other stuff from file1";
//	ds.Write(test_f1, t1_str2.c_str(), t1_str2.size());
//
//	char buf[200];
//	size_t bytes_read = ds.Read(test_f1, buf, 200);
//	std::string t1_res_str(buf, bytes_read);
//
//	bytes_read = ds.Read(test_f2, buf, 200);
//	std::string t2_res_str(buf, bytes_read);
//
//	std::cout << dir;
//}
//
//void test_data_storage_smoke() {
//	DataStorage ds("datastorage.txt", 32);
//	File* test_f = new File;
//	auto node_and_id = ilist.GetFreeINode();
//	auto new_node_ptr = node_and_id.first;
//	auto new_node_id = node_and_id.second;
//
//	if (new_node_ptr) {
//		test_f->inode_obj = *new_node_ptr;
//		test_f->inode_id = new_node_id;
//	}
//
//	std::string test_data = "heres some test data to write into data_storage";
//	ds.Write(test_f, test_data.c_str(), test_data.size());
//
//	char buffer[60];
//	auto bytes_read = ds.Read(test_f, buffer, 60);
//
//	std::string res(buffer, bytes_read);
//
//	assert(test_data == res);
//	std::cout << "res: " << res << '\n';
//	std::cout << ilist;
//}
//
//void test_random_access_read() {
//	DataStorage ds("datastorage.txt", 32);
//	File* test_f = new File;
//	auto node_and_id = ilist.GetFreeINode();
//	auto new_node_ptr = node_and_id.first;
//	auto new_node_id = node_and_id.second;
//
//	if (new_node_ptr) {
//		test_f->inode_obj = *new_node_ptr;
//		test_f->inode_id = new_node_id;
//	}
//
//	std::string test_data = "heres some test data to write into data_storage";
//	ds.Write(test_f, test_data.c_str(), test_data.size());
//
//	test_data = test_data.substr(40);
//	char buffer[60];
//
//	ds.Seekg(test_f, 40);
//	auto bytes_read = ds.Read(test_f, buffer, 60);
//
//	std::string res(buffer, bytes_read);
//
//	assert(test_data == res);
//	std::cout << "res: " << res << '\n';
//	std::cout << ilist;
//}
//
//void test_append() {
//	DataStorage ds("datastorage.txt", 32);
//	File* test_f = new File;
//	auto node_and_id = ilist.GetFreeINode();
//	auto new_node_ptr = node_and_id.first;
//	auto new_node_id = node_and_id.second;
//
//	if (new_node_ptr) {
//		test_f->inode_obj = *new_node_ptr;
//		test_f->inode_id = new_node_id;
//	}
//
//	std::string test_data = "heres some test data to write into data_storage";
//	ds.Write(test_f, test_data.c_str(), test_data.size());
//
//	std::string appendix = " ,bitchass nigga";
//	ds.Append(test_f, appendix.c_str(), appendix.size());
//
//	test_data = test_data + appendix;
//	char buffer[100];
//	auto bytes_read = ds.Read(test_f, buffer, 100);
//
//	std::string res(buffer, bytes_read);
//
//	assert(test_data == res);
//	std::cout << "res: " << res << '\n';
//	std::cout << ilist;
//}
//
//void test_ilist() {
//	IList ilist("ilist.txt", 10, true);
//
//
//	std::cout << ilist;
//}
