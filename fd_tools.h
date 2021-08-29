#pragma once
#include "common_structures.h"
#include <optional>

void Seekg(File* fd, size_t offset);

void Seekp(File* fd, size_t offset);

void SeekgEnd(File* fd);

void SeekpEnd(File* fd);

void AddFileToDir(File* file_fd, File* dir_fd, std::string filename);

std::optional<DirRecord> FindRecordInDir(File* dir_fd, const char name[16]);

File* GetFileByNameInDir(File* dir_fd, const char name[16]);
