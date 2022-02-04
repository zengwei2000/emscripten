/*
 * Copyright 2021 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

#pragma once

#include <stdint.h>
#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Backend* backend_t;

// Obtains the backend_t of a specified path.
backend_t wasmfs_get_backend_by_path(char* path);

// Obtains the backend_t of a specified fd.
backend_t wasmfs_get_backend_by_fd(int fd);

// Creates a JSFile Backend.
backend_t wasmfs_create_js_file_backend();

// Creates a Node.js Backend, which will route Node.js file operations to a
// particular path in the outside ("real") filesystem that Node.js accesses.
//
// For example, if you create a Node.js backend with parameter outside_path set
// to  /home/me/some_dir  and inside WasmFS you create a directory using the
// Node.js backend at  /foo  then if you create a file at /foo/bar it will
// become a file in the outside filesystem at   /home/me/some_dir/bar . That is,
// paths relative to /foo maps to /home/me_some_dir in the outside filesystem.
// You can also create a single WasmFS file in this backend (without a parent
// directory using the same backend), in which case it will do the same, that
// is, it will become a file at  /home/me/some_dir/FILENAME
backend_t wasmfs_create_node_backend(char* outside_path);

// Creates a Proxied Backend.
backend_t wasmfs_create_proxied_backend(backend_t backend);

// Creates a new file under a specific backend.
uint32_t wasmfs_create_file(char* pathname, mode_t mode, backend_t backend);

// Creates a new directory under a specific backend.
long wasmfs_create_directory(char* path, long mode, backend_t backend);

#ifdef __cplusplus
}
#endif
