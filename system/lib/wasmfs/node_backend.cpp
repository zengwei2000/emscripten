// Copyright 2022 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.

// This file defines a Node.js backend. File operations are sent over to Node.js
// to handle, giving access to the real filesystem.

#include "backend.h"
#include "emscripten.h"
#include "wasmfs.h"

EM_JS(

namespace wasmfs {

// This class describes a file that lives in JS Memory
class NodeFile : public DataFile {
  __wasi_errno_t write(const uint8_t* buf, size_t len, off_t offset) override {
..
  }

  __wasi_errno_t read(uint8_t* buf, size_t len, off_t offset) override {
..
  }

  void flush() override {}

  size_t getSize() override { .. }

public:
  NodeFile(mode_t mode, backend_t backend) : DataFile(mode, backend) {
..
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
  return wasmFS.addBackend(std::make_unique<NodeBackend>(outside_path));
}

} // namespace wasmfs
