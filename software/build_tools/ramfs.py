#!/usr/bin/env python
"""
ramfs.py: Create ramfs images for 65816 computer.
"""
import argparse
import os.path
import sys
from dataclasses import dataclass
from struct import unpack_from, pack, unpack
from typing import Optional


@dataclass
class Inode:
    SIZE_BYTES = 128
    TYPE_FREE = 0
    TYPE_FILE = 1
    TYPE_DIR = 2
    MAX_CHILDREN = 43

    inode_id: int
    parent_inode_id: int
    type_mode: int
    user_id: int
    group_id: int
    size_bytes: int
    inode_name: bytes
    children: list[int]

    def is_free(self):
        return self._get_type() == Inode.TYPE_FREE

    def is_file(self):
        return self._get_type() == Inode.TYPE_FILE

    def is_directory(self):
        return self._get_type() == Inode.TYPE_DIR

    def _get_type(self):
        return (self.type_mode & 0xF000) >> 12

    def add_child_inode_id(self, new_inode_id: int):
        for i in range(0, len(self.children)):
            if self.children[i] == 0:
                self.children[i] = new_inode_id
                return
        raise Exception("Problems")

    def to_bytes(self) -> bytearray:
        name = self.inode_name.ljust(32, b"\x00")
        data = pack("<HHHHH32s",
                    self.parent_inode_id,
                    self.type_mode,
                    self.user_id,
                    self.group_id,
                    self.size_bytes,
                    name) + \
               pack("<43H", *self.children)
        assert len(data) == Inode.SIZE_BYTES
        return bytearray(data)

    @staticmethod
    def from_bytes(inode_id: int, data: bytearray):
        assert len(data) == Inode.SIZE_BYTES
        parent_inode_id, type_mode, user_id, group_id, size_bytes, inode_name = unpack_from("<HHHHH32s", data)
        children = unpack_from("<43H", data, 42)
        inode_name = inode_name.rstrip(b"\x00")
        return Inode(inode_id, parent_inode_id, type_mode, user_id, group_id, size_bytes, inode_name,
                     list(children))

    @staticmethod
    def for_directory(inode_id: int, parent_inode_id: int, name: bytes, user_id: int = 0, group_id: int = 0,
                      mode=0o0755):
        children = [0] * Inode.MAX_CHILDREN
        inode_name = name
        type_mode = (Inode.TYPE_DIR << 12) + (mode & 0x0FFF)
        return Inode(inode_id=inode_id,
                     parent_inode_id=parent_inode_id,
                     type_mode=type_mode,
                     user_id=user_id,
                     group_id=group_id,
                     size_bytes=0,
                     inode_name=inode_name,
                     children=children)

    @staticmethod
    def for_file(inode_id: int, parent_inode_id: int, name: bytes, size: int=0, sectors: list[int] = [], user_id: int = 0, group_id: int = 0,
                      mode=0o0755):
        children = sectors + [0] * (Inode.MAX_CHILDREN - len(sectors))
        inode_name = name
        type_mode = (Inode.TYPE_DIR << 12) + (mode & 0x0FFF)
        return Inode(inode_id=inode_id,
                     parent_inode_id=parent_inode_id,
                     type_mode=type_mode,
                     user_id=user_id,
                     group_id=group_id,
                     size_bytes=size,
                     inode_name=inode_name,
                     children=children)


class PathWrapper:
    components: list[bytes]

    def __init__(self, path: str):
        path_components = path.encode('ascii').strip(b'/').split(b'/')
        tmp_components = []
        for item in path_components:
            if len(item) == 0:
                continue
            elif len(item) > 32:
                raise Exception(f"Path component length cannot exceed 32: '{item}'")
            elif item == ".":
                continue
            elif item == "..":
                if len(tmp_components) > 0:
                    tmp_components.pop()
                else:
                    raise Exception(f"Path is outside root directory: '{item}'")
            else:
                tmp_components.append(item)
        self.components = tmp_components


