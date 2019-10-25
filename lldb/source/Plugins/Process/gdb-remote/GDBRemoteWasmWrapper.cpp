//===-- GDBRemoteWasmWrapper.cpp -------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "GDBRemoteWasmWrapper.h"

#include "Plugins/Process/gdb-remote/ProcessGDBRemote.h"

using namespace lldb;
using namespace lldb_private;

GDBRemoteWasmWrapper::GDBRemoteWasmWrapper(process_gdb_remote::GDBRemoteCommunicationClient *gdb_remote_com_client) {
  gdb_comm = gdb_remote_com_client;
}

GDBRemoteWasmWrapper::~GDBRemoteWasmWrapper(){}

// WebAssembly-specific commands
bool GDBRemoteWasmWrapper::GetWasmGlobal(uint32_t module_id, int index, uint64_t &value) {
  return gdb_comm->GetWasmGlobal(module_id, index, value);
}

bool GDBRemoteWasmWrapper::GetWasmLocal(uint32_t module_id, int frame_index, int index,
                    uint64_t &value) {
  return gdb_comm->GetWasmLocal(module_id, frame_index, index, value);
}

bool GDBRemoteWasmWrapper::GetWasmStackValue(uint32_t module_id, int index, uint64_t &value) {
  return gdb_comm->GetWasmStackValue(module_id, index, value);
}
bool GDBRemoteWasmWrapper::WasmReadMemory(uint32_t module_id, lldb::addr_t vm_addr, void *buf,
                    size_t size) {
  return gdb_comm->WasmReadMemory(module_id, vm_addr, buf, size);
}

bool GDBRemoteWasmWrapper::GetWasmCallStack(std::vector<lldb::addr_t> &call_stack_pcs) {
  return gdb_comm->GetWasmCallStack(call_stack_pcs);
}
