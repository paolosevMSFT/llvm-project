//===-- GDBRemoteWasmWrapper.h --------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef liblldb_GDBRemoteWasmWrapper_h_
#define liblldb_GDBRemoteWasmWrapper_h_

#include <vector>
#include "Plugins/Process/gdb-remote/GDBRemoteCommunicationClient.h"
#include "lldb/Utility/WasmWrapper.h"

namespace lldb_private {

class GDBRemoteWasmWrapper : public WasmWrapper {

public:
  GDBRemoteWasmWrapper(process_gdb_remote::GDBRemoteCommunicationClient *gdb_remote_com_client);
  ~GDBRemoteWasmWrapper();

  // WebAssembly-specific commands
  bool GetWasmGlobal(uint32_t module_id, int index, uint64_t &value) override;
  bool GetWasmLocal(uint32_t module_id, int frame_index, int index,
                    uint64_t &value) override;
  bool GetWasmStackValue(uint32_t module_id, int index, uint64_t &value) override;
  bool WasmReadMemory(uint32_t module_id, lldb::addr_t vm_addr, void *buf,
                      size_t size) override;
  bool GetWasmCallStack(std::vector<lldb::addr_t> &call_stack_pcs) override;

private:
  process_gdb_remote::GDBRemoteCommunicationClient *gdb_comm;
};

} // namespace lldb_private

#endif // liblldb_GDBRemoteWasmWrapper_h_
