#include "my_vfs.h"

MYVFS::MYVFS() {
	ilist_ = IList("ilist.txt", 100, true);

	ds_ = DataStorage("datastorage.txt", 256);

	root_fd_ = CreateNewDir(); // initialize root directory
}

MYVFS::MYVFS(const VFSSettings& settings) {
	ilist_ = IList(settings.ilist_filename_, settings.ilist_capacity_,
		settings.reinitialize_);

	ds_ = DataStorage(settings.datastorage_filename_, settings.block_size_);

	root_fd_ = CreateNewDir(); // initialize root directory
}

MYVFS::~MYVFS() { delete root_fd_; }

size_t MYVFS::Write(File* fd, const char* data, size_t data_len) {
	size_t bytes_written = ds_.Write(fd, data, data_len);

	ilist_.WriteINode(fd->inode_id,
		fd->inode_obj); // обновление информации в inode

	return bytes_written;
}

size_t MYVFS::Read(File* fd, char* buf, size_t buf_len) {
	return ds_.Read(fd, buf, buf_len);
}

File* MYVFS::CreateNewFile(Filetype filetype) {
	File* new_file = new File;
	auto inode_id = ilist_.GetFreeINode();

	auto inode = ilist_.ReadINode(inode_id);

	if (!inode)
		return nullptr;

	inode->filetype = filetype;

	new_file->inode_obj = *inode;
	new_file->inode_id = inode_id;

	// new_file->inode_obj.filetype = filetype;

	ilist_.WriteINode(new_file->inode_id, *inode);
	auto inode2 = ilist_.ReadINode(new_file->inode_id);

	return new_file;
}

File* MYVFS::CreateNewDir() {
	auto new_dir_fd = CreateNewFile(Filetype::DIRECTORY);

	return new_dir_fd;
}

std::vector<std::string> MYVFS::SplitPath(const std::string& name) {
	size_t pos = name.find('/');
	std::vector<std::string> dirs;
	while (pos != name.npos) {
		size_t next_pos = name.find('/', pos + 1);
		std::string dir_name = name.substr(pos + 1, next_pos - pos - 1);
		if (dir_name.size() > 0)
			dirs.push_back(dir_name);
		pos = next_pos;
	}
	return dirs;
}

void MYVFS::AddFileToDir(File* file_fd, File* dir_fd, std::string filename) {
	DirRecord new_record;
	new_record.inode_id = file_fd->inode_id;
	std::memcpy(new_record.name, filename.c_str(),
		filename.size() + 1 < 16 ? filename.size() + 1 : 16);

	// Append
	SeekpEnd(dir_fd);
	Write(dir_fd, reinterpret_cast<char*>(&new_record), sizeof(DirRecord));
}

std::optional<DirRecord> MYVFS::GetNextRecord(File* dir_fd) {
	DirRecord dir_record;
	if (ds_.Read(dir_fd, reinterpret_cast<char*>(&dir_record),
		sizeof(DirRecord)) > 0) {
		return dir_record;
	}
	return std::nullopt;
}

std::optional<DirRecord> MYVFS::FindRecordInDir(File* dir_fd,
	const char* name) {
	Seekg(dir_fd, 0);
	while (auto dir_record = GetNextRecord(dir_fd)) {
		if (std::strcmp(dir_record->name, name) == 0) {
			return dir_record;
		}
	}
	return std::nullopt;
}

File* MYVFS::GetFileByNameInDir(File* dir_fd, const char* name) {
	if (std::strcmp(name, "/") == 0)
		return ilist_.GetFile(2); // root handle

	if (auto record = FindRecordInDir(dir_fd, name)) {
		return ilist_.GetFile(record->inode_id);
	}
	
	return nullptr;
}

File* MYVFS::TranslateNameToFd(std::string name) {

	std::vector<std::string> dirs = SplitPath(name);

	File* file_fd = root_fd_;
	for (const auto& name_part : dirs) {
		file_fd = GetFileByNameInDir(file_fd, name_part.c_str());
		if (!file_fd)
			return nullptr;
	}

	return file_fd;
}

File* MYVFS::MkFile(std::string name) {
	auto path = SplitPath(name);
	std::string filename = path.back();
	path.pop_back();

	File* dir_fd = root_fd_;

	// проходим по всем директориям, если они есть идём дальше, если нет,
	// создаём новые
	for (const auto& name_part : path) {
		File* possible_dir = GetFileByNameInDir(dir_fd, name_part.c_str());

		if (possible_dir) { // директория есть
			dir_fd = possible_dir;
		}
		else {
			possible_dir = CreateNewDir(); // создаём новую директорию
			AddFileToDir(possible_dir, dir_fd,
				name_part); // кладём новую директорию в текущую

			dir_fd = possible_dir; // меняем текущую директорию
		}
	}

	
	File* new_file = GetFileByNameInDir(dir_fd, filename.c_str());
	if (new_file) return new_file;

	new_file = CreateNewFile();
	if (!new_file) return nullptr;

	AddFileToDir(new_file, dir_fd, filename.c_str());
	return new_file;
}

void MYVFS::PrintTree(std::ostream& cout) {
	size_t level = 0;
	auto cur_dir = root_fd_;

	std::stack<File*> dir_stack;
	Seekg(cur_dir, 0);
	while (true) {
		auto dir_record = GetNextRecord(cur_dir);

		if (dir_record) {
			cout << std::string(level, '\t') << dir_record->name << '\n';
			auto file_inode = ilist_.ReadINode(dir_record->inode_id);
			if (file_inode && file_inode->filetype == Filetype::DIRECTORY &&
				dir_record->inode_id != cur_dir->inode_id) {
				dir_stack.push(cur_dir);
				cur_dir = ilist_.GetFile(dir_record->inode_id);
				level++;
			}
		}
		else {
			if (not dir_stack.empty()) {
				cur_dir = dir_stack.top();
				dir_stack.pop();
				level = level > 0 ? level - 1 : 0;
			}
			else {
				break;
			}
		}
	}
}