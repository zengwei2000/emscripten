// Copyright 2021 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.
// This file defines the global state of the new file system.
// Current Status: Work in Progress.
// See https://github.com/emscripten-core/emscripten/issues/15041.

#pragma once

#include "file_table.h"
#include <assert.h>
#include <emscripten/html5.h>
#include <map>
#include <mutex>
#include <sys/stat.h>
#include <vector>
#include <wasi/api.h>

// TODO: Locking for global filesystem state that includes the current directory
// static std::shared_ptr<File> cwd = getRootDirectory();

namespace wasmfs {
class GlobalFileState {
  static std::shared_ptr<File> cwd;
  std::mutex mutex;

public:
  class Handle {
    GlobalFileState& globalFileState;
    std::unique_lock<std::mutex> lock;

  public:
    Handle(GlobalFileState& globalFileState)
      : globalFileState(globalFileState), lock(globalFileState.mutex) {}

    std::shared_ptr<File> getCWD() {
      if (cwd) {
        return cwd;
      } else {
        globalFileState.cwd = getRootDirectory();
        return globalFileState.cwd;
      }
    };

    void setCWD(std::shared_ptr<File> directory) {
      globalFileState.cwd = directory;
    };
  };

  static Handle get() {
    static GlobalFileState globalFileState;
    return Handle(globalFileState);
  };
};
} // namespace wasmfs