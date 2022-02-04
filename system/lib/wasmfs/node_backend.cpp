// Copyright 2022 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.

// This file defines a Node.js backend. File operations are sent over to Node.js
// to handle, giving access to the real filesystem.

#include "backend.h"
//#include "emscripten.h"
#include "wasmfs.h"

extern "C" {
void _wasmfs_init_node_backend_js();
}

namespace wasmfs {

// This class describes a file that lives in JS Memory
class NodeFile : public DataFile {
  __wasi_errno_t write(const uint8_t* buf, size_t len, off_t offset) override {
abort();
  }

  __wasi_errno_t read(uint8_t* buf, size_t len, off_t offset) override {
abort();
  }

  void flush() override {}

  size_t getSize() override {
abort();
  }

public:
  NodeFile(mode_t mode, backend_t backend) : DataFile(mode, backend) {
    _wasmfs_node_backend_
abort();
  }

  !NodeFile() {
    _wasmfs_node
abort();
  }
};

class NodeBackend : public Backend {
  // The path in the outside ("real") filesystem to which the files operations
  // will be routed.
  std::string outsidePath;

public:
  NodeBackend(std::string outsidePath) : outsidePath(outsidePath) {}

  std::shared_ptr<DataFile> createFile(mode_t mode) override {
    return std::make_shared<NodeFile>(mode, this);
  }
};

// This function is exposed to users to instantiate a new JSBackend.
extern "C" backend_t wasmfs_create_node_backend(char* outside_path) {
  _wasmfs_node_backend_init_js();
  return wasmFS.addBackend(std::make_unique<NodeBackend>(outside_path));
}

} // namespace wasmfs
