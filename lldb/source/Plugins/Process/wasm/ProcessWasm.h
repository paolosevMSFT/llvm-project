//===-- ProcessWasm.h -------------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLDB_SOURCE_PLUGINS_PROCESS_WASM_PROCESSWASM_H
#define LLDB_SOURCE_PLUGINS_PROCESS_WASM_PROCESSWASM_H

#include "Plugins/Process/gdb-remote/ProcessGDBRemote.h"
#include "lldb/Target/RegisterContext.h"

namespace lldb_private {
namespace wasm {

// Interface IWasmProcess provides the access to the Wasm program state
//  retrieved from the Wasm engine.
struct IWasmProcess {
  ~IWasmProcess() = default;

  virtual lldb::RegisterContextSP
  CreateRegisterContextForFrame(StackFrame *frame, lldb::addr_t pc) = 0;

  virtual bool GetWasmLocal(int frame_index, int index, void *buf,
                            size_t buffer_size, size_t &size) = 0;

  virtual bool GetWasmGlobal(int frame_index, int index, void *buf,
                             size_t buffer_size, size_t &size) = 0;

  virtual bool GetWasmStackValue(int frame_index, int index, void *buf,
                                 size_t buffer_size, size_t &size) = 0;

  virtual bool WasmReadMemory(int frame_index, lldb::addr_t addr, void *buf,
                              size_t buffer_size) = 0;

  virtual bool GetWasmCallStack(std::vector<lldb::addr_t> &call_stack_pcs) = 0;
};

class WasmProcessGDBRemote : public process_gdb_remote::ProcessGDBRemote,
                             public IWasmProcess {
public:
  WasmProcessGDBRemote(lldb::TargetSP target_sp, lldb::ListenerSP listener_sp);

  ~WasmProcessGDBRemote() override;

  static lldb::ProcessSP CreateInstance(lldb::TargetSP target_sp,
                                        lldb::ListenerSP listener_sp,
                                        const FileSpec *crash_file_path);

  static void Initialize();

  static void DebuggerInitialize(Debugger &debugger);

  static void Terminate();

  static ConstString GetPluginNameStatic();

  static const char *GetPluginDescriptionStatic();

  // PluginInterface protocol
  ConstString GetPluginName() override;

  uint32_t GetPluginVersion() override;

  // Process
  size_t ReadMemory(lldb::addr_t vm_addr, void *buf, size_t size,
                    Status &error) override;

  // IWasmProcess
  lldb::RegisterContextSP
  CreateRegisterContextForFrame(StackFrame *frame,
                                lldb::addr_t address) override;

  bool GetWasmLocal(int frame_index, int index, void *buf, size_t buffer_size,
                    size_t &size) override;

  bool GetWasmGlobal(int frame_index, int index, void *buf, size_t buffer_size,
                     size_t &size) override;

  bool GetWasmStackValue(int frame_index, int index, void *buf,
                         size_t buffer_size, size_t &size) override;

  bool WasmReadMemory(int frame_index, lldb::addr_t addr, void *buf,
                      size_t buffer_size) override;

  bool GetWasmCallStack(std::vector<lldb::addr_t> &call_stack_pcs) override;

private:
  DISALLOW_COPY_AND_ASSIGN(WasmProcessGDBRemote);
};

} // namespace wasm
} // namespace lldb_private

#endif // LLDB_SOURCE_PLUGINS_PROCESS_WASM_PROCESSWASM_H
