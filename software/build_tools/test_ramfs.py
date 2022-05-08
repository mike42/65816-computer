from build_tools.ramfs import RamFs


def test_empty_filesystem():
    filesystem = RamFs()
    files = filesystem.ls()
    assert len(files) == 0


def test_mkdir():
    filesystem = RamFs()
    filesystem.mkdir("foo")
    filesystem.mkdir("bar")
    filesystem.mkdir("baz")
    filesystem.mkdir("baz/quux")
    files = filesystem.ls()
    assert len(files) == 3
    files = filesystem.ls("baz/")
    assert len(files) == 1


def test_add_file():
    filesystem = RamFs()
    filesystem.mkdir("etc")
    content = b"Hello world " * 45
    filesystem.add_file("etc/foobar", content)
    inode = filesystem.stat("etc/foobar")
    assert inode.size_bytes == 540
    retrieved_content = filesystem.get_file("etc/foobar")
    assert retrieved_content == content
