#include "fd_tools.h"


void Seekg(File* fd, size_t offset) {
	fd->gpos = offset <= fd->inode_obj.byte_size ? offset : fd->inode_obj.byte_size;
}

void Seekp(File* fd, size_t offset) {
	
	fd->ppos = offset <= fd->inode_obj.byte_size ? offset : fd->inode_obj.byte_size;

}

void SeekgEnd(File* fd) {
	fd->gpos = fd->inode_obj.byte_size;
}

void SeekpEnd(File* fd) {
	fd->ppos = fd->inode_obj.byte_size;
}
