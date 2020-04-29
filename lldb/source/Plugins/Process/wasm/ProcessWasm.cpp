//===-- ProcessWasm.cpp ---------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ProcessWasm.h"
#include "Plugins/Process/gdb-remote/ThreadGDBRemote.h"
#include "lldb/Core/PluginManager.h"

using namespace lldb;
using namespace lldb_private;
using namespace lldb_private::process_gdb_remote;
using namespace lldb_private::wasm;

LLDB_PLUGIN_DEFINE(WasmProcessGDBRemote)

// ProcessGDBRemote constructor
WasmProcessGDBRemote::WasmProcessGDBRemote(lldb::TargetSP target_sp,
                                           ListenerSP listener_sp)
    : ProcessGDBRemote(target_sp, listener_sp) {}

// Destructor
WasmProcessGDBRemote::~WasmProcessGDBRemote() {}

void WasmProcessGDBRemote::Initialize() {
  static llvm::once_flag g_once_flag;

  llvm::call_once(g_once_flag, []() {
    PluginManager::RegisterPlugin(GetPluginNameStatic(),
                                  GetPluginDescriptionStatic(), CreateInstance,
                                  DebuggerInitialize);
  });
}

void WasmProcessGDBRemote::DebuggerInitialize(Debugger &debugger) {
  ProcessGDBRemote::DebuggerInitialize(debugger);
}

// PluginInterface
ConstString WasmProcessGDBRemote::GetPluginName() {
  return GetPluginNameStatic();
}

uint32_t WasmProcessGDBRemote::GetPluginVersion() { return 1; }

ConstString WasmProcessGDBRemote::GetPluginNameStatic() {
  static ConstString g_name("wasm");
  return g_name;
}

const char *WasmProcessGDBRemote::GetPluginDescriptionStatic() {
  return "GDB Remote protocol based WebAssembly debugging plug-in.";
}

void WasmProcessGDBRemote::Terminate() {
  PluginManager::UnregisterPlugin(WasmProcessGDBRemote::CreateInstance);
}

lldb::ProcessSP
WasmProcessGDBRemote::CreateInstance(lldb::TargetSP target_sp,
                                     ListenerSP listener_sp,
                                     const FileSpec *crash_file_path) {
  lldb::ProcessSP process_sp;
  if (crash_file_path == nullptr)
    process_sp = std::make_shared<WasmProcessGDBRemote>(target_sp, listener_sp);
  return process_sp;
}

size_t WasmProcessGDBRemote::ReadMemory(lldb::addr_t vm_addr, void *buf,
                                        size_t size, Status &error) {
  if (vm_addr < 0x100000000) {
    if (GetGDBRemote().WasmReadMemory(0 /*frame_index*/, vm_addr, buf, size)) {
      return size;
    }
    return 0;
  } else
    return ProcessGDBRemote::ReadMemory(vm_addr, buf, size, error);
}

lldb::RegisterContextSP
WasmProcessGDBRemote::CreateRegisterContextForFrame(StackFrame *frame,
                                                    lldb::addr_t pc) {
  ThreadGDBRemote *gdb_thread =
      static_cast<ThreadGDBRemote *>(frame->CalculateThread().get());
  std::shared_ptr<GDBRemoteRegisterContext> reg_ctx_sp =
      std::make_shared<GDBRemoteRegisterContext>(
          *gdb_thread, frame->GetFrameIndex(), m_register_info, false, false);
  reg_ctx_sp->PrivateSetRegisterValue(0, pc);
  return reg_ctx_sp;
}

bool WasmProcessGDBRemote::GetWasmLocal(int frame_index, int index, void *buf,
                                        size_t buffer_size, size_t &size) {
  return GetGDBRemote().GetWasmLocal(frame_index, index, buf, buffer_size,
                                     size);
}

bool WasmProcessGDBRemote::GetWasmGlobal(int frame_index, int index, void *buf,
                                         size_t buffer_size, size_t &size) {
  return GetGDBRemote().GetWasmGlobal(frame_index, index, buf, buffer_size,
                                      size);
}

bool WasmProcessGDBRemote::GetWasmStackValue(int frame_index, int index,
                                             void *buf, size_t buffer_size,
                                             size_t &size) {
  return GetGDBRemote().GetWasmStackValue(frame_index, index, buf, buffer_size,
                                          size);
}

bool WasmProcessGDBRemote::WasmReadMemory(int frame_index, lldb::addr_t addr,
                                          void *buf, size_t buffer_size) {
  return GetGDBRemote().WasmReadMemory(frame_index, addr, buf, buffer_size);
}

bool WasmProcessGDBRemote::GetWasmCallStack(
    std::vector<lldb::addr_t> &call_stack_pcs) {
  return GetGDBRemote().GetWasmCallStack(call_stack_pcs);
}
