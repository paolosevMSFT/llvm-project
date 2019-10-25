//===-- WasmWrapper.h --------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef liblldb_WasmWrapper_h_
#define liblldb_WasmWrapper_h_

#include <vector>
#include "lldb/lldb-types.h"

namespace lldb_private {

class WasmWrapper {

public:
  // WebAssembly-specific commands
  virtual bool GetWasmGlobal(uint32_t module_id, int index, uint64_t &value) = 0;
  virtual bool GetWasmLocal(uint32_t module_id, int frame_index, int index,
                    uint64_t &value) = 0;
  virtual bool GetWasmStackValue(uint32_t module_id, int index, uint64_t &value) = 0;
  virtual bool WasmReadMemory(uint32_t module_id, lldb::addr_t vm_addr, void *buf,
                      size_t size) = 0;
  virtual bool GetWasmCallStack(std::vector<lldb::addr_t> &call_stack_pcs) = 0;
};

} // end lldb_private

#endif // liblldb_WasmWrapper_h_
