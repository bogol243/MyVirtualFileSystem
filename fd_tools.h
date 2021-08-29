#pragma once
#include "common_structures.h"
#include <optional>

void Seekg(File* fd, size_t offset);

void Seekp(File* fd, size_t offset);

void SeekgEnd(File* fd);

void SeekpEnd(File* fd);
