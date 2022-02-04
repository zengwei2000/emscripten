// Copyright 2022 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.

// This file defines a Node.js backend. File operations are sent over to Node.js
// to handle, giving access to the real filesystem.

#include "backend.h"
#include "wasmfs.h"

namespace wasmfs {

// This class describes a file that lives in JS Memory
class NodeFile : public DataFile {
  __wasi_errno_t write(const uint8_t* buf, size_t len, off_t offset) override {
    return _wasmfs_write_js_file(index, buf, len, offset);
  }

  __wasi_errno_t read(uint8_t* buf, size_t len, off_t offset) override {
    return _wasmfs_read_js_file(index, buf, len, offset);
  }

  void flush() override {}

  size_t getSize() override { return _wasmfs_get_js_file_size(index); }

public:
  NodeFile(mode_t mode, backend_t backend) : DataFile(mode, backend) {
    // Create a new file in the backing JS array and store its index.
    index = _wasmfs_create_js_file();
  }

  // Remove the typed array file contents in the backing JS array.
  ~NodeFile() { _wasmfs_remove_js_file(index); }
};

class NodeFileBackend : public Backend {

public:
  std::shared_ptr<DataFile> createFile(mode_t mode) override {
    return std::make_shared<NodeFile>(mode, this);
  }
};

// This function is exposed to users to instantiate a new JSBackend.
extern "C" backend_t wasmfs_create_node_backend() {
  return wasmFS.addBackend(std::make_unique<NodeFileBackend>());
}

} // namespace wasmfs