class RamFs:
    """
    Wrapper for byte array of FS data
    """
    ROOT_INODE = 0
    SECTOR_SIZE_BYTES = 256
    SECTOR_COUNT = 2048
    BITMAP_SECTOR_ID = 255
    FIRST_CONTENT_SECTOR = 256

    data: bytearray

    def __init__(self, filename=None):
        if filename:
            raise Exception("Not implemented")
        else:
            self.data = bytearray(RamFs.SECTOR_SIZE_BYTES * RamFs.SECTOR_COUNT)
            root_inode_id = self.get_next_free_inode_id()
            assert root_inode_id == 0
            root_dir = Inode.for_directory(inode_id=RamFs.ROOT_INODE, parent_inode_id=RamFs.ROOT_INODE, name=b"")
            self._write_inode(root_dir)

    def get_next_free_inode_id(self) -> int:
        """
        Allocate an inode, returns number of allocated inode
        """
        # Number of inodes which will fit before the bitmap
        fs_inode_count = (RamFs.BITMAP_SECTOR_ID * RamFs.SECTOR_SIZE_BYTES) // Inode.SIZE_BYTES
        for inode_id in range(0, fs_inode_count):
            inode = self._read_inode(inode_id)
            if inode.is_free():
                return inode_id
        return -1

    def _read_inode(self, inode_id: int) -> Inode:
        inode_start_byte = inode_id * Inode.SIZE_BYTES
        inode_bytes = self.data[inode_start_byte:inode_start_byte + Inode.SIZE_BYTES]
        return Inode.from_bytes(inode_id, inode_bytes)

    def _get_child_by_name(self, inode: Inode, name: bytes) -> Optional[Inode]:
        if not inode.is_directory():
            raise Exception(f"Path {name} is not a directory")
        for child_inode_id in inode.children:
            if child_inode_id != 0:
                child_inode = self._read_inode(child_inode_id)
                if child_inode.inode_name == name:
                    return child_inode
        return None

    def _write_inode(self, inode: Inode):
        inode_start_byte = inode.inode_id * Inode.SIZE_BYTES
        self.data[inode_start_byte:inode_start_byte + Inode.SIZE_BYTES] = inode.to_bytes()

    def stat(self, filename) -> Inode:
        path = PathWrapper(filename)
        return self._read_inode_by_name(path)

    def mkdir(self, filename):
        path = PathWrapper(filename)
        parent_inode = self._read_inode_by_name(path, parent=True)
        new_directory = Inode.for_directory(self.get_next_free_inode_id(),
                                            parent_inode_id=parent_inode.inode_id,
                                            name=path.components.pop(0))
        self._write_inode(new_directory)
        parent_inode.add_child_inode_id(new_directory.inode_id)
        self._write_inode(parent_inode)

    def _read_inode_by_name(self, path: PathWrapper, parent=False) -> Inode:
        # Start at the root
        current_inode = self._read_inode(RamFs.ROOT_INODE)
        # Navigate down to the requested inode
        while len(path.components) > 0:
            # Parent directory was requested, return it
            if len(path.components) == 1 and parent:
                return current_inode
            next_path = path.components.pop(0)
            next_inode = self._get_child_by_name(current_inode, next_path)
            if next_inode is None:
                raise Exception(f"Path {next_path} does not exist")
            current_inode = next_inode
        return current_inode

    def ls(self, filename="") -> list[Inode]:
        path = PathWrapper(filename)
        directory = self._read_inode_by_name(path)
        result = []
        for child_inode_id in directory.children:
            if child_inode_id != 0:
                child_inode = self._read_inode(child_inode_id)
                result.append(child_inode)
        return result

    def save_to_disk(self, filename: str):
        """ Write entire FS to disk """
        with open(filename, 'wb') as out:
            out.write(self.data)

    def add_file(self, filename, content: bytes):
        path = PathWrapper(filename)
        parent_inode = self._read_inode_by_name(path, parent=True)
        file_size = len(content)
        sector_ids = []
        for i in range(0, len(content), RamFs.SECTOR_SIZE_BYTES):
            sector_id = self._alloc_sector_id()
            sector_ids.append(sector_id)
            sector_data = bytearray(content[i:i+RamFs.SECTOR_SIZE_BYTES].ljust(RamFs.SECTOR_SIZE_BYTES, b"\x00"))
            self._write_sector(sector_id, sector_data)
        new_file = Inode.for_file(self.get_next_free_inode_id(),
                                  parent_inode_id=parent_inode.inode_id,
                                  name=path.components.pop(0),
                                  size=file_size,
                                  sectors=sector_ids)
        self._write_inode(new_file)
        parent_inode.add_child_inode_id(new_file.inode_id)
        self._write_inode(parent_inode)

        """ Does absolutely nothing """
        pass

    def _alloc_sector_id(self) -> int:
        """
        Allocate a sector, returns sector id
        """
        bitmap_start = RamFs.BITMAP_SECTOR_ID * RamFs.SECTOR_SIZE_BYTES
        bitmap_len = (RamFs.SECTOR_COUNT - RamFs.FIRST_CONTENT_SECTOR) // 16  # Read 16 bits (one word) at a time
        # Start at first content sector - skips 16 16-bit words
        sector_idx = RamFs.FIRST_CONTENT_SECTOR
        for bitmap_idx in range(0, bitmap_len):
            this_entry_offset = bitmap_start + bitmap_idx * 2
            this_entry, = unpack_from("<H", self.data, this_entry_offset)  # Note tuple unpack
            if this_entry == 65535:
                # Short-circuit if all 16 sectors in this entry are allocated
                sector_idx = sector_idx + 1
                continue
            # One of these 16 bits indicates an unallocated sector, find it..
            test_val = 1
            for i in range(0, 16):
                masked_val = this_entry & test_val
                if masked_val == 0:
                    this_entry = this_entry | test_val
                    self.data[this_entry_offset:this_entry_offset + 2] = pack("<H", this_entry)
                    return sector_idx
                test_val = test_val << 1
                sector_idx = sector_idx + 1
        raise Exception("No unallocated sectors remaining")

    def _write_sector(self, sector_id: int, sector_bytes: bytearray):
        assert len(sector_bytes) == RamFs.SECTOR_SIZE_BYTES
        sector_start_byte = sector_id * RamFs.SECTOR_SIZE_BYTES
        self.data[sector_start_byte:sector_start_byte + RamFs.SECTOR_SIZE_BYTES] = sector_bytes

    def _read_sector(self, sector_id: int) -> bytearray:
        assert (sector_id >= RamFs.FIRST_CONTENT_SECTOR) and (sector_id < RamFs.SECTOR_COUNT)
        sector_start_byte = sector_id * RamFs.SECTOR_SIZE_BYTES
        return self.data[sector_start_byte:sector_start_byte + RamFs.SECTOR_SIZE_BYTES]

    def get_file(self, filename) -> bytes:
        """ Retrieve a file from the filesystem"""
        path = PathWrapper(filename)
        inode = self._read_inode_by_name(path)
        result = b""
        remaining_bytes = inode.size_bytes
        for sector_id in inode.children:
            if sector_id == 0:
                continue
            full_sector_content = self._read_sector(sector_id)
            if remaining_bytes < RamFs.SECTOR_SIZE_BYTES:
                result = result + full_sector_content[0:remaining_bytes]
                remaining_bytes = 0
            else:
                result = result + full_sector_content
                remaining_bytes = remaining_bytes - RamFs.SECTOR_SIZE_BYTES
        assert len(result) == inode.size_bytes
        return result


def create(filename: str, files: list[str]):
    """ Create new archive """
    fs = RamFs()
    # Add files something like this
    for file in files:
        _add_recursive(fs, file)
    fs.save_to_disk(filename)
    pass


def _add_recursive(fs: RamFs, file: str):
    if os.path.isdir(file):
        fs.mkdir(file)
        for subfile in os.listdir(file):
            _add_recursive(fs, os.path.join(file, subfile))
    else:
        fs.add_file(file, bytearray(open(file, 'rb').read()))


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Create ramfs images for 65816 computer.')

    action_group = parser.add_mutually_exclusive_group(required=True)
    action_group.add_argument('--create', help='create image', action='store_true')
    action_group.add_argument('--extract', help='extract image', action='store_true')

    parser.add_argument('--file', metavar='output.bin', type=str, required=True, help='image file to work on')
    parser.add_argument('fs_file', metavar='FILE', type=str, nargs='*', help='files to add')
    args = parser.parse_args(sys.argv[1:])

    if args.create:
        create(args.file, args.fs_file)
    if args.extract:
        raise Exception("Not implemented")
